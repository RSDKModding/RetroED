#pragma once

namespace RSDKv5
{

class Model
{
public:
    class Color
    {
    public:
        byte r = 0xFF;
        byte g = 0x00;
        byte b = 0xFF;
        byte a = 0xFF;
        Color() {}
        Color(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            b = reader.read<byte>();
            g = reader.read<byte>();
            r = reader.read<byte>();
            a = reader.read<byte>();
        }

        void write(Writer &writer)
        {
            writer.write(b);
            writer.write(g);
            writer.write(r);
            writer.write(a);
        }
    };

    class TexCoord
    {
    public:
        float x = 0.0f;
        float y = 0.0f;

        TexCoord() {}
        TexCoord(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            x = reader.read<float>();
            y = reader.read<float>();
        }

        void write(Writer &writer)
        {
            writer.write(x);
            writer.write(y);
        }
    };

    class Frame
    {
    public:
        class Vertex
        {
        public:
            float x  = 0.0f;
            float y  = 0.0f;
            float z  = 0.0f;
            float nx = 0.0f;
            float ny = 0.0f;
            float nz = 0.0f;

            Vertex() {}
        };

        QList<Vertex> vertices;

        Frame() {}
    };

    Model() {}
    Model(QString filename) { read(filename); }
    Model(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

    inline void write(QString filename = "")
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    void writeAsOBJ(QString filePath, int exportFrame = -1);
    void writeMTL(QString filepath);

    void loadPLY(QString filePath);
    void writeAsPLY(QString filePath, int exportFrame = -1);

    byte signature[4] = { 'M', 'D', 'L', 0 };

    bool hasNormals        = true;
    bool hasTextures       = false;
    bool hasColors        = true;
    byte faceVerticesCount = 3;
    QList<TexCoord> texCoords;
    QList<Color> colors;
    QList<Frame> frames;
    QList<ushort> indices;

    QString filePath = "";

private:
    enum PLYPropTypes {
        UNKNOWN,
        INT8,
        UINT8,
        INT16,
        UINT16,
        INT32,
        UINT32,
        FLOAT,
        DOUBLE,
        LIST,
    };

    struct PLYProperty {
        PLYProperty() {}

        QString sizeType = ""; // used for list types
        QString itemType = ""; // used for list types
        QString type     = "";
        QString name     = "";
    };

    struct PLYPropertyValue {
        PLYPropertyValue() {}

        QList<QVariant> values; // if a list property exists, assume ONLY a list can exist
    };

    struct PLYElement {
        PLYElement() {}

        QList<PLYProperty> properties;
        QList<PLYPropertyValue> propertyValues;
        QString name = "";
    };

    int getPLYPropertyType(QString propertyType);
};

} // namespace RSDKv5


