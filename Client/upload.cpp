#include "upload.h"

Upload::Upload()
{
    caz = 5;
}

void Upload::setare_campuri(detaliiCarte copieCarte)
{
    carte = copieCarte;
}

void Upload::setare_coperta(QByteArray imgData)
{
    dimensiune_coperta = imgData.size();
    coperta = (char *)malloc(dimensiune_coperta);
    for(int i = 0;i<dimensiune_coperta;i++)
    {
        coperta[i] = imgData[i];
    }
}

void Upload::setare_continut(QByteArray fileData)
{
    dimensiune_continut = fileData.size();
    continut = (char*)malloc(dimensiune_continut);
    for(int i = 0;i<dimensiune_continut;i++)
    {
        continut[i] = fileData[i];
    }
}

bool Upload::trimite_datele_la_server()
{
    if(conectat == 0){
        conectare_la_server();
    }
    if(write(sd,&caz,sizeof(int))<=0){
        conectat = 0;
        return 0;
    }
    if(write(sd,&carte,sizeof(detaliiCarte))<=0){
        conectat = 0;
        return 0;
    }
    if(write(sd,&dimensiune_coperta,4)<=0){
        conectat = 0;
        return 0;
    }
    if(write(sd,coperta,dimensiune_coperta)<=0){
        conectat = 0;
        return 0;
    }
    if(write(sd,&dimensiune_continut,4)<=0){
        conectat = 0;
        return 0;
    }
    if(write(sd,continut,dimensiune_continut)!=dimensiune_continut){
        conectat = 0;
        return 0;
    }
    return 1;
}

