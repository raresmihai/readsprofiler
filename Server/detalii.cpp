#include "detalii.h"

void trimite_detalii_la_client(int client_descriptor)
{
    char isbn[20],username[50];
    if(read(client_descriptor,&isbn,20)<0){
        perror("Eroare la citirea isbn-ului.\n");
    }
    if(read(client_descriptor,&username,50)<0){
        perror("Eroare la citirea username-ului.\n");
    }
    char interogare[1000];
    sprintf(interogare,"SELECT coperta FROM carti WHERE isbn = '%s'",isbn);
    QSqlQuery query;
    if( !query.exec(interogare) )
        qDebug() << "Eroare la obtinerea copertii din BD.\n" << query.lastError();
    query.first();
    QByteArray outByteArray = query.value(0).toByteArray();
    int dimens = (int)outByteArray.size();
    if(write(client_descriptor,&dimens,4)<=0){
        perror("Eroare la scrierea dimensiunii copertii.\n");
    }
    char img_data[dimens];
    for(int i=0;i<dimens;i++)
    {
        img_data[i]=outByteArray[i];
    }
    if(write(client_descriptor,&img_data,dimens)!=dimens){
        perror("Eroare la scrierea copertii.\n");
    }

    sprintf(interogare,"SELECT rating_acordat FROM rating_utilizatori WHERE isbn = '%s' AND username = '%s'",isbn,username);
    if( !query.exec(interogare) )
        qDebug() << "Eroare la obtinerea ratingului din BD.\n" << query.lastError();
    bool a_mai_votat = false;
    int rating_acordat = 0;
    if(query.size()>0)
    {
        query.first();
        a_mai_votat = true;
        rating_acordat = query.value(0).toInt();
    }
    if(write(client_descriptor,&a_mai_votat,1)<0){
        perror("Eroare la scrierea flagului de votare.\n");
    }
    if(write(client_descriptor,&rating_acordat,4)<0){
        perror("Eroare la scrierea ratingului acordat cartii.\n");
    }

    //inserare in tabela accesari_utilizatori
    QSqlQuery query_inserareCarte;
    sprintf(interogare,"SELECT id FROM accesari_utilizatori WHERE username = '%s' AND isbn = '%s'",username,isbn);
    if(!query_inserareCarte.exec(interogare))
    {
        qDebug() << "Eroare la verificarea existentei cartii in tabela accesari_utilizatori:\n" << query.lastError();
    }
    if(query_inserareCarte.size()==0)
    {
        sprintf(interogare,"INSERT INTO accesari_utilizatori(username,isbn) VALUES('%s','%s')",username,isbn);
        if(!query_inserareCarte.exec(interogare))
        {
            qDebug() << "Eroare la inserare cartii in tabela accesari_utilizatori:\n" << query.lastError();
        }
    }

}

