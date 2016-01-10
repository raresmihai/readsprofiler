#ifndef UPLOAD_H
#define UPLOAD_H

#include <QCoreApplication>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<wait.h>
#include<arpa/inet.h>
#include<string>
#include <signal.h>
#include <pthread.h>

typedef struct detaliiCarte{
    char isbn[20];
    char titlu[50];
    char nume_autor[30];
    char prenume_autor[30];
    char descriere[200];
    int an_aparitie;
    int nr_genuri;
    int nr_subgenuri;
    char genuri[4][50];
    char subgenuri[4][50];
}detaliiCarte;

void upload_carte(int client_descriptor);


#endif // UPLOAD_H
