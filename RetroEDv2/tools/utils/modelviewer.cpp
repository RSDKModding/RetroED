#include "includes.hpp"

ModelViewer::ModelViewer(QWidget *parent) : QOpenGLWidget(parent)
{
    setMouseTracking(true);

    this->setFocusPolicy(Qt::WheelFocus);

    model = RSDKv5::Model();

    // renderTimer = new QTimer(this);
    // connect(renderTimer, &QTimer::timeout, this, [this] { this->repaint(); });
    // renderTimer->start(1000 / 60.f);
}

ModelViewer::~ModelViewer()
{
    curFrame  = nullptr;
    nextFrame = nullptr;
}

void ModelViewer::setModel(RSDKv5::Model m, QString tex)
{
    model   = m;
    texFile = tex;
    reload  = true;

    setFrame(0);
}

void ModelViewer::setModel(RSDKv4::Model m, QString tex)
{
    model.faceVerticesCount = 3;
    model.indices           = m.indices;

    model.colours.clear();
    model.hasColours = false;

    model.texCoords.clear();
    for (auto &uv : m.texCoords) {
        RSDKv5::Model::TexCoord v5uv;
        v5uv.x = uv.x;
        v5uv.y = uv.y;
        model.texCoords.append(v5uv);
    }
    model.hasTextures = true;

    model.frames.clear();
    for (auto &f : m.frames) {
        RSDKv5::Model::Frame v5f;
        for (auto &v : f.vertices) {
            RSDKv5::Model::Frame::Vertex v5v;
            v5v.x  = v.x;
            v5v.y  = v.y;
            v5v.z  = v.z;
            v5v.nx = v.nx;
            v5v.ny = v.ny;
            v5v.nz = v.nz;
            v5f.vertices.append(v5v);
        }
        model.frames.append(v5f);
    }
    model.hasNormals = true;

    texFile = tex;

    setFrame(0);
}

RSDKv4::Model ModelViewer::getModelv4()
{
    RSDKv4::Model mdl;

    mdl.texCoords.clear();
    if (model.hasTextures) {
        for (auto &texCoord : model.texCoords) {
            RSDKv4::Model::TexCoord coord;
            coord.x = texCoord.x;
            coord.y = texCoord.y;
            mdl.texCoords.append(coord);
        }
    }
    else {
        int vertCnt = 0;
        if (model.frames.count() >= 1)
            vertCnt = model.frames[0].vertices.count();

        for (int t = 0; t < vertCnt; ++t) {
            RSDKv4::Model::TexCoord coord;
            coord.x = 0;
            coord.y = 0;
            mdl.texCoords.append(coord);
        }
    }

    mdl.indices.clear();
    for (auto &index : model.indices) mdl.indices.append(index);

    mdl.frames.clear();
    for (auto &frame : model.frames) {
        RSDKv4::Model::Frame f;

        for (auto &vert : frame.vertices) {
            RSDKv4::Model::Frame::Vertex vertex;
            vertex.x = vert.x;
            vertex.y = vert.y;
            vertex.z = vert.z;

            vertex.nx = 0;
            vertex.ny = 0;
            vertex.nz = 0;

            if (model.hasNormals) {
                vertex.nx = vert.nx;
                vertex.ny = vert.ny;
                vertex.nz = vert.nz;
            }

            f.vertices.append(vertex);
        }

        mdl.frames.append(f);
    }

    return mdl;
}

void ModelViewer::loadTexture(QString texturePath)
{
    texFile = texturePath;
    reload  = true;

    repaint();
}

void ModelViewer::setFrame(int frameID)
{
    curFrame = nullptr;
    if (frameID >= 0 && frameID < model.frames.count()) {
        curFrame = &model.frames[frameID];
        if (frameID + 1 >= model.frames.count())
            nextFrame = &model.frames[loopIndex];
        else
            nextFrame = &model.frames[frameID + 1];

        reload = true;
    }

    repaint();
}

void ModelViewer::setWireframe(bool wireframe)
{
    this->wireframe = wireframe;
    repaint();
}
void ModelViewer::setNormalsVisible(bool show)
{
    showNormals = show;
    repaint();
}

void ModelViewer::setZoom(float zoom)
{
    this->zoom = zoom;

    repaint();
}

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

    glFuncs->glEnable(GL_DEPTH_TEST);
    glFuncs->glDisable(GL_DITHER);

    glFuncs->glEnable(GL_BLEND);
    glFuncs->glDisable(GL_SCISSOR_TEST);
    glFuncs->glDisable(GL_CULL_FACE);

    glFuncs->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFuncs->glDepthFunc(GL_LESS);
    glFuncs->glDepthRangef(0.1f, 256);

    glFuncs->glClearColor(23 / 255.f, 23 / 255.f, 23 / 255.f, 1.0f);

    shader.loadShader(":/shaders/3d/default.vert", QOpenGLShader::Vertex);
    shader.loadShader(":/shaders/3d/default.frag", QOpenGLShader::Fragment);
    shader.link();
    shader.use();
    shader.setValue("tex", 0);

    VAO = new QOpenGLVertexArrayObject;
    VAO->create();
    VAO->bind();

    vertVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertVBO->create();
    vertVBO->bind();
    vertVBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    shader.setAttributeBuffer("in_pos", GL_FLOAT, 0, 3);
    shader.enableAttributeArray("in_pos");

    normalVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    normalVBO->create();
    normalVBO->bind();
    normalVBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    shader.setAttributeBuffer("in_norm", GL_FLOAT, 0, 3);
    shader.enableAttributeArray("in_norm");

    colorVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    colorVBO->create();
    colorVBO->bind();
    colorVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    shader.setAttributeBuffer("in_color", GL_UNSIGNED_BYTE, 0, 4);
    shader.enableAttributeArray("in_color");

    texVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    texVBO->create();
    texVBO->bind();
    texVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    shader.setAttributeBuffer("in_UV", GL_FLOAT, 0, 2);
    shader.enableAttributeArray("in_UV");

    indexVBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexVBO->create();
    indexVBO->bind();
    indexVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);

    resetCamera();
}

void ModelViewer::resetCamera()
{
    matModel.setToIdentity();

    zoom = 1.0f;
    camera.reset();
    camera.translate(0, 0, -128);
}

void ModelViewer::resizeGL(int w, int h)
{
    glFuncs = context()->functions();

    glFuncs->glViewport(0, 0, w, h);

    matWorld.setToIdentity();
    matWorld.perspective(45, w / (float)h, 0.1f, 256);
}

void ModelViewer::paintGL()
{
    glFuncs = context()->functions();
    glFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!curFrame || !curFrame->vertices.count())
        return;

    shader.use();

    if (reload) {
        reload      = false;
        int vc      = curFrame->vertices.count();
        vertBuf     = new float[vc * 3];
        normBuf     = new float[vc * 3];
        auto colors = new RSDKv5::Model::Colour[vc];
        auto uvs    = new RSDKv5::Model::TexCoord[vc];

        int i = 0;
        for (auto &c : model.colours) colors[i++] = c;

        i = 0;
        for (auto &uv : model.texCoords) uvs[i++] = uv;

        // 0 3 5 turned to 3:   0 3 5
        // 0 3 5 4 turned to 3: 0 3 5 5 4 0
        // etc
        int count       = 3 * model.faceVerticesCount - 3;
        int total       = count * (model.indices.count() / model.faceVerticesCount);
        auto indices    = new ushort[total];
        ushort *current = indices;

        i = 0;
        for (; i < model.indices.count() - 1; i += model.faceVerticesCount) {
            for (int j = 0; j < model.faceVerticesCount - 1; ++j) {
                current[j * 3 + 0] = model.indices[i + j];
                current[j * 3 + 1] = model.indices[i + j + 1];
                if (j + 1 < model.faceVerticesCount - 1)
                    current[j * 3 + 2] = model.indices[i + j + 2];
            }
            // if (model.faceVerticesCount != 3)
            current[count - 1] = model.indices[i];
            current            = &current[count];
        }

        vertVBO->bind();
        vertVBO->allocate(vc * sizeof(float) * 3);
        normalVBO->bind();
        normalVBO->allocate(vc * sizeof(float) * 3);
        colorVBO->bind();
        colorVBO->allocate(colors, vc * sizeof(RSDKv5::Model::Colour));
        texVBO->bind();
        texVBO->allocate(uvs, vc * sizeof(RSDKv5::Model::TexCoord));
        indexVBO->bind();
        indexVBO->allocate(indices, total * sizeof(ushort));

        shader.setValue("useColor", model.hasColours);
        shader.setValue("useTextures", model.hasTextures);
        shader.setValue("useNormals", model.hasNormals);

        delete[] colors;
        delete[] uvs;
    }

    if (texFile != curTex) {
        delete tex;
        QImage src(texFile);
        tex = new QOpenGLTexture(QOpenGLTexture::Target2D);
        tex->create();
        tex->bind();
        tex->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::WrapMode::Repeat);
        tex->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::WrapMode::Repeat);
        tex->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        tex->setFormat(QOpenGLTexture::RGBA8_UNorm);
        tex->setSize(src.width(), src.height());
        tex->setData(src, QOpenGLTexture::GenerateMipMaps);
        glFuncs->glActiveTexture(GL_TEXTURE0);
        glFuncs->glBindTexture(GL_TEXTURE_2D, tex->textureId());
    }

    // handle interpolation, set vertVBO properly using vertVBO->write

    for (int i = 0; i < curFrame->vertices.count(); ++i) {
        auto &cv           = curFrame->vertices.at(i);
        auto &nv           = nextFrame->vertices.at(i);
        float interp       = 1 - animTimer;
        float interp2      = animTimer;
        vertBuf[i * 3 + 0] = cv.x * interp + nv.x * interp2;
        vertBuf[i * 3 + 1] = cv.y * interp + nv.y * interp2;
        vertBuf[i * 3 + 2] = cv.z * interp + nv.z * interp2;
        normBuf[i * 3 + 0] = cv.nx * interp + nv.nx * interp2;
        normBuf[i * 3 + 1] = cv.ny * interp + nv.ny * interp2;
        normBuf[i * 3 + 2] = cv.nz * interp + nv.nz * interp2;
    }
    vertVBO->bind();
    vertVBO->write(0, vertBuf, curFrame->vertices.count() * sizeof(float) * 3);
    normalVBO->bind();
    normalVBO->write(0, normBuf, curFrame->vertices.count() * sizeof(float) * 3);

    matModel.setToIdentity();
    matModel.scale(zoom, zoom, zoom);

    shader.setValue("default_color", QVector4D(modelColour.redF(), modelColour.greenF(),
                                               modelColour.blueF(), modelColour.alphaF()));

    shader.setValue("projection", matWorld);
    shader.setValue("view", camera.toMatrix());
    shader.setValue("model", matModel);

    glFuncs->glDrawElements(wireframe ? GL_LINES : GL_TRIANGLES, indexVBO->size() / sizeof(ushort),
                            GL_UNSIGNED_SHORT, 0);
}

#include "moc_modelviewer.cpp"
