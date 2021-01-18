#ifndef TILECONFIG_V3_H
#define TILECONFIG_V3_H

namespace RSDKv3
{

class Tileconfig
{
public:
    class CollisionMask
    {
    public:
        class HeightMask
        {
        public:
            HeightMask() {}

            byte m_height = 0;
            bool m_solid  = false;
        };

        CollisionMask() {}
        CollisionMask(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            byte flags   = reader.read<byte>();
            m_isCeiling  = (flags >> 4) != 0;
            m_behaviour  = (byte)(flags & 0xF);
            m_floorAngle = reader.read<byte>();
            m_lWallAngle = reader.read<byte>();
            m_rWallAngle = reader.read<byte>();
            m_roofAngle  = reader.read<byte>();

            QByteArray collision = reader.readByteArray(8);

            ushort activeCollision = reader.read<byte>() << 8;
            activeCollision |= reader.read<byte>();

            int i  = 0;
            int i2 = 1;

            for (int c = 0; c < 8; ++c) {
                m_collision[i].m_height  = (byte)((collision[c] & 0xF0) >> 4);
                m_collision[i2].m_height = (byte)(collision[c] & 0x0F);
                i += 2;
                i2 += 2;
            }

            for (int c = 0; c < 16; ++c) m_collision[c].m_solid = (activeCollision >> c & 1);
        }
        void write(Writer &writer)
        {
            writer.write((byte)(((byte)m_isCeiling & 0xF) << 4 | (m_behaviour & 0xF)));
            writer.write(m_floorAngle);
            writer.write(m_lWallAngle);
            writer.write(m_rWallAngle);
            writer.write(m_roofAngle);

            QByteArray collision;
            ushort collisionActive = 0;

            for (int i = 0; i < 8; ++i)
                collision.append((byte)(((byte)m_collision[i * 2].m_height & 0xF) << 4
                                        | (m_collision[(i * 2) + 1].m_height & 0xF)));

            for (int c = 0; c < 16; ++c)
                collisionActive ^= (-(bool)m_collision[c].m_solid ^ collisionActive) & (1 << c);

            writer.write(collision); // Write Collision Data

            writer.write((byte)((collisionActive >> 8) & 0xFF)); // Write Collision Solidity byte 1
            writer.write((byte)((collisionActive >> 0) & 0xFF)); // Write Collision Solidity byte 2
        }

        HeightMask m_collision[16];

        bool m_isCeiling  = false;
        byte m_behaviour  = 0;
        byte m_floorAngle = 0x00;
        byte m_rWallAngle = 0xC0;
        byte m_lWallAngle = 0x40;
        byte m_roofAngle  = 0x80;
    };

    Tileconfig() {}
    Tileconfig(QString filename) { read(filename); }
    Tileconfig(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        m_filename = reader.m_filepath;
        for (int c = 0; c < 0x400; ++c) {
            for (int p = 0; p < 2; ++p) m_collisionPaths[p][c].read(reader);
        }
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
    inline void write(Writer &writer)
    {
        m_filename = writer.m_filename;

        for (int c = 0; c < 0x400; ++c) {
            for (int p = 0; p < 2; ++p) m_collisionPaths[p][c].write(writer);
        }
        writer.flush();
    }

    CollisionMask m_collisionPaths[2][0x400];

    QString m_filename = "";
};

} // namespace RSDKv3

#endif // TILECONFIG_V3_H
