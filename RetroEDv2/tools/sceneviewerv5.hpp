#ifndef SCENEVIEWER_V5_H
#define SCENEVIEWER_V5_H

class SceneCamerav5
{
public:
    float m_near = 0.01f;
    float m_far  = 1000.0f;

    Vector2<float> pos = Vector2<float>(0.0f, 0.0f);

    Vector2<float> m_lastMousePos = Vector2<float>(0.0f, 0.0f);

    float m_aspectRatio = 0.0f;
};

class SceneViewerv5 : public QOpenGLWidget
{
public:
    enum EventTypes { EVENT_LOAD, EVENT_CREATE, EVENT_UPDATE, EVENT_DRAW, EVENT_SERIALIZE };

    SceneViewerv5(QWidget *parent);
    ~SceneViewerv5();

    void loadScene(QString path);
    void saveScene(QString path);

    void updateScene();
    void drawScene();
    void unloadScene();

    // viewing properties
    float zoom = 1.0f;
    inline float invZoom() { return 1.0f / zoom; }

    QString dataPath = "";
    RSDKv5::GameConfig gameConfig;

    RSDKv5::Scene scene;
    RSDKv5::StageConfig stageConfig;

    RSDKv5::TileConfig tileconfig;

    QList<SceneObject> objects;
    QList<SceneEntity> entities;

    QPoint m_reference;

    // General Editing
    byte curTool            = TOOL_MOUSE;
    bool isSelecting        = false;
    Vector2<float> mousePos = Vector2<float>(0.0f, 0.0f);

    // Layer Editing
    Vector2<float> tilePos   = Vector2<float>(0.0f, 0.0f);
    Vector2<bool> tileFlip   = Vector2<bool>(false, false);
    Vector2<bool> tileSolidA = Vector2<bool>(false, false);
    Vector2<bool> tileSolidB = Vector2<bool>(false, false);
    int selectedTile         = -1;
    int selectedLayer        = -1;

    // Collision
    bool showPlaneA = false;
    bool showPlaneB = false;

    // Entity Editing
    int selectedObject = -1; // placing
    int selectedEntity = -1; // viewing

    int sceneFilter = 0xFF;

    // Parallax Editing
    bool showParallax      = false;
    int selectedScrollInfo = -1;

    // Camera
    SceneCamerav5 cam;

    QList<QImage> tiles;
    QImage missingObj;

    bool showPixelGrid = false;
    bool showTileGrid  = false;

    // passed from main
    QLabel *statusLabel        = nullptr;
    QScrollBar *m_sbHorizontal = nullptr;
    QScrollBar *m_sbVertical   = nullptr;

    Colour bgColour    = Colour(0x20, 0x20, 0x20);
    Colour altBGColour = Colour(0x30, 0x30, 0x30);

    int storedW, storedH;
    int sceneWidth, sceneHeight;

    QVector3D *vertsPtr  = nullptr;
    QVector2D *tVertsPtr = nullptr;

    QImage *colTex = nullptr;

    // TEMP until SetEditableVar() gets completed
    QList<QString> variableNames;

    inline float incZ()
    {
        float c = currZ;
        currZ += 0.01;
        return c;
    }
    float currZ = 16;

    int prevSprite = -1;

    // Game Logic
    GameEntityBase gameEntityList[v5_ENTITY_COUNT];

    ForeachStackInfo foreachStackList[FOREACH_STACK_COUNT];
    ForeachStackInfo *foreachStackPtr;

    TypeGroupList typeGroups[TYPEGROUP_COUNT];

    void callGameEvent(GameObjectInfo *info, byte eventID, int id);

    SpriteAnimation spriteAnimationList[v5_SPRFILE_COUNT];
    GFXSurface gfxSurface[v5_SURFACE_MAX];

    int addGraphicsFile(char *sheetPath, int sheetID, byte scope);
    void removeGraphicsFile(char *sheetPath, int slot);
    inline void getTileVerts(QVector2D *arr, int index, int tileIndex, byte direction)
    {
        float w = m_tilesetTexture->width(), h = m_tilesetTexture->height();

        float tx = 0.0f;
        float ty = tileIndex / h;
        float tw = 0x10 / w;
        float th = 0x10 / h;

        switch (direction) {
            case 0:
            default: {
                arr[index + 0].setX(tx);
                arr[index + 0].setY(ty);

                arr[index + 1].setX(tx + tw);
                arr[index + 1].setY(ty);

                arr[index + 2].setX(tx + tw);
                arr[index + 2].setY(ty + th);

                arr[index + 3].setX(tx + tw);
                arr[index + 3].setY(ty + th);

                arr[index + 4].setX(tx);
                arr[index + 4].setY(ty + th);

                arr[index + 5].setX(tx);
                arr[index + 5].setY(ty);
                break;
            }
            case 1: {
                arr[index + 0].setX(tx + tw);
                arr[index + 0].setY(ty);

                arr[index + 1].setX(tx);
                arr[index + 1].setY(ty);

                arr[index + 2].setX(tx);
                arr[index + 2].setY(ty + th);

                arr[index + 3].setX(tx);
                arr[index + 3].setY(ty + th);

                arr[index + 4].setX(tx + tw);
                arr[index + 4].setY(ty + th);

                arr[index + 5].setX(tx + tw);
                arr[index + 5].setY(ty);
                break;
            }
            case 2: {
                arr[index + 0].setX(tx);
                arr[index + 0].setY(ty + th);

                arr[index + 1].setX(tx + tw);
                arr[index + 1].setY(ty + th);

                arr[index + 2].setX(tx + tw);
                arr[index + 2].setY(ty);

                arr[index + 3].setX(tx + tw);
                arr[index + 3].setY(ty);

                arr[index + 4].setX(tx);
                arr[index + 4].setY(ty);

                arr[index + 5].setX(tx);
                arr[index + 5].setY(ty + th);
                break;
            }
            case 3: {
                arr[index + 0].setX(tx + tw);
                arr[index + 0].setY(ty + th);

                arr[index + 1].setX(tx);
                arr[index + 1].setY(ty + th);

                arr[index + 2].setX(tx);
                arr[index + 2].setY(ty);

                arr[index + 3].setX(tx);
                arr[index + 3].setY(ty);

                arr[index + 4].setX(tx + tw);
                arr[index + 4].setY(ty);

                arr[index + 5].setX(tx + tw);
                arr[index + 5].setY(ty + th);
                break;
            }
        }
    }

    Shader primitiveShader;
    Shader spriteShader;

    void drawTile(float XPos, float YPos, float ZPos, int tileX, int tileY, byte direction);

    void drawSpriteFlipped(int XPos, int YPos, int width, int height, int sprX, int sprY, int direction,
                           int inkEffect, int alpha, int sheetID);
    void drawSpriteRotozoom(int x, int y, int pivotX, int pivotY, int width, int height, int sprX,
                            int sprY, int scaleX, int scaleY, int direction, short rotation,
                            int inkEffect, int alpha, int sheetID);

    void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, Vector4<float> colour,
                  Shader &shader);

    void drawRect(float x, float y, float z, float w, float h, Vector4<float> colour, Shader &shader,
                  bool outline = false);

    void drawCircle(float x, float y, float z, float r, Vector4<float> colour, Shader &shader,
                    bool outline = false);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    QSize sizeHint() const { return QSize(0, 0); }

private:
    QOpenGLVertexArrayObject screenVAO, rectVAO;
    QOpenGLTexture *m_tilesetTexture = nullptr;

    QMatrix4x4 m_matView;

    inline QOpenGLTexture *createTexture(QImage src)
    {
        QOpenGLTexture *tex = new QOpenGLTexture(QOpenGLTexture::Target::Target2D);
        tex->create();
        tex->bind();
        tex->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::WrapMode::Repeat);
        tex->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::WrapMode::Repeat);
        tex->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        tex->setFormat(QOpenGLTexture::RGBA8_UNorm);
        tex->setSize(src.width(), src.height());
        tex->setData(src, QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
        return tex;
    }

    // void processParallax(Scene::SceneLayer &layer);

    inline QMatrix4x4 getProjectionMatrix()
    {
        QMatrix4x4 matWorld;
        cam.m_aspectRatio = storedW / (float)storedH;
        matWorld.ortho(0.0f, (float)storedW, (float)storedH, 0.0f, -16.0f, 16.0f);
        return matWorld;
    }

    void placeCol(int x, int y, sbyte h, int sol, int w = 1);

    inline void cleanCol(int x, int y, int w, int h);
};

#endif // SCENEVIEWER_V5_H
