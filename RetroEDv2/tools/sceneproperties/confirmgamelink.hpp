#pragma once

#include <QDialog>

namespace Ui
{
class ConfirmGameLink;
}

class ConfirmGameLink : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmGameLink(QWidget *parent = nullptr);
    ~ConfirmGameLink();

private:
    Ui::ConfirmGameLink *ui;
};


