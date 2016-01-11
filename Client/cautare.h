#ifndef CAUTARE_H
#define CAUTARE_H
#include "global.h"

typedef struct detaliiCautare{
    char nume_autor[30];
    char prenume_autor[30];
    char titlu[50];
    char gen[50];
    char subgen[50];
    char isbn[20];
    char an_aparitie[5];
    char rating[2];
}detaliiCautare;

typedef struct rezultateCarte{
    char titlu[50];
    char nume_autor[50];
    char prenume_autor[50];
    char genuri[200];
    char subgenuri[200];
    char isbn[20];
    int an_aparitie;
    int rating;
}rezultateCarte;

class Cautare:public Comunicare
{
private:
    detaliiCautare date;
public:
    int nr_rezultate;
    rezultateCarte rezultate[1000];
    Cautare();
    void setare_campuri(detaliiCautare copieDate);
    bool trimite_datele_la_server();
    bool primeste_date_de_la_server();
};



#endif // CAUTARE_H
