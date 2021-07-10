#ifndef RSDK_COLOUR_H
#define RSDK_COLOUR_H

class Colour
{
public:
    Colour() {}
    Colour(byte r, byte g, byte b) : r(r), g(g), b(b) {}
    Colour(Reader &reader) { read(reader); }

    void read(Reader &reader)
    {
        r = reader.read<byte>();
        g = reader.read<byte>();
        b = reader.read<byte>();
    }

    void write(Writer &writer)
    {
        writer.write(r);
        writer.write(g);
        writer.write(b);
    }

    byte r = 0xFF;
    byte g = 0x00;
    byte b = 0xFF;
};

#endif // RSDK_COLOUR_H
