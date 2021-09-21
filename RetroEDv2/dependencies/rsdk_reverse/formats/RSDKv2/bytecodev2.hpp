#ifndef BYTECODE_V2_H
#define BYTECODE_V2_H

namespace RSDKv2
{

class Bytecode
{
public:
    struct FunctionScript {
    public:
        FunctionScript() {}

        int mainScript    = 0x3FFFF;
        int mainJumpTable = 0x3FFFF;
    };
    struct PlayerScript {
    public:
        PlayerScript() {}

        int scriptCodePtr_PlayerMain = 0x3FFFF;
        int jumpTablePtr_PlayerMain  = 0x3FFFF;
        QList<FunctionScript> playerStates;
    };

    struct ObjectScript {
    public:
        ObjectScript() {}

        int mainScript       = 0x3FFFF;
        int playerScript     = 0x3FFFF;
        int drawScript       = 0x3FFFF;
        int startupScript    = 0x3FFFF;
        int mainJumpTable    = 0x3FFFF;
        int playerJumpTable  = 0x3FFFF;
        int drawJumpTable    = 0x3FFFF;
        int startupJumpTable = 0x3FFFF;
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
            filename = filePath;
        if (filename == "")
            return;
        Writer writer(filename);
        write(writer);
    }
    void write(Writer &writer);

    QList<int> scriptData;
    QList<int> jumpTableData;
    QList<ObjectScript> scriptList;
    PlayerScript playerScript;

    int globalScriptDataCount = 0;
    int globalJumpTableCount  = 0;
    int globalScriptCount     = 0;

    QString filePath = "";
};

} // namespace RSDKv2

#endif // BYTECODE_V2_H
