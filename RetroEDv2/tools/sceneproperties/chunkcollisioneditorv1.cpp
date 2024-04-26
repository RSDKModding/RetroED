#include "includes.hpp"
#include "ui_chunkcollisioneditorv1.h"
#include "chunkcollisioneditorv1.hpp"
#include <RSDKv1/tileconfigv1.hpp>

ChunkCollisionEditorv1::ChunkCollisionEditorv1(FormatHelpers::Chunks *chk, ushort curChunk, QList<QImage> &tiles, SceneViewer *viewer)
                      : ui(new Ui::ChunkCollisionEditorv1), chunks(chk), tileList(tiles)
{
    ui->setupUi(this);
    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    selectedChunk    = curChunk;
    collisionLyr     = defaultVisualPlane;
    selectedDrawTile = 0;
    maskIndex        = 0;
    ui->planeA->setChecked(true);

    int i = 0;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto &tile = chunks->chunks[curChunk].tiles[y][x];
            storedTileList[i]  = tile.tileIndex;
            chunkColMask[0][i] = &viewer->tileconfigv1.collisionPaths[0][tile.tileIndex];
            chunkColMask[1][i] = &viewer->tileconfigv1.collisionPaths[1][tile.tileIndex];
            imageTileList[i++] = tiles[tile.tileIndex];
        }
    }

    ui->labelTID->setText("Tile ID: " + QString::number(storedTileList[selectedDrawTile]));
    ui->textChunkID->setText("Chunk ID: " + QString::number(curChunk));

    chunkViewer = new ChunkColViewerv1(&selectedChunk, chunks, tiles, true);
    chunkViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    collisionViewer = new ChunkColViewerv1(&selectedChunk, chunks, tiles, false);
    collisionViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    for (int i = 0; i < 0x400; ++i) {
        collisionViewer->cmask[0][i] = &viewer->tileconfigv1.collisionPaths[0][i];
        collisionViewer->cmask[1][i] = &viewer->tileconfigv1.collisionPaths[1][i];
    }

    collisionViewer->maskIndex = 0;
    collisionViewer->collisionLyr = 0;
    ui->chunkTileList->layout()->addWidget(chunkViewer);
    ui->chunkCol->layout()->addWidget(collisionViewer);

    colEdit = new ChunkColEditv1();
    colEdit->cmask     =  chunkColMask[collisionLyr][selectedDrawTile];
    colEdit->tileImg   = imageTileList[selectedDrawTile];
    colEdit->maskIndex = 0;
    ui->tileCol->layout()->addWidget(colEdit);

    connect(ui->colMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int i) {
        colEdit->cmask->collisionMode = i;
        colEdit->update();
        collisionViewer->update();
    });

    connect(ui->maskIndex, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int i) {
        collisionViewer->maskIndex = i;
        colEdit->maskIndex = i;
        maskIndex = i;
        colEdit->update();
        collisionViewer->update();
    });

    connect(ui->planeA, &QRadioButton::toggled, [this] {
        collisionLyr     = 0;

        ui->planeB->blockSignals(true);
        ui->planeB->setChecked(false);
        ui->planeB->blockSignals(false);

        colEdit->cmask                = chunkColMask[collisionLyr][selectedDrawTile];
        collisionViewer->collisionLyr = 0;

        colEdit->update();
        collisionViewer->update();
    });

    connect(ui->planeB, &QRadioButton::toggled, [this] {
        collisionLyr     = 1;

        ui->planeA->blockSignals(true);
        ui->planeA->setChecked(false);
        ui->planeA->blockSignals(false);

        collisionViewer->collisionLyr = 1;
        colEdit->cmask                = chunkColMask[collisionLyr][selectedDrawTile];
        colEdit->update();
        collisionViewer->update();
    });

    connect(chunkViewer, &ChunkColViewerv1::tileSelected, this, &ChunkCollisionEditorv1::changeSelTile);

    connect(colEdit, &ChunkColEditv1::updateViewer, [=]{ collisionViewer->update();});
}

ChunkCollisionEditorv1::~ChunkCollisionEditorv1() { delete ui; }

void ChunkCollisionEditorv1::changeSelTile(int c)
{
    selectedDrawTile = c;

    //ui->angMaskDir->setCurrentIndex(chunkColMask[collisionLyr][selectedDrawTile]->direction);

    colEdit->cmask   = chunkColMask[collisionLyr][selectedDrawTile];
    colEdit->tileImg = imageTileList[selectedDrawTile];
    colEdit->update();
    chunkViewer->update();
    ui->colMode->setCurrentIndex(colEdit->cmask->collisionMode);
    ui->labelTID->setText("Tile ID: " + QString::number(storedTileList[selectedDrawTile]));
}

bool ChunkCollisionEditorv1::event(QEvent *e)
{
    return QWidget::event(e);
}

bool ChunkColViewerv1::event(QEvent *e)
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

void ChunkColViewerv1::paintEvent(QPaintEvent *)
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
                        int drawX = (maskIndex & 1) == 0 ? fx : abs(fx - 15);
                        int drawY = (maskIndex & 2) == 0 ? fy : abs(fy - 15);
                        if (((maskIndex & 2) == 0 ? mask->collision[maskIndex][drawX].height <= drawY : mask->collision[maskIndex][drawX].height >= drawY)
                                && mask->collision[maskIndex][drawX].solid)
                            c.drawRect(QRectF((16 * x) + drawX, (16 * y) + drawY, 1, 1));
                    }
                }
            } else { c.drawImage(QRect(16 * x, 16 * y, 16, 16), tiles[tile.tileIndex].mirrored(dx,dy)); }
        }
    }

    if (drawChk)
        c.drawRect(QRectF(offset.x * 16, offset.y * 16 , 16, 16));
}

ChunkColEditv1::ChunkColEditv1(QWidget *parent) : QWidget(parent) { setMouseTracking(true); }

void ChunkColEditv1::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QRectF rect(0, 0, (qreal)width() / 16, (qreal)height() / 16);
    p.drawImage(QRect(0, 0, width(), height()), tileImg);

    for (byte y = 0; y < 16; ++y) {
        for (byte x = 0; x < 16; ++x) {
            int dirX = (maskIndex & 1) == 0 ? x : abs(x - 15);
            int dirY = (maskIndex & 2) == 0 ? y : abs(y - 15);
            if ((maskIndex & 2) == 0
                    ? cmask->collision[maskIndex][dirX].height <= dirY : cmask->collision[maskIndex][dirX].height >= dirY) {

                QPen pen(qApp->palette().base(), 2);
                p.setPen(pen);
                p.setBrush(QColor(0x00FF00));
                p.setOpacity(0.5);

                if (!cmask->collision[maskIndex][dirX].solid)
                    p.setBrush(p.brush().color().darker(255));

                p.drawRect(rect.translated(dirX * (qreal)width() / 16, dirY * (qreal)height() / 16));
            }
        }
    }
}

void ChunkColEditv1::leaveEvent(QEvent *)
{
    highlight = -1;
    update();
}

void ChunkColEditv1::mousePressEvent(QMouseEvent *event)
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
        enabling = !cmask->collision[maskIndex][x].solid;
}

void ChunkColEditv1::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressedL)
        pressedL = !((event->button() & Qt::LeftButton) == Qt::LeftButton);
    if (pressedR)
        pressedR = !((event->button() & Qt::RightButton) == Qt::RightButton);
    updateViewer();
}

void ChunkColEditv1::mouseMoveEvent(QMouseEvent *event)
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
        cmask->collision[maskIndex][x].solid = enabling;

    if (pressedL)
        cmask->collision[maskIndex][x].height = y;

    update();
}

#include "moc_chunkcollisioneditorv1.cpp"
