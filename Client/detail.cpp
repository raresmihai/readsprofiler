#include "detail.h"

Detail::Detail()
{

}

QByteArray Detail::primeste_detalii(char isbn[20],char *username)
{
    if(conectat==0){
        conectare_la_server();
    }
    caz = 6;
    if(write(sd,&caz,4)<=0){
        conectat = 0;
        perror("Eroare la scrierea cazului.\n");
    }
    char copieISBN[20];
    strcpy(copieISBN,isbn);
    if(write(sd,&copieISBN,sizeof(copieISBN))<=0){
        conectat = 0;
        perror("Eroare la scrierea isbn-ului.\n");
    }
    char nume[50];
    strcpy(nume,username);
    if(write(sd,&nume,sizeof(nume))<=0){
        conectat = 0;
        perror("Eroare la scrierea username-ului.\n");
    }
    usleep(1);
    if(read(sd,&dimensiune_coperta,4)<=0){
        conectat = 0;
        perror("Eroare la citirea dimensiunii copertii.\n");
    }
    char img_data[dimensiune_coperta];
    QByteArray img;
    if(read(sd,&img_data,dimensiune_coperta)!=dimensiune_coperta)
    {
        conectat = 0;
        perror("Eroare la citirea copertii.\n");
    }
    for(int i=0;i<dimensiune_coperta;i++)
    {
        img[i]=img_data[i];
    }

    if(read(sd,&a_mai_votat,1)<0){
        conectat = 0;
        perror("Eroare la citirea flagului de rating.\n");
    }
    if(read(sd,&rating_acordat,4)<0){
        conectat = 0;
        perror("Eroare la citirea valorii rating.\n");
    }
    return img;
}

QByteArray Detail::primeste_continut(char isbn[],char *username)
{
    if(conectat==0){
        conectare_la_server();
    }
    caz = 7;
    if(write(sd,&caz,4)<=0){
        conectat = 0;
        perror("Eroare la scrierea cazului.\n");
    }
    char copieISBN[20];
    strcpy(copieISBN,isbn);
    if(write(sd,&copieISBN,sizeof(copieISBN))<=0){
        conectat = 0;
        perror("Eroare la scrierea isbn-ului.");
    }
    char copieUsername[50];
    strcpy(copieUsername,username);
    if(write(sd,&copieUsername,sizeof(copieUsername))<=0){
        conectat = 0;
        perror("Eroare la scrierea username-ului.");
    }
    usleep(1);
    if(read(sd,&dimensiune_continut,4)<=0){
        conectat = 0;
        perror("Eroare la citirea dimensiunii continutului.\n");
    }
    char continut[dimensiune_continut];

    int bytesread = 0;
    while(bytesread<dimensiune_continut)
    {
        char aux[512];
        int de_citit=512;
        if(dimensiune_continut - bytesread < 512)
        {
            de_citit = dimensiune_continut - bytesread;
        }
        int nr = read(sd,&aux,de_citit);
        if(nr<0)
        {
            perror("Eroare la citirea continutului.\n");
        }
        for(int i=0;i<nr;i++)
        {
            continut[i+bytesread]=aux[i];
        }
        bytesread+=nr;
    }
    QByteArray file;
    for(int i=0;i<dimensiune_continut;i++)
    {
        file[i]=continut[i];
    }
    return file;
}

bool Detail::voteaza(char isbn[],char *username,int valoare)
{
    caz = 8;
    if(write(sd,&caz,4)<0){
        conectat=0;
        perror("Eroare la scrierea cazului.\n");
        return 0;
    }
    char copieISBN[20];
    strcpy(copieISBN,isbn);
    if(write(sd,&copieISBN,20)<0){
        conectat=0;
        perror("Eroare la scrierea valorii isbnului.\n");
        return 0;
    }
    char nume[50];
    strcpy(nume,username);
    if(write(sd,&nume,50)<0){
        conectat=0;
        perror("Eroare la scrierea usernameului.\n");
        return 0;
    }
    if(write(sd,&valoare,4)<0){
        conectat=0;
        perror("Eroare la scrierea valorii ratingului.\n");
        return 0;
    }
    bool ok;
    if(read(sd,&ok,1)<0){
        conectat = 0;
        perror("Eroare la citirea raspunsului votarii.\n");
        return 0;
    }
    return ok;
}
