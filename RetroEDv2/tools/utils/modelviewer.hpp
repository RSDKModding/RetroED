#ifndef MODELVIEWER_H
#define MODELVIEWER_H

class ModelViewer : public QOpenGLWidget
{
    Q_OBJECT

public:
    ModelViewer(QWidget *parent = nullptr);
    ~ModelViewer();

    RSDKv5::Model model;

    RSDKv5::Model::Frame *curFrame;
    RSDKv5::Model::Frame *nextFrame;
    int loopIndex;
    float animSpeed = 0.1f;
    float animTimer = 0;

    float zoom = 1.0f;

    void setModel(RSDKv5::Model m, QString tex = "");
    void setModel(RSDKv4::Model m, QString tex = "");

    void loadTexture(QString texturePath);

    void setFrame(int frameID);
    void setWireframe(bool wireframe);
    void setZoom(float zoom);

    QString texFile = "";
    bool reload     = false;

    QTimer *renderTimer;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    QSize sizeHint() const { return QSize(0, 0); }

private:
    QMatrix4x4 matModel;
    QMatrix4x4 matWorld;
    QMatrix4x4 matView;

    void resetMatrices(int w, int h)
    {
        matModel.setToIdentity();
        matWorld.setToIdentity();
        matWorld.perspective(30, w / (float)h, -16, 16);
        matView.setToIdentity();
        matView.translate(1, 1, 1);
        matView.rotate(45, 0, 0, 1);
    }

    QOpenGLFramebufferObject *outFB = nullptr;
    QOpenGLBuffer *vertVBO, *colorVBO, *normalVBO, *texVBO, *indexVBO;
    QOpenGLVertexArrayObject *VAO = nullptr;

    QString curTex      = "";
    QOpenGLTexture *tex = nullptr;

    Shader shader;

    QOpenGLFunctions *glFuncs = nullptr;

    // friend class ModelManager;
};

#endif // MODELVIEWER_H
