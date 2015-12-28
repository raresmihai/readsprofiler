#ifndef GLOBAL
#define GLOBAL
#include <QString>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include<arpa/inet.h>

class Comunicare{
protected:
    bool conectat = 0;
    int sd;
    struct sockaddr_in server;
    int port = 3000;
    void conectare_la_server();
};



bool contine_caractere_invalide(char text[32],int optiune);



#endif // GLOBAL

