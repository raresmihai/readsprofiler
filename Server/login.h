#ifndef LOGIN
#define LOGIN
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

typedef struct user{
    char username[32];
    char parola[32];
}user;

void autentificare(int client_descriptor);

#endif // LOGIN

