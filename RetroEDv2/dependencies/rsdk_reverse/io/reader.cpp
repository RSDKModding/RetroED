#include "include.hpp"

Reader::Reader(QString filepath) : file(new QFile(filepath)), stream(new QDataStream(file.data()))
{
    initialised = file->open(QIODevice::ReadOnly);

    this->filepath = filepath;
    this->filesize = file->size();
}

Reader::Reader(QDataStream *stream) : stream(stream)
{
    filepath    = "Memory";
    initialised = true;
}

QByteArray Reader::readZLib(bool raw)
{
    if (raw) {
        uint decompressed = filesize;

        QByteArray compressed = readByteArray(filesize);
        QByteArray intBytes;
        for (short i = 3; i > -1; i--) intBytes.append((byte)((decompressed >> (i * 8)) & 0xFF));

        compressed.insert(0, intBytes);
        return qUncompress(compressed);
    }
    else {
        uint complen      = read<int>();
        uint decompLE     = read<int>();
        uint decompressed = (uint)((decompLE << 24) | ((decompLE << 8) & 0x00FF0000)
                                   | ((decompLE >> 8) & 0x0000FF00) | (decompLE >> 24));

        QByteArray compressed = readByteArray(complen - 4);
        QByteArray intBytes;
        for (short i = 3; i > -1; i--) intBytes.append((byte)((decompressed >> (i * 8)) & 0xFF));

        compressed.insert(0, intBytes);
        return qUncompress(compressed);
    }
}
