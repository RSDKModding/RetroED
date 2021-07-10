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

        QImage getImage(QList<QImage> tiles);

        Tile tiles[8][8];
    };

    Chunks() {}
    Chunks(byte ver, QString filepath) { read(ver, filepath); }

    void read(byte ver, QString filename);
    void write(byte ver, QString filename);

    Chunk chunks[0x200];

    QString m_filename = "";
};

} // namespace FormatHelpers

#endif // RSDK_CHUNKS_H
