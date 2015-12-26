#include "registerwindow.h"
#include "ui_registerwindow.h"

RegisterWindow::RegisterWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}
