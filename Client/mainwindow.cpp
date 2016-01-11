#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
MainWindow::MainWindow(QWidget *parent,char *user) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    username = user;
    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->label_welcome->setAlignment(Qt::AlignCenter);
    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(close_tab(int)));
    nr_genuri = 1;
    nr_subgenuri = 1;
    tree_index = 0;
    nr_taburi = 2;
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
    QTreeWidgetItem *selectedItem = ui->treeWidget->currentItem();
    if(selectedItem!=NULL)
    {
        item_index = selectedItem->text(0).toInt()-1;
        QString titlu_carte = selectedItem->text(1);
        QString autor_carte = selectedItem->text(2);
        QString genuri_carte = selectedItem->text(3);
        QString subgenuri_carte = selectedItem->text(4);
        QString an_aparitie = selectedItem->text(5);

        double val_rating = selectedItem->text(6).toDouble();


        QByteArray img_data  = detalii_carte.primeste_coperta(cautare.rezultate[item_index].isbn);
        QPixmap pixmap_coperta = QPixmap();
        pixmap_coperta.loadFromData(img_data);
         //   ui->img_lab->setPixmap(outPixmap);*/
        //QString titlu_carte = newItem->text(1);
        //QIcon ico(":/imagini/rating.png");
       // QPixmap img(":/rating.png");
       // ui->label->setPixmap(img);
       // newItem->setIcon(6,ico);
       // QSize size(95,95);
       // ui->treeWidget->setIconSize(size);


       // ui->treeWidget->addTopLevelItem(newItem);

        ui->tabWidget->addTab(new QWidget,tr("Detail"));
        QGroupBox *gbox = new QGroupBox(ui->tabWidget->widget(nr_taburi));

        gbox->setGeometry(0,0,800,270);

        QLabel *ltitlu = new QLabel(titlu_carte,gbox);
        ltitlu->setGeometry(10,10,340,30);
        ltitlu->setAlignment(Qt::AlignCenter);
        QLabel *lautor = new QLabel("Autor:",gbox);
        lautor->setGeometry(10,60,92,17);
        QLabel *lgenuri = new QLabel("Genuri:",gbox);
        lgenuri->setGeometry(10,100,92,17);
        QLabel *lsubgenuri = new QLabel("Subgenuri:",gbox);
        lsubgenuri->setGeometry(10,140,92,17);
        QLabel *lan= new QLabel("An aparitie:",gbox);
        lan->setGeometry(10,180,92,17);
        QLabel *lisbn = new QLabel("ISBN:",gbox);
        lisbn->setGeometry(10,220,92,17);

        QLabel *autor = new QLabel(autor_carte,gbox);
        autor->setGeometry(110,60,240,20);
        autor->setAlignment(Qt::AlignCenter);
        QLabel *genuri = new QLabel(genuri_carte,gbox);
        genuri->setGeometry(110,100,240,20);
        genuri->setAlignment(Qt::AlignCenter);
        QLabel *subgenuri = new QLabel(subgenuri_carte,gbox);
        subgenuri->setGeometry(110,140,240,20);
        subgenuri->setAlignment(Qt::AlignCenter);
        QLabel *an = new QLabel(an_aparitie,gbox);
        an->setGeometry(110,180,240,20);
        an->setAlignment(Qt::AlignCenter);
        QLabel *isbn = new QLabel(cautare.rezultate[item_index].isbn,gbox);
        isbn->setGeometry(110,220,240,20);
        isbn->setAlignment(Qt::AlignCenter);

        QLabel *ldescriere = new QLabel("Descriere",gbox);
        ldescriere->setGeometry(610,0,70,20);
        ldescriere->setAlignment(Qt::AlignCenter);
        QTextBrowser *descriere = new QTextBrowser(gbox);
        descriere->setGeometry(530,30,240,70);
        descriere->setText(cautare.rezultate[item_index].descriere);

        QComboBox *rating = new QComboBox(gbox);
        rating->setGeometry(550,130,90,27);
        rating->addItem("");rating->addItem("1");
        rating->addItem("2");rating->addItem("3");
        rating->addItem("4");rating->addItem("5");
        QPushButton *voteaza = new QPushButton("Voteaza",gbox);
        voteaza->setGeometry(650,130,100,27);

        QCommandLinkButton *descarca = new QCommandLinkButton("Descarca",gbox);
        descarca->setGeometry(560,200,185,40);
        descarca->setObjectName("btn_descarca");
        connect(descarca, SIGNAL (released()), this, SLOT (descarcare_continut()));

        //pozele
        QLabel *coperta = new QLabel(gbox);
        coperta->setScaledContents(true);
        coperta->setGeometry(360,0,100,143);
        coperta->setFrameStyle(QFrame::Box | QFrame::Plain);
        coperta->setPixmap(pixmap_coperta);

        QLabel *stelute = new QLabel(QString::number(val_rating),gbox);
        stelute->setScaledContents(true);
        stelute->setGeometry(355,180,135,27);
        stelute->setFrameStyle(QFrame::Box | QFrame::Plain);

        QLabel *bara = new QLabel(gbox);
        bara->setGeometry(500,0,5,250);
        bara->setScaledContents(true);
        bara->setFrameStyle(QFrame::Box | QFrame::Plain);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(gbox);
        ui->tabWidget->widget(nr_taburi)->setLayout(layout);
        ui->tabWidget->setCurrentIndex(nr_taburi);

        nr_taburi++;
    }

}

void MainWindow::close_tab(int index)
{
    if(index>1)
    {
         ui->tabWidget->removeTab(index);
         nr_taburi--;
    }
}

void MainWindow::descarcare_continut()
{
    QByteArray file_data = detalii_carte.primeste_continut(cautare.rezultate[item_index].isbn);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(file_data);
    file.close();
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

void MainWindow::on_btn_cautare_clicked()
{
    tree_index = 1;
    QString qtitlu = ui->cautare_titlu->text();
    QString qisbn = ui->cautare_isbn->text();
    QString qnume_autor = ui->cautare_nume->text();
    QString qprenume_autor = ui->cautare_prenume->text();
    QString qgen = ui->cautare_gen->text();
    QString qsubgen = ui->cautare_subgen->text();
    QString qan_aparitie = ui->cautare_an->text();
    QString qrating = ui->cautare_rating->currentText();

    detaliiCautare copieDate;
    strcpy(copieDate.an_aparitie,qan_aparitie.toStdString().c_str());
    strcpy(copieDate.rating,qrating.toStdString().c_str());
    strcpy(copieDate.titlu,qtitlu.toStdString().c_str());
    strcpy(copieDate.isbn,qisbn.toStdString().c_str());
    strcpy(copieDate.nume_autor,qnume_autor.toStdString().c_str());
    strcpy(copieDate.prenume_autor,qprenume_autor.toStdString().c_str());
    strcpy(copieDate.gen,qgen.toStdString().c_str());
    strcpy(copieDate.subgen,qsubgen.toStdString().c_str());

    cautare.setare_campuri(copieDate);
    cautare.trimite_datele_la_server();
    cautare.primeste_date_de_la_server();

    ui->treeWidget->clear();
    for(int i=0;i<cautare.nr_rezultate;i++)
    {
        QTreeWidgetItem *newItem = new QTreeWidgetItem;
        newItem->setText(0,QString::number(tree_index));
        newItem->setText(1,cautare.rezultate[i].titlu);
        QString autor(QString::fromStdString(cautare.rezultate[i].nume_autor) + " " + QString::fromStdString(cautare.rezultate[i].prenume_autor));
        newItem->setText(2,autor);
        newItem->setText(3,cautare.rezultate[i].genuri);
        newItem->setText(4,cautare.rezultate[i].subgenuri);
        newItem->setText(5,QString::number(cautare.rezultate[i].an_aparitie));
        newItem->setText(6,QString::number(cautare.rezultate[i].rating));
        ui->treeWidget->addTopLevelItem(newItem);
        tree_index++;
    }
    ui->tabWidget->setCurrentIndex(0);
}


