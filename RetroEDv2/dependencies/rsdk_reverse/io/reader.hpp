#ifndef READER_H
#define READER_H

class Reader
{
public:
    explicit Reader(QString filepath);
    explicit Reader(QDataStream *stream);

    inline bool seek(qint64 position)
    {
        if (m_file)
            return m_file->seek(position);
        else
            return m_stream->device()->seek(position);
    };
    inline qint64 tell()
    {
        if (m_file)
            return m_file->pos();
        else
            return m_stream->device()->pos();
    };
    inline bool isEOF()
    {
        if (m_file)
            return m_file->atEnd();
        else
            return m_stream->atEnd();
    }

    inline void close()
    {
        if (m_file)
            m_file->close();
        else
            m_stream->device()->close();
    }
    inline Reader getCReader() { return Reader(new QDataStream(readZLib())); }

    inline bool matchesSignature(const QByteArray &magic)
    {
        seek(0);
        return magic == readByteArray(magic.length());
    }
    inline bool matchesSignature(byte *magic, byte len)
    {
        seek(0);
        return QByteArray((const char *)magic, len) == readByteArray(len);
    }
    inline bool matchesSignature(const byte *magic, byte len)
    {
        seek(0);
        return QByteArray((const char *)magic, len) == readByteArray(len);
    }

    inline QString readString(int mode = 0)
    {
        if (!mode) {
            byte len = read<byte>();
            QByteArray string;
            for (byte i = 0; i < len; ++i) string += read<byte>();
            return QString::fromLatin1(string);
        }
        else if (mode == 1) {
            ushort len      = read<ushort>();
            ushort *unicode = new ushort[len + 1];
            for (byte i = 0; i < len; ++i) unicode[i] = read<ushort>();
            unicode[len] = 0;
            QString str  = QString::fromUtf16(unicode);
            delete[] unicode;
            return str;
        }
        else if (mode == 2) {
            ushort len = read<ushort>();
            QByteArray string;
            for (byte i = 0; i < len; ++i) string += read<byte>();
            return QString::fromLatin1(string);
        }
    }

    inline QByteArray readByteArray(qint64 len, bool compressed = false)
    {
        QByteArray result;
        char *buffer = new char[sizeof(byte) * len];
        m_stream->readRawData(buffer, sizeof(byte) * len);
        result.append(buffer, len);
        delete[] buffer;

        if (!compressed)
            return result;
        return qUncompress(result);
    }
    QByteArray readZLib();

    QSharedPointer<QFile> m_file;

    template <typename T> inline T read()
    {
        char *buffer = new char[sizeof(T)];

        if (isEOF())
            memset(buffer, 0, sizeof(T));
        else
            m_stream->readRawData(buffer, sizeof(T));

        T result = *(reinterpret_cast<T *>(buffer));
        delete[] buffer;
        return result;
    }

    template <typename T> T peek()
    {
        qint64 pos = tell();
        T nextVal  = read<T>();
        seek(pos);
        return nextVal;
    }

    qint64 m_filesize  = 0;
    QString m_filepath = "";
    bool m_initialised = false;

private:
    QSharedPointer<QDataStream> m_stream;
};

#endif // READER_H
