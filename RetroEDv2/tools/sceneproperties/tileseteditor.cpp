#include "includes.hpp"
#include "ui_tileseteditor.h"

TilesetEditor::TilesetEditor(QList<QImage> *tileList, QList<PaletteColour> *pal, QWidget *parent)
    : QDialog(parent), ui(new Ui::TilesetEditor)
{
    ui->setupUi(this);

    tiles   = tileList;
    palette = pal;

    for (int t = tiles->count(); t < 0x400; ++t) {
        QImage tile = QImage(16, 16, QImage::Format_Indexed8);
        tile.fill(0xFF00FF);
        tiles->append(tile);
    }

    for (int t = tiles->count(); t > 0x400; --t) {
        tiles->removeAt(t);
    }

    ui->tileList->clear();
    for (int t = 0; t < tiles->count(); ++t) {
        auto *item = new QListWidgetItem(QString::number(t), ui->tileList);
        item->setText("");
        item->setIcon(QPixmap::fromImage(tiles->at(t)));
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

    connect(ui->upTile, &QToolButton::clicked, [this] {
        uint c     = ui->tileList->currentRow();
        auto *item = ui->tileList->takeItem(c);
        tiles->move(c, c - 1);
        tileIDs.move(c, c - 1);
        ui->tileList->insertItem(c - 1, item);
        ui->tileList->setCurrentRow(c - 1);
    });

    connect(ui->downTile, &QToolButton::clicked, [this] {
        uint c     = ui->tileList->currentRow();
        auto *item = ui->tileList->takeItem(c);
        tiles->move(c, c + 1);
        tileIDs.move(c, c + 1);
        ui->tileList->insertItem(c + 1, item);
        ui->tileList->setCurrentRow(c + 1);
    });

    connect(ui->editPal, &QToolButton::clicked, [this] {

    });
}

TilesetEditor::~TilesetEditor() { delete ui; }

#include "moc_tileseteditor.cpp"
