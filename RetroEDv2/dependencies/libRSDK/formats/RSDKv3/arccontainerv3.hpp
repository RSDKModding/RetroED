#pragma once

namespace RSDKv3
{

class ArcContainer
{
public:
    class FileInfo
    {
    public:
        FileInfo() {}

        QString fileName = "";
        int fileSize     = 0;
        int offset       = 0;
        QByteArray fileData;
    };
    ArcContainer() {}
    ArcContainer(QString filename) { read(filename); }
    ArcContainer(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

    inline void write(QString filename)
    {
        if (filename == "")
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    byte signature[4] = { 'A', 'R', 'C', 'L' };

    int key = 0;
    QList<FileInfo> files;

    QString filePath = "";
};

} // namespace RSDKv3


