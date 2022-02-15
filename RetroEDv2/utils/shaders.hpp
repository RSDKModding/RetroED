#ifndef SHADERS_H
#define SHADERS_H

class Shader
{
public:
    Shader(QString name = "Shader")
    {
        this->name = name;
        programID  = new QOpenGLShaderProgram();
    }

    ~Shader() { delete programID; }

    inline void restartShader()
    {
        if (programID)
            delete programID;
        programID = new QOpenGLShaderProgram();
    }

    std::function<void(Shader *, void *)> argsCB = [](Shader*, void*){};

    inline void setArgs(void *state) { argsCB(this, state); }

    inline bool loadShader(QString path, QOpenGLShader::ShaderType type)
    {
        if (!programID)
            return false;

        bool success = programID->addShaderFromSourceFile(type, path.toStdString().c_str());
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
        if (!programID)
            return false;
        m_initialised = programID->link();
        return m_initialised;
    }

    inline bool use()
    {
        if (!programID)
            return false;
        return programID->bind();
    }

    inline void unbind()
    {
        if (!programID)
            return;
        programID->release();
    }

    inline QString getError()
    {
        if (!programID)
            return "Program was null";
        return programID->log();
    }

    inline void enableAttributeArray(int loc)
    {
        if (!programID)
            return;
        programID->enableAttributeArray(loc);
    }

    inline void enableAttributeArray(QString name)
    {
        if (!programID)
            return;
        programID->enableAttributeArray(name.toStdString().c_str());
    }

    inline int uniformLocation(QString name)
    {
        if (!programID)
            return -1;
        return programID->uniformLocation(name);
    }

    inline int uniformLocation(const char *name)
    {
        if (!programID)
            return -1;
        return programID->uniformLocation(name);
    }

    inline void setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0)
    {
        if (!programID)
            return;
        programID->setAttributeBuffer(location, type, offset, tupleSize, stride);
    }

    inline void setAttributeBuffer(QString name, GLenum type, int offset, int tupleSize, int stride = 0)
    {
        if (!programID)
            return;
        programID->setAttributeBuffer(name.toStdString().c_str(), type, offset, tupleSize, stride);
    }

    inline void setAttributeBuffer(const char *name, GLenum type, int offset, int tupleSize,
                                   int stride = 0)
    {
        if (!programID)
            return;
        programID->setAttributeBuffer(name, type, offset, tupleSize, stride);
    }

    inline void setValue(QString name, bool value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            programID->setUniformValue(loc, value ? 1 : 0);
    }

    inline void setValue(QString name, int value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            programID->setUniformValue(loc, value);
    }

    inline void setValue(QString name, float value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            programID->setUniformValue(loc, value);
    }

    inline void setValue(QString name, Vector2<float> value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            programID->setUniformValue(loc, value.x, value.y);
    }

    inline void setValue(QString name, Vector3<float> value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            programID->setUniformValue(loc, value.x, value.y, value.z);
    }

    inline void setValue(QString name, Vector4<float> value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            programID->setUniformValue(loc, value.x, value.y, value.z, value.w);
    }

    inline void setValue(QString name, QVector2D value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            programID->setUniformValue(loc, value.x(), value.y());
    }

    inline void setValue(QString name, QVector3D value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            programID->setUniformValue(loc, value.x(), value.y(), value.z());
    }

    inline void setValue(QString name, QVector4D value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1)
            programID->setUniformValue(loc, value.x(), value.y(), value.z(), value.w());
    }

    inline void setValue(QString name, QMatrix2x2 value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1) {
            programID->setUniformValue(loc, value);
        }
    }

    inline void setValue(QString name, QMatrix3x3 value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1) {
            programID->setUniformValue(loc, value);
        }
    }

    inline void setValue(QString name, QMatrix4x4 value)
    {
        if (!programID)
            return;
        uint loc = uniformLocation(name.toStdString().c_str());

        if (loc != (uint)-1) {
            programID->setUniformValue(loc, value);
        }
    }

    // the program ID
    QOpenGLShaderProgram *programID;
    bool m_initialised = false;
    QString name       = "Shader";
};

#endif // SHADERS_H
