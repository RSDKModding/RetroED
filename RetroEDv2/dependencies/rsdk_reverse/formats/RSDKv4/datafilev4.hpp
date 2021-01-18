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
            m_filename     = filename;
            m_filenameHash = Utils::getMd5HashString(m_filename);
            QByteArray md5 = Utils::getMd5HashByteArray(m_filename);
            for (int h = 0; h < 0x10; ++h) m_md5Hash[h] = (byte)md5[h];
        }

        enum ExtensionTypes {
            UNKNOWN,
            OGG,
            WAV,
            R3D,
            PNG,
            GIF,
        };

        QString m_filename     = "";
        QString m_filenameHash = "";

        uint m_dataOffset = 0;
        uint m_fileSize   = 0;
        bool m_encrypted  = false;
        QByteArray m_filedata;

    private:
        byte m_md5Hash[0x10];

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

        void generateELoadKeys(uint arg1, uint arg2, uint size)
        {
            m_encryptionKeyA = generateKey(arg1);
            m_encryptionKeyB = generateKey(arg2);

            m_eKeyNo      = (int)(size / 4) & 0x7F;
            m_eKeyPosA    = 0;
            m_eKeyPosB    = 8;
            m_eNybbleSwap = 0;
        }

        QByteArray decrypt(QByteArray data);

        inline uint mulUnsignedHigh(uint64_t arg1, uint64_t arg2)
        {
            return (uint)(((uint64_t)arg1 * arg2) >> 32);
        }

        byte getExtensionFromData();

        byte m_extension = ExtensionTypes::UNKNOWN;

        QByteArray m_encryptionKeyA;
        QByteArray m_encryptionKeyB;
        int m_eKeyNo;
        int m_eKeyPosA;
        int m_eKeyPosB;
        int m_eNybbleSwap;
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
            filename = m_filename;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    const byte m_signature[6] = { (byte)'R', (byte)'S', (byte)'D', (byte)'K', (byte)'v', (byte)'B' };

    QList<FileInfo> m_files;

    QString m_filename = "";
};

} // namespace RSDKv4

#endif // DATAFILE_V4_H
