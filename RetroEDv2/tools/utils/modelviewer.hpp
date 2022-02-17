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

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    QSize sizeHint() const { return QSize(0, 0); }

private:
    QOpenGLFunctions *glFuncs = nullptr;
};

#endif // MODELVIEWER_H
