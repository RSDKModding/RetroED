#include "includes.hpp"
#include "ui_rsvunpacker.h"

RSVUnpacker::RSVUnpacker(QWidget *parent) : QWidget(parent), ui(new Ui::RSVUnpacker)
{
    ui->setupUi(this);
}

RSVUnpacker::~RSVUnpacker() { delete ui; }

#include "moc_rsvunpacker.cpp"
