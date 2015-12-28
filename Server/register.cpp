#include "register.h"

void inregistrare(int client_descriptor)
{
    utilizator client;
    bzero(&client,sizeof(client));
    if(read(client_descriptor, &client,sizeof(utilizator)) <= 0)
    {
        perror ("Eroare la read() de la client pt user.\n");
    }
    char interogare[1000];
    sprintf(interogare,"SELECT * FROM utilizatori WHERE username = '%s'",client.username);
    QSqlQuery query;
    query.exec(interogare);
    bool raspuns = 0;
    if(query.size()==0)//nume de utilizator valid
    {
        inregistrare_in_bd(client);
        raspuns = 1;
    }
    if(write(client_descriptor, &raspuns,1) <= 0)
    {
        perror ("Eroare la write pentru raspunsul de inregistrare.\n");
    }
}

void inregistrare_in_bd(utilizator client)
{
    char interogare[1000];
    sprintf(interogare,"INSERT INTO utilizatori VALUES('%s','%s','%s','%s','%s')",client.username,client.parola,client.nume,client.prenume,client.email);
    QSqlQuery query;
    query.exec(interogare);
}


