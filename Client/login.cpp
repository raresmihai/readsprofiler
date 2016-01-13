#include "login.h"

Login::Login()
{
    autentificare = 0;
    caz = 1;
}

char * Login::get_username()
{
    return client.username;
}

void Login::setare_campuri(QString un, QString pw)
{
    strcpy(client.username,strlwr(un.toStdString().c_str()));
    strcpy(client.parola,pw.toStdString().c_str());
}

int Login::datele_sunt_valide()
{
    if(strlen(client.username)==0||strlen(client.parola)==0){
        return 0;
    }
    if(contine_caractere_invalide(client.username,1)){
        return -1;
    }
    if(strlen(client.parola)<6){
        return -2;
    }
    return 1;
}

QString Login::mesaj_date_invalide(int caz)
{
    switch(caz){
    case 0:
        return "Va rugam completati campurile marcate cu *.";
    case -1:
        return "Numele de utilizator contine caractere invalide.";
    case -2:
        return "Parola introdusa este prea scurta.";
    }
}

bool Login::trimite_datele_la_server(){
    if(conectat == 0){
        conectare_la_server();
    }
    if(write(sd,&caz,sizeof(int))<=0){
        conectat = 0;
        return 0;
    }
    if(write(sd,&client,sizeof(user))<=0){
        conectat = 0;
        return 0;
    }
    return 1;
}

bool Login::primeste_raspuns_de_la_server(){
    if(read(sd,&autentificare,1)<=0){
        conectat = 0;
        return 0;
    }
    return 1;
}

bool Login::este_autentificat(){
    return autentificare;
}

