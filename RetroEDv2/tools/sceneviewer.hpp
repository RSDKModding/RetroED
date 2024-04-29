#pragma once

#include <RSDKv5.hpp>
#include <RSDKv1.hpp>

#include "sceneproperties/sceneincludesv5.hpp"

#define AS_ENTITY(var, type) ((type *)var)

class SceneObjectPropertiesv5;

class SceneViewer : public QOpenGLWidget
{
    Q_OBJECT

public:
    enum SceneManagerTool {
        TOOL_MOUSE,
        TOOL_SELECT,
        TOOL_PENCIL,
        TOOL_STAMP,
        TOOL_ERASER,
        TOOL_ENTITY,
        TOOL_STAMP_MAKER,
    };

    enum EventTypes {
        EVENT_STATICLOAD,
        EVENT_LOAD,
        EVENT_CREATE,
        EVENT_UPDATE,
        EVENT_DRAW,
        EVENT_SERIALIZE,
        EVENT_EDIT
    };

    struct DrawVertex {
        DrawVertex() {}

        QVector2D pos;
        QVector2D uv;
        QVector4D color;
    };

    struct RenderState {
        RenderState() {}

        Shader *shader    = nullptr;
        Shader *fbShader  = nullptr;
        Shader *fbShader2 = nullptr;

        byte blendMode = INK_NONE;
        byte alpha     = 0xFF;

        ushort indexCount = 0;
        ushort indices[0x8000 * 6];

        byte argBuffer[0x20];
        byte fsArgs[0x20];

        ScreenInfo *screen;
        Vector2<int> clipRectTL;
        Vector2<int> clipRectBR;
    };

    SceneViewer(byte gameType, QWidget *parent);
    ~SceneViewer();

    void initScene(QImage tileset);

    void updateScene();
    void drawScene();
    void unloadScene();

    void processObjects(bool isImage);

    void dispose();


    // viewing properties
    float zoom = 1.0f;
    inline float invZoom() { return 1.0f / zoom; }
    bool disableObjects   = true;
    bool disableDrawScene = false;

    double fps;
    QElapsedTimer fpsTimer;

    byte gameType = ENGINE_NONE;

    QString dataPath = "";

    RSDKv5::Scene::SceneEditorMetadata metadata;
    RSDKv5::Stamps stamps;
    RSDKv5::TileConfig tileconfig;
    RSDKv1::TileConfig tileconfigv1;


    byte tileSize = 0x10;
    QList<SceneHelpers::TileLayer> layers;
    FormatHelpers::Chunks chunkset;

    bool useLayerScrollInfo = true;
    QList<SceneHelpers::TileLayer::ScrollIndexInfo> hScroll;
    QList<SceneHelpers::TileLayer::ScrollIndexInfo> vScroll;

    QList<SceneObject> objects;
    QList<SceneEntity> entities;

    // Stuff for game link
    int engineRevision = 3;

    SceneInfo sceneInfo;
    EngineInfo gameInfo;
    SKUInfo skuInfo;
    ControllerState controller[5];
    AnalogState stickL[5];
    AnalogState stickR[5];
    TriggerState triggerL[5];
    TriggerState triggerR[5];
    TouchMouseData touchMouse;
    UnknownInfo unknownInfo;
    ScreenInfo screens[4];

    SceneInfoV1 sceneInfoV1;
    EngineInfoV1 gameInfoV1;
    ControllerStateV1 controllerV1[5];
    AnalogStateV1 stickLV1[5];
    TouchMouseDataV1 touchMouseV1;

    inline int activeEntityCount()
    {
        int count = 0;
        for (int i = 0; i < entities.count(); ++i) {
            SceneEntity &entity = entities[i];
            int filter          = 0;
            for (int v = 0; v < objects[entity.type].variables.count(); ++v) {
                if (objects[entity.type].variables[v].name == "filter") {
                    if (v < entity.variables.count())
                        filter = entity.variables[v].value_uint8;
                    break;
                }
            }

            if ((filter & sceneFilter) || !filter)
                ++count;
        }
        return count;
    }

    QString currentSceneID = "1";
    QString currentFolder  = "Blank";

    QPoint reference;

    // General Editing
    byte curTool            = TOOL_MOUSE;
    bool isSelecting        = false;
    Vector2<float> mousePos = Vector2<float>(0.0f, 0.0f);

    Vector2<float> selectPos  = Vector2<float>(0.0f, 0.0f);
    Vector2<float> selectSize = Vector2<float>(0.0f, 0.0f);
    QList<int> selectedEntities;

    // Layer Editing
    Vector2<float> tilePos   = Vector2<float>(0.0f, 0.0f);
    Vector2<bool> tileFlip   = Vector2<bool>(false, false);
    Vector2<bool> tileSolidA = Vector2<bool>(false, false);
    Vector2<bool> tileSolidB = Vector2<bool>(false, false);
    ushort selectedTile      = 0xFFFF;
    ushort selectedChunk     = 0xFFFF;
    ushort selectedStamp     = 0xFFFF;

    int selectedLayer        = -1;

    // Collision
    bool showPlaneA = false;
    bool showPlaneB = false;

    bool updateCTex[2] = {false, false};

    // Entity Editing
    int selectedObject = -1; // placing
    int selectedEntity = -1; // viewing

    // Retro-Sonic only
    Vector2<int> playerPos = Vector2<int>(0, 0);

    SceneEntity *activeDrawEntity = nullptr;

    int sceneFilter = 0xFF;

    // Parallax Editing
    bool showParallax       = false;
    int selectedHScrollInfo = -1;
    int selectedVScrollInfo = -1;

    // Camera
    Vector2<float> cameraPos = Vector2<float>(0.0f, 0.0f);

    QList<PaletteColor> tilePalette;
    QList<QImage> tiles;
    QList<QImage> chunks;
    QImage missingObj;

    bool showGrid         = false;
    Vector2<int> gridSize = Vector2<int>(0x10, 0x10);

    // passed from main
    QLabel *statusLabel              = nullptr;
    SceneObjectPropertiesv5 *objProp = nullptr;

    int prevStoredW = -1, prevStoredH = -1;
    int storedW, storedH;
    int sceneBoundsL, sceneBoundsT, sceneBoundsR, sceneBoundsB;

    QImage *colTex[2]   = {nullptr, nullptr};
    QImage *colTexStore = nullptr;
    QPainter colPaint;

    static const int vertexListLimit = 0x8000;

    DrawVertex vertexList[vertexListLimit];
    ushort baseIndexList[vertexListLimit * 6];

    static const int renderStatesLimit = 0x200;

    RenderState renderStates[renderStatesLimit];

    short renderStateCount = 0;
    ushort renderCount     = 0;
    ushort lastRenderCount = 0;

    int prevSprite = -1;

    // Game Logic
    GameEntityBasev1 *gameEntityListv1;
    GameEntityBasev2 *gameEntityListv2;
    GameEntityBasevU *gameEntityListvU;

    void *gameEntityList;

    ForeachStackInfo foreachStackList[FOREACH_STACK_COUNT];
    ForeachStackInfo *foreachStackPtr;

    TypeGroupList typeGroups[TYPEGROUP_COUNT_v5];

    DrawList drawLayers[v5_DRAWGROUP_COUNT];

    SpriteAnimation spriteAnimationList[v5_SPRFILE_COUNT];
    GFXSurface gfxSurface[v5_SURFACE_MAX];

    int addGraphicsFile(QString sheetPath, int sheetID, byte scope);
    void removeGraphicsFile(QString sheetPath, int slot);

    ushort tileUVArray[0x400 * 4 * 4];

    inline QOpenGLTexture *
    createTexture(QImage src, QOpenGLTexture::Target target = QOpenGLTexture::Target::Target2D)
    {
        QOpenGLTexture *tex = new QOpenGLTexture(target);
        tex->create();
        tex->bind();
        tex->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::WrapMode::Repeat);
        tex->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::WrapMode::Repeat);
        tex->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        tex->setFormat(QOpenGLTexture::RGBA8_UNorm);
        tex->setSize(src.width(), src.height());
        tex->setData(src, target == QOpenGLTexture::Target2D
                              ? QOpenGLTexture::MipMapGeneration::GenerateMipMaps
                              : QOpenGLTexture::MipMapGeneration::DontGenerateMipMaps);
        return tex;
    }

    Shader placeShader  = Shader("Place");
    Shader circleShader = Shader("Circle");
    Shader tileShader   = Shader("Tile");
    // the... Impostor... you may say...
    Shader lineShader = Shader("Place");

    Shader passthroughFBShader = Shader("PassthroughFB");
    Shader tileFBShader        = Shader("TileFB");
    Shader finalFBShader       = Shader("FinalFB");

    QOpenGLVertexArrayObject *VAO = nullptr, *fbpVAO = nullptr;
    QOpenGLBuffer *attribVBO = nullptr, *indexVBO = nullptr, *fbpVBO = nullptr, *fbiVBO = nullptr;
    QOpenGLFramebufferObject *outFB = nullptr, *tFB = nullptr, *t2FB = nullptr;
    QOpenGLTexture *outFBT = nullptr, *tFBT = nullptr, *t2FBT = nullptr;

    QOpenGLFunctions *glFuncs = nullptr;

    QTimer *updateTimer = nullptr;

    void startTimer();
    void stopTimer();

    void drawSpriteFlipped(float XPos, float YPos, float width, float height, float sprX, float sprY,
                           int direction, InkEffects inkEffect, int alpha, int sheetID);
    void drawSpriteRotozoom(float x, float y, float pivotX, float pivotY, float width, float height,
                            float sprX, float sprY, int scaleX, int scaleY, int direction,
                            short rotation, InkEffects inkEffect, int alpha, int sheetID);

    void drawLine(float x1, float y1, float x2, float y2, Vector4<float> color, int alpha = 0xFF,
                  InkEffects inkEffect = INK_NONE);

    void drawRect(float x, float y, float w, float h, Vector4<float> color, bool outline = false,
                  int alpha = 0xFF, InkEffects inkEffect = INK_NONE);

    void drawFace(Vector2<int> *vertices, int vertCount, int r, int g, int b, int alpha = 0xFF,
                  InkEffects inkEffect = INK_NONE);
    void drawBlendedFace(Vector2<int> *vertices, uint *colors, int vertCount, int alpha = 0xFF,
                         InkEffects inkEffect = INK_NONE);

    inline void addPoly(float x, float y, float u, float v, uint color = 0, GFXSurface *surface = NULL)
    {
        Vector4<float> rcolor = { ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
                                  (color & 0xFF) / 255.0f, ((color >> 24) & 0xFF) / 255.0f };

        addPoly(x, y, u, v, rcolor, surface);
    };

    void addPoly(float x, float y, float u, float v, Vector4<float> color, GFXSurface *surface = NULL);

    void addRenderState(int blendMode, ushort vertCount, ushort indexCount, void *args = nullptr,
                        byte alpha = 0xFF, Shader *shader = nullptr, ushort *altIndex = nullptr,
                        Shader *fbShader = nullptr, Shader *fbShader2 = nullptr,
                        Vector2<int> *clipRect = nullptr);
    void renderRenderStates();

    inline void queueRender(QString filename)
    {
        fileRender     = 1;
        renderFilename = filename;
    }
    byte fileRender = 0;
    QString renderFilename;

    void refreshResize();

    // For Variable Aliases & Stuff
    bool objectsLoaded  = false;
    int activeVar       = -1;
    int activeVarObj    = -1;
    int variableID      = -1;
    int variableValue   = -1;
    bool returnVariable = false;

    void addEditorVariable(QString name);
    void setActiveVariable(QString name);
    void addEnumVariable(QString name, int value);
    void setVariableAlias(int varID, QString alias);


signals:
    void callGameEvent(byte eventID, int entityID);
    void callGameEventv5(QString objName, byte eventID, SceneEntity *entity);

public slots:
    void updateTileColMap(RSDKv5::TileConfig::CollisionMask *cmask, ushort sel, int colLyr);
    void updateChunkColTile(RSDKv5::TileConfig::CollisionMask *cmask, ushort sel, int colLyr);
    void updateChunkColTilev1(RSDKv1::TileConfig::CollisionMask *cmask, ushort sel, int colLyr);
    void updateChunkColMap();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    QSize sizeHint() const { return QSize(0, 0); }

private:
    QMatrix4x4 matWorld;
    QMatrix4x4 matView;

    friend class SceneEditorv5;
};


