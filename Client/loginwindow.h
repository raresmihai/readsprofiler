#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include "login.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_login_clicked();

    void on_inregistrare_clicked();

    void closeEvent(QCloseEvent *);

private:
    Ui::LoginWindow *ui;
    Login utilizator;
    void deschide_aplicatia();
    void afiseaza_mesaj_invalidare(int caz);
};

#endif // LOGINWINDOW_H
