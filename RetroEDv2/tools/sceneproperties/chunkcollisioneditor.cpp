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

    ui->angMaskDir->setCurrentIndex(chunkColMask[collisionLyr][selectedDrawTile]->direction);
    ui->angFlag->setValue(chunkColMask[collisionLyr][selectedDrawTile]->flags);
    ui->angFloor->setValue(chunkColMask[collisionLyr][selectedDrawTile]->floorAngle);
    ui->angRoof->setValue(chunkColMask[collisionLyr][selectedDrawTile]->roofAngle);
    ui->angLWall->setValue(chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle);
    ui->angRWall->setValue(chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle);

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
        collisionLyr = 0;

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

        colEdit->cmask        = chunkColMask[collisionLyr][selectedDrawTile];
        colEdit->DrawPoint1.x = -1, colEdit->DrawPoint2.x = -1;
        colEdit->DrawPoint1.y = -1, colEdit->DrawPoint2.y = -1;
        collisionViewer->collisionLyr = 0;

        colEdit->update();
        collisionViewer->update();
    });

    connect(ui->planeB, &QRadioButton::toggled, [this] {
        collisionLyr = 1;

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

        ui->angMaskDir->blockSignals(false);
        ui->angFlag->blockSignals(false);
        ui->angFloor->blockSignals(false);
        ui->angRoof->blockSignals(false);
        ui->angLWall->blockSignals(false);
        ui->angRWall->blockSignals(false);

        ui->planeA->blockSignals(false);

        colEdit->cmask        = chunkColMask[collisionLyr][selectedDrawTile];
        colEdit->DrawPoint1.x = -1, colEdit->DrawPoint2.x = -1;
        colEdit->DrawPoint1.y = -1, colEdit->DrawPoint2.y = -1;
        collisionViewer->collisionLyr = 1;

        colEdit->update();
        collisionViewer->update();
    });

    connect(chunkViewer, &ChunkColViewer::tileSelected, this, &ChunkCollisionEditor::changeSelTile);

    connect(colEdit, &ChunkColEdit::updateViewer, [=]{ collisionViewer->update();});

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

    connect(ui->angleCheck, &QCheckBox::toggled, [this](bool c){
        colEdit->mAngleMode = c;
        if (colEdit->mAngleMode)
            ui->helpLabel->setText(QString("Left Click and Drag: Set angle points"));
        else
            ui->helpLabel->setText(QString("Left Click: Set height | Right Click: Set solidity"));
    });

    auto getColAngle = [this](RSDKv5::TileConfig::CollisionMask *mask, bool isRoof) {
        byte angle = 0;
        Vector2<float> start(-1, -1), end(-1, -1);
        if (isRoof ? mask->direction : !mask->direction) // Ceiling Tile
        {
            if (!colEdit->mAngleMode || colEdit->DrawPoint1.x == -1 || colEdit->DrawPoint1.y == -1
                || colEdit->DrawPoint2.x == -1 || colEdit->DrawPoint2.y == -1) {
                int step = 0;
                int curStepH = 0;
                for (int x = 0; x < 16; ++x) {
                    if (mask->collision[x].solid) {
                        if (start.x == -1 && mask->collision[x + 1].height != (isRoof ? 15 : 0)) {
                            start.x = x;
                            start.y = mask->collision[x].height;
                        }

                        if (mask->collision[x - 1].height != (isRoof ? 15 : 0)){
                            end.x = x;
                            end.y = mask->collision[x].height;
                        }
                        if (curStepH == mask->collision[x].height)
                            step++;
                        else if (mask->collision[x].height != (isRoof ? 15 : 0)){
                            step = 0;
                            curStepH = mask->collision[x].height;
                        }
                    }
                }
                if (isRoof ? (end.y < start.y) : (start.y < end.y)) {
                    if (start.y == 0 || start.y == 15) {
                        start.x = start.x - step;
                        if (start.x < 0)
                            start.x = 0;
                        start.y = mask->collision[(int)start.x].height;
                    }
                } else {
                    if (end.y == 0 || end.y == 15) {
                        end.x = end.x + step;
                        if (end.x > 15)
                            end.x = 15;
                        end.y = mask->collision[(int)end.x].height;
                    }
                }
                colEdit->DrawPoint1.x = start.x; colEdit->DrawPoint2.x = end.x;
                colEdit->DrawPoint1.y = start.y; colEdit->DrawPoint2.y = end.y;
            } else {
                if (colEdit->DrawPoint1.x > colEdit->DrawPoint2.x) {
                    qSwap(colEdit->DrawPoint1.x, colEdit->DrawPoint2.x);
                    qSwap(colEdit->DrawPoint1.y, colEdit->DrawPoint2.y);
                }

                start.x = colEdit->DrawPoint1.x, end.x = colEdit->DrawPoint2.x;
                start.y = colEdit->DrawPoint1.y, end.y = colEdit->DrawPoint2.y;
            }
            if (isRoof){
                qSwap(start.x, end.x);
                qSwap(start.y, end.y);
            }
            float angleF = atan2((end.y - start.y), (end.x - start.x));
            angle = (byte)(angleF * (256 / (2 * RSDK_PI))) & 0xFE;
        } else
            angle = isRoof ? 0x80 : 0x00;

        return angle;
    };

    connect(ui->calcFloor, &QPushButton::clicked, [this, getColAngle] {
        chunkColMask[collisionLyr][selectedDrawTile]->floorAngle = getColAngle(chunkColMask[collisionLyr][selectedDrawTile], false);
        ui->angFloor->blockSignals(true);
        ui->angFloor->setValue(chunkColMask[collisionLyr][selectedDrawTile]->floorAngle);
        ui->angFloor->blockSignals(false);
    });

    connect(ui->calcRoof, &QPushButton::clicked, [this, getColAngle] {
        chunkColMask[collisionLyr][selectedDrawTile]->roofAngle = getColAngle(chunkColMask[collisionLyr][selectedDrawTile], true);
        ui->angRoof->blockSignals(true);
        ui->angRoof->setValue(chunkColMask[collisionLyr][selectedDrawTile]->roofAngle);
        ui->angRoof->blockSignals(false);
    });

    connect(ui->calcLWall, &QPushButton::clicked, [this, getColAngle] {
        RSDKv5::TileConfig::CollisionMask rotMask = *chunkColMask[collisionLyr][selectedDrawTile];

        if (chunkColMask[collisionLyr][selectedDrawTile]->direction) { // Ceiling Tile
            chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle = getColAngle(chunkColMask[collisionLyr][selectedDrawTile], true);

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
            chunkColMask[collisionLyr][selectedDrawTile]->lWallAngle = getColAngle(chunkColMask[collisionLyr][selectedDrawTile], false);

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

    connect(ui->calcRWall, &QPushButton::clicked, [this, getColAngle] {
        RSDKv5::TileConfig::CollisionMask rotMask = *chunkColMask[collisionLyr][selectedDrawTile];

        if (chunkColMask[collisionLyr][selectedDrawTile]->direction) { // Ceiling Tile
            chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle = getColAngle(chunkColMask[collisionLyr][selectedDrawTile], true);

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
            chunkColMask[collisionLyr][selectedDrawTile]->rWallAngle = getColAngle(chunkColMask[collisionLyr][selectedDrawTile], false);

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
    colEdit->DrawPoint1.x = -1, colEdit->DrawPoint2.x = -1;
    colEdit->DrawPoint1.y = -1, colEdit->DrawPoint2.y = -1;
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
                        int drawY = dy ? 15 - fy : fy;
                        if ((mask->direction ? mask->collision[drawX].height >= fy : mask->collision[drawX].height <= fy) && mask->collision[drawX].solid)
                            c.drawRect(QRectF((16 * x) + fx, (16 * y) + drawY, 1, 1));
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

    QPen pen(qApp->palette().base(), 2);
    p.setPen(pen);
    p.setOpacity(0.5);

    if (!cmask->direction) {
        for (byte y = 0; y < 16; ++y) {
            for (byte x = 0; x < 16; ++x) {
                if (cmask->collision[x].height <= y) {
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
                    p.setOpacity(0.5);

                    if (!cmask->collision[x].solid)
                        p.setBrush(p.brush().color().darker(255));

                    p.drawRect(rect.translated(x * (qreal)width() / 16, y * (qreal)height() / 16));
                }
            }
        }
    }

    p.setPen(QPen(Qt::red, 2));
    p.setOpacity(1);
    p.setBrush(Qt::NoBrush);
    if (DrawPoint1.x != -1 && DrawPoint1.y != -1)
        p.drawRect(rect.translated(DrawPoint1.x * (qreal)width() / 16, DrawPoint1.y * (qreal)height() / 16));
    if (DrawPoint2.x != -1 && DrawPoint2.y != -1)
        p.drawRect(rect.translated(DrawPoint2.x * (qreal)width() / 16, DrawPoint2.y * (qreal)height() / 16));
    if (DrawPoint1.x != -1 && DrawPoint1.y != -1 && DrawPoint2.x != -1 && DrawPoint2.y != -1)
        p.drawLine(DrawPoint1.x * (qreal)width() / 16 + 8, DrawPoint1.y * (qreal)height() / 16 + 8,
                   DrawPoint2.x * (qreal)width() / 16 + 8, DrawPoint2.y * (qreal)height() / 16 + 8);
    update();
}

void ChunkColEdit::leaveEvent(QEvent *)
{
    highlight = -1;
    update();
}

void ChunkColEdit::mousePressEvent(QMouseEvent *event)
{
    short x = floor((float)(event->x() / ((qreal)width() / 16)));
    short y = floor((float)(event->y() / ((qreal)height() / 16)));
    if (x > 15)
        x = 15;

    if (y > 15)
        y = 15;

    if (y < 0)
        y = 0;

    if (x < 0)
        x = 0;

    selection = highlight;
    pressedL  = (event->button() & Qt::LeftButton) == Qt::LeftButton;
    pressedR  = (event->button() & Qt::RightButton) == Qt::RightButton;

    if (pressedL && mAngleMode){
        DrawPoint1.x = x;
        DrawPoint1.y = y;
    }
    if (pressedR)
        enabling = !cmask->collision[x].solid;
}

void ChunkColEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressedL)
        pressedL = !((event->button() & Qt::LeftButton) == Qt::LeftButton);
    if (pressedR)
        pressedR = !((event->button() & Qt::RightButton) == Qt::RightButton);
    updateViewer();
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

    if (!mAngleMode) {
        if (pressedR)
            cmask->collision[x].solid = enabling;

        if (pressedL)
            cmask->collision[x].height = y;
    } else {
        if (pressedL){
            DrawPoint2.x = x;
            DrawPoint2.y = y;
        }
    }

    update();
}

#include "moc_chunkcollisioneditor.cpp"
