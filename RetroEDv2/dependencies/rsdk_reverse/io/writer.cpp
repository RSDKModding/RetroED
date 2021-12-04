#include "include.hpp"

Writer::Writer(QString filename)
    : file(new QSaveFile(filename)), stream(new QDataStream(file.data()))
{
    filePath    = filename;
    initialised = file->open(QIODevice::WriteOnly);
}

Writer::Writer(QDataStream *customDataStream) : stream(customDataStream)
{
    initialised = true;
    filePath    = "Memory";
}

Writer::Writer(QByteArray *byteArray, QIODevice::OpenModeFlag mode)
{
    filePath        = "Memory";
    QBuffer *buffer = new QBuffer(byteArray);
    initialised     = buffer->open(mode);
    stream.reset(new QDataStream(buffer));
}
