#include "includes.hpp"

#include "compilerv1.hpp"

struct AliasInfov1 {
    AliasInfov1()
    {
        name  = "";
        value = "";
    }
    AliasInfov1(QString aliasName, QString aliasVal)
    {
        name  = aliasName;
        value = aliasVal;
    }

    QString name  = "";
    QString value = "";
};

const QString scriptEvaluationTokens_v1[] = { "=", "+=", "-=", "++", "--", "*=", "/=", ">>=", "<<=" };

enum ScriptReadModes {
    READMODE_NORMAL      = 0,
    READMODE_STRING      = 1,
    READMODE_COMMENTLINE = 2,
    READMODE_ENDLINE     = 3,
    READMODE_EOF         = 4
};
enum ScriptParseModes { PARSEMODE_SCOPELESS = 0, PARSEMODE_FUNCTION = 1, PARSEMODE_ERROR = 0xFF };

enum ScrVar {
    VAR_UNUSED0,
    VAR_OBJECTTYPE,
    VAR_OBJECTPROPERTYVALUE,
    VAR_OBJECTXPOS,
    VAR_OBJECTYPOS,
    VAR_OBJECTVALUEA,
    VAR_OBJECTVALUEB,
    VAR_OBJECTVALUEC,
    VAR_OBJECTVALUED,
    VAR_OBJECTVALUEE,
    VAR_OBJECTVALUEF,
    VAR_TEMPOBJECTTYPE,
    VAR_TEMPOBJECTPROPERTYVALUE,
    VAR_TEMPOBJECTXPOS,
    VAR_TEMPOBJECTYPOS,
    VAR_TEMPOBJECTVALUEA,
    VAR_TEMPOBJECTVALUEB,
    VAR_TEMPOBJECTVALUEC,
    VAR_TEMPOBJECTVALUED,
    VAR_TEMPOBJECTVALUEE,
    VAR_TEMPOBJECTVALUEF,
    VAR_COUNT,
};

enum ScrFunc {
    FUNC_END,
    FUNC_EQUAL,
    FUNC_ADD,
    FUNC_SUB,
    FUNC_INC,
    FUNC_DEC,
    FUNC_MUL,
    FUNC_DIV,
    FUNC_SHR,
    FUNC_SHL,
    FUNC_UNUSEDA,
    FUNC_GOTO,
    FUNC_RAND,
    FUNC_COUNT,
};

AliasInfov1 aliases_v1[ALIAS_COUNT_v1] = {
    AliasInfov1("true", "1"),
    AliasInfov1("false", "0"),
    AliasInfov1("TYPE_BLANKOBJ", "0"),
    AliasInfov1("TYPE_RING", "1"),
    AliasInfov1("TYPE_RINGLOSS", "2"),
    AliasInfov1("TYPE_RINGSPARKLE", "3"),
    AliasInfov1("TYPE_MONITOR", "4"),
    AliasInfov1("TYPE_BROKENMONITOR", "5"),
    AliasInfov1("TYPE_YELLOWSPRING", "6"),
    AliasInfov1("TYPE_REDSPRING", "7"),
    AliasInfov1("TYPE_SPIKES", "8"),
    AliasInfov1("TYPE_LAMPPOST", "9"),
    AliasInfov1("TYPE_PLANESWITCH_L", "10"),
    AliasInfov1("TYPE_PLANESWITCH_R", "11"),
    AliasInfov1("TYPE_PLANESWITCH_U", "12"),
    AliasInfov1("TYPE_PLANESWITCH_D", "13"),
    AliasInfov1("TYPE_FORCESPIN_R", "14"),
    AliasInfov1("TYPE_FORCESPIN_L", "15"),
    AliasInfov1("TYPE_UNKNOWN16", "16"),
    AliasInfov1("TYPE_UNKNOWN17", "17"),
    AliasInfov1("TYPE_SIGNPOST", "18"),
    AliasInfov1("TYPE_EGGPRISON", "19"),
    AliasInfov1("TYPE_SMALL_EXPLOSION", "20"),
    AliasInfov1("TYPE_LARGE_EXPLOSION", "21"),
    AliasInfov1("TYPE_EGGPRISONDEBRIS", "22"),
    AliasInfov1("TYPE_ANIMAL", "23"),
    AliasInfov1("TYPE_UNKNOWN24", "24"),
    AliasInfov1("TYPE_UNKNOWN25", "25"),
    AliasInfov1("TYPE_SPECIALRING", "26"),
    AliasInfov1("TYPE_WATERSPLASH", "27"),
    AliasInfov1("TYPE_BUBBLER", "28"),
    AliasInfov1("TYPE_SMALL_AIRBUBBBLE", "29"),
    AliasInfov1("TYPE_SMOKEPUFF", "30"),
    AliasInfov1("Sfx_Jump", "0"),
    AliasInfov1("Sfx_RingL", "1"),
    AliasInfov1("Sfx_RingR", "2"),
    AliasInfov1("Sfx_Spring", "3"),
    AliasInfov1("Sfx_LoseRings", "4"),
    AliasInfov1("Sfx_Destroy", "5"),
    AliasInfov1("Sfx_LampPost", "6"),
    AliasInfov1("Sfx_Hurt", "7"),
    AliasInfov1("Sfx_Spin", "8"),
    AliasInfov1("Sfx_SpinDash", "9"),
    AliasInfov1("Sfx_Release", "10"),
    AliasInfov1("Sfx_SignPost", "11"),
    AliasInfov1("Sfx_Button", "12"),
    AliasInfov1("Sfx_ScoreAdd", "13"),
    AliasInfov1("Sfx_Skidding", "14"),
    AliasInfov1("Sfx_Explode", "15"),
    AliasInfov1("Sfx_WarpRing", "16"),
    AliasInfov1("Sfx_EnterSS", "17"),
    AliasInfov1("Sfx_BossHit", "18"),
    AliasInfov1("Sfx_Breathe", "19"),
    AliasInfov1("Sfx_Crumble", "20"),
    AliasInfov1("Sfx_BlueShield", "21"),
    AliasInfov1("Sfx_FireShield", "22"),
    AliasInfov1("Sfx_WaterShield", "23"),
    AliasInfov1("Sfx_ElecShield", "24"),
    AliasInfov1("Sfx_Flame", "25"),
    AliasInfov1("Sfx_ElecJump", "26"),
    AliasInfov1("Sfx_Bounce", "27"),
    AliasInfov1("Sfx_Catch", "28"),
    AliasInfov1("Sfx_Splash", "29"),
    AliasInfov1("Sfx_SSFail", "30"),
    AliasInfov1("Sfx_WaterChime", "31"),
    AliasInfov1("Sfx_Drowning", "32"),
    AliasInfov1("Sfx_Bumper", "33"),
    AliasInfov1("Sfx_EnterBonus", "34"),
    AliasInfov1("Sfx_ExitSS", "35"),
    AliasInfov1("Sfx_Landing", "36"),
    AliasInfov1("Sfx_GSlide", "37"),
    AliasInfov1("Sfx_Flying", "38"),
    AliasInfov1("Sfx_Tired", "39"),
    AliasInfov1("PRIORITY_INACTIVE", "0"),
    AliasInfov1("PRIORITY_BOUNDS", "1"),
    AliasInfov1("PRIORITY_ACTIVE", "2"),
};

int Compilerv1::findStringToken(QString &string, QString token, char stopID)
{
    int tokenCharID  = 0;
    bool tokenMatch  = true;
    int stringCharID = 0;
    int foundTokenID = 0;

    while (stringCharID < string.length()) {
        tokenCharID = 0;
        tokenMatch  = true;
        while (tokenCharID < token.length()) {
            if (tokenCharID + stringCharID >= string.length())
                return -1;

            if (string[tokenCharID + stringCharID] != token[tokenCharID])
                tokenMatch = false;

            ++tokenCharID;
        }
        if (tokenMatch && ++foundTokenID == stopID)
            return stringCharID;

        ++stringCharID;
    }
    return -1;
}

void Compilerv1::checkAliasText(QString &text)
{
    if (findStringToken(text, "#alias", 1))
        return;
    int textPos    = 6;
    int aliasMatch = 0;
    while (aliasMatch < 2) {
        if (aliasMatch) {
            if (aliasMatch == 1) {
                aliases_v1[aliasCount].name += text[textPos];
                if (textPos + 1 >= text.length())
                    ++aliasMatch;
            }
        }
        else if (text[textPos] == ':') {
            aliasMatch = 1;
        }
        else {
            aliases_v1[aliasCount].value += text[textPos];
        }
        ++textPos;
    }
    ++aliasCount;
}
void Compilerv1::convertArithmaticSyntax(QString &text)
{
    int token    = 0;
    int offset   = 0;
    int findID   = 0;
    QString dest = "";

    for (int i = FUNC_EQUAL; i < FUNC_UNUSEDA; ++i) {
        findID = findStringToken(text, scriptEvaluationTokens_v1[i - 1], 1);
        if (findID > -1) {
            offset = findID;
            token  = i;
        }
    }

    if (token > 0) {
        dest = RSDKv1::Script::opcodeList[token].name + "(";
        for (int i = 0; i < offset; ++i) dest += text[i];
        if (RSDKv1::Script::opcodeList[token].paramCount > 1) {
            dest += ',';
            offset += scriptEvaluationTokens_v1[token - 1].length();
            ++findID;
            while (offset < text.length()) dest += text[offset++];
        }

        dest = dest + ")";
        text = dest;
    }
}
bool Compilerv1::convertSwitchStatement(QString &text)
{
    if (findStringToken(text, "switch", 1))
        return false;
    QString switchText = "";
    switchText         = "switch(";
    for (int i = 6; i < text.length(); ++i) {
        if (text[i] != '=' && text[i] != '(' && text[i] != ')')
            switchText += text[i];
    }
    switchText += ",";
    appendIntegerToString(switchText, 0);
    switchText += ")";
    text = switchText;

    RSDKv1::Script::SwitchInfo jumpInfo;
    jumpInfo.scriptCodePos = scriptCodePos;
    if (scriptSub >= 0) {
        jumpTableStack.append(script.subs[scriptSub].jumpTable.count());
        script.subs[scriptSub].jumpTable.append(jumpInfo);
    }

    return true;
}
void Compilerv1::convertFunctionText(QString &text)
{
    RSDKv1::Script::OpcodeInfo opcodeInfo;
    QString strBuffer = "";
    QString funcName  = "";
    int opcode        = 0;
    int opcodeSize    = 0;
    int textPos       = 0;
    int namePos       = 0;
    for (namePos = 0; namePos < text.length(); ++namePos) {
        if (text[namePos] == '(')
            break;

        funcName += text[namePos];
    }

    for (int i = 0; i < RSDKv1::Script::opcodeList.count(); ++i) {
        if (funcName == RSDKv1::Script::opcodeList[i].name) {
            opcode     = i;
            opcodeSize = RSDKv1::Script::opcodeList[i].paramCount;
            textPos    = RSDKv1::Script::opcodeList[i].name.length();
            i          = RSDKv1::Script::opcodeList.count();
        }
    }

    if (opcode <= 0) {
        scriptError = true;
        errorMsg    = "OPCODE NOT FOUND";
        errorPos    = funcName;
    }
    else {
        opcodeInfo.opcode = opcode;

        if (RSDKv1::Script::opcodeList[opcode].name == "endswitch") {
            script.subs[scriptSub].jumpTable[jumpTableStack.last()].endScriptCodePos = scriptCodePos;

            jumpTableStack.removeAt(jumpTableStack.count() - 1);
        }

        if (RSDKv1::Script::opcodeList[opcode].name == "endswitch"
            || RSDKv1::Script::opcodeList[opcode].name == "break") {

            // idk what happened here
            RSDKv1::Script::ParamInfo param;
            opcodeInfo.parameters.append(param);

            opcodeSize = 0;
        }

        for (int i = 0; i < opcodeSize; ++i) {
            RSDKv1::Script::ParamInfo param;

            ++textPos;
            int value = 0;
            funcName  = "";
            strBuffer = "";
            while (textPos < text.length()) {
                if (text[textPos] == ',' || text[textPos] == ')')
                    break;

                if (value) {
                    if (text[textPos] == ']')
                        value = 0;
                    else
                        strBuffer += text[textPos];
                    ++textPos;
                }
                else {
                    if (text[textPos] == '[')
                        value = 1;
                    else
                        funcName += text[textPos];
                    ++textPos;
                }
            }

            // Eg: TempValue0 = FX_SCALE
            for (int a = 0; a < aliasCount; ++a) {
                if (funcName == aliases_v1[a].name) {
                    copyAliasStr(funcName, aliases_v1[a].value, 0);
                    if (findStringToken(aliases_v1[a].value, "[", 1) > -1)
                        copyAliasStr(strBuffer, aliases_v1[a].value, 1);
                }
            }

            if (convertStringToInteger(funcName, &value)) {
                param.isVariable = false;
                param.value      = value;
            }
            else if (funcName[0] == '"') {
                scriptError = true;
                errorMsg    = "STRING CONSTANT IS ILLEGAL";
                errorPos    = funcName;
                errorLine   = scriptLineID;
                value       = 0;
            }
            else {
                param.isVariable = true;
                param.value      = value;

                if (strBuffer.length()) {
                    if (strBuffer[0] == '+')
                        strBuffer.remove(0, 1);

                    if (convertStringToInteger(strBuffer, &value) == 1) {
                        param.arrayIndex = value;
                    }
                    else {
                        scriptError = true;
                        errorMsg    = "ARRAY INDEX NOT VALID";
                        errorPos    = funcName;
                        errorLine   = scriptLineID;
                        value       = 0;
                    }
                }

                value = -1;
                for (int i = 0; i < RSDKv1::Script::variableList.count(); ++i) {
                    if (funcName == RSDKv1::Script::variableList[i])
                        value = i;
                }

                if (value == -1 && !scriptError) {
                    scriptError = true;
                    errorMsg    = "OPERAND NOT FOUND";
                    errorPos    = funcName;
                    errorLine   = scriptLineID;
                    value       = 0;
                }
                param.value = value;
            }

            opcodeInfo.parameters.append(param);
        }

        if (scriptSub >= 0)
            script.subs[scriptSub].scriptCode.append(opcodeInfo);
        scriptCodePos += opcodeInfo.size();
    }
}
bool Compilerv1::readLabel(QString &text)
{
    QString labelText = "";
    if (!findStringToken(text, "LABEL", 1)) {
        int textPos = 5;
        while (textPos < text.length()) {
            if (text[textPos] != ':')
                labelText += text[textPos];
            ++textPos;
        }

        for (int a = 0; a < aliasCount; ++a) {
            if (labelText == aliases_v1[a].name)
                labelText = aliases_v1[a].value;
        }

        RSDKv1::Script::LabelInfo label;
        label.scriptCodePos = scriptCodePos;
        label.lineID        = subLineID;

        int val = 0;
        if (convertStringToInteger(labelText, &val))
            label.id = val;

        if (scriptSub >= 0)
            script.subs[scriptSub].labels.append(label);

        text = "";
        return true;
    }
    return false;
}

bool Compilerv1::readSwitchCase(QString &text)
{
    QString caseText = "";
    if (findStringToken(text, "case", 1)) {
        if (findStringToken(text, "default", 1)) {
            return false;
        }
        else {
            if (jumpTableStack.count() >= 0 && scriptSub >= 0) {
                script.subs[scriptSub].jumpTable[jumpTableStack.last()].defaultScriptCodePos =
                    scriptCodePos;
                script.subs[scriptSub].jumpTable[jumpTableStack.last()].defaultCaseLineID = subLineID;
            }
            return true;
        }
    }
    else {
        int textPos = 4;
        while (textPos < text.length()) {
            if (text[textPos] != ':')
                caseText += text[textPos];
            ++textPos;
        }

        for (int a = 0; a < aliasCount; ++a) {
            if (caseText == aliases_v1[a].name)
                caseText = aliases_v1[a].value;
        }

        int caseID = 0;
        if (!convertStringToInteger(caseText, &caseID)) {
            PrintLog(QString("WARNING: unable to convert case string \"%1\" to int, on line %2")
                         .arg(caseText)
                         .arg(scriptLineID));
        }

        RSDKv1::Script::SwitchCaseInfo caseInfo;
        caseInfo.lineID        = subLineID;
        caseInfo.caseNum       = caseID;
        caseInfo.scriptCodePos = scriptCodePos;
        if (scriptSub >= 0)
            script.subs[scriptSub].jumpTable[jumpTableStack.last()].cases.append(caseInfo);
        return true;
    }
    return false;
}
void Compilerv1::appendIntegerToString(QString &text, int value) { text += QString::number(value); }
bool Compilerv1::convertStringToInteger(QString &text, int *value)
{
    *value  = 0;
    bool ok = false;

    bool negative = false;
    if (text.startsWith("-")) {
        negative = true;
        text.remove(0, 1);
    }

    if (text.startsWith("0x") || text.startsWith("0X")) {
        text.remove(0, 2);
        *value = text.toInt(&ok, 0x10);
    }
    else if (text.startsWith("0b") || text.startsWith("0B")) {
        text.remove(0, 2);
        *value = text.toInt(&ok, 0b10);
    }
    else if (text.startsWith("0o") || text.startsWith("0O")) {
        text.remove(0, 2);
        *value = text.toInt(&ok, 0010);
    }
    else {
        *value = text.toInt(&ok, 10);
    }

    if (negative)
        *value = -*value;

    return ok;
}
void Compilerv1::copyAliasStr(QString &dest, QString text, bool arrayIndex)
{
    int textPos     = 0;
    bool arrayValue = false;
    dest            = "";
    if (arrayIndex) {
        while (textPos < text.length()) {
            if (arrayValue) {
                if (text[textPos] == ']')
                    arrayValue = false;
                else
                    dest += text[textPos];
                ++textPos;
            }
            else {
                if (text[textPos] == '[')
                    arrayValue = true;
                ++textPos;
            }
        }
    }
    else {
        while (textPos < text.length()) {
            if (arrayValue) {
                if (text[textPos] == ']')
                    arrayValue = false;
                ++textPos;
            }
            else {
                if (text[textPos] == '[')
                    arrayValue = true;
                else
                    dest += text[textPos];
                ++textPos;
            }
        }
    }
}

void Compilerv1::parseScriptFile(QString scriptName)
{
    // this->inEditor = inEditor;

    scriptLineID = 0;
    subLineID    = 0;

    aliasCount = COMMONALIAS_COUNT_v1;
    for (int i = COMMONALIAS_COUNT_v1; i < ALIAS_COUNT_v1; ++i) {
        aliases_v1[i].name  = "";
        aliases_v1[i].value = "";
    }

    scriptError = false;

    Reader reader(scriptName);

    script = RSDKv1::Script();
    if (reader.initialised) {
        int readMode  = READMODE_NORMAL;
        int parseMode = PARSEMODE_SCOPELESS;
        char prevChar = 0;
        char curChar  = 0;
        scriptSub     = -1;

        while (readMode < READMODE_EOF) {
            scriptText    = "";
            int textPos   = 0;
            readMode      = READMODE_NORMAL;
            bool semiFlag = false;
            while (readMode < READMODE_ENDLINE) {
                prevChar = curChar;
                curChar  = reader.read<char>();

                if (readMode == READMODE_STRING) {
                    if (curChar == '\t' || curChar == '\r' || curChar == '\n' || curChar == ';'
                        || readMode >= READMODE_COMMENTLINE) {
                        if (curChar == '\r') {
                            size_t pos = reader.tell();
                            char chr   = reader.read<char>();
                            if (chr != '\n') {
                                reader.seek(pos);
                            }
                            else {
                                curChar  = '\n';
                                prevChar = '\r';
                            }
                        }

                        if ((curChar == '\n' && prevChar != '\r')
                            || (curChar == '\n' && prevChar == '\r') || curChar == ';') {
                            readMode = READMODE_ENDLINE;
                            if (curChar == ';') {
                                semiFlag = true;

                                char nextChar = reader.peek<char>();
                                while (nextChar == ' ' || nextChar == '\t' || nextChar == '\r'
                                       || nextChar == '\n' || nextChar == ';') {
                                    reader.read<char>();
                                    nextChar = reader.peek<char>();
                                }
                            }
                        }
                    }
                    else if (curChar != '/' || textPos <= 0) {
                        scriptText += curChar;
                        textPos++;
                        if (curChar == '"')
                            readMode = READMODE_NORMAL;
                    }
                    else if (curChar == '/' && prevChar == '/') {
                        readMode = READMODE_COMMENTLINE;
                        scriptText.remove(--textPos, 1);
                    }
                    else {
                        scriptText += curChar;
                        textPos++;
                    }
                }
                else if (curChar == ' ' || curChar == '\t' || curChar == '\r' || curChar == '\n'
                         || curChar == ';' || readMode >= READMODE_COMMENTLINE) {
                    if (curChar == '\r') {
                        size_t pos = reader.tell();
                        char chr   = reader.read<char>();
                        if (chr != '\n') {
                            reader.seek(pos);
                        }
                        else {
                            curChar  = '\n';
                            prevChar = '\r';
                        }
                    }

                    if ((curChar == '\n' && prevChar != '\r') || (curChar == '\n' && prevChar == '\r')
                        || curChar == ';') {
                        readMode = READMODE_ENDLINE;
                        if (curChar == ';') {
                            semiFlag = true;

                            char nextChar = reader.peek<char>();
                            while (nextChar == ' ' || nextChar == '\t' || nextChar == '\r'
                                   || nextChar == '\n' || nextChar == ';') {
                                reader.read<char>();
                                nextChar = reader.peek<char>();
                            }
                        }
                        textPos++;
                    }
                }
                else if (curChar != '/' || textPos <= 0) {
                    scriptText += curChar;
                    textPos++;
                    if (curChar == '"' && !readMode)
                        readMode = READMODE_STRING;
                }
                else if (curChar == '/' && prevChar == '/') {
                    readMode = READMODE_COMMENTLINE;
                    scriptText.remove(--textPos, 1);
                }
                else {
                    scriptText += curChar;
                    textPos++;
                }
                if (reader.isEOF())
                    readMode = READMODE_EOF;
            }

            switch (parseMode) {
                case PARSEMODE_SCOPELESS:
                    if (!semiFlag)
                        ++scriptLineID;
                    checkAliasText(scriptText);

                    if (scriptText == "subMain") {
                        parseMode     = PARSEMODE_FUNCTION;
                        scriptSub     = SUB_MAIN;
                        subLineID     = 0;
                        scriptCodePos = 0;
                    }
                    if (scriptText == "subPlayerInteraction") {
                        parseMode     = PARSEMODE_FUNCTION;
                        scriptSub     = SUB_PLAYERINTERACTION;
                        subLineID     = 0;
                        scriptCodePos = 0;
                    }
                    if (scriptText == "subDraw") {
                        parseMode     = PARSEMODE_FUNCTION;
                        scriptSub     = SUB_DRAW;
                        subLineID     = 0;
                        scriptCodePos = 0;
                    }
                    if (scriptText == "subStartup") {
                        parseMode     = PARSEMODE_FUNCTION;
                        scriptSub     = SUB_STARTUP;
                        subLineID     = 0;
                        scriptCodePos = 0;
                    }
                    if (scriptText == "subRSDK") {
                        parseMode     = PARSEMODE_FUNCTION;
                        scriptSub     = SUB_RSDK;
                        subLineID     = 0;
                        scriptCodePos = 0;
                    }

                    if (scriptSub >= 0) {
                        script.subs[scriptSub].scriptCode.clear();
                        script.subs[scriptSub].blankLines.clear();
                        script.subs[scriptSub].labels.clear();
                        script.subs[scriptSub].jumpTable.clear();
                    }
                    break;
                case PARSEMODE_FUNCTION:
                    if (!semiFlag)
                        ++scriptLineID;
                    if (scriptText.length()) {
                        if (scriptText == "endsub") {
                            parseMode = PARSEMODE_SCOPELESS;
                            scriptSub = -1;
                        }
                        else {
                            convertSwitchStatement(scriptText);
                            if (!readLabel(scriptText)) {
                                convertArithmaticSyntax(scriptText);
                                if (!readSwitchCase(scriptText)) {
                                    convertFunctionText(scriptText);
                                    if (scriptError) {
                                        errorScr  = scriptName;
                                        parseMode = PARSEMODE_ERROR;
                                    }
                                }
                            }
                        }

                        if (!semiFlag)
                            subLineID++;
                    }
                    else {
                        if (scriptSub >= 0)
                            script.subs[scriptSub].blankLines.append(subLineID);
                    }
                    break;
                default: break;
            }
        }
    }
}

void Compilerv1::clearScriptData() {}
