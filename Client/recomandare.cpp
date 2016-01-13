#include "recomandare.h"

Recomandare::Recomandare()
{
    caz = 4;
}

bool Recomandare::trimite_username_la_server(char *username)
{
    if(conectat==0){
        conectare_la_server();
    }
    if(write(sd,&caz,4)<0){
        conectat = 0;
        return 0;
    }
    char copieUsername[50];
    strcpy(copieUsername,username);
    if(write(sd,&copieUsername,50)<0){
        conectat = 0;
        return 0;
    }
    return 1;
}
