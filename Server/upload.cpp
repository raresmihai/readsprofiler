#include "upload.h"

void upload_carte(int client_descriptor)
{
    detaliiCarte carte;
    bzero(&carte,sizeof(detaliiCarte));
    if(read(client_descriptor,&carte,sizeof(carte))<0)
    {
        perror("Eroare la citirea detaliilor cartii.\n");
    }

    int dimensiune_coperta;
    if(read(client_descriptor,&dimensiune_coperta,4)<0)
    {
        perror("Eroare la citirea dimensiunii copertei.\n");
    }
    int bytesread = 0;
    char *coperta = (char*)malloc(dimensiune_coperta);
    while(bytesread<dimensiune_coperta)
    {
        char *aux=(char*)malloc(512);
        int de_citit=512;
        if(dimensiune_coperta - bytesread < 512)
        {
            de_citit = dimensiune_coperta - bytesread;
        }
        int nr = read(client_descriptor,aux,de_citit);
        if(nr<0)
        {
            perror("Eroare la citirea copertii.\n");
        }
        for(int i=0;i<nr;i++)
        {
            coperta[i+bytesread]=aux[i];
        }
        bytesread+=nr;
    }


    int dimensiune_continut;
    if(read(client_descriptor,&dimensiune_continut,4)<=0)
    {
        perror("Eroare la citirea dimensiunii continutului.\n");
    }
    bytesread = 0;
    char *continut = (char*)malloc(dimensiune_continut);
    while(bytesread<dimensiune_continut)
    {
        char *aux=(char*)malloc(512);
        int de_citit=512;
        if(dimensiune_continut - bytesread < 512)
        {
            de_citit = dimensiune_continut - bytesread;
        }
        int nr = read(client_descriptor,aux,de_citit);
        if(nr<0)
        {
            perror("Eroare la citirea copertii.\n");
        }
        for(int i=0;i<nr;i++)
        {
            continut[i+bytesread]=aux[i];
        }
        bytesread+=nr;
    }

    QByteArray img_data;
    for(int i=0;i<dimensiune_coperta;i++)
    {
        img_data[i]=coperta[i];
    }
    QByteArray file_data;
    for(int i=0;i<dimensiune_continut;i++)
    {
        file_data[i]=continut[i];
    }

    QSqlQuery query;

    char interogare[5000];
    sprintf(interogare,"SELECT * FROM autori WHERE nume = '%s' AND prenume = '%s'",carte.nume_autor,carte.prenume_autor);
    query.exec(interogare);
    if(query.size() == 0)
    {//insereaza autorul in BD
        sprintf(interogare,"INSERT INTO autori(nume,prenume) VALUES('%s','%s')",carte.nume_autor,carte.prenume_autor);
        query.exec(interogare);
    }

    sprintf(interogare,"SELECT id_autor FROM autori WHERE nume = '%s' AND prenume = '%s'",carte.nume_autor,carte.prenume_autor);
    query.exec(interogare);
    query.first();
    int id_autor = query.value(0).toInt();

    sprintf(interogare,"INSERT INTO carti VALUES('%s','%s',%d,%d,'%s', :fileData, :imageData, CURRENT_TIMESTAMP)",carte.isbn,carte.titlu,id_autor,carte.an_aparitie,carte.descriere);
    QString qry(interogare);
    query.prepare(qry);
    query.bindValue(":imageData",img_data);
    query.bindValue(":fileData",file_data);
    if( !query.exec() )
        qDebug() << "Eroare la inserare cartii in tabela:\n" << query.lastError();
    sprintf(interogare,"INSERT INTO rating VALUES('%s',0,0)",carte.isbn);
    query.exec(interogare);

    int id_genuri[carte.nr_genuri];
    for(int i=0;i<carte.nr_genuri;i++)
    {
        sprintf(interogare,"SELECT * FROM genuri WHERE nume_gen = '%s'",carte.genuri[i]);
        query.exec(interogare);
        if(query.size() == 0)
        {
            sprintf(interogare,"INSERT INTO genuri(nume_gen) VALUES('%s')",carte.genuri[i]);
            query.exec(interogare);
        }
        sprintf(interogare,"SELECT id_gen FROM genuri WHERE nume_gen = '%s'",carte.genuri[i]);
        query.exec(interogare);
        query.first();
        id_genuri[i]=query.value(0).toInt();
        sprintf(interogare,"INSERT INTO genuri_carte(isbn,id_gen) VALUES('%s',%d)",carte.isbn,id_genuri[i]);
        query.exec(interogare);
        sprintf(interogare,"INSERT INTO genuri_autori(id_autor,id_gen) VALUES(%d,%d)",id_autor,id_genuri[i]);
        query.exec(interogare);
    }

    int id_subgenuri[carte.nr_subgenuri];
    for(int i=0;i<carte.nr_subgenuri;i++)
    {
        sprintf(interogare,"SELECT * FROM subgenuri WHERE nume_subgen = '%s'",carte.subgenuri[i]);
        query.exec(interogare);
        if(query.size() == 0)
        {
            sprintf(interogare,"INSERT INTO subgenuri(nume_subgen) VALUES('%s')",carte.subgenuri[i]);
            query.exec(interogare);
        }
        sprintf(interogare,"SELECT id_subgen FROM subgenuri WHERE nume_subgen = '%s'",carte.subgenuri[i]);
        query.exec(interogare);
        query.first();
        id_subgenuri[i]=query.value(0).toInt();
        sprintf(interogare,"INSERT INTO subgenuri_carte(isbn,id_subgen) VALUES('%s',%d)",carte.isbn,id_subgenuri[i]);
        query.exec(interogare);
    }

}

