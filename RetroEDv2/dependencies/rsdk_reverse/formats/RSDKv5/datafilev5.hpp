#ifndef DATAFILE_V5_H
#define DATAFILE_V5_H

namespace RSDKv5
{

class Datafile
{
public:
    class FileInfo
    {
    public:
        FileInfo() {}
        FileInfo(Reader &reader, QList<QString> fileList, int cnt = 0) { read(reader, fileList, cnt); }

        void read(Reader &reader, QList<QString> fileList, int cnt = 0);
        void writeHeader(Writer &writer);
        void writeData(Writer &writer);

        inline void setName(QString filename)
        {
            fileName       = filename;
            filenameHash   = Utils::getMd5HashString(fileName);
            QByteArray md5 = Utils::getMd5HashByteArray(fileName);
            for (int h = 0; h < 0x10; ++h) hash[h] = (byte)md5[h];
        }

        QString fileName     = "";
        QString filenameHash = "";

        uint fileOffset = 0;
        uint fileSize   = 0;
        bool encrypted  = false;
        QByteArray fileData;

        byte hash[0x10];

    private:
        inline QString getMd5Hash(QString input)
        {
            return QCryptographicHash::hash(input.toLatin1(), QCryptographicHash::Md5).toHex();
        }

        inline QByteArray calculateMD5Hash(QString input)
        {
            return QCryptographicHash::hash(input.toLatin1(), QCryptographicHash::Md5);
        }

        void generateELoadKeys(QString filename, uint size);
        QByteArray decrypt(QByteArray data, bool encrypting);

        QByteArray encryptionKeyA;
        QByteArray encryptionKeyB;
        int eKeyNo;
        int eKeyPosA;
        int eKeyPosB;
        int eNybbleSwap;
    };

    Datafile() {}
    Datafile(QString filename, QList<QString> fileList) { read(filename, fileList); }
    Datafile(Reader &reader, QList<QString> fileList) { read(reader, fileList); }

    inline void read(QString filename, QList<QString> fileList)
    {
        Reader reader(filename);
        read(reader, fileList);
    }
    void read(Reader &reader, QList<QString> fileList);

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

    const byte signature[6] = { 'R', 'S', 'D', 'K', 'v', '5' };

    QList<FileInfo> files;

    QString filePath = "";
};

} // namespace RSDKv5

#endif // DATAFILE_V5_H
