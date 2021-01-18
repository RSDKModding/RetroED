#include "include.hpp"

void RSDKv2::GFX::read(Reader &reader, bool dcGFX)
{
    m_filename = reader.m_filepath;

    if (dcGFX)
        reader.read<byte>();

    m_width = (ushort)(reader.read<byte>() << 8);
    m_width |= reader.read<byte>();

    m_height = (ushort)(reader.read<byte>() << 8);
    m_height |= reader.read<byte>();

    // Read & Process palette
    for (int i = 0; i < 255; ++i) {
        m_palette[i].read(reader);
    }

    // Read Image Data
    byte buf[3];

    m_pixelData.clear();
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

                for (int l = 0; l < buf[2]; ++l) m_pixelData.append(buf[1]);
            }
        }
        else
            m_pixelData.append(buf[0]);
    }
}

void rle_writeRSDKv2(Writer writer, int pixel, int count, bool dcGFX)
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

void RSDKv2::GFX::write(Writer &writer, bool dcGFX)
{
    m_filename = writer.m_filename;

    if (dcGFX)
        writer.write((byte)0);

    // Output width and height
    writer.write((byte)(m_width >> 8));
    writer.write((byte)(m_width & 0xFF));

    writer.write((byte)(m_height >> 8));
    writer.write((byte)(m_height & 0xFF));

    // Output palette
    for (int x = 0; x < 0xFF; ++x) {
        m_palette[x].write(writer);
    }

    // Output data
    int p   = 0;
    int cnt = 0;

    for (int x = 0; x < m_height * m_width; ++x) {
        if ((byte)m_pixelData[x] != p && x > 0) {
            rle_writeRSDKv2(writer, p, cnt, dcGFX);
            cnt = 0;
        }
        p = (byte)m_pixelData[x];
        ++cnt;
    }

    rle_writeRSDKv2(writer, p, cnt, dcGFX);

    // End of GFX file
    writer.write((byte)0xFF);
    writer.write((byte)0xFF);

    writer.flush();
}

void RSDKv2::GFX::importImage(QImage image)
{
    if (image.format() != QImage::Format_Indexed8) {
        return;
    }

    int width  = image.width();
    int height = image.height();
    // m_imageData.clear();
    m_pixelData.resize(width * height);

    int size = image.colorCount();
    for (int c = 0; c < size; ++c) {
        m_palette[c] = Colour(qRed(image.color(c)), qGreen(image.color(c)), qBlue(image.color(c)));
    }

    for (int c = size; c < 0xFF; ++c) {
        m_palette[c] = Colour(0xFF, 0x00, 0xFF);
    }

    int i    = 0;
    m_width  = width;
    m_height = height;
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if (image.format() == QImage::Format_Indexed8) {
                m_pixelData[i++] = (byte)image.pixelIndex(x, y);
            }
            else {
                m_pixelData[i++] = 0;
            }
        }
    }
}

QImage RSDKv2::GFX::exportImage()
{
    QImage img(m_width, m_height, QImage::Format_Indexed8);

    QVector<QRgb> colours;

    for (int i = 0; i < 0xFF; ++i) {
        Colour c = m_palette[i];
        colours.append(qRgb(c.m_r, c.m_g, c.m_b));
    }
    img.setColorTable(colours);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            byte index = (byte)m_pixelData[(y * m_width) + x];
            if (index < img.colorCount())
                img.setPixel(x, y, index);
        }
    }

    return img;
}
