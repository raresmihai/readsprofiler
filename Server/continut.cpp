#include "continut.h"

void trimite_continut_la_client(int client_descriptor)
{
    char isbn[20];
    if(read(client_descriptor,&isbn,20)<0){
        perror("Eroare la citirea isbn-ului.\n");
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
}
