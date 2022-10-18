#pragma once

class Writer
{
public:
    Writer(QString filename);
    Writer(QDataStream *customStream);
    Writer(QByteArray *byteArray, QIODevice::OpenModeFlag mode = QIODevice::WriteOnly);

    inline bool seek(qint64 position) { return stream->device()->seek(position); };
    inline qint64 tell()
    {
        if (file)
            return file->pos();
        else if (stream)
            return writePos;
        return -1;
    };
    inline bool flush()
    {
        if (!file)
            stream->device()->close();
        else
            return file->commit();
        return false;
    }

    inline QString filename() { return file->fileName(); }

    template <typename T> inline void write(T data)
    {
        writePos += sizeof(T);
        stream->writeRawData((char *)&data, sizeof(data));
    }

    inline void write(byte *data, int len, bool compressed = false)
    {
        if (!compressed) {
            writePos += len;
            stream->writeRawData((const char *)data, len);
        }
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

    inline void writeCompressedRaw(QByteArray data)
    {
        QByteArray compressed = qCompress(data);
        compressed.remove(0, 4);
        write(compressed);
    }
    inline void write(QString string, int mode = 0)
    {
        bool nullTerminate = (mode & 0x80) != 0;
        mode &= 0x7F;

        if (!mode) {
            write((byte)(string.length() + (nullTerminate ? 1 : 0)));
            QByteArray ascii = string.toLatin1();
            write(ascii);
            if (nullTerminate)
                write((byte)0);
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

    inline void writeStringV5(QString string, int mode = 0) { write(string, mode | 0x80); }

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

    bool initialised = false;
    QString filePath = "";

private:
    int writePos = 0;

    QSharedPointer<QSaveFile> file;
    QSharedPointer<QDataStream> stream;
};


