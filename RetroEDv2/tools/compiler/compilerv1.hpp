#pragma once

#define COMMONALIAS_COUNT_v1 (75)
#define ALIAS_COUNT_v1       (COMMONALIAS_COUNT_v1 + 0x80)

#define SCRIPTDATA_COUNT_v1 (0xFFF)
#define LABEL_COUNT_v1      (0xFFF)
#define JUMPTABLE_COUNT_v1  (0xFFF)

#define JUMPSTACK_COUNT_v1 (0x40)

#define SPRITEFRAME_COUNT_v1 (0x100)

#include <RSDKv1/scriptv1.hpp>

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

    void ParseScriptFile(QString scriptName);

    void ClearScriptData();

    RSDKv1::Script script;

    Compilerv1() {}

private:
    int FindStringToken(QString &string, QString token, char stopID);

    void CheckAliasText(QString &text);
    void ConvertArithmaticSyntax(QString &text);
    bool ConvertSwitchStatement(QString &text);
    void ConvertFunctionText(QString &text);
    bool ReadLabel(QString &text);
    bool ReadSwitchCase(QString &text);
    void AppendIntegerToString(QString &text, int value);
    bool ConvertStringToInteger(QString &text, int *value);
    void CopyAliasStr(QString &dest, QString text, bool arrayIndex);

    int scriptSub = 0;

    QList<int> jumpTableStack;
};


