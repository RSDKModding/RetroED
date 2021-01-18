#ifndef ZONELIST_VRS_H
#define ZONELIST_VRS_H

namespace RSDKv1
{

class ZoneList
{
public:
    ZoneList() {}
    ZoneList(QString filename) { read(filename); }
    ZoneList(Reader &reader) { read(reader); }

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

    QString m_filename = "";
};

} // namespace RSDKv1

#endif // ZONELIST_VRS_H
