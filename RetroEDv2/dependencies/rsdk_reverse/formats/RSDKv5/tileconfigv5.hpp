#ifndef TILECONFIG_V5_H
#define TILECONFIG_V5_H

namespace RSDKv5
{

class TileConfig
{
public:
    class CollisionMask
    {
    public:
        struct HeightMask {
            HeightMask() {}

            byte height = 0;
            bool solid  = false;
        };

        CollisionMask() {}

        void read(Reader &reader)
        {
            QByteArray buf = reader.readByteArray(0x10);
            for (int i = 0; i < 0x10; ++i) collision[i].height = buf[i];
            buf = reader.readByteArray(0x10);
            for (int i = 0; i < 0x10; ++i) collision[i].solid = buf[i];
            flipY      = reader.read<bool>();
            floorAngle = reader.read<byte>();
            rWallAngle = reader.read<byte>();
            lWallAngle = reader.read<byte>();
            roofAngle  = reader.read<byte>();
            behaviour  = reader.read<byte>();
        }

        void write(Writer &writer)
        {
            for (int i = 0; i < 0x10; ++i) writer.write(collision[i].height);
            for (int i = 0; i < 0x10; ++i) writer.write(collision[i].solid);

            writer.write(flipY);
            writer.write(floorAngle);
            writer.write(rWallAngle);
            writer.write(lWallAngle);
            writer.write(roofAngle);
            writer.write(behaviour);
        }

        HeightMask collision[16];
        byte floorAngle = 0x00;
        byte rWallAngle = 0xC0;
        byte lWallAngle = 0x40;
        byte roofAngle  = 0x80;
        byte behaviour  = 0;
        bool flipY      = false;
    };

    TileConfig() {}
    TileConfig(QString filename) { read(filename); }
    TileConfig(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader)
    {
        filePath = reader.filePath;
        if (!reader.matchesSignature(signature, 4))
            return;

        Reader creader = reader.getCReader();

        for (int c = 0; c < 2; ++c)
            for (int t = 0; t < 0x400; ++t) collisionPaths[c][t].read(creader);
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer)
    {
        filePath = writer.filePath;
        QByteArray compressed;
        QBuffer buffer(&compressed);
        buffer.open(QIODevice::Append);
        QDataStream *cmem = new QDataStream(&buffer);
        Writer cwriter(cmem);

        for (int c = 0; c < 2; ++c)
            for (int t = 0; t < 0x400; ++t) collisionPaths[c][t].write(cwriter);

        writer.write(signature, 4);
        writer.writeCompressed(compressed);
        writer.flush();
    }

    byte signature[4] = { 'T', 'I', 'L', 0 };

    CollisionMask collisionPaths[2][0x400];

    QString filePath = "";
};

} // namespace RSDKv5

#endif // TILECONFIG_V5_H
