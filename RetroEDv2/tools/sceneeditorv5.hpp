#ifndef SCENEEDITOR_V5_H
#define SCENEEDITOR_V5_H

#include <QWidget>

class TileSelector;

namespace Ui
{
class SceneEditorv5;
}

class SceneEditorv5 : public QWidget
{
    Q_OBJECT

public:
    class ActionState
    {
    public:
        QString name = "Action";

        QList<PaletteColour> tilePalette;
        QList<QImage> tiles;

        RSDKv5::GameConfig gameConfig;

        RSDKv5::Scene scene;
        RSDKv5::StageConfig stageConfig;

        RSDKv5::TileConfig tileconfig;

        // General Editing
        byte curTool            = SceneViewerv5::TOOL_MOUSE;
        bool selecting          = false;
        Vector2<float> mousePos = Vector2<float>(0.0f, 0.0f);

        // Layer Editing
        Vector2<float> tilePos = Vector2<float>(0.0f, 0.0f);
        Vector2<bool> tileFlip = Vector2<bool>(false, false);
        int selectedLayer      = -1;

        // Entities & Objects
        QList<SceneObject> objects;
        QList<SceneEntity> entities;

        // Collision
        bool showPlaneA = false;
        bool showPlaneB = false;

        // Entity Editing
        int selectedObject = -1; // placing
        int selectedEntity = -1; // viewing

        // Parallax Editing
        bool showParallax      = false;
        int selectedScrollInfo = -1;

        bool showGrid         = false;
        Vector2<int> gridSize = Vector2<int>(0x10, 0x10);
    };

    explicit SceneEditorv5(QWidget *parent = nullptr);
    ~SceneEditorv5();

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;
    bool ctrlDownL  = false;
    bool altDownL   = false;
    bool shiftDownL = false;

    Vector2<float> selectionOffset = Vector2<float>(0.0f, 0.0f);

    SceneViewerv5 *viewer             = nullptr;
    ScenePropertiesv5 *scnProp        = nullptr;
    SceneLayerPropertiesv5 *lyrProp   = nullptr;
    SceneTilePropertiesv5 *tileProp   = nullptr;
    SceneStampPropertiesv5 *stampProp = nullptr;
    SceneObjectPropertiesv5 *objProp  = nullptr;
    SceneScrollPropertiesv5 *scrProp  = nullptr;
    TileSelector *tileSel             = nullptr;
    TilesetEditor *tsetEdit           = nullptr;

    void loadScene(QString scnPath, QString gcfPath, byte gameType);

    inline void updateTitle(bool modified)
    {
        this->modified = modified;
        if (modified)
            emit titleChanged(tabTitle + " *");
        else
            emit titleChanged(tabTitle);
    }

    QString dataPath = "";

    // Game Data Storage
    DataStorage dataStorage[DATASET_MAX];

    void undoAction();
    void redoAction();
    void resetAction();
    void doAction(QString name = "Action", bool setModified = true);
    void clearActions();

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

    void createEntityList();
    void createScrollList();

    void parseGameXML(QString path);

    Ui::SceneEditorv5 *ui;

    QList<ActionState> actions;
    int actionIndex = 0;

    bool modified    = false;
    QString tabTitle = "Scene Editor (v5)";
};

class TileLabel : public QLabel
{
    Q_OBJECT
public:
    TileLabel(int *select, int tileIndex, QWidget *parent)
        : QLabel(parent), sel(select), index(tileIndex)
    {
    }

signals:
    void requestRepaint();

protected:
    void mousePressEvent(QMouseEvent *) override
    {
        *sel = index;
        emit requestRepaint();
    }
    void paintEvent(QPaintEvent *event) override
    {
        QLabel::paintEvent(event);
        QPainter p(this);
        if (index == *sel) {
            p.setBrush(qApp->palette().highlight());
            p.setOpacity(0.5);
            p.drawRect(this->rect());
        }
    }
    QSize sizeHint() const override { return QSize(0, 0); }

private:
    int *sel = nullptr;
    int index;
};

class TileSelector : public QWidget
{
    Q_OBJECT
public:
    TileSelector(QWidget *parent = nullptr);

    SceneEditorv5 *parentPtr = nullptr;
};

#endif // SCENEEDITOR_V5_H
