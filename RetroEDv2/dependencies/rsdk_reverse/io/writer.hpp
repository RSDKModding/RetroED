#ifndef WRITER_H
#define WRITER_H

class Writer
{
public:
    Writer(QString filename);
    Writer(QDataStream *customStream);
    Writer(QByteArray *byteArray, QIODevice::OpenModeFlag mode = QIODevice::WriteOnly);

    inline bool seek(qint64 position) { return m_stream->device()->seek(position); };
    inline qint64 tell() { return m_file->pos(); };
    inline bool flush()
    {
        if (!m_file)
            m_stream->device()->close();
        else
            return m_file->commit();
        return false;
    }

    inline QString filename() { return m_file->fileName(); }

    template <typename T> inline void write(T data)
    {
        m_stream->writeRawData((char *)&data, sizeof(data));
    }

    inline void write(byte *data, int len, bool compressed = false)
    {
        if (!compressed)
            m_stream->writeRawData((const char *)data, len);
        else {
            QByteArray compressed = qCompress(data, len);
            compressed.remove(0, 4);
            write(compressed);
        }
    }

    inline void write(QByteArray data, bool compressed = false)
    {
        write((byte *)data.data(), data.length(), compressed);
    }
    inline void writeCompressed(QByteArray data)
    {
        QByteArray compressed = qCompress(data);
        compressed.remove(0, 4);
        write((int)(compressed.size() + sizeof(int)));
        uint decomp = data.size();
        write((uint)((decomp << 24) | ((decomp << 8) & 0x00FF0000) | ((decomp >> 8) & 0x0000FF00)
                     | (decomp >> 24)));
        write(compressed);
    }

    inline void write(QString string, int mode = 0)
    {
        if (!mode) {
            write((byte)string.length());
            QByteArray ascii = string.toLatin1();
            write(ascii);
        }
        else if (mode == 1) {
            write((ushort)string.length());
            QTextCodec *codec               = QTextCodec::codecForName("UTF-16");
            QTextEncoder *encoderWithoutBom = codec->makeEncoder(QTextCodec::IgnoreHeader);
            QByteArray bytes                = encoderWithoutBom->fromUnicode(string);

            write(bytes);
        }
        else if (mode == 2) {
            write((ushort)string.length());
            QByteArray ascii = string.toLatin1();
            write(ascii);
        }
    }

    inline void writeText(QString string = "")
    {
        for (int i = 0; i < string.length(); ++i) write((byte)string[i].toLatin1());
    }

    inline void writeLine(QString string = "", byte format = 0)
    {
        for (int i = 0; i < string.length(); ++i) write((byte)string[i].toLatin1());

        switch (format) {
            default:
            case 0: write((byte)'\n'); break;
            case 1:
                write((byte)'\r');
                write((byte)'\n');
                break;
            case 2: write((byte)'\r'); break;
        }
    }

    bool m_initialised = false;
    QString m_filename = "";

private:
    QSharedPointer<QSaveFile> m_file;
    QSharedPointer<QDataStream> m_stream;
};

#endif // WRITER_H
