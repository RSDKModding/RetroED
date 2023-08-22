#include "includes.hpp"
#include "ui_gotopos.h"
#include "gotopos.hpp"


GoToPos::GoToPos(int scnMaxX, int scnMaxY, QString selLayer, QWidget *parent) :
    QDialog(parent), ui(new Ui::GoToPos)
{
    ui->setupUi(this);

    this->setWindowTitle("Go To...");
    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->selLayer->setText(QString("Selected Layer: %1").arg(selLayer));
    ui->maxX->setText(QString("/ %1").arg(scnMaxX));
    ui->maxY->setText(QString("/ %1").arg(scnMaxY));
    ui->goToX->setMaximum(scnMaxX);
    ui->goToY->setMaximum(scnMaxY);

    connect(ui->goToButton, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->goToButton, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(ui->goToX, QOverload<int>::of(&QSpinBox::valueChanged), [this](int x){ posX = x;});
    connect(ui->goToY, QOverload<int>::of(&QSpinBox::valueChanged), [this](int y){ posY = y;});
};

GoToPos::~GoToPos() { delete ui; }
