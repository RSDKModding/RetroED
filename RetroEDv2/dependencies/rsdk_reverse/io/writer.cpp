#include "include.hpp"

Writer::Writer(QString filename)
    : m_file(new QSaveFile(filename)), m_stream(new QDataStream(m_file.data()))
{
    filename    = filename;
    m_initialised = m_file->open(QIODevice::WriteOnly);
}

Writer::Writer(QDataStream *customDataStream) : m_stream(customDataStream)
{
    m_initialised = true;
    filePath    = "Memory";
}

Writer::Writer(QByteArray *byteArray, QIODevice::OpenModeFlag mode)
{
    filePath      = "Memory";
    QBuffer *buffer = new QBuffer(byteArray);
    m_initialised   = buffer->open(mode);
    m_stream.reset(new QDataStream(buffer));
}
