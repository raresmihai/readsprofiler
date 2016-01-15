#include "recomandare.h"

Recomandare::Recomandare()
{
    caz = 4;
}

int Recomandare::cere_recomandari(int caz_recomandare,char * username,char *isbn)
{
    if(conectat==0){
        conectare_la_server();
    }
    if(write(sd,&caz,4)<0){
        conectat = 0;
        return 0;
    }
    if(write(sd,&caz_recomandare,4)<0){
        conectat = 0;
        return 0;
    }
    trimite_username_la_server(username);

    if(caz_recomandare == 2)
    {
        return recomandari_similare(isbn);
    }
    if(caz_recomandare == 3)
    {
        return recomandari_bazate_pe_istoric();
    }
    return 1;
}

int Recomandare::recomandari_bazate_pe_istoric()
{
    int caz_recomandare_primit;
    if(read(sd,&caz_recomandare_primit,4)<0)
    {
        conectat = 0;
        return 0;
    }
    return caz_recomandare_primit;
}

int Recomandare::recomandari_similare(char *isbn)
{
    char copieISBN[20];
    strcpy(copieISBN,isbn);
    if(write(sd,&copieISBN,20)<0){
        conectat = 0;
        return 0;
    }
    int caz_recomandare_primit;
    if(read(sd,&caz_recomandare_primit,4)<0)
    {
        conectat = 0;
        return 0;
    }
    return caz_recomandare_primit;
}

bool Recomandare::trimite_username_la_server(char *username)
{

    char copieUsername[50];
    strcpy(copieUsername,username);
    if(write(sd,&copieUsername,50)<0){
        conectat = 0;
        return 0;
    }
    return 1;
}

QByteArray Recomandare::primeste_recomandare(int index)
{
    if(read(sd,&isbn_recomandari[index],20)<=0){
        conectat = 0;
        perror("Eroare la citirea isbunului recomandarii.\n");
    }

    int dimensiune_continut;
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
