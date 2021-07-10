#include "include.hpp"

int blockLength    = 0;
int bitCache       = 0;
int bitCacheLength = 0;

struct CodeTableEntry {
    ushort m_length;
    ushort m_prefix;
    byte m_suffix;
};

uint readCode(Reader &reader, int codeSize)
{
    if (blockLength == 0)
        blockLength = reader.read<byte>();

    while (bitCacheLength <= codeSize && blockLength > 0) {
        uint bytedata = reader.read<byte>();
        (blockLength)--;
        bitCache |= (int)(bytedata << bitCacheLength);
        bitCacheLength += 8;

        if (blockLength == 0)
            blockLength = reader.read<byte>();
    }
    uint result = (uint)(bitCache & ((1 << codeSize) - 1));
    bitCache >>= codeSize;
    bitCacheLength -= codeSize;

    return result;
}

void RSDKv3::Video::VideoFrame::read(Reader &reader, RSDKv3::Video *vid)
{
    width  = vid->width;
    height = vid->height;
    m_imageData.resize(width * height);

    if (reader.read<byte>() != 0x3B)
        reader.seek(reader.tell() - 1);

    nextFramePos = (uint)(reader.read<byte>() + (reader.read<byte>() << 8) + (reader.read<byte>() << 16)
                       + (reader.read<byte>() << 24));

    m_framePalette.clear();
    for (int i = 0; i < 128; ++i) {
        byte r = reader.read<byte>(); // r
        byte g = reader.read<byte>(); // g
        byte b = reader.read<byte>(); // b
        m_framePalette.append(QColor(r, g, b));
    }

    // READ GIF DATA
    int eighthHeight  = height >> 3;
    int quarterHeight = height >> 2;
    int halfHeight    = height >> 1;
    int bitsWidth     = 0;
    int width         = 0;

    CodeTableEntry codeTable[0x10000];

    // Get frame
    byte type, subtype, temp;
    type = reader.read<byte>();

    while (type != 0) {
        bool tableFull = false, interlaced = false;
        int codeSize = 0, initCodeSize = 0;
        int clearCode = 0, eoiCode = 0, emptyCode = 0;
        int codeToAddFrom = 0, mark = 0, str_len = 0, frm_off = 0;
        int currentCode = 0;

        switch (type) {
            // Extension
            case 0x21:
                subtype = reader.read<byte>();
                switch (subtype) {
                    // Graphics Control Extension
                    case 0xF9:
                        reader.readByteArray(0x06);
                        // temp = reader.read<byte>();  // Block Size [byte] (always 0x04)
                        // temp = reader.read<byte>();  // Packed Field [byte] //
                        // temp16 = reader.read<short>(); // Delay Time [short] //
                        // temp = reader.read<byte>();  // Transparent Color Index? [byte] //
                        // temp = reader.read<byte>();  // Block Terminator [byte] //
                        break;
                    // Plain Text Extension
                    case 0x01:
                    // Comment Extension
                    case 0xFE:
                    // Application Extension
                    case 0xFF:
                        temp = reader.read<byte>(); // Block Size
                                                    // Continue until we run out of blocks
                        while (temp != 0) {
                            // Read block
                            reader.readByteArray(temp);
                            temp = reader.read<byte>(); // next block Size
                        }
                        break;
                    default: qDebug() << "GIF LOAD FAILED"; return;
                }
                break;
            // Image descriptor
            case 0x2C:
                // temp16 = reader.ReadUInt16(); // Destination X
                // temp16 = reader.ReadUInt16(); // Destination Y
                // temp16 = reader.ReadUInt16(); // Destination Width
                // temp16 = reader.ReadUInt16(); // Destination Height
                reader.readByteArray(8);
                temp = reader.read<byte>(); // Packed Field [byte]

                // If a local color table exists,
                if ((temp & 0x80) != 0) {
                    int size = 2 << (temp & 0x07);
                    // Load all colors
                    for (int i = 0; i < size; i++) {
                        byte r = reader.read<byte>();
                        byte g = reader.read<byte>();
                        byte b = reader.read<byte>();
                        m_framePalette.append(QColor(r, g, b));
                    }
                }

                interlaced = (temp & 0x40) == 0x40;
                if (interlaced) {
                    bitsWidth = 0;
                    while (width != 0) {
                        width >>= 1;
                        bitsWidth++;
                    }
                    width--;
                }

                codeSize = reader.read<byte>();

                clearCode = 1 << codeSize;
                eoiCode   = clearCode + 1;
                emptyCode = eoiCode + 1;

                codeSize++;
                initCodeSize = codeSize;

                // Init table
                for (int i = 0; i <= eoiCode; ++i) {
                    codeTable[i].m_length = 1;
                    codeTable[i].m_prefix = 0xFFF;
                    codeTable[i].m_suffix = (byte)i;
                }

                blockLength    = 0;
                bitCache       = 0b00000000;
                bitCacheLength = 0;
                tableFull      = false;

                currentCode = readCode(reader, codeSize);

                codeSize  = initCodeSize;
                emptyCode = eoiCode + 1;
                tableFull = false;

                CodeTableEntry entry;
                entry.m_suffix = 0;

                while (blockLength != 0) {
                    codeToAddFrom = -1;
                    mark          = 0;

                    if (currentCode == clearCode) {
                        codeSize  = initCodeSize;
                        emptyCode = eoiCode + 1;
                        tableFull = false;
                    }
                    else if (!tableFull) {
                        codeTable[emptyCode].m_length = (ushort)(str_len + 1);
                        codeTable[emptyCode].m_prefix = (ushort)currentCode;
                        codeTable[emptyCode].m_suffix = entry.m_suffix;
                        emptyCode++;

                        // Once we reach highest code, increase code size
                        if ((emptyCode & (emptyCode - 1)) == 0)
                            mark = 1;
                        else
                            mark = 0;

                        if (emptyCode >= 0x1000) {
                            mark      = 0;
                            tableFull = true;
                        }
                    }

                    currentCode = readCode(reader, codeSize);

                    if (currentCode == clearCode)
                        continue;
                    if (currentCode == eoiCode)
                        return;
                    if (mark == 1)
                        codeSize++;

                    entry   = codeTable[currentCode];
                    str_len = entry.m_length;

                    while (true) {
                        int p = frm_off + entry.m_length - 1;
                        if (interlaced) {
                            int row = p >> bitsWidth;
                            if (row < eighthHeight)
                                p = (p & width) + ((((row) << 3) + 0) << bitsWidth);
                            else if (row < quarterHeight)
                                p = (p & width) + ((((row - eighthHeight) << 3) + 4) << bitsWidth);
                            else if (row < halfHeight)
                                p = (p & width) + ((((row - quarterHeight) << 2) + 2) << bitsWidth);
                            else
                                p = (p & width) + ((((row - halfHeight) << 1) + 1) << bitsWidth);
                        }

                        m_imageData[p] = entry.m_suffix;
                        if (entry.m_prefix != 0xFFF)
                            entry = codeTable[entry.m_prefix];
                        else
                            break;
                    }
                    frm_off += str_len;
                    if (currentCode < emptyCode - 1 && !tableFull)
                        codeTable[emptyCode - 1].m_suffix = entry.m_suffix;
                }
                break;
        }

        type = reader.read<byte>();

        if (type == 0x3B)
            break;
    }
}

void RSDKv3::Video::VideoFrame::write(Writer &writer)
{
    writer.write((byte)0x3B);
    nextFramePos = writer.tell();

    for (int i = 0; i < 128; ++i) {
        writer.write((byte)m_framePalette[i].red());
        writer.write((byte)m_framePalette[i].green());
        writer.write((byte)m_framePalette[i].blue());
    }

    // write GIF data
}

QImage RSDKv3::Video::VideoFrame::toImage()
{
    QImage img(width, height, QImage::Format_Indexed8);
    m_imageData.resize(width * height);
    QVector<QRgb> colours;

    int id = 0;
    for (int y = 0; y < 0x10; ++y) {
        for (int x = 0; x < 0x10; ++x) {
            if (id >= m_framePalette.count()) {
                colours.append(qRgb(0xFF, 0x00, 0xFF));
            }
            else {
                QColor c = m_framePalette[id++];
                colours.append(qRgb(c.red(), c.green(), c.blue()));
            }
        }
    }
    img.setColorTable(colours);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            img.setPixel(x, y, m_imageData[(y * width) + x]);
        }
    }

    return img;
}

void RSDKv3::Video::VideoFrame::fromImage(QImage img)
{
    if (img.format() != QImage::Format_Indexed8)
        return;

    width  = img.width();
    height = img.height();
    m_imageData.resize(width * height);

    int size = img.colorCount();
    m_framePalette.clear();
    for (int c = 0; c < size; ++c)
        m_framePalette.append(QColor(qRed(img.color(c)), qGreen(img.color(c)), qBlue(img.color(c))));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            m_imageData[(y * width) + x] = img.pixelIndex(x, y);
        }
    }
}
