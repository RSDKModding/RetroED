#include "includes.hpp"

void Palette::read(Reader &reader, int clrCnt)
{
    if (clrCnt < 0)
        clrCnt = reader.filesize / 3;

    colours.clear();
    for (int c = 0; c < clrCnt; ++c) {
        colours.append(Colour(reader));
    }
}

void Palette::write(Writer &writer)
{
    for (Colour &c : colours) {
        c.write(writer);
    }
}
