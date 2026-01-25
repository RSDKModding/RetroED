#pragma once

#include <QDialog>

namespace Ui
{
    class CopyPlane;
}

class CopyPlane : public QDialog
{
    Q_OBJECT
public:
    explicit CopyPlane(QWidget *parent = nullptr);
    ~CopyPlane();

    void setupUI();
    bool copyChunkPlane = false;
    bool copyTilePlanes = false;
    bool applyAllTiles  = false;
    bool applyAllChks   = false;

private:
    Ui::CopyPlane *ui;
};
