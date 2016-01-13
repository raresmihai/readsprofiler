#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QTextBrowser>
#include "upload.h"
#include "cautare.h"
#include "detail.h"
#include "recomandare.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0,char *user = 0);
    ~MainWindow();

private slots:


    void on_pushButton_clicked();

    void close_tab(int index);

    void on_gen_plus_clicked();

    void on_gen_minus_clicked();

    void on_subgen_plus_clicked();

    void on_subgen_minus_clicked();

    void on_upload_btn_clicked();

    void on_browse_coperta_clicked();

    void on_browse_continut_clicked();

    void on_btn_cautare_clicked();

    void descarcare_continut();

    void voteaza_carte();

    void closeEvent(QCloseEvent *);

    void on_recomandare1_clicked();

    void on_recomandare2_clicked();

    void on_recomandare3_clicked();

    void on_recomandare4_clicked();

    void on_recomandare5_clicked();

private:
    Ui::MainWindow *ui;
    int nr_genuri;
    int nr_subgenuri;
    int tree_index;
    int nr_taburi;
    int total_taburi;
    int item_index;
    int taburi_inchise;
    char *username;

    Upload upload;
    Cautare cautare;
    Detail detalii_carte;
    Recomandare recomandare;
    void setare_genuri(detaliiCarte &copieCarte);
    void setare_subgenuri(detaliiCarte &copieCarte);
    void clear_qlines();
    void afisare_recomandari();
    QIcon setare_rating(double rating);
    QPixmap imagine_rating(double rating);
};

#endif // MAINWINDOW_H
