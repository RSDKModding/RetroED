#include "includes.hpp"
#include "importgfxtool.hpp"
#include "ui_importgfxtool.h"

ImportGFXTool::ImportGFXTool(byte gameVer, QList<QImage> &chunkListSrc, QList<QImage> &chunkListDst, QList<PaletteColor> stagePal, QList<QRgb> importPal, QWidget *parent) :
    QDialog(parent), ui(new Ui::ImportGFXTool), chunkImgList(chunkListDst)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    ui->setupUi(this);

    this->setWindowTitle("GFX Import");

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    chkView = new GFXImportViewer(&selectedChunk, chunkListSrc, chunkListDst[0].width() == 16);
    chkView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->chunkView->layout()->addWidget(chkView);

    paletteFrame = new ViewerWidget(this);
    if (gameVer == ENGINE_v1){
        for (int i = 0; i < 128; ++i){
            palFramePalette.append((PaletteColor)stagePal[i]);
        }
    } else {
        for (int i = 128; i < 256; ++i){
            palFramePalette.append((PaletteColor)stagePal[i]);
        }
    }
    paletteFrame->palette = &palFramePalette;
    paletteFrame->palSize = 128;
    paletteFrame->setMinimumSize(256,256);
    ui->palFrameIn->layout()->addWidget(paletteFrame);

    ui->importPal->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->importPal->setDragDropMode(QListView::NoDragDrop);

    connect(ui->importPal, &QListWidget::itemSelectionChanged,[this, importPal]{
        int c = ui->importPal->currentRow();
        auto item = ui->importPal->item(c);

        item->setSelected(true);
        paletteFrame->blockSignals(true);
        paletteFrame->importPalette.clear();

        for (int s = 0; s < ui->importPal->selectedItems().count(); s++){
            QRgb slot = ui->importPal->selectedItems().at(s)->data(Qt::UserRole + 10).value<QRgb>();
            paletteFrame->importPalette.append((PaletteColor)slot);
        }
        paletteFrame->blockSignals(false);
        paletteFrame->update();

    });

    connect(ui->resetPal, &QPushButton::pressed, [=]{
        if (gameVer == ENGINE_v1){
            for (int i = 0; i < 128; ++i){
                palFramePalette.replace(i,(PaletteColor)importPal[i]);
            }
        } else {
            for (int i = 128; i < 256; ++i){
                palFramePalette.replace(i - 128, (PaletteColor)importPal[i]);
            }
        }
        ui->importPal->clear();
        QList<QRgb> newClrs(chkPal.begin(), chkPal.end());
        for (int c = 0; c < newClrs.count(); c++){
            PaletteColor clr((newClrs[c] >> 16) & 0xFF, (newClrs[c] >> 8) & 0xFF, newClrs[c] & 0xFF);
            auto *item = new QListWidgetItem(NULL, ui->importPal);
            QImage swatch(16, 16, QImage::Format_RGB888);
            swatch.fill(newClrs[c]);
            item->setIcon(QPixmap::fromImage(swatch));
            item->setData(Qt::UserRole + 10, newClrs[c]);
        }
        paletteFrame->importPalette.clear();
        curPalette = palFramePalette;
        ui->palFrameIn->repaint();
    });

    curPalette = palFramePalette;
    connect(paletteFrame, &ViewerWidget::pastePalette, [=](int x, int y){
        ui->importPal->blockSignals(true);
        QList<QRgb> newClrs(chkPal.begin(), chkPal.end());
        curPalette = palFramePalette;
        int startPos = (y * 0x10) + x;
        int index = 0;
        for (int c = startPos; c < startPos + paletteFrame->importPalette.count(); c++){
            if (c >= curPalette.count())
                break;
            PaletteColor slot = paletteFrame->importPalette[index++].toQColor();
            // add back the replaced color if it was new
            if (newClrs.contains(curPalette[c].toQColor().rgb())){
                auto *item = new QListWidgetItem(NULL, ui->importPal);
                QImage swatch(16, 16, QImage::Format_RGB888);
                swatch.fill(curPalette[c].toQColor().rgb());
                item->setIcon(QPixmap::fromImage(swatch));
                item->setData(Qt::UserRole + 10, curPalette[c].toQColor().rgb());
            }
            curPalette.replace(c, slot);
            paletteFrame->palette->replace(c, slot);
            delete ui->importPal->selectedItems().takeFirst();
        }
        ui->importPal->blockSignals(false);
        paletteFrame->importPalette.clear();
        ui->palFrameIn->repaint();
    });

    connect(ui->genColCheck, &QCheckBox::clicked, [=](bool c){ generateCol = c; });

    ui->srcList->clear();
    for (int c = 0; c < chunkListSrc.count(); ++c) {
        auto *item = new QListWidgetItem(QString::number(c), ui->srcList);
        item->setIcon(QPixmap::fromImage(chunkListSrc[c]));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }

    ui->dstList->clear();
    for (int c = 0; c < chunkListDst.count(); ++c) {
        if (chunkListDst.count() != 0x400 && gameVer == ENGINE_v1 && c == 0x100)
            break;
        auto *item = new QListWidgetItem(QString::number(c), ui->dstList);
        item->setIcon(QPixmap::fromImage(chunkListDst[c]));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }

    connect(ui->srcList, &QListWidget::itemSelectionChanged,[this]{
        int c = ui->srcList->currentRow();
        if (ui->dstList->currentRow() == -1)
            ui->dstList->setCurrentRow(0);
        int destChunk = ui->dstList->currentRow();
        auto item = ui->srcList->item(c);

        item->setSelected(true);

        ui->dstList->blockSignals(true);

        selectedChunk = c;
        PrintLog(QString("%1").arg(selectedChunk));
        chkView->repaint();

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

        replacedChunk = c;

        int i = c + ui->srcList->selectedItems().count();
        int o = c;
        for (int y = o; y < i; y++)
          ui->dstList->item(y)->setSelected(true);

        ui->dstList->blockSignals(false);
    });

    connect(ui->flipX, &QCheckBox::toggled, [this](bool b) { chkView->flipX = b; chkView->repaint(); });
    connect(ui->flipY, &QCheckBox::toggled, [this](bool b) { chkView->flipY = b; chkView->repaint(); });

    connect(ui->replaceButton, &QPushButton::clicked, [=]{
        QList<int> importIDs;
        for (QListWidgetItem *chunk : ui->srcList->selectedItems()){
            int i = ui->srcList->row(chunk);
            importIDs.append(i);
        }

        int scID = 0;
        for (QListWidgetItem *chunk : ui->dstList->selectedItems()){
            int i = ui->dstList->row(chunk);
            chunkImgList[i] = chunkListSrc[importIDs[scID]].mirrored(chkView->flipX, chkView->flipY);

            for (int y = 0; y < chunkImgList[i].height(); ++y) {
                for (int x = 0; x < chunkImgList[i].width(); ++x) {
                    if (chunkImgList[i].pixel(x,y) != qRgb(stagePal[0].r,stagePal[0].g, stagePal[0].b))
                        chkPal.insert(chunkImgList[i].pixel(x,y));
                }
            }

            ui->dstList->item(i)->setIcon(QPixmap::fromImage(chunkListSrc[importIDs[scID]].mirrored(chkView->flipX, chkView->flipY)));
            rplChkIDs.insert(i);
            scID++;
        }
        modified = true;

        ui->importPal->clear();
        QList<QRgb> newClrs(chkPal.begin(), chkPal.end());
        for (int c = 0; c < newClrs.count(); c++){
            PaletteColor clr((newClrs[c] >> 16) & 0xFF, (newClrs[c] >> 8) & 0xFF, newClrs[c] & 0xFF);
            bool exists = false;
            for (auto palSlot : curPalette){
                if (palSlot.toQColor() == clr.toQColor())
                    exists = true;
            }
            if (!exists){
                auto *item = new QListWidgetItem(NULL, ui->importPal);
                QImage swatch(16, 16, QImage::Format_RGB888);
                swatch.fill(newClrs[c]);
                item->setIcon(QPixmap::fromImage(swatch));
                item->setData(Qt::UserRole + 10, newClrs[c]);
            }
        }
    });
}

ImportGFXTool::~ImportGFXTool(){ delete ui; }


bool ImportGFXTool::event(QEvent *e)
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

void GFXImportViewer::paintEvent(QPaintEvent *event)
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

#include "moc_importgfxtool.cpp"
