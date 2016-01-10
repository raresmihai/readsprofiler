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
public:
    static bool conectat;
    static int sd;
    struct sockaddr_in server;
    int port;
    int caz;
    Comunicare();
    void conectare_la_server();
    void inchide_conexiunea();
};



bool contine_caractere_invalide(char text[32],int optiune);

const char *strlwr(const char *str);



#endif // GLOBAL

