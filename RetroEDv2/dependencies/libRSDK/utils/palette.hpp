#pragma once

class Palette
{
public:
    Palette() {}
    Palette(Reader &reader, int clrCnt = -1) { read(reader, clrCnt); }

    void read(Reader &reader, int clrCnt = -1);
    void write(Writer &writer);

    QList<Color> colors;
};


