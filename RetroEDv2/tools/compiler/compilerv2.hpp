#ifndef COMPILERV2_H
#define COMPILERV2_H

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
    enum InkFlags { INK_NONE, INK_BLEND };
    enum DrawFXFlags { FX_SCALE, FX_ROTATE, FX_INK, FX_TINT };

    enum ScriptSubs { SUB_RSDK };

    ObjectScript objectScriptList[OBJECT_COUNT];

    int scriptData[SCRIPTDATA_COUNT];
    int jumpTableData[JUMPTABLE_COUNT];

    int jumpTableStack[JUMPSTACK_COUNT];

    int jumpTableStackPos = 0;

    QString scriptText = "";

    int scriptDataPos       = 0;
    int scriptDataOffset    = 0;
    int jumpTableDataPos    = 0;
    int jumpTableDataOffset = 0;
    int functionStackPos    = 0;

    int functionCount = 0;
    QString functionNames[FUNCTION_COUNT];

    int aliasCount = 0;
    int lineID     = 0;

    int scriptCount = 0;

    int globalScriptCount     = 0;
    int globalScriptDataCount = 0;
    int globalJumpTableCount  = 0;

    bool scriptError = false;
    QString errorMsg = "";
    QString errorPos = "";
    QString errorScr = "";
    int errorLine    = 0;

    QList<QString> globalVariables;

    int findStringToken(QString &string, QString token, char stopID);

    void checkAliasText(QString &text);
    void convertArithmaticSyntax(QString &text);
    void convertIfWhileStatement(QString &text);
    bool convertSwitchStatement(QString &text);
    void convertFunctionText(QString &text);
    void checkCaseNumber(QString &text);
    bool readSwitchCase(QString &text);
    void appendIntegerToSting(QString &text, int value);
    bool convertStringToInteger(QString &text, int *value);
    void copyAliasStr(QString &dest, QString text, bool arrayIndex);

    void parseScriptFile(QString scriptName, int scriptID);

    void clearScriptData();

    void writeBytecode(QString path);

    int objectLoop = 0;

    ScriptEngine scriptEng;
    Entity objectEntityList[ENTITY_COUNT];
    SpriteFrame scriptFrames[SPRITEFRAME_COUNT];
    int scriptFrameCount = 0;

    int xScrollOffset = 0;
    int yScrollOffset = 0;

    void *viewer = nullptr;

    void processScript(int scriptCodePtr, int jumpTablePtr, byte scriptSub);

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
