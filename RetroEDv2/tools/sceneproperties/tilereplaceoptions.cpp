#include "includes.hpp"
#include "ui_tilereplaceoptions.h"
#include "tilereplaceoptions.hpp"

TileReplaceOptions::TileReplaceOptions(QWidget *parent)
    : QDialog(parent), ui(new Ui::TileReplaceOptions)
{
    ui->setupUi(this);

    dstTile = ui->dstTile;
    srcTile = ui->srcTile;

    replaceTileIndex   = ui->replaceTileIndex;
    replaceVisualPlane = ui->replaceVisualPlane;

    replaceFlipX = ui->replaceFlipX;
    replaceFlipY = ui->replaceFlipY;

    replaceSolidATop = ui->replaceSolidTopA;
    replaceSolidALRB = ui->replaceSolidLRBA;

    replaceSolidBTop = ui->replaceSolidTopB;
    replaceSolidBLRB = ui->replaceSolidLRBB;

    replaceCollision = ui->replaceCollision;
    replaceGraphics  = ui->replaceGraphics;

    hasVisualPlane = ui->hasVisualPlane;

    hasFlipX = ui->hasFlipX;
    hasFlipY = ui->hasFlipY;

    hasSolidATop = ui->hasSolidATop;
    hasSolidALRB = ui->hasSolidALRB;

    hasSolidBTop = ui->hasSolidBTop;
    hasSolidBLRB = ui->hasSolidBLRB;

    connect(ui->replaceVisualPlane, &QCheckBox::toggled,
            [this](bool c) { ui->hasVisualPlane->setDisabled(!c); });

    connect(ui->replaceFlipX, &QCheckBox::toggled, [this](bool c) { ui->hasFlipX->setDisabled(!c); });
    connect(ui->replaceFlipY, &QCheckBox::toggled, [this](bool c) { ui->hasFlipY->setDisabled(!c); });

    connect(ui->replaceSolidTopA, &QCheckBox::toggled,
            [this](bool c) { ui->hasSolidATop->setDisabled(!c); });
    connect(ui->replaceSolidLRBA, &QCheckBox::toggled,
            [this](bool c) { ui->hasSolidALRB->setDisabled(!c); });

    connect(ui->replaceSolidTopB, &QCheckBox::toggled,
            [this](bool c) { ui->hasSolidBTop->setDisabled(!c); });
    connect(ui->replaceSolidLRBB, &QCheckBox::toggled,
            [this](bool c) { ui->hasSolidBLRB->setDisabled(!c); });

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [this] { this->accept(); });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, [this] { this->reject(); });
}

TileReplaceOptions::~TileReplaceOptions() { delete ui; }

#include "moc_tilereplaceoptions.cpp"
