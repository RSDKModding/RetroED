#include "image-viewer.hpp"
#include <cmath>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneHoverEvent>
#include <QWheelEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>

namespace pal
{

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::NoModifier) {
        if (event->angleDelta().y() > 0)
            m_viewer->zoomIn(3);
        else if (event->angleDelta().y() < 0)
            m_viewer->zoomOut(3);
        event->accept();
    }
    else
        QGraphicsView::wheelEvent(event);
};

ImageViewer::ImageViewer(QWidget *parent)
    : QFrame(parent), m_zoom_level(0), m_fit(true), m_bar_mode(ToolBarMode::Visible)
{
    auto scene = new QGraphicsScene(this);
    view       = new GraphicsView(this);
    view->setScene(scene);

    // graphic object holding the image buffer
    m_pixmap = new PixmapItem;
    scene->addItem(m_pixmap);
    connect(m_pixmap, SIGNAL(mouseMoved(int, int)), SLOT(mouseAt(int, int)));

    makeToolbar();

    auto box = new QVBoxLayout;
    box->setContentsMargins(5, 0, 5, 0);
    box->addWidget(m_toolbar);
    box->addWidget(view, 1);
    setLayout(box);
}

// toolbar with a few quick actions and display information
void ImageViewer::makeToolbar()
{
    // text and value at pixel
    m_text_label = new QLabel(this);
    m_text_label->setStyleSheet(QString("QLabel { font-weight: bold; }"));
    m_pixel_value = new QLabel(this);

    auto fit = new QToolButton(this);
    fit->setToolTip(tr("Fit image to window"));
    fit->setIcon(QIcon(":/icons/zoom-fit-best.png"));
    connect(fit, SIGNAL(clicked()), SLOT(zoomFit()));

    auto orig = new QToolButton(this);
    orig->setToolTip(tr("Resize image to its original size"));
    orig->setIcon(QIcon(":/icons/zoom-original.png"));
    connect(orig, SIGNAL(clicked()), SLOT(zoomOriginal()));

    m_toolbar = new QWidget;
    auto box  = new QHBoxLayout(m_toolbar);
    m_toolbar->setContentsMargins(0, 0, 0, 0);
    box->setContentsMargins(0, 0, 0, 0);
    box->addWidget(m_text_label);
    box->addStretch(1);
    box->addWidget(m_pixel_value);
    box->addWidget(fit);
    box->addWidget(orig);
}

QString ImageViewer::text() const { return m_text_label->text(); }

void ImageViewer::setText(const QString &txt) { m_text_label->setText(txt); }

const QImage &ImageViewer::image() const { return m_pixmap->image(); }

void ImageViewer::setImage(const QImage &im)
{
    m_pixmap->setImage(im);

    if (m_fit)
        zoomFit();

    emit imageChanged();
}

const PixmapItem *ImageViewer::pixmapItem() const { return m_pixmap; }

PixmapItem *ImageViewer::pixmapItem() { return m_pixmap; }

PixmapItem *ImageViewer::setPixmapItem(PixmapItem *item)
{
    // delete m_pixmap;
    m_pixmap   = item;
    auto scene = new QGraphicsScene(this);
    view       = new GraphicsView(this);
    scene->addItem(m_pixmap);
    return item;
}

ImageViewer::ToolBarMode ImageViewer::toolBarMode() const { return m_bar_mode; }

void ImageViewer::setToolBarMode(ToolBarMode mode)
{
    m_bar_mode = mode;
    if (mode == ToolBarMode::Hidden)
        m_toolbar->hide();
    else if (mode == ToolBarMode::Visible)
        m_toolbar->show();
    else
        m_toolbar->setVisible(underMouse());
}

bool ImageViewer::isAntialiasingEnabled() const { return view->renderHints() & QPainter::Antialiasing; }

void ImageViewer::enableAntialiasing(bool on) { view->setRenderHint(QPainter::Antialiasing, on); }

void ImageViewer::repaintView() { view->repaint(); }

void ImageViewer::addTool(QWidget *tool) { m_toolbar->layout()->addWidget(tool); }

void ImageViewer::setMatrix()
{
    qreal scale = std::pow(2.0, m_zoom_level / 10.0);

    QTransform matrix;
    matrix.scale(scale, scale);

    view->setTransform(matrix);
    emit zoomChanged(view->transform().m11());
}

void ImageViewer::zoomFit()
{
    view->fitInView(m_pixmap, Qt::KeepAspectRatio);
    m_zoom_level = int(10.0 * std::log2(view->transform().m11()));
    m_fit        = true;
    emit zoomChanged(view->transform().m11());
}

void ImageViewer::zoomOriginal()
{
    m_zoom_level = 0;
    m_fit        = false;
    setMatrix();
}

void ImageViewer::zoomIn(int level)
{
    m_zoom_level += level;
    m_fit = false;
    setMatrix();
}

void ImageViewer::zoomOut(int level)
{
    m_zoom_level -= level;
    m_fit = false;
    setMatrix();
}

void ImageViewer::mouseAt(int x, int y)
{
    if (m_pixmap->image().valid(x, y)) {
        QRgb rgb = m_pixmap->image().pixel(x, y);
        auto s   = QString("[%1, %2] (%3, %4, %5)")
                     .arg(x)
                     .arg(y)
                     .arg(qRed(rgb))
                     .arg(qGreen(rgb))
                     .arg(qBlue(rgb));
        m_pixel_value->setText(s);
    }
    else
        m_pixel_value->setText(QString());
}

void ImageViewer::enterEvent(QEvent *event)
{
    QFrame::enterEvent(event);
    if (m_bar_mode == ToolBarMode::AutoHidden) {
        m_toolbar->show();
        if (m_fit)
            zoomFit();
    }
}

void ImageViewer::leaveEvent(QEvent *event)
{
    QFrame::leaveEvent(event);
    if (m_bar_mode == ToolBarMode::AutoHidden) {
        m_toolbar->hide();
        if (m_fit)
            zoomFit();
    }
}

void ImageViewer::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    if (m_fit)
        zoomFit();
}

void ImageViewer::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);
    if (m_fit)
        zoomFit();
}

PixmapItem::PixmapItem(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{
    setAcceptHoverEvents(true);
}

void PixmapItem::setImage(QImage im)
{
    if (im.isNull()) {
        m_image.fill(Qt::white);
        im = m_image.copy();
    }
    std::swap(m_image, im);

    setPixmap(QPixmap::fromImage(m_image));

    if (m_image.size() != im.size())
        emit sizeChanged(m_image.width(), m_image.height());

    emit imageChanged(m_image);
}

void PixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    auto pos = event->pos();
    mouseX   = pos.x();
    mouseY   = pos.y();
    emit mouseMoved(int(pos.x()), int(pos.y()));
    QGraphicsItem::hoverMoveEvent(event);
}

void PixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);
    if (rect.width() > 0 && rect.height() > 0) {
        painter->setBrush(qApp->palette().highlight());
        painter->setOpacity(0.5);
        painter->drawRect(rect);
    }
}

} // namespace pal

#include "image-viewer.moc"
