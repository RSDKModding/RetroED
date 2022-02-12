#include "includes.hpp"
#include "ui_scenetileproperties.h"

SceneTileProperties::SceneTileProperties(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneTileProperties)
{
    ui->setupUi(this);
}

SceneTileProperties::~SceneTileProperties() { delete ui; }

void SceneTileProperties::setupUI(RSDKv4::TileConfig::CollisionMask *cmA,
                                  RSDKv4::TileConfig::CollisionMask *cmB, ushort tid, QImage tileImgRef)
{
    unsetUI();

    if (!cmA || !cmB)
        return;
    cmask[0] = cmA;
    cmask[1] = cmB;

    tileImg = tileImgRef;

    collisionLyr = 0;

    ui->colPlaneA->setChecked(true);

    ui->maskDir->setCurrentIndex(cmask[collisionLyr]->direction);

    ui->behaviour->setValue(cmask[collisionLyr]->flags);
    ui->floorAngle->setValue(cmask[collisionLyr]->floorAngle);
    ui->roofAngle->setValue(cmask[collisionLyr]->roofAngle);
    ui->lWallAngle->setValue(cmask[collisionLyr]->lWallAngle);
    ui->rWallAngle->setValue(cmask[collisionLyr]->rWallAngle);

    ui->tID->setText("Tile ID: " + QString::number(tid));

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

        ui->maskDir->blockSignals(false);
        ui->behaviour->blockSignals(false);
        ui->floorAngle->blockSignals(false);
        ui->roofAngle->blockSignals(false);
        ui->lWallAngle->blockSignals(false);
        ui->rWallAngle->blockSignals(false);

        ui->colPlaneB->blockSignals(false);
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
        ui->behaviour->blockSignals(false);
        ui->floorAngle->blockSignals(false);
        ui->roofAngle->blockSignals(false);
        ui->lWallAngle->blockSignals(false);
        ui->rWallAngle->blockSignals(false);

        ui->colPlaneA->blockSignals(false);
    });

    connect(ui->maskDir, &QComboBox::currentIndexChanged,
            [this](int i) { cmask[collisionLyr]->direction = i != 0; });

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

    connect(ui->editCollision, &QPushButton::clicked, [&] {
        TileCollisionWidget *edit = new TileCollisionWidget();
        edit->cmask               = cmask[collisionLyr];
        edit->tileImg             = tileImg;
        edit->setWindowTitle("Edit Collision");
        edit->exec();
    });

    auto calcFloorAngle = [](RSDKv4::TileConfig::CollisionMask *mask) {
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

    auto calcRoofAngle = [](RSDKv4::TileConfig::CollisionMask *mask) {
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
        RSDKv4::TileConfig::CollisionMask rotMask = *cmask[collisionLyr];

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
        RSDKv4::TileConfig::CollisionMask rotMask = *cmask[collisionLyr];

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
void SceneTileProperties::unsetUI()
{

    if (!cmask[0] || !cmask[1]) // already unset
        return;

    ui->tID->setText("Tile ID: ");

    disconnect(ui->maskDir, nullptr, nullptr, nullptr);
    disconnect(ui->floorAngle, nullptr, nullptr, nullptr);
    disconnect(ui->roofAngle, nullptr, nullptr, nullptr);
    disconnect(ui->lWallAngle, nullptr, nullptr, nullptr);
    disconnect(ui->rWallAngle, nullptr, nullptr, nullptr);
    disconnect(ui->behaviour, nullptr, nullptr, nullptr);

    disconnect(ui->editCollision, nullptr, nullptr, nullptr);
    disconnect(ui->colPlaneA, nullptr, nullptr, nullptr);
    disconnect(ui->colPlaneB, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleF, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleC, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleL, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleR, nullptr, nullptr, nullptr);

    cmask[0] = nullptr;
    cmask[1] = nullptr;
}

#include "moc_scenetileproperties.cpp"

TileCollisionWidget::TileCollisionWidget(QWidget *parent) : QDialog(parent) { setMouseTracking(true); }

void TileCollisionWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QRectF rect(0, 0, (qreal)width() / 16, (qreal)height() / 16);
    p.drawImage(QRect(0, 0, width(), height()), tileImg);

    if (!cmask->direction) {
        for (byte y = 0; y < 16; ++y) {
            for (byte x = 0; x < 16; ++x) {
                if (cmask->collision[x].height <= y) {
                    QPen pen(qApp->palette().base(), 2);
                    p.setPen(pen);
                    p.setBrush(QColor(0x00FF00));

                    if (!cmask->collision[x].solid)
                        p.setBrush(p.brush().color().darker(255));

                    p.drawRect(rect.translated(x * (qreal)width() / 16, y * (qreal)height() / 16));
                }
            }
        }
    }
    else {
        for (int y = 15; y > -1; --y) {
            for (int x = 15; x > -1; --x) {
                if (cmask->collision[x].height >= y) {

                    QPen pen(qApp->palette().base(), 2);
                    p.setPen(pen);
                    p.setBrush(QColor(0x00FF00));

                    if (!cmask->collision[x].solid)
                        p.setBrush(p.brush().color().darker(255));

                    p.drawRect(rect.translated(x * (qreal)width() / 16, y * (qreal)height() / 16));
                }
            }
        }
    }
}

void TileCollisionWidget::leaveEvent(QEvent *)
{
    highlight = -1;
    update();
}

void TileCollisionWidget::mousePressEvent(QMouseEvent *event)
{
    short x = floor((float)(event->x() / ((qreal)width() / 16)));
    if (x > 15)
        x = 15;

    if (x < 0)
        x = 0;

    selection = highlight;
    pressedL  = (event->button() & Qt::LeftButton) == Qt::LeftButton;
    pressedR  = (event->button() & Qt::RightButton) == Qt::RightButton;

    if (pressedR)
        enabling = !cmask->collision[x].solid;
}

void TileCollisionWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressedL)
        pressedL = !((event->button() & Qt::LeftButton) == Qt::LeftButton);
    if (pressedR)
        pressedR = !((event->button() & Qt::RightButton) == Qt::RightButton);
}

void TileCollisionWidget::mouseMoveEvent(QMouseEvent *event)
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

    if (pressedR)
        cmask->collision[x].solid = enabling;

    if (pressedL)
        cmask->collision[x].height = y;

    update();
}
