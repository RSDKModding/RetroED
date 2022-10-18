#pragma once

#include <QDialog>

namespace Ui
{
class SyncGCDetails;
}

class SyncGCDetails : public QDialog
{
    Q_OBJECT

public:
    explicit SyncGCDetails(int curObjCount = 0, QWidget *parent = nullptr);
    ~SyncGCDetails();

    QSpinBox *oldObjCount = nullptr;
    QSpinBox *newObjCount = nullptr;

private:
    Ui::SyncGCDetails *ui;
};


