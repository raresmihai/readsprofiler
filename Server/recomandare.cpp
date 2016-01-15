#include "recomandare.h"

void recomandare_carti(int client_descriptor)
{
    int caz_recomandare;
    if(read(client_descriptor,&caz_recomandare,4)<0)
    {
        perror("Eroare la citirea cazului de recomandare.\n");
    }
    char username[50];
    if(read(client_descriptor,&username,50)<0)
    {
        perror("Eroare la citirea usernameului pentru recomandare.\n");
    }

    char isbn[20];
    if(caz_recomandare == 2)
    {
        if(read(client_descriptor,&isbn,20)<0)
        {
            perror("Eroare la citirea isbn-ului pentru recomandare.\n");
        }
    }


    switch(caz_recomandare)
    {
    case 1:
        recomanda_top_5_carti(client_descriptor);
        break;
    case 2:
        recomandari_similare(client_descriptor,isbn);
        break;
    case 3:
        recomandari_dupa_istoric(client_descriptor,username);
        break;
    }
}

void recomandari_dupa_istoric(int client_descriptor, char *username)
{
    SugestieCarte recomandariCarti[1000];
    bzero(&recomandariCarti,sizeof(recomandariCarti));
    Tabela tabele[3];
    strcpy(tabele[0].denumire_tabela,"cautari_utilizatori");
    tabele[0].importanta_tabela = 1;
    strcpy(tabele[1].denumire_tabela,"accesari_utilizatori");
    tabele[1].importanta_tabela = 2;
    strcpy(tabele[2].denumire_tabela,"descarcari_utilizatori");
    tabele[2].importanta_tabela = 3;

    char interogare[1000];
    QSqlQuery query_tabelaPrincipala;
    char isbn[20];
    for(int i=0;i<3;i++)
    {
        sprintf(interogare,"SELECT isbn FROM %s WHERE username = '%s'",tabele[i].denumire_tabela,username);
        if(!query_tabelaPrincipala.exec(interogare)){
            qDebug() << "Eroare la selectarea isbnurilor de baza in recomandare:\n" << query_tabelaPrincipala.lastError();
        }
        while(query_tabelaPrincipala.next())
        {
            strcpy(isbn,query_tabelaPrincipala.value(0).toString().toStdString().c_str());
            adauga_recomandari(recomandariCarti,tabele[i].importanta_tabela,isbn,username);
        }
    }
    int nr_recomandari = sortare_dupa_grad(recomandariCarti);
    int caz_recomandare_client = 3;
    if(nr_recomandari < 5)
    {
        if(nr_recomandari == 0)
        {
            caz_recomandare_client = 1;
            write(client_descriptor,&caz_recomandare_client,4);
            recomanda_top_5_carti(client_descriptor);
        }
        else
        {
            write(client_descriptor,&caz_recomandare_client,4);
            SugestieCarte topCarti[1000];
            bzero(&topCarti,sizeof(topCarti));
            int nr_top_recomandari = top_carti_dupa_rating(topCarti);
            int i=0;
            while(nr_recomandari<5 && i<nr_top_recomandari)
            {
                if(cartea_nu_a_fost_deja_adaugata(recomandariCarti,topCarti[i].isbn))
                {
                    recomandariCarti[nr_recomandari] = topCarti[i];
                    nr_recomandari++;
                }
                i++;
            }
            trimite_recomandarile_clientului(client_descriptor,recomandariCarti);
        }
    }
    else
    {
        write(client_descriptor,&caz_recomandare_client,4);
        trimite_recomandarile_clientului(client_descriptor,recomandariCarti);
    }
}

void adauga_recomandari(SugestieCarte *recomandariCarti, int importanta_tabela, char *isbn, char *username)
{
    char interogare[1000];
    QSqlQuery query;

    //selectare rating
    sprintf(interogare,"SELECT rating_acordat FROM rating_utilizatori WHERE isbn = '%s' AND username = '%s'",isbn,username);
    if(!query.exec(interogare)){
        qDebug() << "Eroare la selectarea ratingului de baza in recomandare:\n" << query.lastError();
    }
    int rating_carte_de_baza = 0;
    if(query.size()>0){
        query.first();
        rating_carte_de_baza = query.value(0).toInt();
    }


    //selectare id_autor
    sprintf(interogare,"SELECT id_autor FROM carti WHERE isbn = '%s'",isbn);
    if(!query.exec(interogare)){
        qDebug() << "Eroare la selectarea autorului in recomandare:\n" << query.lastError();
    }
    query.first();
    int id_autor = query.value(0).toInt();

    //selectare an_aparitie
    sprintf(interogare,"SELECT an_aparitie FROM carti WHERE isbn = '%s'",isbn);
    if(!query.exec(interogare)){
        qDebug() << "Eroare la selectarea anului in recomandare:\n" << query.lastError();
    }
    query.first();
    int an_aparitie = query.value(0).toInt();

    //selectare genuri
    sprintf(interogare,"SELECT id_gen FROM genuri_carte WHERE isbn = '%s'",isbn);
    if(!query.exec(interogare)){
        qDebug() << "Eroare la selectarea genurilor in recomandare:\n" << query.lastError();
    }
    int nr_genuri = query.size();
    int genuri[nr_genuri];
    int k=0;
    while(query.next())
    {
        genuri[k]=query.value(0).toInt();
        k++;
    }

    //selectare subgenuri
    sprintf(interogare,"SELECT id_subgen FROM subgenuri_carte WHERE isbn = '%s'",isbn);
    if(!query.exec(interogare)){
        qDebug() << "Eroare la selectarea subgenurilor in recomandare:\n" << query.lastError();
    }
    int nr_subgenuri = query.size();
    int subgenuri[nr_subgenuri];
    k=0;
    while(query.next())
    {
        subgenuri[k]=query.value(0).toInt();
        k++;
    }

    //setare campuri pentru interogari
    int nr_interogari = 2 + nr_genuri + nr_subgenuri;
    InterogareDinamica interogari[nr_interogari];

    strcpy(interogari[0].nume_tabela,"carti");
    strcpy(interogari[0].nume_camp,"id_autor");
    interogari[0].valoare_camp = id_autor;
    interogari[0].importanta_camp = 3;

    strcpy(interogari[1].nume_tabela,"carti");
    strcpy(interogari[1].nume_camp,"an_aparitie");
    interogari[1].valoare_camp = an_aparitie;
    interogari[1].importanta_camp = 2;

    for(int i=2;i<2+nr_genuri;i++)
    {
        strcpy(interogari[i].nume_tabela,"genuri_carte");
        strcpy(interogari[i].nume_camp,"id_gen");
        interogari[i].valoare_camp = genuri[i-2];
        interogari[i].importanta_camp = 5;
    }

    for(int i=2+nr_genuri;i<nr_interogari;i++)
    {
        strcpy(interogari[i].nume_tabela,"subgenuri_carte");
        strcpy(interogari[i].nume_camp,"id_subgen");
        interogari[i].valoare_camp = subgenuri[i-2-nr_genuri];
        interogari[i].importanta_camp = 4;
    }

    //INTEROGARI
    char isbn_sugestie[20];
    for(int i=0;i<nr_interogari;i++)
    {
        sprintf(interogare,"SELECT isbn FROM %s WHERE isbn != '%s' AND %s = %d",interogari[i].nume_tabela,isbn,interogari[i].nume_camp,interogari[i].valoare_camp);
        if(!query.exec(interogare)){
            qDebug() << "Eroare la selectarea isbnului recomandarii:\n" << query.lastError();
        }
        while(query.next())
        {
            strcpy(isbn_sugestie,query.value(0).toString().toStdString().c_str());
            if(user_nu_a_mai_accesat_cartea(username,isbn_sugestie))
            {
                double rating_carte_sugestie = rating_carte(isbn_sugestie);
                double grad_recomandare = calculare_grad_recomandare(importanta_tabela,interogari[i].importanta_camp,rating_carte_de_baza,rating_carte_sugestie);
                adauga_sugestie_noua(recomandariCarti,isbn_sugestie,grad_recomandare);
            }
        }
    }
}

void adauga_sugestie_noua(SugestieCarte *recomandariCarti, char *isbn_sugestie, double grad_recomandare)
{
    bool sugestieGasita = false;
    int lungime = 0;
    while(recomandariCarti[lungime].grad_recomandare>0)
    {
        if(strcmp(recomandariCarti[lungime].isbn,isbn_sugestie)==0)
        {
            recomandariCarti[lungime].grad_recomandare+=grad_recomandare;
            sugestieGasita = true;
            break;
        }
        lungime++;
    }
    if(sugestieGasita==false)
    {
        recomandariCarti[lungime].grad_recomandare=grad_recomandare;
        strcpy(recomandariCarti[lungime].isbn,isbn_sugestie);
    }
}

double calculare_grad_recomandare(int importanta_tabela, int importanta_camp, int rating_carte_de_baza, double rating_carte_sugestie)
{
    return (double)(importanta_tabela * importanta_tabela * (3*rating_carte_sugestie + 2*importanta_camp + rating_carte_de_baza));
}

int sortare_dupa_grad(SugestieCarte *recomandariCarti)
{
    int lungime = 0;
    while(recomandariCarti[lungime].grad_recomandare>0) lungime++;
    SugestieCarte aux;
    for(int i = 0;i<lungime-1;i++)
    {
        for(int j = i+1; j<lungime;j++)
        {
            if(recomandariCarti[i].grad_recomandare<recomandariCarti[j].grad_recomandare)
            {
                aux = recomandariCarti[i];
                recomandariCarti[i]=recomandariCarti[j];
                recomandariCarti[j]=aux;
            }
        }
    }
    return lungime;
}

int top_carti_dupa_rating(SugestieCarte *topCarti)
{
    QSqlQuery query;
    if(!query.exec("SELECT isbn,valoare/nr_voturi FROM rating WHERE nr_voturi > 3 ORDER BY valoare/nr_voturi DESC")){
        qDebug() << "Eroare la selectarea TOP recomandari:\n" << query.lastError();
    }
    int nr_top_carti = 0;
    while(query.next()&&nr_top_carti<30)
    {
        strcpy(topCarti[nr_top_carti].isbn,query.value(0).toString().toStdString().c_str());
        topCarti[nr_top_carti].grad_recomandare = query.value(1).toDouble();
        nr_top_carti++;
    }
    return nr_top_carti;
}

double rating_carte(char *isbn)
{
    char interogare[1000];
    QSqlQuery query;
    sprintf(interogare,"SELECT IFNULL(valoare/nr_voturi,2.5) FROM rating WHERE isbn = '%s'",isbn);
    if(!query.exec(interogare)){
        qDebug() << "Eroare la selectarea ratingului cartii sugestie in recomandare:\n" << query.lastError();
    }
    query.first();
    double rating = query.value(0).toDouble();
    return rating;
}

bool user_nu_a_mai_accesat_cartea(char *username, char *isbn)
{
    char interogare[1000];
    QSqlQuery query;
    sprintf(interogare,"SELECT id FROM accesari_utilizatori WHERE username = '%s' AND isbn = '%s'",username,isbn);
    if(!query.exec(interogare)){
        qDebug() << "Eroare la verificare accesare in recomandare:\n" << query.lastError();
    }
    if(query.size()==0)
    {
        return true;
    }
    return false;
}

void recomanda_top_5_carti(int client_descriptor)
{
    QSqlQuery query;
    if(!query.exec("SELECT isbn FROM rating WHERE nr_voturi > 3 ORDER BY valoare/nr_voturi DESC LIMIT 5")){
        qDebug() << "Eroare la top 5 recomandari:\n" << query.lastError();
    }
    SugestieCarte top5[5];
    int index = 0;
    while(query.next())
    {
        strcpy(top5[index].isbn,query.value(0).toString().toStdString().c_str());
        index++;
    }
    trimite_recomandarile_clientului(client_descriptor,top5);
}

bool cartea_nu_a_fost_deja_adaugata(SugestieCarte *recomandari, char *isbn)
{
    int i = 0;
    while(recomandari[i].grad_recomandare>0)
    {
        if(strcmp(recomandari[i].isbn,isbn)==0)
        {
            return false;
        }
        i++;
    }
    return true;
}

void recomandari_similare(int client_descriptor,char *isbn)
{
    SugestieCarte recomandariSimilare[1000];
    bzero(&recomandariSimilare,sizeof(recomandariSimilare));
    char interogare[1000];
    char username_similar[50];
    char isbn_sugestie[20];
    int nr_voturi;
    QSqlQuery query;
    QSqlQuery query_nrVoturi;
    QSqlQuery query_isbn;
    sprintf(interogare,"SELECT username FROM rating_utilizatori WHERE isbn = '%s'",isbn);
    if(!query.exec(interogare)){
        qDebug() << "Eroare la select usernames in recomandare:\n" << query.lastError();
    }
    while(query.next())
    {
        strcpy(username_similar,query.value(0).toString().toStdString().c_str());
        sprintf(interogare,"SELECT isbn FROM rating_utilizatori WHERE username = '%s' AND isbn != '%s'",username_similar,isbn);
        if(!query_isbn.exec(interogare)){
            qDebug() << "Eroare la selectarea isbnurilor recomandarilor similare:\n" << query.lastError();
        }
        while(query_isbn.next())
        {
            strcpy(isbn_sugestie,query_isbn.value(0).toString().toStdString().c_str());
            sprintf(interogare,"SELECT nr_voturi FROM rating WHERE isbn = '%s'",isbn_sugestie);
            if(!query_nrVoturi.exec(interogare)){
                qDebug() << "Eroare la selectarea isbnurilor recomandarilor similare:\n" << query.lastError();
            }
            query_nrVoturi.first();
            nr_voturi = query_nrVoturi.value(0).toInt();
            adauga_sugestie_noua(recomandariSimilare,isbn_sugestie,(double)nr_voturi);
        }
    }

    int nr_recomandari = sortare_dupa_grad(recomandariSimilare);

    int caz_recomandare_client = 2;
    if(nr_recomandari < 5)
    {
        if(nr_recomandari == 0)
        {
            caz_recomandare_client = 1;
            write(client_descriptor,&caz_recomandare_client,4);
            recomanda_top_5_carti(client_descriptor);
        }
        else
        {
            write(client_descriptor,&caz_recomandare_client,4);
            SugestieCarte topCarti[1000];
            bzero(&topCarti,sizeof(topCarti));
            int nr_top_recomandari = top_carti_dupa_rating(topCarti);
            int i=0;
            while(nr_recomandari<5 && i<nr_top_recomandari)
            {
                if(cartea_nu_a_fost_deja_adaugata(recomandariSimilare,topCarti[i].isbn))
                {
                    recomandariSimilare[nr_recomandari] = topCarti[i];
                    nr_recomandari++;
                }
                i++;
            }
            trimite_recomandarile_clientului(client_descriptor,recomandariSimilare);
        }
    }
    else
    {
        write(client_descriptor,&caz_recomandare_client,4);
        trimite_recomandarile_clientului(client_descriptor,recomandariSimilare);
    }
}

void trimite_recomandarile_clientului(int client_descriptor, SugestieCarte *recomandari)
{
    QSqlQuery query;
    char interogare[1000];
    for(int i=0;i<5;i++)
    {
        if(write(client_descriptor,&recomandari[i].isbn,20)<=0){
            perror("Eroare la scrierea isbnului in recomandare.\n");
        }
        sprintf(interogare,"SELECT coperta FROM carti WHERE isbn = '%s'",recomandari[i].isbn);
        if(!query.exec(interogare)){
            qDebug() << "Eroare la selectarea copertii cartii in recomandare:\n" << query.lastError();
        }
        query.first();
        QByteArray outByteArray = query.value(0).toByteArray();
        int dimens = (int)outByteArray.size();
        if(write(client_descriptor,&dimens,4)<=0){
            perror("Eroare la scrierea dimensiunii copertii in recomandare.\n");
        }
        char img_data[dimens];
        for(int i=0;i<dimens;i++)
        {
            img_data[i]=outByteArray[i];
        }
        if(write(client_descriptor,&img_data,dimens)!=dimens){
            perror("Eroare la scrierea copertii in recomandare.\n");
        }
    }
}


