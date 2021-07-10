#ifndef TILECONFIG_VRS_H
#define TILECONFIG_VRS_H

namespace RSDKv1
{

class Tileconfig
{
public:
    class CollisionMask
    {
    public:
        enum CollisionSides {
            Floor,
            LWall,
            RWall,
            Roof,
            Max,
        };

        class HeightMask
        {
        public:
            HeightMask() {}

            byte height = 0;
            byte solid  = 0;
        };

        CollisionMask() {}
        CollisionMask(Reader &reader) { read(reader); }

        void read(Reader &reader, bool dcVer = false)
        {
            if (!dcVer) {
                byte buffer      = reader.read<byte>();
                collisionMode[0] = (byte)((buffer & 0xF0) >> 4);
                collisionMode[1] = (byte)(buffer & 0x0F);
            }

            for (int p = 0; p < 2; ++p) {
                for (int f = 0; f < CollisionSides::Max; ++f) {
                    for (int c = 0; c < 16; ++c) {
                        byte buffer                    = reader.read<byte>();
                        collisionPaths[p][f][c].height = (byte)((buffer & 0xF0) >> 4);
                        collisionPaths[p][f][c].solid  = (byte)(buffer & 0x0F);
                    }
                }
            }
        }
        void write(Writer &writer, bool dcVer = false)
        {
            if (!dcVer)
                writer.write(Utils::addNybbles(collisionMode[0], collisionMode[1]));

            for (int p = 0; p < 2; ++p) {
                for (int f = 0; f < CollisionSides::Max; ++f) {
                    for (int c = 0; c < 16; ++c) {
                        writer.write(Utils::addNybbles(collisionPaths[p][f][c].height,
                                                       collisionPaths[p][f][c].solid));
                    }
                }
            }
        }

        HeightMask collisionPaths[2][4][16];

        byte collisionMode[2];
    };

    Tileconfig() {}
    Tileconfig(QString filename) { read(filename); }
    Tileconfig(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader, bool dcVer = false)
    {
        filepath = reader.filepath;

        for (int c = 0; c < 0x400; ++c) collision[c].read(reader, dcVer);
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
    inline void write(Writer &writer, bool dcVer = false)
    {
        filepath = writer.filePath;

        for (int c = 0; c < 0x400; ++c) collision[c].write(writer, dcVer);

        writer.flush();
    }

    CollisionMask collision[0x400];

    QString filepath = "";
};

} // namespace RSDKv1

#endif // TILECONFIG_VRS_H
