#ifndef MODELVIEWER_H
#define MODELVIEWER_H

class ModelViewer : public QOpenGLWidget
{
    Q_OBJECT

public:
    struct Camera {
        Camera() { reset(); }

        inline void reset()
        {
            m_position *= 0;
            m_rotation *= 0;
            m_qRotation = m_qRotation.fromEulerAngles(m_rotation);
            m_scale     = { 1, 1, 1 };
            changed     = true;
        }

        inline void setPosition(float x, float y, float z)
        {
            m_position = { x, y, z };
            changed    = true;
        }

        inline void setPosition(QVector3D pos)
        {
            m_position = pos;
            changed    = true;
        }

        inline void translate(float x, float y, float z)
        {
            m_position += { x, y, z };
            changed = true;
        }

        inline void translate(QVector3D pos)
        {
            m_position += pos;
            changed = true;
        }

        inline void translateX(float x)
        {
            m_position.setX(m_position.x() + x);
            changed = true;
        }

        inline void translateY(float y)
        {
            m_position.setY(m_position.y() + y);
            changed = true;
        }

        inline void translateZ(float z)
        {
            m_position.setZ(m_position.z() + z);
            changed = true;
        }

        inline void translateLeft(float dist)
        {
            if (!changed)
                updateMatrix();

            m_position += this->left * dist;
            changed = true;
        }

        inline void translateUp(float dist)
        {
            if (!changed)
                updateMatrix();

            m_position += this->up * dist;
            changed = true;
        }

        inline void translateForward(float dist)
        {
            if (!changed)
                updateMatrix();

            m_position += this->forward * dist;
            changed = true;
        }

        inline void setRotation(float x, float y, float z)
        {
            m_rotation  = { x, y, z };
            m_qRotation = m_qRotation.fromEulerAngles(m_rotation);

            changed = true;
        }

        inline void setRotation(QVector3D rot)
        {
            m_rotation  = rot;
            m_qRotation = m_qRotation.fromEulerAngles(m_rotation);

            changed = true;
        }

        inline void setRotation(QQuaternion rot)
        {
            m_qRotation = rot;
            float yaw, pitch, roll;
            m_qRotation.getEulerAngles(&pitch, &yaw, &roll);
            m_rotation = { pitch, yaw, roll };
            changed    = true;
        }

        inline void rotateX(float x)
        {
            m_rotation.setX(m_rotation.x() + x);
            m_qRotation = m_qRotation.fromEulerAngles(m_rotation);
            changed     = true;
        }

        inline void rotateY(float y)
        {
            m_rotation.setY(m_rotation.y() + y);
            m_qRotation = m_qRotation.fromEulerAngles(m_rotation);
            changed     = true;
        }

        inline void rotateZ(float z)
        {
            m_rotation.setZ(m_rotation.z() + z);
            m_qRotation = m_qRotation.fromEulerAngles(m_rotation);
            changed     = true;
        }

        inline void rotate(QQuaternion rot)
        {
            float yaw, pitch, roll;
            m_qRotation *= rot;
            m_qRotation.getEulerAngles(&pitch, &yaw, &roll);
            m_rotation = { pitch, yaw, roll };
            changed    = true;
        }

        inline void rotate(float angle, QVector3D axis)
        {
            if (!changed)
                updateMatrix();

            float yaw, pitch, roll;
            m_qRotation = m_qRotation * QQuaternion::fromAxisAndAngle(axis, angle);
            m_qRotation.getEulerAngles(&pitch, &yaw, &roll);
            m_rotation = { pitch, yaw, roll };
            changed    = true;
        }

        inline void rotateAxisLeft(float angle)
        {
            if (!changed)
                updateMatrix();

            rotate(angle, { 1, 0, 0 });
        }

        inline void rotateAxisUp(float angle)
        {
            if (!changed)
                updateMatrix();

            rotate(angle, { 0, 1, 0 });
        }

        inline void rotateAxisForward(float angle)
        {
            if (!changed)
                updateMatrix();

            rotate(angle, { 0, 0, 1 });
        }

        inline void removeRoll()
        {
            m_rotation.setZ(0);
            changed = true;
        }

        inline void setScale(QVector3D scale)
        {
            m_scale = scale;
            changed = true;
        }

        inline void focus(QVector3D focus)
        {
            if (changed)
                updateMatrix();

            m_qRotation = QQuaternion::fromDirection(
                m_position - focus, QVector3D::crossProduct(focus - m_position, this->forward));

            // calc new left vector
            updateMatrix();

            m_qRotation = QQuaternion::fromDirection(
                m_position - focus, QVector3D::crossProduct(focus - m_position, this->left));

            float yaw, pitch, roll;
            m_qRotation.getEulerAngles(&pitch, &yaw, &roll);
            m_rotation = { pitch, yaw, roll };
            changed    = true;

            rotateAxisForward(-roll);
        }

        inline void orbit(float x, float y, QVector3D focus)
        {
            if (changed)
                updateMatrix();

            float dist = m_position.distanceToPoint(focus);
            m_position = focus;

            QQuaternion rot;
            float yaw, pitch, roll;
            m_qRotation = m_qRotation * rot.fromAxisAndAngle({ 0, 1, 0 }, x);
            m_qRotation = m_qRotation * rot.fromAxisAndAngle({ 1, 0, 0 }, y);
            m_qRotation.getEulerAngles(&pitch, &yaw, &roll);
            m_rotation = { pitch, yaw, roll };

            updateMatrix();

            m_position -= this->forward * dist;
            changed = true;
        }

        inline QVector3D position() const { return m_position; }
        inline QVector3D rotation() const { return m_rotation; }
        inline QVector3D scale() const { return m_scale; }

        inline QMatrix4x4 toMatrix()
        {
            updateMatrix();
            return matView;
        }

    private:
        void updateMatrix()
        {
            if (!changed)
                return;

            matView.setToIdentity();

            matView.translate(m_position);
            matView.rotate(m_qRotation);
            matView.scale(m_scale);

            left    = -matView.column(0).toVector3D().normalized();
            up      = matView.column(1).toVector3D().normalized();
            forward = -matView.column(2).toVector3D().normalized();

            changed = false;
        }

        QVector3D m_position;
        QVector3D m_rotation;
        QQuaternion m_qRotation;
        QVector3D m_scale;

        QVector3D left;
        QVector3D up;
        QVector3D forward;

        QMatrix4x4 matView;

        bool changed = false;
    };

    ModelViewer(QWidget *parent = nullptr);
    ~ModelViewer();

    RSDKv5::Model model;

    RSDKv5::Model::Frame *curFrame  = nullptr;
    RSDKv5::Model::Frame *nextFrame = nullptr;
    int loopIndex                   = 0;
    float animSpeed                 = 0.1f;
    float animTimer                 = 0;

    Camera camera;

    void resetCamera();

    bool showNormals   = false;
    float zoom         = 1.0f;
    QColor modelColour = QColor(0xFFFFFFFF);

    void setModel(RSDKv5::Model m, QString tex = "");
    void setModel(RSDKv4::Model m, QString tex = "");

    void loadTexture(QString texturePath);

    void setFrame(int frameID);
    void setWireframe(bool wireframe);
    void setNormalsVisible(bool show);
    void setZoom(float zoom);

    QString texFile = "";
    bool reload     = false;

    float *vertBuf;
    float *normBuf;

    bool wireframe = false;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    QSize sizeHint() const { return QSize(0, 0); }

private:
    QMatrix4x4 matModel;
    QMatrix4x4 matWorld;

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
