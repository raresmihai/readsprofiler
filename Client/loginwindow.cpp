#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "login.h"
#include "registerwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
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

    Login utilizator(username,parola);
    if(utilizator.datele_sunt_valide()){
        utilizator.trimite_datele_la_server();
        utilizator.primeste_raspuns_de_la_server();
        if(utilizator.este_autentificat()){
            deschide_aplicatia();
        }
        else{
            afiseaza_mesaj_invalidare();
        }
    }
    else{
        afiseaza_mesaj_invalidare();
    }

}

void LoginWindow::deschide_aplicatia(){
    ui->login_status->setText("Merge");
}

void LoginWindow::afiseaza_mesaj_invalidare(){
    ui->login_status->setText(" Numele de utilizator inexistent sau parola incorecta.");
    ui->login_status->setStyleSheet("QLabel { background-color : red; color : white; }");
}

void LoginWindow::on_inregistrare_clicked()
{
    RegisterWindow inregistrare;
    inregistrare.setModal(true);
    inregistrare.exec();
    int i;
}
