#ifndef DETAIL_H
#define DETAIL_H
#include "global.h"

class Detail:public Comunicare
{
private:
    int dimensiune_coperta;
    int dimensiune_continut;

public:
    int rating_acordat;
    bool a_mai_votat;
    Detail();
    QByteArray primeste_detalii(char isbn[20],char *username);
    QByteArray primeste_continut(char isbn[20],char *username);
    bool voteaza(char isbn[20],char *username,int valoare);
};

#endif // DETAIL_H
