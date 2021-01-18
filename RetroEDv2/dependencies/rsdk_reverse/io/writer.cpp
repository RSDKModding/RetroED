#include "include.hpp"

Writer::Writer(QString filename)
    : m_file(new QSaveFile(filename)), m_stream(new QDataStream(m_file.data()))
{
    m_filename    = filename;
    m_initialised = m_file->open(QIODevice::WriteOnly);
}

Writer::Writer(QDataStream *customDataStream) : m_stream(customDataStream)
{
    m_initialised = true;
    m_filename    = "Memory";
}

Writer::Writer(QByteArray *byteArray, QIODevice::OpenModeFlag mode)
{
    m_filename      = "Memory";
    QBuffer *buffer = new QBuffer(byteArray);
    m_initialised   = buffer->open(mode);
    m_stream.reset(new QDataStream(buffer));
}
