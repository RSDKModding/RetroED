#ifndef RSDK_CHUNKS_H
#define RSDK_CHUNKS_H

namespace FormatHelpers
{

class Chunks
{
public:
    class Tile
    {
    public:
        Tile() {}

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

        QImage getImage(QList<QImage> tiles);

        Tile m_tiles[8][8];
    };

    Chunks() {}
    Chunks(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    Chunk m_chunks[0x200];

    QString m_filename = "";
};

} // namespace FormatHelpers

#endif // RSDK_CHUNKS_H
