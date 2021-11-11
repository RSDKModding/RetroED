#ifndef STAMPS_V5_H
#define STAMPS_V5_H

namespace RSDKv5
{

class Stamps
{
public:
    class StampEntry
    {
    public:
        StampEntry() {}
        StampEntry(Reader &reader) { read(reader); }

        QString name  = "Stamp";
        ushort width  = 0;
        ushort height = 0;

        QVector<QVector<ushort>> layout;

        // TODO: objects?

        void read(Reader &reader);
        void write(Writer &writer);
    };

    Stamps() {}
    Stamps(QString filename) { read(filename); }
    Stamps(Reader &reader) { read(reader); }

    inline void read(QString filename)
    {
        Reader reader(filename);
        read(reader);
    }
    void read(Reader &reader);

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

    QList<StampEntry> stampList;

    byte signature[4] = { 'S', 'C', 'N', 0 };

    QString filePath = "";
};

}; // namespace RSDKv5

#endif // STAMPS_V5_H
