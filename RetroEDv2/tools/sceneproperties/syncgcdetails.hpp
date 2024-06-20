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
    explicit SyncGCDetails(int scnObjCount = 0, QWidget *parent = nullptr);
    ~SyncGCDetails();

    QSpinBox *oldObjCount = nullptr;

private:
    Ui::SyncGCDetails *ui;
};


