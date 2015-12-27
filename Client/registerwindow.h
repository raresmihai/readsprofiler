#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>
#include "register.h"

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = 0);
    ~RegisterWindow();

private slots:
    void on_inregistrare_clicked();

private:
    Ui::RegisterWindow *ui;
    Register utilizator;
};

#endif // REGISTERWINDOW_H
