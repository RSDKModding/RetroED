#ifndef BYTECODE_V3_H
#define BYTECODE_V3_H

namespace RSDKv3
{

class Bytecode
{
public:
    struct FunctionScript {
    public:
        FunctionScript() {}

        int mainScript    = 0;
        int mainJumpTable = 0;
    };

    struct ObjectScript {
    public:
        ObjectScript() {}

        int mainScript       = 0;
        int playerScript     = 0;
        int drawScript       = 0;
        int m_startupScript    = 0;
        int mainJumpTable    = 0;
        int playerJumpTable  = 0;
        int drawJumpTable    = 0;
        int m_startupJumpTable = 0;
    };

    Bytecode() {}
    Bytecode(QString filename, int scriptCount = 0, bool clear = true)
    {
        read(filename, scriptCount, clear);
    }
    Bytecode(Reader &reader, int scriptCount = 0, bool clear = true)
    {
        read(reader, scriptCount, clear);
    }

    inline void read(QString filename, int scriptCount = 0, bool clear = true)
    {
        Reader reader(filename);
        read(reader, scriptCount, clear);
    }
    void read(Reader &reader, int scriptCount = 0, bool clear = true);

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

    QList<int> scriptData;
    QList<int> jumpTableData;
    QList<ObjectScript> scriptList;
    QList<FunctionScript> functionList;

    int m_globalScriptDataCount = 0;
    int m_globalJumpTableCount  = 0;
    int globalScriptCount     = 0;

    QString m_filename = "";
};

} // namespace RSDKv3

#endif // BYTECODE_V3_H
