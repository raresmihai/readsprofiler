#include "cautare.h"

void cautare(int client_descriptor)
{
    detaliiCautare cautare;
    bzero(&cautare,sizeof(detaliiCautare));
    if(read(client_descriptor,&cautare,sizeof(detaliiCautare))<0)
    {
        perror("Eroare la citirea datelor pentru cautare.\n");
    }

    char interogare[1000];
    QSqlQuery query;
    sprintf(interogare,"SELECT DISTINCT carti.isbn,titlu,nume,prenume,an_aparitie,IFNULL(floor(valoare/nr_voturi),0) FROM carti JOIN autori ON carti.id_autor=autori.id_autor JOIN genuri_carte ON carti.isbn=genuri_carte.isbn JOIN genuri ON genuri_carte.id_gen=genuri.id_gen JOIN rating ON carti.isbn=rating.isbn JOIN subgenuri_carte ON subgenuri_carte.isbn=carti.isbn JOIN subgenuri ON subgenuri.id_subgen=subgenuri_carte.id_subgen WHERE nume LIKE '%%%s%%' AND prenume LIKE '%%%s%%' AND titlu LIKE '%%%s%%' AND carti.isbn LIKE '%%%s%%' AND nume_gen LIKE '%%%s%%' AND nume_subgen LIKE '%%%s%%' AND an_aparitie LIKE '%%%s%%' AND IFNULL(floor(valoare/nr_voturi),0) LIKE '%%%s%%'",cautare.nume_autor,cautare.prenume_autor,cautare.titlu,cautare.isbn,cautare.gen,cautare.subgen,cautare.an_aparitie,cautare.rating);
    if(!query.exec(interogare))
    {
        qDebug() << "Eroare la cautarea cartii in tabele:\n" << query.lastError();
    }
    int nr_linii = query.size();
    if(write(client_descriptor,&nr_linii,4)<0)
    {
        perror("Eroare la scrierea numarului de linii pentru cautare.\n");
    }
    while(query.next())
    {
        rezultateCarte rezultate;
        bzero(&rezultate,sizeof(rezultateCarte));
        strcpy(rezultate.titlu,query.value(1).toString().toStdString().c_str());
        strcpy(rezultate.nume_autor,query.value(2).toString().toStdString().c_str());
        strcpy(rezultate.prenume_autor,query.value(3).toString().toStdString().c_str());
        rezultate.an_aparitie = query.value(4).toInt();
        rezultate.rating = query.value(5).toInt();
        char isbn[20];
        strcpy(isbn,query.value(0).toString().toStdString().c_str());
        strcpy(rezultate.isbn,isbn);

        QSqlQuery query_genuri;
        sprintf(interogare,"SELECT nume_gen FROM genuri JOIN genuri_carte ON genuri.id_gen=genuri_carte.id_gen WHERE isbn='%s'",isbn);
        if(!query_genuri.exec(interogare))
        {
            qDebug() << "Eroare la aflarea genurilor:\n" << query.lastError();
        }
        char genuri_aflate[200];
        strcpy(genuri_aflate,"\0");
        while(query_genuri.next())
        {
            strcat(genuri_aflate,query_genuri.value(0).toString().toStdString().c_str());
            strcat(genuri_aflate," \0");
        }
        strcpy(rezultate.genuri,genuri_aflate);

        QSqlQuery query_subgenuri;
        sprintf(interogare,"SELECT nume_subgen FROM subgenuri JOIN subgenuri_carte ON subgenuri.id_subgen=subgenuri_carte.id_subgen WHERE isbn='%s'",isbn);
        if(!query_subgenuri.exec(interogare))
        {
            qDebug() << "Eroare la aflarea subgenurilor:\n" << query.lastError();
        }
        char subgenuri_aflate[200];
        strcpy(subgenuri_aflate,"\0");
        while(query_subgenuri.next())
        {
            strcat(subgenuri_aflate,query_subgenuri.value(0).toString().toStdString().c_str());
            strcat(subgenuri_aflate," \0");
        }
        strcpy(rezultate.subgenuri,subgenuri_aflate);

        if(write(client_descriptor,&rezultate,sizeof(rezultateCarte))<0)
        {
            perror("Eroare la scrierea rezultatelor pentru cautare.\n");
        }
    }
}
