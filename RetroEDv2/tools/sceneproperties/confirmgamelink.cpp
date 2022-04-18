#include "includes.hpp"
#include "ui_confirmgamelink.h"

ConfirmGameLink::ConfirmGameLink(QWidget *parent) : QDialog(parent), ui(new Ui::ConfirmGameLink)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [this] { this->accept(); });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, [this] { this->reject(); });
}

ConfirmGameLink::~ConfirmGameLink() { delete ui; }

#include "moc_confirmgamelink.cpp"
