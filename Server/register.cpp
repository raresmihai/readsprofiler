#include "register.h"

void inregistrare(int client_descriptor, QSqlDatabase db)
{
    utilizator client;
    if(read(client_descriptor, &client,sizeof(utilizator)) <= 0)
    {
        perror ("Eroare la read() de la client pt user.\n");
    }
    char interogare[1000];
    sprintf(interogare,"SELECT * FROM utilizatori WHERE username = '%s'",client.username);
    QSqlQuery query;
    query.exec(interogare);
    while(query.next()){
    qDebug()<<query.value(0).toString()<<query.value(1).toString();
          }
    if(query.size()==0)
    {
        qDebug("merge");
    }
    int i;
}
