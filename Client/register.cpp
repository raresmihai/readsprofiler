#include "register.h"

Register::Register(){
    raspuns = 0;
    caz = 2;
}

void Register::setare_campuri(QString un,QString pw,QString cp,QString nm,QString pren,QString em){
    strcpy(client.username,strlwr(un.toStdString().c_str()));
    strcpy(client.parola,pw.toStdString().c_str());
    strcpy(client.nume,nm.toStdString().c_str());
    strcpy(client.prenume,pren.toStdString().c_str());
    strcpy(client.email,em.toStdString().c_str());
    strcpy(confParola,cp.toStdString().c_str());

}

int Register::datele_sunt_valide(){
    if(strlen(client.username)==0||strlen(client.parola)==0||strlen(confParola)==0){
        return 0;
    }
    if(contine_caractere_invalide(client.username,1)){
        return -1;
    }
    if(contine_caractere_invalide(client.nume,2)){
        return -2;
    }
    if(contine_caractere_invalide(client.prenume,2)){
        return -3;
    }
    if(strlen(client.parola)<6){
        return -4;
    }
    if(strcmp(client.parola,confParola)!=0){
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
    if(write(sd,&caz,sizeof(int))<=0){
        conectat = 0;
        return 0;
    }
    if(write(sd,&client,sizeof(utilizator))<=0){
        conectat = 0;
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
