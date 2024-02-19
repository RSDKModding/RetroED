#include "includes.hpp"
#include "ui_chunkcollisioneditor.h"
#include "chunkcollisioneditor.hpp"
#include <RSDKv5/tileconfigv5.hpp>

ChunkCollisionEditor::ChunkCollisionEditor(FormatHelpers::Chunks *chk, ushort curChunk, QList<QImage> &tiles, SceneViewer *viewer)
                      : ui(new Ui::ChunkCollisionEditor), chunks(chk), tileList(tiles)
{
    ui->setupUi(this);
    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    selectedChunk    = curChunk;
    collisionLyr     = defaultVisualPlane;
    selectedDrawTile = 0;
    ui->planeA->setChecked(true);

    int i = 0;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto &tile = chunks->chunks[curChunk].tiles[y][x];
            storedTileList[i] = tile.tileIndex;
            chunkColMask[0][i] = &viewer->tileconfig.collisionPaths[0][tile.tileIndex];
            chunkColMask[1][i] = &viewer->tileconfig.collisionPaths[1][tile.tileIndex];
            imageTileList[i++] = tiles[tile.tileIndex];
        }
    }

    ui->textTileID->setText("Tile ID: " + QString::number(storedTileList[selectedDrawTile]));
    ui->textChunkID->setText("Chunk ID: " + QString::number(curChunk));

    chunkViewer = new ChunkColViewer(&selectedChunk, chunks, tiles, true);
    chunkViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    collisionViewer = new ChunkColViewer(&selectedChunk, chunks, tiles, false);
    collisionViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    for (int i = 0; i < 0x400; ++i) {
        collisionViewer->cmask[0][i] = &viewer->tileconfig.collisionPaths[0][i];
        collisionViewer->cmask[1][i] = &viewer->tileconfig.collisionPaths[1][i];
    }

    collisionViewer->collisionLyr = 0;
    ui->chunkTileList->layout()->addWidget(chunkViewer);
    ui->chunkCol->layout()->addWidget(collisionViewer);

    colEdit = new ChunkColEdit();
    colEdit->cmask =  chunkColMask[collisionLyr][selectedDrawTile];
    colEdit->tileImg = imageTileList[selectedDrawTile];
    ui->tileCol->layout()->addWidget(colEdit);

    connect(ui->planeA, &QRadioButton::toggled, [this] {
        collisionLyr     = 0;

        ui->planeB->blockSignals(true);
        ui->angMaskDir->blockSignals(true);
        ui->angFlag->blockSignals(true);
        ui->angFloor->blockSignals(true);
        ui->angRoof->blockSignals(true);
        ui->angLWall->blockSignals(true);
        ui->angRWall->blockSignals(true);

        ui->planeB->setChecked(false);

        ui->angMaskDir->setCurrentIndex(chunkColMask[collisionLyr][selectedDrawTile]->direction);
        ui->angFlag->setValue(chunkColMask[collisionLyr][selectedDrawTile]->flags);
        ui->angFloor->setValue(chunkColMask[collisionLyr][selectedDrawTile]->floorAngle);
        ui->angRoof->setValue(chunkColMask[collisionLyr][selectedDrawTile]->roofAngle);
        ui->angLWall->setValue(chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle);
        ui->angRWall->setValue(chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle);

        ui->angMaskDir->blockSignals(false);
        ui->angFlag->blockSignals(false);
        ui->angFloor->blockSignals(false);
        ui->angRoof->blockSignals(false);
        ui->angLWall->blockSignals(false);
        ui->angRWall->blockSignals(false);

        ui->planeB->blockSignals(false);

        colEdit->cmask                = chunkColMask[collisionLyr][selectedDrawTile];
        collisionViewer->collisionLyr = 0;

        colEdit->update();
        collisionViewer->update();
    });

    connect(ui->planeB, &QRadioButton::toggled, [this] {
        collisionLyr     = 1;

        ui->planeA->blockSignals(true);
        ui->angMaskDir->blockSignals(true);
        ui->angFlag->blockSignals(true);
        ui->angFloor->blockSignals(true);
        ui->angRoof->blockSignals(true);
        ui->angLWall->blockSignals(true);
        ui->angRWall->blockSignals(true);

        ui->planeA->setChecked(false);

        ui->angMaskDir->setCurrentIndex(chunkColMask[collisionLyr][selectedDrawTile]->direction);
        ui->angFlag->setValue(chunkColMask[collisionLyr][selectedDrawTile]->flags);
        ui->angFloor->setValue(chunkColMask[collisionLyr][selectedDrawTile]->floorAngle);
        ui->angRoof->setValue(chunkColMask[collisionLyr][selectedDrawTile]->roofAngle);
        ui->angLWall->setValue(chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle);
        ui->angRWall->setValue(chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle);

        ui->planeA->blockSignals(false);
        ui->angFlag->blockSignals(false);
        ui->angFloor->blockSignals(false);
        ui->angRoof->blockSignals(false);
        ui->angLWall->blockSignals(false);
        ui->angRWall->blockSignals(false);
        ui->planeA->blockSignals(false);

        colEdit->cmask                = chunkColMask[collisionLyr][selectedDrawTile];
        collisionViewer->collisionLyr = 1;
        colEdit->update();
        collisionViewer->update();
    });

    connect(ui->refreshCol, &QPushButton::pressed, [this]{
        colEdit->cmask                = chunkColMask[collisionLyr][selectedDrawTile];
        collisionViewer->update();
    });

    connect(chunkViewer, &ChunkColViewer::tileSelected, this, &ChunkCollisionEditor::changeSelTile);

    connect(ui->angMaskDir, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int i) { chunkColMask[collisionLyr][selectedDrawTile]->direction = i != 0; });

    connect(ui->angFlag, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { chunkColMask[collisionLyr][selectedDrawTile]->flags = (byte)v; });

    connect(ui->angFloor, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { chunkColMask[collisionLyr][selectedDrawTile]->floorAngle = (byte)v; });

    connect(ui->angRoof, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { chunkColMask[collisionLyr][selectedDrawTile]->roofAngle = (byte)v; });

    connect(ui->angLWall, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle = (byte)v; });

    connect(ui->angRWall, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int v) { chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle = (byte)v; });

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

    connect(ui->calcFloor, &QPushButton::clicked, [this, calcFloorAngle] {
        chunkColMask[collisionLyr][selectedDrawTile]->floorAngle = calcFloorAngle(chunkColMask[collisionLyr][selectedDrawTile]);
        ui->angFloor->blockSignals(true);
        ui->angFloor->setValue(chunkColMask[collisionLyr][selectedDrawTile]->floorAngle);
        ui->angFloor->blockSignals(false);
    });

    connect(ui->calcRoof, &QPushButton::clicked, [this, calcRoofAngle] {
        chunkColMask[collisionLyr][selectedDrawTile]->roofAngle = calcRoofAngle(chunkColMask[collisionLyr][selectedDrawTile]);
        ui->angRoof->blockSignals(true);
        ui->angRoof->setValue(chunkColMask[collisionLyr][selectedDrawTile]->roofAngle);
        ui->angRoof->blockSignals(false);
    });

    connect(ui->calcLWall, &QPushButton::clicked, [this, calcFloorAngle, calcRoofAngle] {
        RSDKv5::TileConfig::CollisionMask rotMask = *chunkColMask[collisionLyr][selectedDrawTile];

        if (chunkColMask[collisionLyr][selectedDrawTile]->direction) { // Ceiling Tile
            chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle = calcRoofAngle(chunkColMask[collisionLyr][selectedDrawTile]);

            // LWall rotations
            for (int c = 0; c < 16; ++c) {
                int h                      = 0;
                rotMask.collision[c].solid = true;
                while (true) {
                    if (h == 16) {
                        rotMask.collision[c].solid = false;
                        break;
                    }

                    byte m = chunkColMask[collisionLyr][selectedDrawTile]->collision[h].height;
                    if (chunkColMask[collisionLyr][selectedDrawTile]->collision[h].solid && c <= m) {
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
            chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle = calcFloorAngle(chunkColMask[collisionLyr][selectedDrawTile]);

            // LWall rotations
            for (int c = 0; c < 16; ++c) {
                int h                      = 0;
                rotMask.collision[c].solid = true;
                while (true) {
                    if (h == 16) {
                        rotMask.collision[c].solid = false;
                        break;
                    }

                    byte m = chunkColMask[collisionLyr][selectedDrawTile]->collision[h].height;
                    if (chunkColMask[collisionLyr][selectedDrawTile]->collision[h].solid && c >= m) {
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
            chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle = 0xC0;

        ui->angLWall->blockSignals(true);
        ui->angLWall->setValue(chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle);
        ui->angLWall->blockSignals(false);
    });

    connect(ui->calcRWall, &QPushButton::clicked, [this, calcFloorAngle, calcRoofAngle] {
        RSDKv5::TileConfig::CollisionMask rotMask = *chunkColMask[collisionLyr][selectedDrawTile];

        if (chunkColMask[collisionLyr][selectedDrawTile]->direction) { // Ceiling Tile
            chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle = calcRoofAngle(chunkColMask[collisionLyr][selectedDrawTile]);

            // RWall rotations
            for (int c = 0; c < 16; ++c) {
                int h                      = 15;
                rotMask.collision[c].solid = true;
                while (true) {
                    if (h == -1) {
                        rotMask.collision[c].solid = false;
                        break;
                    }

                    byte m = chunkColMask[collisionLyr][selectedDrawTile]->collision[h].height;
                    if (chunkColMask[collisionLyr][selectedDrawTile]->collision[h].solid && c <= m) {
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
            chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle = calcFloorAngle(chunkColMask[collisionLyr][selectedDrawTile]);

            // RWall rotations
            for (int c = 0; c < 16; ++c) {
                int h                      = 15;
                rotMask.collision[c].solid = true;
                while (true) {
                    if (h == -1) {
                        rotMask.collision[c].solid = false;
                        break;
                    }

                    byte m = chunkColMask[collisionLyr][selectedDrawTile]->collision[h].height;
                    if (chunkColMask[collisionLyr][selectedDrawTile]->collision[h].solid && c >= m) {
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
            chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle = 0x40;

        ui->angRWall->blockSignals(true);
        ui->angRWall->setValue(chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle);
        ui->angRWall->blockSignals(false);
    });

}

ChunkCollisionEditor::~ChunkCollisionEditor() { delete ui; }

void ChunkCollisionEditor::changeSelTile(int c)
{
    selectedDrawTile = c;

    ui->angMaskDir->setCurrentIndex(chunkColMask[collisionLyr][selectedDrawTile]->direction);
    ui->angFlag->setValue(chunkColMask[collisionLyr][selectedDrawTile]->flags);
    ui->angFloor->setValue(chunkColMask[collisionLyr][selectedDrawTile]->floorAngle);
    ui->angRoof->setValue(chunkColMask[collisionLyr][selectedDrawTile]->roofAngle);
    ui->angLWall->setValue(chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle);
    ui->angRWall->setValue(chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle);

    colEdit->cmask   = chunkColMask[collisionLyr][selectedDrawTile];
    colEdit->tileImg = imageTileList[selectedDrawTile];
    colEdit->update();
    chunkViewer->update();

    ui->textTileID->setText("Tile ID: " + QString::number(storedTileList[selectedDrawTile]));
}

bool ChunkCollisionEditor::event(QEvent *e)
{
    return QWidget::event(e);
}

bool ChunkColViewer::event(QEvent *e)
{
    switch (e->type()) {
        default: break;
        case QEvent::Paint: {
            paintEvent((QPaintEvent *)e);
            break;
        }
        case QEvent::MouseButtonPress: {
            QMouseEvent *mEvent = static_cast<QMouseEvent *>(e);
            short x = floor((float)(mEvent->x() / ((qreal)width() / 8)));
            short y = floor((float)(mEvent->y() / ((qreal)height() / 8)));
            offset.x = x;
            offset.y = y;
            emit tileSelected(x + (8 * y));
            break;
        }
    }
    return QWidget::event(e);
}

void ChunkColViewer::paintEvent(QPaintEvent *)
{
    QPainter c(this);
    const QBrush brush = c.brush();
    c.scale(2.0f, 2.0f);
    QPen pen(QColor(0xFF, 0xFF, 0x00));
    c.setPen(pen);
    c.drawImage(QRect(0, 0, width(), height()), tiles[0]);

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto &tile = chunks->chunks[*cSel].tiles[y][x];
            bool dx = (tile.direction & 1) == 1;
            bool dy = (tile.direction & 2) == 2;
            if (!drawChk){
                auto mask = cmask[collisionLyr][tile.tileIndex];
                // c.drawImage(QRect(16 * x, 16 * y, 16, 16), tiles[tile.tileIndex].mirrored(dx,dy));

                for (int fy = 0; fy < 16; ++fy) {
                    for (int fx = 0; fx < 16; ++fx) {
                        int drawX = dx ? 15 - fx : fx;
                        if (dy && mask->collision[fx].height >= y && mask->collision[fx].solid)
                            c.drawRect(QRectF(drawX + (16 * x), (15 - fy) + (16 * y), 1, 1));

                        else if (!dy && mask->collision[fx].height <= fy && mask->collision[fx].solid)
                            c.drawRect(QRectF(drawX + (16 * x), fy + (16 * y), 1, 1));
                    }
                }
            } else { c.drawImage(QRect(16 * x, 16 * y, 16, 16), tiles[tile.tileIndex].mirrored(dx,dy)); }
        }
    }

    if (drawChk)
        c.drawRect(QRectF(offset.x * 16, offset.y * 16 , 16, 16));
}

ChunkColEdit::ChunkColEdit(QWidget *parent) : QWidget(parent) { setMouseTracking(true); }

void ChunkColEdit::paintEvent(QPaintEvent *)
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
                    p.setOpacity(0.5);

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
                    p.setOpacity(0.5);

                    if (!cmask->collision[x].solid)
                        p.setBrush(p.brush().color().darker(255));

                    p.drawRect(rect.translated(x * (qreal)width() / 16, y * (qreal)height() / 16));
                }
            }
        }
    }
}

void ChunkColEdit::leaveEvent(QEvent *)
{
    highlight = -1;
    update();
}

void ChunkColEdit::mousePressEvent(QMouseEvent *event)
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

void ChunkColEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressedL)
        pressedL = !((event->button() & Qt::LeftButton) == Qt::LeftButton);
    if (pressedR)
        pressedR = !((event->button() & Qt::RightButton) == Qt::RightButton);
}

void ChunkColEdit::mouseMoveEvent(QMouseEvent *event)
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

#include "moc_chunkcollisioneditor.cpp"
