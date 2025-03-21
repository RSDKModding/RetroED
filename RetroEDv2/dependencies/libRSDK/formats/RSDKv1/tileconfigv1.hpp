#pragma once

namespace RSDKv1
{

class TileConfig
{
public:
    enum CollisionSides {
        Floor,
        LWall,
        RWall,
        Roof,
        Max,
    };

    class CollisionMask
    {
    public:
        class HeightMask
        {
        public:
            HeightMask() {}

            byte height = 0;
            byte solid  = 0;
        };

        CollisionMask() {}

        HeightMask collision[4][16];

        // 0 = 0
        // 1 = RWall
        // 2 = 2
        // 3 = LWall
        // 4 = 4
        byte collisionMode;
    };

    TileConfig() {}
    TileConfig(QString filename, bool isDCVer) { read(filename, isDCVer); }
    TileConfig(Reader &reader) { read(reader); }

    inline void read(QString filename, bool isDCVer)
    {
        Reader reader(filename);
        read(reader, isDCVer);
    }
    inline void read(Reader &reader, bool dcVer = false)
    {
        filePath = reader.filePath;

        for (int c = 0; c < 0x400; ++c) {
            if (!dcVer) {
                byte buffer                        = reader.read<byte>();
                collisionPaths[0][c].collisionMode = (byte)((buffer & 0xF0) >> 4);
                collisionPaths[1][c].collisionMode = (byte)(buffer & 0x0F);
            }

            for (int p = 0; p < 2; ++p) {
                for (int f = 0; f < CollisionSides::Max; ++f) {
                    for (int i = 0; i < 16; ++i) {
                        byte buffer                                 = reader.read<byte>();
                        collisionPaths[p][c].collision[f][i].height = (byte)((buffer & 0xF0) >> 4);
                        collisionPaths[p][c].collision[f][i].solid  = (byte)(buffer & 0x0F);
                    }
                }
            }
        }
    }

    inline void write(QString filename, bool isDCVer = false)
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer, isDCVer);
    }
    inline void write(Writer &writer, bool dcVer = false)
    {
        filePath = writer.filePath;

        for (int c = 0; c < 0x400; ++c) {
            if (!dcVer)
                writer.write(Utils::addNybbles(collisionPaths[0][c].collisionMode,
                                               collisionPaths[1][c].collisionMode));

            for (int p = 0; p < 2; ++p) {
                for (int f = 0; f < CollisionSides::Max; ++f) {
                    for (int i = 0; i < 16; ++i) {
                        writer.write(Utils::addNybbles(collisionPaths[p][c].collision[f][i].height,
                                                       collisionPaths[p][c].collision[f][i].solid));
                    }
                }
            }
        }

        writer.flush();
    }

    CollisionMask collisionPaths[2][0x400];

    QString filePath = "";
};

} // namespace RSDKv1


