#ifndef SHADERS_H
#define SHADERS_H

class Shader
{
public:
    Shader() { m_program = new QOpenGLShaderProgram(); }

    ~Shader() { delete m_program; }

    inline void restartShader()
    {
        if (m_program)
            delete m_program;
        m_program = new QOpenGLShaderProgram();
    }

    inline bool loadShader(QString path, QOpenGLShader::ShaderType type)
    {
        if (!m_program)
            return false;

        bool success = m_program->addShaderFromSourceFile(type, path.toStdString().c_str());
        if (success) {
            qDebug() << QString("Loaded OpenGL Shader: ") + QFileInfo(path).fileName();
        }
        else {
            qDebug() << QString("Error Loading OpenGL Shader: ") + QFileInfo(path).fileName();
            qDebug() << QString("OpenGL Shader Error: ") + getError();
        }
        return success;
    }

    inline bool link()
    {
        if (!m_program)
            return false;
        m_initialised = m_program->link();
        return m_initialised;
    }

    inline bool use()
    {
        if (!m_program)
            return false;
        return m_program->bind();
    }

    inline void unbind()
    {
        if (!m_program)
            return;
        m_program->release();
    }

    inline QString getError()
    {
        if (!m_program)
            return "Program was null";
        return m_program->log();
    }

    inline void enableAttributeArray(int loc)
    {
        if (!m_program)
            return;
        m_program->enableAttributeArray(loc);
    }

    inline void enableAttributeArray(QString name)
    {
        if (!m_program)
            return;
        m_program->enableAttributeArray(name.toStdString().c_str());
    }

    inline int uniformLocation(QString name)
    {
        if (!m_program)
            return -1;
        return m_program->uniformLocation(name);
    }

    inline int uniformLocation(const char *name)
    {
        if (!m_program)
            return -1;
        return m_program->uniformLocation(name);
    }

    inline void setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0)
    {
        if (!m_program)
            return;
        m_program->setAttributeBuffer(location, type, offset, tupleSize, stride);
    }

    inline void setAttributeBuffer(QString name, GLenum type, int offset, int tupleSize, int stride = 0)
    {
        if (!m_program)
            return;
        m_program->setAttributeBuffer(name.toStdString().c_str(), type, offset, tupleSize, stride);
    }

    inline void setAttributeBuffer(const char *name, GLenum type, int offset, int tupleSize,
                                   int stride = 0)
    {
        if (!m_program)
            return;
        m_program->setAttributeBuffer(name, type, offset, tupleSize, stride);
    }

    inline void setValue(QString name, bool value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            m_program->setUniformValue(loc, value ? 1 : 0);
    }

    inline void setValue(QString name, int value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            m_program->setUniformValue(loc, value);
    }

    inline void setValue(QString name, float value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            m_program->setUniformValue(loc, value);
    }

    inline void setValue(QString name, Vector2<float> value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            m_program->setUniformValue(loc, value.x, value.y);
    }

    inline void setValue(QString name, Vector3<float> value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            m_program->setUniformValue(loc, value.x, value.y, value.z);
    }

    inline void setValue(QString name, Vector4<float> value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            m_program->setUniformValue(loc, value.x, value.y, value.z, value.w);
    }

    inline void setValue(QString name, QVector2D value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            m_program->setUniformValue(loc, value.x(), value.y());
    }

    inline void setValue(QString name, QVector3D value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            m_program->setUniformValue(loc, value.x(), value.y(), value.z());
    }

    inline void setValue(QString name, QVector4D value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            m_program->setUniformValue(loc, value.x(), value.y(), value.z(), value.w());
    }

    inline void setValue(QString name, QMatrix2x2 value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1) {
            m_program->setUniformValue(loc, value);
        }
    }

    inline void setValue(QString name, QMatrix3x3 value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1) {
            m_program->setUniformValue(loc, value);
        }
    }

    inline void setValue(QString name, QMatrix4x4 value)
    {
        if (!m_program)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1) {
            m_program->setUniformValue(loc, value);
        }
    }

    // the program ID
    QOpenGLShaderProgram *m_program;
    bool m_initialised = false;
};

#endif // SHADERS_H
