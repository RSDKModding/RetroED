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
    void *m_clipboard    = nullptr;
    byte m_clipboardType = COPY_NONE;
    int m_clipboardInfo  = 0;

    void createEntityList();
    void createScrollList();

    void exportRSDKv5(ExportRSDKv5Scene *dlg);

    Ui::SceneEditor *ui;
};

#endif // SCENEEDITOR_H
