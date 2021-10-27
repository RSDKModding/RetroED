#ifndef TILECONFIG_V4_H
#define TILECONFIG_V4_H

namespace RSDKv4
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
        CollisionMask(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            byte flags = reader.read<byte>();
            flipY      = (flags >> 4) != 0;
            behaviour  = (byte)(flags & 0xF);
            floorAngle = reader.read<byte>();
            lWallAngle = reader.read<byte>();
            rWallAngle = reader.read<byte>();
            roofAngle  = reader.read<byte>();

            QByteArray collisionBytes = reader.readByteArray(8);

            ushort activeCollision = reader.read<byte>() << 8;
            activeCollision |= reader.read<byte>();

            int i  = 0;
            int i2 = 1;

            for (int c = 0; c < 8; ++c) {
                collision[i].height  = (byte)((collisionBytes[c] & 0xF0) >> 4);
                collision[i2].height = (byte)(collisionBytes[c] & 0x0F);
                i += 2;
                i2 += 2;
            }

            for (int c = 0; c < 16; ++c) collision[c].solid = (activeCollision >> c & 1);
        }
        void write(Writer &writer)
        {
            writer.write((byte)(((byte)flipY & 0xF) << 4 | (behaviour & 0xF)));
            writer.write(floorAngle);
            writer.write(lWallAngle);
            writer.write(rWallAngle);
            writer.write(roofAngle);

            QByteArray collisionBytes;
            ushort collisionActive = 0;

            for (int i = 0; i < 8; ++i)
                collisionBytes.append((byte)(((byte)collision[i * 2].height & 0xF) << 4
                                             | (collision[(i * 2) + 1].height & 0xF)));

            for (int c = 0; c < 16; ++c)
                collisionActive ^= (-(bool)collision[c].solid ^ collisionActive) & (1 << c);

            writer.write(collisionBytes); // Write Collision Data

            writer.write((byte)((collisionActive >> 8) & 0xFF)); // Write Collision Solidity byte 1
            writer.write((byte)((collisionActive >> 0) & 0xFF)); // Write Collision Solidity byte 2
        }

        HeightMask collision[16];

        bool flipY      = false;
        byte behaviour  = 0;
        byte floorAngle = 0x00;
        byte rWallAngle = 0xC0;
        byte lWallAngle = 0x40;
        byte roofAngle  = 0x80;
    };

    TileConfig() {}
    TileConfig(QString filename) { read(filename); }
    TileConfig(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        filepath = reader.filepath;
        for (int c = 0; c < 0x400; ++c) {
            for (int p = 0; p < 2; ++p) collisionPaths[p][c].read(reader);
        }
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filepath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    inline void write(Writer &writer)
    {
        filepath = writer.filePath;

        for (int c = 0; c < 0x400; ++c) {
            for (int p = 0; p < 2; ++p) collisionPaths[p][c].write(writer);
        }
        writer.flush();
    }

    CollisionMask collisionPaths[2][0x400];

    QString filepath = "";
};

} // namespace RSDKv4

#endif // TILECONFIG_V4_H
