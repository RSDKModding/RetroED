#ifndef RSDK_COLOR_H
#define RSDK_COLOR_H

class Color
{
public:
    Color() {}
    Color(byte r, byte g, byte b) : r(r), g(g), b(b) {}
    Color(Reader &reader) { read(reader); }

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

struct PaletteColor {
    PaletteColor() {}
    PaletteColor(byte r, byte g, byte b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    PaletteColor(QColor c)
    {
        this->r = c.red();
        this->g = c.green();
        this->b = c.blue();
    }

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

    inline QColor toQColor() { return QColor(r, g, b); }

    byte r = 0xFF;
    byte g = 0x00;
    byte b = 0xFF;
};

#endif // RSDK_COLOR_H
