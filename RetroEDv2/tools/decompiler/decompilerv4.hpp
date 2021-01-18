#ifndef DECOMPILER_V4_H
#define DECOMPILER_V4_H

namespace RSDKv4
{

class Decompiler
{
public:
    class SwitchJumpPtr
    {
    public:
        QList<int> m_cases;
        int m_jump;

        inline bool operator==(const SwitchJumpPtr &rhs)
        {
            if (m_cases.count() != rhs.m_cases.count())
                return false;
            for (int c = 0; c < m_cases.count(); ++c) {
                if (m_cases[c] != rhs.m_cases[c])
                    return false;
            }
            return m_jump == rhs.m_jump;
        }
    };

    struct SwitchState {
    public:
        int m_jumpTableOffset;
        int m_lowCase;
        int m_highCase;
        int m_case;
        int m_defaultJmp;
        int m_endJmp;
        QList<SwitchJumpPtr> m_jumpPtr;
    };

    class StateScriptEngine
    {
    public:
        int m_scriptCodePtr    = 0;
        int m_jumpTablePtr     = 0;
        int m_scriptCodeOffset = 0;
        int m_jumpTableOffset  = 0;
        int m_scriptSub        = 0;
        int m_deep             = 0;
        int m_switchDeep       = 0;
        bool m_endFlag         = false;
        SwitchState m_switchState[0x100];

        StateScriptEngine()
        {
            m_scriptCodePtr    = 0;
            m_jumpTablePtr     = 0;
            m_scriptCodeOffset = 0;
            m_jumpTableOffset  = 0;
            m_scriptSub        = 0;
            m_deep             = 0;
            m_switchDeep       = 0;
            m_endFlag          = false;
            for (int i = 0; i < 0x100; i++) {
                m_switchState[i]           = SwitchState();
                m_switchState[i].m_jumpPtr = QList<SwitchJumpPtr>();
            }
        }

        StateScriptEngine IncDeep()
        {
            m_deep += 1;
            return *this;
        }
    };

    Decompiler() {}

    void decompile(RSDKv4::Bytecode bytecode, QString destPath = "");
    void clearScriptData();

    QList<QString> m_functionNames;
    int m_functionCount       = 0;
    int m_globalFunctionCount = 0;

    QList<QString> m_variableNames;

    QList<QString> m_sourceNames;
    QList<QString> m_typeNames;

    QList<QString> m_sfxNames;

    int m_globalScriptCount = 0;
    int m_globalArrayCount  = 0;
    int m_globalStaticCount = 0;
    int m_globalSFXCount    = 0;

    int m_lastOffset = 0;

    bool m_useHex           = false;
    bool m_useCustomAliases = false;
    bool m_seperateFolders  = false;
    bool m_mobileVer        = false;

    struct StaticVarInfo {
        QString m_name = "";
        int m_dataPos  = 0x3FFFF;

        StaticVarInfo() {}
        StaticVarInfo(QString n, int p)
        {
            m_name    = n;
            m_dataPos = p;
        }

        inline bool operator==(const StaticVarInfo &rhs)
        {
            return /*(m_name == rhs.m_name) && */ (m_dataPos == rhs.m_dataPos);
        }
    };

    QList<StaticVarInfo> m_staticVars;
    QList<StaticVarInfo> m_tables;

private:
    int decompileScript(RSDKv4::Bytecode &bytecode, Writer &writer, int scriptCodePtr, int jumpTablePtr,
                        QString subName, bool isFunction);
    void decompileSub(RSDKv4::Bytecode &bytecode, Writer writer, StateScriptEngine &state,
                      bool isFunction);

    inline QString toHexString(QString str)
    {
        QString original = str;
        if (!m_useHex)
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

    int m_scriptDataPos    = 0;
    int m_jumpTableDataPos = 0;

    StateScriptEngine m_state;
};

}; // namespace RSDKv4

#endif // DECOMPILER_V4_H
