#include "include.hpp"

Reader::Reader(QString filepath) : m_file(new QFile(filepath)), m_stream(new QDataStream(m_file.data()))
{
    m_initialised = m_file->open(QIODevice::ReadOnly);

    m_filepath = filepath;
    m_filesize = m_file->size();
}

Reader::Reader(QDataStream *stream) : m_stream(stream)
{
    m_filepath    = "Memory";
    m_initialised = true;
}

QByteArray Reader::readZLib()
{
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
