#ifndef REGISTER_H
#define REGISTER_H

#include "global.h"

typedef struct utilizator{
    char username[32];
    char parola[32];
    char nume[32];
    char prenume[32];
    char email[32];
}utilizator;

class Register:public Comunicare
{
private:
    utilizator client;
    char confParola[32];
    bool raspuns;
public:
    Register();
    void setare_campuri(QString un,QString pw,QString cp,QString nm,QString pren,QString em);
    int datele_sunt_valide();
    bool trimite_datele_la_server();
    bool primeste_raspuns_de_la_server();
    bool este_inregistrat();
    QString obtine_mesaj_invalidare1(int caz);
    QString obtine_mesaj_invalidare2(int caz);
};



#endif // REGISTER_H
