#ifndef RSDK_COLOUR_H
#define RSDK_COLOUR_H

class Colour
{
public:
    Colour() {}
    Colour(byte r, byte g, byte b) : m_r(r), m_g(g), m_b(b) {}
    Colour(Reader &reader) { read(reader); }

    void read(Reader &reader)
    {
        m_r = reader.read<byte>();
        m_g = reader.read<byte>();
        m_b = reader.read<byte>();
    }

    void write(Writer &writer)
    {
        writer.write(m_r);
        writer.write(m_g);
        writer.write(m_b);
    }

    byte m_r = 0xFF;
    byte m_g = 0x00;
    byte m_b = 0xFF;
};

#endif // RSDK_COLOUR_H
