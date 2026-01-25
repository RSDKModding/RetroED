#include "includes.hpp"
#include "ui_chunkeditor.h"
#include "chunkeditor.hpp"

#include "gfxreplacetool.hpp"
#include "importgfxtool.hpp"
#include "copyplane.hpp"
#include "../paletteeditor/colourdialog.hpp"
#include "../paletteeditor/paletteimport.hpp"
#include "dependencies/QtGifImage/src/gifimage/qgifimage.h"

enum editorModes { EDITOR_CHUNK, EDITOR_TILE, EDITOR_COLLISION };
enum drawTools { DRAW_PENCIL, DRAW_RECT, DRAW_CIRCLE, DRAW_ERASER, DRAW_LINE };
enum tileDisplayFormat {FORMAT_CHUNK, FORMAT_TILES};
enum colTools { COL_SOLIDITY, COL_ANGLES};
enum PaletteFormatTypes { PALTYPE_ACT, PALTYPE_IMAGE};

ChunkEditor::ChunkEditor(FormatHelpers::Chunks *chk, QList<QImage> &chunkList, QList<QImage> &tiles, QList<PaletteColor> &stgPal,
                         byte gameVer, RSDKv5::TileConfig &tConf, RSDKv1::TileConfig &tConfv1, QWidget *parent)
    : QDialog(parent), ui(new Ui::ChunkEditor), chunks(chk), tileList(tiles), chunkImgList(chunkList), tileConfig(tConf), tileConfigv1(tConfv1)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    ui->setupUi(this);

    this->setWindowTitle("Chunk Editor");

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    if (!chk)
        return;

    stagePal.clear();
    for (int c = gameVer == ENGINE_v1 ? 0 : 128; c < stgPal.count(); c++){
        stagePal.append(&stgPal[c]);
    }

    palWidget = new ChunkPalette;
    for (auto &c : stagePal)
        palWidget->palette.append(c);
    palWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    palWidget->setMaximumWidth(16 * 16 + 64);
    palWidget->setMaximumHeight(16 * 8 + 96);
    palWidget->setMinimumWidth(16 * 16 + 64);
    palWidget->setMinimumHeight(16 * 8 + 96);
    ui->paletteLayout->addWidget(palWidget, 1);

    ui->pencilTool->setDown(true);
    ui->colTool->setDown(true);

    viewer = new ChunkViewer(&selectedChunk, &selectedTile, chunks);
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    for (auto &t : tiles){
        viewer->tiles.append(&t);
        usedTiles.append(false);
    }

    for (auto c : chunks->chunks){
        for (int y = 0; y < 8; y++){
            for (int x = 0; x < 8; x++){
                int slot = c.tiles[y][x].tileIndex;
                usedTiles[slot] = true;
            }
        }
    }

    int count = 0;
    for (int t = 0; t < 0x400; t++){
        if (usedTiles[t])
            count++;
    }

    ui->usedTiles->setText(QString("Used Tiles: %1/1024").arg(count));

    if (gameVer == ENGINE_v1){
        viewer->tileConfigv1 = &tileConfigv1;
        ui->tileInfoTable->setHidden(true);
        ui->floorCheck->setHidden(true);
        ui->lWallCheck->setHidden(true);
        ui->roofCheck->setHidden(true);
        ui->rWallCheck->setHidden(true);
        ui->calcAngle->setHidden(true);
        ui->calcAngleLabel->setHidden(true);
        ui->line_3->setHidden(true);
        ui->clearPoints->setHidden(true);
        ui->angleTool->setIcon(Utils::getColoredIcon(":/icons/ic_highlight_alt_48px.svg"));
        ui->angleTool->setToolTip("Selection Tool\n\nLeft Click | Right Click - Select tile to display it's Collision Mode");
        ui->selectedTilesLabel->setText(QString("Selected Tile:     "));

        colIndex = new QComboBox();
        colIndex->addItems({"0", "1", "2", "3"});
        colIndex->setMaximumWidth(40);
        colIndex->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QLabel *colIndexLabel = new QLabel("Col Index");
        colIndexLabel->setMinimumWidth(48);
        colIndexLabel->setMaximumWidth(48);

        colMode = new QComboBox();
        colMode->addItems({"Floor", "LWall", "RWall", "Roof", "All"});
        colMode->setDisabled(true);

        colModeLabel = new QLabel("  Col Mode");
        colModeLabel->setMaximumWidth(50);
        colModeLabel->setDisabled(true);

        ui->gridLayout_2->removeWidget(ui->selectedTilesLabel);
        ui->gridLayout_2->addWidget(colIndexLabel, 0, 5);
        ui->gridLayout_2->addWidget(colIndex, 0, 6);
        ui->gridLayout_2->addWidget(ui->selectedTilesLabel, 0, 7);
        ui->gridLayout_2->addWidget(colModeLabel, 1, 7);
        ui->gridLayout_2->addWidget(colMode, 1, 8);
    } else {
        viewer->tileConfig   = &tileConfig;
    }

    ui->viewerFrame->layout()->addWidget(viewer);

    ui->chunkList->clear();
    for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); ++c) {
        auto *item = new QListWidgetItem(QString::number(c), ui->chunkList);
        item->setIcon(QPixmap::fromImage(chunkList[c]));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }

    ui->tileList->clear();
    for (int t = 0; t < tiles.count(); ++t) {
        auto *item = new QListWidgetItem(QString::number(t), ui->tileList);
        item->setIcon(QPixmap::fromImage(tiles[t]));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }

    chunkIDs.clear();
    changedChunks.clear();
    for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); ++c) {
        chunkIDs.append(c);
        changedChunks.append(false);
    }
    tileIDs.clear();
    changedTiles.clear();
    for (int c = 0; c < 0x400; ++c) {
        tileIDs.append(c);
        changedTiles.append(false);
    }

    QList<QString> solidityv3 = { "Solid (All)", "Solid (Top Only)", "Solid (All Except Top)",
                                  "Not Solid" };
    QList<QString> solidityv4 = { "Solid (All)", "Solid (Top Only, Grip)",
                                  "Solid (All Except Top)", "Not Solid",
                                  "Solid (Top Only, No Grip)" };

    ui->defSolidA->clear();
    ui->defSolidB->clear();
    ui->solidityA->clear();
    ui->solidityB->clear();

    if (gameVer == ENGINE_v4) {
        ui->defSolidA->addItems(solidityv4);
        ui->defSolidB->addItems(solidityv4);
        ui->solidityA->addItems(solidityv4);
        ui->solidityB->addItems(solidityv4);
    }
    else {
        ui->defSolidA->addItems(solidityv3);
        ui->defSolidB->addItems(solidityv3);
        ui->solidityA->addItems(solidityv3);
        ui->solidityB->addItems(solidityv3);
    }
    ui->defSolidA->setCurrentIndex(3);
    ui->defSolidB->setCurrentIndex(3);

    auto chunkRowChanged = [this](int c) {
        ui->flipX->setDisabled(c == -1);
        ui->flipY->setDisabled(c == -1);
        ui->visualPlane->setDisabled(c == -1);
        ui->tileIndex->setDisabled(c == -1);
        ui->solidityA->setDisabled(c == -1);
        ui->solidityB->setDisabled(c == -1);

        selectedChunk = c;

        viewer->offset.x = 0;
        viewer->offset.y = 0;

        if (c == -1)
            return;

        ui->flipX->blockSignals(true);
        ui->flipY->blockSignals(true);
        ui->visualPlane->blockSignals(true);
        ui->tileIndex->blockSignals(true);
        ui->solidityA->blockSignals(true);
        ui->solidityB->blockSignals(true);

        ui->flipX->setChecked(Utils::getBit(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].direction, 0));
        ui->flipY->setChecked(Utils::getBit(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].direction, 1));

        ui->visualPlane->setCurrentIndex(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].visualPlane);

        ui->tileIndex->setValue(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].tileIndex);

        ui->solidityA->setCurrentIndex(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].solidityA);
        ui->solidityB->setCurrentIndex(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].solidityB);

        ui->flipX->blockSignals(false);
        ui->flipY->blockSignals(false);
        ui->visualPlane->blockSignals(false);
        ui->tileIndex->blockSignals(false);
        ui->solidityA->blockSignals(false);
        ui->solidityB->blockSignals(false);

        viewer->viewerTiles.clear();
        for(int y = 0; y < 8; ++y){
            for(int x = 0; x < 8; ++x){
                viewer->viewerTiles.append(chunks->chunks[selectedChunk].tiles[y][x].tileIndex);
            }
        }
        viewer->highlightTile = Vector2<int>(-1,-1);
        viewer->angleTiles.clear();
        ui->tileInfoTable->blockSignals(true);
        ui->tileInfoTable->setRowCount(0);
        ui->tileInfoTable->blockSignals(false);
        viewer->repaint();
    };
    chunkRowChanged(0);
    connect(ui->chunkList, &QListWidget::currentRowChanged, chunkRowChanged);

    connect(ui->copyPlaneCol, &QPushButton::clicked, [=]{
        CopyPlane *sel = new CopyPlane(this);
        if (sel->exec() == QDialog::Accepted) {
            if (sel->copyTilePlanes){
                if (sel->applyAllTiles){
                    if (gameVer == ENGINE_v1){ for (int i = 0; i < 0x400; ++i) {
                            tileConfigv1.collisionPaths[1][i] = tileConfigv1.collisionPaths[0][i]; }; }
                    else{ for (int i = 0; i < 0x400; ++i) {
                            tileConfig.collisionPaths[1][i] = tileConfig.collisionPaths[0][i]; }; }
                } else {
                    for (int i = 0; i < viewer->viewerTiles.count(); i++){
                        int t = viewer->viewerTiles[i];
                        if (gameVer == ENGINE_v1)
                            tileConfigv1.collisionPaths[1][t] = tileConfigv1.collisionPaths[0][t];
                        else
                            tileConfig.collisionPaths[1][t] = tileConfig.collisionPaths[0][t];
                    }
                }
            }

            if (sel->copyChunkPlane){
                if (sel->applyAllTiles){
                    for (int i = 0; i < 0x200; ++i) {
                        for(int y = 0; y < 8; ++y){
                            for(int x = 0; x < 8; ++x){
                                chunks->chunks[i].tiles[y][x].solidityB = chunks->chunks[i].tiles[y][x].solidityA;
                            }
                        }
                    };
                } else {
                    for(int y = 0; y < 8; ++y){
                        for(int x = 0; x < 8; ++x){
                            chunks->chunks[selectedChunk].tiles[y][x].solidityB = chunks->chunks[selectedChunk].tiles[y][x].solidityA;
                        }
                    }
                }
            }
        }
    });

    connect(ui->importPal, &QPushButton::clicked, [=] {
        QStringList types        = { "Adobe Color Table Palettes (*.act)",
            "Image Files (*.gif *.png *.bmp)",};
        QFileDialog filedialog(this, tr("Import Palette"), "",
                               tr(types.join(";;").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString fileName = filedialog.selectedFiles()[0];
            QList<PaletteColor> newPal;
            for (int c = gameVer == ENGINE_v1 ? 0 : 128; c < stgPal.count(); c++){
                newPal.append(PaletteColor(stgPal[c]));
            }
            switch (types.indexOf(filedialog.selectedNameFilter())) {
                case PALTYPE_ACT: { //.act
                    QList<PaletteColor> pal;
                    Reader reader(fileName);
                    pal.clear();
                    while (!reader.isEOF() && pal.count() < 256) {
                        PaletteColor clr;
                        clr.read(reader);
                        pal.append(clr);
                    }
                    importFile = new PaletteImport(pal, newPal, false);
                    if (importFile->exec() == QDialog::Accepted) {
                        for (int p = 128; p < 256; p++){
                            stagePal[p] = &newPal[p];
                        }
                    };
                    importFile = nullptr;
                    break;
                }

                case PALTYPE_IMAGE: { // .gif, .png, .bmp
                    QImage img;
                    if (fileName.endsWith(".gif"))
                    {
                        QGifImage gif(fileName);
                        img = gif.frame(0);
                    }
                    else
                        img.load(fileName);

                    if (img.format() != QImage::Format_Indexed8)
                    {
                        QMessageBox::critical(this, "Palette Import Error", "No palette found in image!\nPalettes can only be imported from indexed images.");
                        return;
                    }

                    QList<PaletteColor> pal;
                    pal.clear();
                    for (auto& c : img.colorTable())
                        pal.append(PaletteColor(c));

                    importFile = new PaletteImport(pal, newPal, false);
                    if (importFile->exec() == QDialog::Accepted) {
                        for (int p = 128; p < 256; p++){
                            stagePal[p] = &newPal[p];
                        }
                    };
                    break;
                }
            }
        }
    });

    connect(ui->exportPal, &QPushButton::clicked, [=] {
        QFileDialog filedialog(this, tr("Export Palette"), "",
                               tr("Adobe Color Table Palettes (*.act);;Replace GIF Palette (*.gif)"));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString filepath = filedialog.selectedFiles()[0];

            if (filepath.endsWith(".gif"))
            {
                if (!QFile::exists(filepath))
                {
                    QMessageBox::critical(this, "Palette Export Error", "File not found!\nPlease select the existing GIF sheet you wish to save the palette to.");
                    return;
                }

                FormatHelpers::Gif gif;
                gif.read(filepath);

                for (int i = 0; i < stagePal.count() && i < 256; i++)
                    gif.palette[i] = stagePal[i]->toQColor().rgb();

                gif.write(filepath);
            }
            else
            {
                if (!CheckOverwrite(filepath, ".act", this))
                    return;

                Writer writer(filepath);
                for (auto &c : stagePal) c->write(writer);
                writer.flush();
            }
        }
    });

    connect(ui->tileList, &QListWidget::itemSelectionChanged, [this]{
        ui->tileList->blockSignals(true);
        int c = ui->tileList->currentRow();
        auto item = ui->tileList->item(c);
        ui->tileInfoTable->blockSignals(true);
        ui->tileInfoTable->setRowCount(0);
        ui->tileInfoTable->blockSignals(false);
        if (viewer->tileDrawMode == FORMAT_TILES) {
            bool isSelected = ui->tileList->selectedItems().indexOf(item) != -1;
            item->setSelected(isSelected);

            viewer->viewerTiles.clear();
            for (auto tile : ui->tileList->selectedItems())
                viewer->viewerTiles.append(tile->listWidget()->row(tile));
            selectedDrawTile = c;
        }
        else {
            selectedDrawTile = c;
            ui->tileList->clearSelection();
        };
        ui->tileList->blockSignals(false);
        viewer->repaint();
    });

    auto tileChangedCB = [this] {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        ui->flipX->blockSignals(true);
        ui->flipY->blockSignals(true);
        ui->visualPlane->blockSignals(true);
        ui->tileIndex->blockSignals(true);
        ui->solidityA->blockSignals(true);
        ui->solidityB->blockSignals(true);

        auto tile = chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x];
        ui->flipX->setChecked(Utils::getBit(tile.direction, 0));
        ui->flipY->setChecked(Utils::getBit(tile.direction, 1));

        ui->visualPlane->setCurrentIndex(tile.visualPlane);

        ui->tileIndex->setValue(tile.tileIndex);

        ui->solidityA->setCurrentIndex(tile.solidityA);
        ui->solidityB->setCurrentIndex(tile.solidityB);

        ui->flipX->blockSignals(false);
        ui->flipY->blockSignals(false);
        ui->visualPlane->blockSignals(false);
        ui->tileIndex->blockSignals(false);
        ui->solidityA->blockSignals(false);
        ui->solidityB->blockSignals(false);
    };

    QPixmap clrIcon(44,44);
    clrIcon.fill(stgPal[0].toQColor());
    ui->bgClrBtn->setIcon(clrIcon);

    connect(ui->bgClrBtn, &QToolButton::clicked, [=, &stgPal, &chunkList] {
        RSDKColorDialog *dlg = new RSDKColorDialog((PaletteColor)tileList[0].color(0), this);
        dlg->show();
        if (dlg->exec() == QDialog::Accepted) {
            stgPal[0].r = dlg->color().r;
            stgPal[0].g = dlg->color().g;
            stgPal[0].b = dlg->color().b;
            for (int t = 0; t < tileList.count(); ++t) {
                tileList[t].setColor(0, qRgb(dlg->color().r, dlg->color().g, dlg->color().b));
                viewer->tiles[t]->setColor(0, qRgb(dlg->color().r, dlg->color().g, dlg->color().b));
                ui->tileList->item(t)->setIcon(QPixmap::fromImage(tileList[t]));
            }
            ui->tileList->update();

            for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); ++c) {
                QImage img = chunkList[c];
                QPainter p(&img);
                FormatHelpers::Chunks::Chunk chk = chunks->chunks[c];
                for (int h = 0; h < 0x8; ++h) {
                    for (int w = 0; w < 0x8; ++w) {
                        Vector2<bool> flip;
                        flip.x = (chk.tiles[h][w].direction & 1) == 1;
                        flip.y = (chk.tiles[h][w].direction & 2) == 2;

                        p.drawImage(QRectF(w * 0x10, h * 0x10, 0x10, 0x10), tileList[chk.tiles[h][w].tileIndex].mirrored(flip.x, flip.y));
                    }
                }
                chunkList[c] = img;
                ui->chunkList->item(c)->setIcon(QPixmap::fromImage(chunkList[c]));
            }
            ui->chunkList->update();

            if (gameVer == ENGINE_v1){
                palWidget->palette[0] = &stgPal[0];
            }
            QPixmap clrIcon(44,44);
            clrIcon.fill(tileList[0].color(0));

            ui->bgClrBtn->setIcon(clrIcon);
            viewer->update();
        }

    });

    connect(viewer, &ChunkViewer::setColor, [=] (int c){
        palWidget->selection = c;
        if (gameVer != ENGINE_v1)
            palWidget->selection -= 128;
        palWidget->update();
    });

    connect(palWidget, &ChunkPalette::setColor, [=] (int c){
        viewer->selColor = c;
        if (gameVer != ENGINE_v1)
            viewer->selColor += 128;
    });

    connect(palWidget, &ChunkPalette::colorChange, [this, &stgPal, gameVer, &chunkList] (PaletteColor clr){
        stgPal[viewer->selColor].r = clr.r;
        stgPal[viewer->selColor].g = clr.g;
        stgPal[viewer->selColor].b = clr.b;
        for (int t = 0; t < tileList.count(); ++t) {
            tileList[t].setColor(viewer->selColor, qRgb(clr.r, clr.g, clr.b));
            viewer->tiles[t]->setColor(viewer->selColor, qRgb(clr.r, clr.g, clr.b));
            ui->tileList->item(t)->setIcon(QPixmap::fromImage(tileList[t]));
        }
        ui->tileList->update();

        for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); ++c) {
            QImage img = chunkList[c];
            QPainter p(&img);
            FormatHelpers::Chunks::Chunk chk = chunks->chunks[c];
            for (int h = 0; h < 0x8; ++h) {
                for (int w = 0; w < 0x8; ++w) {
                    Vector2<bool> flip;
                    flip.x = (chk.tiles[h][w].direction & 1) == 1;
                    flip.y = (chk.tiles[h][w].direction & 2) == 2;

                    p.drawImage(QRectF(w * 0x10, h * 0x10, 0x10, 0x10), tileList[chk.tiles[h][w].tileIndex].mirrored(flip.x, flip.y));
                }
            }
            chunkList[c] = img;
            ui->chunkList->item(c)->setIcon(QPixmap::fromImage(chunkList[c]));
        }
        if (gameVer == ENGINE_v1 && viewer->selColor == 0){
            QPixmap clrIcon(44,44);
            clrIcon.fill(tileList[0].color(0));
            ui->bgClrBtn->setIcon(clrIcon);
        }
        ui->chunkList->update();
    });

    connect(viewer, &ChunkViewer::updateLists, [this, &chunkList, gameVer] {
        for (auto i : viewer->viewerTiles){
            ui->tileList->item(i)->setIcon(QPixmap::fromImage(*viewer->tiles[i]));
            ui->tileList->update();
        }

        // getImage
        for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); ++c) {
            QImage *img = &chunkList[c];
            bool swap = false;
            FormatHelpers::Chunks::Chunk chk = chunks->chunks[c];
            for (int h = 0; h < 0x8; ++h) {
                for (int w = 0; w < 0x8; ++w) {
                    if (viewer->viewerTiles.contains(chk.tiles[h][w].tileIndex)){
                        swap = true;
                        Vector2<bool> flip;
                        flip.x = (chk.tiles[h][w].direction & 1) == 1;
                        flip.y = (chk.tiles[h][w].direction & 2) == 2;

                        for (int y = 0; y < 0x10; ++y) {
                            for (int x = 0; x < 0x10; ++x) {
                                int xpos = x;
                                int ypos = y;
                                if (flip.y)
                                    ypos = 0x0F - y;
                                if (flip.x)
                                    xpos = 0x0F - x;

                                img->setPixelColor((w * 0x10) + x, (h * 0x10) + y,
                                                  viewer->tiles[chk.tiles[h][w].tileIndex]->pixelColor(xpos, ypos).rgb());
                            }
                        }
                    }
                }
            }
            if (swap)
                ui->chunkList->item(c)->setIcon(QPixmap::fromImage(chunkList[c]));
        }
        ui->chunkList->update();
    });

    connect(viewer, &ChunkViewer::updateColTilev1, [this](int tile){
        byte colPlane = ui->colPlaneA->isChecked() ? 0 : 1;
        ui->selectedTilesLabel->setText(QString("Tile Selected %1").arg(tile));
        colMode->setCurrentIndex(tileConfigv1.collisionPaths[colPlane][tile].collisionMode);
    });

    connect(viewer, &ChunkViewer::updateAngles, [this](QSet<int> tileIndexes){
        ui->tileInfoTable->blockSignals(true);
        ui->tileInfoTable->setRowCount(0);
        ui->tileInfoTable->setUpdatesEnabled(false);
        int i = 0;
        for (auto t : tileIndexes){
            ui->tileInfoTable->insertRow(ui->tileInfoTable->rowCount());
            angleTile = t;
            byte colPlane = ui->colPlaneA->isChecked() ? 0 : 1;
            auto tile = tileConfig.collisionPaths[colPlane][angleTile];

            QTableWidgetItem *index = new QTableWidgetItem(QString::number(angleTile));
            index->setFlags(Qt::ItemIsSelectable);
            ui->tileInfoTable->setItem(i, 0, index);

            QTableWidgetItem *floor = new QTableWidgetItem(QString::number(tile.floorAngle));
            floor->setData(Qt::DisplayRole, QString::number(tile.floorAngle, 16));
            floor->setFlags(floor->flags() | Qt::ItemIsEditable);
            ui->tileInfoTable->setItem(i, 1, floor);

            QTableWidgetItem *lWall = new QTableWidgetItem(QString::number(tile.lWallAngle));
            lWall->setData(Qt::DisplayRole, QString::number(tile.lWallAngle, 16));
            lWall->setFlags(lWall->flags() | Qt::ItemIsEditable);
            ui->tileInfoTable->setItem(i, 2, lWall);

            QTableWidgetItem *roof = new QTableWidgetItem(QString::number(tile.roofAngle));
            roof->setData(Qt::DisplayRole, QString::number(tile.roofAngle, 16));
            roof->setFlags(roof->flags() | Qt::ItemIsEditable);
            ui->tileInfoTable->setItem(i, 3, roof);

            QTableWidgetItem *rWall = new QTableWidgetItem(QString::number(tile.rWallAngle));
            rWall->setData(Qt::DisplayRole, QString::number(tile.rWallAngle, 16));
            rWall->setFlags(rWall->flags() | Qt::ItemIsEditable);
            ui->tileInfoTable->setItem(i, 4, rWall);

            QTableWidgetItem *flags = new QTableWidgetItem(QString::number(tile.flags));
            flags->setFlags(flags->flags() | Qt::ItemIsEditable);
            ui->tileInfoTable->setItem(i, 5, flags);

            //QTableWidgetItem *direction = new QTableWidgetItem(tile.direction ? "Down" : "Up");
            //direction->setFlags(flags->flags() | Qt::ItemIsEditable);
            //ui->tileInfoTable->setItem(i, 5, direction);
            QComboBox *direction = new QComboBox();
            direction->addItem("Up");
            direction->addItem("Down");
            direction->setCurrentIndex(tile.direction);
            connect(direction, QOverload<int>::of(&QComboBox::currentIndexChanged), [=]{ emit ui->tileInfoTable->cellChanged(i, 6); });
            ui->tileInfoTable->setCellWidget(i, 6, direction);
            i++;
        }
        ui->tileInfoTable->setUpdatesEnabled(true);
        ui->tileInfoTable->blockSignals(false);
    });

    connect(ui->fillChunk, &QToolButton::pressed, [=]{
        QMessageBox::StandardButton warning = QMessageBox::question(this, "Fill with Unused Tiles",
                                                                    "This will override every tile in the currently selected chunk with tiles not found in any other chunk, proceed?\n"
                                                                    "(if all tiles are used, tile 0 will fill the tiles instead)");
        if (warning == QMessageBox::No)
            return;

        for (int t = 0; t < 0x400; t++){
            usedTiles.replace(t, false);
        }

        for (auto c : chunks->chunks){
            for (int y = 0; y < 8; y++){
                for (int x = 0; x < 8; x++){
                    int slot = c.tiles[y][x].tileIndex;
                    usedTiles[slot] = true;
                }
            }
        }
        for (int y = 0; y < 8; y++){
            for (int x = 0; x < 8; x++){
                auto &tile = chunks->chunks[selectedChunk].tiles[y][x];
                int t = usedTiles.indexOf(false);
                if (t == -1){
                    tile.tileIndex   = 0;
                } else {
                    tile.tileIndex   = t;
                    usedTiles[t]     = true;
                }
                tile.direction   = 0;
                tile.solidityA   = 3;
                tile.solidityB   = 3;
                tile.visualPlane = 0;
            }
        }

        int count = 0;
        for (int t = 0; t < 0x400; t++){
            if (usedTiles[t])
                count++;
        }
        ui->usedTiles->setText(QString("Used Tiles: %1/1024").arg(count));
        chunkImgList[selectedChunk] = chunks->chunks[selectedChunk].getImage(tiles);
        ui->chunkList->item(selectedChunk)->setIcon(QPixmap::fromImage(chunkImgList[selectedChunk]));

        ui->flipX->blockSignals(true);
        ui->flipY->blockSignals(true);
        ui->visualPlane->blockSignals(true);
        ui->tileIndex->blockSignals(true);
        ui->solidityA->blockSignals(true);
        ui->solidityB->blockSignals(true);

        ui->flipX->setChecked(Utils::getBit(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].direction, 0));
        ui->flipY->setChecked(Utils::getBit(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].direction, 1));

        ui->visualPlane->setCurrentIndex(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].visualPlane);

        ui->tileIndex->setValue(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].tileIndex);

        ui->solidityA->setCurrentIndex(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].solidityA);
        ui->solidityB->setCurrentIndex(
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].solidityB);

        ui->flipX->blockSignals(false);
        ui->flipY->blockSignals(false);
        ui->visualPlane->blockSignals(false);
        ui->tileIndex->blockSignals(false);
        ui->solidityA->blockSignals(false);
        ui->solidityB->blockSignals(false);

        viewer->update();
    });

    if (gameVer == ENGINE_v1){
        connect(colMode, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int c){
            byte colPlane = ui->colPlaneA->isChecked() ? 0 : 1;
            if (viewer->tileDrawMode == FORMAT_CHUNK){
                auto &tile = chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x];
                viewer->tileConfigv1->collisionPaths[colPlane][tile.tileIndex].collisionMode = c;
            }
        });
        connect(colIndex, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int c){
            viewer->colIndex = c;
        });
    }

    connect(ui->tileInfoTable, &QTableWidget::cellChanged, [=](int r, int c){
        int index = ui->tileInfoTable->item(r, 0)->text().toInt();
        byte colPlane = ui->colPlaneA->isChecked() ? 0 : 1;

        if (c != 6){
            int value = ui->tileInfoTable->item(r,c)->text().toInt();
            if (value > 0xFF)
                value = 0xFF;
            if (value < 0x00)
                value = 0x00;
            ui->tileInfoTable->item(r, c)->setText(QString::number(value, 16));
            switch (c){
                case 1: tileConfig.collisionPaths[colPlane][index].floorAngle = value; break;
                case 2: tileConfig.collisionPaths[colPlane][index].lWallAngle = value; break;
                case 3: tileConfig.collisionPaths[colPlane][index].roofAngle = value; break;
                case 4: tileConfig.collisionPaths[colPlane][index].rWallAngle = value; break;
                case 5: tileConfig.collisionPaths[colPlane][index].flags = value; break;
            }
        } else {
            QComboBox* comboBox = (QComboBox*)ui->tileInfoTable->cellWidget(r, 6);
            tileConfig.collisionPaths[colPlane][index].direction = comboBox->currentIndex();
        }

    });

    connect(ui->calcAngle, &QPushButton::clicked, [=]{
        QLineF line(viewer->DrawPoint1.x, viewer->DrawPoint1.y, viewer->DrawPoint2.x, viewer->DrawPoint2.y);
        if (viewer->DrawPoint1.x < 0)
            viewer->DrawPoint1.x = 0;
        if (viewer->DrawPoint1.y < 0)
            viewer->DrawPoint1.y = 0;
        if (viewer->DrawPoint1.x > viewer->DrawPoint2.x){
            qSwap(viewer->DrawPoint1.x, viewer->DrawPoint2.x);
            qSwap(viewer->DrawPoint1.y, viewer->DrawPoint2.y);
        }
        byte colPlane = ui->colPlaneA->isChecked() ? 0 : 1;
        int limit = viewer->tileDrawMode == FORMAT_CHUNK ? 64 : viewer->viewerTiles.count();
        for (int t = 0; t < limit; ++t){
            Vector2<int> start(-1, -1), end(-1, -1);
            int index = 0, tileDir = 0;
            if (viewer->tileDrawMode == FORMAT_CHUNK){
                auto &tile = chunks->chunks[selectedChunk].tiles[t / 8][t % 8];
                index   = tile.tileIndex;
                tileDir = tile.direction;
            } else{
                index   = viewer->viewerTiles[t];
            }
            RSDKv5::TileConfig::CollisionMask maskCopy = tileConfig.collisionPaths[colPlane][index];
            float angleF = 0, angleR = 0;
            if (viewer->usingAngleLine){
                QRectF curBlock((t % viewer->rowSize) * 16, (t / viewer->rowSize) * 16, 16, 16);

                bool valid = false;
                // check if the tile should be considered
                // first if either point of the line is inside the tile
                if (curBlock.contains(viewer->DrawPoint1.x, viewer->DrawPoint1.y) || curBlock.contains(viewer->DrawPoint2.x, viewer->DrawPoint2.y)){
                    valid = true;
                } else { // otherwise check if the line intersects
                    QPointF intersection;
                    QLineF top(curBlock.topLeft(), curBlock.topRight());
                    QLineF bottom(curBlock.bottomLeft(), curBlock.bottomRight());
                    QLineF left(curBlock.topLeft(), curBlock.bottomLeft());
                    QLineF right(curBlock.topRight(), curBlock.bottomRight());
                    if (line.intersects(top, &intersection) == QLineF::BoundedIntersection)
                        valid = true;
                    if (line.intersects(bottom, &intersection) == QLineF::BoundedIntersection)
                        valid = true;
                    if (line.intersects(left, &intersection) == QLineF::BoundedIntersection)
                        valid = true;
                    if (line.intersects(right, &intersection) == QLineF::BoundedIntersection)
                        valid = true;
                }

                if (!valid)
                    continue;
                start = viewer->DrawPoint1;
                end = viewer->DrawPoint2;
                if (viewer->keepProps && (tileDir & 1) == 1){
                    qSwap(start.y, end.y);
                }
                if (viewer->keepProps && (tileDir & 2) == 2){
                    start.y = ((start.y / viewer->rowSize) * viewer->rowSize) + (15 - start.y % 16);
                    end.y = ((end.y / viewer->rowSize) * viewer->rowSize) + (15 - end.y % 16);
                }

                angleF = atan2((end.y - start.y), (end.x - start.x));
                angleR = atan2((start.y - end.y), (start.x - end.x));
            } else {
                // only change angles for tiles that are on the angle list
                if (!viewer->angleTiles.contains(index))
                    continue;
                int step = 0;
                int curStepH = 0;
                if (ui->floorCheck->isChecked() || ui->lWallCheck->isChecked()){
                    for (int x = 0; x < 16; ++x) {
                        if (maskCopy.collision[x].solid) {
                            if (start.x == -1 && maskCopy.collision[x + 1].height != 0) {
                                start.x = x;
                                start.y = maskCopy.collision[x].height;
                            }

                            if (maskCopy.collision[x - 1].height != 0){
                                end.x = x;
                                end.y = maskCopy.collision[x].height;
                            }
                            if (curStepH == maskCopy.collision[x].height)
                                step++;
                            else if (maskCopy.collision[x].height != 0){
                                step = 0;
                                curStepH = maskCopy.collision[x].height;
                            }
                        }
                    }
                    if (start.y < end.y) {
                        if (start.y == 0 || start.y == 15) {
                            start.x = start.x - step;
                            if (start.x < 0)
                                start.x = 0;
                            start.y = maskCopy.collision[(int)start.x].height;
                        }
                    } else {
                        if (end.y == 0 || end.y == 15) {
                            end.x = end.x + step;
                            if (end.x > 15)
                                end.x = 15;
                            end.y = maskCopy.collision[(int)end.x].height;
                        }
                    }
                    angleF = atan2((end.y - start.y), (end.x - start.x));
                }
                if (ui->roofCheck->isChecked() || ui->rWallCheck->isChecked()){
                    for (int x = 0; x < 16; ++x) {
                        if (maskCopy.collision[x].solid) {
                            if (start.x == -1 && maskCopy.collision[x + 1].height != 15) {
                                start.x = x;
                                start.y = maskCopy.collision[x].height;
                            }

                            if (maskCopy.collision[x - 1].height != 15){
                                end.x = x;
                                end.y = maskCopy.collision[x].height;
                            }
                            if (curStepH == maskCopy.collision[x].height)
                                step++;
                            else if (maskCopy.collision[x].height != 15){
                                step = 0;
                                curStepH = maskCopy.collision[x].height;
                            }
                        }
                    }
                    if (end.y < start.y) {
                        if (start.y == 0 || start.y == 15) {
                            start.x = start.x - step;
                            if (start.x < 0) start.x = 0;
                            start.y = maskCopy.collision[(int)start.x].height;
                        }
                    } else {
                        if (end.y == 0 || end.y == 15) {
                            end.x = end.x + step;
                            if (end.x > 15) end.x = 15;
                            end.y = maskCopy.collision[(int)end.x].height;
                        }
                    }
                    angleR = atan2((start.y - end.y), (start.x - end.x));
                }
            }


            if (ui->floorCheck->isChecked()){
                if (!tileConfig.collisionPaths[colPlane][index].direction)
                    tileConfig.collisionPaths[colPlane][index].floorAngle = ((byte)(angleF * (256 / (2 * RSDK_PI))) & 0xFE);
                else
                    tileConfig.collisionPaths[colPlane][index].floorAngle = 0x00;
            }
            if (ui->roofCheck->isChecked()){
                if (tileConfig.collisionPaths[colPlane][index].direction)
                    tileConfig.collisionPaths[colPlane][index].roofAngle = ((byte)(angleR * (256 / (2 * RSDK_PI))) & 0xFE);
                else
                    tileConfig.collisionPaths[colPlane][index].roofAngle = 0x80;
            }

            if (ui->lWallCheck->isChecked()){
                tileConfig.collisionPaths[colPlane][index].lWallAngle = ((byte)(angleF * (256 / (2 * RSDK_PI))) & 0xFE);
                for (int c = 0; c < 16; ++c) {
                    int h                       = 0;
                    maskCopy.collision[c].solid = true;
                    while (true) {
                        if (h == 16) {
                            maskCopy.collision[c].solid = false;
                            break;
                        }

                        byte m = tileConfig.collisionPaths[colPlane][index].collision[h].height;
                        if (tileConfig.collisionPaths[colPlane][index].collision[h].solid &&
                            (tileConfig.collisionPaths[colPlane][index].direction ? c <= m : c >= m)) {
                            maskCopy.collision[c].height = h;
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
                    if (maskCopy.collision[x].solid) {
                        if (startX == -1)
                            startX = maskCopy.collision[x].height;

                        endX = maskCopy.collision[x].height;
                    }
                }

                if (startX == endX)
                    tileConfig.collisionPaths[colPlane][index].lWallAngle = 0xC0;
            }
            if (ui->rWallCheck->isChecked()){
                tileConfig.collisionPaths[colPlane][index].rWallAngle = ((byte)(angleR * (256 / (2 * RSDK_PI))) & 0xFE);

                 // RWall rotations
                for (int c = 0; c < 16; ++c) {
                    int h                       = 15;
                    maskCopy.collision[c].solid = true;
                    while (true) {
                        if (h == -1) {
                            maskCopy.collision[c].solid = false;
                            break;
                        }

                        byte m = tileConfig.collisionPaths[colPlane][index].collision[h].height;
                        if (tileConfig.collisionPaths[colPlane][index].collision[h].solid &&
                            (tileConfig.collisionPaths[colPlane][index].direction ? c <= m : c >= m)) {
                            maskCopy.collision[c].height = h;
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
                    if (maskCopy.collision[x].solid) {
                        if (startX == -1)
                            startX = maskCopy.collision[x].height;

                        endX = maskCopy.collision[x].height;
                    }
                }

                if (startX == endX)
                    tileConfig.collisionPaths[colPlane][index].rWallAngle = 0x40;
            }
        }
        emit viewer->updateAngles(viewer->angleTiles);

    });

    auto tileSrc = [this] (int c){
        viewer->tileDrawMode  = c;
        viewer->viewerTiles.clear();

        ui->rowSize->setEnabled(c);
        ui->rowSizeCol->setEnabled(c);
        ui->rowLabel->setEnabled(c);
        ui->rowLabel_2->setEnabled(c);
        ui->keepMirror->setDisabled(c);
        ui->keepMirrorCol->setDisabled(c);
        ui->ignoreTile->setDisabled(c);
        ui->ignoreTileCol->setDisabled(c);

        switch (c){
            case FORMAT_CHUNK:{
                if (ui->tileList->selectedItems().count()){
                    QListWidgetItem *tile = ui->tileList->selectedItems().last();
                    selectedDrawTile = tile->listWidget()->row(tile);
                    ui->tileList->clearSelection();
                }

                for(int y = 0; y < 8; ++y){
                    for(int x = 0; x < 8; ++x){
                        viewer->viewerTiles.append(chunks->chunks[selectedChunk].tiles[y][x].tileIndex);
                    }
                }
                break;
            }
            case FORMAT_TILES:{
                for (auto tile : ui->tileList->selectedItems()){
                    viewer->viewerTiles.append(tile->listWidget()->row(tile));
                }

                break;
            }
        }

        ui->tileSrc->blockSignals(true);
        ui->tileSrc->setCurrentIndex(c);
        ui->tileSrc->blockSignals(false);
        ui->tileColSrc->blockSignals(true);
        ui->tileColSrc->setCurrentIndex(c);
        ui->tileColSrc->blockSignals(false);

        viewer->repaint();
    };

    connect(ui->tileSrc, QOverload<int>::of(&QComboBox::currentIndexChanged), tileSrc);
    connect(ui->tileColSrc, QOverload<int>::of(&QComboBox::currentIndexChanged), tileSrc);

    auto ignoreTile = [this] (bool c){
        viewer->ignoreFirstTile = c;
        ui->ignoreTile->setChecked(c);
        ui->ignoreTileCol->setChecked(c);
    };
    connect(ui->ignoreTile, &QCheckBox::clicked, ignoreTile);
    connect(ui->ignoreTileCol, &QCheckBox::clicked, ignoreTile);

    auto keepMirror = [this] (bool c){
        viewer->keepProps = c;
        ui->keepMirror->setChecked(c);
        ui->keepMirrorCol->setChecked(c);

        viewer->update();
    };
    connect(ui->keepMirror, &QCheckBox::clicked, keepMirror);
    connect(ui->keepMirrorCol, &QCheckBox::clicked, keepMirror);

    auto rowSize = [this] (int v){
        ui->rowSize->blockSignals(true);
        ui->rowSize->setValue(v);
        ui->rowSize->blockSignals(false);
        ui->rowSizeCol->blockSignals(true);
        ui->rowSizeCol->setValue(v);
        ui->rowSizeCol->blockSignals(false);

        viewer->rowSize = v;
        viewer->update();
    };
    connect(ui->rowSize, QOverload<int>::of(&QSpinBox::valueChanged), rowSize);
    connect(ui->rowSizeCol, QOverload<int>::of(&QSpinBox::valueChanged), rowSize);

    auto resetDrawTools = [this] (int t){
        ui->pencilTool->blockSignals(true);
        ui->rectTool->blockSignals(true);
        ui->circleTool->blockSignals(true);
        ui->eraserTool->blockSignals(true);
        ui->lineTool->blockSignals(true);

        ui->pencilTool->setDown(false);
        ui->rectTool->setDown(false);
        ui->circleTool->setDown(false);
        ui->eraserTool->setDown(false);
        ui->lineTool->setDown(false);

        switch (t){
            case DRAW_PENCIL:{
                ui->pencilTool->setDown(true);
                break;
            }
            case DRAW_RECT:{
                ui->rectTool->setDown(true);
                break;
            }
            case DRAW_CIRCLE: {
                ui->circleTool->setDown(true);
                break;
            }
            case DRAW_ERASER: {
                ui->eraserTool->setDown(true);
                break;
            }
            case DRAW_LINE: {
                ui->lineTool->setDown(true);
                break;
            }
        }
        viewer->drawTool = t;

        ui->pencilTool->blockSignals(false);
        ui->rectTool->blockSignals(false);
        ui->circleTool->blockSignals(false);
        ui->eraserTool->blockSignals(false);
        ui->lineTool->blockSignals(false);
    };

    connect(ui->pencilTool, &QToolButton::clicked, [resetDrawTools]{ resetDrawTools(DRAW_PENCIL); });
    connect(ui->rectTool,   &QToolButton::clicked, [resetDrawTools]{ resetDrawTools(DRAW_RECT); });
    connect(ui->circleTool, &QToolButton::clicked, [resetDrawTools]{ resetDrawTools(DRAW_CIRCLE); });
    connect(ui->eraserTool, &QToolButton::clicked, [resetDrawTools]{ resetDrawTools(DRAW_ERASER); });
    connect(ui->lineTool,   &QToolButton::clicked, [resetDrawTools]{ resetDrawTools(DRAW_LINE); });

    connect(ui->colTool, &QToolButton::clicked, [=]{
        viewer->colTool = COL_SOLIDITY;
        ui->colTool->setDown(true);
        ui->angleTool->blockSignals(true);
        ui->angleTool->setDown(false);
        ui->angleTool->blockSignals(false);
        ui->modeControls->setText("Viewer Controls: Left Click - Set Collision Height | Right Click - Set Solidity of Collision");
        if (gameVer == ENGINE_v1){
            colMode->setDisabled(true);
            colModeLabel->setDisabled(true);
        }
    });
    connect(ui->angleTool, &QToolButton::clicked, [=]{
        viewer->colTool = COL_ANGLES;
        ui->angleTool->setDown(true);
        ui->colTool->blockSignals(true);
        ui->colTool->setDown(false);
        ui->colTool->blockSignals(false);
        if (gameVer == ENGINE_v1){
            colMode->setDisabled(false);
            colModeLabel->setDisabled(false);
            ui->modeControls->setText("Viewer Controls: Left Click | Right Click - Select tile to display it's Collision Mode");
        } else
            ui->modeControls->setText("Viewer Controls: Drag Left Click - Set angle points for the 'Calculate Angle' tool | Right Click - Add hovered Tile to the Angle List (hold Control for Multi-Selection)");
    });

    connect(ui->clearPoints, &QPushButton::pressed, [=]{
        viewer->DrawPoint1 = Vector2<int>(-1,-1);
        viewer->DrawPoint2 = Vector2<int>(-1,-1);
        viewer->usingAngleLine = false;
        viewer->repaint();
    });

    connect(viewer, &ChunkViewer::tileChanged, [=] {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        ui->flipX->blockSignals(true);
        ui->flipY->blockSignals(true);
        ui->visualPlane->blockSignals(true);
        ui->tileIndex->blockSignals(true);
        ui->solidityA->blockSignals(true);
        ui->solidityB->blockSignals(true);

        auto tile = chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x];
        ui->flipX->setChecked(Utils::getBit(tile.direction, 0));
        ui->flipY->setChecked(Utils::getBit(tile.direction, 1));

        ui->visualPlane->setCurrentIndex(tile.visualPlane);

        ui->tileIndex->setValue(tile.tileIndex);

        ui->solidityA->setCurrentIndex(tile.solidityA);
        ui->solidityB->setCurrentIndex(tile.solidityB);

        ui->flipX->blockSignals(false);
        ui->flipY->blockSignals(false);
        ui->visualPlane->blockSignals(false);
        ui->tileIndex->blockSignals(false);
        ui->solidityA->blockSignals(false);
        ui->solidityB->blockSignals(false);
    });

    connect(viewer, &ChunkViewer::tileDrawn, [this, tileChangedCB](int tileX, int tileY) {
        if (selectedChunk < 0 || tileX == -1 || tileY == -1)
            return;
        if (!ui->copyTileCheck->isChecked()){
            if (selectedDrawTile == -1)
                return;
            chunks->chunks[selectedChunk].tiles[tileY][tileX].visualPlane = defaultVisualPlane;
            chunks->chunks[selectedChunk].tiles[tileY][tileX].direction   = (byte)defaultFlip.x | ((byte)defaultFlip.y << 1);
            chunks->chunks[selectedChunk].tiles[tileY][tileX].tileIndex   = (ushort)selectedDrawTile;
            chunks->chunks[selectedChunk].tiles[tileY][tileX].solidityA   = defaultSolidA;
            chunks->chunks[selectedChunk].tiles[tileY][tileX].solidityB   = defaultSolidB;
            tileChangedCB();
        } else {
            chunks->chunks[selectedChunk].tiles[tileY][tileX].visualPlane = ui->visualPlane->currentIndex();
            chunks->chunks[selectedChunk].tiles[tileY][tileX].direction   = ui->flipX->isChecked() | ui->flipY->isChecked() << 1;
            chunks->chunks[selectedChunk].tiles[tileY][tileX].tileIndex   = ui->tileIndex->value();
            chunks->chunks[selectedChunk].tiles[tileY][tileX].solidityA   = ui->solidityA->currentIndex();
            chunks->chunks[selectedChunk].tiles[tileY][tileX].solidityB   = ui->solidityB->currentIndex();
        }

        viewer->repaint();

        chunkImgList[selectedChunk] = chunks->chunks[selectedChunk].getImage(tileList);
        ui->chunkList->item(selectedChunk)->setIcon(QPixmap::fromImage(chunkImgList[selectedChunk]));
    });

    connect(ui->flipX, &QCheckBox::toggled, [this](bool v) {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        Utils::setBit(chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].direction, v, 0);
        viewer->repaint();

        chunkImgList[selectedChunk] = chunks->chunks[selectedChunk].getImage(tileList);
        ui->chunkList->item(selectedChunk)->setIcon(QPixmap::fromImage(chunkImgList[selectedChunk]));

        changedChunks[selectedChunk] = true;
    });
    connect(ui->flipY, &QCheckBox::toggled, [this](bool v) {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        Utils::setBit(chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].direction, v, 1);
        viewer->repaint();

        chunkImgList[selectedChunk] = chunks->chunks[selectedChunk].getImage(tileList);
        ui->chunkList->item(selectedChunk)->setIcon(QPixmap::fromImage(chunkImgList[selectedChunk]));

        changedChunks[selectedChunk] = true;
    });

    connect(ui->visualPlane, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].visualPlane = (byte)v;
    });

    connect(ui->tileIndex, QOverload<int>::of(&QSpinBox::valueChanged), [this, tiles](int v) {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].tileIndex = (ushort)v;
        viewer->repaint();

        chunkImgList[selectedChunk] = chunks->chunks[selectedChunk].getImage(tiles);
        ui->chunkList->item(selectedChunk)->setIcon(QPixmap::fromImage(chunkImgList[selectedChunk]));

        changedChunks[selectedChunk] = true;
    });

    connect(ui->solidityA, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].solidityA = (byte)v;
        viewer->repaint();
    });

    connect(ui->solidityB, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].solidityB = (byte)v;
        viewer->repaint();
    });

    connect(ui->defFlipX, &QCheckBox::toggled, [this, tiles](bool v) { defaultFlip.x = v; });
    connect(ui->defFlipY, &QCheckBox::toggled, [this, tiles](bool v) { defaultFlip.y = v; });

    connect(ui->defVisPlane, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) { defaultVisualPlane = v; });

    connect(ui->defSolidA, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) { defaultSolidA = v; });

    connect(ui->defSolidB, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) { defaultSolidB = v; });

    connect(ui->colPlaneA, &QRadioButton::clicked, [=]{
        ui->colPlaneB->blockSignals(true);
        ui->colPlaneB->setChecked(false);
        ui->colPlaneB->blockSignals(false);
        viewer->colLyr = 0;
        emit viewer->updateAngles(viewer->angleTiles);
    });
    connect(ui->colPlaneB, &QRadioButton::clicked, [=]{
        ui->colPlaneA->blockSignals(true);
        ui->colPlaneA->setChecked(false);
        ui->colPlaneA->blockSignals(false);
        viewer->colLyr = 1;
        emit viewer->updateAngles(viewer->angleTiles);
    });

    auto generateCol = [=](RSDKv5::TileConfig::CollisionMask &mask, QImage tile) {
        // Check how many painted tiles are at the top and bottom to determine direction
        byte topPixels = 0, bottomPixels = 0;
        for (int x = 0; x < 15; x++){
            if (tile.pixel(x, 0) != tile.color(0))
                topPixels++;
            if (tile.pixel(x, 15) != tile.color(0))
                bottomPixels++;
        }
        if (topPixels > bottomPixels)
            mask.direction = 1;

        // Paint collision
        if (!mask.direction){
            for (int x = 0; x < 16; x++){
                mask.collision[x].solid  = false;
                mask.collision[x].height = 0;
                for (int y = 15; y > 0; y--){
                    QRgb nextColor = tile.pixel(x, y - 1);
                    if (nextColor == tile.color(0)){
                        mask.collision[x].height = y;
                        break;
                    }
                }
                if (mask.collision[x].height != 15 || (tile.pixel(x, 15) != tile.color(0)))
                    mask.collision[x].solid = true;
            }
        } else {
            for (int x = 0; x < 16; x++){
                mask.collision[x].height = 15;
                mask.collision[x].solid  = false;
                for (int y = 0; y < 15; y++){
                    QRgb nextColor = tile.pixel(x, y + 1);
                    if (nextColor == tile.color(0)){
                        mask.collision[x].height = y;
                        break;
                    }
                }
                if (mask.collision[x].height != 0 || (tile.pixel(x, 0) != tile.color(0)))
                    mask.collision[x].solid = true;
            }
        }
    };

    auto generateColv1 = [=](RSDKv1::TileConfig::CollisionMask &mask, QImage tile) {
        // Paint collision
        for (int maskIndex = 0; maskIndex < 4; maskIndex++){
            for (int x = 0; x < 16; x++){
                mask.collision[maskIndex][x].solid  = false;
                mask.collision[maskIndex][x].height = 0;
                for (int y = 15; y > 0; y--){
                    QRgb nextColor = tile.pixel(x, y - 1);
                    if (nextColor == tile.color(0)){
                        mask.collision[maskIndex][x].height = y;
                        break;
                    }
                }
                if (mask.collision[maskIndex][x].height != 15 || (tile.pixel(x,15) != tile.color(0)))
                    mask.collision[maskIndex][x].solid = true;
            }
        }
    };

    connect(ui->generateCol, &QPushButton::clicked, [=]{
        for (int t = 0; t < viewer->viewerTiles.count(); t++){
            auto tile  = &tileList[t];
            if (gameVer == ENGINE_v1) {
                RSDKv1::TileConfig::CollisionMask &mask = tileConfigv1.collisionPaths[viewer->colLyr][t];
                generateColv1(mask, *tile);
            } else {
                RSDKv5::TileConfig::CollisionMask &mask = tileConfig.collisionPaths[viewer->colLyr][t];
                generateCol(mask, *tile);
            }
        }
        viewer->repaint();
    });

    connect(ui->toolTab, &QTabWidget::currentChanged, [this](int t){
        viewer->editMode = t;
        viewer->viewerTiles.clear();
        if (viewer->editMode == EDITOR_CHUNK) {
            if (ui->tileList->selectedItems().count() > 0) {
                QListWidgetItem *tile = ui->tileList->selectedItems().last();

                ui->tileList->blockSignals(true);
                ui->tileList->clearSelection();
                ui->tileList->item(tile->listWidget()->row(tile))->setSelected(true);
                ui->tileList->blockSignals(false);

                selectedDrawTile = tile->listWidget()->row(tile);
            }

            for(int y = 0; y < 8; ++y) {
                for(int x = 0; x < 8; ++x){
                    viewer->viewerTiles.append(chunks->chunks[selectedChunk].tiles[y][x].tileIndex);
                }
            }
        } else {
            switch (viewer->tileDrawMode) {
                case FORMAT_CHUNK:{
                    for(int y = 0; y < 8; ++y) {
                        for(int x = 0; x < 8; ++x){
                            viewer->viewerTiles.append(chunks->chunks[selectedChunk].tiles[y][x].tileIndex);
                        }
                    }
                    break;
                }
                case FORMAT_TILES:{
                    for (auto tile : ui->tileList->selectedItems()){
                        viewer->viewerTiles.append(tile->listWidget()->row(tile));
                    }
                    break;
                }
            }
        }
        switch (t){
            case 0: { ui->modeControls->setText("Viewer Controls: Left Click - Draw Tile based on 'TileList Tile Properties' | Right Click - Select Tile"); break; }
            case 1: { ui->modeControls->setText("Viewer Controls: Left Click - Draws on top of tiles based on the selected Draw Tool | Right Click - Picks color on mouse position"); break; }
            case 2: {
                if (viewer->colTool == COL_SOLIDITY){
                    ui->modeControls->setText("Viewer Controls: Left Click - Set Collision Height | Right Click - Set Solidity of Collision");
                } else{
                    ui->modeControls->setText("Viewer Controls: Drag Left Click - Set angle points for the 'Calculate Angle' tool | Right Click - Add hovered Tile to the Angle List (hold Control for Multi-Selection)");
                }
                break;
            }
        }
        viewer->repaint();
    });

    auto moveChunk = [this](char translation) {
        uint c = ui->chunkList->currentRow();
        uint n = ui->chunkList->currentRow() + translation;
        if (n >= (uint)chunkImgList.count())
            return;

        auto *item = ui->chunkList->takeItem(c);
        chunkIDs.move(c, n);
        changedChunks.move(c, n);
        chunkImgList.move(c, n);
        ui->chunkList->insertItem(n, item);
        ui->chunkList->setCurrentRow(n);
    };

    connect(ui->upChunk, &QToolButton::clicked, [moveChunk] { moveChunk(-1); });

    connect(ui->downChunk, &QToolButton::clicked, [moveChunk] { moveChunk(1); });

    connect(ui->replaceChunk, &QToolButton::clicked, [=]{
        GFXReplaceTool *chunkRpl = new GFXReplaceTool(gameVer == ENGINE_v1 ? 0x100 : 0x200, chunkImgList, chk);
        if (chunkRpl->exec() == QDialog::Accepted){
            for (auto c : chunkRpl->rplIDs) {
                chk->chunks[c]  = chunkRpl->chunksDst.chunks[c];
                chunkImgList[c] = chunkRpl->chunksDst.chunks[c].getImage(tileList);
                ui->chunkList->item(c)->setIcon(QPixmap::fromImage(chunkImgList[c]));
            }

            for (int t = 0; t < 0x400; t++){
                usedTiles[t] = false;
            }

            for (auto c : chunks->chunks){
                for (int y = 0; y < 8; y++){
                    for (int x = 0; x < 8; x++){
                        int slot = c.tiles[y][x].tileIndex;
                        usedTiles[slot] = true;
                    }
                }
            }
            int count = 0;
            for (int t = 0; t < 0x400; t++){
                if (usedTiles[t])
                    count++;
            }

            ui->usedTiles->setText(QString("Used Tiles: %1/1024").arg(count));
        }
    });

    connect(ui->importChunk, &QToolButton::clicked, [=]{
        QFileDialog filedialog(this, tr("Import Image"), "", tr("GIF Images (*.gif)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);

        if (filedialog.exec() == QDialog::Accepted){
            QImage import;
            import.load(filedialog.selectedFiles()[0]);
            import = import.convertToFormat(QImage::Format_Indexed8);
            if (import.width() < 128 || import.height() < 128){
                QMessageBox::critical(this, "Import Chunk", "Image must be atleast 128x128px", QMessageBox::Ok);
                return;
            }
            QList<QRgb> importPal;
            for(int i = 0; i < 256; i++){
                if (!tileList[0].colorTable().contains(import.color(i)) && !importPal.contains(import.color(i))){
                    importPal.append(import.color(i));
                }
            }
            import = import.convertToFormat(QImage::Format_RGB888);
            QImage chunk(128, 128, QImage::Format_Indexed8);
            chunk.fill(qRgb(stgPal[0].r, stgPal[0].g, stgPal[0].b));
            QList<QImage> importChunks;
            FormatHelpers::Chunks chkImport;
            for (int w = 0; w < import.width() / 128; w++) {
                for (int h = 0; h < import.height() / 128; h++) {
                    QImage chunk = import.copy(w * 128, h * 128, 128, 128);
                    // prevent dupes
                    if (!importChunks.contains(chunk))
                        importChunks.append(chunk);
                }
            }
            ImportGFXTool *chunkImp = new ImportGFXTool(gameVer, importChunks, chunkImgList, stgPal, importPal);
            if (chunkImp->exec() == QDialog::Accepted){
                QVector<QRgb> newPalette = tileList[0].colorTable();
                if (gameVer == ENGINE_v1){
                    for (int i = 0; i < 128; ++i){
                        newPalette[i] = qRgb(chunkImp->curPalette.at(i).r, chunkImp->curPalette.at(i).g, chunkImp->curPalette.at(i).b);
                    }
                } else {
                    for (int i = 128; i < 256; ++i){
                        newPalette[i] = qRgb(chunkImp->curPalette.at(i - 128).r, chunkImp->curPalette.at(i - 128).g, chunkImp->curPalette.at(i - 128).b);
                    }
                }
                for (int c = 0; c < stagePal.count(); c++){
                    QRgb clr = newPalette[gameVer == ENGINE_v1 ? c : c + 128];
                    stagePal[c]->r = (clr >> 16) & 0xFF;
                    stagePal[c]->g = (clr >> 8) & 0xFF;
                    stagePal[c]->b = clr & 0xFF;
                }
                import = import.convertToFormat(QImage::Format_Indexed8, newPalette);

                FormatHelpers::Chunks::Chunk formatChk;
                int nextUnusedTile = usedTiles.indexOf(false);
                byte mirror  = 0;
                for (auto t : usedTiles){
                    if (t != false)
                        tileList[t] = tileList[t].convertToFormat(QImage::Format_Indexed8, newPalette);
                    else
                        tileList[t] = tileList[0].convertToFormat(QImage::Format_Indexed8, newPalette);
                }
                QList<int> rplIDs = chunkImp->rplChkIDs.values();
                for (int c = 0; c < rplIDs.count(); c++){
                    QImage chunk = chunkImgList[rplIDs[c]].convertToFormat(QImage::Format_Indexed8, newPalette);
                    for (int x = 0; x < 8; x++) {
                        for(int y = 0; y < 8; y++) {
                            QImage impTile   = chunk.copy(x * 16, y * 16, 16, 16);
                            QImage impTileX  = impTile.mirrored(true,false);
                            QImage impTileY  = impTile.mirrored(false,true);
                            QImage impTileXY = impTile.mirrored(true,true);
                            bool matchingTile = false;
                            int j = 0;
                            for (; j < 0x400; j++){
                                auto &tile = tileList[j];
                                if (impTile == tile) { matchingTile = true; mirror = 0; break; }
                                else if (impTileX  == tile) { matchingTile = true; mirror = 1; break; }
                                else if (impTileY  == tile) { matchingTile = true; mirror = 2; break; }
                                else if (impTileXY == tile) { matchingTile = true; mirror = 3; break; }
                            }
                            if (matchingTile){
                                formatChk.tiles[y][x].tileIndex = j;
                                formatChk.tiles[y][x].direction = mirror;
                            } else if (nextUnusedTile != -1) {
                                int emptyTile = usedTiles.indexOf(false);
                                usedTiles[emptyTile] = true;

                                formatChk.tiles[y][x].tileIndex = emptyTile;
                                formatChk.tiles[y][x].direction = 0;
                                tileList[emptyTile]  = impTile;
                                viewer->tiles[emptyTile] = &tileList[emptyTile];
                                if (chunkImp->generateCol){
                                    auto tile  = &tileList[emptyTile];
                                    if (gameVer == ENGINE_v1) {
                                        generateColv1(tileConfigv1.collisionPaths[0][emptyTile], *tile);
                                        generateColv1(tileConfigv1.collisionPaths[1][emptyTile], *tile);
                                    } else {
                                        generateCol(tileConfig.collisionPaths[0][emptyTile], *tile);
                                        generateCol(tileConfig.collisionPaths[1][emptyTile], *tile);
                                    }
                                } else {
                                    if (gameVer == ENGINE_v1) {
                                        RSDKv1::TileConfig::CollisionMask &mask = tileConfigv1.collisionPaths[0][emptyTile];
                                        RSDKv1::TileConfig::CollisionMask &maskB = tileConfigv1.collisionPaths[1][emptyTile];
                                        for (int maskIndex = 0; maskIndex < 4; maskIndex++){
                                            for (int m = 0; m < 16; m++){
                                                mask.collision[maskIndex][m].height  = 15;    maskB.collision[maskIndex][m].height = 15;
                                                mask.collision[maskIndex][m].solid   = false; maskB.collision[maskIndex][m].solid  = false;
                                            }
                                        }
                                    } else {
                                        RSDKv5::TileConfig::CollisionMask &mask = tileConfig.collisionPaths[0][emptyTile];
                                        RSDKv5::TileConfig::CollisionMask &maskB = tileConfig.collisionPaths[1][emptyTile];
                                        for (int m = 0; m < 16; m++){
                                            mask.collision[m].height  = 15;    maskB.collision[m].height = 15;
                                            mask.collision[m].solid   = false; maskB.collision[m].solid  = false;
                                        }
                                    }
                                }
                                ui->tileList->item(emptyTile)->setIcon(QPixmap::fromImage(impTile));
                                nextUnusedTile = usedTiles.indexOf(false);
                            } else {
                                formatChk.tiles[y][x].tileIndex = 0;
                                formatChk.tiles[y][x].direction = 0;
                            }
                        }
                    }
                    chunks->chunks[rplIDs[c]] = formatChk;
                }
                ui->tileList->update();
                for (int i = 0; i < (gameVer == ENGINE_v1 ? 0x100 : 0x200); i++){
                    chk->chunks[i] = chunks->chunks[i];
                    ui->chunkList->item(i)->setIcon(QPixmap::fromImage(chunkImgList[i]));
                }
                ui->chunkList->update();

                int count = 0;
                for (int t = 0; t < 0x400; t++){
                    if (usedTiles[t])
                        count++;
                }
                ui->usedTiles->setText(QString("Used Tiles: %1/1024").arg(count));
            }
        }
    });

    connect(ui->exportChunk, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Select folder to place images"), "", "");
        filedialog.setFileMode(QFileDialog::Directory);
        if (filedialog.exec() == QDialog::Accepted) {
            QString path = filedialog.selectedFiles()[0];
            int id       = 0;
            SetStatus("Exporting chunks as images...");
            for (int c = 0; c < 0x200; ++c) {
                chunks->chunks[c].getImage(tileList).save(QString(path + "/Chunk %1.png").arg(id++));
                SetStatusProgress(c / (float)0x200);
            }
            SetStatus(QString("Exported chunks to %1/").arg(path));
        }
    });

    connect(ui->importTiles, &QToolButton::clicked, [=]{
        QFileDialog filedialog(this, tr("Import Image"), "", tr("GIF Images (*.gif)"));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);

        if (filedialog.exec() == QDialog::Accepted){
            QImage import;
            import.load(filedialog.selectedFiles()[0]);
            import = import.convertToFormat(QImage::Format_Indexed8);
            if (import.width() < 16 || import.height() < 16){
                QMessageBox::critical(this, "Import Tiles", "Image must be atleast 16x16px", QMessageBox::Ok);
                return;
            }
            QList<QRgb> importPal;
            for(int i = 0; i < 256; i++){
                if (!tileList[0].colorTable().contains(import.color(i)) && !importPal.contains(import.color(i))){
                    importPal.append(import.color(i));
                }
            }
            import = import.convertToFormat(QImage::Format_RGB888);
            QImage tile(16, 16, QImage::Format_Indexed8);
            tile.fill(qRgb(stgPal[0].r, stgPal[0].g, stgPal[0].b));
            QList<QImage> importTiles;
            for (int w = 0; w < import.width() / 16; w++) {
                for (int h = 0; h < import.height() / 16; h++) {
                    QImage tile = import.copy(w * 16, h * 16, 16, 16);
                    // prevent dupes
                    if (!importTiles.contains(tile))
                        importTiles.append(tile);
                }
            }

            QList<QImage> tileListCopy;
            for (int t = 0; t < 0x400; t++){
                tileListCopy.append(tileList[t].convertToFormat(QImage::Format_RGB888));
            }
            ImportGFXTool *tileImp = new ImportGFXTool(gameVer, importTiles, tileListCopy, stgPal, importPal);
            if (tileImp->exec() == QDialog::Accepted){
                QVector<QRgb> newPalette = tileList[0].colorTable();
                if (gameVer == ENGINE_v1){
                    for (int i = 0; i < 128; ++i){
                        newPalette[i] = qRgb(tileImp->curPalette.at(i).r, tileImp->curPalette.at(i).g, tileImp->curPalette.at(i).b);
                    }
                } else {
                    for (int i = 128; i < 256; ++i){
                        newPalette[i] = qRgb(tileImp->curPalette.at(i - 128).r, tileImp->curPalette.at(i - 128).g, tileImp->curPalette.at(i - 128).b);
                    }
                }
                for (int c = 0; c < stagePal.count(); c++){
                    QRgb clr = newPalette[gameVer == ENGINE_v1 ? c : c + 128];
                    stagePal[c]->r = (clr >> 16) & 0xFF;
                    stagePal[c]->g = (clr >> 8) & 0xFF;
                    stagePal[c]->b = clr & 0xFF;
                }

                for (int t = 0; t < 0x400; t++){
                    tileList[t] = tileListCopy[t].convertToFormat(QImage::Format_Indexed8, newPalette);
                    viewer->tiles[t] = &tileList[t];
                    ui->tileList->item(t)->setIcon(QPixmap::fromImage(tileList[t]));
                }
                if (tileImp->generateCol){
                    for (auto t : tileImp->rplChkIDs){
                        auto tile  = &tileList[t];
                        if (gameVer == ENGINE_v1) {
                            generateColv1(tileConfigv1.collisionPaths[0][t], *tile);
                            generateColv1(tileConfigv1.collisionPaths[1][t], *tile);
                        } else {
                            generateCol(tileConfig.collisionPaths[0][t], *tile);
                            generateCol(tileConfig.collisionPaths[1][t], *tile);
                        }
                    }
                }

                for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); c++){
                    chunkImgList[c] = chunks->chunks[c].getImage(tileList);
                    ui->chunkList->item(c)->setIcon(QPixmap::fromImage(chunkImgList[c]));
                }
            }
        }
    });

    connect(ui->exportTiles, &QPushButton::clicked, [=] {
        QFileDialog filedialog(this, tr("Save Image"), "", tr(QString("GIF Images (*.gif)").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptSave);
        if (filedialog.exec() == QDialog::Accepted) {
            QString path = filedialog.selectedFiles()[0];
            if (!CheckOverwrite(path, ".gif", this))
                return;

            int rowSize = ui->rowSize->value();
            QImage exportImg = QImage(16 * rowSize, 16 * (0x400 / rowSize), QImage::Format_RGB888);
            QPainter p(&exportImg);
            int t = 0;
            for (int y = 0; y < 0x400 / rowSize; y++){
                for (int x = 0; x < rowSize; x++){
                    p.drawImage(x * 0x10, y * 0x10, tileList[t++]);
                }
            }
            p.end();
            QGifImage gif;
            gif.addFrame(exportImg.convertToFormat(QImage::Format_Indexed8, tileList[0].colorTable()));
            gif.save(path);
        }
    });

    connect(ui->replaceTiles, &QToolButton::clicked, [=]{
        GFXReplaceTool *tileRpl = new GFXReplaceTool(0x400, tileList);
        if (tileRpl->exec() == QDialog::Accepted){
            QMapIterator<int,int> c(tileRpl->srcToRplIDs);
            while (c.hasNext()){
                c.next();
                ui->tileList->item(c.key())->setIcon(QPixmap::fromImage(tileList[c.value()]));
                viewer->tiles[c.key()] = &tileList[c.value()];
                if (gameVer != ENGINE_v1){
                    tileConfig.collisionPaths[0][c.key()] = tileConfig.collisionPaths[0][c.value()];
                    tileConfig.collisionPaths[1][c.key()] = tileConfig.collisionPaths[1][c.value()];
                } else{
                    tileConfigv1.collisionPaths[0][c.key()] = tileConfigv1.collisionPaths[0][c.value()];
                    tileConfigv1.collisionPaths[1][c.key()] = tileConfigv1.collisionPaths[1][c.value()];
                }
            }

            for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); c++){
                chunkImgList[c] = chunks->chunks[c].getImage(tileList);
                ui->chunkList->item(c)->setIcon(QPixmap::fromImage(chunkImgList[c]));
            }
        }
    });

    for (QWidget *w : findChildren<QWidget *>()) {
        w->installEventFilter(this);
    }
}

ChunkEditor::~ChunkEditor() { delete ui; }

bool ChunkEditor::event(QEvent *e)
{
    switch (e->type()) {
        default: break;
        case QEvent::KeyPress: keyPressEvent((QKeyEvent *)e); break;
    }
    return QWidget::event(e);
}

bool ChunkViewer::event(QEvent *e)
{
    setMouseTracking(true);
    switch (e->type()) {
        default: break;

        case QEvent::MouseButtonPress: mousePressEvent((QMouseEvent *)e); return true;

        case QEvent::MouseMove: mouseMoveEvent((QMouseEvent *)e); return true;

        case QEvent::MouseButtonRelease: mouseReleaseEvent((QMouseEvent *)e); return true;

        case QEvent::Paint: paintEvent((QPaintEvent *)e); return true;

        case QEvent::Wheel: {
            QWheelEvent *wEvent = static_cast<QWheelEvent *>(e);
            if (wEvent->angleDelta().y() > 0 && zoom < 20){
                zoom *= 1.1f;
                offset.x += (offset.x * 1.1f) - offset.x;
                offset.y += (offset.y * 1.1f) - offset.y;
            }
            else if (wEvent->angleDelta().y() < 0 && zoom > 0.5f){
                zoom /= 1.1f;
                offset.x += (offset.x / 1.1f) - offset.x;
                offset.y += (offset.y / 1.1f) - offset.y;
            }
            // ui->zoomLabel->setText(QString("Zoom: %1%").arg(zoom * 100));
            // updateView();
            update();
            return true;
        }
    }
    return QWidget::event(e);
}

void ChunkViewer::mousePressEvent(QMouseEvent *event)
{
/*
    float w = this->width(), h = this->height();
    float originX = w / 2, originY = h / 2;
    originX -= offset.x;
    originY -= offset.y;
    PrintLog(QString("pos(%1, %2), origin(%3, %4), mousePos(%5, %6)")
                 .arg(gridPos.x)
                 .arg(gridPos.y)
                 .arg(originX)
                 .arg(originY)
                 .arg(event->pos().x())
                 .arg(event->pos().y()));
*/
    if ((event->button() & Qt::LeftButton) == Qt::LeftButton)
        mouseDownL = true;
    if ((event->button() & Qt::MiddleButton) == Qt::MiddleButton)
        mouseDownM = true;
    if ((event->button() & Qt::RightButton) == Qt::RightButton)
        mouseDownR = true;

    reference = event->pos();

    short index = -1;

    switch (editMode){
        case EDITOR_CHUNK:{
            if (mouseDownL)
                emit tileDrawn(selection->x, selection->y);
            if (mouseDownR)
                emit tileChanged();
            if (mouseDownL || mouseDownR)
                highlightTile = *selection;
            break;
        }

        case EDITOR_TILE: {
            if (mouseDownL && (drawTool == DRAW_RECT || drawTool == DRAW_CIRCLE || drawTool == DRAW_LINE))
                rect.setRect(gridPos.x, gridPos.y, gridPos.x, gridPos.y);

            if (mouseDownR && inGrid) {
                int tilePxX = gridPos.x % 16, tilePxY = gridPos.y % 16;
                if (tileDrawMode == FORMAT_CHUNK)
                    index = chunks->chunks[*cSel].tiles[selection->y][selection->x].tileIndex;
                else
                    index = viewerTiles.indexOf(selection->y * rowSize + selection->x);
                if (index == -1) break;
                QImage *tileImg = tiles.at(index);
                selColor = tileImg->pixelIndex(tilePxX, tilePxY);
                emit setColor(selColor);
            }
            break;
        }

        case EDITOR_COLLISION:{
            switch (colTool){
                case COL_SOLIDITY: {
                    if (mouseDownR && inGrid) {
                        int tilePxX = gridPos.x % 16;
                        if (tileDrawMode == FORMAT_CHUNK){
                            auto &tile = chunks->chunks[*cSel].tiles[selection->y][selection->x];
                            index = tile.tileIndex;
                            if ((tile.direction & 1) == 1 && keepProps)
                                tilePxX = 15 - tilePxX;
                        } else {
                            int pos = selection->y * rowSize + selection->x;
                            if (pos >= viewerTiles.count())
                                break;
                            index = viewerTiles[pos];
                        }
                        if (index == -1)
                            break;
                        if (tileConfig != nullptr)
                            solidityCheck = tileConfig->collisionPaths[colLyr][index].collision[tilePxX].solid == 0 ? 1 : 0;
                        else
                            solidityCheck = tileConfigv1->collisionPaths[colLyr][index].collision[colIndex][tilePxX].solid == 0 ? 1 : 0;
                    }
                    break;
                }
                case COL_ANGLES: {
                    usingAngleLine = false;
                    DrawPoint1 = Vector2<int>(-1,-1);
                    DrawPoint2 = Vector2<int>(-1,-1);

                    if (tileConfig != nullptr){
                        if (mouseDownL)
                            DrawPoint1 = gridPos;

                        if (mouseDownR && inGrid){
                            if (tileDrawMode == FORMAT_CHUNK)
                                index = chunks->chunks[*cSel].tiles[selection->y][selection->x].tileIndex;
                            else{
                                int pos = selection->y * rowSize + selection->x;
                                if (pos >= viewerTiles.count())
                                    break;
                                index = viewerTiles[pos];
                            }
                            if (index == -1)
                                break;

                            if ((event->modifiers() & Qt::ControlModifier) != Qt::ControlModifier)
                                angleTiles.clear();

                            if (!ignoreFirstTile || (ignoreFirstTile && index))
                                angleTiles.insert(index);

                            emit updateAngles(angleTiles);
                        }
                    } else {
                        if ((mouseDownR || mouseDownL) && inGrid){
                            if (tileDrawMode == FORMAT_CHUNK)
                                index = chunks->chunks[*cSel].tiles[selection->y][selection->x].tileIndex;
                            else{
                                int pos = selection->y * rowSize + selection->x;
                                if (pos >= viewerTiles.count())
                                    break;
                                index = viewerTiles[pos];
                            }
                            if (index == -1)
                                break;
                            emit updateColTilev1(index);
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
    repaint();
}

void ChunkViewer::mouseMoveEvent(QMouseEvent *event)
{
    float w = this->width(), h = this->height();
    float originX = w / 2, originY = h / 2;
    originX -= offset.x;
    originY -= offset.y;

    gridPos.x = event->pos().x() - originX;
    gridPos.y = event->pos().y() - originY;

    gridPos.x = (gridPos.x * (1.0f / zoom) + 256) / scale;
    gridPos.y = (gridPos.y * (1.0f / zoom) + 256) / scale;

    if (mouseDownM) {
        offset.x -= event->pos().x() - reference.x();
        offset.y -= event->pos().y() - reference.y();
        reference = event->pos();

        repaint();
    }

    int TilePxX, TilePxY;

    selection->x = gridPos.x / 16; selection->y = gridPos.y / 16;

    if (selection->x >= rowSize || selection->x < 0 || (selection->y >= viewerTiles.count() / rowSize
                                                     && selection->x >= viewerTiles.count() % rowSize))
        selection->x = -1;

    if (selection->y > viewerTiles.count() / rowSize || selection->y < 0 || (selection->y >= viewerTiles.count() / rowSize
                                                                          && selection->x >= viewerTiles.count() % rowSize))
        selection->y = -1;

    TilePxX = gridPos.x % 16;
    TilePxY = gridPos.y % 16;
    inGrid = (selection->x != -1 && selection->y != -1);
    short index = -1;

    switch (editMode){
        case EDITOR_CHUNK:{
            if (mouseDownR)
                emit tileChanged();

            if (mouseDownL)
                emit tileDrawn(selection->x, selection->y);
            if (mouseDownL || mouseDownR)
                highlightTile = *selection;
            break;
        }
        case EDITOR_TILE: {
            if (drawTool == DRAW_RECT || drawTool == DRAW_CIRCLE || drawTool == DRAW_LINE){
                rect.setWidth(gridPos.x);
                rect.setHeight(gridPos.y);
                break;
            }

            if (inGrid){
                switch (tileDrawMode){
                    case FORMAT_CHUNK: {
                        auto &tile = chunks->chunks[*cSel].tiles[selection->y][selection->x];
                        index = tile.tileIndex;
                        bool fx = keepProps ? ((tile.direction & 1) == 1) : false;
                        bool fy = keepProps ? ((tile.direction & 2) == 2) : false;

                        if (fx)
                            TilePxX = 15 - TilePxX;
                        if (fy)
                            TilePxY = 15 - TilePxY;
                        break;
                    }
                    case FORMAT_TILES:{
                        int pos = selection->y * rowSize + selection->x;
                        if (pos >= viewerTiles.count())
                            break;
                        index = viewerTiles[pos];
                        break;
                    }
                }

                if (index == -1)
                    break;

                QImage *tileImg = tiles.at(index);

                if (mouseDownL){
                    switch (drawTool){
                        case DRAW_PENCIL:{
                            tileImg->setPixel(TilePxX, TilePxY, selColor);
                            break;
                        }
                        case DRAW_ERASER:{
                            tileImg->setPixel(TilePxX, TilePxY, 0);
                            break;
                        }
                    }
                } else if (mouseDownR)
                    emit setColor(tileImg->pixelIndex(TilePxX, TilePxY));
            }
            break;
        }
        case EDITOR_COLLISION: {
            if (mouseDownL && colTool == COL_ANGLES){
                if (tileConfig == nullptr)
                    break;
                DrawPoint2.x = gridPos.x;
                DrawPoint2.y = gridPos.y;
                break;
            }

            if (inGrid){
                switch (tileDrawMode){
                    case FORMAT_CHUNK: {
                        auto &tile = chunks->chunks[*cSel].tiles[selection->y][selection->x];
                        index = tile.tileIndex;
                        bool fx = keepProps ? ((tile.direction & 1) == 1) : false;
                        bool fy = keepProps ? ((tile.direction & 2) == 2) : false;

                        if (fx) TilePxX = 15 - TilePxX;
                        if (fy) TilePxY = 15 - TilePxY;
                        break;
                    }
                    case FORMAT_TILES:{
                        int pos = selection->y * rowSize + selection->x;
                        if (pos >= viewerTiles.count())
                            break;
                        index = viewerTiles[pos];
                        break;
                    }
                }
                if (index == -1)
                    break;

                if (colTool == COL_SOLIDITY && (!ignoreFirstTile || (ignoreFirstTile && index))){
                    if (mouseDownL) {
                            if (tileConfig != nullptr)
                                tileConfig->collisionPaths[colLyr][index].collision[TilePxX].height = TilePxY;
                            else
                                tileConfigv1->collisionPaths[colLyr][index].collision[colIndex][TilePxX].height = TilePxY;
                    } else if (mouseDownR){
                        if (tileConfig != nullptr)
                            tileConfig->collisionPaths[colLyr][index].collision[TilePxX].solid = solidityCheck;
                        else
                            tileConfigv1->collisionPaths[colLyr][index].collision[colIndex][TilePxX].solid = solidityCheck;
                    }
                }
            }
            break;

        }
    }
    update();
}

void ChunkViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (editMode != EDITOR_CHUNK && (mouseDownL || mouseDownR)){
        if (mouseDownL){
            if (editMode == EDITOR_TILE && drawTool != DRAW_PENCIL && drawTool != DRAW_ERASER){

                rect.setWidth(gridPos.x);
                rect.setHeight(gridPos.y);

                QImage shape(16 * rowSize * scale, 16 * rowSize * scale, QImage::Format_ARGB32);
                shape.fill(Qt::transparent);
                QPainter p(&shape);
                p.setPen(QPen(QColor(tiles.at(0)->color(selColor)),1));
                p.setBrush(QColor(tiles.at(0)->color(selColor)));
                switch (drawTool) {
                    case DRAW_RECT: p.drawRect(rect.x(), rect.y(), (rect.width() - rect.x()), (rect.height() - rect.y())); break;
                    case DRAW_CIRCLE: p.drawEllipse(rect.x(), rect.y(), (rect.width() - rect.x()), (rect.height() - rect.y())); break;
                    case DRAW_LINE: p.drawLine(rect.x(), rect.y(), rect.width(), rect.height()); break;
                }

                if (tileDrawMode == FORMAT_CHUNK){
                    for (int x = 0; x < 8; ++x){
                        for (int y = 0; y < 8; ++y){
                            auto &tile = chunks->chunks[*cSel].tiles[y][x];
                            bool fx = keepProps ? ((tile.direction & 1) == 1) : false;
                            bool fy = keepProps ? ((tile.direction & 2) == 2) : false;
                            QImage *tileImg = tiles.at(tile.tileIndex);
                            QImage area  = shape.copy(x * 16, y * 16, 16, 16);
                            if (!ignoreFirstTile || (ignoreFirstTile && tile.tileIndex)){
                                for (int ty = 0; ty < 16; ++ty){
                                    for (int tx = 0; tx < 16; ++tx){
                                        if (area.pixelColor(tx, ty) == tileImg->color(selColor))
                                            tileImg->setPixel(fx ? 15 - tx : tx, fy ? 15 - ty : ty, selColor);
                                    }
                                }
                            }
                        }
                    }
                } else{
                    for (int i = 0; i < viewerTiles.count(); ++i){
                        int x = i % rowSize, y = i / rowSize;
                        QImage *tile = tiles.at(viewerTiles[i]);
                        QImage area  = shape.copy(x * 16, y * 16, 16, 16);
                        if (!ignoreFirstTile || (ignoreFirstTile && viewerTiles[i] != 0)){
                            for (int ty = 0; ty < 16; ++ty){
                                for (int tx = 0; tx < 16; ++tx){
                                    if (area.pixelColor(tx, ty) == tile->color(selColor)){
                                        tile->setPixel(tx, ty, selColor);
                                    }
                                }
                            }
                        }
                    }
                }
                repaint();
            } else {
                if (colTool == COL_ANGLES){
                    DrawPoint2.x = gridPos.x;
                    DrawPoint2.y = gridPos.y;
                    if (DrawPoint1.x < 0) DrawPoint1.x = 0;
                    if (DrawPoint1.y < 0) DrawPoint1.y = 0;

                    if (DrawPoint1.x > DrawPoint2.x){
                        qSwap(DrawPoint1.x, DrawPoint2.x);
                        qSwap(DrawPoint1.y, DrawPoint2.y);
                    }
                    usingAngleLine = true;
                }
            }
        }
        emit updateLists();
    }

    if ((event->button() & Qt::LeftButton) == Qt::LeftButton)
        mouseDownL = false;
    if ((event->button() & Qt::MiddleButton) == Qt::MiddleButton)
        mouseDownM = false;
    if ((event->button() & Qt::RightButton) == Qt::RightButton)
        mouseDownR = false;
}

void ChunkViewer::paintEvent(QPaintEvent *event)
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
    originX -= (16 * 16);
    originY -= (16 * 16);
    switch (editMode){
        case EDITOR_CHUNK: {
            const QBrush brush = p.brush();
            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    auto &tile = chunks->chunks[*cSel].tiles[y][x];

                    bool fx = (tile.direction & 1) == 1, fy = (tile.direction & 2) == 2;
                    p.drawImage(QPointF(originX + x * (0x10 * scale), originY + y * (0x10 * scale)),
                                tiles.at(tile.tileIndex)->mirrored(fx, fy).scaled((0x10 * scale), (0x10 * scale)));

                    if (y == highlightTile.y && x == highlightTile.x) {
                        p.setBrush(qApp->palette().highlight());
                        p.setOpacity(0.5);
                        p.drawRect(QRectF(originX + x * (0x10 * scale), originY + y * (0x10 * scale), (0x10 * scale), (0x10 * scale)));
                        p.setBrush(brush);
                        p.setOpacity(1.0);
                    }
                }
            }
            break;
        }
        case EDITOR_TILE: {
            switch (tileDrawMode){
                case FORMAT_CHUNK: {
                    for (int y = 0; y < 8; ++y) {
                        for (int x = 0; x < 8; ++x) {
                            auto &tile = chunks->chunks[*cSel].tiles[y][x];
                            p.setOpacity(0.25);
                            if (selection->y != -1 && selection->x != -1 && chunks->chunks[*cSel].tiles[selection->y][selection->x].tileIndex == tile.tileIndex)
                                p.setOpacity(1.0);

                            bool fx = keepProps ? (tile.direction & 1) == 1 : false;
                            bool fy = keepProps ? (tile.direction & 2) == 2 : false;
                            QPointF coords = QPointF(originX + x * (0x10 * scale), originY + y * (0x10 * scale));
                            p.drawImage(coords, tiles.at(tile.tileIndex)->mirrored(fx,fy).scaled((0x10 * scale),(0x10 * scale)));
                        }
                    }
                    break;
                }
                case FORMAT_TILES: {
                    for (int index = 0; index < viewerTiles.count(); index++){
                        int t = viewerTiles[index];
                        p.setOpacity(0.25);
                        if (selection->y != -1 && selection->x != -1 && selection->y * rowSize + selection->x == index)
                            p.setOpacity(1.0);
                        QPointF coords = QPointF(originX + ((index % rowSize) * (0x10 * scale)), originY + (index / rowSize) * (0x10 * scale));
                        p.drawImage(coords, tiles.at(t)->scaled((0x10 * scale),(0x10 * scale)));
                    }
                    break;
                }
            }

            if (mouseDownL && drawTool != DRAW_PENCIL && drawTool != DRAW_ERASER){
                QImage shape(16 * rowSize, 16 * rowSize, QImage::Format_ARGB32);
                shape.fill(Qt::transparent);
                QPainter s(&shape);
                s.setPen(QPen(QColor(tiles.at(0)->color(selColor)),1));
                s.setBrush(QColor(tiles.at(0)->color(selColor)));
                switch (drawTool){
                    case DRAW_RECT:
                        s.drawRect(int(rect.x()), int(rect.y()), int((rect.width() - rect.x())), int((rect.height() - rect.y()))); break;
                    case DRAW_CIRCLE:
                        s.drawEllipse(int(rect.x()), int(rect.y()), int((rect.width() - rect.x())), int((rect.height() - rect.y()))); break;
                    case DRAW_LINE:
                        s.drawLine(rect.x(), rect.y(), rect.width(), rect.height()); break;
                }
                s.end();
                shape = shape.scaled(0x10 * rowSize * scale, 0x10 * rowSize * scale);
                p.setOpacity(1);
                p.drawImage(originX,originY, shape);
            }
            break;
        }

        case EDITOR_COLLISION:{
            switch (tileDrawMode){
                case FORMAT_CHUNK:{
                    QRgb colors[5] = {0xFFE0E000, 0xFFE08000, 0xFFE0E0E0, 0x80E0E000, 0xFF0000E0};
                    for (int y = 0; y < 8; ++y) {
                        for (int x = 0; x < 8; ++x) {
                            p.setPen(QColor(0xFF, 0xFF, 0x00));
                            auto &tile = chunks->chunks[*cSel].tiles[y][x];

                            p.setOpacity(0.25);
                            if (selection->y != -1 && selection->x != -1 && chunks->chunks[*cSel].tiles[selection->y][selection->x].tileIndex == tile.tileIndex)
                                p.setOpacity(1.0);

                            QImage tileImg = tiles[tile.tileIndex]->copy().convertToFormat(QImage::Format_RGB888);
                            bool fx = keepProps ? (tile.direction & 1) == 1 : false;
                            bool fy = keepProps ? (tile.direction & 2) == 2 : false;
                            if (tileConfig != nullptr){
                                auto mask = tileConfig->collisionPaths[colLyr][tile.tileIndex];
                                for (int py = 0; py < 16; ++py) {
                                    for (int px = 0; px < 16; ++px) {
                                        int drawX = fx ? 15 - px : px;
                                        if ((mask.direction ? mask.collision[drawX].height >= py : mask.collision[drawX].height <= py)){
                                            if (mask.collision[drawX].solid){
                                                if (colLyr == 0)
                                                    tileImg.setPixel(drawX, py, colors[tile.solidityA]);
                                                else
                                                    tileImg.setPixel(drawX, py, colors[tile.solidityB]);
                                            } else {
                                                QRgb pc = QColor(tileImg.pixel(drawX,py)).darker(255).rgba();
                                                tileImg.setPixel(drawX, py, pc);
                                            }
                                        }
                                    }
                                }
                            } else{
                                auto mask = tileConfigv1->collisionPaths[colLyr][tile.tileIndex];
                                for (int py = 0; py < 16; ++py) {
                                    for (int px = 0; px < 16; ++px) {
                                        int drawX = fx ? 15 - px : px;
                                        if ((mask.collision[colIndex][drawX].height <= py)){
                                            if (mask.collision[colIndex][drawX].solid){
                                                if (colLyr == 0)
                                                    tileImg.setPixel(drawX, py, colors[tile.solidityA]);
                                                else
                                                    tileImg.setPixel(drawX, py, colors[tile.solidityB]);
                                            } else {
                                                QRgb pc = QColor(tileImg.pixel(drawX,py)).darker(255).rgba();
                                                tileImg.setPixel(drawX, py, pc);
                                            }
                                        }
                                    }
                                }

                            }
                            QPointF coords = QPointF(originX + x * (0x10 * scale), originY + y * (0x10 * scale));
                            p.drawImage(coords, tileImg.mirrored(fx,fy).scaled((0x10 * scale),(0x10 * scale)));
                        }
                    }
                    break;
                }
                case FORMAT_TILES:{
                    for (int index = 0; index < viewerTiles.count(); index++){
                        int t = viewerTiles[index];
                        QImage tileImg = tiles[t]->copy().convertToFormat(QImage::Format_RGB888);;
                        p.setOpacity(0.25);
                        if (selection->y != -1 && selection->x != -1 && selection->y * rowSize + selection->x == index)
                            p.setOpacity(1.0);
                        if (tileConfig != nullptr){
                            auto mask = tileConfig->collisionPaths[colLyr][t];
                            for (int y = 0; y < 16; ++y) {
                                for (int x = 0; x < 16; ++x) {
                                    QRgb colColor = 0xFFFFFF00;
                                    if (!mask.collision[x].solid)
                                        colColor = QColor(tileImg.pixel(x,y)).darker(255).rgba();
                                    if ((mask.direction ? mask.collision[x].height >= y : mask.collision[x].height <= y))
                                        tileImg.setPixel(x, y, colColor);
                                }
                            }
                        } else{
                            auto mask = tileConfigv1->collisionPaths[colLyr][t];
                            for (int y = 0; y < 16; ++y) {
                                for (int x = 0; x < 16; ++x) {
                                    QRgb colColor = 0xFFFFFF00;
                                    if (!mask.collision[colIndex][x].solid)
                                        colColor = QColor(tileImg.pixel(x,y)).darker(255).rgba();
                                    if (mask.collision[colIndex][x].height <= y)
                                        tileImg.setPixel(x, y, colColor);
                                }
                            }
                        }
                        QPointF coords = QPointF(originX + ((index % rowSize) * (0x10 * scale)), originY + (index / rowSize) * (0x10 * scale));
                        p.drawImage(coords, tileImg.scaled((0x10 * scale),(0x10 * scale)));
                    }
                    break;
                }
            };
            if (DrawPoint1.x != -1 && DrawPoint1.y != -1 && DrawPoint2.x != -1 && DrawPoint2.y != -1){
                QRectF rect2(0, 0, 4, 4);
                p.setPen(QPen(Qt::red, 0.5f));
                p.setOpacity(1);
                p.setBrush(Qt::NoBrush);
                p.drawRect(rect2.translated(DrawPoint1.x * 4 + originX, DrawPoint1.y * 4 + originY));
                p.drawRect(rect2.translated(DrawPoint2.x * 4 + originX, DrawPoint2.y * 4 + originY));
                p.drawLine(DrawPoint1.x * 4 + originX + 2, DrawPoint1.y * 4 + originY + 2,
                       DrawPoint2.x * 4 + originX + 2, DrawPoint2.y * 4 + originY + 2);
            }
            break;
        }
    }

    if (viewerTiles.count()){
        int gridSize = tileDrawMode == FORMAT_TILES ? rowSize : 8;
        p.setPen(QPen(Qt::white, 0.5f));
        p.setOpacity(1);
        for (int y = 0; y <= viewerTiles.count() / gridSize; ++y){
            if (y == viewerTiles.count() / gridSize){
                if (viewerTiles.count() < gridSize)
                    p.drawLine(QLineF(originX + 0, originY + y * (0x10 * scale), originX + viewerTiles.count() * (0x10 * scale), originY + y * (0x10 * scale)));
                else
                    p.drawLine(QLineF(originX + 0, originY + y * (0x10 * scale), originX + gridSize * (0x10 * scale), originY + y * (0x10 * scale)));
                p.drawLine(QLineF(originX + 0, originY + (y + 1) * (0x10 * scale), originX + (viewerTiles.count() % gridSize) * (0x10 * scale), originY + (y + 1) * (0x10 * scale)));
            } else{
                p.drawLine(QLineF(originX + 0, originY + y * (0x10 * scale), originX + gridSize * (0x10 * scale), originY + y * (0x10 * scale)));
            }
        }
        for (int x = 0; x <= gridSize; x++){
            if (x > viewerTiles.count())
                break;
            if (x > viewerTiles.count() % gridSize || (viewerTiles.count() % gridSize == 0 && x == 0))
                p.drawLine(QLineF(originX + x * (0x10 * scale), originY + 0, originX + x * (0x10 * scale), originY + (viewerTiles.count() / gridSize) * (0x10 * scale)));
            else
                p.drawLine(QLineF(originX + x * (0x10 * scale), originY + 0, originX + x * (0x10 * scale), originY + (viewerTiles.count() / gridSize + 1) * (0x10 * scale)));
        }
    }
}

void ChunkEditor::keyPressEvent(QKeyEvent *event)
{
    /*
    bool ctrlDownL  = false;
    bool altDownL   = false;
    bool shiftDownL = false;
    if (event->key() == Qt::Key_Control)
        ctrlDownL = true;
    if (event->key() == Qt::Key_Alt)
        altDownL = true;
    if (event->key() == Qt::Key_Shift)
        shiftDownL = true;//*/

    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
        && event->key() == Qt::Key_C) {
        copiedChunk = selectedChunk;
        SetStatus("copied chunk: " + QString::number(copiedChunk));
    }

    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
        && event->key() == Qt::Key_V) {
        if (copiedChunk < 0 || selectedChunk < 0)
            return;

        FormatHelpers::Chunks::Chunk &src = chunks->chunks[copiedChunk];
        FormatHelpers::Chunks::Chunk &dst = chunks->chunks[selectedChunk];
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                dst.tiles[y][x].tileIndex   = src.tiles[y][x].tileIndex;
                dst.tiles[y][x].visualPlane = src.tiles[y][x].visualPlane;
                dst.tiles[y][x].direction   = src.tiles[y][x].direction;
                dst.tiles[y][x].solidityA   = src.tiles[y][x].solidityA;
                dst.tiles[y][x].solidityB   = src.tiles[y][x].solidityB;
            }
        }

        viewer->repaint();

        changedChunks[selectedChunk] = true;
        ui->chunkList->item(selectedChunk)
            ->setIcon(QPixmap::fromImage(chunks->chunks[selectedChunk].getImage(tileList)));
        ui->toolTab->setCurrentIndex(0);
    }
}

ChunkPalette::ChunkPalette(QWidget *parent) : QWidget(parent) { setMouseTracking(true); }

void ChunkPalette::leaveEvent(QEvent *)
{
    highlight = -1;
    update();
}

void ChunkPalette::mousePressEvent(QMouseEvent *event)
{
    selection = highlight;
    enabling  = (event->button() & Qt::LeftButton) == Qt::LeftButton;
    pressed   = true;
    emit setColor(selection);
    update();
}

void ChunkPalette::mouseDoubleClickEvent(QMouseEvent *)
{
    if (selection >= palette.count())
        return;

    PaletteColor *prev   = palette.at(selection);
    RSDKColorDialog *dlg = new RSDKColorDialog(*palette.at(selection), this);
    dlg->show();
    if (dlg->exec() == QDialog::Accepted) {
        prev->r = dlg->color().r;
        prev->g = dlg->color().g;
        prev->b = dlg->color().b;
        emit colorChange(*prev);
        update();
    }
    delete dlg;
}


void ChunkPalette::mouseReleaseEvent(QMouseEvent *) { pressed = false;}

void ChunkPalette::mouseMoveEvent(QMouseEvent *event)
{
    byte x = floor((float)(event->x() / ((qreal)width() / 16)));
    byte y = floor((float)(event->y() / ((qreal)height() / 16)));

    if (y > 15)
        y = 15;

    highlight = x % 16 + y * 16;

    update();
}

void ChunkPalette::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QRectF rect(1, 2, (qreal)width() / 16 - 1, (qreal)height() / 16 - 1);
    short index = -1;
    if (palette.count() == 0)
        return;

    QPen swatchPen(qApp->palette().base(), 2);
    for (byte y = 0; y < palette.count() / 0x10; ++y) {
        for (byte x = 0; x < 0x10; ++x) {
            ++index;
            if (index >= palette.count())
                return;
            PaletteColor *clr = palette.at(index);
            p.setPen(swatchPen);
            p.setBrush(clr->toQColor());

            QRectF swatchRect = rect.translated(x * ((qreal)width()) / 16 - 1, y * ((qreal)height()) / 16 - 1);

            p.drawRect(swatchRect);
        }
    }

    QPen pen(appConfig.lightMode ? darkPal.base() : lightPal.base(), 1);
    index = -1;
    for (byte y = 0; y < palette.count() / 0x10; ++y) {
        for (byte x = 0; x < 0x10; ++x) {
            ++index;
            if (index >= palette.count())
                return;
            QRectF swatchRect = rect.translated(x * ((qreal)width()) / 16 - 1, y * ((qreal)height()) / 16 - 1);
            p.setOpacity(1.0);

            if (highlight == index) {
                p.setBrush(qApp->palette().highlight());
                p.setPen(Qt::NoPen);
                p.setOpacity(0.7);
                p.drawRect(swatchRect.adjusted(1,1,0,0));
            }

            if (selection == index){
                p.setPen(pen);
                p.setBrush(Qt::NoBrush);
                p.drawRect(swatchRect.adjusted(-1,-1,0,0));
            }
        }
    }
}

#include "moc_chunkeditor.cpp"
