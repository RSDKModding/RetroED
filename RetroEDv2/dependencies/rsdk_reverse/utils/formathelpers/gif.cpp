#include "rsdkreverse.hpp"

void FormatHelpers::Gif::read(Reader &reader, bool skipHeader, int clrCnt)
{
    filePath = reader.filePath;

    if (!skipHeader) {
        reader.seek(6); // GIF89a

        width = reader.read<byte>();
        width |= (ushort)(reader.read<byte>() << 8);

        height = reader.read<byte>();
        height |= (ushort)(reader.read<byte>() << 8);

        byte info = reader.read<byte>(); // Palette Size
        clrCnt    = (info & 0x7) + 1;
        if (clrCnt > 0)
            clrCnt = 1 << clrCnt;
        reader.read<byte>(); // background colour index
        reader.read<byte>(); // unused
    }

    for (int c = 0; c < clrCnt; ++c) {
        byte r     = reader.read<byte>();
        byte g     = reader.read<byte>();
        byte b     = reader.read<byte>();
        palette[c] = QColor(r, g, b, 0xFF);
    }

    byte blockType = reader.read<byte>();
    while (blockType != 0 && blockType != ';') {
        switch (blockType) {
            default: // Unknown
                // Unknown Block Type
                break;
            case '!': // Extension
            {
                byte extensionType = reader.read<byte>();
                switch (extensionType) {
                    case 0xF9: // Graphics Control Extension
                    {
                        int blockSize         = reader.read<byte>();
                        byte disposalFlag     = reader.read<byte>();
                        ushort frameDelay     = reader.read<ushort>();
                        byte transparentIndex = reader.read<byte>();
                        Q_UNUSED(blockSize);
                        Q_UNUSED(disposalFlag);
                        Q_UNUSED(frameDelay);
                        Q_UNUSED(transparentIndex);

                        reader.read<byte>(); // terminator
                        break;
                    }
                    case 0x01: // Plain Text Extension
                    case 0xFE: // Comment Extension
                    case 0xFF: // Application Extension
                    {
                        int blockSize = reader.read<byte>();
                        while (blockSize != 0) {
                            // Read block
                            reader.seek(reader.tell() + blockSize);
                            blockSize = reader.read<byte>(); // next block Size, if its 0 we know its
                                                             // the end of block
                        }
                        break;
                    }
                    default: // Unknown
                        // Unknown Extension Type
                        return;
                }
                break;
            }
            case ',': // Image descriptor
            {
                int left   = reader.read<ushort>();
                int top    = reader.read<ushort>();
                int right  = reader.read<ushort>();
                int bottom = reader.read<ushort>();
                Q_UNUSED(left);
                Q_UNUSED(top);
                Q_UNUSED(right);
                Q_UNUSED(bottom);

                byte info2      = reader.read<byte>();
                bool interlaced = (info2 & 0x40) != 0;
                if (info2 >> 7 == 1) {
                    for (int c = 0x80; c < 0x100; ++c) {
                        byte r     = reader.read<byte>();
                        byte g     = reader.read<byte>();
                        byte b     = reader.read<byte>();
                        palette[c] = QColor(r, g, b, 0xFF);
                    }
                }

                readPictureData(width, height, interlaced, reader);
                break;
            }
        }

        blockType = reader.read<byte>();
    }
}
void FormatHelpers::Gif::write(Writer &writer, bool skipHeader, bool useLocal)
{
    filePath = writer.filePath;

    // [GIF HEADER]
    if (!skipHeader) {
        byte fileType[] = { 'G', 'I', 'F' };
        byte fileVer[]  = { '8', '9', 'a' };
        writer.write(fileType, 3); // File type
        writer.write(fileVer, 3);  // File Version

        writer.write(width);
        writer.write(height);

        if (useLocal)
            writer.write<byte>((1 << 7) | (6 << 4)
                               | 6); // 1 == hasColours, 6 == paletteSize of 128, 6 == 7bpp
        else
            writer.write<byte>((1 << 7) | (7 << 4)
                               | 7); // 1 == hasColours, 7 == paletteSize of 256, 7 == 8bpp
        writer.write<byte>(0);
        writer.write<byte>(0);
    }

    // [GLOBAL PALETTE]
    for (int c = 0; c < (useLocal ? 0x80 : 0x100); ++c) {
        writer.write<byte>(palette[c].red());
        writer.write<byte>(palette[c].green());
        writer.write<byte>(palette[c].blue());
    }

    // [EXTENSION BLOCKS]

    // [IMAGE DESCRIPTOR HEADER]
    writer.write<byte>(',');

    writer.write<ushort>(0);
    writer.write<ushort>(0);
    writer.write<ushort>(width);
    writer.write<ushort>(height);
    if (useLocal)
        writer.write<byte>((1 << 7) | (0 << 6) | 6); // 1 == useLocal, 0 == no interlacing, 6 == 7bpp
    else
        writer.write<byte>((0 << 7) | (0 << 6)
                           | 0); // 0 == noLocal, 0 == no interlacing, no local palette, so we dont care

    // [LOCAL PALETTE]
    if (useLocal) {
        for (int c = 0x80; c < 0x100; ++c) {
            writer.write<byte>(palette[c].red());
            writer.write<byte>(palette[c].green());
            writer.write<byte>(palette[c].blue());
        }
    }

    // [IMAGE DATA]
    writePictureData(width, height, false, useLocal ? (byte)7 : (byte)8, writer);

    // [BLOCK END MARKER]
    writer.write<byte>(';'); // ';' used for image descriptor, 0 would be used for other blocks

    writer.flush();
}

void FormatHelpers::Gif::fromImage(QImage img)
{
    width  = img.width();
    height = img.height();
    pixels.resize(width * height);

    // pixel
}
QImage FormatHelpers::Gif::toImage()
{
    QImage img(width, height, QImage::Format_Indexed8);

    // mm pixel

    return img;
}

// GIF READING
void FormatHelpers::Gif::initDecoder(Reader &reader)
{
    byte initCodeSize      = reader.read<byte>();
    decoder.fileState      = PARSING_IMAGE;
    decoder.position       = 0;
    decoder.bufferSize     = 0;
    decoder.buffer[0]      = 0;
    decoder.depth          = initCodeSize;
    decoder.clearCode      = 1 << initCodeSize;
    decoder.eofCode        = decoder.clearCode + 1;
    decoder.runningCode    = decoder.eofCode + 1;
    decoder.runningBits    = initCodeSize + 1;
    decoder.maxCodePlusOne = 1 << decoder.runningBits;
    decoder.stackPtr       = 0;
    decoder.prevCode       = NO_SUCH_CODE;
    decoder.shiftState     = 0;
    decoder.shiftData      = 0;
    for (int i = 0; i <= LZ_MAX_CODE; ++i) decoder.prefix[i] = NO_SUCH_CODE;
}
void FormatHelpers::Gif::readLine(Reader &reader, int length, int offset)
{
    int i         = 0;
    int stackPtr  = decoder.stackPtr;
    int eofCode   = decoder.eofCode;
    int clearCode = decoder.clearCode;
    int prevCode  = decoder.prevCode;
    if (stackPtr != 0) {
        while (stackPtr != 0) {
            if (i >= length)
                break;

            pixels[offset++] = decoder.stack[--stackPtr];
            i++;
        }
    }

    while (i < length) {
        int gifCode = readCode(reader);
        if (gifCode == eofCode) {
            if (i != length - 1 || decoder.pixelCount != 0)
                return;

            i++;
        }
        else {
            if (gifCode == clearCode) {
                for (int p = 0; p <= LZ_MAX_CODE; p++) decoder.prefix[p] = NO_SUCH_CODE;

                decoder.runningCode    = decoder.eofCode + 1;
                decoder.runningBits    = decoder.depth + 1;
                decoder.maxCodePlusOne = 1 << decoder.runningBits;
                prevCode = decoder.prevCode = NO_SUCH_CODE;
            }
            else {
                if (gifCode < clearCode) {
                    pixels[offset] = (byte)gifCode;
                    offset++;
                    i++;
                }
                else {
                    if (gifCode < 0 || gifCode > LZ_MAX_CODE)
                        return;

                    int code;
                    if (decoder.prefix[gifCode] == NO_SUCH_CODE) {
                        if (gifCode != decoder.runningCode - 2)
                            return;

                        code                                    = prevCode;
                        decoder.suffix[decoder.runningCode - 2] = decoder.stack[stackPtr++] =
                            tracePrefix(prevCode, clearCode);
                    }
                    else {
                        code = gifCode;
                    }
                    int c = 0;
                    while (c++ <= LZ_MAX_CODE && code > clearCode && code <= LZ_MAX_CODE) {
                        decoder.stack[stackPtr++] = decoder.suffix[code];
                        code                      = (int)decoder.prefix[code];
                    }
                    if (c >= LZ_MAX_CODE | code > LZ_MAX_CODE)
                        return;

                    decoder.stack[stackPtr++] = (byte)code;
                    while (stackPtr != 0 && i++ < length) pixels[offset++] = decoder.stack[--stackPtr];
                }

                if (prevCode != NO_SUCH_CODE) {
                    if (decoder.runningCode < 2 || decoder.runningCode > FIRST_CODE)
                        return;

                    decoder.prefix[decoder.runningCode - 2] = (uint)prevCode;
                    if (gifCode == decoder.runningCode - 2)
                        decoder.suffix[decoder.runningCode - 2] = tracePrefix(prevCode, clearCode);
                    else
                        decoder.suffix[decoder.runningCode - 2] = tracePrefix(gifCode, clearCode);
                }
                prevCode = gifCode;
            }
        }
    }
    decoder.prevCode = prevCode;
    decoder.stackPtr = stackPtr;
}
int FormatHelpers::Gif::readCode(Reader &reader)
{
    while (decoder.shiftState < decoder.runningBits) {
        byte b = readByte(reader);
        decoder.shiftData |= (uint)b << decoder.shiftState;
        decoder.shiftState += 8;
    }
    int result = (int)(decoder.shiftData & (uint)codeMasks[decoder.runningBits]);
    decoder.shiftData >>= decoder.runningBits;
    decoder.shiftState -= decoder.runningBits;
    if (++decoder.runningCode > decoder.maxCodePlusOne && decoder.runningBits < LZ_BITS) {
        decoder.maxCodePlusOne <<= 1;
        decoder.runningBits++;
    }
    return result;
}
byte FormatHelpers::Gif::readByte(Reader &reader)
{
    byte c = 0;
    if (decoder.fileState == PARSE_COMPLETE)
        return c;

    byte b;
    if (decoder.position == decoder.bufferSize) {
        b                  = reader.read<byte>();
        decoder.bufferSize = (int)b;
        if (decoder.bufferSize == 0) {
            decoder.fileState = PARSE_COMPLETE;
            return c;
        }
        reader.readBytes(decoder.buffer, decoder.bufferSize);
        b                = decoder.buffer[0];
        decoder.position = 1;
    }
    else {
        b = decoder.buffer[decoder.position++];
    }
    return b;
}
byte FormatHelpers::Gif::tracePrefix(int code, int clearCode)
{
    int i = 0;
    while (code > clearCode && i++ <= LZ_MAX_CODE) code = (int)decoder.prefix[code];

    return (byte)code;
}
void FormatHelpers::Gif::readPictureData(int width, int height, bool interlaced, Reader &reader)
{
    pixels.resize(width * height);

    initDecoder(reader);
    if (interlaced) {
        for (int p = 0; p < 4; ++p) {
            for (int y = initialRows[p]; y < height; y += rowInc[p]) readLine(reader, width, y * width);
        }
    }
    else {
        for (int h = 0; h < height; ++h) readLine(reader, width, h * width);
    }
}

// GIF WRITING
void FormatHelpers::Gif::insertHashTable(uint key, int code)
{
    uint hKey = ((key >> 12) ^ key) & HT_KEY_MASK;

    while ((encoder.hashTable[hKey] >> 12) != 0xFFFFF) hKey = (hKey + 1) & HT_KEY_MASK;

    encoder.hashTable[hKey] = (uint)((key << 12) | (code & 0x0FFF));
}
int FormatHelpers::Gif::existsHashTable(uint key)
{
    uint hKey     = ((key >> 12) ^ key) & HT_KEY_MASK;
    uint tableKey = 0;

    while ((tableKey = encoder.hashTable[hKey] >> 12) != 0xFFFFF) {
        if (key == tableKey)
            return (int)(encoder.hashTable[hKey] & 0x0FFF);
        hKey = (hKey + 1) & HT_KEY_MASK;
    }

    return -1;
}

void FormatHelpers::Gif::initEncoder(Writer &writer, byte bitsPerPixel)
{
    byte initCodeSize = bitsPerPixel < 2 ? (byte)2 : bitsPerPixel;
    writer.write<byte>(initCodeSize);

    encoder.fileState      = PARSING_IMAGE;
    encoder.bufferSize     = 0;
    encoder.buffer[0]      = 0;
    encoder.depth          = initCodeSize;
    encoder.clearCode      = 1 << initCodeSize;
    encoder.eofCode        = encoder.clearCode + 1;
    encoder.runningCode    = encoder.eofCode + 1;
    encoder.runningBits    = initCodeSize + 1;
    encoder.maxCodePlusOne = 1 << encoder.runningBits;
    encoder.currentCode    = FIRST_CODE;
    encoder.shiftState     = 0;
    encoder.shiftData      = 0;
    for (int i = 0; i < HT_SIZE; ++i) encoder.hashTable[i] = 0xFFFFFFFF;

    writeCode(writer, encoder.clearCode);
}
void FormatHelpers::Gif::writeByte(Writer &writer, int b, bool flush)
{
    if (flush) {
        // write everything we've got
        writer.write<byte>(encoder.bufferSize);
        writer.write(encoder.buffer, encoder.bufferSize);
        encoder.bufferSize = 0;
    }
    else {
        if (encoder.bufferSize == 0xFF) {
            // buffer is full, write it to file
            writer.write<byte>(encoder.bufferSize);
            writer.write(encoder.buffer, encoder.bufferSize);
            encoder.bufferSize = 0;
        }
        encoder.buffer[encoder.bufferSize++] = (byte)b;
    }
}
void FormatHelpers::Gif::writeCode(Writer &writer, int code, bool flush)
{
    if (flush) {
        // write remaining data
        while (encoder.shiftState > 0) {
            writeByte(writer, (int)(encoder.shiftData & 0xFF));
            encoder.shiftData >>= 8;
            encoder.shiftState -= 8;
        }
        // clear & reset
        encoder.shiftState = 0;
        writeByte(writer, 0, true);
    }
    else {
        encoder.shiftData |= ((uint)code) << encoder.shiftState;
        encoder.shiftState += encoder.runningBits;

        // write any full bytes we have
        while (encoder.shiftState >= 8) {
            writeByte(writer, (int)(encoder.shiftData & 0xFF));
            encoder.shiftData >>= 8;
            encoder.shiftState -= 8;
        }
    }

    // add more bits for the code if needed
    if (encoder.runningCode >= encoder.maxCodePlusOne && code <= LZ_MAX_CODE)
        encoder.maxCodePlusOne = 1 << ++encoder.runningBits;
}
void FormatHelpers::Gif::writeLine(Writer &writer, byte *line, int length)
{
    int mask = codeMasks[encoder.depth];

    int pxPos = 0, curCode = 0;
    if (encoder.currentCode == FIRST_CODE)
        curCode = line[pxPos++] & mask;
    else
        curCode = encoder.currentCode;

    while (pxPos < length) {
        byte pixel = line[pxPos++] & mask;

        // create a key based on our code & the next pixel
        int newCode = 0;
        uint newKey = (((uint)curCode) << 8) + pixel;
        if ((newCode = existsHashTable(newKey)) >= 0) {
            curCode = newCode;
        }
        else {
            writeCode(writer, curCode);
            curCode = pixel;

            // handle clear codes if the hash table is full
            if (encoder.runningCode >= LZ_MAX_CODE) {
                writeCode(writer, encoder.clearCode);
                encoder.runningCode    = encoder.eofCode + 1;
                encoder.runningBits    = encoder.depth + 1;
                encoder.maxCodePlusOne = 1 << encoder.runningBits;

                // clear hash table
                for (int h = 0; h < HT_SIZE; ++h) encoder.hashTable[h] = 0xFFFFFFFF;
            }
            else {
                // add this to the hash table
                insertHashTable(newKey, encoder.runningCode++);
            }
        }
    }

    // keep this, it'll be needed later or at the end
    encoder.currentCode = curCode;
}
void FormatHelpers::Gif::writePictureData(int width, int height, bool interlaced, byte bitsPerPixel,
                                          Writer &writer)
{
    initEncoder(writer, bitsPerPixel);

    byte *px = reinterpret_cast<byte *>(pixels.data());
    if (interlaced) {
        for (int p = 0; p < 4; ++p) {
            for (int y = initialRows[p]; y < height; y += rowInc[p])
                writeLine(writer, &px[y * width], width);
        }
    }
    else {
        for (int y = 0; y < height; ++y) writeLine(writer, &px[y * width], width);
    }

    // write extra data
    writeCode(writer, encoder.currentCode);
    writeCode(writer, encoder.eofCode);
    writeCode(writer, 0, true);

    writer.write<byte>(0); // block terminator

    encoder.fileState = PARSE_COMPLETE;
}
