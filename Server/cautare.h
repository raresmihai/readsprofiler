#ifndef CAUTARE_H
#define CAUTARE_H
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

typedef struct detaliiCautare{
    char nume_autor[30];
    char prenume_autor[30];
    char titlu[50];
    char gen[50];
    char subgen[50];
    char isbn[20];
    char an_aparitie[5];
    char rating[2];
}detaliiCautare;

typedef struct rezultateCarte{
    char titlu[50];
    char nume_autor[50];
    char prenume_autor[50];
    char genuri[200];
    char subgenuri[200];
    char isbn[20];
    int an_aparitie;
    int rating;
}rezultateCarte;

void cautare(int client_descriptor);

#endif // CAUTARE_H
