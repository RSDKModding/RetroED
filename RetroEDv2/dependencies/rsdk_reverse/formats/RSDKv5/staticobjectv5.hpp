#ifndef STATICOBJECT_V5_H
#define STATICOBJECT_V5_H

namespace RSDKv5
{

class StaticObject
{
public:
    struct ArrayInfo {
        byte m_type    = 0;
        int m_size     = 0;
        int m_dataSize = 0;
        QList<int> m_values;
    };
    StaticObject() {}
    StaticObject(QString filename) { read(filename); }
    StaticObject(Reader &reader) { read(reader); }

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

    uint getOffset(int arrayID);

    int getDataSize(int type);

    byte m_signature[4] = { 'O', 'B', 'J', 0 };

    QList<ArrayInfo> m_arrays;

    QString m_filename = "";
};

} // namespace RSDKv5

#endif // STATICOBJECT_V5_H
