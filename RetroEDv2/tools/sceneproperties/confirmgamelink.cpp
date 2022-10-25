#include "includes.hpp"
#include "ui_confirmgamelink.h"

#include "confirmgamelink.hpp"

ConfirmGameLink::ConfirmGameLink(QWidget *parent) : QDialog(parent), ui(new Ui::ConfirmGameLink)
{
    ui->setupUi(this);

    // remove question mark from the title bar & disable resizing
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(QSize(width(), height()));

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [this] { this->accept(); });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, [this] { this->reject(); });
}

ConfirmGameLink::~ConfirmGameLink() { delete ui; }

#include "moc_confirmgamelink.cpp"
