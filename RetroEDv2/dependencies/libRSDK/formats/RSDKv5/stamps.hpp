#pragma once

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

        QString name        = "Stamp";
        Vector2<int> size   = Vector2<int>(0, 0);
        QList<ushort> tiles;

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


