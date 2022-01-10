#ifndef DECOMPILER_V3_H
#define DECOMPILER_V3_H

namespace RSDKv3
{

class Decompiler
{
public:
    class SwitchJumpPtr
    {
    public:
        QList<int> cases;
        int jumpPos;

        inline bool operator==(const SwitchJumpPtr &rhs)
        {
            if (cases.count() != rhs.cases.count())
                return false;
            for (int c = 0; c < cases.count(); ++c) {
                if (cases[c] != rhs.cases[c])
                    return false;
            }
            return jumpPos == rhs.jumpPos;
        }
    };

    struct SwitchState {
    public:
        int startJumpTablePos;
        int lowCase;
        int highCase;
        int caseID;
        int defaultJmp;
        int endJmp;
        QList<SwitchJumpPtr> jumpPtr;
    };

    class StateScriptEngine
    {
    public:
        int scriptCodePos      = 0;
        int jumpTablePtr       = 0;
        int startScriptCodePos = 0;
        int startJumpTablePos  = 0;
        int scriptSub          = 0;
        int scopeDepth         = 0;
        int switchDeep         = 0;
        bool endFlag           = false;
        SwitchState switchState[0x100];

        StateScriptEngine()
        {
            scriptCodePos      = 0;
            jumpTablePtr       = 0;
            startScriptCodePos = 0;
            startJumpTablePos  = 0;
            scriptSub          = 0;
            scopeDepth         = 0;
            switchDeep         = -1;
            endFlag            = false;
            for (int i = 0; i < 0x100; ++i) {
                switchState[i]         = SwitchState();
                switchState[i].jumpPtr = QList<SwitchJumpPtr>();
            }
        }

        StateScriptEngine IncDeep()
        {
            scopeDepth += 1;
            return *this;
        }
    };

    Decompiler() {}

    void decompile(QString destPath = "");
    void clearScriptData();

    QList<QString> functionNames;
    int functionCount       = 0;
    int globalFunctionCount = 0;

    QList<QString> variableNames;

    QList<QString> sourceNames;
    QList<QString> typeNames;

    QList<QString> sfxNames;

    int globalScriptCount = 0;
    int globalSFXCount    = 0;

    bool useHex           = false;
    bool useCustomAliases = false;
    bool seperateFolders  = false;
    bool mobileVer        = false;

    // Bytecode info
    QList<int> scriptCode;
    QList<int> jumpTable;
    QList<RSDKv3::Bytecode::ScriptInfo> scriptList;
    QList<RSDKv3::Bytecode::FunctionInfo> functionList;
    QString bytecodePath = "";

private:
    void decompileScript(Writer &writer, int scriptCodePtr, int jumpTablePtr, QString subName,
                         bool isFunction);
    void decompileSub(Writer writer, StateScriptEngine &state, bool isFunction);

    inline QString toHexString(QString str)
    {
        QString original = str;
        if (!useHex)
            return original;
        bool negative = str.startsWith("-");

        if (negative)
            str = str.replace("-", "");

        bool ok   = false;
        int value = str.toInt(&ok);
        if (ok) {
            if (value < 0x3FF && value > -0x3FF) {
                if (negative)
                    str = "-" + str;
                return str;
            }

            str = QString::number(value, 0x10);
            str = str.toUpper();

            str = "0x" + str;
            if (negative)
                str = "-" + str;
            return str;
        }
        return original;
    }

    QString setArrayValue(QString strIn, QString index);

    int scriptDataPos    = 0;
    int jumpTableDataPos = 0;

    StateScriptEngine state;
};

}; // namespace RSDKv3

#endif // DECOMPILER_V3_H
