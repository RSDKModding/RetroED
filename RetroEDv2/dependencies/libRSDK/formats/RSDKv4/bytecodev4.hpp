#pragma once

namespace RSDKv4
{

class Bytecode
{
public:
    struct FunctionInfo {
    public:
        FunctionInfo() {}

        int scriptCodePos = 0;
        int jumpTablePos  = 0;
    };

    struct ScriptInfo {
    public:
        ScriptInfo() {}

        FunctionInfo update;
        FunctionInfo draw;
        FunctionInfo startup;
    };

    Bytecode() {}
    Bytecode(QString filename) { read(filename); }
    Bytecode(Reader &reader) { read(reader); }

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

    QList<int> scriptCode;
    QList<int> jumpTable;
    QList<ScriptInfo> scriptList;
    QList<FunctionInfo> functionList;

    QString filePath = "";
};

} // namespace RSDKv4


