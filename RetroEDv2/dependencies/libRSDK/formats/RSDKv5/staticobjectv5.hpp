#pragma once

namespace RSDKv5
{

class StaticObject
{
public:
    enum StaticObjectTypes {
        SVAR_UINT8,
        SVAR_UINT16,
        SVAR_UINT32,
        SVAR_INT8,
        SVAR_INT16,
        SVAR_INT32,
        SVAR_BOOL,
        SVAR_PTR,
        SVAR_VEC2,
        SVAR_TEXT,
        SVAR_ANIMATOR,
        SVAR_HITBOX,
        SVAR_UNKNOWN,
    };

    struct ArrayInfo {
        byte type    = 0;
        int size     = 0;
        int dataSize = 0;
        QList<int> entries;
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
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    uint getOffset(int arrayID);

    int getDataSize(int type);

    byte signature[4] = { 'O', 'B', 'J', 0 };

    QList<ArrayInfo> values;

    QString filePath = "";
};

} // namespace RSDKv5


