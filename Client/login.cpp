#include "login.h"

Login::Login(char* nume, char* pw)
{
    username = nume;
    parola = pw;
}

bool Login::datele_sunt_valide(){
    return false;
}

bool Login::trimite_datele_la_server(){
    return true;
}

bool Login::primeste_raspuns_de_la_server(){
    return true;
}

bool Login::este_autentificat(){
    return true;
}
