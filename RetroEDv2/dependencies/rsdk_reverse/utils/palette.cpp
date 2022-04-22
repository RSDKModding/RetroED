#include "includes.hpp"

void Palette::read(Reader &reader, int clrCnt)
{
    if (clrCnt < 0)
        clrCnt = reader.filesize / 3;

    colors.clear();
    for (int c = 0; c < clrCnt; ++c) colors.append(Color(reader));
}

void Palette::write(Writer &writer)
{
    for (Color &c : colors) c.write(writer);
}
