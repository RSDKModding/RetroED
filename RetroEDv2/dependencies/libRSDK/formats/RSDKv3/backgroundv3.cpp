#include "libRSDK.hpp"

void RSDKv3::Background::read(Reader &reader)
{
    filePath = reader.filePath;

    byte layerCount = reader.read<byte>();

    byte hLineCount = reader.read<byte>();
    for (int h = 0; h < hLineCount; ++h) hScroll.append(ScrollInfo(reader));

    byte vLineCount = reader.read<byte>();
    for (int v = 0; v < vLineCount; ++v) vScroll.append(ScrollInfo(reader));

    for (int l = 0; l < layerCount; ++l) layers.append(Layer(reader));
}

void RSDKv3::Background::write(Writer &writer)
{
    filePath = writer.filePath;

    writer.write((byte)layers.count());

    writer.write((byte)hScroll.count());
    for (int h = 0; h < hScroll.count(); h++) hScroll[h].write(writer);

    writer.write((byte)vScroll.count());
    for (int v = 0; v < vScroll.count(); v++) vScroll[v].write(writer);

    for (int l = 0; l < layers.count(); ++l) layers[l].write(writer);

    writer.flush();
}

void RSDKv3::Background::Layer::read(Reader &reader)
{
    width          = reader.read<byte>();
    height         = reader.read<byte>();
    type           = reader.read<byte>();
    parallaxFactor = (short)(reader.read<byte>() << 8);
    parallaxFactor |= (short)reader.read<byte>();
    scrollSpeed = reader.read<byte>();

    lineScroll.clear();
    lineScroll.reserve(height * 128);

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
                for (int c = 0; c < cnt; ++c) lineScroll.append(val);
            }
        }
        else {
            lineScroll.append(buf[0]);
        }
    }

    layout.reserve(height);
    for (int y = 0; y < height; ++y) {
        layout.append(QList<ushort>());
        layout[y].reserve(width);
        for (int x = 0; x < width; ++x) {
            byte b0 = 0, b1 = 0;
            b0 = reader.read<byte>();
            b1 = reader.read<byte>();
            layout[y].append((ushort)(b1 + (b0 << 8)));
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
        }
    }
}

void RSDKv3::Background::Layer::write(Writer &writer)
{
    writer.write(width);
    writer.write(height);
    writer.write(type);
    writer.write((byte)(parallaxFactor >> 8));
    writer.write((byte)(parallaxFactor & 0xFF));
    writer.write(scrollSpeed);

    // Output data
    int l   = 0;
    int cnt = 0;

    for (int x = 0; x < lineScroll.count(); ++x) {
        if ((byte)lineScroll[x] != l && x > 0) {
            rle_writev3(writer, l, cnt);
            cnt = 0;
        }
        l = lineScroll[x];
        cnt++;
    }

    rle_writev3(writer, l, cnt);

    writer.write((byte)0xFF);
    writer.write((byte)0xFF);

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            writer.write((byte)(layout[h][w] >> 8));
            writer.write((byte)(layout[h][w] & 0xFF));
        }
    }
}
