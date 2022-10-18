#pragma once

namespace RSDKv5
{

class Palette
{
public:
    Palette()
    {
        // filePath = "";
        for (int r = 0; r < 0x10; ++r) {
            activeRows[r] = false;
            for (int c = 0; c < 0x10; ++c) colors[r][c] = QColor(0xFF, 0x00, 0xFF);
        }
    }
    Palette(QString filename) { read(filename); }
    Palette(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        // filePath        = reader.filePath;
        ushort activeRowBits = reader.read<ushort>();
        for (int r = 0; r < 0x10; ++r) activeRows[r] = Utils::getBit(activeRowBits, r);

        for (int r = 0; r < 0x10; ++r) {
            if (activeRows[r]) {
                for (int c = 0; c < 0x10; ++c) {
                    byte red     = reader.read<byte>();
                    byte green   = reader.read<byte>();
                    byte blue    = reader.read<byte>();
                    colors[r][c] = QColor(red, green, blue);
                }
            }
        }
    }

    inline void write(QString filename)
    {
        // if (filename == "")
        //    filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    inline void write(Writer &writer, bool internal = false)
    {
        // if (!internal)
        //    filePath = writer.filePath;
        ushort activeRowBits = 0x00;
        for (int r = 0; r < 0x10; ++r) Utils::setBit(activeRowBits, activeRows[r], r);
        writer.write(activeRowBits);

        for (int r = 0; r < 0x10; ++r) {
            if (activeRows[r]) {
                for (int c = 0; c < 0x10; ++c) {
                    writer.write((byte)colors[r][c].red());
                    writer.write((byte)colors[r][c].green());
                    writer.write((byte)colors[r][c].blue());
                }
            }
        }

        if (!internal)
            writer.flush();
    }

    QColor colors[0x10][0x10];
    bool activeRows[0x10];

    // QString filePath;
};

} // namespace RSDKv5


