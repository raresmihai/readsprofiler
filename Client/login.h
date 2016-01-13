#ifndef LOGIN_H
#define LOGIN_H
#include "global.h"

typedef struct user{
    char username[32];
    char parola[32];
}user;

class Login:public Comunicare
{
private:
    user client;
    bool autentificare;
public:
    Login();
    void setare_campuri(QString un,QString pw);
    int datele_sunt_valide();
    bool trimite_datele_la_server();
    bool primeste_raspuns_de_la_server();
    bool este_autentificat();
    char * get_username();
    QString mesaj_utilizator_existent();
    QString mesaj_date_invalide(int caz);
};

#endif // LOGIN_H
