#ifndef DATAFILE_V1_H
#define DATAFILE_V1_H

namespace RSDKv2
{

class Datafile
{
public:
    class DirInfo
    {
    public:
        DirInfo() {}
        DirInfo(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        QString directory = "dir/";
        int m_address       = 0;
    };

    class FileInfo
    {
    public:
        FileInfo() {}
        FileInfo(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        QString fileName     = "File.ext";
        QString fullFileName = "Folder/File.ext";
        uint fileSize        = 0;
        QByteArray fileData;
        ushort m_dirID = 0;
    };

    Datafile() {}
    Datafile(QString filename) { read(filename); }
    Datafile(Reader &reader) { read(reader); }

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

    QList<DirInfo> directories;
    QList<FileInfo> files;

    QString m_filename = "";
};

} // namespace RSDKv2

#endif // DATAFILE_V1_H
