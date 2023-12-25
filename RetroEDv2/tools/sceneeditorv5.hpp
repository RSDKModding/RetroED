#pragma once

#include <QWidget>

#include "tools/sceneviewer.hpp"

class ScenePropertiesv5;
class SceneLayerPropertiesv5;
class SceneTilePropertiesv5;
class SceneStampPropertiesv5;
class SceneObjectPropertiesv5;
class SceneScrollPropertiesv5;
class TileSelector;
class TilesetEditor;

#include <RSDKv5/gameconfigv5.hpp>
#include <RSDKv5/scenev5.hpp>
#include <RSDKv5/tileconfigv5.hpp>
#include <RSDKv5/stageconfigv5.hpp>

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

        QList<PaletteColor> tilePalette;
        QList<QImage> tiles;

        RSDKv5::GameConfig gameConfig;

        RSDKv5::Scene scene;
        RSDKv5::StageConfig stageConfig;

        RSDKv5::TileConfig tileconfig;

        // General Editing
        byte curTool            = SceneViewer::TOOL_MOUSE;
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

    bool viewerActive = false; // prevents shortcut windows from opening twice when sceneViewer is active

    int initGameFilter = 0;

    Vector2<float> selectionOffset = Vector2<float>(0.0f, 0.0f);

    // The thingos
    RSDKv5::GameConfig gameConfig;
    RSDKv5::Scene scene;
    RSDKv5::StageConfig stageConfig;

    RSDKv5::TileConfig tileconfig;

    SceneViewer *viewer               = nullptr;
    ScenePropertiesv5 *scnProp        = nullptr;
    SceneLayerPropertiesv5 *lyrProp   = nullptr;
    SceneTilePropertiesv5 *tileProp   = nullptr;
    SceneStampPropertiesv5 *stampProp = nullptr;
    SceneObjectPropertiesv5 *objProp  = nullptr;
    SceneScrollPropertiesv5 *scrProp  = nullptr;
    TileSelector *tileSel             = nullptr;
    TilesetEditor *tsetEdit           = nullptr;

    GameEntityBasev1 createGameEntityv1;
    GameEntityBasev2 createGameEntityv2;
    GameEntityBasevU createGameEntityvU;
    SceneEntity createTempEntity;

    void CreateNewScene();
    void LoadScene(QString scnPath, QString gcfPath, byte gameType);
    void SaveScene(QString path);

    void SetTile(float x, float y);
    void ResetTools(byte tool);

    bool HandleKeyPress(QKeyEvent *event);
    bool HandleKeyRelease(QKeyEvent *event);

    void CreateScrollList(bool update = false);

    QList<GameLink> gameLinks;
    GameObjectInfo *GetObjectInfo(QString name);

    QString dataPath = "";

    // Event stuff
    bool waitForRelease = false;

    // Game Data Storage
    DataStorage dataStorage[DATASET_MAX];

    inline void UpdateTitle(bool modified)
    {
        this->modified = modified;
        if (modified)
            emit TitleChanged(tabTitle + " *");
        else
            emit TitleChanged(tabTitle);
    }

    void UndoAction();
    void RedoAction();
    void ResetAction();
    void DoAction(QString name = "Action", bool setModified = true);
    void ClearActions();

signals:
    void TitleChanged(QString title);

public slots:
    void updateType(SceneEntity *entity, byte type);
    void updateTileSel();

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

    int AddEntity(int type, float x, float y);
    void DeleteEntity(int slot, bool updateUI = false);

    void FilterObjectList(QString filter);
    void FilterEntityList(QString filter);

    void CreateEntityList(int startSlot = -1);
    void SetupObjects();

    void UnloadGameLinks();
    void LoadGameLinks();
    void InitGameLink();

    void SetGameEntityVariables(SceneEntity *entity, void *gameEntity);
    void GetGameEntityVariables(SceneEntity *entity, void *gameEntity);

    bool CallGameEvent(QString objName, byte eventID, SceneEntity *entity);

    void ParseGameXML(QString path);

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
    TileLabel(ushort *select, int tileIndex, QWidget *parent)
        : QLabel(parent), sel(select), index(tileIndex)
    {
    }

signals:
    void tileSelected();
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
    ushort *sel = nullptr;
    int index;
};

class TileSelector : public QWidget
{
    Q_OBJECT
public:
    TileSelector(QWidget *parent = nullptr);

    void RefreshList();

private:
    TileLabel *tiles[0x400];

    SceneEditorv5 *parentPtr = nullptr;
};
