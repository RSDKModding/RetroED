#include "includes.hpp"
#include "ui_chunkeditor.h"

ChunkEditor::ChunkEditor(FormatHelpers::Chunks *chk, QList<QImage> &chunks, QList<QImage> &tiles,
                         bool v1, QWidget *parent)
    : m_chunks(chk), QDialog(parent), ui(new Ui::ChunkEditor)
{
    ui->setupUi(this);

    this->setWindowTitle("Chunks Editor");

    if (!chk)
        return;

    m_viewer = new ChunkViewer(&m_selectedChunk, &m_selectedTile, m_chunks, tiles);
    m_viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->viewerFrame->layout()->addWidget(m_viewer);

    ui->chunkList->clear();
    for (int c = 0; c < (v1 ? 0x100 : 0x200); ++c) {
        auto *item = new QListWidgetItem(QString::number(c), ui->chunkList);
        item->setIcon(QPixmap::fromImage(chunks[c]));
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

        m_selectedChunk = c;

        if (c == -1)
            return;

        ui->flipX->blockSignals(true);
        ui->flipY->blockSignals(true);
        ui->visualPlane->blockSignals(true);
        ui->tileIndex->blockSignals(true);
        ui->solidityA->blockSignals(true);
        ui->solidityB->blockSignals(true);

        ui->flipX->setChecked(Utils::getBit(
            m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_direction,
            0));
        ui->flipY->setChecked(Utils::getBit(
            m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_direction,
            1));

        ui->visualPlane->setCurrentIndex(m_chunks->m_chunks[m_selectedChunk]
                                             .m_tiles[m_selectedTile.y][m_selectedTile.x]
                                             .m_visualPlane);

        ui->tileIndex->setValue(m_chunks->m_chunks[m_selectedChunk]
                                    .m_tiles[m_selectedTile.y][m_selectedTile.x]
                                    .m_tileIndex);

        ui->solidityA->setCurrentIndex(m_chunks->m_chunks[m_selectedChunk]
                                           .m_tiles[m_selectedTile.y][m_selectedTile.x]
                                           .m_solidityA);
        ui->solidityB->setCurrentIndex(m_chunks->m_chunks[m_selectedChunk]
                                           .m_tiles[m_selectedTile.y][m_selectedTile.x]
                                           .m_solidityB);

        ui->flipX->blockSignals(false);
        ui->flipY->blockSignals(false);
        ui->visualPlane->blockSignals(false);
        ui->tileIndex->blockSignals(false);
        ui->solidityA->blockSignals(false);
        ui->solidityB->blockSignals(false);

        m_viewer->repaint();
    };
    chunkRowChanged(0);
    connect(ui->chunkList, &QListWidget::currentRowChanged, chunkRowChanged);

    auto tileRowChanged = [this, tiles](int c) { m_selectedDrawTile = c; };
    tileRowChanged(-1);
    connect(ui->tileList, &QListWidget::currentRowChanged, tileRowChanged);

    connect(m_viewer, &ChunkViewer::tileChanged, [this] {
        if (m_selectedChunk < 0 || m_selectedTile.x == -1 || m_selectedTile.y == -1)
            return;

        ui->flipX->blockSignals(true);
        ui->flipY->blockSignals(true);
        ui->visualPlane->blockSignals(true);
        ui->tileIndex->blockSignals(true);
        ui->solidityA->blockSignals(true);
        ui->solidityB->blockSignals(true);

        ui->flipX->setChecked(Utils::getBit(
            m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_direction,
            0));
        ui->flipY->setChecked(Utils::getBit(
            m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_direction,
            1));

        ui->visualPlane->setCurrentIndex(m_chunks->m_chunks[m_selectedChunk]
                                             .m_tiles[m_selectedTile.y][m_selectedTile.x]
                                             .m_visualPlane);

        ui->tileIndex->setValue(m_chunks->m_chunks[m_selectedChunk]
                                    .m_tiles[m_selectedTile.y][m_selectedTile.x]
                                    .m_tileIndex);

        ui->solidityA->setCurrentIndex(m_chunks->m_chunks[m_selectedChunk]
                                           .m_tiles[m_selectedTile.y][m_selectedTile.x]
                                           .m_solidityA);
        ui->solidityB->setCurrentIndex(m_chunks->m_chunks[m_selectedChunk]
                                           .m_tiles[m_selectedTile.y][m_selectedTile.x]
                                           .m_solidityB);

        ui->flipX->blockSignals(false);
        ui->flipY->blockSignals(false);
        ui->visualPlane->blockSignals(false);
        ui->tileIndex->blockSignals(false);
        ui->solidityA->blockSignals(false);
        ui->solidityB->blockSignals(false);
    });

    connect(m_viewer, &ChunkViewer::tileDrawn, [this, tiles](float mx, float my) {
        if (m_selectedChunk < 0 || m_selectedTile.x == -1 || m_selectedTile.y == -1
            || m_selectedDrawTile == -1)
            return;

        m_selectedTile.x = mx / 16;
        m_selectedTile.y = my / 16;

        if (m_selectedTile.x > 0x80)
            m_selectedTile.x = -1;

        if (m_selectedTile.y > 0x80)
            m_selectedTile.y = -1;

        m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_tileIndex =
            (ushort)m_selectedDrawTile;
        m_viewer->repaint();

        ui->chunkList->item(m_selectedChunk)
            ->setIcon(QPixmap::fromImage(m_chunks->m_chunks[m_selectedChunk].getImage(tiles)));
    });

    connect(ui->flipX, &QCheckBox::toggled, [this, tiles](bool v) {
        if (m_selectedChunk < 0 || m_selectedTile.x == -1 || m_selectedTile.y == -1)
            return;

        Utils::setBit(
            m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_direction,
            v, 0);
        m_viewer->repaint();

        ui->chunkList->item(m_selectedChunk)
            ->setIcon(QPixmap::fromImage(m_chunks->m_chunks[m_selectedChunk].getImage(tiles)));
    });
    connect(ui->flipY, &QCheckBox::toggled, [this, tiles](bool v) {
        if (m_selectedChunk < 0 || m_selectedTile.x == -1 || m_selectedTile.y == -1)
            return;

        Utils::setBit(
            m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_direction,
            v, 1);
        m_viewer->repaint();

        ui->chunkList->item(m_selectedChunk)
            ->setIcon(QPixmap::fromImage(m_chunks->m_chunks[m_selectedChunk].getImage(tiles)));
    });

    connect(ui->visualPlane, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) {
        if (m_selectedChunk < 0 || m_selectedTile.x == -1 || m_selectedTile.y == -1)
            return;

        m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_visualPlane =
            (byte)v;
    });

    connect(ui->tileIndex, QOverload<int>::of(&QSpinBox::valueChanged), [this, tiles](int v) {
        if (m_selectedChunk < 0 || m_selectedTile.x == -1 || m_selectedTile.y == -1)
            return;

        m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_tileIndex =
            (ushort)v;
        m_viewer->repaint();

        ui->chunkList->item(m_selectedChunk)
            ->setIcon(QPixmap::fromImage(m_chunks->m_chunks[m_selectedChunk].getImage(tiles)));
    });

    connect(ui->solidityA, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) {
        if (m_selectedChunk < 0 || m_selectedTile.x == -1 || m_selectedTile.y == -1)
            return;

        m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_solidityA =
            (byte)v;
        m_viewer->repaint();
    });

    connect(ui->solidityB, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int v) {
        if (m_selectedChunk < 0 || m_selectedTile.x == -1 || m_selectedTile.y == -1)
            return;

        m_chunks->m_chunks[m_selectedChunk].m_tiles[m_selectedTile.y][m_selectedTile.x].m_solidityB =
            (byte)v;
        m_viewer->repaint();
    });

    for (QWidget *w : findChildren<QWidget *>()) {
        w->installEventFilter(this);
    }
}

ChunkEditor::~ChunkEditor() { delete ui; }

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
    bool mouseDownR = false;
    if ((event->button() & Qt::LeftButton) == Qt::LeftButton)
        mouseDownL = true;
    if ((event->button() & Qt::RightButton) == Qt::RightButton)
        mouseDownR = true;

    if (mouseDownR) {
        m_sel->x = (mousePosX * m_zoom) / 16;
        m_sel->y = (mousePosY * m_zoom) / 16;

        if (m_sel->x > 0x80)
            m_sel->x = -1;

        if (m_sel->y > 0x80)
            m_sel->y = -1;

        emit tileChanged();

        repaint();
    }

    if (mouseDownL) {
        emit tileDrawn(mousePosX * m_zoom, mousePosY * m_zoom);
    }
}

void ChunkViewer::mouseMoveEvent(QMouseEvent *event)
{
    float mousePosX = event->pos().x();
    float mousePosY = event->pos().y();

    bool mouseDownL = false;
    bool mouseDownR = false;
    if ((event->button() & Qt::LeftButton) == Qt::LeftButton)
        mouseDownL = true;
    if ((event->button() & Qt::RightButton) == Qt::RightButton)
        mouseDownR = true;

    if (mouseDownR) {
        m_sel->x = (mousePosX * m_zoom) / 16;
        m_sel->y = (mousePosY * m_zoom) / 16;

        if (m_sel->x > 0x80)
            m_sel->x = -1;

        if (m_sel->y > 0x80)
            m_sel->y = -1;

        emit tileChanged();

        repaint();
    }

    if (mouseDownL) {
        emit tileDrawn(mousePosX * m_zoom, mousePosY * m_zoom);
    }
}

void ChunkViewer::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter p(this);
    p.scale(m_zoom, m_zoom);

    const QBrush brush = p.brush();
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto &tile = m_chunks->m_chunks[*m_cSel].m_tiles[y][x];

            bool fx = (tile.m_direction & 1) == 1;
            bool fy = (tile.m_direction & 2) == 2;
            p.drawImage(x * 0x10, y * 0x10, m_tiles[tile.m_tileIndex].mirrored(fx, fy));

            if (y == m_sel->y && x == m_sel->x) {
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

#include "moc_chunkeditor.cpp"
