#include "includes.hpp"
#include "ui_syncgcdetails.h"

#include "syncgcdetails.hpp"

SyncGCDetails::SyncGCDetails(int curObjCount, QWidget *parent)
    : QDialog(parent), ui(new Ui::SyncGCDetails)
{
    ui->setupUi(this);

    // remove question mark from the title bar & disable resizing
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(QSize(width(), height()));

    ui->newObjCount->setValue(curObjCount);

    oldObjCount = ui->oldObjCount;
    newObjCount = ui->newObjCount;
}

SyncGCDetails::~SyncGCDetails() { delete ui; }

#include "moc_syncgcdetails.cpp"
