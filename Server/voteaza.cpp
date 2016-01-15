#include "voteaza.h"

void voteaza_carte(int client_descriptor)
{
    char isbn[20],username[50];
    int rating;
    bool ok = true;
    if(read(client_descriptor,&isbn,20)<0){
        perror("Eroare la citirea isbnului pentru votarea cartii.\n");
        ok = false;
    }
    if(read(client_descriptor,&username,50)<0){
        perror("Eroare la citirea usernameului pentru votarea cartii.\n");
        ok = false;
    }
    if(read(client_descriptor,&rating,4)<0){
        perror("Eroare la citirea ratingului acordat pentru votarea cartii.\n");
        ok = false;
    }
    char interogare[1000];
    QSqlQuery query;

    sprintf(interogare,"SELECT * FROM rating_utilizatori where username = '%s'AND isbn = '%s'",username,isbn);
    query.exec(interogare);
    if(query.size()>0)
    {
        ok = false;
    }
    else
    {
        sprintf(interogare,"SELECT * from rating WHERE isbn = '%s'",isbn);
        if(!query.exec(interogare)){
            qDebug() << "Eroare la verificarea ratingului in tabele:\n" << query.lastError();
            ok = false;
        }
        if(query.size()==0){
            sprintf(interogare,"INSERT INTO rating(isbn) VALUES('%s')",isbn);
            if(!query.exec(interogare)){
                qDebug() << "Eroare la inserarea unui isbn in tabela rating:\n" << query.lastError();
                ok = false;
            }
        }
        sprintf(interogare,"UPDATE rating SET nr_voturi = nr_voturi + 1,valoare = valoare + %d WHERE isbn = '%s'",rating,isbn);
        if(!query.exec(interogare)){
            qDebug() << "Eroare la actualizarea ratingului:\n" << query.lastError();
            ok = false;
        }
        sprintf(interogare,"INSERT INTO rating_utilizatori(isbn,username,rating_acordat) VALUES('%s','%s',%d)",isbn,username,rating);
        if(!query.exec(interogare)){
            qDebug() << "Eroare la inserarea perechii rating(isbn,username):\n" << query.lastError();
            ok = false;
        }
    }

    if(write(client_descriptor,&ok,1)<0){
        perror("Eroare la scrierea raspunsului in urma votarii:\n");
    }
}

