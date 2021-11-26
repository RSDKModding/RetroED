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
    class ActionState
    {
    public:
        QString name = "Action";

        QList<PaletteColour> tilePalette;
        QList<QImage> tiles;
        QList<QImage> chunks;

        FormatHelpers::Gameconfig gameConfig;

        FormatHelpers::Scene scene;
        FormatHelpers::Background background;
        FormatHelpers::Chunks chunkset;
        FormatHelpers::Stageconfig stageConfig;

        RSDKv4::TileConfig tileconfig;

        QList<SceneViewer::ObjectInfo> objects;
        QList<SceneViewer::EntityInfo> entities;

        // General Editing
        byte curTool            = TOOL_MOUSE;
        bool selecting          = false;
        Vector2<float> mousePos = Vector2<float>(0.0f, 0.0f);

        // Layer Editing
        Vector2<float> tilePos = Vector2<float>(0.0f, 0.0f);
        Vector2<bool> tileFlip = Vector2<bool>(false, false);
        int selectedChunk      = -1;
        int selectedLayer      = -1;

        // Collision
        bool showPlaneA = false;
        bool showPlaneB = false;

        // Entity Editing
        int selectedObject = -1; // placing
        int selectedEntity = -1; // viewing

        // Parallax Editing
        bool showParallax      = false;
        int selectedScrollInfo = -1;

        // Camera
        Vector3<float> camPos = Vector3<float>(0.0f, 0.0f, 0.0f);

        bool showPixelGrid = false;
        bool showChunkGrid = false;
        bool showTileGrid  = false;

        // Compilerv2 compilerv2;
        // Compilerv3 compilerv3;
        // Compilerv4 compilerv4;
    };

    explicit SceneEditor(QWidget *parent = nullptr);
    ~SceneEditor();

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;
    bool ctrlDownL  = false;
    bool altDownL   = false;
    bool shiftDownL = false;

    Vector2<int> snapSize          = Vector2<int>(0, 0);
    Vector2<float> selectionOffset = Vector2<float>(0.0f, 0.0f);

    SceneViewer *viewer            = nullptr;
    SceneProperties *scnProp       = nullptr;
    SceneLayerProperties *lyrProp  = nullptr;
    SceneTileProperties *tileProp  = nullptr;
    SceneObjectProperties *objProp = nullptr;
    SceneScrollProperties *scrProp = nullptr;
    ChunkSelector *chkProp         = nullptr;
    TilesetEditor *tsetEdit        = nullptr;

    void loadScene(QString scnPath, QString gcfPath, byte gameType);

    inline void updateTitle(bool modified)
    {
        this->modified = modified;
        if (modified)
            emit titleChanged(tabTitle + " *");
        else
            emit titleChanged(tabTitle);
    }

signals:
    void titleChanged(QString title);

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

    QList<ActionState> actions;
    int actionIndex = 0;

    void createEntityList();
    void createScrollList();

    void exportRSDKv5(ExportRSDKv5Scene *dlg);

    void parseGameXML(byte gameType, QString path);

    void undoAction();
    void redoAction();
    void resetAction();
    void doAction(QString name = "Action", bool setModified = true);
    void clearActions();

    Ui::SceneEditor *ui;

    ChunkEditor *chunkEdit = nullptr;

    bool modified    = false;
    QString tabTitle = "Scene Editor";
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

    void refreshList();

    SceneEditor *parentWidget = nullptr;

private:
    QList<ChunkLabel *> labels;
};

#endif // SCENEEDITOR_H
