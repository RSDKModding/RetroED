#ifndef COMPILER_V1_H
#define COMPILER_V1_H

#define SCRIPTDATA_COUNT_v1 (0xFFF)
#define LABEL_COUNT_v1      (0xFFF)
#define JUMPTABLE_COUNT_v1  (0xFFF)

#define JUMPSTACK_COUNT_v1 (0x40)

#define SPRITEFRAME_COUNT_v1 (0x100)

class Compilerv1
{
public:
    QString scriptText = "";

    int scriptCodePos    = 0;
    int jumpTableDataPos = 0;

    int aliasCount   = 0;
    int scriptLineID = 0;
    int subLineID    = 0;

    bool scriptError = false;
    QString errorMsg = "";
    QString errorPos = "";
    QString errorScr = "";
    int errorLine    = 0;

    enum ScriptSubs { SUB_MAIN, SUB_PLAYERINTERACTION, SUB_DRAW, SUB_STARTUP, SUB_RSDK };

    void parseScriptFile(QString scriptName);

    void clearScriptData();

    RSDKv1::Script script;

    Compilerv1() {}

private:
    int findStringToken(QString &string, QString token, char stopID);

    void checkAliasText(QString &text);
    void convertArithmaticSyntax(QString &text);
    bool convertSwitchStatement(QString &text);
    void convertFunctionText(QString &text);
    bool readLabel(QString &text);
    bool readSwitchCase(QString &text);
    void appendIntegerToString(QString &text, int value);
    bool convertStringToInteger(QString &text, int *value);
    void copyAliasStr(QString &dest, QString text, bool arrayIndex);

    int scriptSub = 0;

    QList<int> jumpTableStack;
};

#endif // COMPILER_V1_H
