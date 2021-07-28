#ifndef SCENEEDITOR_V5_H
#define SCENEEDITOR_V5_H

#include <QWidget>

class ChunkSelector;

namespace Ui
{
class SceneEditorv5;
}

class SceneEditorv5 : public QWidget
{
    Q_OBJECT

public:
    explicit SceneEditorv5(QWidget *parent = nullptr);
    ~SceneEditorv5();

    bool m_mouseDownL = false;
    bool m_mouseDownM = false;
    bool m_mouseDownR = false;
    bool ctrlDownL    = false;
    bool altDownL     = false;
    bool shiftDownL   = false;

    Vector2<int> snapSize;

    SceneViewerv5 *viewer;
    SceneProperties *m_scnProp       = nullptr;
    SceneLayerProperties *m_lyrProp  = nullptr;
    SceneTileProperties *m_tileProp  = nullptr;
    SceneObjectProperties *m_objProp = nullptr;
    SceneScrollProperties *m_scrProp = nullptr;
    ChunkSelector *m_chkProp         = nullptr;

    void loadScene(QString scnPath, QString gcfPath, byte gameType);

protected:
    bool event(QEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

private:
    enum SceneManagerCopyTypes {
        COPY_NONE,
        COPY_LAYER,
        COPY_TILE,
        COPY_ENTITY,
        COPY_SCROLLINFO,
    };
    void *clipboard    = nullptr;
    byte clipboardType = COPY_NONE;
    int clipboardInfo  = 0;

    void createEntityList();
    void createScrollList();

    void exportRSDKv5(ExportRSDKv5Scene *dlg);

    Ui::SceneEditorv5 *ui;
};

class ChunkLabel : public QLabel
{
    Q_OBJECT
public:
    ChunkLabel(int *sel, int index, QWidget *parent) : QLabel(parent), m_sel(sel), m_index(index) {}

signals:
    void requestRepaint();

protected:
    void mousePressEvent(QMouseEvent *) override
    {
        *m_sel = m_index;
        emit requestRepaint();
    }
    void paintEvent(QPaintEvent *event) override
    {
        QLabel::paintEvent(event);
        QPainter p(this);
        if (m_index == *m_sel) {
            p.setBrush(qApp->palette().highlight());
            p.setOpacity(0.5);
            p.drawRect(this->rect());
        }
    }
    QSize sizeHint() const override { return QSize(0, 0); }

private:
    int *m_sel = nullptr;
    int m_index;
};

class ChunkSelector : public QWidget
{
    Q_OBJECT
public:
    ChunkSelector(QWidget *parent = nullptr);

    SceneEditorv5 *m_parent = nullptr;
};

#endif // SCENEEDITOR_V5_H
