#ifndef REGISTER_H
#define REGISTER_H

#include "global.h"



class Register:public Comunicare
{
private:
    char *username, *parola, *confParola;
    char *nume, *prenume, *email;
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
