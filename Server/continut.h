#ifndef CONTINUT_H
#define CONTINUT_H
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


void trimite_continut_la_client(int client_descriptor);

#endif // CONTINUT_H
