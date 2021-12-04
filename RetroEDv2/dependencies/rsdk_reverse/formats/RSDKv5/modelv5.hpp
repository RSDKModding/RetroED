#ifndef MODEL_V5_H
#define MODEL_V5_H

namespace RSDKv5
{

class Model
{
public:
    class Colour
    {
    public:
        byte r = 0xFF;
        byte g = 0x00;
        byte b = 0xFF;
        byte a = 0xFF;
        Colour() {}
        Colour(Reader &reader) { read(reader); }

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

    byte signature[4] = { 'M', 'D', 'L', 0 };

    bool hasNormals         = true;
    bool hasTextures        = false;
    bool hasColours         = true;
    byte faceVerticiesCount = 3;
    QList<TexCoord> texCoords;
    QList<Colour> colours;
    QList<Frame> frames;
    QList<ushort> indices;

    QString filePath = "";
};

} // namespace RSDKv5

#endif // MODEL_V5_H
