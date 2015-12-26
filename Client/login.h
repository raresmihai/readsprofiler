#ifndef LOGIN_H
#define LOGIN_H
#include<QString>

class Login{
private:
    char *username, *parola;
    bool autentificare = 0;
public:
    Login(char* nume, char* pw);
    bool datele_sunt_valide();
    bool trimite_datele_la_server();
    bool primeste_raspuns_de_la_server();
    bool este_autentificat();
};

#endif // LOGIN_H
