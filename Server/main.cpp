#include "login.h"
#include "register.h"
#include "upload.h"
#include "cautare.h"
#include "detalii.h"
#include "continut.h"
#include "voteaza.h"
#include "recomandare.h"

using namespace std;
#define PORT 3000

typedef struct thData{
    int idThread;
    int cl;
}thData;

static void *treat_client(void *);

QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    struct sockaddr_in server;
    struct sockaddr_in from;
    int sd;
    pthread_t th[10000];
    int i=0;

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server]Eroare la socket().\n");
        return errno;
    }

    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);
    int opt = 1;
    setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,(void *) &opt,sizeof(opt));

    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
        perror ("[server]Eroare la bind().\n");
        return errno;
    }

    if (listen (sd, 2) == -1)
    {
        perror ("[server]Eroare la listen().\n");
        return errno;
    }

    db.setHostName("localhost");
    db.setDatabaseName("readsprofiler");
    db.setUserName("admin");
    db.setPassword("retele");
    if(!db.open()){
        qDebug("Eroare la conectarea cu baza de date.");
    }
    printf ("[server]Asteptam clienti la portul %d...\n",PORT);
    while (1)
    {
        int client;
        thData * td;
        socklen_t length = sizeof (from);


        fflush (stdout);

        if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
        {
            perror ("[server]Eroare la accept().\n");
            continue;
        }


        td=(struct thData*)malloc(sizeof(struct thData));
        td->idThread=i++;
        td->cl=client;

        pthread_create(&th[i], NULL, &treat_client, td);
    }
    /*char buffer[10000];

    FILE* p_infile = 0;
    FILE* p_outfile = 0;
    int length = 0;
    int done = 0;
    p_infile = fopen("rating.png","r");
    if(p_infile==NULL) perror("Eroare deschidere fisier");
    else{
        fgets(buffer,10000,p_infile);
    }
    QByteArray inByteArray(buffer,10000);
    QSqlQuery query;
    query.prepare( "INSERT INTO fisiere (nume, imagedata) VALUES ('rating.png', :imageData)" );
        query.bindValue( ":imageData", inByteArray );
        if( !query.exec() )
            qDebug() << "Error inserting image into table:\n" << query.lastError();
*/
    return a.exec();
}


static void *treat_client(void * arg)
{
    struct thData tdL;
    tdL= *((struct thData*)arg);
    printf ("[thread] S-a conectat clientul %d...\n", tdL.idThread);
    pthread_detach(pthread_self());
    int caz;
    bool clientulEsteConectat = true;
    while(clientulEsteConectat)
    {
        if(read(tdL.cl, &caz,sizeof(int)) <= 0) //e posibil sa trebuiasca select
        {
            printf("[Thread %d]\n",tdL.idThread);
            perror ("Eroare la read() de la client pt caz. Se inchide conexiunea...\n");
            close ((long)arg);//inchide conexiunea
            return(NULL);
        }
        else
        {
            switch(caz)
            {
            case 0:
                printf("Am inchis conexiunea pentru clientul %d.\n",tdL.idThread);
                close ((long)arg);//inchide conexiunea
                clientulEsteConectat = false;
                return(NULL);
            case 1:
                printf("Se incearca autentificarea pentru clientul %d.\n",tdL.idThread);
                autentificare(tdL.cl);
                break;
            case 2:
                printf("Se incearca inregistrarea pentru clientul %d.\n",tdL.idThread);
                inregistrare(tdL.cl);
                break;
            case 3:
                printf("Se incearca cautarea de carti pentru clientul %d.\n",tdL.idThread);
                cautare(tdL.cl);
                break;
            case 4:
                printf("Se incearca recomandarea de carti pentru clientul %d.\n",tdL.idThread);
                recomandare_carti(tdL.cl);
                break;
            case 5:
                printf("Se incearca Upload de carte pentru clientul %d.\n",tdL.idThread);
                upload_carte(tdL.cl);
                break;
            case 6:
                printf("Se incearca descarcarea detaliilor unei coperti pentru clientul %d.\n",tdL.idThread);
                trimite_detalii_la_client(tdL.cl);
                break;
            case 7:
                printf("Se incearca descarcarea unei continut de carte pentru clientul %d.\n",tdL.idThread);
                trimite_continut_la_client(tdL.cl);
                break;
            case 8:
                printf("Se incearca votarea unei carti de catre clientul %d.\n",tdL.idThread);
                voteaza_carte(tdL.cl);
                break;
            case 20:
                printf("Clientul %d a parasit serviciul inregistrare.\n",tdL.idThread);
                break;

            }
        }

    }

}

