#include "continut.h"

void trimite_continut_la_client(int client_descriptor)
{
    char isbn[20];
    if(read(client_descriptor,&isbn,20)<0){
        perror("Eroare la citirea isbn-ului.\n");
    }
    char username[50];
    if(read(client_descriptor,&username,50)<0){
        perror("Eroare la citirea username-ului.\n");
    }
    char interogare[1000];
    sprintf(interogare,"SELECT text FROM carti WHERE isbn = '%s'",isbn);
    QSqlQuery query;
    if( !query.exec(interogare) )
        qDebug() << "Eroare la obtinerea continutului din BD.\n" << query.lastError();
    query.first();
    QByteArray outByteArray = query.value(0).toByteArray();
    int dimens = (int)outByteArray.size();
    if(write(client_descriptor,&dimens,4)<=0){
        perror("Eroare la scrierea dimensiunii continutului.\n");
    }
    char file_data[dimens];
    for(int i=0;i<dimens;i++)
    {
        file_data[i]=outByteArray[i];
    }
    if(write(client_descriptor,&file_data,dimens)!=dimens){
        perror("Eroare la scrierea continutului.\n");
    }

    //inserare in tabela descarcari_utilizatori
    QSqlQuery query_inserareCarte;
    sprintf(interogare,"SELECT id FROM descarcari_utilizatori WHERE username = '%s' AND isbn = '%s'",username,isbn);
    if(!query_inserareCarte.exec(interogare))
    {
        qDebug() << "Eroare la verificarea existentei cartii in tabela descarcari_utilizatori:\n" << query.lastError();
    }
    if(query_inserareCarte.size()==0)
    {
        sprintf(interogare,"INSERT INTO descarcari_utilizatori(username,isbn) VALUES('%s','%s')",username,isbn);
        if(!query_inserareCarte.exec(interogare))
        {
            qDebug() << "Eroare la inserare cartii in tabela descarcari_utilizatori:\n" << query.lastError();
        }
    }
}
