#include "coperta.h"

void trimite_coperta_la_client(int client_descriptor)
{
    char isbn[20];
    if(read(client_descriptor,&isbn,20)<0){
        perror("Eroare la citirea isbn-ului.\n");
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
}

