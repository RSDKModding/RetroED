#ifndef BACKGROUND_V1_H
#define BACKGROUND_V1_H

namespace RSDKv2
{

class Background
{
public:
    class ScrollInfo
    {
    public:
        ScrollInfo() {}
        ScrollInfo(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            parallaxFactor = reader.read<byte>();
            scrollSpeed = reader.read<byte>();
            deform     = reader.read<byte>();
        }
        void write(Writer &writer)
        {
            writer.write(parallaxFactor);
            writer.write(scrollSpeed);
            writer.write(deform);
        }

        byte parallaxFactor = 0;
        byte scrollSpeed = 0;
        byte deform     = 0;
    };

    class Layer
    {
    public:
        Layer() {}
        Layer(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        QList<QList<byte>> layout;

        byte width         = 0;
        byte height        = 0;
        byte type     = 0;
        byte parallaxFactor = 0;
        byte scrollSpeed = 0;
        QByteArray lineIndexes;
    };

    Background() {}
    Background(QString filename) { read(filename); }
    Background(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

    inline void write(QString filename)
    {
        if (filename == "")
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QList<Layer> layers;
    QList<ScrollInfo> hScroll;
    QList<ScrollInfo> vScroll;

    QString m_filename = "";
};

} // namespace RSDKv2

#endif // BACKGROUND_V1_H
