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
    connect(ui->copyTileCol, &QCheckBox::toggled, [this](bool v){
        copyTilePlanes = v;
        ui->allTileCheck->blockSignals(true);
        ui->allTileCheck->setEnabled(v);
        ui->allTileCheck->blockSignals(false);
    });
    connect(ui->allTileCheck, &QCheckBox::toggled, [this](bool v){ applyAllTiles = v; });
    connect(ui->copyChunkPlane, &QCheckBox::toggled, [this](bool v){
        copyChunkPlane = v;
        ui->allChkCheck->blockSignals(true);
        ui->allChkCheck->setEnabled(v);
        ui->allChkCheck->blockSignals(false);
    });
    connect(ui->allChkCheck, &QCheckBox::toggled, [this](bool v){ applyAllChks = v; });
}

CopyPlane::~CopyPlane() { delete ui; }
