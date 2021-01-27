#ifndef COMPILERV3_H
#define COMPILERV3_H

#define OBJECT_COUNT     (0x100)
#define ENTITY_COUNT     (0x4A0)
#define TEMPENTITY_START (ENTITY_COUNT - 0x80)

#define SCRIPTDATA_COUNT (0x40000)
#define JUMPTABLE_COUNT  (0x4000)
#define FUNCTION_COUNT   (0x200)

#define JUMPSTACK_COUNT (0x400)
#define FUNCSTACK_COUNT (0x400)

#define SPRITEFRAME_COUNT (0x1000)

class Compilerv3
{
public:
    Compilerv3();

public:
    struct ScriptPtr {
        int m_scriptCodePtr;
        int m_jumpTablePtr;
    };

    struct ObjectScript {
        ScriptPtr subRSDKDraw;
        ScriptPtr subRSDKLoad;
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
        int XPos = 0;
        int YPos = 0;
        int values[8];
        int scale          = 0x200;
        int rotation       = 0;
        int animationTimer = 0;
        int animationSpeed = 0;
        byte type          = 0;
        byte propertyValue = 0;
        byte state         = 0;
        byte priority      = 1;
        byte drawOrder     = 3;
        byte direction     = 0;
        byte inkEffect     = 0;
        byte alpha         = 0xFF;
        byte animation     = 0;
        byte prevAnimation = 0;
        byte frame         = 0;
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
    enum InkFlags { INK_NONE, INK_BLEND, INK_ALPHA, INK_ADD, INK_SUB };
    enum DrawFXFlags { FX_SCALE, FX_ROTATE, FX_ROTOZOOM, FX_INK, FX_TINT, FX_FLIP };

    enum ScriptSubs { SUB_RSDKDRAW = 0, SUB_RSDKLOAD = 1 };

    ObjectScript m_objectScriptList[OBJECT_COUNT];
    ScriptPtr m_functionList[FUNCTION_COUNT];

    int m_scriptData[SCRIPTDATA_COUNT];
    int m_jumpTableData[JUMPTABLE_COUNT];

    int m_jumpTableStack[JUMPSTACK_COUNT];

    int m_jumpTableStackPos = 0;

    QString m_scriptText = "";

    int m_scriptDataPos       = 0;
    int m_scriptDataOffset    = 0;
    int m_jumpTableDataPos    = 0;
    int m_jumpTableDataOffset = 0;
    int m_functionStackPos    = 0;

    int m_functionCount = 0;
    QString m_functionNames[FUNCTION_COUNT];

    int m_aliasCount = 0;
    int m_lineID     = 0;

    int m_scriptCount = 0;

    int m_globalScriptCount     = 0;
    int m_globalScriptDataCount = 0;
    int m_globalJumpTableCount  = 0;

    bool m_scriptError = false;
    QString errorMsg   = "";
    QString errorPos   = "";
    QString m_errorScr = "";
    int m_errorLine    = 0;

    QString m_gamePlatform      = "Standard";
    QString m_gameRenderType    = "SW_Rendering";
    QString m_gameHapticSetting = "No_Haptics";

    QList<QString> m_globalVariables;

    QString m_typeNames[OBJECT_COUNT];

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

    int m_objectLoop = 0;

    ScriptEngine m_scriptEng;
    Entity m_objectEntityList[ENTITY_COUNT];
    char scriptText[0x100];

    SpriteFrame scriptFrames[SPRITEFRAME_COUNT];
    int scriptFrameCount = 0;

    int xScrollOffset = 0;
    int yScrollOffset = 0;

    void *m_viewer = nullptr;

    void processScript(int scriptCodePtr, int jumpTablePtr, byte scriptSub);

    RSDKv3::Bytecode m_bytecode;

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

#endif // COMPILERV3_H
