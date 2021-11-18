#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QFrame>
#include <QGraphicsPixmapItem>
#include "image-viewer-global.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

namespace pal
{

class PixmapItem;
class GraphicsView;

/**
 * @brief ImageViewer displays images and allows basic interaction with it
 */
class IMAGE_VIEWER_PUBLIC ImageViewer : public QFrame
{
    Q_OBJECT

public:
    /**
     * ToolBar visibility
     */
    enum class ToolBarMode { Visible, Hidden, AutoHidden };

public:
    explicit ImageViewer(QWidget *parent = nullptr);

    /// Text displayed on the left side of the toolbar
    QString text() const;

    /// The currently displayed image
    const QImage &image() const;

    /// Access to the pixmap so that other tools can add things to it
    const PixmapItem *pixmapItem() const;
    PixmapItem *pixmapItem();

    PixmapItem *setPixmapItem(PixmapItem *item);

    /// Add a tool to the toolbar
    void addTool(QWidget *tool);

    /// Toolbar visibility
    ToolBarMode toolBarMode() const;
    void setToolBarMode(ToolBarMode mode);

    /// Anti-aliasing
    bool isAntialiasingEnabled() const;
    void enableAntialiasing(bool on = true);

    void repaintView();

public slots:
    void setText(const QString &txt);
    void setImage(const QImage &);

    void zoomFit();
    void zoomOriginal();
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);
    void mouseAt(int x, int y);

signals:
    void imageChanged();
    void zoomChanged(double scale);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void setMatrix();
    void makeToolbar();

private:
    int m_zoom_level;
    QLabel *m_text_label;
    QLabel *m_pixel_value;
    GraphicsView *m_view;
    PixmapItem *m_pixmap;
    QWidget *m_toolbar;
    bool m_fit;
    ToolBarMode m_bar_mode;
};

class IMAGE_VIEWER_PUBLIC PixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    PixmapItem(QGraphicsItem *parent = nullptr);
    const QImage &image() const { return m_image; }

    int mouseX = 0;
    int mouseY = 0;
    QRect rect = QRect(0, 0, 0, 0);

public slots:
    void setImage(QImage im);

signals:
    void imageChanged(const QImage &);
    void sizeChanged(int w, int h);
    void mouseMoved(int x, int y);
    void mouseDownL();
    void mouseDownM();
    void mouseDownR();
    void mouseUpL();
    void mouseUpM();
    void mouseUpR();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QImage m_image;
};

} // namespace pal

#endif // IMAGEVIEWER_H
