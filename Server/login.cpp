#include "login.h"

void autentificare(int client_descriptor)
{
    user client;
    bzero(&client,sizeof(client));
    if(read(client_descriptor, &client,sizeof(user)) <= 0)
    {
        perror ("Eroare la read() de la client pt user.\n");
    }
    char interogare[1000];
    sprintf(interogare,"SELECT * FROM utilizatori WHERE username = '%s' AND parola = '%s'",client.username,client.parola);
    QSqlQuery query;
    query.exec(interogare);
    bool raspuns = 1;
    if(query.size()==0)//utilizator invalid
    {
        raspuns = 0;
    }
    if(write(client_descriptor, &raspuns,1) <= 0)
    {
        perror ("Eroare la write pentru raspunsul de inregistrare.\n");
    }
}
