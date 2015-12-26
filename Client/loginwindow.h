#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::LoginWindow *ui;
    void deschide_aplicatia();
    void afiseaza_mesaj_invalidare();
};

#endif // LOGINWINDOW_H
