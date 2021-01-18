#ifndef DATAFILE_V3_H
#define DATAFILE_V3_H

namespace RSDKv3
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

    private:
        int m_eKeyNo      = 0;
        int m_eNybbleSwap = 0;
        int m_eKeyPosB    = 0;
        int m_eKeyPosA    = 0;

        const char *m_decryptionKeyA = "4RaS9D7KaEbxcp2o5r6t";
        const char *m_encryptionKeyB = "3tRaUxLmEaSn";
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

} // namespace RSDKv3

#endif // DATAFILE_V3_H
