#pragma once

#include <QDialog>

namespace Ui
{
class TileReplaceOptions;
}

class TileReplaceOptions : public QDialog
{
    Q_OBJECT

public:
    explicit TileReplaceOptions(QWidget *parent = nullptr);
    ~TileReplaceOptions();

    QSpinBox *dstTile = nullptr;
    QSpinBox *srcTile = nullptr;

    QCheckBox *replaceTileIndex   = nullptr;
    QCheckBox *replaceVisualPlane = nullptr;

    QCheckBox *replaceFlipX = nullptr;
    QCheckBox *replaceFlipY = nullptr;

    QCheckBox *replaceSolidATop = nullptr;
    QCheckBox *replaceSolidALRB = nullptr;

    QCheckBox *replaceSolidBTop = nullptr;
    QCheckBox *replaceSolidBLRB = nullptr;

    QCheckBox *replaceCollision = nullptr;
    QCheckBox *replaceGraphics  = nullptr;

    QCheckBox *hasVisualPlane = nullptr;

    QCheckBox *hasFlipX = nullptr;
    QCheckBox *hasFlipY = nullptr;

    QCheckBox *hasSolidATop = nullptr;
    QCheckBox *hasSolidALRB = nullptr;

    QCheckBox *hasSolidBTop = nullptr;
    QCheckBox *hasSolidBLRB = nullptr;

private:
    Ui::TileReplaceOptions *ui;
};


