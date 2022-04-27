#include "includes.hpp"
#include "ui_chunkeditor.h"

ChunkEditor::ChunkEditor(FormatHelpers::Chunks *chk, QList<QImage> &chunkList, QList<QImage> &tiles,
                         byte gameVer, QWidget *parent)
    : QDialog(parent), ui(new Ui::ChunkEditor), chunks(chk), tileList(tiles), chunkImgList(chunkList)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    ui->setupUi(this);

    this->setWindowTitle("Chunk Editor");

    if (!chk)
        return;

    viewer = new ChunkViewer(&selectedChunk, &selectedTile, chunks, tiles);
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(viewer);

    ui->chunkList->clear();
    for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); ++c) {
        auto *item = new QListWidgetItem(QString::number(c), ui->chunkList);
        item->setIcon(QPixmap::fromImage(chunkList[c]));
    }

    ui->tileList->clear();
    for (int t = 0; t < tiles.count(); ++t) {
        auto *item = new QListWidgetItem(QString::number(t), ui->tileList);
        item->setIcon(QPixmap::fromImage(tiles[t]));
    }

    chunkIDs.clear();
    changedChunks.clear();
    for (int c = 0; c < (gameVer == ENGINE_v1 ? 0x100 : 0x200); ++c) {
        chunkIDs.append(c);
        changedChunks.append(false);
    }

    QList<QString> solidityv3 = { "Solid (All)", "Solid (Top Only)", "Solid (All Except Top)",
                                  "Not Solid" };
    QList<QString> solidityv4 = { "Solid (All)", "Solid (Top Only, Grippable)",
                                  "Solid (All Except Top)", "Not Solid",
                                  "Solid (Top Only, Not Grippable)" };

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

        changedChunks[selectedChunk] = true;
    };

    connect(viewer, &ChunkViewer::tileChanged, tileChangedCB);

    connect(viewer, &ChunkViewer::tileDrawn, [this, tiles, tileChangedCB](float mx, float my) {
        selectedTile.x = mx / 16;
        selectedTile.y = my / 16;

        if (selectedTile.x >= 8 || selectedTile.x < 0)
            selectedTile.x = -1;

        if (selectedTile.y >= 8 || selectedTile.y < 0)
            selectedTile.y = -1;

        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1 || selectedDrawTile == -1)
            return;

        chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].visualPlane =
            defaultVisualPlane;
        chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].direction =
            (byte)defaultFlip.x | ((byte)defaultFlip.y << 1);
        chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].tileIndex =
            (ushort)selectedDrawTile;
        chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].solidityA = defaultSolidA;
        chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].solidityB = defaultSolidB;

        viewer->repaint();

        chunkImgList[selectedChunk] = chunks->chunks[selectedChunk].getImage(tiles);
        ui->chunkList->item(selectedChunk)->setIcon(QPixmap::fromImage(chunkImgList[selectedChunk]));

        tileChangedCB();
    });

    connect(viewer, &ChunkViewer::tileCopy, [this, tiles](float mx, float my) {
        selectedTile.x = mx / 16;
        selectedTile.y = my / 16;

        if (selectedTile.x >= 8 || selectedTile.x < 0)
            selectedTile.x = -1;

        if (selectedTile.y >= 8 || selectedTile.y < 0)
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

        chunkImgList[selectedChunk] = chunks->chunks[selectedChunk].getImage(tiles);
        ui->chunkList->item(selectedChunk)->setIcon(QPixmap::fromImage(chunkImgList[selectedChunk]));

        changedChunks[selectedChunk] = true;
    });
    connect(ui->flipY, &QCheckBox::toggled, [this, tiles](bool v) {
        if (selectedChunk < 0 || selectedTile.x == -1 || selectedTile.y == -1)
            return;

        Utils::setBit(chunks->chunks[selectedChunk].tiles[selectedTile.y][selectedTile.x].direction, v,
                      1);
        viewer->repaint();

        chunkImgList[selectedChunk] = chunks->chunks[selectedChunk].getImage(tiles);
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

    connect(ui->defVisPlane, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int v) { defaultVisualPlane = v; });

    connect(ui->defSolidA, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int v) { defaultSolidA = v; });

    connect(ui->defSolidB, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int v) { defaultSolidB = v; });

    connect(ui->exportImg, &QPushButton::clicked, [this] {
        QFileDialog filedialog(this, tr("Select folder to place images"), "", "");
        filedialog.setFileMode(QFileDialog::Directory);
        if (filedialog.exec() == QDialog::Accepted) {
            QString path = filedialog.selectedFiles()[0];
            int id       = 0;
            setStatus("Exporting chunks as images...");
            for (int c = 0; c < 0x200; ++c) {
                chunks->chunks[c].getImage(tileList).save(QString(path + "/Chunk %1.png").arg(id++));
                setStatusProgress(c / (float)0x200);
            }
            setStatus(QString("Exported chunks to %1/").arg(path));
        }
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

        // doAction("Moved chunk", true);

        ui->chunkList->setCurrentRow(n);
    };

    connect(ui->upChunk, &QToolButton::clicked, [moveChunk] { moveChunk(-1); });

    connect(ui->downChunk, &QToolButton::clicked, [moveChunk] { moveChunk(1); });

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

        case QEvent::MouseButtonRelease: mouseReleaseEvent((QMouseEvent *)e); break;

        case QEvent::Paint: paintEvent((QPaintEvent *)e); break;

        case QEvent::Wheel: {
            QWheelEvent *wEvent = static_cast<QWheelEvent *>(e);
            if (wEvent->modifiers() & Qt::ControlModifier) {
                if (wEvent->angleDelta().y() > 0 && zoom < 20)
                    zoom += 1;
                else if (wEvent->angleDelta().y() < 0 && zoom > 1.5)
                    zoom -= 1;

                // round to nearest whole number
                zoom = (int)zoom;
                if (zoom < 1)
                    zoom = 1;
            }
            else {
                if (wEvent->angleDelta().y() > 0 && zoom < 20)
                    zoom *= 1.1f;
                else if (wEvent->angleDelta().y() < 0 && zoom > 0.5)
                    zoom /= 1.1f;
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
    float w = this->width(), h = this->height();
    float originX = w / 2, originY = h / 2;
    originX -= offset.x;
    originY -= offset.y;
    originX -= (4 * 16);
    originY -= (4 * 16);

    float chunkPosX = event->pos().x() - originX;
    float chunkPosY = event->pos().y() - originY;

    chunkPosX *= (1.0f / zoom);
    chunkPosY *= (1.0f / zoom);

    printLog(QString("pos(%1, %2), origin(%3, %4), mousePos(%5, %6)")
                 .arg(chunkPosX)
                 .arg(chunkPosY)
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

    if (mouseDownR) {
        selection->x = chunkPosX / 16;
        selection->y = chunkPosY / 16;

        if (selection->x >= 8 || selection->x < 0)
            selection->x = -1;

        if (selection->y >= 8 || selection->y < 0)
            selection->y = -1;

        emit tileChanged();

        repaint();
    }

    // if (mouseDownM)
    //     setCursor(Qt::ClosedHandCursor);

    // if (mouseDownM) {
    //     emit tileCopy(chunkPosX, chunkPosY);
    // }

    if (mouseDownL) {
        emit tileDrawn(chunkPosX, chunkPosY);
    }
}

void ChunkViewer::mouseMoveEvent(QMouseEvent *event)
{
    float w = this->width(), h = this->height();
    float originX = w / 2, originY = h / 2;
    originX -= offset.x;
    originY -= offset.y;
    originX -= (4 * 16);
    originY -= (4 * 16);

    float chunkPosX = event->pos().x() - originX;
    float chunkPosY = event->pos().y() - originY;

    chunkPosX *= (1.0f / zoom);
    chunkPosY *= (1.0f / zoom);

    if (mouseDownM) {
        offset.x -= event->pos().x() - reference.x();
        offset.y -= event->pos().y() - reference.y();
        reference = event->pos();

        repaint();
    }

    if (mouseDownR) {
        selection->x = chunkPosX / 16;
        selection->y = chunkPosY / 16;

        if (selection->x >= 8 || selection->x < 0)
            selection->x = -1;

        if (selection->y >= 8 || selection->y < 0)
            selection->y = -1;

        emit tileChanged();

        repaint();
    }

    // if (mouseDownM) {
    //     emit tileCopy(chunkPosX, chunkPosY);
    // }

    if (mouseDownL) {
        emit tileDrawn(chunkPosX, chunkPosY);
    }
}

void ChunkViewer::mouseReleaseEvent(QMouseEvent *event)
{
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
    originX -= (4 * 16);
    originY -= (4 * 16);

    originX *= (1.0f / zoom);
    originY *= (1.0f / zoom);

    const QBrush brush = p.brush();
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto &tile = chunks->chunks[*cSel].tiles[y][x];

            bool fx = (tile.direction & 1) == 1;
            bool fy = (tile.direction & 2) == 2;
            p.drawImage(QPointF(originX + x * 0x10, originY + y * 0x10),
                        tiles[tile.tileIndex].mirrored(fx, fy));

            if (y == selection->y && x == selection->x) {
                p.setBrush(qApp->palette().highlight());
                p.setOpacity(0.5);
                p.drawRect(QRectF(originX + x * 0x10, originY + y * 0x10, 0x10, 0x10));
                p.setBrush(brush);
                p.setOpacity(1.0);
            }
        }
    }

    for (int y = 0; y < 8; ++y)
        p.drawLine(QLineF(originX + 0, originY + y * 0x10, originX + 8 * 0x10, originY + y * 0x10));

    for (int x = 0; x < 8; ++x)
        p.drawLine(QLineF(originX + x * 0x10, originY + 0, originX + x * 0x10, originY + 8 * 0x10));
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

        changedChunks[selectedChunk] = true;
        ui->chunkList->item(selectedChunk)
            ->setIcon(QPixmap::fromImage(chunks->chunks[selectedChunk].getImage(tileList)));
    }
}

#include "moc_chunkeditor.cpp"
