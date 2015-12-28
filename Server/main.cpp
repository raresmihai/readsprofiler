#include "login.h"
#include "register.h"
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

    while (1)
    {
        int client;
        thData * td;
        socklen_t length = sizeof (from);

        printf ("[server]Asteptam clienti la portul %d...\n",PORT);
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


    return a.exec();
}

static void *treat_client(void * arg)
{
    struct thData tdL;
    tdL= *((struct thData*)arg);
    printf ("[thread] Se trateaza clietul %d...\n", tdL.idThread);
    pthread_detach(pthread_self());
    int caz;
    while(1)
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
                close ((long)arg);//inchide conexiunea
                return(NULL);
            case 1:
                autentificare(tdL.cl,db);
                break;
            case 2:
                inregistrare(tdL.cl,db);
                break;
            }
        }

    }

}
