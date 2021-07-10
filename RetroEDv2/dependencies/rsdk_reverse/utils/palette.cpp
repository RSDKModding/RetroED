#include "includes.hpp"

void Palette::read(Reader &reader, int clrCnt)
{
    if (clrCnt < 0)
        clrCnt = reader.filesize / 3;

    m_colours.clear();
    for (int c = 0; c < clrCnt; ++c) {
        m_colours.append(Colour(reader));
    }
}

void Palette::write(Writer &writer)
{
    for (Colour &c : m_colours) {
        c.write(writer);
    }
}
