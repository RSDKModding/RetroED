#ifndef DATAFILE_V4_H
#define DATAFILE_V4_H

namespace RSDKv4
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

        enum ExtensionTypes {
            UNKNOWN,
            OGG,
            WAV,
            R3D,
            PNG,
            GIF,
        };

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

        QByteArray generateKey(uint value)
        {
            QByteArray md5Buf;
            QByteArray buffer;

            QString strbuf = QString::number(value);

            md5Buf = calculateMD5Hash(strbuf);

            buffer.clear();
            buffer.resize(0x10);

            for (int y = 0; y < 16; y += 4) {
                // convert to Little Endian
                buffer[y + 3] = (byte)md5Buf[y + 0];
                buffer[y + 2] = (byte)md5Buf[y + 1];
                buffer[y + 1] = (byte)md5Buf[y + 2];
                buffer[y + 0] = (byte)md5Buf[y + 3];
            }
            return buffer;
        }

        inline void generateELoadKeys(uint arg1, uint arg2, uint size)
        {
            encryptionKeyA = generateKey(arg1);
            encryptionKeyB = generateKey(arg2);

            eKeyNo      = (int)(size / 4) & 0x7F;
            eKeyPosA    = 0;
            eKeyPosB    = 8;
            eNybbleSwap = 0;
        }

        QByteArray decrypt(QByteArray data, bool encrypting);

        inline uint mulUnsignedHigh(uint64_t arg1, uint64_t arg2)
        {
            return (uint)(((uint64_t)arg1 * arg2) >> 32);
        }

        byte getExtensionFromData();

        byte extension = ExtensionTypes::UNKNOWN;

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

    const byte signature[6] = { 'R', 'S', 'D', 'K', 'v', 'B' };

    QList<FileInfo> files;

    QString filePath = "";
};

} // namespace RSDKv4

#endif // DATAFILE_V4_H
