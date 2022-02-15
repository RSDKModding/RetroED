#ifndef SCENEVIEWER_V5_H
#define SCENEVIEWER_V5_H

class SceneCamerav5
{
public:
    float m_near = 0.01f;
    float m_far  = 1000.0f;

    Vector2<float> pos = Vector2<float>(0.0f, 0.0f);

    Vector2<float> lastMousePos = Vector2<float>(0.0f, 0.0f);

    float m_aspectRatio = 0.0f;
};

class SceneViewerv5 : public QOpenGLWidget
{
public:
    enum SceneManagerTool {
        TOOL_MOUSE,
        TOOL_SELECT,
        TOOL_PENCIL,
        TOOL_STAMP,
        TOOL_ERASER,
        TOOL_ENTITY,
        TOOL_PARALLAX,
    };

    enum EventTypes { EVENT_LOAD, EVENT_CREATE, EVENT_UPDATE, EVENT_DRAW, EVENT_SERIALIZE };

    struct DrawVertex {
        DrawVertex() {}

        QVector2D pos;
        QVector2D uv;
        QVector4D colour;
    };

    struct RenderState {
        RenderState() {}

        Shader *shader    = nullptr;
        Shader *fbShader  = nullptr;
        Shader *fbShader2 = nullptr;

        byte blendMode = INK_NONE;
        byte alpha     = 0xFF;

        ushort indexCount = 0;
        ushort indecies[0x8000 * 6];

        byte argBuffer[0x20];
        byte fsArgs[0x20];

        ScreenInfo *screen;
        Vector2<int> clipRectTL;
        Vector2<int> clipRectBR;
    };

    SceneViewerv5(QWidget *parent);
    ~SceneViewerv5();

    void startTimer();

    void loadScene(QString path);
    void saveScene(QString path);

    void updateScene();
    void drawScene();
    void unloadScene();

    void processObjects();

    void dispose();

    // viewing properties
    float zoom = 1.0f;
    inline float invZoom() { return 1.0f / zoom; }
    bool disableObjects = true;

    QString dataPath = "";
    RSDKv5::GameConfig gameConfig;

    RSDKv5::Scene scene;
    RSDKv5::StageConfig stageConfig;
    RSDKv5::Stamps stamps;

    RSDKv5::TileConfig tileconfig;

    QList<SceneObject> objects;
    QList<SceneEntity> entities;

    QList<GameLink> gameLinks;

    // Stuff for game link
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

    QString currentFolder = "Blank";

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
    int selectedTile         = -1;
    int selectedLayer        = -1;

    int selectedStamp = -1;

    // Collision
    bool showPlaneA = false;
    bool showPlaneB = false;

    // Entity Editing
    int selectedObject = -1; // placing
    int selectedEntity = -1; // viewing

    GameEntityBase createGameEntity;
    SceneEntity createTempEntity;

    SceneEntity *activeDrawEntity = nullptr;

    // For Variable Aliases
    int activeVar    = -1;
    int activeVarObj = -1;

    int sceneFilter = 0xFF;

    // Parallax Editing
    bool showParallax      = false;
    int selectedScrollInfo = -1;

    // Camera
    SceneCamerav5 cam;

    QList<PaletteColour> tilePalette;
    QList<QImage> tiles;
    QImage missingObj;

    bool showGrid         = false;
    Vector2<int> gridSize = Vector2<int>(0x10, 0x10);

    // passed from main
    QLabel *statusLabel              = nullptr;
    QScrollBar *sbHorizontal         = nullptr;
    QScrollBar *sbVertical           = nullptr;
    SceneObjectPropertiesv5 *objProp = nullptr;

    Colour bgColour    = Colour(0x20, 0x20, 0x20);
    Colour altBGColour = Colour(0x30, 0x30, 0x30);

    int prevStoredW = -1, prevStoredH = -1;
    int storedW, storedH;
    int sceneWidth, sceneHeight;

    QImage *colTex = nullptr;

    DrawVertex vertexList[0x8000];
    ushort baseIndexList[0x8000 * 6];

    QList<RenderState> renderStates;

    sbyte renderStateIndex = -1;
    ushort renderCount     = 0;
    ushort lastRenderCount = 0;

    int prevSprite = -1;

    // Game Logic
    GameEntityBase gameEntityList[v5_ENTITY_COUNT * 2];

    ForeachStackInfo foreachStackList[FOREACH_STACK_COUNT];
    ForeachStackInfo *foreachStackPtr;

    TypeGroupList typeGroups[TYPEGROUP_COUNT];

    DrawList drawLayers[v5_DRAWLAYER_COUNT];

    void callGameEvent(GameObjectInfo *info, byte eventID, SceneEntity *entity);

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

    QOpenGLTexture *tilesetTexture = nullptr;

    QOpenGLFunctions *glFuncs = nullptr;

    QTimer *updateTimer = nullptr;

    void drawTile(float XPos, float YPos, int tileX, int tileY, byte direction);

    void drawSpriteFlipped(float XPos, float YPos, float width, float height, float sprX, float sprY,
                           int direction, InkEffects inkEffect, int alpha, int sheetID);
    void drawSpriteRotozoom(float x, float y, float pivotX, float pivotY, float width, float height,
                            float sprX, float sprY, int scaleX, int scaleY, int direction,
                            short rotation, InkEffects inkEffect, int alpha, int sheetID);

    void drawLine(float x1, float y1, float x2, float y2, Vector4<float> colour, int alpha = 0xFF,
                  InkEffects inkEffect = INK_NONE);

    void drawRect(float x, float y, float w, float h, Vector4<float> colour, bool outline = false,
                  int alpha = 0xFF, InkEffects inkEffect = INK_NONE);

    void drawCircle(float x, float y, float innerR, float outerR, Vector4<float> colour,
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

    void refreshResize();

    GameObjectInfo *GetObjectInfo(QString name);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    QSize sizeHint() const { return QSize(0, 0); }

private:
    QMatrix4x4 m_matView;

    inline QMatrix4x4 getProjectionMatrix()
    {
        QMatrix4x4 matWorld;
        cam.m_aspectRatio = storedW / (float)storedH;
        matWorld.ortho(0.0f, (float)storedW, (float)storedH, 0.0f, -1.0f, 1.0f);
        return matWorld;
    }

    void placeCol(int x, int y, sbyte h, int sol, int w = 1);

    inline void cleanCol(int x, int y, int w, int h);
};

#endif // SCENEVIEWER_V5_H
