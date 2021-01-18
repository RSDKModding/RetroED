#include "includes.hpp"
#include "ui_chunkeditor.h"

ChunkEditor::ChunkEditor(FormatHelpers::Chunks *chk, QList<QImage> &chunks, QList<QImage> &tiles, bool v1, QWidget *parent) :
    m_chunks(chk),
    QDialog(parent),
    ui(new Ui::ChunkEditor)
{
    ui->setupUi(this);

    this->setWindowTitle("Chunks Editor");

    if (!chk)
        return;

    ui->chunkList->clear();
    for (int c = 0; c < (v1 ? 0x100 : 0x200); ++c) {
        auto *item    = new QListWidgetItem(QString::number(c), ui->chunkList);
        item->setIcon(QPixmap::fromImage(chunks[c]));
    }

    ui->tileList->clear();
    for (int t = 0; t < tiles.count(); ++t) {
        auto *item    = new QListWidgetItem(QString::number(t), ui->tileList);
        item->setIcon(QPixmap::fromImage(tiles[t]));
    }

    auto chunkRowChanged = [this](int c) {
        ui->flipX->setDisabled(c == -1);
        ui->flipY->setDisabled(c == -1);
        ui->visualPlane->setDisabled(c == -1);
        ui->tileIndex->setDisabled(c == -1);
        ui->solidityA->setDisabled(c == -1);
        ui->solidityB->setDisabled(c == -1);

        m_selectedChunk = c;

        if (c == -1)
            return;

        ui->flipX->blockSignals(true);
        ui->flipY->blockSignals(true);
        ui->visualPlane->blockSignals(true);
        ui->tileIndex->blockSignals(true);
        ui->solidityA->blockSignals(true);
        ui->solidityB->blockSignals(true);

        ui->flipX->setChecked(Utils::getBit(m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_direction, 0));
        ui->flipY->setChecked(Utils::getBit(m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_direction, 1));

        ui->visualPlane->setCurrentIndex(m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_visualPlane);

        ui->tileIndex->setValue(m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_tileIndex);

        ui->solidityA->setCurrentIndex(m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_solidityA);
        ui->solidityB->setCurrentIndex(m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_solidityB);


        ui->flipX->blockSignals(false);
        ui->flipY->blockSignals(false);
        ui->visualPlane->blockSignals(false);
        ui->tileIndex->blockSignals(false);
        ui->solidityA->blockSignals(false);
        ui->solidityB->blockSignals(false);
    };
    chunkRowChanged(0);
    connect(ui->chunkList, &QListWidget::currentRowChanged, chunkRowChanged);

    auto tileRowChanged = [this, tiles](int c) {
        m_selectedDrawTile = c;

        if (m_selectedChunk >= 0 && c >= 0) {
            m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_tileIndex = (ushort)c;

            ui->tileIndex->blockSignals(true);
            ui->tileIndex->setValue(m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_tileIndex);
            ui->tileIndex->blockSignals(false);
            ui->chunkList->item(m_selectedChunk)->setIcon(QPixmap::fromImage(m_chunks->m_chunks[m_selectedChunk].getImage(tiles)));
        }
    };
    tileRowChanged(-1);
    connect(ui->tileList, &QListWidget::currentRowChanged, tileRowChanged);

    connect(ui->flipX, &QCheckBox::toggled, [this, tiles](bool v) {
        if (m_selectedChunk < 0)
            return;

        Utils::setBit(m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_direction, v, 0);

        ui->chunkList->item(m_selectedChunk)->setIcon(QPixmap::fromImage(m_chunks->m_chunks[m_selectedChunk].getImage(tiles)));
    });
    connect(ui->flipY, &QCheckBox::toggled, [this, tiles](bool v) {
        if (m_selectedChunk < 0)
            return;

        Utils::setBit(m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_direction, v, 1);

        ui->chunkList->item(m_selectedChunk)->setIcon(QPixmap::fromImage(m_chunks->m_chunks[m_selectedChunk].getImage(tiles)));
    });

    connect(ui->visualPlane, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int v) {
        if (m_selectedChunk < 0)
            return;

        m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_visualPlane = (byte)v;
    });

    connect(ui->tileIndex, QOverload<int>::of(&QSpinBox::valueChanged),
            [this, tiles](int v) {
        if (m_selectedChunk < 0)
            return;

        m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_tileIndex = (ushort)v;

        ui->chunkList->item(m_selectedChunk)->setIcon(QPixmap::fromImage(m_chunks->m_chunks[m_selectedChunk].getImage(tiles)));
     });

    connect(ui->solidityA, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int v) {
        if (m_selectedChunk < 0)
            return;

        m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_solidityA = (byte)v;
    });

    connect(ui->solidityB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int v) {
        if (m_selectedChunk < 0)
            return;

        m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_solidityB = (byte)v;
    });



    for (QWidget *w : findChildren<QWidget*>()) {
        w->installEventFilter(this);
    }
}

ChunkEditor::~ChunkEditor()
{
    delete ui;
}
#include "moc_chunkeditor.cpp"
