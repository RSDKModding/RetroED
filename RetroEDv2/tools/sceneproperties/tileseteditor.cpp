#include "includes.hpp"
#include "ui_tileseteditor.h"
#include "tileseteditor.hpp"
#include "../paletteeditor/colourdialog.hpp"
#include "gfxreplacetool.hpp"
#include "importgfxtool.hpp"

#include "qgifimage.h"


enum editorModes { EDITOR_TILE, EDITOR_COLLISION, EDITOR_STAMP };
enum drawTools { DRAW_PENCIL, DRAW_RECT, DRAW_CIRCLE, DRAW_ERASER, DRAW_LINE };
enum colTools { COL_SOLIDITY, COL_ANGLES };
enum stampTools { STAMP_DRAW, STAMP_CLEAR };
enum PaletteFormatTypes { PALTYPE_ACT, PALTYPE_IMAGE };

TilesetEditor::TilesetEditor(QList<QImage> &tileList, RSDKv5::TileConfig &tConf, QList<PaletteColor> &pal, QList<RSDKv5::Stamps::StampEntry> &stampList, QWidget *parent)
    : QDialog(parent), ui(new Ui::TilesetEditor), tileConfig(tConf), tiles(tileList), palette(pal), stamps(stampList)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    ui->setupUi(this);

    this->setWindowTitle("Tileset Editor");

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->pencilTool->blockSignals(true);
    ui->colTool->blockSignals(true);
    ui->drawTool->blockSignals(true);

    ui->pencilTool->setDown(true);
    ui->colTool->setDown(true);
    ui->drawTool->setDown(true);

    ui->pencilTool->blockSignals(false);
    ui->colTool->blockSignals(false);
    ui->drawTool->blockSignals(false);

    for (int i = 0; i < palette.count(); ++i) clrTable.append(qRgb(pal[i].r, pal[i].g, pal[i].b));


    palWidget = new TilePalette;
    for (int c = 0; c < palette.count(); c++)
        palWidget->palette.append(&palette[c]);
    palWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    palWidget->setMaximumWidth(16 * 16 + 64);
    palWidget->setMaximumHeight(16 * 8 + 96);
    palWidget->setMinimumWidth(16 * 16 + 64);
    palWidget->setMinimumHeight(16 * 8 + 96);
    ui->paletteLayout->addWidget(palWidget, 1);

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

    connect(ui->importTile, &QPushButton::pressed, [=]{
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
            tile.fill(palette[0].toQColor().rgb());
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
            ImportGFXTool *tileImp = new ImportGFXTool(ENGINE_v5, importTiles, tileListCopy, pal, importPal);
            if (tileImp->exec() == QDialog::Accepted){
                QVector<QRgb> newPalette = tileList[0].colorTable();
                for (int i = 128; i < 256; ++i){
                    newPalette[i] = qRgb(tileImp->curPalette.at(i - 128).r, tileImp->curPalette.at(i - 128).g, tileImp->curPalette.at(i - 128).b);
                }
                for (int c = 128; c < palette.count(); c++){
                    QRgb clr = newPalette[c];
                    palette[c].r = (clr >> 16) & 0xFF;
                    palette[c].g = (clr >> 8) & 0xFF;
                    palette[c].b =  clr & 0xFF;
                    palWidget->palette[c] = &palette[c];
                }

                for (int t = 0; t < 0x400; t++){
                    tiles[t] = tileListCopy[t].convertToFormat(QImage::Format_Indexed8, newPalette);
                    viewer->tiles[t] = &tiles[t];
                    ui->tileList->item(t)->setIcon(QPixmap::fromImage(tiles[t]));
                }
                if (tileImp->generateCol){
                    for (auto t : tileImp->rplChkIDs){
                        auto tile  = &tiles[t];
                        generateCol(tileConfig.collisionPaths[0][t], *tile);
                        generateCol(tileConfig.collisionPaths[1][t], *tile);
                    }
                }

            }
        }
    });

    connect(ui->importPal, &QPushButton::pressed, [=] {
        QStringList types        = { "Adobe Color Table Palettes (*.act)",
            "Image Files (*.gif *.png *.bmp)",};
        QFileDialog filedialog(this, tr("Import Palette"), "",
                               tr(types.join(";;").toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            QString fileName = filedialog.selectedFiles()[0];
            QList<PaletteColor> newPal;
            for (int c = 128; c < palette.count(); c++){
                newPal.append(PaletteColor(palette[c]));
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
                            palette[p] = newPal[p];
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
                            palette[p] = newPal[p];
                        }
                    };
                    break;
                }
            }
        }
    });

    connect(ui->exportPal, &QToolButton::clicked, [=] {
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

                for (int i = 0; i < palette.count() && i < 256; i++)
                    gif.palette[i] = palette[i].toQColor().rgb();

                gif.write(filepath);
            }
            else
            {
                if (!CheckOverwrite(filepath, ".act", this))
                    return;

                Writer writer(filepath);
                for (auto &c : palette) c.write(writer);
                writer.flush();
            }
        }
    });

    // Add any filler tiles
    for (int t = tiles.count(); t < 0x400; ++t) {
        QImage tile = QImage(16, 16, QImage::Format_Indexed8);
        tile.setColorTable(clrTable);
        tiles.append(tile);
    }

    // Remove Excess tiles
    for (int t = tiles.count(); t > 0x400; --t) tiles.removeAt(t);

    viewer = new TilesetViewer(&selectedTile, &selectedStamp);
    for (auto &t : tiles){
        viewer->tiles.append(&t);
        //usedTiles.append(false);
    }
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(viewer);

    ui->tileList->clear();
    for (int t = 0; t < tiles.count(); ++t) {
        auto *item = new QListWidgetItem(QString::number(t), ui->tileList);
        item->setIcon(QPixmap::fromImage(tiles.at(t)));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }

    tileIDs.clear();
    changedTiles.clear();
    for (int t = 0; t < tiles.count(); ++t) {
        tileIDs.append(t);
        changedTiles.append(false);
    }

    for (auto &s : stamps)
        ui->stampList->addItem(s.name);

    viewer->tileConfig = &tileConfig;
    connect(ui->toolTab, &QTabWidget::currentChanged, [this](int t){
        viewer->viewerTiles.clear();
        if (t == EDITOR_STAMP) {
            if (ui->tileList->selectedItems().count() > 0) {
                QListWidgetItem *tile = ui->tileList->selectedItems().last();

                ui->tileList->blockSignals(true);
                ui->tileList->clearSelection();
                ui->tileList->item(tile->listWidget()->row(tile))->setSelected(true);
                ui->tileList->blockSignals(false);
            }
            if (selectedStamp != -1 && stamps[selectedStamp].tiles.count() > 0){
                for (ushort t : stamps[selectedStamp].tiles)
                    viewer->viewerTiles.append(t);
            }
        } else {
            if (viewer->editMode == EDITOR_STAMP && selectedStamp != -1){
                QSet<ushort> tiles;
                for (ushort t : stamps[selectedStamp].tiles){
                    tiles.insert(t & 0x3FF);
                }
                viewer->viewerTiles = tiles.values();
            } else{
                for (auto tile : ui->tileList->selectedItems()){
                    viewer->viewerTiles.append(tile->listWidget()->row(tile));
                }
            }
        }
        viewer->editMode = t;
        switch (viewer->editMode){
            case EDITOR_TILE: { ui->modeControls->setText("Viewer Controls: Left Click - Draws on top of tiles based on the selected Draw Tool | Right Click - Picks color on mouse position"); break; }
            case EDITOR_COLLISION: {
                if (viewer->colTool == COL_SOLIDITY){
                    ui->modeControls->setText("Viewer Controls: Left Click - Set Collision Height | Right Click - Set Solidity of Collision");
                } else{
                    ui->modeControls->setText("Viewer Controls: Drag Left Click - Set angle points for the 'Calculate Angle' tool | Right Click - Add hovered Tile to the Angle List (hold Control for Multi-Selection)");
                }
                break;
            }
            case EDITOR_STAMP: { ui->modeControls->setText("Viewer Controls: Left Click - Draw Tile based on 'TileList Tile Properties' | Right Click - Update Selected Tile Info | Double Right Click - Copy Selected Tile Info"); break; }
        }
        viewer->repaint();
    });

    connect(ui->resetCamera, &QPushButton::clicked, [=]{
        viewer->offset = Vector2<int>(0,0);
        viewer->repaint();
    });

    connect(ui->stampList, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *item){
        ui->stampList->blockSignals(true);
        ui->tileList->blockSignals(true);
        ui->tileList->clearSelection();

        int c = ui->stampList->row(item);
        auto stamp = &stamps[c];

        ui->stampName->blockSignals(true);
        ui->stampSizeX->blockSignals(true);
        ui->stampSizeY->blockSignals(true);

        ui->stampName->setEnabled(true);
        ui->stampName->setText(stamp->name);
        ui->stampSizeX->setEnabled(true);
        ui->stampSizeX->setValue(stamp->size.x);
        ui->stampSizeY->setEnabled(true);
        ui->stampSizeY->setValue(stamp->size.y);
        viewer->stampGrid.x = stamp->size.x;
        viewer->stampGrid.y = stamp->size.y;

        viewer->viewerTiles.clear();
        for (ushort t : stamp->tiles)
            viewer->viewerTiles.append(t);

        selectedStamp = c;

        ui->stampName->blockSignals(false);
        ui->stampSizeX->blockSignals(false);
        ui->stampSizeY->blockSignals(false);
        ui->tileList->blockSignals(false);
        ui->stampList->blockSignals(false);

        ui->toolTab->setCurrentIndex(EDITOR_STAMP);
        viewer->repaint();
    });

    connect(ui->addStamp, &QToolButton::clicked, [=]{
        ui->stampList->blockSignals(true);
        RSDKv5::Stamps::StampEntry newStamp;
        newStamp.name = "New Stamp";
        newStamp.size = Vector2<int>(0,0);
        stamps.append(newStamp);
        ui->stampList->addItem("New Stamp");
        selectedStamp = ui->stampList->count() - 1;

        ui->stampName->setEnabled(true);
        ui->stampSizeX->setEnabled(true);
        ui->stampSizeY->setEnabled(true);

        ui->stampName->blockSignals(true);
        ui->stampSizeX->blockSignals(true);
        ui->stampSizeY->blockSignals(true);

        ui->stampName->setText("New Stamp");
        ui->stampSizeX->setValue(0);
        ui->stampSizeY->setValue(0);
        viewer->stampGrid.x = 0;
        viewer->stampGrid.y = 0;

        ui->stampName->blockSignals(false);
        ui->stampSizeX->blockSignals(false);
        ui->stampSizeY->blockSignals(false);

        ui->stampList->setCurrentRow(ui->stampList->count() - 1);
        ui->stampList->blockSignals(false);
        ui->toolTab->setCurrentIndex(EDITOR_STAMP);
        viewer->repaint();
    });

    connect(ui->removeStamp, &QToolButton::clicked, [=]{
        int c = ui->stampList->currentRow();
        int n = ui->stampList->currentRow() == ui->stampList->count() - 1 ? c - 1 : c;
        delete ui->stampList->item(c);
        stamps.removeAt(c);

        bool e = ui->stampList->count() > 0;
        ui->stampName->setEnabled(e);
        ui->stampSizeX->setEnabled(e);
        ui->stampSizeY->setEnabled(e);

        ui->stampName->blockSignals(true);
        ui->stampSizeX->blockSignals(true);
        ui->stampSizeY->blockSignals(true);
        if (e){
            selectedStamp = n;
            RSDKv5::Stamps::StampEntry *stamp = &stamps[selectedStamp];

            ui->stampSizeX->setValue(stamp->size.x);
            ui->stampSizeY->setValue(stamp->size.y);
            ui->stampName->setText(stamp->name);
            viewer->stampGrid.x = stamp->size.x;
            viewer->stampGrid.y = stamp->size.y;


            if (viewer->editMode == EDITOR_STAMP){
                viewer->viewerTiles.clear();
                for (ushort t : stamp->tiles)
                    viewer->viewerTiles.append(t);
                viewer->repaint();
            }
        } else{
            selectedStamp = -1;
            if (viewer->editMode == EDITOR_STAMP){
                ui->stampSizeX->setValue(0);
                ui->stampSizeY->setValue(0);
                ui->stampName->setText("");
                viewer->stampGrid.x = 0;
                viewer->stampGrid.y = 0;
                viewer->viewerTiles.clear();
                viewer->repaint();
            }
        }

        ui->stampName->blockSignals(false);
        ui->stampSizeX->blockSignals(false);
        ui->stampSizeY->blockSignals(false);
    });

    connect(ui->stampName, &QLineEdit::textChanged, [=](QString name){
        RSDKv5::Stamps::StampEntry &stamp = stamps[selectedStamp];
        stamp.name = name;
        ui->stampList->currentItem()->setText(name);
    });

    connect(ui->stampSizeX, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newX){
        RSDKv5::Stamps::StampEntry &stamp = stamps[selectedStamp];
        QList<ushort> newList;
        int t = 0;
        if (newX < stamp.size.x){
            for (int y = 0; y < stamp.size.y; y++){
                for (int x = 0; x < stamp.size.x; x++){
                    if (x < newX)
                        newList.append(stamp.tiles[t]);
                    t++;
                }
            }
        } else{
            for (int y = 0; y < stamp.size.y; y++){
                for (int x = 0; x < newX; x++){
                    if (x >= stamp.size.x)
                        newList.append(0xFFFF);
                    else{
                        newList.append(stamp.tiles[t]);
                        t++;
                    }
                }
            }
        }
        stamp.tiles  = newList;
        viewer->viewerTiles.clear();
        for (ushort t : newList)
            viewer->viewerTiles.append(t);

        stamp.size.x = newX;
        viewer->stampGrid.x = newX;
        viewer->repaint();
    });

    connect(ui->stampSizeY, QOverload<int>::of(&QSpinBox::valueChanged), [=](int newY){
        RSDKv5::Stamps::StampEntry &stamp = stamps[selectedStamp];
        QList<ushort> newList;
        int t = 0;
        if (newY < stamp.size.y){
            for (int y = 0; y < newY; y++){
                for (int x = 0; x < stamp.size.x; x++){
                    newList.append(stamp.tiles[t++]);
                }
            }
        } else{
            for (int y = 0; y < newY; y++){
                for (int x = 0; x < stamp.size.x; x++){
                    if (y >= stamp.size.y)
                        newList.append(0xFFFF);
                    else
                        newList.append(stamp.tiles[t++]);
                }
            }
        }
        stamp.tiles  = newList;
        viewer->viewerTiles.clear();
        for (ushort t : newList)
            viewer->viewerTiles.append(t);

        stamp.size.y = newY;
        viewer->stampGrid.y = newY;
        viewer->repaint();
    });

    connect(ui->tileList, &QListWidget::itemSelectionChanged, [this]{
        ui->tileList->blockSignals(true);
        int c = ui->tileList->currentRow();
        auto item = ui->tileList->item(c);
        bool isSelected = ui->tileList->selectedItems().indexOf(item) != -1;
        item->setSelected(isSelected);
        ui->tileInfoTable->clear();
        if (viewer->editMode != EDITOR_STAMP){
            viewer->viewerTiles.clear();
            for (auto tile : ui->tileList->selectedItems())
                viewer->viewerTiles.append(tile->listWidget()->row(tile));
            ui->tileList->blockSignals(false);
        }
        viewer->repaint();
    });

    auto moveChunk = [this](char translation) {
        uint c = ui->tileList->currentRow();
        uint n = ui->tileList->currentRow() + translation;
        if (n >= (uint)tiles.count())
            return;

        auto *item = ui->tileList->takeItem(c);
        changedTiles.move(c, n);
        tileIDs.move(c, n);
        tiles.move(c, n);
        ui->tileList->insertItem(n, item);

        // DoAction("Moved tile", true);

        ui->tileList->setCurrentRow(n);
    };

    connect(ui->upTile, &QToolButton::clicked, [moveChunk] { moveChunk(-1); });

    connect(ui->downTile, &QToolButton::clicked, [moveChunk] { moveChunk(1); });

    // connect(ui->editPal, &QPushButton::clicked, [this, &tileList] {
    //
    // });

    connect(ui->exportTile, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Select folder to place images"), "", "");
        filedialog.setFileMode(QFileDialog::Directory);
        if (filedialog.exec() == QDialog::Accepted) {
            QString path = filedialog.selectedFiles()[0];
            SetStatus("Exporting tiles as images...");
            for (int t = 0; t < tiles.count(); ++t) {
                tiles.at(t).save(QString(path + "/Tile %1.png").arg(t));
                SetStatusProgress(t / (float)tiles.count());
            }
            SetStatus(QString("Exported tiles to %1/").arg(path));
        }
    });

    connect(viewer, &TilesetViewer::tileChanged, [=](ushort tile) {
        if (selectedStamp < 0 || tile == 0xFFFF)
            return;

        ui->tileList->blockSignals(true);
        ui->flipX->blockSignals(true);
        ui->flipY->blockSignals(true);
        ui->solidLRBA->blockSignals(true);
        ui->solidLRBB->blockSignals(true);
        ui->solidTopA->blockSignals(true);
        ui->solidTopB->blockSignals(true);

        ui->tileList->setCurrentRow(tile & 0x3FF);
        ui->flipX->setChecked(Utils::getBit(tile, 10));
        ui->flipY->setChecked(Utils::getBit(tile, 11));
        ui->solidLRBA->setChecked(Utils::getBit(tile, 12));
        ui->solidLRBB->setChecked(Utils::getBit(tile, 13));
        ui->solidTopA->setChecked(Utils::getBit(tile, 14));
        ui->solidTopB->setChecked(Utils::getBit(tile, 15));

        ui->tileList->blockSignals(false);
        ui->flipX->blockSignals(true);
        ui->flipY->blockSignals(true);
        ui->solidLRBA->blockSignals(true);
        ui->solidLRBB->blockSignals(true);
        ui->solidTopA->blockSignals(true);
        ui->solidTopB->blockSignals(true);
    });

    connect(viewer, &TilesetViewer::tileInfo, [=](ushort tile) {
        if (selectedStamp < 0 || tile == 0xFFFF)
            return;
        ui->tileInfo->setText(QString("Selected Tile Index: %1, Flags: %2 | %3 | %4 | %5 | %6 | %7")
                                  .arg(tile & 0x3FF)
                                  .arg(Utils::getBit(tile, 10) ? "FX" : "--")
                                  .arg(Utils::getBit(tile, 11) ? "FY" : "--")
                                  .arg(Utils::getBit(tile, 12) ? "AT" : "--")
                                  .arg(Utils::getBit(tile, 13) ? "AS" : "--")
                                  .arg(Utils::getBit(tile, 14) ? "BT" : "--")
                                  .arg(Utils::getBit(tile, 15) ? "BS" : "--"));
    });

    connect(viewer, &TilesetViewer::tileDrawn, [=](ushort selectedTile) {
        if (selectedStamp < 0)
            return;
        RSDKv5::Stamps::StampEntry &stamp = stamps[selectedStamp];
        if (stampTool == STAMP_DRAW){
            ushort tile    = ui->tileList->currentRow();
            bool flipX     = ui->flipX->checkState() == Qt::Checked;
            bool flipY     = ui->flipY->checkState() == Qt::Checked;
            bool solidATop = ui->solidTopA->checkState() == Qt::Checked;
            bool solidALRB = ui->solidLRBA->checkState() == Qt::Checked;
            bool solidBTop = ui->solidTopB->checkState() == Qt::Checked;
            bool solidBLRB = ui->solidLRBB->checkState() == Qt::Checked;

            Utils::setBit(tile, flipX, 10);
            Utils::setBit(tile, flipY, 11);
            Utils::setBit(tile, solidATop, 12);
            Utils::setBit(tile, solidALRB, 13);
            Utils::setBit(tile, solidBTop, 14);
            Utils::setBit(tile, solidBLRB, 15);
            stamp.tiles[selectedTile] = tile;
            viewer->viewerTiles[selectedTile] = tile;
        } else{
            stamp.tiles[selectedTile] = 0xFFFF;
            viewer->viewerTiles[selectedTile] = 0xFFFF;
        }

        viewer->repaint();
    });

    connect(ui->drawTool, &QToolButton::clicked, [=]{
        ui->clearTool->blockSignals(true);
        ui->clearTool->setDown(false);
        ui->clearTool->blockSignals(false);
        stampTool = STAMP_DRAW;
    });

    connect(ui->clearTool, &QToolButton::clicked, [=]{
        ui->drawTool->blockSignals(true);
        ui->drawTool->setDown(false);
        ui->drawTool->blockSignals(false);
        stampTool = STAMP_CLEAR;
    });

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

    });
    connect(ui->angleTool, &QToolButton::clicked, [=]{
        viewer->colTool = COL_ANGLES;
        ui->angleTool->setDown(true);
        ui->colTool->blockSignals(true);
        ui->colTool->setDown(false);
        ui->colTool->blockSignals(false);
        ui->modeControls->setText("Viewer Controls: Drag Left Click - Set angle points for the 'Calculate Angle' tool | Right Click - Add hovered Tile to the Angle List (hold Control for Multi-Selection)");
    });

    connect(ui->clearPoints, &QPushButton::pressed, [=]{
        viewer->DrawPoint1 = Vector2<int>(-1,-1);
        viewer->DrawPoint2 = Vector2<int>(-1,-1);
        viewer->usingAngleLine = false;
        viewer->repaint();
    });

    QPixmap clrIcon(44,44);
    clrIcon.fill(pal[0].toQColor());
    ui->bgClrBtn->setIcon(clrIcon);

    connect(ui->bgClrBtn, &QToolButton::clicked, [=] {
        RSDKColorDialog *dlg = new RSDKColorDialog((PaletteColor)tiles[0].color(0), this);
        dlg->show();
        if (dlg->exec() == QDialog::Accepted) {
            palette[0] = dlg->color();
            for (int t = 0; t < tiles.count(); ++t) {
                tiles[t].setColor(0, qRgb(dlg->color().r, dlg->color().g, dlg->color().b));
                viewer->tiles[t]->setColor(0, qRgb(dlg->color().r, dlg->color().g, dlg->color().b));
                ui->tileList->item(t)->setIcon(QPixmap::fromImage(tiles[t]));
            }
            ui->tileList->update();

            QPixmap clrIcon(44,44);
            clrIcon.fill(tiles[0].color(0));

            ui->bgClrBtn->setIcon(clrIcon);
            viewer->update();
        }

    });

    connect(ui->replaceTiles, &QToolButton::clicked, [=]{
        GFXReplaceTool *tileRpl = new GFXReplaceTool(0x400, tiles);
        if (tileRpl->exec() == QDialog::Accepted){
            QMapIterator<int,int> c(tileRpl->srcToRplIDs);
            while (c.hasNext()){
                c.next();
                ui->tileList->item(c.key())->setIcon(QPixmap::fromImage(tileList[c.value()]));
                viewer->tiles[c.key()] = &tiles[c.value()];
                tileConfig.collisionPaths[0][c.key()] = tileConfig.collisionPaths[0][c.value()];
                tileConfig.collisionPaths[1][c.key()] = tileConfig.collisionPaths[1][c.value()];
            }
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
        int limit = viewer->viewerTiles.count();
        for (int t = 0; t < limit; ++t){
            Vector2<int> start(-1, -1), end(-1, -1);
            int index = 0;
            index   = viewer->viewerTiles[t];
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

    connect(viewer, &TilesetViewer::updateAngles, [this](QSet<ushort> tileIndexes){
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

    connect(viewer, &TilesetViewer::setColor, [=] (int c){
        palWidget->selection = c;
    });

    connect(palWidget, &TilePalette::setColor, [=] (int c){
        viewer->selColor = c;
    });

    connect(palWidget, &TilePalette::colorChange, [=] (PaletteColor clr){
        palette[viewer->selColor].r = clr.r;
        palette[viewer->selColor].g = clr.g;
        palette[viewer->selColor].b = clr.b;
        for (int t = 0; t < tileList.count(); ++t) {
            tiles[t].setColor(viewer->selColor, qRgb(clr.r, clr.g, clr.b));
            viewer->tiles[t]->setColor(viewer->selColor, qRgb(clr.r, clr.g, clr.b));
            ui->tileList->item(t)->setIcon(QPixmap::fromImage(tileList[t]));
        }
        ui->tileList->update();
        viewer->repaint();
    });

    for (QWidget *w : findChildren<QWidget *>()) {
        w->installEventFilter(this);
    }
}

TilesetEditor::~TilesetEditor() { delete ui; }

bool TilesetEditor::event(QEvent *e)
{
    switch (e->type()) {
        default: break;
        case QEvent::KeyPress: keyPressEvent((QKeyEvent *)e); break;
    }
    return QWidget::event(e);
}

bool TilesetViewer::event(QEvent *e)
{
    setMouseTracking(true);
    switch (e->type()) {
        default: break;

        case QEvent::MouseButtonPress: mousePressEvent((QMouseEvent *)e); break;

        case QEvent::MouseMove: mouseMoveEvent((QMouseEvent *)e); break;

        case QEvent::MouseButtonRelease: mouseReleaseEvent((QMouseEvent *)e); break;

        case QEvent::Paint: paintEvent((QPaintEvent *)e); break;

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

void TilesetViewer::mouseDoubleClickEvent(QMouseEvent *event){

    if ((event->button() & Qt::RightButton) == Qt::RightButton)
        mouseDownR = true;

    if (mouseDownR && editMode == EDITOR_STAMP){
        int tilePos = selection.y * stampGrid.x + selection.x;
        if (tilePos > 0){
            emit tileChanged(viewerTiles[tilePos]);
            highlightTile = selection;
        }
    }
}

void TilesetViewer::mousePressEvent(QMouseEvent *event)
{
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

    if ((event->button() & Qt::LeftButton) == Qt::LeftButton)
        mouseDownL = true;
    if ((event->button() & Qt::MiddleButton) == Qt::MiddleButton)
        mouseDownM = true;
    if ((event->button() & Qt::RightButton) == Qt::RightButton)
        mouseDownR = true;

    reference = event->pos();

    short index = -1;

    switch (editMode){
        case EDITOR_TILE: {
            if (mouseDownL && (drawTool == DRAW_RECT || drawTool == DRAW_CIRCLE || drawTool == DRAW_LINE))
                rect.setRect(gridPos.x, gridPos.y, gridPos.x, gridPos.y);

            if (mouseDownR && inGrid) {
                int tilePxX = gridPos.x % 16, tilePxY = gridPos.y % 16;
                index = viewerTiles.indexOf(selection.y * rowSize + selection.x);
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
                        int pos = selection.y * rowSize + selection.x;
                        if (pos >= viewerTiles.count())
                            break;
                        index = viewerTiles[pos];
                        if (index == -1)
                            break;
                        solidityCheck = tileConfig->collisionPaths[colLyr][index].collision[tilePxX].solid == 0 ? 1 : 0;
                    }
                    break;
                }
                case COL_ANGLES: {
                    usingAngleLine = false;
                    DrawPoint1 = Vector2<int>(-1,-1);
                    DrawPoint2 = Vector2<int>(-1,-1);

                    if (mouseDownL)
                        DrawPoint1 = gridPos;

                    if (mouseDownR && inGrid){
                        int pos = selection.y * rowSize + selection.x;
                        if (pos >= viewerTiles.count())
                            break;
                        index = viewerTiles[pos];

                        if (index == -1)
                            break;

                        if ((event->modifiers() & Qt::ControlModifier) != Qt::ControlModifier)
                            angleTiles.clear();

                        angleTiles.insert(index);

                        emit updateAngles(angleTiles);
                    }
                    break;
                }
            }
            break;
        }
        case EDITOR_STAMP: {
            int tilePos = selection.y * stampGrid.x + selection.x;
            if (tilePos > 0){
                if (mouseDownL){
                    emit tileDrawn(tilePos);
                }
                if (mouseDownR)
                    emit tileInfo(viewerTiles[tilePos]);
                if (mouseDownL || mouseDownR)
                    highlightTile = selection;
            }
            break;
        }
    }
    repaint();
}

void TilesetViewer::mouseMoveEvent(QMouseEvent *event)
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

    selection.x = gridPos.x / 16; selection.y = gridPos.y / 16;

    if (editMode == EDITOR_STAMP){
        if (selection.x >= stampGrid.x || selection.x < 0)
            selection.x = -1;

        if (selection.y > stampGrid.y || selection.y < 0)
            selection.y = -1;
    } else{
        if (selection.x >= rowSize || selection.x < 0 || (selection.y >= viewerTiles.count() / rowSize
                                                            && selection.x >= viewerTiles.count() % rowSize))
            selection.x = -1;

        if (selection.y > viewerTiles.count() / rowSize || selection.y < 0 || (selection.y >= viewerTiles.count() / rowSize
                                                                                 && selection.x >= viewerTiles.count() % rowSize))
            selection.y = -1;
    }

    TilePxX = gridPos.x % 16;
    TilePxY = gridPos.y % 16;
    inGrid = (selection.x != -1 && selection.y != -1);
    short index = -1;

    switch (editMode){
        case EDITOR_TILE: {
            if (drawTool == DRAW_RECT || drawTool == DRAW_CIRCLE || drawTool == DRAW_LINE){
                rect.setWidth(gridPos.x);
                rect.setHeight(gridPos.y);
                break;
            }

            if (inGrid){
                int pos = selection.y * rowSize + selection.x;
                if (pos >= viewerTiles.count())
                    break;
                index = viewerTiles[pos];

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
                    int pos = selection.y * rowSize + selection.x;
                    if (pos >= viewerTiles.count())
                        break;
                    index = viewerTiles[pos];
                if (index == -1)
                    break;

                if (colTool == COL_SOLIDITY){
                    if (mouseDownL) {
                        tileConfig->collisionPaths[colLyr][index].collision[TilePxX].height = TilePxY;
                    } else if (mouseDownR){
                        tileConfig->collisionPaths[colLyr][index].collision[TilePxX].solid = solidityCheck;
                    }
                }
            }
            break;

        }
        case EDITOR_STAMP: {
            int tilePos = selection.y * stampGrid.x + selection.x;
            if (tilePos > 0){
                if (mouseDownL){
                    emit tileDrawn(tilePos);
                }
                if (mouseDownR)
                    emit tileInfo(viewerTiles[tilePos]);
                if (mouseDownL || mouseDownR)
                    highlightTile = selection;
            }
            break;
        }
    }
    repaint();
}

void TilesetViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (mouseDownL || mouseDownR){
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

            for (int i = 0; i < viewerTiles.count(); ++i){
                int x = i % rowSize, y = i / rowSize;
                QImage *tile  = tiles.at(viewerTiles[i]);
                QImage area  = shape.copy(x * 16, y * 16, 16, 16);
                for (int ty = 0; ty < 16; ++ty){
                    for (int tx = 0; tx < 16; ++tx){
                        if (area.pixelColor(tx, ty) == tile->color(selColor)){
                            tile->setPixel(tx, ty, selColor);
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
        emit updateLists();
    }

    if ((event->button() & Qt::LeftButton) == Qt::LeftButton)
        mouseDownL = false;
    if ((event->button() & Qt::MiddleButton) == Qt::MiddleButton)
        mouseDownM = false;
    if ((event->button() & Qt::RightButton) == Qt::RightButton)
        mouseDownR = false;
}

void TilesetViewer::paintEvent(QPaintEvent *event)
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
    const QBrush brush = p.brush();

    switch (editMode){
        case EDITOR_TILE: {
            for (int index = 0; index < viewerTiles.count(); index++){
                int t = viewerTiles[index];
                p.setOpacity(1.0);
                QPointF coords = QPointF(originX + ((index % rowSize) * (0x10 * scale)), originY + (index / rowSize) * (0x10 * scale));
                p.drawImage(coords, tiles.at(t)->scaled((0x10 * scale),(0x10 * scale)));
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
            for (int index = 0; index < viewerTiles.count(); index++){
                int t = viewerTiles[index];
                QImage tileImg = tiles[t]->copy().convertToFormat(QImage::Format_RGB888);;
                p.setOpacity(0.25);
                if (selection.y != -1 && selection.x != -1 && selection.y * rowSize + selection.x == index)
                    p.setOpacity(1.0);
                auto mask = tileConfig->collisionPaths[colLyr][t];
                for (int y = 0; y < 16; ++y) {
                    for (int x = 0; x < 16; ++x) {
                        QColor colColor = 0xFFFFFF00;
                        if (!mask.collision[x].solid)
                            colColor = colColor.darker(255);
                        if ((mask.direction ? mask.collision[x].height >= y : mask.collision[x].height <= y) && mask.collision[x].solid)
                            tileImg.setPixel(x, y, colColor.rgb());
                    }
                }
                QPointF coords = QPointF(originX + ((index % rowSize) * (0x10 * scale)), originY + (index / rowSize) * (0x10 * scale));
                p.drawImage(coords, tileImg.scaled((0x10 * scale),(0x10 * scale)));
            }
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
        case EDITOR_STAMP: {
            if (*sSel != -1 && viewerTiles.count() > 0){
                const QBrush brush = p.brush();
                int t = 0;
                for (int y = 0; y < stampGrid.y; ++y) {
                    for (int x = 0; x < stampGrid.x; ++x) {
                        auto tile = viewerTiles[t++];

                        bool fx = Utils::getBit(tile, 10), fy = Utils::getBit(tile, 11);
                        p.drawImage(QPointF(originX + x * (0x10 * scale), originY + y * (0x10 * scale)),
                                    tiles.at(tile & 0x3FF)->mirrored(fx, fy).scaled((0x10 * scale), (0x10 * scale)));

                        if (y == highlightTile.y && x == highlightTile.x) {
                            p.setBrush(qApp->palette().highlight());
                            p.setOpacity(0.5);
                            p.drawRect(QRectF(originX + x * (0x10 * scale), originY + y * (0x10 * scale), (0x10 * scale), (0x10 * scale)));
                            p.setBrush(brush);
                            p.setOpacity(1.0);
                        }
                    }
                }
            }
            break;
        }
    }

    if (viewerTiles.count()){
        p.setPen(QPen(Qt::white, 0.5f));
        p.setOpacity(0.5);
        if (editMode != EDITOR_STAMP){
            int gridSize = rowSize;
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
        } else{
            for (int y = 0; y <= stampGrid.y; ++y){
                p.drawLine(QLineF(originX + 0, originY + y * (0x10 * scale), originX + stampGrid.x * (0x10 * scale), originY + y * (0x10 * scale)));
            }
            for (int x = 0; x <= stampGrid.x; x++){
                p.drawLine(QLineF(originX + x * (0x10 * scale), originY + 0, originX + x * (0x10 * scale), originY + stampGrid.y * (0x10 * scale)));
            }
        }
    }
}

void TilesetEditor::keyPressEvent(QKeyEvent *event)
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
        copiedTile = selectedTile;
        SetStatus("copied tile: " + QString::number(copiedTile));
    }

    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
        && event->key() == Qt::Key_V) {
        if (copiedTile < 0 || selectedTile < 0)
            return;

        uchar *pixels = tiles[selectedTile].bits();
        uchar *src    = tiles[copiedTile].bits();
        for (int y = 0; y < 16; ++y) {
            for (int x = 0; x < 16; ++x) {
                *pixels++ = *src++;
            }
        }

        viewer->repaint();

        changedTiles[selectedTile] = true;
        ui->tileList->item(selectedTile)->setIcon(QPixmap::fromImage(tiles.at(selectedTile)));
    }
}

TilePalette::TilePalette(QWidget *parent) : QWidget(parent) { setMouseTracking(true); }

void TilePalette::leaveEvent(QEvent *)
{
    highlight = -1;
    update();
}

void TilePalette::mousePressEvent(QMouseEvent *event)
{
    selection = highlight;
    enabling  = (event->button() & Qt::LeftButton) == Qt::LeftButton;
    pressed   = true;
    emit setColor(selection);
    update();
}

void TilePalette::mouseDoubleClickEvent(QMouseEvent *)
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


void TilePalette::mouseReleaseEvent(QMouseEvent *) { pressed = false;}

void TilePalette::mouseMoveEvent(QMouseEvent *event)
{
    byte x = floor((float)(event->x() / ((qreal)width() / 16)));
    byte y = floor((float)(event->y() / ((qreal)height() / 16)));

    if (y > 15)
        y = 15;

    highlight = x % 16 + y * 16 + 128;

    update();
}

void TilePalette::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QRectF rect(1, 2, (qreal)width() / 16 - 1, (qreal)height() / 16 - 1);
    short index = 127;
    if (palette.count() == 0)
        return;

    QPen swatchPen(qApp->palette().base(), 2);
    for (byte y = 8; y < palette.count() / 0x10; ++y) {
        for (byte x = 0; x < 0x10; ++x) {
            ++index;
            if (index >= palette.count())
                return;
            PaletteColor *clr = palette.at(index);
            p.setPen(swatchPen);
            p.setBrush(clr->toQColor());

            QRectF swatchRect = rect.translated(x * ((qreal)width()) / 16 - 1, (y - 8) * ((qreal)height()) / 16 - 1);

            p.drawRect(swatchRect);
        }
    }

    QPen pen(appConfig.lightMode ? darkPal.base() : lightPal.base(), 1);
    index = 127;
    for (byte y = 8; y < palette.count() / 0x10; ++y) {
        for (byte x = 0; x < 0x10; ++x) {
            ++index;
            if (index >= palette.count())
                return;
            QRectF swatchRect = rect.translated(x * ((qreal)width()) / 16 - 1, (y - 8) * ((qreal)height()) / 16 - 1);
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

#include "moc_tileseteditor.cpp"
