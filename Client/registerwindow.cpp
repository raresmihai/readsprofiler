#include "registerwindow.h"
#include "ui_registerwindow.h"
RegisterWindow::RegisterWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->register_status1->setAlignment(Qt::AlignCenter);
    ui->register_status2->setAlignment(Qt::AlignCenter);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_inregistrare_clicked()
{
    QString username = ui->username_input->text();
    QString parola = ui->password_input->text();
    QString confParola = ui->password_confirm->text();
    QString nume = ui->nume_input->text();
    QString prenume = ui->prenume_input->text();
    QString email = ui->email_input->text();

    utilizator.setare_campuri(username,parola,confParola,nume,prenume,email);
    int validareDate=utilizator.datele_sunt_valide();
    if(validareDate>0){
        ui->register_status1->setText("");
        ui->register_status2->setText("");
        if(utilizator.trimite_datele_la_server()==0){
            ui->register_status1->setText("Eroare la trimiterea datelor catre server.");
        }
        else{
            if(utilizator.primeste_raspuns_de_la_server()==0){
                ui->register_status1->setText("Eroare la primirea raspunsului de la server.");
            }
            else{
                if(utilizator.este_inregistrat()){
                    ui->register_status1->setText("Inregistrare reusita!");
                   // ui->register_status2->setText("Fereastra se va inchide automat in 3 secunde.");
                }
                else{
                    ui->register_status1->setText("Utilizator deja existent.");
                    ui->register_status2->setText("Incercati un alt nume de utilizator.");
                }
            }
        }

    }
    else{
        ui->register_status1->setText(utilizator.obtine_mesaj_invalidare1(validareDate));
        ui->register_status2->setText(utilizator.obtine_mesaj_invalidare2(validareDate));
    }
}


