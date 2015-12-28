#ifndef REGISTER
#define REGISTER
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

typedef struct utilizator{
    char username[32];
    char parola[32];
    char nume[32];
    char prenume[32];
    char email[32];
}utilizator;

void inregistrare(int client_descriptor);

void inregistrare_in_bd(utilizator client);

#endif // REGISTER

