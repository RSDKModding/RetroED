#ifndef MODEL_V4_H
#define MODEL_V4_H

namespace RSDKv4
{

class Model
{
public:
    class TexCoord
    {
    public:
        float x;
        float y;

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
            float x;
            float y;
            float z;
            float nx;
            float ny;
            float nz;

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

    byte signature[4] = { 'R', '3', 'D', 0 };

    QList<TexCoord> texCoords;
    QList<Frame> frames;
    QList<ushort> indices;

    QString filePath = "";
};

} // namespace RSDKv4

#endif // MODEL_V4_H
