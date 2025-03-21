#include "includes.hpp"
#include "ui_scenetileproperties.h"
#include "scenetileproperties.hpp"
#include "chunkcollisioneditor.hpp"
#include "chunkcollisioneditorv1.hpp"

#include <RSDKv1/tileconfigv1.hpp>
#include <RSDKv5/tileconfigv5.hpp>
#include "tools/sceneviewer.hpp"

SceneTileProperties::SceneTileProperties(QWidget *parent)
    : QWidget(parent), ui(new Ui::SceneTileProperties)
{
    ui->setupUi(this);

    replaceTile  = ui->replaceTile;
    edit.tileImg = QImage(16,16, QImage::Format_Indexed8);
    ui->tileFrame->layout()->addWidget(&edit);
}

SceneTileProperties::~SceneTileProperties() { delete ui; }

void SceneTileProperties::setupUI(ushort tid, QList<QImage> &tiles, SceneViewer *viewer, byte engineVer)
{
    unsetUI();

    if (!viewer)
        return;

    tileImg = tiles[tid];
    selectedDrawTile = tid;
    collisionLyr = 0;

    gameType = engineVer;

    if (gameType == ENGINE_v1){
        setColMask(&viewer->tileconfigv1.collisionPaths[0][0], &viewer->tileconfigv1.collisionPaths[1][0]);
        edit.cmaskv1 = cmaskv1[collisionLyr];
        ui->AngleFrame->setDisabled(true);
    }
    else {
        setColMask(&viewer->tileconfig.collisionPaths[0][0], &viewer->tileconfig.collisionPaths[1][0]);
        edit.cmask   = cmask[collisionLyr];
        ui->RSonicFrame->setDisabled(true);
    }
    edit.paintVer            = gameType;
    edit.tileImg             = tileImg;
    edit.update();

    ui->colPlaneA->setChecked(true);

    ui->tID->setText("Tile ID: " + QString::number(tid));

    ui->tileList->blockSignals(true);
    ui->tileList->clear();
    for (int t = 0; t < tiles.count(); ++t) {
        auto *item = new QListWidgetItem(QString::number(t), ui->tileList);
        item->setIcon(QPixmap::fromImage(tiles[t]));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    ui->tileList->blockSignals(false);

    connect(ui->colPlaneA, &QRadioButton::toggled, [this] {
        collisionLyr = 0;

        ui->colPlaneB->blockSignals(true);

        ui->AngleFrame->blockSignals(true);

        ui->colPlaneB->setChecked(false);
        if (gameType == ENGINE_v1){
            ui->colMode->setCurrentIndex(cmaskv1[collisionLyr]->collisionMode);
            edit.cmaskv1             = cmaskv1[collisionLyr];
        } else {
            ui->maskDir->setCurrentIndex(cmask[collisionLyr]->direction);
            ui->behaviour->setValue(cmask[collisionLyr]->flags);
            ui->floorAngle->setValue(cmask[collisionLyr]->floorAngle);
            ui->roofAngle->setValue(cmask[collisionLyr]->roofAngle);
            ui->lWallAngle->setValue(cmask[collisionLyr]->lWallAngle);
            ui->rWallAngle->setValue(cmask[collisionLyr]->rWallAngle);
            edit.cmask               = cmask[collisionLyr];
        }

        ui->AngleFrame->blockSignals(false);

        ui->colPlaneB->blockSignals(false);
        edit.update();
    });

    connect(ui->colPlaneB, &QRadioButton::toggled, [this] {
        collisionLyr = 1;

        ui->colPlaneA->blockSignals(true);

        ui->AngleFrame->blockSignals(true);

        ui->colPlaneA->setChecked(false);
        if (gameType == ENGINE_v1){
            ui->colMode->setCurrentIndex(cmaskv1[collisionLyr]->collisionMode);
            edit.cmaskv1             = cmaskv1[collisionLyr];
        } else {
            ui->maskDir->setCurrentIndex(cmask[collisionLyr]->direction);
            ui->behaviour->setValue(cmask[collisionLyr]->flags);
            ui->floorAngle->setValue(cmask[collisionLyr]->floorAngle);
            ui->roofAngle->setValue(cmask[collisionLyr]->roofAngle);
            ui->lWallAngle->setValue(cmask[collisionLyr]->lWallAngle);
            ui->rWallAngle->setValue(cmask[collisionLyr]->rWallAngle);
            edit.cmask               = cmask[collisionLyr];
        }

        ui->colPlaneA->blockSignals(false);
        ui->AngleFrame->blockSignals(false);
        edit.update();
    });

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

    // v1
    connect(ui->colMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int i) { cmaskv1[collisionLyr]->collisionMode = i; edit.update();});
    connect(ui->maskIndex, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int i) { edit.maskIndex = i; edit.update();});


    connect(ui->editChunkCol, &QPushButton::clicked, [this, viewer] {
        ui->tileFrame->setDisabled(true);
        if (viewer->gameType != ENGINE_v1){
            ChunkCollisionEditor *cColEditor = new ChunkCollisionEditor(&viewer->chunkset, viewer->selectedChunk,
                                                   viewer->tiles, viewer);
            cColEditor->setWindowTitle("Edit Chunk Collision");
            cColEditor->show();
            connect(cColEditor, &QDialog::finished, [this] {
                emit updateChunkColMap();
                ui->tileFrame->setDisabled(false);
            });
        } else{
            ChunkCollisionEditorv1 *cColEditor = new ChunkCollisionEditorv1(&viewer->chunkset, viewer->selectedChunk,
                                                   viewer->tiles, viewer);
            cColEditor->setWindowTitle("Edit Chunk Collision");
            cColEditor->show();

            connect(cColEditor, &QDialog::finished, [this] {
                emit updateChunkColMap();
                ui->tileFrame->setDisabled(false);
            });
        }
    });

    connect(ui->tileList, &QListWidget::currentRowChanged,[this, tiles, viewer](int selDrawTile){
        selectedDrawTile = selDrawTile;
        ui->tID->setText("Tile ID: " + QString::number(selectedDrawTile));
        if (viewer->gameType != ENGINE_v1){
            setColMask(&viewer->tileconfig.collisionPaths[0][selectedDrawTile],&viewer->tileconfig.collisionPaths[1][selectedDrawTile]);
            edit.cmask               = cmask[collisionLyr];
        } else {
            setColMask(&viewer->tileconfigv1.collisionPaths[0][selectedDrawTile],&viewer->tileconfigv1.collisionPaths[1][selectedDrawTile]);
            edit.cmaskv1 = cmaskv1[collisionLyr];
        }
        edit.tileImg = tiles[selectedDrawTile];
        edit.update();
    });

    connect(&edit, &TileCollisionWidget::UpdateCol, [&]{
        if (gameType != ENGINE_v1)
            emit updateChunkColTile(cmask[collisionLyr], selectedDrawTile, collisionLyr);
        else
            emit updateChunkColTilev1(cmaskv1[collisionLyr], selectedDrawTile, collisionLyr);
    });

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
void SceneTileProperties::checkChunk(bool valid){ ui->editChunkCol->setDisabled(!valid); }

void SceneTileProperties::calcv1Angles(RSDKv5::TileConfig::CollisionMask *outputMask, RSDKv1::TileConfig::CollisionMask *inputMask)
{

    auto calcFloorAngle = [](RSDKv1::TileConfig::CollisionMask *mask) {
        byte angle = 0;
        Vector2<float> start(-1, -1), end(-1, -1);

        for (int x = 0; x < 16; ++x) {
            if (mask->collision[0][x].solid) {
                if (start.x == -1) {
                    start.x = x;
                    start.y = mask->collision[0][x].height;
                }

                end.x = x;
                end.y = mask->collision[0][x].height;
            }
        }

        float angleF = atan2((float)(end.y - start.y), (end.x - start.x));
        angle        = (int)(angleF * 40.764331) & 0xFC;

        return angle;
    };

    auto calcRoofAngle = [](RSDKv1::TileConfig::CollisionMask *mask) {
        byte angle = 0;
        Vector2<float> start(-1, -1), end(-1, -1);

        for (int x = 0; x < 16; ++x) {
            if (mask->collision[0][x].solid) {
                if (start.x == -1) {
                    start.x = x;
                    start.y = mask->collision[0][x].height;
                }

                end.x = x;
                end.y = mask->collision[0][x].height;
            }
        }

        float angleF = atan2((float)(start.y - end.y), (start.x - end.x));
        angle        = (int)(angleF * 40.764331) & 0xFC;

        return angle;
    };

    auto calclWallAngle = [calcFloorAngle, outputMask](RSDKv1::TileConfig::CollisionMask *mask) {
        RSDKv1::TileConfig::CollisionMask rotMask = *mask;
        outputMask->lWallAngle = calcFloorAngle(mask);

            // LWall rotations
        for (int c = 0; c < 16; ++c) {
            int h                      = 0;
            rotMask.collision[0][c].solid = true;
            while (true) {
                if (h == 16) {
                    rotMask.collision[0][c].solid = false;
                    break;
                }

                byte m = outputMask->collision[h].height;
                if (outputMask->collision[h].solid && c >= m) {
                    rotMask.collision[0][c].height = h;
                    break;
                }
                else {
                    ++h;
                    if (h <= -1)
                        break;
                }
            }
        }

        int startX = -1, endX = -1;
        for (int x = 0; x < 16; ++x) {
            if (rotMask.collision[0][x].solid) {
                if (startX == -1)
                    startX = rotMask.collision[0][x].height;

                endX = rotMask.collision[0][x].height;
            }
        }

        if (startX == endX)
            outputMask->lWallAngle = 0xC0;
    };

    auto calcrWallAngle = [calcFloorAngle, outputMask](RSDKv1::TileConfig::CollisionMask *mask) {
        RSDKv1::TileConfig::CollisionMask rotMask = *mask;
        outputMask->rWallAngle = calcFloorAngle(mask);

        for (int c = 0; c < 16; ++c) {
            int h                      = 15;
            rotMask.collision[0][c].solid = true;
            while (true) {
                if (h == -1) {
                    rotMask.collision[0][c].solid = false;
                    break;
                }

                byte m = outputMask->collision[h].height;
                if (outputMask->collision[h].solid && c >= m) {
                    rotMask.collision[0][c].height = h;
                    break;
                }
                else {
                    --h;
                    if (h >= 16)
                        break;
                }
            }
        }

        int startX = -1, endX = -1;
        for (int x = 0; x < 16; ++x) {
            if (rotMask.collision[0][x].solid) {
                if (startX == -1)
                    startX = rotMask.collision[0][x].height;

                endX = rotMask.collision[0][x].height;
            }
        }

        if (startX == endX)
            outputMask->rWallAngle = 0x40;
    };

    outputMask->floorAngle = calcFloorAngle(inputMask);
    calclWallAngle(inputMask);
    outputMask->roofAngle = calcRoofAngle(inputMask);
    calcrWallAngle(inputMask);
}

void SceneTileProperties::unsetUI()
{

    if (!cmask[0] || !cmask[1]) // already unset
        return;

    ui->tID->setText("Tile ID: ");

    collisionLyr = 0;
    ui->colPlaneA->setChecked(true);
    ui->colPlaneB->setChecked(false);

    disconnect(ui->maskDir, nullptr, nullptr, nullptr);
    disconnect(ui->floorAngle, nullptr, nullptr, nullptr);
    disconnect(ui->roofAngle, nullptr, nullptr, nullptr);
    disconnect(ui->lWallAngle, nullptr, nullptr, nullptr);
    disconnect(ui->rWallAngle, nullptr, nullptr, nullptr);
    disconnect(ui->behaviour, nullptr, nullptr, nullptr);
    disconnect(ui->colMode, nullptr, nullptr, nullptr);

    disconnect(ui->editChunkCol, nullptr, nullptr, nullptr);
    disconnect(ui->colPlaneA, nullptr, nullptr, nullptr);
    disconnect(ui->colPlaneB, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleF, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleC, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleL, nullptr, nullptr, nullptr);
    disconnect(ui->calcAngleR, nullptr, nullptr, nullptr);

    cmask[0] = nullptr;
    cmask[1] = nullptr;
    cmaskv1[0] = nullptr;
    cmaskv1[1] = nullptr;

    edit.cmask = cmask[0];
    edit.tileImg = QImage(16,16, QImage::Format_Indexed8);
    ui->tileFrame->layout()->removeWidget(&edit);
}

#include "moc_scenetileproperties.cpp"

void SceneTileProperties::setColMask(RSDKv5::TileConfig::CollisionMask *cmA, RSDKv5::TileConfig::CollisionMask *cmB){
    cmask[0] = cmA;
    cmask[1] = cmB;
    ui->maskDir->setCurrentIndex(cmask[collisionLyr]->direction);

    ui->behaviour->setValue(cmask[collisionLyr]->flags);
    ui->floorAngle->setValue(cmask[collisionLyr]->floorAngle);
    ui->roofAngle->setValue(cmask[collisionLyr]->roofAngle);
    ui->lWallAngle->setValue(cmask[collisionLyr]->lWallAngle);
    ui->rWallAngle->setValue(cmask[collisionLyr]->rWallAngle);
}
void SceneTileProperties::setColMask(RSDKv1::TileConfig::CollisionMask *cmA, RSDKv1::TileConfig::CollisionMask *cmB){
    cmaskv1[0] = cmA;
    cmaskv1[1] = cmB;
    ui->colMode->setCurrentIndex(cmaskv1[collisionLyr]->collisionMode);
}

void SceneTileProperties::tileSelected(ushort tile){ ui->tileList->setCurrentRow(tile); }

TileCollisionWidget::TileCollisionWidget(QWidget *parent) : QWidget(parent) { setMouseTracking(true); }

void TileCollisionWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QRectF rect(0, 0, (qreal)width() / 16, (qreal)height() / 16);
    p.drawImage(QRect(0, 0, width(), height()), tileImg);

    QPen pen(qApp->palette().base(), 2);
    p.setPen(pen);
    p.setOpacity(0.5);

    if (!parentWidget()->isEnabled()){
        p.setBrush(QColor(Qt::black));
        p.drawRect(QRect(0, 0, width(), height()));
    }

    for (byte y = 0; y < 16; ++y) {
        for (byte x = 0; x < 16; ++x) {
            if (paintVer != ENGINE_v1){
                int dirY = !cmask->direction ? y : abs(y - 15);
                if (!cmask->direction ? cmask->collision[x].height <= dirY : cmask->collision[x].height >= dirY) {

                    p.setBrush(QColor(0x00FF00));
                    if (!cmask->collision[x].solid)
                        p.setBrush(p.brush().color().darker(255));

                    p.drawRect(rect.translated(x * (qreal)width() / 16, dirY * (qreal)height() / 16));
                }
            } else {
                // todo: find out how RSonic detects the correct mask index to use
                int dirX = (maskIndex & 1) == 0 ? x : abs(x - 15);
                int dirY = (maskIndex & 2) == 0 ? y : abs(y - 15);
                if ((maskIndex & 2) == 0 ? cmaskv1->collision[maskIndex][dirX].height <= dirY : cmaskv1->collision[maskIndex][dirX].height >= dirY) {


                    p.setBrush(QColor(0x00FF00));
                    if (!cmaskv1->collision[maskIndex][dirX].solid)
                        p.setBrush(p.brush().color().darker(255));

                    p.drawRect(rect.translated(dirX * (qreal)width() / 16, dirY * (qreal)height() / 16));
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
        enabling = paintVer == ENGINE_v1 ? !cmaskv1->collision[maskIndex][x].solid : !cmask->collision[x].solid;
}

void TileCollisionWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressedL)
        pressedL = !((event->button() & Qt::LeftButton) == Qt::LeftButton);
    if (pressedR)
        pressedR = !((event->button() & Qt::RightButton) == Qt::RightButton);
    emit UpdateCol();
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

    if (paintVer != ENGINE_v1){
        if (pressedR)
            cmask->collision[x].solid = enabling;

        if (pressedL)
            cmask->collision[x].height = y;
    } else {
        if (pressedR)
            cmaskv1->collision[maskIndex][x].solid = enabling;

        if (pressedL)
            cmaskv1->collision[maskIndex][x].height = y;
    }

    update();
}
