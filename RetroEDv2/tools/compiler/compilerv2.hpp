#ifndef COMPILERV2_H
#define COMPILERV2_H

#define OBJECT_COUNT_v2     (0x100)
#define SFX_COUNT_v2        (0x100)
#define ENTITY_COUNT_v2     (0x4A0)
#define TEMPENTITY_START_v2 (ENTITY_COUNT - 0x80)

#define SCRIPTDATA_COUNT_v2 (0x40000)
#define JUMPTABLE_COUNT_v2  (0x4000)

#define JUMPSTACK_COUNT_v2 (0x400)

#define SPRITEFRAME_COUNT_v2 (0x1000)

class Compilerv2
{
public:
    Compilerv2();

public:
    struct ScriptPtr {
        int scriptCodePtr;
        int jumpTablePtr;
    };

    struct ObjectScript {
        ScriptPtr subRSDK;
        byte spriteSheetID;
        int frameListOffset;
        byte spriteFrameCount;
    };

    struct ScriptEngine {
        int operands[10];
        int tempValue[8];
        int arrayPosition[3];
        int checkResult;
    };

    struct Entity {
        int XPos;
        int YPos;
        int values[8];
        int scale;
        int rotation;
        byte type;
        byte propertyValue;
        byte state;
        byte priority;
        byte drawOrder;
        byte direction;
        byte inkEffect;
        byte frame;
    };

    struct SpriteFrame {
        int sprX;
        int sprY;
        int width;
        int height;
        int pivotX;
        int pivotY;
        byte sheetID;
        byte hitboxID;
    };
    enum FlipFlags { FLIP_NONE, FLIP_X, FLIP_Y, FLIP_XY };
    enum DrawFXFlags { FX_SCALE, FX_ROTATE, FX_INK, FX_TINT };

    enum ScriptSubs { SUB_RSDK };

    ObjectScript objectScriptList[OBJECT_COUNT_v2];

    int scriptCode[SCRIPTDATA_COUNT_v2];
    int jumpTable[JUMPTABLE_COUNT_v2];

    int jumpTableStack[JUMPSTACK_COUNT_v2];

    int jumpTableStackPos = 0;

    QString scriptText = "";

    int scriptCodePos    = 0;
    int scriptCodeOffset = 0;
    int jumpTablePos     = 0;
    int jumpTableOffset  = 0;

    int aliasCount = 0;
    int lineID     = 0;

    int scriptCount = 0;

    int globalScriptCount     = 0;
    int globalScriptCodeCount = 0;
    int globalJumpTableCount  = 0;

    bool scriptError = false;
    QString errorMsg = "";
    QString errorPos = "";
    QString errorScr = "";
    int errorLine    = 0;

    QList<QString> globalVariables;

    int FindStringToken(QString &string, QString token, char stopID);

    void CheckAliasText(QString &text);
    void ConvertArithmaticSyntax(QString &text);
    void ConvertIfWhileStatement(QString &text);
    bool ConvertSwitchStatement(QString &text);
    void ConvertFunctionText(QString &text);
    void CheckCaseNumber(QString &text);
    bool ReadSwitchCase(QString &text);
    void AppendIntegerToSting(QString &text, int value);
    bool ConvertStringToInteger(QString &text, int *value);
    void CopyAliasStr(QString &dest, QString text, bool arrayIndex);

    void ParseScriptFile(QString scriptName, int scriptID);

    void ClearScriptData();

    void WriteBytecode(QString path);

    int objectLoop = 0;

    ScriptEngine scriptEng;
    Entity objectEntityList[ENTITY_COUNT_v2];
    SpriteFrame scriptFrames[SPRITEFRAME_COUNT_v2];
    int scriptFrameCount = 0;

    int xScrollOffset = 0;
    int yScrollOffset = 0;

    void *editor = nullptr;

    void ProcessScript(int scriptCodePtr, int jumpTablePtr);

    RSDKv2::Bytecode bytecode;

private:
    int sinVal512[0x200];
    int cosVal512[0x200];

    int sinVal256[0x100];
    int cosVal256[0x100];

    byte atanVal256[0x100 * 0x100];

    inline int sin512(int angle)
    {
        if (angle < 0)
            angle = 0x200 - angle;
        angle &= 0x1FFu;
        return sinVal512[angle];
    }

    inline int cos512(int angle)
    {
        if (angle < 0)
            angle = 0x200 - angle;
        angle &= 0x1FFu;
        return cosVal512[angle];
    }

    inline int sin256(int angle)
    {
        if (angle < 0)
            angle = 0x100 - angle;
        angle &= 0xFFu;
        return sinVal256[angle];
    }

    inline int cos256(int angle)
    {
        if (angle < 0)
            angle = 0x100 - angle;
        angle &= 0xFFu;
        return cosVal256[angle];
    }

    // Get Arc Tan value
    byte ArcTanLookup(int X, int Y);
};

#endif // COMPILERV2_H
