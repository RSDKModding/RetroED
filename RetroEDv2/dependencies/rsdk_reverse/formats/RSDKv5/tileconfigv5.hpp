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
        CollisionMask() {}

        void read(Reader &reader)
        {
            QByteArray buf = reader.readByteArray(0x10);
            for (int i = 0; i < 0x10; ++i) m_collision[i] = buf[i];
            buf = reader.readByteArray(0x10);
            for (int i = 0; i < 0x10; ++i) m_hasCollision[i] = buf[i];
            m_isCeiling    = reader.read<bool>();
            m_floorAngle   = reader.read<byte>();
            m_rWallAngle   = reader.read<byte>();
            m_lWallAngle   = reader.read<byte>();
            m_roofAngle = reader.read<byte>();
            m_behaviour    = reader.read<byte>();
        }

        void write(Writer &writer)
        {
            for (int i = 0; i < 0x10; ++i) writer.write(m_collision[i]);
            for (int i = 0; i < 0x10; ++i) writer.write(m_hasCollision[i]);

            writer.write(m_isCeiling);
            writer.write(m_floorAngle);
            writer.write(m_rWallAngle);
            writer.write(m_lWallAngle);
            writer.write(m_roofAngle);
            writer.write(m_behaviour);
        }

        byte m_collision[0x10];
        bool m_hasCollision[0x10];
        byte m_floorAngle   = 0x00;
        byte m_rWallAngle   = 0xC0;
        byte m_lWallAngle   = 0x40;
        byte m_roofAngle = 0x80;
        byte m_behaviour    = 0;
        bool m_isCeiling    = false;
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
        m_filename = reader.m_filepath;
        if (!reader.matchesSignature(m_signature, 4))
            return;

        Reader creader = reader.getCReader();

        for (int c = 0; c < 2; ++c)
            for (int t = 0; t < 0x400; ++t) m_collisionPaths[c][t].read(creader);
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer)
    {
        m_filename = writer.m_filename;
        QByteArray compressed;
        QBuffer buffer(&compressed);
        buffer.open(QIODevice::Append);
        QDataStream *cmem = new QDataStream(&buffer);
        Writer cwriter(cmem);

        for (int c = 0; c < 2; ++c)
            for (int t = 0; t < 0x400; ++t) m_collisionPaths[c][t].write(cwriter);

        writer.write(m_signature, 4);
        writer.writeCompressed(compressed);
        writer.flush();
    }

    byte m_signature[4] = { 'T', 'I', 'L', 0 };

    CollisionMask m_collisionPaths[0x2][0x400];

    QString m_filename = "";
};

} // namespace RSDKv5

#endif // TILECONFIG_V5_H
