#ifndef SCENEVIEWER_H
#define SCENEVIEWER_H

class SceneCamera
{
public:
    float m_near = 0.01f;
    float m_far  = 1000.0f;

    Vector3<float> m_position = Vector3<float>(0.0f, 0.0f, 0.0f);

    Vector2<float> m_lastMousePos = Vector2<float>(0.0f, 0.0f);

    float m_aspectRatio = 0.0f;
};

enum SceneManagerTool {
    TOOL_MOUSE,
    TOOL_SELECT,
    TOOL_PENCIL,
    TOOL_ERASER,
    TOOL_ENTITY,
    TOOL_PARALLAX,
};

class SceneViewer : public QOpenGLWidget
{
public:
    SceneViewer(QWidget *parent);
    ~SceneViewer();

    struct TextureInfo {
        TextureInfo() {}

        QString m_name            = "";
        QOpenGLTexture *m_texture = nullptr;
    };

    void loadScene(QString path, byte ver);

    void updateScene();
    void drawScene();
    void unloadScene();

    // viewing properties
    float m_zoom = 1.0f;
    inline float invZoom() { return 1.0f / m_zoom; }

    byte m_gameType = ENGINE_NONE;

    QString m_dataPath = "";
    FormatHelpers::Gameconfig m_gameconfig;

    FormatHelpers::Scene m_scene;
    FormatHelpers::Background m_background;
    FormatHelpers::Chunks m_chunkset;
    FormatHelpers::Stageconfig m_stageconfig;

    RSDKv4::Tileconfig m_tileconfig;
    RSDKv1::Tileconfig m_tileconfigRS;

    QPoint m_reference;

    // General Editing
    byte m_tool               = TOOL_MOUSE;
    bool m_selecting          = false;
    Vector2<float> m_mousePos = Vector2<float>(0.0f, 0.0f);

    // Layer Editing
    Vector2<float> m_tilePos = Vector2<float>(0.0f, 0.0f);
    Vector2<bool> m_tileFlip = Vector2<bool>(false, false);
    int m_selectedChunk      = -1;
    int m_selectedLayer      = -1;

    // Collision
    bool m_showPlaneA = false;
    bool m_showPlaneB = false;

    // Entity Editing
    int m_selectedObject = -1; // placing
    int m_selectedEntity = -1; // viewing

    // Parallax Editing
    bool m_showParallax      = true;
    int m_selectedScrollInfo = -1;

    // Camera
    SceneCamera m_cam;

    QList<QImage> m_tiles;
    QList<QImage> m_chunks;
    QImage m_missingObj;

    bool m_showPixelGrid = false;
    bool m_showTileGrid  = false;

    Compilerv3 m_compilerv3;
    Compilerv4 m_compilerv4;

    // passed from main
    QLabel *m_statusLabel      = nullptr;
    QScrollBar *m_sbHorizontal = nullptr;
    QScrollBar *m_sbVertical   = nullptr;

    Colour m_bgColour    = Colour(0x20, 0x20, 0x20);
    Colour m_altBGColour = Colour(0x30, 0x30, 0x30);

    inline int layerWidth(byte layer)
    {
        int weight = m_scene.m_width;

        if (layer > 0) {
            weight = m_background.m_layers[layer - 1].m_width;
        }
        return weight;
    }

    inline int layerHeight(byte layer)
    {
        int height = m_scene.m_height;

        if (layer > 0) {
            height = m_background.m_layers[layer - 1].m_height;
        }
        return height;
    }

    inline QList<QList<ushort>> layerLayout(byte layer)
    {
        QList<QList<ushort>> layout = m_scene.m_layout;

        if (layer > 0) {
            layout = m_background.m_layers[layer - 1].m_layout;
        }
        return layout;
    }

    int m_storedW, m_storedH;

    int m_prevSprite = -1;

    int addGraphicsFile(char *sheetPath);
    void removeGraphicsFile(char *sheetPath, int slot);

    void drawSprite(int XPos, int YPos, int width, int height, int sprX, int sprY, int sheetID);
    void drawSpriteFlipped(int XPos, int YPos, int width, int height, int sprX, int sprY, int direction,
                           int sheetID);

    void drawBlendedSprite(int XPos, int YPos, int width, int height, int sprX, int sprY, int sheetID);
    void drawAlphaBlendedSprite(int XPos, int YPos, int width, int height, int sprX, int sprY,
                                int alpha, int sheetID);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    QSize sizeHint() const { return QSize(0, 0); }

private:
    QOpenGLVertexArrayObject m_screenVAO, m_rectVAO;
    QList<QOpenGLTexture *> m_chunkTextures;
    QList<TextureInfo> m_objectSprites;

    QOpenGLTexture *m_rsPlayerSprite = nullptr;

    QMatrix4x4 m_matView, m_skyboxMatrix;

    Shader m_primitiveShader;
    Shader m_spriteShader;

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
        m_cam.m_aspectRatio = m_storedW / (float)m_storedH;
        matWorld.ortho(0.0f, (float)m_storedW, (float)m_storedH, 0.0f, -16.0f, 16.0f);
        return matWorld;
    }

    inline void drawPoint(float x, float y, float z, Vector4<float> colour, Shader &shader)
    {
        Q_UNUSED(x)
        Q_UNUSED(y)
        Q_UNUSED(z)
        Q_UNUSED(colour)
        Q_UNUSED(shader)
        // QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    }

    inline void drawLine(float x1, float y1, float z1, float x2, float y2, float z2,
                         Vector4<float> colour, Shader &shader)
    {
        shader.use();
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

        shader.setValue("colour", QVector4D(colour.x, colour.y, colour.z, colour.w));

        QVector3D vertsPtr[2] = { QVector3D(x1, y1, z1), QVector3D(x2, y2, z2) };

        QOpenGLVertexArrayObject VAO;
        VAO.create();
        VAO.bind();

        QOpenGLBuffer VBO;
        VBO.create();
        VBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        VBO.bind();
        VBO.allocate(vertsPtr, 2 * sizeof(QVector3D));
        shader.enableAttributeArray(0);
        shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

        f->glDrawArrays(GL_LINES, 0, 2);

        VAO.release();
        VBO.release();

        VAO.destroy();
        VBO.destroy();
    }

    inline void drawRect(float x, float y, float z, float w, float h, Vector4<float> colour,
                         Shader &shader, bool outline = false)
    {
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

        shader.use();
        if (outline) {
            // top
            drawLine(x, y, z, x + w, y, z, colour, shader);
            // bottom
            drawLine(x, y + h, z, x + w, y + h, z, colour, shader);
            // left
            drawLine(x, y, z, x, y + h, z, colour, shader);
            // right
            drawLine(x + w, y, z, x + w, y + h, z, colour, shader);
        }
        else {
            shader.setValue("colour", QVector4D(colour.x, colour.y, colour.z, colour.w));

            m_rectVAO.bind();

            QMatrix4x4 matModel;
            matModel.scale(w, h, 1.0f);
            matModel.translate(x / w, y / h, z);
            shader.setValue("model", matModel);

            f->glDrawArrays(GL_TRIANGLES, 0, 6);

            m_rectVAO.release();
        }
    }

    inline void drawCircle(float x, float y, float z, float r, Vector4<float> colour, Shader &shader,
                           bool outline = false)
    {
        Q_UNUSED(x)
        Q_UNUSED(y)
        Q_UNUSED(z)
        Q_UNUSED(r)
        Q_UNUSED(colour)
        Q_UNUSED(shader)

        // QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

        if (outline) {
        }
        else {
        }
    }
};

#endif // SCENEVIEWER_H
