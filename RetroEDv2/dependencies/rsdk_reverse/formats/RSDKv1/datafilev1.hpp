#ifndef DATAFILE_VRS_H
#define DATAFILE_VRS_H

namespace RSDKv1
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
        int offset        = 0;
    };

    class FileInfo
    {
    public:
        FileInfo() {}
        FileInfo(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        QString filename     = "File.ext";
        QString fullFilename = "Folder/File.ext";
        uint fileSize        = 0;
        ushort dirID         = 0;
        QByteArray fileData;
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
            filename = filepath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QList<DirInfo> directories;
    QList<FileInfo> files;

    QString filepath = "";
};

} // namespace RSDKv1

#endif // DATAFILE_VRS_H
