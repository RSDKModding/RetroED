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

        QString m_directory = "dir/";
        int m_address       = 0;
    };

    class FileInfo
    {
    public:
        FileInfo() {}
        FileInfo(Reader &reader) { read(reader); }

        void read(Reader &reader);
        void write(Writer &writer);

        QString m_filename     = "File.ext";
        QString m_fullFilename = "Folder/File.ext";
        uint m_fileSize        = 0;
        QByteArray m_filedata;
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

    QList<DirInfo> m_directories;
    QList<FileInfo> m_files;

    QString m_filename = "";
};

} // namespace RSDKv1

#endif // DATAFILE_VRS_H
