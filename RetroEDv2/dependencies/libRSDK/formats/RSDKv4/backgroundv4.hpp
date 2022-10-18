#pragma once

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
            parallaxFactor = (short)reader.read<byte>();
            parallaxFactor |= (short)(reader.read<byte>() << 8);
            scrollSpeed = reader.read<byte>();
            deform      = reader.read<byte>();
        }
        void write(Writer &writer)
        {
            writer.write((byte)(parallaxFactor & 0xFF));
            writer.write((byte)(parallaxFactor >> 8));
            writer.write(scrollSpeed);
            writer.write(deform);
        }

        short parallaxFactor = 1 << 8;
        byte scrollSpeed     = 0 << 6;
        bool deform          = false;
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
        byte type            = 1;
        short parallaxFactor = 1 << 8;
        byte scrollSpeed     = 0 << 6;
        QByteArray lineScroll;
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
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QList<Layer> layers;
    QList<ScrollInfo> hScroll;
    QList<ScrollInfo> vScroll;

    QString filePath = "";
};

} // namespace RSDKv4


