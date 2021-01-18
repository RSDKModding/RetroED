#include "include.hpp"

void RSDKv4::Background::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    byte layerCount = reader.read<byte>();

    m_hScroll.clear();
    byte hLineCount = reader.read<byte>();
    for (int h = 0; h < hLineCount; ++h) {
        m_hScroll.append(ScrollInfo(reader));
    }

    m_vScroll.clear();
    byte vLineCount = reader.read<byte>();
    for (int v = 0; v < vLineCount; ++v) {
        m_vScroll.append(ScrollInfo(reader));
    }

    m_layers.clear();
    for (int l = 0; l < layerCount; ++l) {
        m_layers.append(Layer(reader));
    }
}

void RSDKv4::Background::write(Writer &writer)
{
    m_filename = writer.m_filename;

    writer.write((byte)m_layers.count());

    writer.write((byte)m_hScroll.count());
    for (int h = 0; h < m_hScroll.count(); h++) {
        m_hScroll[h].write(writer);
    }

    writer.write((byte)m_vScroll.count());
    for (int v = 0; v < m_vScroll.count(); v++) {
        m_vScroll[v].write(writer);
    }

    for (int l = 0; l < m_layers.count(); ++l) {
        m_layers[l].write(writer);
    }

    writer.flush();
}

void RSDKv4::Background::Layer::read(Reader &reader)
{
    m_width = (short)(reader.read<byte>() & 0xFF);
    m_width |= (short)(reader.read<byte>() << 8);
    m_height = (short)(reader.read<byte>() & 0xFF);
    m_height |= (short)(reader.read<byte>() << 8);
    m_behaviour     = reader.read<byte>();
    m_relativeSpeed = (short)(reader.read<byte>() & 0xFF);
    m_relativeSpeed |= (short)(reader.read<byte>() << 8);
    m_constantSpeed = reader.read<byte>();

    m_lineIndexes.clear();
    m_lineIndexes.reserve(m_height * 128);

    byte buf[3];
    while (true) {
        while (true) {
            buf[0] = reader.read<byte>();
            if (buf[0] == 0xFF)
                break;
            m_lineIndexes.append(buf[0]);
        }

        buf[1] = reader.read<byte>();
        if (buf[1] == 0xFF)
            break;

        buf[2] = (byte)(reader.read<byte>() - 1);

        for (int l = 0; l < buf[2]; ++l) m_lineIndexes.append(buf[1]);
    }

    m_layout.reserve(m_height);
    for (int y = 0; y < m_height; ++y) {
        m_layout.append(QList<ushort>());
        m_layout[y].reserve(m_width);
        for (int x = 0; x < m_width; ++x) {
            byte b0 = 0, b1 = 0;
            b0 = reader.read<byte>();
            b1 = reader.read<byte>();
            m_layout[y].append((ushort)(b0 + (b1 << 8)));
        }
    }
}

void rle_writev4(Writer writer, int value, int count)
{
    if (count <= 2) {
        for (int y = 0; y < count; ++y) {
            writer.write((byte)value);
        }
    }
    else {
        while (count > 0) {
            writer.write((byte)0xFF);
            writer.write((byte)value);
            writer.write((byte)((count > 254) ? 255 : (count + 1)));
            count -= 254;

            // Keep an eye on this, might be wrong
        }
    }
}

void RSDKv4::Background::Layer::write(Writer &writer)
{
    writer.write((byte)(m_width & 0xFF));
    writer.write((byte)(m_width >> 8));
    writer.write((byte)(m_height & 0xFF));
    writer.write((byte)(m_height >> 8));
    writer.write(m_behaviour);
    writer.write((byte)(m_relativeSpeed & 0xFF));
    writer.write((byte)(m_relativeSpeed >> 8));
    writer.write(m_constantSpeed);

    // Output data
    int l   = 0;
    int cnt = 0;

    for (int x = 0; x < m_lineIndexes.count(); ++x) {
        if ((byte)m_lineIndexes[x] != l && x > 0) {
            rle_writev4(writer, l, cnt);
            cnt = 0;
        }
        l = m_lineIndexes[x];
        cnt++;
    }

    rle_writev4(writer, l, cnt);

    writer.write((byte)0xFF);
    writer.write((byte)0xFF);

    for (int h = 0; h < m_height; ++h) {
        for (int w = 0; w < m_width; ++w) {
            writer.write((byte)(m_layout[h][w] & 0xFF));
            writer.write((byte)(m_layout[h][w] >> 8));
        }
    }
}
