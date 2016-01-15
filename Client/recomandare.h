#ifndef RECOMANDARE_H
#define RECOMANDARE_H
#include "global.h"

class Recomandare:public Comunicare
{
public:
    char isbn_recomandari[5][20];
    Recomandare();
    int cere_recomandari(int caz_recomandare,char *username,char *isbn);
    bool trimite_username_la_server(char * username);
    int recomandari_similare(char *isbn);
    int recomandari_bazate_pe_istoric();
    QByteArray primeste_recomandare(int index);
};

#endif // RECOMANDARE_H
