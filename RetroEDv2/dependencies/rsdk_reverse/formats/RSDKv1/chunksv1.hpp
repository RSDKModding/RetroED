#ifndef CHUNKS_VRS_H
#define CHUNKS_VRS_H

namespace RSDKv1
{

class Chunks
{
public:
    class Tile
    {
    public:
        Tile() {}
        Tile(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            QByteArray bytes = reader.readByteArray(3);
            bytes[0]         = (byte)((byte)bytes[0] - ((byte)bytes[0] >> 6 << 6));

            m_visualPlane = (byte)(bytes[0] >> 4);
            bytes[0]      = (byte)((byte)bytes[0] - ((byte)bytes[0] >> 4 << 4));

            m_direction = (byte)((byte)bytes[0] >> 2);
            bytes[0]    = (byte)((byte)bytes[0] - ((byte)bytes[0] >> 2 << 2));

            m_tileIndex = (ushort)(((byte)bytes[0] << 8) + (byte)bytes[1]);

            m_solidityA = (byte)((byte)bytes[2] >> 4);
            m_solidityB = (byte)((byte)bytes[2] - ((byte)bytes[2] >> 4 << 4));
        }
        void write(Writer &writer)
        {
            byte bytes[3];
            bytes[0] = (byte)(m_tileIndex >> 8); // Put the first bit onto buffer[0]
            bytes[0] += (bytes[0] >> 2 << 2);
            bytes[0] |= (m_direction) << 2; // Put the Flip of the tile two bits in
            bytes[0] += (bytes[0] >> 4 << 4);
            bytes[0] |= (m_visualPlane) << 4; // Put the Layer of the tile four bits in
            bytes[0] += (bytes[0] >> 6 << 6);

            bytes[1] = (byte)m_tileIndex; // Put the rest of the Tile16x16 Value into this buffer

            bytes[2] = m_solidityB;       // Colision Flag 1 is all bytes before bit 5
            bytes[2] |= m_solidityA << 4; // Colision Flag 0 is all bytes after bit 4

            writer.write(bytes[0]);
            writer.write(bytes[1]);
            writer.write(bytes[2]);
        }

        byte m_visualPlane = 0;
        byte m_direction   = 0;
        ushort m_tileIndex = 0;
        byte m_solidityA   = 0;
        byte m_solidityB   = 0;
    };

    class Chunk
    {
    public:
        Chunk() {}
        Chunk(Reader &reader) { read(reader); }

        void read(Reader &reader)
        {
            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    m_tiles[y][x].read(reader);
                }
            }
        }
        void write(Writer &writer)
        {
            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    m_tiles[y][x].write(writer);
                }
            }
        }

        Tile m_tiles[8][8];
    };

    Chunks() {}
    Chunks(QString filename) { read(filename); }
    Chunks(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        m_filename = reader.m_filepath;

        for (int c = 0; c < 0x100; ++c) m_chunkList[c].read(reader);
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

        for (int c = 0; c < 0x100; ++c) m_chunkList[c].write(writer);

        writer.flush();
    }

    Chunk m_chunkList[0x100];

    QString m_filename = "";
};

} // namespace RSDKv1

#endif // CHUNKS_VRS_H
