#include "includes.hpp"

// Model will use v5 container, even if its a v4 one
// Texture UVs exist in v4 but for now they're not the main focus so dont worry about that
// They exist in v5 but they're literally unused in-engine so worry even less about that :]

ModelViewer::ModelViewer(QWidget *parent) : QOpenGLWidget(parent) {}

ModelViewer::~ModelViewer() {}

void ModelViewer::initializeGL()
{
    // Set up the rendering context, load shaders and other resources, etc.
    glFuncs = context()->functions();

    QSurfaceFormat fmt = context()->format();

    const unsigned char *vendor     = glFuncs->glGetString(GL_VENDOR);
    const unsigned char *renderer   = glFuncs->glGetString(GL_RENDERER);
    const unsigned char *version    = glFuncs->glGetString(GL_VERSION);
    const unsigned char *sdrVersion = glFuncs->glGetString(GL_SHADING_LANGUAGE_VERSION);
    const unsigned char *extensions = glFuncs->glGetString(GL_EXTENSIONS);

    QString vendorStr     = reinterpret_cast<const char *>(vendor);
    QString rendererStr   = reinterpret_cast<const char *>(renderer);
    QString versionStr    = reinterpret_cast<const char *>(version);
    QString sdrVersionStr = reinterpret_cast<const char *>(sdrVersion);
    QString extensionsStr = reinterpret_cast<const char *>(extensions);

    printLog("ModelViewer OpenGL Details");
    printLog("GL Version:   " + QString::number(fmt.majorVersion()) + "."
             + QString::number(fmt.minorVersion()));
    printLog("Vendor:       " + vendorStr);
    printLog("Renderer:     " + rendererStr);
    printLog("Version:      " + versionStr);
    printLog("GLSL version: " + sdrVersionStr);
    printLog("Extensions:   " + (extensionsStr == "" ? "None" : extensionsStr));
    printLog("GLType:       "
             + QString(QOpenGLContext::currentContext()->isOpenGLES() ? "OpenGLES" : "OpenGL"));
    printLog("Valid:        " + QString(QOpenGLContext::currentContext()->isValid() ? "Yes" : "No"));

    glFuncs->glDisable(GL_DEPTH_TEST);
    glFuncs->glDisable(GL_DITHER);

    glFuncs->glEnable(GL_BLEND);
    glFuncs->glDisable(GL_SCISSOR_TEST);
    glFuncs->glDisable(GL_CULL_FACE);

    glFuncs->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glFuncs->glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
}
void ModelViewer::resizeGL(int w, int h)
{
    glFuncs = context()->functions();

    glFuncs->glViewport(0, 0, w, h);
}
void ModelViewer::paintGL()
{
    glFuncs = context()->functions();

    glFuncs->glClear(GL_COLOR_BUFFER_BIT);
}

#include "moc_modelviewer.cpp"
