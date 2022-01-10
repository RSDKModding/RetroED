#ifndef CHUNKS_V4_H
#define CHUNKS_V4_H

namespace RSDKv4
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

            visualPlane = (byte)(bytes[0] >> 4);
            bytes[0]    = (byte)((byte)bytes[0] - ((byte)bytes[0] >> 4 << 4));

            direction = (byte)((byte)bytes[0] >> 2);
            bytes[0]  = (byte)((byte)bytes[0] - ((byte)bytes[0] >> 2 << 2));

            tileIndex = (ushort)(((byte)bytes[0] << 8) + (byte)bytes[1]);

            solidityA = (byte)((byte)bytes[2] >> 4);
            solidityB = (byte)((byte)bytes[2] - ((byte)bytes[2] >> 4 << 4));
        }
        void write(Writer &writer)
        {
            byte bytes[3];
            bytes[0] = (byte)(tileIndex >> 8); // Put the first bit onto buffer[0]
            bytes[0] += (bytes[0] >> 2 << 2);
            bytes[0] |= (direction) << 2; // Put the Flip of the tile two bits in
            bytes[0] += (bytes[0] >> 4 << 4);
            bytes[0] |= (visualPlane) << 4; // Put the Layer of the tile four bits in
            bytes[0] += (bytes[0] >> 6 << 6);

            bytes[1] = (byte)tileIndex; // Put the rest of the Tile16x16 Value into this buffer

            bytes[2] = solidityB;       // Colision Flag 1 is all bytes before bit 5
            bytes[2] |= solidityA << 4; // Colision Flag 0 is all bytes after bit 4

            writer.write(bytes[0]);
            writer.write(bytes[1]);
            writer.write(bytes[2]);
        }

        byte visualPlane = 0;
        byte direction   = 0;
        ushort tileIndex = 0;
        byte solidityA   = 0;
        byte solidityB   = 0;
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
                    tiles[y][x].read(reader);
                }
            }
        }
        void write(Writer &writer)
        {
            for (int y = 0; y < 8; ++y) {
                for (int x = 0; x < 8; ++x) {
                    tiles[y][x].write(writer);
                }
            }
        }

        Tile tiles[8][8];
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
        filePath = reader.filePath;

        for (int c = 0; c < 0x200; ++c) chunkList[c].read(reader);
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
    inline void write(Writer &writer)
    {
        filePath = writer.filePath;

        for (int c = 0; c < 0x200; ++c) chunkList[c].write(writer);

        writer.flush();
    }

    Chunk chunkList[0x200];

    QString filePath = "";
};

} // namespace RSDKv4

#endif // CHUNKS_V4_H
