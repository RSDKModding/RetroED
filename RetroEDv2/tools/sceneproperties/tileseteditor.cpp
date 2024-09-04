#include "includes.hpp"
#include "ui_tileseteditor.h"
#include "tileseteditor.hpp"

#include "qgifimage.h"

TilesetEditor::TilesetEditor(QList<QImage> &tileList, QList<PaletteColor> &pal, QWidget *parent)
    : QDialog(parent), ui(new Ui::TilesetEditor), tiles(tileList), palette(pal)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);

    ui->setupUi(this);

    this->setWindowTitle("Tileset Editor");

    // remove question mark from the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVector<QRgb> clrTable;
    for (int i = 0; i < pal.count(); ++i) clrTable.append(qRgb(pal[i].r, pal[i].g, pal[i].b));

    // Add any filler tiles
    for (int t = tiles.count(); t < 0x400; ++t) {
        QImage tile = QImage(16, 16, QImage::Format_Indexed8);
        tile.setColorTable(clrTable);
        tiles.append(tile);
    }

    // Remove Excess tiles
    for (int t = tiles.count(); t > 0x400; --t) tiles.removeAt(t);

    viewer = new TilesetViewer(&selectedTile, tiles);
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

    auto tileRowChanged = [this](int c) {
        selectedTile = c;

        viewer->offset.x = 0;
        viewer->offset.y = 0;

        if (ui->upTile)
            ui->upTile->setDisabled(c == -1);
        if (ui->downTile)
            ui->downTile->setDisabled(c == -1);

        if (c == -1)
            return;

        if (ui->upTile)
            ui->upTile->setDisabled(c == 0);
        if (ui->downTile)
            ui->downTile->setDisabled(c == ui->tileList->count() - 1);

        viewer->repaint();
    };

    tileRowChanged(-1);
    connect(ui->tileList, &QListWidget::currentRowChanged, tileRowChanged);

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

    connect(ui->exportImg, &QPushButton::clicked, [this] {
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

    connect(ui->importImg, &QPushButton::clicked, [this, clrTable] {
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

            SetStatus("Importing tiles...", true);

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

                        changedTiles[pos] = true;
                        tiles.replace(pos++, tile);

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

                                int index = getColorIndex(img, clrTable, x + sx, y + sy);
                                tile.setPixel(sx, sy, index);
                            }
                        }

                        changedTiles[pos] = true;
                        tiles.replace(pos++, tile);

                        if (pos >= 0x400) {
                            x = img.width();
                            y = img.height();
                        }
                    }
                }
            }

            for (int p = startPos; p < pos; ++p) {
                ui->tileList->item(p)->setIcon(QPixmap::fromImage(tiles.at(p)));
            }

            SetStatus("Finished importing tiles!");
        }
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

void TilesetViewer::mousePressEvent(QMouseEvent *event)
{
    /*
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

    PrintLog(QString("pos(%1, %2), origin(%3, %4), mousePos(%5, %6)")
                 .arg(chunkPosX)
                 .arg(chunkPosY)
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

    // if (mouseDownM)
    //     setCursor(Qt::ClosedHandCursor);

    // if (mouseDownM) {
    // }

    if (mouseDownL) {
    }
}

void TilesetViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (mouseDownM) {
        offset.x -= event->pos().x() - reference.x();
        offset.y -= event->pos().y() - reference.y();
        reference = event->pos();

        repaint();
    }

    if (mouseDownM) {
    }

    if (mouseDownL) {
    }
}

void TilesetViewer::mouseReleaseEvent(QMouseEvent *event)
{
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
    originX -= 8;
    originY -= 8;

    originX *= 1.0f / zoom;
    originY *= 1.0f / zoom;

    const QBrush brush = p.brush();

    if (tSel && *tSel >= 0)
        p.drawImage(QPointF(originX, originY), tiles.at(*tSel));
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

#include "moc_tileseteditor.cpp"
