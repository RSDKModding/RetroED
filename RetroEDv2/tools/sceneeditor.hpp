#ifndef SCENEEDITOR_H
#define SCENEEDITOR_H

#include <QWidget>

class ChunkSelector;

namespace Ui
{
class SceneEditor;
}

class SceneEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SceneEditor(QWidget *parent = nullptr);
    ~SceneEditor();

    bool m_mouseDownL = false;
    bool m_mouseDownM = false;
    bool m_mouseDownR = false;
    bool m_ctrlDownL  = false;
    bool m_altDownL   = false;
    bool m_shiftDownL = false;

    Vector2<int> m_snapSize;

    SceneViewer *viewer;
    SceneProperties *scnProp       = nullptr;
    SceneLayerProperties *lyrProp  = nullptr;
    SceneTileProperties *tileProp  = nullptr;
    SceneObjectProperties *objProp = nullptr;
    SceneScrollProperties *scrProp = nullptr;
    ChunkSelector *chkProp         = nullptr;

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
    void *m_clipboard    = nullptr;
    byte m_clipboardType = COPY_NONE;
    int m_clipboardInfo  = 0;

    void createEntityList();
    void createScrollList();

    void exportRSDKv5(ExportRSDKv5Scene *dlg);

    void parseGameXML(byte gameType, QString path);

    Ui::SceneEditor *ui;
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

    SceneEditor *m_parent = nullptr;
};

#endif // SCENEEDITOR_H
