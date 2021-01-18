#ifndef PALETTE_V5_H
#define PALETTE_V5_H

namespace RSDKv5
{

class Palette
{
public:
    Palette() {}
    Palette(QString filename) { read(filename); }
    Palette(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    inline void read(Reader &reader)
    {
        m_filename        = reader.m_filepath;
        ushort activeRows = reader.read<ushort>();
        for (int r = 0; r < 0x10; ++r) m_activeRows[r] = Utils::getBit(activeRows, r);

        for (int r = 0; r < 0x10; ++r) {
            if (m_activeRows[r]) {
                for (int c = 0; c < 0x10; ++c) {
                    byte red        = reader.read<byte>();
                    byte green      = reader.read<byte>();
                    byte blue       = reader.read<byte>();
                    m_colours[r][c] = QColor(red, green, blue);
                }
            }
        }
    }

    inline void write(QString filename)
    {
        if (filename == "")
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    inline void write(Writer &writer)
    {
        m_filename        = writer.m_filename;
        ushort activeRows = 0x00;
        for (int r = 0; r < 0x10; ++r) Utils::setBit(activeRows, m_activeRows[r], r);
        writer.write(activeRows);

        for (int r = 0; r < 0x10; ++r) {
            if (m_activeRows[r]) {
                for (int c = 0; c < 0x10; ++c) {
                    writer.write((byte)m_colours[r][c].red());
                    writer.write((byte)m_colours[r][c].green());
                    writer.write((byte)m_colours[r][c].blue());
                }
            }
        }
    }

    QColor m_colours[0x10][0x10];
    bool m_activeRows[0x10];

    QString m_filename = "";
};

} // namespace RSDKv5

#endif // PALETTE_V5_H
