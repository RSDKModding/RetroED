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
    ScenePropertiesv5 *m_scnProp       = nullptr;
    SceneLayerPropertiesv5 *m_lyrProp  = nullptr;
    SceneTilePropertiesv5 *m_tileProp  = nullptr;
    SceneObjectPropertiesv5 *m_objProp = nullptr;
    SceneScrollPropertiesv5 *m_scrProp = nullptr;

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

#endif // SCENEEDITOR_V5_H
