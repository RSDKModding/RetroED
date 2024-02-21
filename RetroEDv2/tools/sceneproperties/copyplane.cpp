#include "includes.hpp"
#include "ui_copyplane.h"
#include "copyplane.hpp"

CopyPlane::CopyPlane(QWidget *parent) : QDialog(parent), ui(new Ui::CopyPlane)
{
    ui->setupUi(this);

    this->setWindowTitle("Copy Plane...");
    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(ui->confirmButton, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->confirmButton, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->copyTileCol, &QCheckBox::toggled, [this](bool v){ copyTilePlanes = v;});
    connect(ui->copyChunkPlane, &QCheckBox::toggled, [this](bool v){ copyChunkPlane = v;});
}

CopyPlane::~CopyPlane() { delete ui; }
