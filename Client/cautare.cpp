#include "cautare.h"

Cautare::Cautare()
{
    caz = 3;
    bzero(&date,sizeof(detaliiCautare));
}

void Cautare::setare_campuri(detaliiCautare copieDate)
{
    date = copieDate;
}

bool Cautare::trimite_datele_la_server(char *nume)
{
    if(conectat == 0){
        conectare_la_server();
    }
    if(write(sd,&caz,sizeof(int))<0){
        conectat = 0;
        return 0;
    }
    if(write(sd,&date,sizeof(detaliiCautare))<0){
        conectat = 0;
        return 0;
    }
    char username[50];
    strcpy(username,nume);
    if(write(sd,&username,50)<0){
        conectat = 0;
        return 0;
    }
    return 1;
}

bool Cautare::primeste_date_de_la_server()
{
    if(conectat == 0){
        conectare_la_server();
    }
    if(read(sd,&nr_rezultate,4)<0){
        conectat = 0;
        return 0;
    }
    rezultateCarte rezultat;
    for(int i=0;i<nr_rezultate;i++){
        bzero(&rezultat,sizeof(rezultateCarte));
        if(read(sd,&rezultat,sizeof(rezultateCarte))<0){
            conectat = 0;
            return 0;
        }
        rezultate[i] = rezultat;
    }
}
