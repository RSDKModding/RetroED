#include "includes.hpp"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    //QGraphicsScene
}

AboutWindow::~AboutWindow()
{
    delete ui;
}
