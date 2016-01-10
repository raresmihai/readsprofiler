#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->label_welcome->setAlignment(Qt::AlignCenter);
    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(close_tab(int)));
    nr_genuri = 1;
    nr_subgenuri = 1;
    ui->gen2->hide();ui->gen3->hide();ui->gen4->hide();
    ui->subgen2->hide();ui->subgen3->hide();ui->subgen4->hide();
    ui->tabWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
   // ui->tabWidget->setTabEnabled(1,false);
    QTreeWidgetItem *newItem = new QTreeWidgetItem;
    newItem->setText(0, "TItlu");
    newItem->setText(1,"Autor");
    newItem->setText(2,"gen1 gen2");
    QIcon ico(":/imagini/rating.png");
   // QPixmap img(":/rating.png");
   // ui->label->setPixmap(img);
    newItem->setIcon(3,ico);
    QSize size(100,100);
    ui->treeWidget->setIconSize(size);


    ui->treeWidget->addTopLevelItem(newItem);
    ui->tabWidget->addTab(new QWidget,tr("Detail"));
    
}

void MainWindow::close_tab(int index)
{
    if(index>1)
    {
         ui->tabWidget->removeTab(index);
    }
}

void MainWindow::on_gen_plus_clicked()
{
    if(nr_genuri < 4)
    {
        nr_genuri++;
        switch(nr_genuri)
        {
        case 2:
            ui->gen2->show();
            break;
        case 3:
            ui->gen3->show();
            break;
        case 4:
            ui->gen4->show();
            break;
        }
    }
}

void MainWindow::on_gen_minus_clicked()
{
    if(nr_genuri > 1)
    {
        switch(nr_genuri)
        {
        case 2:
            ui->gen2->hide();
            break;
        case 3:
            ui->gen3->hide();
            break;
        case 4:
            ui->gen4->hide();
            break;
        }
        nr_genuri--;
    }
}

void MainWindow::on_subgen_plus_clicked()
{
    if(nr_subgenuri < 4)
    {
        nr_subgenuri++;
        switch(nr_subgenuri)
        {
        case 2:
            ui->subgen2->show();
            break;
        case 3:
            ui->subgen3->show();
            break;
        case 4:
            ui->subgen4->show();
            break;
        }
    }
}

void MainWindow::on_subgen_minus_clicked()
{
    if(nr_subgenuri > 1)
    {
        switch(nr_subgenuri)
        {
        case 2:
            ui->subgen2->hide();
            break;
        case 3:
            ui->subgen3->hide();
            break;
        case 4:
            ui->subgen4->hide();
            break;
        }
        nr_subgenuri--;
    }
}

void MainWindow::on_upload_btn_clicked()
{
    QString qtitlu = ui->titlu_carte->text();
    QString qisbn = ui->isbn->text();
    QString qnume_autor = ui->nume_autor->text();
    QString qprenume_autor = ui->prenume_autor->text();
    QString qdescriere = ui->descriere->toPlainText();
    int  qan_aparitie = ui->an_aparitie->text().toInt();

    detaliiCarte copieCarte;
    copieCarte.nr_genuri = nr_genuri;
    copieCarte.nr_subgenuri = nr_subgenuri;
    copieCarte.an_aparitie = qan_aparitie;
    strcpy(copieCarte.titlu,qtitlu.toStdString().c_str());
    strcpy(copieCarte.isbn,qisbn.toStdString().c_str());
    strcpy(copieCarte.nume_autor,qnume_autor.toStdString().c_str());
    strcpy(copieCarte.prenume_autor,qprenume_autor.toStdString().c_str());
    strcpy(copieCarte.descriere,qdescriere.toStdString().c_str());
    setare_genuri(copieCarte);
    setare_subgenuri(copieCarte);

    upload.setare_campuri(copieCarte);
    upload.trimite_datele_la_server();
    //de trimis structura la server
    //de trimis coperta
    //de trimis continutul
}

void MainWindow::setare_genuri(detaliiCarte &copieCarte)
{
    for(int i = 0;i<4;i++)
    {
        strcpy(copieCarte.genuri[i],"\0");
    }
    switch(nr_genuri)
    {
    case 1:
        strcpy(copieCarte.genuri[0],ui->gen1->text().toStdString().c_str());
        break;
    case 2:
        strcpy(copieCarte.genuri[0],ui->gen1->text().toStdString().c_str());
        strcpy(copieCarte.genuri[1],ui->gen2->text().toStdString().c_str());
        break;
    case 3:
        strcpy(copieCarte.genuri[0],ui->gen1->text().toStdString().c_str());
        strcpy(copieCarte.genuri[1],ui->gen2->text().toStdString().c_str());
        strcpy(copieCarte.genuri[2],ui->gen3->text().toStdString().c_str());
        break;
    case 4:
        strcpy(copieCarte.genuri[0],ui->gen1->text().toStdString().c_str());
        strcpy(copieCarte.genuri[1],ui->gen2->text().toStdString().c_str());
        strcpy(copieCarte.genuri[2],ui->gen3->text().toStdString().c_str());
        strcpy(copieCarte.genuri[3],ui->gen4->text().toStdString().c_str());
        break;
    }
}

void MainWindow::setare_subgenuri(detaliiCarte &copieCarte)
{
    for(int i = 0;i<4;i++)
    {
        strcpy(copieCarte.subgenuri[i],"\0");
    }
    switch(nr_subgenuri)
    {
    case 1:
        strcpy(copieCarte.subgenuri[0],ui->subgen1->text().toStdString().c_str());
        break;
    case 2:
        strcpy(copieCarte.subgenuri[0],ui->subgen1->text().toStdString().c_str());
        strcpy(copieCarte.subgenuri[1],ui->subgen2->text().toStdString().c_str());
        break;
    case 3:
        strcpy(copieCarte.subgenuri[0],ui->subgen1->text().toStdString().c_str());
        strcpy(copieCarte.subgenuri[1],ui->subgen2->text().toStdString().c_str());
        strcpy(copieCarte.subgenuri[2],ui->subgen3->text().toStdString().c_str());
        break;
    case 4:
        strcpy(copieCarte.subgenuri[0],ui->subgen1->text().toStdString().c_str());
        strcpy(copieCarte.subgenuri[1],ui->subgen2->text().toStdString().c_str());
        strcpy(copieCarte.subgenuri[2],ui->subgen3->text().toStdString().c_str());
        strcpy(copieCarte.subgenuri[3],ui->subgen4->text().toStdString().c_str());
        break;
    }
}

void MainWindow::on_browse_coperta_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Alege coperta"), "",tr("Imagini (*.png *.jpg)"));
    QFile fileIn(filename);
    fileIn.open(QFile::ReadOnly);
    QByteArray imgData = fileIn.readAll();
    upload.setare_coperta(imgData);
    if(imgData.size() > 0)
    {
        ui->status_coperta->setText("Coperta selectata.");
    }
}

void MainWindow::on_browse_continut_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Alege fisierul"), "",tr("Fisiere (*)"));
    QFile fileIn(filename);
    fileIn.open(QFile::ReadOnly);
    QByteArray fileData = fileIn.readAll();
    upload.setare_continut(fileData);
    if(fileData.size() > 0)
    {
        ui->status_continut->setText("Fisier selectat.");
    }
}
