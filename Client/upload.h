#ifndef UPLOAD_H
#define UPLOAD_H
#include "global.h"

typedef struct detaliiCarte{
    char isbn[20];
    char titlu[50];
    char nume_autor[30];
    char prenume_autor[30];
    char descriere[200];
    int an_aparitie;
    int nr_genuri;
    int nr_subgenuri;
    char genuri[4][50];
    char subgenuri[4][50];
}detaliiCarte;

class Upload:public Comunicare
{
private:
    detaliiCarte carte;
    char *continut,*coperta;
    int dimensiune_coperta,dimensiune_continut;
public:
    Upload();
    void setare_campuri(detaliiCarte copieCarte);
    void setare_coperta(QByteArray imgData);
    void setare_continut(QByteArray fileData);
    bool trimite_datele_la_server();
};

#endif // UPLOAD_H
