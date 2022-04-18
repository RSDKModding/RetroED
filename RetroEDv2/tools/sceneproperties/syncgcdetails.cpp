#include "includes.hpp"
#include "ui_syncgcdetails.h"

SyncGCDetails::SyncGCDetails(int curObjCount, QWidget *parent)
    : QDialog(parent), ui(new Ui::SyncGCDetails)
{
    ui->setupUi(this);

    ui->newObjCount->setValue(curObjCount);

    oldObjCount = ui->oldObjCount;
    newObjCount = ui->newObjCount;
}

SyncGCDetails::~SyncGCDetails() { delete ui; }

#include "moc_syncgcdetails.cpp"
