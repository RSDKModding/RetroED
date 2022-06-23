#ifndef DECOMPILER_V4_H
#define DECOMPILER_V4_H

namespace RSDKv4
{

class Decompiler
{
public:
    struct AliasInfo {
        AliasInfo() {}
        AliasInfo(QString aliasName, QString aliasVal)
        {
            name  = aliasName;
            value = aliasVal;
        }

        QString name  = "";
        QString value = "";
    };

    struct FunctionInfo {
        FunctionInfo() {}
        FunctionInfo(QString functionName, int opSize)
        {
            name       = functionName;
            opcodeSize = opSize;
        }

        QString name   = "";
        int opcodeSize = 0;
    };

    struct SwitchJumpPtr {
        QList<int> cases;
        int jump;

        inline bool operator==(const SwitchJumpPtr &rhs)
        {
            if (cases.count() != rhs.cases.count())
                return false;
            for (int c = 0; c < cases.count(); ++c) {
                if (cases[c] != rhs.cases[c])
                    return false;
            }
            return jump == rhs.jump;
        }
    };

    struct SwitchState {
        int jumpTableOffset;
        int lowCase;
        int highCase;
        int caseID;
        int defaultJmp;
        int endJmp;
        QList<SwitchJumpPtr> jumpPtr;
    };

    struct StateScriptEngine {
        int scriptCodePtr    = 0;
        int jumpTablePtr     = 0;
        int scriptCodeOffset = 0;
        int jumpTableOffset  = 0;
        int scriptSub        = 0;
        int deep             = 0;
        int switchDeep       = 0;
        bool endFlag         = false;
        SwitchState switchState[0x100];

        StateScriptEngine()
        {
            scriptCodePtr    = 0;
            jumpTablePtr     = 0;
            scriptCodeOffset = 0;
            jumpTableOffset  = 0;
            scriptSub        = 0;
            deep             = 0;
            switchDeep       = 0;
            endFlag          = false;
            for (int i = 0; i < 0x100; i++) {
                switchState[i]         = SwitchState();
                switchState[i].jumpPtr = QList<SwitchJumpPtr>();
            }
        }

        StateScriptEngine IncDeep()
        {
            deep += 1;
            return *this;
        }
    };

    Decompiler();

    void decompile(QString destPath = "");
    void clearScriptData();

    QList<FunctionInfo> functionList_rev00;
    QList<FunctionInfo> functionList_rev01;
    QList<FunctionInfo> functionList_rev02;
    QList<FunctionInfo> functionList_rev03;

    QList<QString> variableList_rev00;
    QList<QString> variableList;
    QList<QString> variableList_rev03;

    QList<QString> functionNames;
    int functionCount       = 0;
    int globalFunctionCount = 0;

    QList<QString> globalVarNames;

    QList<QString> sourceNames;
    QList<QString> typeNames;

    QList<QString> sfxNames;

    int globalScriptCount = 0;
    int globalArrayCount  = 0;
    int globalStaticCount = 0;
    int globalSFXCount    = 0;

    int lastOffset = 0;

    bool useHex           = false;
    bool useCustomAliases = false;
    bool seperateFolders  = false;
    byte engineRevision   = 2;

    struct StaticVarInfo {
        QString name = "";
        int dataPos  = 0x3FFFF;

        StaticVarInfo() {}
        StaticVarInfo(QString n, int p)
        {
            name    = n;
            dataPos = p;
        }

        inline bool operator==(const StaticVarInfo &rhs)
        {
            return /*(name == rhs.name) && */ (dataPos == rhs.dataPos);
        }
    };

    enum lineFormats {
        LINE_LF,
        LINE_CRLF,
        LINE_CR,
    };

    QList<StaticVarInfo> staticVars;
    QList<StaticVarInfo> tables;

    // Bytecode info
    QList<int> scriptCode;
    QList<int> jumpTable;
    QList<RSDKv4::Bytecode::ScriptInfo> scriptList;
    QList<RSDKv4::Bytecode::FunctionInfo> functionList;
    QString bytecodePath = "";

private:
    int decompileScript(Writer &writer, int scriptCodePtr, int jumpTablePtr, QString subName,
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

}; // namespace RSDKv4

#endif // DECOMPILER_V4_H
