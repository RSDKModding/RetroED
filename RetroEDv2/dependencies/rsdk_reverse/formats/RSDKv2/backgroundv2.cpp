#include "include.hpp"

void RSDKv2::Background::read(Reader &reader)
{
    m_filename = reader.filepath;

    byte layerCount = reader.read<byte>();

    byte hLineCount = reader.read<byte>();
    for (int h = 0; h < hLineCount; ++h) hScroll.append(ScrollInfo(reader));

    byte vLineCount = reader.read<byte>();
    for (int v = 0; v < vLineCount; ++v) vScroll.append(ScrollInfo(reader));

    for (int l = 0; l < layerCount; ++l) layers.append(Layer(reader));
}

void RSDKv2::Background::write(Writer &writer)
{
    m_filename = writer.filePath;

    writer.write((byte)layers.count());

    writer.write((byte)hScroll.count());
    for (int h = 0; h < hScroll.count(); ++h) hScroll[h].write(writer);

    writer.write((byte)vScroll.count());
    for (int v = 0; v < vScroll.count(); ++v) vScroll[v].write(writer);

    for (int l = 0; l < layers.count(); ++l) layers[l].write(writer);

    writer.flush();
}

void RSDKv2::Background::Layer::read(Reader &reader)
{
    width         = reader.read<byte>();
    height        = reader.read<byte>();
    type     = reader.read<byte>();
    parallaxFactor = reader.read<byte>();
    scrollSpeed = reader.read<byte>();

    lineIndexes.clear();
    lineIndexes.reserve(height * 128);

    byte buf[3];
    while (true) {
        while (true) {
            buf[0] = reader.read<byte>();
            if (buf[0] == 0xFF)
                break;
            lineIndexes.append(buf[0]);
        }

        buf[1] = reader.read<byte>();
        if (buf[1] == 0xFF)
            break;

        buf[2] = (byte)(reader.read<byte>() - 1);

        for (int l = 0; l < buf[2]; ++l) lineIndexes.append(buf[1]);
    }

    layout.reserve(height);
    for (int y = 0; y < height; ++y) {
        layout.append(QList<byte>());
        layout[y].reserve(width);
        for (int x = 0; x < width; ++x) {
            layout[y].append(reader.read<byte>());
        }
    }
}

void rle_writev2(Writer writer, int value, int count)
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

void RSDKv2::Background::Layer::write(Writer &writer)
{
    writer.write(width);
    writer.write(height);
    writer.write(type);
    writer.write(parallaxFactor);
    writer.write(scrollSpeed);

    // Output data
    int l   = 0;
    int cnt = 0;

    for (int x = 0; x < lineIndexes.count(); ++x) {
        if ((byte)lineIndexes[x] != l && x > 0) {
            rle_writev2(writer, l, cnt);
            cnt = 0;
        }
        l = lineIndexes[x];
        cnt++;
    }

    rle_writev2(writer, l, cnt);

    writer.write((byte)0xFF);
    writer.write((byte)0xFF);

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            writer.write((byte)layout[h][w]);
        }
    }
}
