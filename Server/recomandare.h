#ifndef RECOMANDARE_H
#define RECOMANDARE_H

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
#include <algorithm>
#include<string>
#include <signal.h>
#include <pthread.h>

typedef struct SugestieCarte{
    char isbn[20];
    double grad_recomandare;
}SugestieCarte;

typedef struct Tabela{
    char denumire_tabela[30];
    int importanta_tabela;
}Tabela;

typedef struct InterogareDinamica
{
    char nume_tabela[20];
    char nume_camp[20];
    int valoare_camp;
    int importanta_camp;
}InterogareDinamica;

void recomandare_carti(int client_descriptor);

void adauga_recomandari(SugestieCarte *recomandariCarti,int importanta_tabela,char *isbn,char *username);

bool user_nu_a_mai_accesat_cartea(char *username,char *isbn);

double rating_carte(char *isbn);

double calculare_grad_recomandare(int importanta_tabela,int importanta_camp,int rating_carte_de_baza,double rating_carte_sugestie);

void adauga_sugestie_noua(SugestieCarte *recomandariCarti,char *isbn_sugestie,double grad_recomandare);

void sortare_dupa_grad(SugestieCarte *recomandariCarti);

#endif // RECOMANDARE_H
