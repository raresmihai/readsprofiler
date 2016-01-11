#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QFile>
#include <QFileDialog>
#include "upload.h"
#include "cautare.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
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

private:
    Ui::MainWindow *ui;
    int nr_genuri;
    int nr_subgenuri;
    int tree_index;
    Upload upload;
    Cautare cautare;
    void setare_genuri(detaliiCarte &copieCarte);
    void setare_subgenuri(detaliiCarte &copieCarte);
};

#endif // MAINWINDOW_H
