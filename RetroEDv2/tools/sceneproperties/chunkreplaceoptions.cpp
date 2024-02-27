#include "includes.hpp"
#include "chunkreplaceoptions.hpp"
#include "ui_chunkreplaceoptions.h"

ChunkReplaceOptions::ChunkReplaceOptions(byte gameVer, FormatHelpers::Chunks *chk, QList<QImage> &chunkList, QList<QImage> &tiles, QWidget *parent) :
    QDialog(parent), ui(new Ui::ChunkReplaceOptions), chunks(chk), tileList(tiles), chunkImgList(chunkList)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    ui->setupUi(this);

    this->setWindowTitle("Chunk Replace");

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    if (!chk)
        return;

    chkView = new ChunkReplaceViewer(&selectedChunk, chunks, tiles);
    chkView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->chunkView->layout()->addWidget(chkView);

    ui->srcChunkList->clear();
    for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); ++c) {
        auto *item = new QListWidgetItem(QString::number(c), ui->srcChunkList);
        item->setIcon(QPixmap::fromImage(chunkList[c]));
    }

    ui->dstChunkList->clear();
    for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); ++c) {
        auto *item = new QListWidgetItem(QString::number(c), ui->dstChunkList);
        item->setIcon(QPixmap::fromImage(chunkList[c]));
    }

    if (gameVer == ENGINE_v1){
        ui->srcChunk->setMaximum(255);
        ui->dstChunk->setMaximum(255);
    }

    connect(ui->srcChunkList, &QListWidget::itemSelectionChanged,[this]{
        int c = ui->srcChunkList->currentRow();
        int destChunk = ui->dstChunkList->currentRow();
        auto item = ui->srcChunkList->item(c);

        item->setSelected(true);

        ui->dstChunkList->blockSignals(true);

        if (!shiftSelect && !ctrlSelect)
        {
            ui->srcChunk->setValue(c);
        }

        selectedChunk = c;
        chkView->repaint();

        if (ui->srcChunkList->selectedItems().count() > 1){
          int i = destChunk + ui->srcChunkList->selectedItems().count();

          for (int s = destChunk; s < i; s++)
              ui->dstChunkList->item(s)->setSelected(true);
        } else
            ui->dstChunkList->item(destChunk)->setSelected(true);

        ui->dstChunkList->blockSignals(false);
    });

    connect(ui->dstChunkList, &QListWidget::itemSelectionChanged,[this]{

        ui->dstChunkList->blockSignals(true);
        int c = ui->dstChunkList->currentRow();
        ui->dstChunkList->clearSelection();

        ui->dstChunk->setValue(c);
        ui->dstChunkList->item(c)->setSelected(true);

        replacedChunk = c;

        int i = c + ui->srcChunkList->selectedItems().count();
        int o = c;
        for (int y = o; y < i; y++)
          ui->dstChunkList->item(y)->setSelected(true);

        ui->dstChunkList->blockSignals(false);
    });


    connect(ui->srcChunk, QOverload<int>::of(&QSpinBox::valueChanged),[this](int c){
        ui->srcChunkList->setCurrentRow(c);
        ui->srcChunkList->clearSelection();
    });

    connect(ui->dstChunk, QOverload<int>::of(&QSpinBox::valueChanged),[this](int c){
        ui->dstChunkList->setCurrentRow(c);
        ui->dstChunkList->clearSelection();
    });

    connect(ui->flipX, &QCheckBox::toggled, [this](bool b) { chkView->flipX = b; chkView->repaint(); });
    connect(ui->flipY, &QCheckBox::toggled, [this](bool b) { chkView->flipY = b; chkView->repaint(); });


    connect(ui->replaceButton, &QPushButton::clicked, [this]{
        auto chunkStore = ui->srcChunkList->selectedItems();

        if (chunkStore.count() > 1){
            // PrintLog(QString("Mutiple replace mode"));

            QList<FormatHelpers::Chunks::Chunk> storedChunks = {};

            for (QListWidgetItem *chunk : ui->srcChunkList->selectedItems()){
                int i = chunk->listWidget()->row(chunk);

                FormatHelpers::Chunks::Chunk &src = chunks->chunks[i];
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        tempChunk.tiles[y][x].tileIndex   = src.tiles[y][x].tileIndex;
                        tempChunk.tiles[y][x].visualPlane = src.tiles[y][x].visualPlane;
                        tempChunk.tiles[y][x].solidityA   = src.tiles[y][x].solidityA;
                        tempChunk.tiles[y][x].solidityB   = src.tiles[y][x].solidityB;
                        tempChunk.tiles[y][x].direction   = src.tiles[y][x].direction;
                    }
                }
                storedChunks.append(tempChunk);
            }

            for (int i = 0; i < storedChunks.count(); i++){
                FormatHelpers::Chunks::Chunk &dst = chunks->chunks[replacedChunk + i];
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        byte yPos = 0; byte xPos = 0;
                        if (chkView->flipX && chkView->flipY)
                            {yPos = 7 - y; xPos = 7 - x;}
                        else if (chkView->flipX)
                            {yPos = y; xPos = 7 - x;}
                        else if (chkView->flipY)
                            {yPos = 7 - y; xPos = x;}
                        else
                            {yPos = y; xPos = x;}

                        dst.tiles[yPos][xPos].tileIndex   = storedChunks[i].tiles[y][x].tileIndex;
                        dst.tiles[yPos][xPos].visualPlane = storedChunks[i].tiles[y][x].visualPlane;
                        dst.tiles[yPos][xPos].solidityA   = storedChunks[i].tiles[y][x].solidityA;
                        dst.tiles[yPos][xPos].solidityB   = storedChunks[i].tiles[y][x].solidityB;
                        dst.tiles[yPos][xPos].direction   = storedChunks[i].tiles[y][x].direction ^ (chkView->flipX + (chkView->flipY * 2));
                    }
                }

                chunkImgList[replacedChunk + i] = chunks->chunks[replacedChunk + i].getImage(tileList);
                ui->srcChunkList->item(replacedChunk + i)->setIcon(QPixmap::fromImage(chunks->chunks[replacedChunk + i].getImage((tileList))));
                ui->dstChunkList->item(replacedChunk + i)->setIcon(QPixmap::fromImage(chunks->chunks[replacedChunk + i].getImage((tileList))));
            }
        } else {
            // PrintLog(QString("Single replace mode"));

            FormatHelpers::Chunks::Chunk &src = chunks->chunks[selectedChunk];

            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    tempChunk.tiles[y][x].tileIndex   = src.tiles[y][x].tileIndex;
                    tempChunk.tiles[y][x].visualPlane = src.tiles[y][x].visualPlane;
                    tempChunk.tiles[y][x].solidityA   = src.tiles[y][x].solidityA;
                    tempChunk.tiles[y][x].solidityB   = src.tiles[y][x].solidityB;
                    tempChunk.tiles[y][x].direction   = src.tiles[y][x].direction;
                }
            }

            FormatHelpers::Chunks::Chunk &dst = chunks->chunks[replacedChunk];

            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    byte yPos = 0; byte xPos = 0;
                    if (chkView->flipX && chkView->flipY)
                        {yPos = 7 - y; xPos = 7 - x;}
                    else if (chkView->flipX)
                        {yPos = y; xPos = 7 - x;}
                    else if (chkView->flipY)
                        {yPos = 7 - y; xPos = x;}
                    else
                        {yPos = y; xPos = x;}

                    dst.tiles[yPos][xPos].tileIndex   = tempChunk.tiles[y][x].tileIndex;
                    dst.tiles[yPos][xPos].visualPlane = tempChunk.tiles[y][x].visualPlane;
                    dst.tiles[yPos][xPos].solidityA   = tempChunk.tiles[y][x].solidityA;
                    dst.tiles[yPos][xPos].solidityB   = tempChunk.tiles[y][x].solidityB;
                    dst.tiles[yPos][xPos].direction   = tempChunk.tiles[y][x].direction ^ (chkView->flipX + (chkView->flipY * 2));
                }
            }

            modified = true;

            chunkImgList[replacedChunk] = chunks->chunks[replacedChunk].getImage(tileList);
            ui->srcChunkList->item(replacedChunk)->setIcon(QPixmap::fromImage(chunks->chunks[replacedChunk].getImage((tileList))));
            ui->dstChunkList->item(replacedChunk)->setIcon(QPixmap::fromImage(chunks->chunks[replacedChunk].getImage((tileList))));
        }
    });
}

ChunkReplaceOptions::~ChunkReplaceOptions(){ delete ui; }


bool ChunkReplaceOptions::event(QEvent *e)
{
    switch (e->type()) {
        default: break;
        case QEvent::KeyPress: {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
            if (keyEvent->key() == Qt::Key_Control)
                ctrlSelect  = true;
            if (keyEvent->key() == Qt::Key_Shift)
                shiftSelect = true;
            break;
        }
        case QEvent::KeyRelease: {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
            if (keyEvent->key() == Qt::Key_Control)
                ctrlSelect  = false;
            if (keyEvent->key() == Qt::Key_Shift)
                shiftSelect = false;
            break;
        }
    }
    return QWidget::event(e);
}

void ChunkReplaceViewer::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter p(this);
    p.scale(zoom, zoom);

    float w = this->width(), h = this->height();
    float originX = w / 2, originY = h / 2;
    originX -= offset.x;
    originY -= offset.y;
    originX *= (1.0f / zoom);
    originY *= (1.0f / zoom);
    originX -= (4 * 16);
    originY -= (4 * 16);


    const QBrush brush = p.brush();

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            byte yPos = 0; byte xPos = 0;
            if (flipX && flipY)
                {yPos = 7 - y; xPos = 7 - x;}
            else if (flipX)
                {yPos = y; xPos = 7 - x;}
            else if (flipY)
                {yPos = 7 - y; xPos = x;}
            else
                {yPos = y; xPos = x;}

            auto &tile = chunks->chunks[*cSel].tiles[yPos][xPos];

            bool fx = !flipX ? (tile.direction & 1) == 1 : (tile.direction & 1) != 1;
            bool fy = !flipY ? (tile.direction & 2) == 2 : (tile.direction & 2) != 2;
            p.drawImage(QPointF(originX + x * 0x10, originY + y * 0x10),
                        tiles[tile.tileIndex].mirrored(fx, fy));
        }
    }
}

#include "moc_chunkreplaceoptions.cpp"
