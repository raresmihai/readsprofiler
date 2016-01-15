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
    ui->label_welcome->setText("Bine ai venit, " + QString(username));
    this->setFixedSize(this->size());
    ui->label_welcome->setAlignment(Qt::AlignCenter);
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->tabBar()->tabButton(0,QTabBar::RightSide)->resize(0,0);
    ui->tabWidget->tabBar()->tabButton(1,QTabBar::RightSide)->resize(0,0);
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(close_tab(int)));
    nr_genuri = 1;
    nr_subgenuri = 1;
    tree_index = 0;
    total_taburi = 2;
    nr_taburi = 2;
    ui->gen2->hide();ui->gen3->hide();ui->gen4->hide();
    ui->subgen2->hide();ui->subgen3->hide();ui->subgen4->hide();
    ui->tabWidget->setCurrentIndex(0);
    ui->tab_main->setWindowFlags(Qt::WindowTitleHint);
    afisare_recomandari(1,0);
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

        double val_rating = cautare.rezultate[item_index].rating;


        QByteArray img_data  = detalii_carte.primeste_detalii(cautare.rezultate[item_index].isbn,username);
        QPixmap pixmap_coperta = QPixmap();
        pixmap_coperta.loadFromData(img_data);

        QPixmap img_rating(imagine_rating(val_rating));

        ui->tabWidget->addTab(new QWidget,tr("Detalii"));
        ui->tabWidget->widget(nr_taburi)->setObjectName(QString::number(total_taburi));

        QGroupBox *gbox = new QGroupBox(ui->tabWidget->widget(nr_taburi));
        gbox->setGeometry(0,0,800,270);
        gbox->setObjectName("gbox" + QString::number(total_taburi));

        QLabel *ltitlu = new QLabel(titlu_carte,gbox);
        ltitlu->setGeometry(10,10,340,30);
        ltitlu->setAlignment(Qt::AlignCenter);
        QFont f("Serif",13,QFont::Bold,1);
        ltitlu->setFont(f);
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
        autor->setFrameStyle(QFrame::Box | QFrame::Sunken);
        QLabel *genuri = new QLabel(genuri_carte,gbox);
        genuri->setGeometry(110,100,240,20);
        genuri->setAlignment(Qt::AlignCenter);
        genuri->setFrameStyle(QFrame::Box | QFrame::Sunken);
        QLabel *subgenuri = new QLabel(subgenuri_carte,gbox);
        subgenuri->setGeometry(110,140,240,20);
        subgenuri->setAlignment(Qt::AlignCenter);
        subgenuri->setFrameStyle(QFrame::Box | QFrame::Sunken);
        QLabel *an = new QLabel(an_aparitie,gbox);
        an->setGeometry(110,180,240,20);
        an->setAlignment(Qt::AlignCenter);
        an->setFrameStyle(QFrame::Box | QFrame::Sunken);
        QLabel *isbn = new QLabel(cautare.rezultate[item_index].isbn,gbox);
        isbn->setGeometry(110,220,240,20);
        isbn->setAlignment(Qt::AlignCenter);
        isbn->setFrameStyle(QFrame::Box | QFrame::Sunken);
        isbn->setObjectName("isbn"+ QString::number(total_taburi));

        QLabel *ldescriere = new QLabel("Descriere",gbox);
        ldescriere->setGeometry(610,0,70,20);
        ldescriere->setAlignment(Qt::AlignCenter);
        QTextBrowser *descriere = new QTextBrowser(gbox);
        descriere->setGeometry(530,30,240,70);
        descriere->setText(cautare.rezultate[item_index].descriere);

        QLabel *status_votare = new QLabel("Vot inregistrat!",gbox);
        status_votare->hide();
        status_votare->setGeometry(550,140,220,25);
        status_votare->setAlignment(Qt::AlignCenter);
        status_votare->setFrameStyle(QFrame::Box | QFrame::Sunken);
        status_votare->setObjectName("status_votare" + QString::number(total_taburi));
        if(detalii_carte.a_mai_votat==false)
        {
            QComboBox *rating = new QComboBox(gbox);
            rating->setGeometry(550,130,90,27);
            rating->addItem("");rating->addItem("1");
            rating->addItem("2");rating->addItem("3");
            rating->addItem("4");rating->addItem("5");
            rating->setObjectName("valori_rating" + QString::number(total_taburi));
            QPushButton *voteaza = new QPushButton("Voteaza",gbox);
            voteaza->setGeometry(650,130,100,27);
            voteaza->setObjectName("btn_voteaza" + QString::number(total_taburi));
            connect(voteaza, SIGNAL (released()), this, SLOT (voteaza_carte()));
        }
        else
        {
            QLabel *afisare_rating = new QLabel("Rating acordat: ",gbox);
            afisare_rating->setGeometry(550,130,120,20);
            afisare_rating->setAlignment(Qt::AlignCenter);
            QLabel *rating_acordat = new QLabel(QString::number(detalii_carte.rating_acordat),gbox);
            rating_acordat->setGeometry(680,130,23,23);
            rating_acordat->setAlignment(Qt::AlignCenter);
            rating_acordat->setFrameStyle(QFrame::Box | QFrame::Sunken);
        }




        QCommandLinkButton *descarca = new QCommandLinkButton("Descarcare continut",gbox);
        descarca->setGeometry(560,200,185,40);
        descarca->setObjectName("btn_descarca" + QString::number(total_taburi));
        connect(descarca, SIGNAL (released()), this, SLOT (descarcare_continut()));

        //pozele
        QLabel *coperta = new QLabel(gbox);
        coperta->setScaledContents(true);
        coperta->setGeometry(360,0,100,143);
        coperta->setFrameStyle(QFrame::Box | QFrame::Plain);
        coperta->setPixmap(pixmap_coperta);

        QLabel *stelute = new QLabel(gbox);
        stelute->setScaledContents(true);
        stelute->setGeometry(355,170,135,27);
        //stelute->setFrameStyle(QFrame::Box | QFrame::Plain);
        stelute->setPixmap(img_rating);

        QLabel *bara = new QLabel(gbox);
        bara->setGeometry(500,0,5,250);
        bara->setScaledContents(true);
        bara->setFrameStyle(QFrame::Box | QFrame::Plain);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(gbox);
        ui->tabWidget->widget(nr_taburi)->setLayout(layout);
        ui->tabWidget->setCurrentIndex(nr_taburi);

        nr_taburi++;
        total_taburi++;

        afisare_recomandari(2,(char *)isbn->text().toStdString().c_str());
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

void MainWindow::descarcare_continut()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        QString nr_tab = ui->tabWidget->currentWidget()->objectName();
        QLabel *isbn_label = ui->tabWidget->findChild<QGroupBox *>("gbox"+nr_tab)->findChild<QLabel *>("isbn" + nr_tab);
        char isbn[20];
        strcpy(isbn,isbn_label->text().toStdString().c_str());
        QByteArray file_data = detalii_carte.primeste_continut(isbn,username);
        file.write(file_data);
        file.close();
    }
}

void MainWindow::voteaza_carte()
{
    QString nr_tab = ui->tabWidget->currentWidget()->objectName();
    QComboBox *valori_rating = ui->tabWidget->findChild<QGroupBox *>("gbox"+nr_tab)->findChild<QComboBox *>("valori_rating" + nr_tab);
    QString qs_rating_acordat = valori_rating->currentText();
    int rating_acordat = qs_rating_acordat.toInt();
    QLabel *isbn_label = ui->tabWidget->findChild<QGroupBox *>("gbox"+nr_tab)->findChild<QLabel *>("isbn" + nr_tab);
    char isbn[20];
    strcpy(isbn,isbn_label->text().toStdString().c_str());
    bool ok = detalii_carte.voteaza(isbn,username,rating_acordat);
    if(ok)
    {
        QGroupBox *gbox = ui->tabWidget->findChild<QGroupBox *>("gbox" + nr_tab);
        QPushButton *btn_voteaza = gbox->findChild<QPushButton *>("btn_voteaza" + nr_tab);
        btn_voteaza->close();
        valori_rating->close();
        QLabel *status_votare = gbox->findChild<QLabel *>("status_votare" + nr_tab);

        status_votare->show();
    }
    else
    {
        QGroupBox *gbox = ui->tabWidget->findChild<QGroupBox *>("gbox" + nr_tab);
        QPushButton *btn_voteaza = gbox->findChild<QPushButton *>("btn_voteaza" + nr_tab);
        btn_voteaza->close();
        valori_rating->close();
        QLabel *status_votare = gbox->findChild<QLabel *>("status_votare" + nr_tab);
        status_votare->setText("Ai votat deja aceasta carte.");
        status_votare->show();
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
    if(fileData.size() > 0 && fileData.size() < 2000000)
    {
        upload.setare_continut(fileData);
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

    bool ok;
    int val_rating;
    val_rating = qrating.toInt(&ok,10);
    if(ok){
        qrating = QString::number(val_rating);
    }

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
    cautare.trimite_datele_la_server(username);
    cautare.primeste_date_de_la_server();

    ui->treeWidget->clear();
    for(int i=0;i<cautare.nr_rezultate;i++)
    {
        QTreeWidgetItem *newItem = new QTreeWidgetItem;
        newItem->setText(0,QString::number(tree_index));
        newItem->setText(1,cautare.rezultate[i].titlu);
        QString autor(QString::fromStdString(cautare.rezultate[i].prenume_autor) + " " + QString::fromStdString(cautare.rezultate[i].nume_autor));
        newItem->setText(2,autor);
        newItem->setText(3,cautare.rezultate[i].genuri);
        newItem->setText(4,cautare.rezultate[i].subgenuri);
        newItem->setText(5,QString::number(cautare.rezultate[i].an_aparitie));
        QIcon ico = setare_rating(cautare.rezultate[i].rating);
        newItem->setIcon(6,ico);
        QSize size(95,95);
        ui->treeWidget->setIconSize(size);
        ui->treeWidget->addTopLevelItem(newItem);
        tree_index++;
    }
    ui->tabWidget->setCurrentIndex(0);
    afisare_recomandari(1,0);
}

void MainWindow::afisare_recomandari(int pagina,char *isbn)
{

    int caz_recomandare = 3;//istoric
    if(pagina == 2)
    {
        caz_recomandare = 2;//similare
    }
    else
    {
        int caz_afisare = qrand() % 4;//sansa 1/4 sa nimereaca top, in rest -> recomandari bazate pe istoric
        if(caz_afisare == 0)
        {
            caz_recomandare = 1;//top
        }
    }
    caz_recomandare = recomandare.cere_recomandari(caz_recomandare,username,isbn);
    if(caz_recomandare == 1)
    {
        ui->label_recomandari->setText("Top 5 carti cu cel putin 3 voturi:");
    }
    else
        if(caz_recomandare == 2)
        {
            ui->label_recomandari->setText("Utilizatorii au apreciat de asemenea:");
        }
        else
        {
            ui->label_recomandari->setText("S-ar putea sa iti placa:");
        }

    for(int i=0;i<5;i++)
    {
        QByteArray coperta = recomandare.primeste_recomandare(i);
        QPixmap pixmap_coperta = QPixmap();
        pixmap_coperta.loadFromData(coperta);
        QIcon ButtonIcon(pixmap_coperta);
        switch(i)
        {
        case 0:
            ui->recomandare1->setIcon(ButtonIcon);
            break;
        case 1:
            ui->recomandare2->setIcon(ButtonIcon);
            break;
        case 2:
            ui->recomandare3->setIcon(ButtonIcon);
            break;
        case 3:
            ui->recomandare4->setIcon(ButtonIcon);
            break;
        case 4:
            ui->recomandare5->setIcon(ButtonIcon);
            break;
        }
    }
}

QIcon MainWindow::setare_rating(double rating)
{
    if(rating==0){ QIcon ico(":/rating/ratings/0_star.png"); return ico; }
    else if(rating<=0.5){ QIcon ico(":/rating/ratings/05_star.png"); return ico;}
    else if(rating<=1){ QIcon ico(":/rating/ratings/1_star.png"); return ico;}
    else if(rating<=1.5){ QIcon ico(":/rating/ratings/15_star.png"); return ico;}
    else if(rating<=2){ QIcon ico(":/rating/ratings/2_star.png"); return ico;}
    else if(rating<=2.5){ QIcon ico(":/rating/ratings/25_star.png"); return ico;}
    else if(rating<=3){ QIcon ico(":/rating/ratings/3_star.png"); return ico;}
    else if(rating<=3.5){ QIcon ico(":/rating/ratings/35_star.png"); return ico;}
    else if(rating<=4){ QIcon ico(":/rating/ratings/4_star.png"); return ico;}
    else if(rating<=4.5){ QIcon ico(":/rating/ratings/45_star.png"); return ico;}
    else{ QIcon ico(":/rating/ratings/5_star.png"); return ico;}
}

QPixmap MainWindow::imagine_rating(double rating)
{
    if(rating==0){ QPixmap ico(":/rating/ratings/0_star.png"); return ico; }
    else if(rating<=0.5){ QPixmap ico(":/rating/ratings/05_star.png"); return ico;}
    else if(rating<=1){ QPixmap ico(":/rating/ratings/1_star.png"); return ico;}
    else if(rating<=1.5){ QPixmap ico(":/rating/ratings/15_star.png"); return ico;}
    else if(rating<=2){ QPixmap ico(":/rating/ratings/2_star.png"); return ico;}
    else if(rating<=2.5){ QPixmap ico(":/rating/ratings/25_star.png"); return ico;}
    else if(rating<=3){ QPixmap ico(":/rating/ratings/3_star.png"); return ico;}
    else if(rating<=3.5){ QPixmap ico(":/rating/ratings/35_star.png"); return ico;}
    else if(rating<=4){ QPixmap ico(":/rating/ratings/4_star.png"); return ico;}
    else if(rating<=4.5){ QPixmap ico(":/rating/ratings/45_star.png"); return ico;}
    else{ QPixmap ico(":/rating/ratings/5_star.png"); return ico;}
}

void MainWindow::closeEvent(QCloseEvent *)
{
    cautare.inchide_conexiunea();
}

void MainWindow::clear_qlines()
{
    ui->cautare_an->clear();
    ui->cautare_gen->clear();
    ui->cautare_nume->clear();
    ui->cautare_prenume->clear();
    ui->cautare_rating->setCurrentIndex(0);
    ui->cautare_subgen->clear();
    ui->cautare_titlu->clear();
}

void MainWindow::on_recomandare1_clicked()
{
    clear_qlines();
    ui->cautare_isbn->setText(QString(recomandare.isbn_recomandari[0]));
    ui->btn_cautare->click();
}

void MainWindow::on_recomandare2_clicked()
{
    clear_qlines();
    ui->cautare_isbn->setText(QString(recomandare.isbn_recomandari[1]));
    ui->btn_cautare->click();
}

void MainWindow::on_recomandare3_clicked()
{
    clear_qlines();
    ui->cautare_isbn->setText(QString(recomandare.isbn_recomandari[2]));
    ui->btn_cautare->click();
}

void MainWindow::on_recomandare4_clicked()
{
    clear_qlines();
    ui->cautare_isbn->setText(QString(recomandare.isbn_recomandari[3]));
    ui->btn_cautare->click();
}

void MainWindow::on_recomandare5_clicked()
{
    clear_qlines();
    ui->cautare_isbn->setText(QString(recomandare.isbn_recomandari[4]));
    ui->btn_cautare->click();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index>1)
    {
        QString nr_tab = ui->tabWidget->currentWidget()->objectName();
        QLabel *isbn_label = ui->tabWidget->findChild<QGroupBox *>("gbox"+nr_tab)->findChild<QLabel *>("isbn" + nr_tab);
        char isbn[20];
        strcpy(isbn,isbn_label->text().toStdString().c_str());
        afisare_recomandari(2,isbn);
    }
    else
    {
        afisare_recomandari(1,0);
    }
}
