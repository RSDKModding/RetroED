#ifndef ARCCONTAINER_V3_H
#define ARCCONTAINER_V3_H

namespace RSDKv3
{

class ArcContainer
{
public:
    class FileInfo
    {
    public:
        FileInfo() {}

        QString m_name = "";
        int m_fileSize = 0;
        int m_offset   = 0;
        QByteArray m_fileData;
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
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    byte m_signature[4] = { 'A', 'R', 'C', 'L' };

    int m_arcKey = 0;
    QList<FileInfo> m_files;

    QString m_filename = "";
};

} // namespace RSDKv3

#endif // ARCCONTAINER_V3_H
