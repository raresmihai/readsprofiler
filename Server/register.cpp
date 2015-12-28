#include "register.h"

void inregistrare(int client_descriptor, QSqlDatabase db)
{
    utilizator client;
    if(read(client_descriptor, &client,sizeof(utilizator)) <= 0)
    {
        perror ("Eroare la read() de la client pt user.\n");
    }
    printf("Am citit utilizatorul %s\n",client.username);
}
