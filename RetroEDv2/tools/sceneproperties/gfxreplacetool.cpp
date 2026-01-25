#include "includes.hpp"
#include "gfxreplacetool.hpp"
#include "ui_gfxreplacetool.h"

GFXReplaceTool::GFXReplaceTool(short listSize, QList<QImage> &gfxSrc, FormatHelpers::Chunks *chkS, QWidget *parent) :
            QDialog(parent), ui(new Ui::GFXReplaceTool), chunksFmt(chkS), imgList(gfxSrc)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    ui->setupUi(this);

    this->setWindowTitle("GFX Replace");

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    gfxView = new GFXReplaceViewer(&selectedItem, gfxSrc, gfxSrc[0].width() == 16);
    gfxView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->gfxView->layout()->addWidget(gfxView);

    ui->srcList->clear();
    ui->dstList->clear();

    for (int c = 0; c < listSize; ++c) {
        auto *itemSrc = new QListWidgetItem(QString::number(c));
        itemSrc->setIcon(QPixmap::fromImage(gfxSrc[c]));
        itemSrc->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->srcList->addItem(itemSrc);

        auto *itemDst = new QListWidgetItem(QString::number(c));
        itemDst->setIcon(QPixmap::fromImage(gfxSrc[c]));
        itemDst->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->dstList->addItem(itemDst);

        if (chkS)
            chunksDst.chunks[c] = chunksFmt->chunks[c];
    }

    connect(ui->srcList, &QListWidget::itemSelectionChanged,[this]{
        int c = ui->srcList->currentRow();
        if (ui->dstList->currentRow() == -1)
            ui->dstList->setCurrentRow(0);
        int destChunk = ui->dstList->currentRow();
        auto item = ui->srcList->item(c);

        item->setSelected(true);

        ui->dstList->blockSignals(true);

        selectedItem = c;
        gfxView->repaint();

        if (ui->srcList->selectedItems().count() > 1){
          int i = destChunk + ui->srcList->selectedItems().count();

          for (int s = destChunk; s < i; s++)
              ui->dstList->item(s)->setSelected(true);
        } else
            ui->dstList->item(destChunk)->setSelected(true);

        ui->dstList->blockSignals(false);
    });

    connect(ui->dstList, &QListWidget::itemSelectionChanged,[this]{

        ui->dstList->blockSignals(true);
        int c = ui->dstList->currentRow();
        ui->dstList->clearSelection();

        ui->dstList->item(c)->setSelected(true);

        replacedItem = c;

        int i = c + ui->srcList->selectedItems().count();
        int o = c;
        for (int y = o; y < i; y++)
          ui->dstList->item(y)->setSelected(true);

        ui->dstList->blockSignals(false);
    });

    connect(ui->flipX, &QCheckBox::toggled, [this](bool b) { gfxView->flipX = b; gfxView->repaint(); });
    connect(ui->flipY, &QCheckBox::toggled, [this](bool b) { gfxView->flipY = b; gfxView->repaint(); });

    connect(ui->replaceButton, &QPushButton::clicked, [=]{
        if (chkS){
            QList<int> importIDs;
            QList<FormatHelpers::Chunks::Chunk> storedChunks = {};
            for (QListWidgetItem *chunk : ui->srcList->selectedItems()){
                int i = ui->srcList->row(chunk);
                FormatHelpers::Chunks::Chunk &src = chunksFmt->chunks[i];
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
                importIDs.append(i);
            }

            int scID = 0;
            for (QListWidgetItem *chunk : ui->dstList->selectedItems()){
                int i = ui->dstList->row(chunk);
                FormatHelpers::Chunks::Chunk &dst = chunksDst.chunks[i];
                for (int y = 0; y < 8; ++y) {
                    for (int x = 0; x < 8; ++x) {
                        byte yPos = 0; byte xPos = 0;
                        if (gfxView->flipX && gfxView->flipY)
                            {yPos = 7 - y; xPos = 7 - x;}
                        else if (gfxView->flipX)
                            {yPos = y; xPos = 7 - x;}
                        else if (gfxView->flipY)
                            {yPos = 7 - y; xPos = x;}
                        else
                            {yPos = y; xPos = x;}

                        dst.tiles[yPos][xPos].tileIndex   = storedChunks[scID].tiles[y][x].tileIndex;
                        dst.tiles[yPos][xPos].visualPlane = storedChunks[scID].tiles[y][x].visualPlane;
                        dst.tiles[yPos][xPos].solidityA   = storedChunks[scID].tiles[y][x].solidityA;
                        dst.tiles[yPos][xPos].solidityB   = storedChunks[scID].tiles[y][x].solidityB;
                        dst.tiles[yPos][xPos].direction   = storedChunks[scID].tiles[y][x].direction ^ (gfxView->flipX + (gfxView->flipY * 2));
                    }
                }

                ui->srcList->item(i)->setIcon(QPixmap::fromImage(imgList[importIDs[scID]].mirrored(gfxView->flipX, gfxView->flipY)));
                ui->dstList->item(i)->setIcon(QPixmap::fromImage(imgList[importIDs[scID]].mirrored(gfxView->flipX, gfxView->flipY)));
                rplIDs.insert(i);
                scID++;
            }
        } else {
            QList<int> importIDs;
            for (QListWidgetItem *tile : ui->srcList->selectedItems()){
                int i = ui->srcList->row(tile);
                importIDs.append(i);
            }

            int scID = 0;
            for (QListWidgetItem *tile : ui->dstList->selectedItems()){
                int i = ui->dstList->row(tile);
                imgList[i] = imgList[importIDs[scID]];
                ui->srcList->item(i)->setIcon(QPixmap::fromImage(imgList[importIDs[scID]].mirrored(gfxView->flipX, gfxView->flipY)));
                ui->dstList->item(i)->setIcon(QPixmap::fromImage(imgList[importIDs[scID]].mirrored(gfxView->flipX, gfxView->flipY)));
                srcToRplIDs.insert(i,importIDs[scID]);
                scID++;
            }
        }
        modified = true;
    });
}

GFXReplaceTool::~GFXReplaceTool(){ delete ui; }


bool GFXReplaceTool::event(QEvent *e)
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

void GFXReplaceViewer::paintEvent(QPaintEvent *event)
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

    if (scaleImg)
        p.drawImage(QPointF(originX,originY),gfx[*cSel].mirrored(flipX, flipY).scaled(128,128));
    else
        p.drawImage(QPointF(originX,originY),gfx[*cSel].mirrored(flipX, flipY));

}

#include "moc_gfxreplacetool.cpp"
