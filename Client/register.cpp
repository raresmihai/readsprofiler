#include "register.h"

Register::Register(){
    raspuns = 0;
}

void Register::setare_campuri(QString un,QString pw,QString cp,QString nm,QString pren,QString em){
    username = (char*)malloc(sizeof(un)); strcpy(username,un.toStdString().c_str());
    parola = (char*)malloc(sizeof(pw)); strcpy(parola,pw.toStdString().c_str());
    confParola = (char*)malloc(sizeof(cp)); strcpy(confParola,cp.toStdString().c_str());
    nume = (char*)malloc(sizeof(nm)); strcpy(nume,nm.toStdString().c_str());
    prenume = (char*)malloc(sizeof(pren)); strcpy(prenume,pren.toStdString().c_str());
    email = (char*)malloc(sizeof(em)); strcpy(email,em.toStdString().c_str());
}

int Register::datele_sunt_valide(){
    if(strlen(username)==0||strlen(parola)==0||strlen(confParola)==0){
        return 0;
    }
    if(contine_caractere_invalide(username,1)){
        return -1;
    }
    if(contine_caractere_invalide(nume,2)){
        return -2;
    }
    if(contine_caractere_invalide(prenume,2)){
        return -3;
    }
    if(strlen(parola)<6){
        return -4;
    }
    if(strcmp(parola,confParola)!=0){
        return -5;
    }
    return 1;
}

QString Register::obtine_mesaj_invalidare1(int caz){
    switch(caz){
    case 0:
        return "Va rugam completati campurile marcate cu *.";
    case -1:
        return "Numele de utilizator contine caractere invalide.";
    case -2:
        return "Numele introdus contine caractere invalide.";
    case -3:
        return "Prenumele introdus contine caractere invalide.";
    case -4:
        return "Parola introdusa este prea scurta.";
    case -5:
        return "Parolele nu corespund.";
    }
}

QString Register::obtine_mesaj_invalidare2(int caz){
    switch(caz){
    case -1:
        return "Caracterele valide sunt: litere,cifre.";
    case -2:
        return "Caracterele valide sunt literele (a-zA-Z).";
    case -3:
        return "Caracterele valide sunt literele (a-zA-Z).";
    case -4:
        return "Lungimea minima a parolei este de 6 caractere.";
    default:
        return "Incercati din nou.";
    }
}



bool Register::trimite_datele_la_server(){
    if(conectat == 0){
        conectare_la_server();
    }
    if(write(sd,username,sizeof(username))<=0){
        return 0;
    }
    if(write(sd,parola,sizeof(parola))<=0){
        return 0;
    }
    if(write(sd,nume,sizeof(nume))<0){
        return 0;
    }
    if(write(sd,prenume,sizeof(prenume))<0){
        return 0;
    }
    if(write(sd,email,sizeof(email))<0){
        return 0;
    }
    return 1;
}

bool Register::primeste_raspuns_de_la_server(){
    return 1;
}

bool Register::este_inregistrat(){
    return 1;
}
