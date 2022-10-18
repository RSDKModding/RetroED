#include "libRSDK.hpp"

#include "gfxv1.hpp"

void RSDKv1::GFX::read(Reader &reader, bool dcGFX)
{
    filePath = reader.filePath;

    if (dcGFX)
        reader.read<byte>();

    width = (ushort)(reader.read<byte>() << 8);
    width |= reader.read<byte>();

    height = (ushort)(reader.read<byte>() << 8);
    height |= reader.read<byte>();

    // Read & Process palette
    for (int i = 0; i < 255; ++i) {
        palette[i].read(reader);
    }

    // Read Image Data
    byte buf[3];

    pixels.clear();
    while (true) {
        buf[0] = reader.read<byte>();
        if (buf[0] == 255) {
            buf[1] = reader.read<byte>();
            if (buf[1] == 255)
                break;
            else {
                buf[2] = reader.read<byte>();

                // Repeat value needs to decreased by one to decode
                // the graphics from the Dreamcast demo
                if (dcGFX)
                    --buf[2];

                for (int l = 0; l < buf[2]; ++l) pixels.append(buf[1]);
            }
        }
        else
            pixels.append(buf[0]);
    }
}

void rle_writev1(Writer writer, int pixel, int count, bool dcGFX)
{
    if (count <= 2) {
        for (int y = 0; y < count; ++y) {
            writer.write((byte)pixel);
        }
    }
    else {
        while (count > 0) {
            writer.write((byte)0xFF);
            writer.write((byte)pixel);

            if (dcGFX) {
                writer.write((byte)((count > 253) ? 254 : (count + 1)));
                count -= 253;
            }
            else {
                writer.write((byte)((count > 254) ? 254 : count));
                count -= 254;
            }
        }
    }
}

void RSDKv1::GFX::write(Writer &writer, bool dcGFX)
{
    filePath = writer.filePath;

    if (dcGFX)
        writer.write((byte)0);

    // Output width and height
    writer.write((byte)(width >> 8));
    writer.write((byte)(width & 0xFF));

    writer.write((byte)(height >> 8));
    writer.write((byte)(height & 0xFF));

    // Output palette
    for (int x = 0; x < 0xFF; ++x) {
        palette[x].write(writer);
    }

    // Output data
    int p   = 0;
    int cnt = 0;

    for (int x = 0; x < height * width; ++x) {
        if ((byte)pixels[x] != p && x > 0) {
            rle_writev1(writer, p, cnt, dcGFX);
            cnt = 0;
        }
        p = (byte)pixels[x];
        ++cnt;
    }

    rle_writev1(writer, p, cnt, dcGFX);

    // End of GFX file
    writer.write((byte)0xFF);
    writer.write((byte)0xFF);

    writer.flush();
}

void RSDKv1::GFX::importImage(QImage image)
{
    if (image.format() != QImage::Format_Indexed8) {
        return;
    }

    int w = image.width();
    int h = image.height();
    // m_imageData.clear();
    pixels.resize(w * h);

    int size = image.colorCount();
    for (int c = 0; c < size; ++c) {
        palette[c] = Color(qRed(image.color(c)), qGreen(image.color(c)), qBlue(image.color(c)));
    }

    for (int c = size; c < 0xFF; ++c) {
        palette[c] = Color(0xFF, 0x00, 0xFF);
    }

    int i  = 0;
    width  = w;
    height = h;
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if (image.format() == QImage::Format_Indexed8) {
                pixels[i++] = (byte)image.pixelIndex(x, y);
            }
            else {
                pixels[i++] = 0;
            }
        }
    }
}

void RSDKv1::GFX::importImage(FormatHelpers::Gif image)
{

    int w = image.width;
    int h = image.height;
    pixels.resize(w * h);

    for (int c = 0; c < 0xFF; ++c)
        palette[c] = Color(image.palette[c].red(), image.palette[c].green(), image.palette[c].blue());

    int i  = 0;
    width  = w;
    height = h;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            pixels[i] = image.pixels[i];
            i++;
        }
    }
}

QImage RSDKv1::GFX::exportImage()
{
    QImage img(width, height, QImage::Format_Indexed8);

    QVector<QRgb> colors;

    for (int i = 0; i < 0xFF; ++i) {
        Color c = palette[i];
        colors.append(qRgb(c.r, c.g, c.b));
    }
    img.setColorTable(colors);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            byte index = (byte)pixels[(y * width) + x];
            if (index < img.colorCount())
                img.setPixel(x, y, index);
        }
    }

    return img;
}

FormatHelpers::Gif RSDKv1::GFX::exportGif()
{
    FormatHelpers::Gif img(width, height);

    for (int i = 0; i < 0xFF; ++i) {
        Color c        = palette[i];
        img.palette[i] = QColor(c.r, c.g, c.b);
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            byte index                  = (byte)pixels[(y * width) + x];
            img.pixels[(y * width) + x] = index;
        }
    }

    return img;
}
