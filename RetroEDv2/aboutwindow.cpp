#include "includes.hpp"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    // remove question mark from the title bar & disable resizing
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setFixedSize(QSize(this->width(), this->height()));

    ui->versionLabel->setText(RE_VERSION);
#if RE_BUILD_TYPE != (2)
    ui->autobuildLabel->hide();
#endif
}

AboutWindow::~AboutWindow() { delete ui; }
