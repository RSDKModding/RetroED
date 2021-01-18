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

            byte m_height = 0;
            byte m_solid  = 0;
        };

        CollisionMask() {}
        CollisionMask(Reader &reader) { read(reader); }

        void read(Reader &reader, bool dcVer = false)
        {
            if (!dcVer) {
                byte buffer = reader.read<byte>();
                m_angle     = (byte)((buffer & 0xF0) >> 4);
                m_behaviour = (byte)(buffer & 0x0F);
            }

            for (int p = 0; p < 2; ++p) {
                for (int f = 0; f < CollisionSides::Max; ++f) {
                    for (int c = 0; c < 16; ++c) {
                        byte buffer                        = reader.read<byte>();
                        m_collisionPaths[p][f][c].m_height = (byte)((buffer & 0xF0) >> 4);
                        m_collisionPaths[p][f][c].m_solid  = (byte)(buffer & 0x0F);
                    }
                }
            }
        }
        void write(Writer &writer, bool dcVer = false)
        {
            if (!dcVer)
                writer.write(Utils::addNybbles(m_angle, m_behaviour));

            for (int p = 0; p < 2; ++p) {
                for (int f = 0; f < CollisionSides::Max; ++f) {
                    for (int c = 0; c < 16; ++c) {
                        writer.write(Utils::addNybbles(m_collisionPaths[p][f][c].m_height,
                                                       m_collisionPaths[p][f][c].m_solid));
                    }
                }
            }
        }

        HeightMask m_collisionPaths[2][4][16];

        byte m_behaviour = 0;
        byte m_angle     = 0;
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
        m_filename = reader.m_filepath;

        for (int c = 0; c < 0x400; ++c) m_collision[c].read(reader, dcVer);
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
    inline void write(Writer &writer, bool dcVer = false)
    {
        m_filename = writer.m_filename;

        for (int c = 0; c < 0x400; ++c) m_collision[c].write(writer, dcVer);

        writer.flush();
    }

    CollisionMask m_collision[0x400];

    QString m_filename = "";
};

} // namespace RSDKv1

#endif // TILECONFIG_VRS_H
