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
    char username[30];
    char parola[30];
    char nume[30];
    char prenume[30];
    char email[30];
}utilizator;

void inregistrare(int client_descriptor, QSqlDatabase db);

#endif // REGISTER

