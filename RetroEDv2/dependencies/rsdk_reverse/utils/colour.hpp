#ifndef RSDK_COLOUR_H
#define RSDK_COLOUR_H

class Colour
{
public:
    Colour() {}
    Colour(byte r, byte g, byte b) : m_r(r), m_g(g), m_b(b) {}
    Colour(byte r, byte g, byte b, byte a) : m_r(r), m_g(g), m_b(b), m_a(a) {}
    Colour(Reader &reader, bool useAlpha = false) { read(reader, useAlpha); }

    void read(Reader &reader, bool useAlpha = false)
    {
        m_r = reader.read<byte>();
        m_g = reader.read<byte>();
        m_b = reader.read<byte>();

        if (useAlpha)
            m_a = reader.read<byte>();
    }

    void write(Writer &writer, bool useAlpha = false)
    {
        writer.write(m_r);
        writer.write(m_g);
        writer.write(m_b);

        if (useAlpha)
            writer.write(m_a);
    }

    byte m_r = 0xFF;
    byte m_g = 0x00;
    byte m_b = 0xFF;
    byte m_a = 0xFF;
};

#endif // RSDK_COLOUR_H
