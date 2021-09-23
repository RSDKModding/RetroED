#include "includes.hpp"
#include "ui_chunkeditor.h"

ChunkEditor::ChunkEditor(FormatHelpers::Chunks *chk, QList<QImage> &chunkList, QList<QImage> &tiles,
                         bool v1, QWidget *parent)
    : chunks(chk), QDialog(parent), ui(new Ui::ChunkEditor)
{
    ui->setupUi(this);

    this->setWindowTitle("Chunk Editor");

    if (!chk)
        return;

    tileList = tiles;

    viewer = new ChunkViewer(&selectedChunk, &selectedTile, chunks, tiles);
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(viewer);

    ui->chunkList->clear();
    for (int c = 0; c < (v1 ? 0x100 : 0x200); ++c) {
        auto *item = new QListWidgetItem(QString::number(c), ui->chunkList);
        item->setIcon(QPixmap::fromImage(chunkList[c]));
    }

    ui->tileList->clear();
    for (int t = 0; t < tiles.count(); ++t) {
        auto *item = new QListWidgetItem(QString::number(t), ui->tileList);
        item->setIcon(QPixmap::fromImage(tiles[t]));
    }

    auto chunkRowChanged = [this](int c) {
        ui->flipX->setDisabled(c == -1);
        ui->flipY->setDisabled(c == -1);
        ui->visualPlane->setDisabled(c == -1);
        ui->tileIndex->setDisabled(c == -1);
        ui->solidityA->setDisabled(c == -1);
        ui->solidityB->setDisabled(c == -1);

        selectedChunk = c;

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

        viewer->repaint();
    };
    chunkRowChanged(0);
    connect(ui->chunkList, &QListWidget::currentRowChanged, chunkRowChanged);

    auto tileRowChanged = [this, tiles](int c) { selectedDrawTile = c; };
    tileRowChanged(-1);
    connect(ui->tileList, &QListWidget::currentRowChanged, tileRowChanged);

    auto tileChangedCB = [this] {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
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
    };

    connect(viewer, &ChunkViewer::tileChanged, tileChangedCB);

    connect(viewer, &ChunkViewer::tileDrawn, [this, tiles, tileChangedCB](float mx, float my) {
        selectedTile.x = mx / 16;
        selectedTile.y = my / 16;

        if (selectedTile.x > 0x80)
            selectedTile.x = -1;

        if (selectedTile.y > 0x80)
            selectedTile.y = -1;

        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1 || selectedDrawTile == -1)
            return;

        chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].tileIndex =
            (ushort)selectedDrawTile;
        viewer->repaint();

        ui->chunkList->item(selectedChunk)
            ->setIcon(QPixmap::fromImage(chunks->chunks[selectedChunk].getImage(tiles)));

        tileChangedCB();
    });

    connect(viewer, &ChunkViewer::tileCopy, [this, tiles](float mx, float my) {
        selectedTile.x = mx / 16;
        selectedTile.y = my / 16;

        if (selectedTile.x > 0x80)
            selectedTile.x = -1;

        if (selectedTile.y > 0x80)
            selectedTile.y = -1;

        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        selectedDrawTile =
            chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].tileIndex;

        ui->tileList->setCurrentRow(selectedDrawTile);
    });

    connect(ui->flipX, &QCheckBox::toggled, [this, tiles](bool v) {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        Utils::setBit(chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].direction, v,
                      0);
        viewer->repaint();

        ui->chunkList->item(selectedChunk)
            ->setIcon(QPixmap::fromImage(chunks->chunks[selectedChunk].getImage(tiles)));
    });
    connect(ui->flipY, &QCheckBox::toggled, [this, tiles](bool v) {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        Utils::setBit(chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].direction, v,
                      1);
        viewer->repaint();

        ui->chunkList->item(selectedChunk)
            ->setIcon(QPixmap::fromImage(chunks->chunks[selectedChunk].getImage(tiles)));
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

        ui->chunkList->item(selectedChunk)
            ->setIcon(QPixmap::fromImage(chunks->chunks[selectedChunk].getImage(tiles)));
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
    switch (e->type()) {
        default: break;
        case QEvent::MouseButtonPress: mousePressEvent((QMouseEvent *)e); break;
        case QEvent::MouseMove: mouseMoveEvent((QMouseEvent *)e); break;
        case QEvent::Paint: paintEvent((QPaintEvent *)e); break;
    }
    return QWidget::event(e);
}

void ChunkViewer::mousePressEvent(QMouseEvent *event)
{
    float mousePosX = event->pos().x();
    float mousePosY = event->pos().y();

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;
    if ((event->button() & Qt::LeftButton) == Qt::LeftButton)
        mouseDownL = true;
    if ((event->button() & Qt::MidButton) == Qt::MidButton)
        mouseDownM = true;
    if ((event->button() & Qt::RightButton) == Qt::RightButton)
        mouseDownR = true;

    if (mouseDownR) {
        selection->x = (mousePosX * zoom) / 16;
        selection->y = (mousePosY * zoom) / 16;

        if (selection->x > 0x80)
            selection->x = -1;

        if (selection->y > 0x80)
            selection->y = -1;

        emit tileChanged();

        repaint();
    }

    if (mouseDownM) {
        emit tileCopy(mousePosX * zoom, mousePosY * zoom);
    }

    if (mouseDownL) {
        emit tileDrawn(mousePosX * zoom, mousePosY * zoom);
    }
}

void ChunkViewer::mouseMoveEvent(QMouseEvent *event)
{
    float mousePosX = event->pos().x();
    float mousePosY = event->pos().y();

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;
    if ((event->button() & Qt::LeftButton) == Qt::LeftButton)
        mouseDownL = true;
    if ((event->button() & Qt::MidButton) == Qt::MidButton)
        mouseDownM = true;
    if ((event->button() & Qt::RightButton) == Qt::RightButton)
        mouseDownR = true;

    if (mouseDownR) {
        selection->x = (mousePosX * zoom) / 16;
        selection->y = (mousePosY * zoom) / 16;

        if (selection->x > 0x80)
            selection->x = -1;

        if (selection->y > 0x80)
            selection->y = -1;

        emit tileChanged();

        repaint();
    }

    if (mouseDownM) {
        emit tileCopy(mousePosX * zoom, mousePosY * zoom);
    }

    if (mouseDownL) {
        emit tileDrawn(mousePosX * zoom, mousePosY * zoom);
    }
}

void ChunkViewer::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter p(this);
    p.scale(zoom, zoom);

    const QBrush brush = p.brush();
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto &tile = chunks->chunks[*cSel].tiles[y][x];

            bool fx = (tile.direction & 1) == 1;
            bool fy = (tile.direction & 2) == 2;
            p.drawImage(x * 0x10, y * 0x10, tiles[tile.tileIndex].mirrored(fx, fy));

            if (y == selection->y && x == selection->x) {
                p.setBrush(qApp->palette().highlight());
                p.setOpacity(0.5);
                p.drawRect(QRect(x * 0x10, y * 0x10, 0x10, 0x10));
                p.setBrush(brush);
                p.setOpacity(1.0);
            }
        }
    }

    for (int y = 0; y < 8; ++y) {
        p.drawLine(0, y * 0x10, 8 * 0x10, y * 0x10);
    }

    for (int x = 0; x < 8; ++x) {
        p.drawLine(x * 0x10, 0, x * 0x10, 8 * 0x10);
    }
}

void ChunkEditor::keyPressEvent(QKeyEvent *event)
{
    bool ctrlDownL  = false;
    bool altDownL   = false;
    bool shiftDownL = false;
    if (event->key() == Qt::Key_Control)
        ctrlDownL = true;
    if (event->key() == Qt::Key_Alt)
        altDownL = true;
    if (event->key() == Qt::Key_Shift)
        shiftDownL = true;

    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
        && event->key() == Qt::Key_C) {
        copiedChunk = selectedChunk;
        setStatus("copied chunk: " + QString::number(copiedChunk));
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

        ui->chunkList->item(selectedChunk)
            ->setIcon(QPixmap::fromImage(chunks->chunks[selectedChunk].getImage(tileList)));
    }
}

#include "moc_chunkeditor.cpp"
