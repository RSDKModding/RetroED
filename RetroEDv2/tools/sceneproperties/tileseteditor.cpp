#include "includes.hpp"
#include "ui_tileseteditor.h"

#include "qgifimage.h"

TilesetEditor::TilesetEditor(QList<QImage> &tileList, QList<PaletteColour> &pal, QWidget *parent)
    : QDialog(parent), ui(new Ui::TilesetEditor), tiles(tileList), palette(pal)
{
    ui->setupUi(this);

    QVector<QRgb> clrTable;
    for (int i = 0; i < pal.count(); ++i) clrTable.append(qRgb(pal[i].r, pal[i].g, pal[i].b));

    for (int t = tileList.count(); t < 0x400; ++t) {
        QImage tile = QImage(16, 16, QImage::Format_Indexed8);
        tile.setColorTable(clrTable);
        tileList.append(tile);
    }

    // Remove Excess tiles
    for (int t = tileList.count(); t > 0x400; --t) tileList.removeAt(t);

    ui->tileList->clear();
    for (int t = 0; t < tileList.count(); ++t) {
        auto *item = new QListWidgetItem(QString::number(t), ui->tileList);
        item->setText("");
        item->setIcon(QPixmap::fromImage(tileList.at(t)));
    }

    for (int t = 0; t < 0x400; ++t) {
        tileIDs.append(t);
    }

    // ----------------
    // TILES
    // ----------------
    connect(ui->tileList, &QListWidget::currentRowChanged, [this](int c) {
        if (ui->upTile)
            ui->upTile->setDisabled(c == -1);
        if (ui->downTile)
            ui->downTile->setDisabled(c == -1);

        if (c == -1)
            return;

        if (ui->downTile)
            ui->downTile->setDisabled(c == ui->tileList->count() - 1);
        if (ui->upTile)
            ui->upTile->setDisabled(c == 0);
    });

    connect(ui->upTile, &QToolButton::clicked, [this, &tileList] {
        uint c     = ui->tileList->currentRow();
        auto *item = ui->tileList->takeItem(c);
        tileList.move(c, c - 1);
        tileIDs.move(c, c - 1);
        ui->tileList->insertItem(c - 1, item);
        ui->tileList->setCurrentRow(c - 1);
    });

    connect(ui->downTile, &QToolButton::clicked, [this, &tileList] {
        uint c     = ui->tileList->currentRow();
        auto *item = ui->tileList->takeItem(c);
        tileList.move(c, c + 1);
        tileIDs.move(c, c + 1);
        ui->tileList->insertItem(c + 1, item);
        ui->tileList->setCurrentRow(c + 1);
    });

    connect(ui->editPal, &QToolButton::clicked, [this, &tileList] {

    });

    connect(ui->importTiles, &QToolButton::clicked, [this, &tileList, clrTable] {
        QList<QString> types = {
            "Gif Images (*.gif)",
            "PNG Images (*.png)",
            "BMP Images (*.bmp)",
        };

        QFileDialog filedialog(
            this, tr("Open Image"), "",
            tr(QString("%1;;%2;;%3").arg(types[0]).arg(types[1]).arg(types[2]).toStdString().c_str()));
        filedialog.setAcceptMode(QFileDialog::AcceptOpen);
        if (filedialog.exec() == QDialog::Accepted) {
            int filter = types.indexOf(filedialog.selectedNameFilter());
            QImage img;

            setStatus("Finished Importing Tile(s)...");

            if (!filter) {
                QGifImage gif(filedialog.selectedFiles()[0]);
                img = gif.frame(0);
            }
            else {
                img.load(filedialog.selectedFiles()[0]);
            }

            int pos = ui->tileList->currentRow();
            if (pos < 0)
                pos = 0;
            int startPos = pos;

            if (img.format() == QImage::Format_Indexed8) {
                // Assume palette is correct if its already indexed
                for (int y = 0; y < img.height(); y += 16) {
                    for (int x = 0; x < img.width(); x += 16) {
                        QImage tile(16, 16, QImage::Format_Indexed8);
                        tile.setColorTable(clrTable);

                        for (int sy = 0; sy < 16; ++sy) {
                            for (int sx = 0; sx < 16; ++sx) {
                                if (x + sx >= img.width() || y + sy >= img.height())
                                    continue;

                                int index = img.pixelIndex(x + sx, y + sy);
                                tile.setPixel(sx, sy, index);
                            }
                        }

                        tileList[pos++] = tile;

                        if (pos >= 0x400) {
                            x = img.width();
                            y = img.height();
                        }
                    }
                }
            }
            else {
                // we'll have to match the palette
                for (int y = 0; y < img.height(); y += 16) {
                    for (int x = 0; x < img.width(); x += 16) {
                        QImage tile(16, 16, QImage::Format_Indexed8);
                        tile.setColorTable(clrTable);

                        for (int sy = 0; sy < 16; ++sy) {
                            for (int sx = 0; sx < 16; ++sx) {
                                if (x + sx >= img.width() || y + sy >= img.height())
                                    continue;

                                int index = getColourIndex(img, clrTable, x + sx, y + sy);
                                tile.setPixel(sx, sy, index);
                            }
                        }

                        tileList[pos++] = tile;

                        if (pos >= 0x400) {
                            x = img.width();
                            y = img.height();
                        }
                    }
                }
            }

            for (int p = startPos; p < pos; ++p) {
                ui->tileList->item(p)->setIcon(QPixmap::fromImage(tileList.at(p)));
            }

            setStatus("Finished Importing Tile(s)!");
        }
    });
}

TilesetEditor::~TilesetEditor() { delete ui; }

#include "moc_tileseteditor.cpp"
