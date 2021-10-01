#ifndef BACKGROUND_V4_H
#define BACKGROUND_V4_H

namespace RSDKv4
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
            parallaxFactor |= (short)reader.read<byte>();
            parallaxFactor = (short)(reader.read<byte>() << 8);
            scrollSpeed    = reader.read<byte>();
            deform         = reader.read<byte>();
        }
        void write(Writer &writer)
        {
            writer.write((byte)(parallaxFactor & 0xFF));
            writer.write((byte)(parallaxFactor >> 8));
            writer.write(scrollSpeed);
            writer.write(deform);
        }

        short parallaxFactor = 1 << 8;
        byte scrollSpeed     = 0 << 8;
        byte deform          = 0;
    };

    class Layer
    {
    public:
        Layer() {}
        Layer(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        QList<QList<ushort>> layout;

        byte width           = 0;
        byte height          = 0;
        byte type            = 0;
        short parallaxFactor = 1 << 8;
        byte scrollSpeed     = 0 << 0;
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

} // namespace RSDKv4

#endif // BACKGROUND_V4_H
