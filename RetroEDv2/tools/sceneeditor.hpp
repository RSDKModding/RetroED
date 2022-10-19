#pragma once

#include <QWidget>

#include "tools/sceneviewer.hpp"

#include "tools/compiler/compilerv2.hpp"
#include "tools/compiler/compilerv3.hpp"
#include "tools/compiler/compilerv4.hpp"

class SceneProperties;
class SceneLayerProperties;
class SceneTileProperties;
class SceneObjectProperties;
class SceneScrollProperties;
class ChunkEditor;
class ChunkSelector;
class TilesetEditor;

#include <RSDKv5/tileconfigv5.hpp>

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

        QList<PaletteColor> tilePalette;
        QList<QImage> tiles;
        QList<QImage> chunks;

        FormatHelpers::GameConfig gameConfig;

        FormatHelpers::Scene scene;
        FormatHelpers::Background background;
        FormatHelpers::Chunks chunkset;
        FormatHelpers::StageConfig stageConfig;

        RSDKv5::TileConfig tileconfig;

        QList<SceneObject> objects;
        QList<SceneEntity> entities;

        // General Editing
        byte curTool            = SceneViewer::TOOL_MOUSE;
        bool selecting          = false;
        Vector2<float> mousePos = Vector2<float>(0.0f, 0.0f);

        // Layer Editing
        Vector2<float> tilePos = Vector2<float>(0.0f, 0.0f);
        Vector2<bool> tileFlip = Vector2<bool>(false, false);
        int selectedTile       = -1;
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
        Vector2<float> camPos = Vector2<float>(0.0f, 0.0f);

        bool showTileGrid = false;
    };

    explicit SceneEditor(QWidget *parent = nullptr);
    ~SceneEditor();

    bool mouseDownL = false;
    bool mouseDownM = false;
    bool mouseDownR = false;
    bool ctrlDownL  = false;
    bool altDownL   = false;
    bool shiftDownL = false;

    Vector2<float> selectionOffset = Vector2<float>(0.0f, 0.0f);

    SceneViewer *viewer            = nullptr;
    SceneProperties *scnProp       = nullptr;
    SceneLayerProperties *lyrProp  = nullptr;
    SceneTileProperties *tileProp  = nullptr;
    SceneObjectProperties *objProp = nullptr;
    SceneScrollProperties *scrProp = nullptr;
    ChunkSelector *chkProp         = nullptr;
    TilesetEditor *tsetEdit        = nullptr;

    SceneEntity createTempEntity;

    FormatHelpers::GameConfig gameConfig;

    FormatHelpers::Scene scene;
    FormatHelpers::Background background;
    FormatHelpers::Chunks chunkset;
    FormatHelpers::StageConfig stageConfig;

    void createNewScene();
    void loadScene(QString scnPath, QString gcfPath, byte gameType);
    bool saveScene(bool forceSaveAs = false);

    void initGameLink();

    bool callGameEvent(byte eventID, int id);
    ushort loadSpriteSheet(QString filename);
    void drawSpriteFlipped(float XPos, float YPos, float width, float height, float sprX, float sprY,
                           int direction, InkEffects inkEffect, int alpha, int sheetID,
                           bool screenRelative);
    void drawSpriteRotozoom(float XPos, float YPos, float pivotX, float pivotY, float width,
                            float height, float sprX, float sprY, int scaleX, int scaleY, int direction,
                            short rotation, InkEffects inkEffect, int alpha, int sheetID,
                            bool screenRelative);
    Compilerv2 *compilerv2 = nullptr;
    Compilerv3 *compilerv3 = nullptr;
    Compilerv4 *compilerv4 = nullptr;
    bool scriptError       = false;

    // Event Handlers
    void setTile(float x, float y);
    void resetTools(byte tool);

    bool handleKeyPress(QKeyEvent *event);
    bool handleKeyRelease(QKeyEvent *event);

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

    int addEntity(int type, float x, float y);
    void deleteEntity(int slot, bool updateUI = false);

    void filterObjectList(QString filter);
    void filterEntityList(QString filter);

    void createEntityList(int startSlot = -1);
    void createScrollList();

    // XML Management
    void parseGameXML(QString path);

    inline void writeXMLIndentation(Writer &writer, int tabCount)
    {
        for (int t = 0; t < tabCount; ++t) writer.write<char>('\t');
    }

    void readXMLScrollInfo(QXmlStreamReader &xmlReader, int layerID, byte mode = 0);
    void readXMLLayout(QXmlStreamReader &xmlReader, int layerID, byte mode = 0);
    void readXMLLayers(QXmlStreamReader &xmlReader);

    void writeXMLScrollInfo(Writer &writer, int layerID, int indentPos);
    void writeXMLLayout(Writer &writer, int layerID, int indentPos);
    void writeXMLLayer(Writer &writer, int layerID, int indentPos);

    void writeXMLObject(Writer &writer, int objID, int indentPos);
    void writeXMLEntity(Writer &writer, int entityID, int indentPos);

    void writeXMLChunk(Writer &writer, int chunkID, int indentPos);

    void writeXMLScene(Writer &writer);

    Ui::SceneEditor *ui;

    ChunkEditor *chunkEdit = nullptr;

    QList<ActionState> actions;
    int actionIndex = 0;

    bool modified    = false;
    QString tabTitle = "Scene Editor";
};

class ChunkLabel : public QLabel
{
    Q_OBJECT
public:
    ChunkLabel(ushort *sel, int index, QWidget *parent) : QLabel(parent), m_sel(sel), m_index(index) {}

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
    ushort *m_sel = nullptr;
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
    ChunkLabel *labels[0x200];
};
