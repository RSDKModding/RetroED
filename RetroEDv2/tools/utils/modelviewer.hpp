#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QWidget>

class ModelViewer : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit ModelViewer(QWidget *parent = nullptr);
    ~ModelViewer();

    RSDKv5::Model model;

    RSDKv5::Model::Frame* curFrame;
    RSDKv5::Model::Frame* nextFrame;
    float animTimer = 0;

    void setModel(RSDKv5::Model m, QString tex = "");
    void setModel(RSDKv4::Model m, QString tex = "");
    
    QString texFile = "";
    bool reload = false;

    QTimer* updateTimer = nullptr;
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    QSize sizeHint() const { return QSize(0, 0); }

    QMatrix4x4 matModel;
    QMatrix4x4 matWorld;
    QMatrix4x4 matView;

    void resetMatrices(int w, int h) {
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

    QString curTex = "";
    QOpenGLTexture* tex = nullptr; 

    Shader shader;

private:
    QOpenGLFunctions *glFuncs = nullptr;

    friend class ModelManager;
};

#endif // MODELVIEWER_H
