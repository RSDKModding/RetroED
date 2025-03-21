#include "includes.hpp"
#include "ui_scenetilepropertiesv5.h"
#include "scenetilepropertiesv5.hpp"

#include <RSDKv5/tileconfigv5.hpp>

SceneTilePropertiesv5::SceneTilePropertiesv5(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneTilePropertiesv5)
{
    ui->setupUi(this);

    replaceTile  = ui->replaceTile;
    edit.tileImg = QImage(16, 16, QImage::Format_Indexed8);
    ui->frame->layout()->addWidget(&edit);
}

SceneTilePropertiesv5::~SceneTilePropertiesv5() { delete ui; }

void SceneTilePropertiesv5::setupUI(RSDKv5::TileConfig::CollisionMask *cmA,
                                    RSDKv5::TileConfig::CollisionMask *cmB, ushort *tile,
                                    QImage tileImg)
{
    unsetUI();

    if (!cmA || !cmB || !tile)
        return;

    cmask[0] = cmA;
    cmask[1] = cmB;
    tileID = *tile & 0x3FF;

    curTile = tile;
    edit.cmask                 = cmask[collisionLyr];
    edit.tileImg               = tileImg;
    edit.update();

    ui->colPlaneA->setChecked(true);

    ui->flipX->setChecked(Utils::getBit(*tile, 10));
    ui->flipY->setChecked(Utils::getBit(*tile, 11));
    ui->solidTopA->setChecked(Utils::getBit(*tile, 12));
    ui->solidLRBA->setChecked(Utils::getBit(*tile, 13));
    ui->solidTopB->setChecked(Utils::getBit(*tile, 14));
    ui->solidLRBB->setChecked(Utils::getBit(*tile, 15));

    ui->maskDir->setCurrentIndex(cmask[collisionLyr]->direction);
    ui->behaviour->setValue(cmask[collisionLyr]->flags);
    ui->floorAngle->setValue(cmask[collisionLyr]->floorAngle);
    ui->roofAngle->setValue(cmask[collisionLyr]->roofAngle);
    ui->lWallAngle->setValue(cmask[collisionLyr]->lWallAngle);
    ui->rWallAngle->setValue(cmask[collisionLyr]->rWallAngle);

    ui->tID->setText("Tile ID: " + QString::number(tileID));

    connect(ui->colPlaneA, &QRadioButton::toggled, [this] {
        collisionLyr = 0;

        ui->colPlaneB->blockSignals(true);

        ui->maskDir->blockSignals(true);
        ui->behaviour->blockSignals(true);
        ui->floorAngle->blockSignals(true);
        ui->roofAngle->blockSignals(true);
        ui->lWallAngle->blockSignals(true);
        ui->rWallAngle->blockSignals(true);

        ui->colPlaneB->setChecked(false);
        ui->maskDir->setCurrentIndex(cmask[collisionLyr]->direction);
        ui->behaviour->setValue(cmask[collisionLyr]->flags);
        ui->floorAngle->setValue(cmask[collisionLyr]->floorAngle);
        ui->roofAngle->setValue(cmask[collisionLyr]->roofAngle);
        ui->lWallAngle->setValue(cmask[collisionLyr]->lWallAngle);
        ui->rWallAngle->setValue(cmask[collisionLyr]->rWallAngle);

        ui->colPlaneB->blockSignals(false);

        ui->maskDir->blockSignals(false);
        ui->behaviour->blockSignals(false);
        ui->floorAngle->blockSignals(false);
        ui->roofAngle->blockSignals(false);
        ui->lWallAngle->blockSignals(false);
        ui->rWallAngle->blockSignals(false);
        edit.cmask = cmask[collisionLyr];
        edit.update();
    });

    connect(ui->colPlaneB, &QRadioButton::toggled, [this] {
        collisionLyr = 1;

        ui->colPlaneA->blockSignals(true);

        ui->maskDir->blockSignals(true);
        ui->behaviour->blockSignals(true);
        ui->floorAngle->blockSignals(true);
        ui->roofAngle->blockSignals(true);
        ui->lWallAngle->blockSignals(true);
        ui->rWallAngle->blockSignals(true);

        ui->colPlaneA->setChecked(false);
        ui->maskDir->setCurrentIndex(cmask[collisionLyr]->direction);
        ui->behaviour->setValue(cmask[collisionLyr]->flags);
        ui->floorAngle->setValue(cmask[collisionLyr]->floorAngle);
        ui->roofAngle->setValue(cmask[collisionLyr]->roofAngle);
        ui->lWallAngle->setValue(cmask[collisionLyr]->lWallAngle);
        ui->rWallAngle->setValue(cmask[collisionLyr]->rWallAngle);

        ui->colPlaneA->blockSignals(false);

        ui->maskDir->blockSignals(false);
        ui->behaviour->blockSignals(false);
        ui->floorAngle->blockSignals(false);
        ui->roofAngle->blockSignals(false);
        ui->lWallAngle->blockSignals(false);
        ui->rWallAngle->blockSignals(false);

        edit.cmask               = cmask[collisionLyr];
        edit.update();
    });

    connect(ui->flipX, &QCheckBox::toggled, [tile, this](bool c) { Utils::setBit(*tile, c, 10); updateTileFlags(c, 10); });
    connect(ui->flipY, &QCheckBox::toggled, [tile, this](bool c) { Utils::setBit(*tile, c, 11); updateTileFlags(c, 11); });
    connect(ui->solidTopA, &QCheckBox::toggled, [tile, this](bool c) { Utils::setBit(*tile, c, 12); updateTileFlags(c, 12); });
    connect(ui->solidLRBA, &QCheckBox::toggled, [tile, this](bool c) { Utils::setBit(*tile, c, 13); updateTileFlags(c, 13); });
    connect(ui->solidTopB, &QCheckBox::toggled, [tile, this](bool c) { Utils::setBit(*tile, c, 14); updateTileFlags(c, 14); });
    connect(ui->solidLRBB, &QCheckBox::toggled, [tile, this](bool c) { Utils::setBit(*tile, c, 15); updateTileFlags(c, 15); });

    connect(ui->maskDir, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int i) { cmask[collisionLyr]->direction = i != 0; edit.update();});

    connect(ui->behaviour, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { cmask[collisionLyr]->flags = (byte)v; });

    connect(ui->floorAngle, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { cmask[collisionLyr]->floorAngle = (byte)v; });

    connect(ui->roofAngle, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { cmask[collisionLyr]->roofAngle = (byte)v; });

    connect(ui->lWallAngle, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { cmask[collisionLyr]->lWallAngle = (byte)v; });

    connect(ui->rWallAngle, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { cmask[collisionLyr]->rWallAngle = (byte)v; });

    connect(&edit, &TileCollisionWidgetv5::UpdateCol, [&]{ emit updateTileColMap(cmask[collisionLyr], tileID, collisionLyr); });

    auto calcFloorAngle = [](RSDKv5::TileConfig::CollisionMask *mask) {
        byte angle = 0;
        Vector2<float> start(-1, -1), end(-1, -1);

        if (!mask->direction) // Ceiling Tile
        {
            for (int x = 0; x < 16; ++x) {
                if (mask->collision[x].solid) {
                    if (start.x == -1) {
                        start.x = x;
                        start.y = mask->collision[x].height;
                    }

                    end.x = x;
                    end.y = mask->collision[x].height;
                }
            }

            float angleF = atan2((float)(end.y - start.y), (end.x - start.x));
            angle        = (int)(angleF * 40.764331) & 0xFC;
        }
        else {
            angle = 0x00;
        }

        return angle;
    };

    auto calcRoofAngle = [](RSDKv5::TileConfig::CollisionMask *mask) {
        byte angle = 0;
        Vector2<float> start(-1, -1), end(-1, -1);

        if (mask->direction) // Ceiling Tile
        {
            for (int x = 0; x < 16; ++x) {
                if (mask->collision[x].solid) {
                    if (start.x == -1) {
                        start.x = x;
                        start.y = mask->collision[x].height;
                    }

                    end.x = x;
                    end.y = mask->collision[x].height;
                }
            }

            float angleF = atan2((float)(start.y - end.y), (start.x - end.x));
            angle        = (int)(angleF * 40.764331) & 0xFC;
        }
        else {
            angle = 0x80;
        }

        return angle;
    };

    connect(ui->calcAngleF, &QPushButton::clicked, [this, calcFloorAngle] {
        cmask[collisionLyr]->floorAngle = calcFloorAngle(cmask[collisionLyr]);
        ui->floorAngle->blockSignals(true);
        ui->floorAngle->setValue(cmask[collisionLyr]->floorAngle);
        ui->floorAngle->blockSignals(false);
    });

    connect(ui->calcAngleC, &QPushButton::clicked, [this, calcRoofAngle] {
        cmask[collisionLyr]->roofAngle = calcRoofAngle(cmask[collisionLyr]);
        ui->roofAngle->blockSignals(true);
        ui->roofAngle->setValue(cmask[collisionLyr]->roofAngle);
        ui->roofAngle->blockSignals(false);
    });

    connect(ui->calcAngleL, &QPushButton::clicked, [this, calcFloorAngle, calcRoofAngle] {
        RSDKv5::TileConfig::CollisionMask rotMask = *cmask[collisionLyr];

        if (cmask[collisionLyr]->direction) { // Ceiling Tile
            cmask[collisionLyr]->lWallAngle = calcRoofAngle(cmask[collisionLyr]);

                   // LWall rotations
            for (int c = 0; c < 16; ++c) {
                int h                      = 0;
                rotMask.collision[c].solid = true;
                while (true) {
                    if (h == 16) {
                        rotMask.collision[c].solid = false;
                        break;
                    }

                    byte m = cmask[collisionLyr]->collision[h].height;
                    if (cmask[collisionLyr]->collision[h].solid && c <= m) {
                        rotMask.collision[c].height = h;
                        break;
                    }
                    else {
                        ++h;
                        if (h <= -1)
                            break;
                    }
                }
            }
        }
        else { // Regular Tile
            cmask[collisionLyr]->lWallAngle = calcFloorAngle(cmask[collisionLyr]);

                   // LWall rotations
            for (int c = 0; c < 16; ++c) {
                int h                      = 0;
                rotMask.collision[c].solid = true;
                while (true) {
                    if (h == 16) {
                        rotMask.collision[c].solid = false;
                        break;
                    }

                    byte m = cmask[collisionLyr]->collision[h].height;
                    if (cmask[collisionLyr]->collision[h].solid && c >= m) {
                        rotMask.collision[c].height = h;
                        break;
                    }
                    else {
                        ++h;
                        if (h <= -1)
                            break;
                    }
                }
            }
        }

        int startX = -1, endX = -1;
        for (int x = 0; x < 16; ++x) {
            if (rotMask.collision[x].solid) {
                if (startX == -1)
                    startX = rotMask.collision[x].height;

                endX = rotMask.collision[x].height;
            }
        }

        if (startX == endX)
            cmask[collisionLyr]->lWallAngle = 0xC0;

        ui->lWallAngle->blockSignals(true);
        ui->lWallAngle->setValue(cmask[collisionLyr]->lWallAngle);
        ui->lWallAngle->blockSignals(false);
    });

    connect(ui->calcAngleR, &QPushButton::clicked, [this, calcFloorAngle, calcRoofAngle] {
        RSDKv5::TileConfig::CollisionMask rotMask = *cmask[collisionLyr];

        if (cmask[collisionLyr]->direction) { // Ceiling Tile
            cmask[collisionLyr]->rWallAngle = calcRoofAngle(cmask[collisionLyr]);

                   // RWall rotations
            for (int c = 0; c < 16; ++c) {
                int h                      = 15;
                rotMask.collision[c].solid = true;
                while (true) {
                    if (h == -1) {
                        rotMask.collision[c].solid = false;
                        break;
                    }

                    byte m = cmask[collisionLyr]->collision[h].height;
                    if (cmask[collisionLyr]->collision[h].solid && c <= m) {
                        rotMask.collision[c].height = h;
                        break;
                    }
                    else {
                        --h;
                        if (h >= 16)
                            break;
                    }
                }
            }
        }
        else { // Regular Tile
            cmask[collisionLyr]->rWallAngle = calcFloorAngle(cmask[collisionLyr]);

                   // RWall rotations
            for (int c = 0; c < 16; ++c) {
                int h                      = 15;
                rotMask.collision[c].solid = true;
                while (true) {
                    if (h == -1) {
                        rotMask.collision[c].solid = false;
                        break;
                    }

                    byte m = cmask[collisionLyr]->collision[h].height;
                    if (cmask[collisionLyr]->collision[h].solid && c >= m) {
                        rotMask.collision[c].height = h;
                        break;
                    }
                    else {
                        --h;
                        if (h >= 16)
                            break;
                    }
                }
            }
        }

        int startX = -1, endX = -1;
        for (int x = 0; x < 16; ++x) {
            if (rotMask.collision[x].solid) {
                if (startX == -1)
                    startX = rotMask.collision[x].height;

                endX = rotMask.collision[x].height;
            }
        }

        if (startX == endX)
            cmask[collisionLyr]->rWallAngle = 0x40;

        ui->rWallAngle->blockSignals(true);
        ui->rWallAngle->setValue(cmask[collisionLyr]->rWallAngle);
        ui->rWallAngle->blockSignals(false);
    });
}
void SceneTilePropertiesv5::unsetUI()
{
    if (!cmask[0] || !cmask[1]) // already unset
        return;

    ui->tID->setText("Tile ID: ");

    collisionLyr = 0;

    disconnect(ui->flipX, nullptr, nullptr, nullptr);
    disconnect(ui->flipY, nullptr, nullptr, nullptr);
    disconnect(ui->solidTopA, nullptr, nullptr, nullptr);
    disconnect(ui->solidLRBA, nullptr, nullptr, nullptr);
    disconnect(ui->solidTopB, nullptr, nullptr, nullptr);
    disconnect(ui->solidLRBB, nullptr, nullptr, nullptr);

    disconnect(ui->maskDir, nullptr, nullptr, nullptr);
    disconnect(ui->floorAngle, nullptr, nullptr, nullptr);
    disconnect(ui->roofAngle, nullptr, nullptr, nullptr);
    disconnect(ui->lWallAngle, nullptr, nullptr, nullptr);
    disconnect(ui->rWallAngle, nullptr, nullptr, nullptr);
    disconnect(ui->behaviour, nullptr, nullptr, nullptr);

    disconnect(ui->colPlaneA, nullptr, nullptr, nullptr);
    disconnect(ui->colPlaneB, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleF, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleC, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleL, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleR, nullptr, nullptr, nullptr);

    ui->colPlaneA->setChecked(true);
    ui->colPlaneB->setChecked(false);

    ui->maskDir->setCurrentIndex(0);
    ui->behaviour->setValue(0);
    ui->floorAngle->setValue(0);
    ui->roofAngle->setValue(0);
    ui->lWallAngle->setValue(0);
    ui->rWallAngle->setValue(0);

    ui->flipX->setChecked(false);
    ui->flipY->setChecked(false);
    ui->solidTopA->setChecked(false);
    ui->solidLRBA->setChecked(false);
    ui->solidTopB->setChecked(false);
    ui->solidLRBB->setChecked(false);

    cmask[0] = nullptr;
    cmask[1] = nullptr;

    edit.cmask = cmask[0];
    edit.tileImg = QImage(16,16, QImage::Format_Indexed8);
    ui->frame->layout()->removeWidget(&edit);
}

void SceneTilePropertiesv5::updatePropFlags(bool c, byte pos){
    Utils::setBit(*curTile, c, pos);
    switch (pos){
        case 10:
            ui->flipX->setChecked(c); break;
        case 11:
            ui->flipY->setChecked(c); break;
        case 12:
            ui->solidTopA->setChecked(c); break;
        case 13:
            ui->solidLRBA->setChecked(c); break;
        case 14:
            ui->solidTopB->setChecked(c); break;
        case 15:
            ui->solidLRBB->setChecked(c); break;
    }
    emit updateTileColMap(cmask[collisionLyr], tileID, collisionLyr);
}

#include "moc_scenetilepropertiesv5.cpp"

TileCollisionWidgetv5::TileCollisionWidgetv5(QWidget *parent) : QWidget(parent) { setMouseTracking(true); }

void TileCollisionWidgetv5::paintEvent(QPaintEvent *)
{
    if (!cmask)
        return;

    QPainter p(this);

    QRectF rect(0, 0, (qreal)width() / 16, (qreal)height() / 16);
    p.drawImage(QRect(0, 0, width(), height()), tileImg);

    QPen pen(qApp->palette().base(), 2);
    p.setPen(pen);
    p.setOpacity(0.5);

    for (byte y = 0; y < 16; ++y) {
        for (byte x = 0; x < 16; ++x) {
            int dirY = !cmask->direction ? y : abs(y - 15);
            if (!cmask->direction ? cmask->collision[x].height <= dirY : cmask->collision[x].height >= dirY) {
                p.setBrush(QColor(0x00FF00));
                if (!cmask->collision[x].solid)
                    p.setBrush(p.brush().color().darker(255));

                p.drawRect(rect.translated(x * (qreal)width() / 16, dirY * (qreal)height() / 16));
            }
        }
    }
}

void TileCollisionWidgetv5::leaveEvent(QEvent *)
{
    highlight = -1;
    update();
}

void TileCollisionWidgetv5::mousePressEvent(QMouseEvent *event)
{
    short x = floor((float)(event->x() / ((qreal)width() / 16)));
    if (x > 15)
        x = 15;

    if (x < 0)
        x = 0;

    selection = highlight;
    pressedL  = (event->button() & Qt::LeftButton) == Qt::LeftButton;
    pressedR  = (event->button() & Qt::RightButton) == Qt::RightButton;

    if (pressedR && cmask)
        enabling = !cmask->collision[x].solid;
}

void TileCollisionWidgetv5::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressedL)
        pressedL = !((event->button() & Qt::LeftButton) == Qt::LeftButton);
    if (pressedR)
        pressedR = !((event->button() & Qt::RightButton) == Qt::RightButton);
    emit UpdateCol();
}

void TileCollisionWidgetv5::mouseMoveEvent(QMouseEvent *event)
{
    short x = floor((float)(event->x() / ((qreal)width() / 16)));
    short y = floor((float)(event->y() / ((qreal)height() / 16)));

    if (x > 15)
        x = 15;

    if (y > 15)
        y = 15;

    if (x < 0)
        x = 0;

    if (y < 0)
        y = 0;

    highlight = x % 16 + y * 16;

    if (pressedR && cmask)
        cmask->collision[x].solid = enabling;

    if (pressedL && cmask)
        cmask->collision[x].height = y;

    update();
}
