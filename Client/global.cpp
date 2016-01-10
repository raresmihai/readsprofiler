#include "global.h"

bool contine_caractere_invalide(char text[32],int optiune){
    switch(optiune){
    case 1: //username
        for(int i=0;i<strlen(text);++i){
            if(!isalpha(text[i])&&!isdigit(text[i])){
                return 1;
            }
        }
        break;
    case 2: //nume,prenume
        for(int i=0;i<strlen(text);++i){
            if(!isalpha(text[i])){
                return 1;
            }
        }
        break;
    }
    return 0;
}

const char *strlwr(const char *str)
{
    unsigned char *p = (unsigned char *)str;
    while (*p) {
        *p = tolower(*p);
        p++;
    }
    return str;
}

Comunicare::Comunicare(){
    port = 3000;
}

bool Comunicare::conectat = 0;
int Comunicare::sd = 16;

void Comunicare::conectare_la_server(){
    sd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons (port);
    char * eroare;
    if(connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr))<0){
        eroare = strerror(errno);
    }
    conectat = 1;
}

void Comunicare::inchide_conexiunea(){
    caz = 0;
    if(conectat == 0)
    {
        conectare_la_server();
    }
    write(sd,&caz,sizeof(int));
}
