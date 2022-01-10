#ifndef SCRIPT_VRS_H
#define SCRIPT_VRS_H

namespace RSDKv1
{

class Script
{
public:
    class FunctionInfo
    {
    public:
        QString name   = "Function";
        int paramCount = 0;

        FunctionInfo() {}
        FunctionInfo(QString name, int paramCount)
        {
            this->name       = name;
            this->paramCount = paramCount;
        }
    };

    class ParamInfo
    {
    public:
        int value        = 0;
        sbyte arrayIndex = 0;
        bool isVariable  = false;

        QString name()
        {
            if (isVariable) {
                if (value >= 0 && value < variableList.count())
                    return variableList[value];
                else
                    return QString("Unknown Variable %1").arg(value);
            }
            else
                return "Constant Value";
        }

        ParamInfo() {}
    };

    class OpcodeInfo
    {
    public:
        byte opcode = 0xFF;
        QList<ParamInfo> parameters;

        QString name()
        {
            if (opcode >= 0 && opcode < opcodeList.count())
                return opcodeList[opcode].name;
            else
                return QString("Unknown Opcode %1").arg(opcode);
        }

        int paramCount()
        {
            if (opcode >= 0 && opcode < opcodeList.count())
                return opcodeList[opcode].paramCount;
            else
                return 0;
        }

        int size()
        {
            int size = 1;
            for (ParamInfo param : parameters) size += param.isVariable ? 3 : 2;
            return size;
        }

        OpcodeInfo() {}
    };

    class LabelInfo
    {
    public:
        int scriptCodePos = 0;
        int id            = 0;
        int lineID        = 0;

        LabelInfo() {}
    };

    class SwitchCaseInfo
    {
    public:
        int scriptCodePos = 0;
        int caseNum       = 0;
        int lineID        = 0;

        SwitchCaseInfo() {}
    };

    class SwitchInfo
    {
    public:
        int scriptCodePos        = 0;
        int lowestCase           = 0;
        int highestCase          = 0;
        int defaultScriptCodePos = 0;
        int defaultCaseLineID    = 0;
        int endScriptCodePos     = 0;

        QList<SwitchCaseInfo> cases;

        SwitchInfo() {}
    };

    class ScriptSub
    {
    public:
        QList<OpcodeInfo> scriptCode;
        QList<SwitchInfo> jumpTable;
        QList<LabelInfo> labels;
        QList<int> blankLines;

        int scriptCodeLength()
        {
            int size = 0;
            for (OpcodeInfo op : scriptCode) size += op.size();
            return size;
        }

        ScriptSub() {}
    };

    Script() {}
    Script(QString filename) { read(filename); }
    Script(Reader &reader) { read(reader); }

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

    static QList<QString> variableList;
    static QList<FunctionInfo> opcodeList;

    ScriptSub subs[5];

    QString filePath = "";
};

} // namespace RSDKv1

#endif // SCRIPT_VRS_H
