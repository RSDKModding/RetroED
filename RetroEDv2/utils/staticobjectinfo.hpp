#ifndef STATICOBJECTINFO_H
#define STATICOBJECTINFO_H

class StaticObjectInfo
{
public:
    class ArrayInfo
    {
    public:
        ArrayInfo() {}
        ArrayInfo(Reader &reader) { read(reader); }

        inline void read(Reader &reader)
        {
            m_name        = reader.readString();
            ushort valCnt = reader.read<ushort>();
            entries.clear();
            for (int v = 0; v < valCnt; ++v) entries.append(reader.readString());
        }

        inline void write(Writer &writer)
        {
            writer.write(m_name);
            writer.write((ushort)entries.count());
            for (auto &v : entries) writer.write(v);
        }

        QString m_name = "";
        QList<QString> entries;

        // not written
        int id = -1;
    };
    StaticObjectInfo() {}
    StaticObjectInfo(QString filename) { read(filename); }
    StaticObjectInfo(Reader &reader) { read(reader); }

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

    QString m_name = "Unknown Object";
    QList<ArrayInfo> values;

    QString m_filename;
    byte m_fileVer = 1;
};

#endif // STATICOBJECTINFO_H
