#include "include.hpp"

void RSDKv3::Background::read(Reader &reader)
{
    m_filename = reader.m_filepath;

    byte layerCount = reader.read<byte>();

    byte hLineCount = reader.read<byte>();
    for (int h = 0; h < hLineCount; ++h) {
        m_hScroll.append(ScrollInfo(reader));
    }

    byte vLineCount = reader.read<byte>();
    for (int v = 0; v < vLineCount; ++v) {
        m_vScroll.append(ScrollInfo(reader));
    }

    for (int l = 0; l < layerCount; ++l) {
        m_layers.append(Layer(reader));
    }
}

void RSDKv3::Background::write(Writer &writer)
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

void RSDKv3::Background::Layer::read(Reader &reader)
{
    m_width         = reader.read<byte>();
    m_height        = reader.read<byte>();
    m_behaviour     = reader.read<byte>();
    m_relativeSpeed = (short)(reader.read<byte>() << 8);
    m_relativeSpeed |= (short)reader.read<byte>();
    m_constantSpeed = reader.read<byte>();

    m_lineIndexes.clear();
    m_lineIndexes.reserve(m_height * 128);

    byte buf[3];
    while (true) {
        buf[0] = reader.read<byte>();
        if (buf[0] == 0xFF) {
            buf[1] = reader.read<byte>();
            if (buf[1] == 0xFF) {
                break;
            }
            else {
                buf[2]  = reader.read<byte>();
                int val = buf[1];
                int cnt = buf[2] - 1;
                for (int c = 0; c < cnt; ++c) m_lineIndexes.append(val);
            }
        }
        else {
            m_lineIndexes.append(buf[0]);
        }
    }

    m_layout.reserve(m_height);
    for (int y = 0; y < m_height; ++y) {
        m_layout.append(QList<ushort>());
        m_layout[y].reserve(m_width);
        for (int x = 0; x < m_width; ++x) {
            byte b0 = 0, b1 = 0;
            b0 = reader.read<byte>();
            b1 = reader.read<byte>();
            m_layout[y].append((ushort)(b1 + (b0 << 8)));
        }
    }
}

void rle_writev3(Writer writer, int value, int count)
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

void RSDKv3::Background::Layer::write(Writer &writer)
{
    writer.write(m_width);
    writer.write(m_height);
    writer.write(m_behaviour);
    writer.write((byte)(m_relativeSpeed >> 8));
    writer.write((byte)(m_relativeSpeed & 0xFF));
    writer.write(m_constantSpeed);

    // Output data
    int l   = 0;
    int cnt = 0;

    for (int x = 0; x < m_lineIndexes.count(); ++x) {
        if ((byte)m_lineIndexes[x] != l && x > 0) {
            rle_writev3(writer, l, cnt);
            cnt = 0;
        }
        l = m_lineIndexes[x];
        cnt++;
    }

    rle_writev3(writer, l, cnt);

    writer.write((byte)0xFF);
    writer.write((byte)0xFF);

    for (int h = 0; h < m_height; ++h) {
        for (int w = 0; w < m_width; ++w) {
            writer.write((byte)(m_layout[h][w] >> 8));
            writer.write((byte)(m_layout[h][w] & 0xFF));
        }
    }
}
