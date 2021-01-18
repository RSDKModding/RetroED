#ifndef RSDK_PALETTE_H
#define RSDK_PALETTE_H

class Palette
{
public:
    Palette() {}
    Palette(Reader &reader, int clrCnt = -1) { read(reader, clrCnt); }

    void read(Reader &reader, int clrCnt = -1);
    void write(Writer &writer);

    QList<Colour> m_colours;
};

#endif // RSDK_PALETTE_H
