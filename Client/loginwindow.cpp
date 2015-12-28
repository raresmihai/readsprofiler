#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "login.h"
#include "registerwindow.h"
#include "mainwindow.h"
LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->login_status->setAlignment(Qt::AlignCenter);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_login_clicked(){
    QString nume = ui->username_input->text();
    QString pw = ui->password_input->text();
    char *username = (char*)malloc(sizeof(nume));
    char *parola = (char*)malloc(sizeof(pw));
    strcpy(username,nume.toStdString().c_str());
    strcpy(parola,pw.toStdString().c_str());

    utilizator.setare_campuri(username,parola);
    int validareDate=utilizator.datele_sunt_valide();
    if(validareDate>0){
        if(utilizator.trimite_datele_la_server()==0){
            ui->login_status->setText("Eroare la trimiterea datelor catre server.");
        }
        else{
            if(utilizator.primeste_raspuns_de_la_server()==0){
                ui->login_status->setText("Eroare la primirea raspunsului de la server.");
            }
            else{
                if(utilizator.este_autentificat()){
                    MainWindow *app = 0;
                    app = new MainWindow();
                    app->show();
                    this->close();
                }
                else{
                    ui->login_status->setText("Numele de utilizator inexistent sau parola incorecta.");
                }
            }
        }
    }
    else{
        afiseaza_mesaj_invalidare(validareDate);
    }

}

void LoginWindow::deschide_aplicatia(){
    ui->login_status->setText("Merge");
}

void LoginWindow::afiseaza_mesaj_invalidare(int caz){
    ui->login_status->setText(utilizator.mesaj_date_invalide(caz));
    ui->login_status->setStyleSheet("QLabel { background-color : red; color : white; }");
}

void LoginWindow::on_inregistrare_clicked()
{
    RegisterWindow inregistrare;
    inregistrare.setModal(true);
    inregistrare.exec();
}

void LoginWindow::closeEvent(QCloseEvent *event)
{
    utilizator.inchide_conexiunea();
}
