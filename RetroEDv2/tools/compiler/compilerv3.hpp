#ifndef COMPILERV3_H
#define COMPILERV3_H

#define OBJECT_COUNT     (0x100)
#define SFX_COUNT        (0x100)
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
        int scriptCodePtr;
        int jumpTablePtr;
    };

    struct ObjectScript {
        // Game
        ScriptPtr subMain;
        ScriptPtr subPlayerInteraction;
        ScriptPtr subDraw;
        ScriptPtr subStartup;

        // Editor
        ScriptPtr subRSDKDraw;
        ScriptPtr subRSDKLoad;
        ScriptPtr subRSDKEdit;

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
    // enum InkFlags { INK_NONE, INK_BLEND, INK_ALPHA, INK_ADD, INK_SUB };
    enum DrawFXFlags { FX_SCALE, FX_ROTATE, FX_ROTOZOOM, FX_INK, FX_TINT, FX_FLIP };

    enum ScriptSubs { SUB_RSDKDRAW = 0, SUB_RSDKLOAD = 1, SUB_RSDKEDIT = 2 };

    ObjectScript objectScriptList[OBJECT_COUNT];
    ScriptPtr functionList[FUNCTION_COUNT];

    int scriptData[SCRIPTDATA_COUNT];
    int jumpTableData[JUMPTABLE_COUNT];

    int jumpTableStack[JUMPSTACK_COUNT];

    int jumpTableStackPos = 0;

    QString scriptText = "";

    int scriptDataPos       = 0;
    int scriptDataOffset    = 0;
    int jumpTableDataPos    = 0;
    int jumpTableDataOffset = 0;

    int functionStack[FUNCSTACK_COUNT];
    int functionStackPos = 0;

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

    QString gamePlatform      = "Editor";
    QString gameRenderType    = "SW_Rendering";
    QString gameHapticSetting = "No_Haptics";

    QList<QString> globalVariables;

    QString typeNames[OBJECT_COUNT];
    QList<QString> globalSfxNames;
    QList<QString> stageSfxNames;

    int findStringToken(QString &string, QString token, char stopID);

    void checkAliasText(QString &text);
    void convertArithmaticSyntax(QString &text);
    void convertIfWhileStatement(QString &text);
    bool convertSwitchStatement(QString &text);
    void convertFunctionText(QString &text);
    void checkCaseNumber(QString &text);
    bool readSwitchCase(QString &text);
    void appendIntegerToString(QString &text, int value);
    bool convertStringToInteger(QString &text, int *value);
    void copyAliasStr(QString &dest, QString text, bool arrayIndex);

    void parseScriptFile(QString scriptName, int scriptID, bool inEditor = true);

    void clearScriptData();

    void writeBytecode(QString path);

    int objectLoop = 0;

    ScriptEngine scriptEng;
    Entity objectEntityList[ENTITY_COUNT];
    SpriteFrame scriptFrames[SPRITEFRAME_COUNT];
    int scriptFrameCount = 0;

    int xScrollOffset = 0;
    int yScrollOffset = 0;

    void *editor = nullptr;

    void processScript(int scriptCodePtr, int jumpTablePtr, byte scriptSub);

    RSDKv3::Bytecode bytecode;

    RSDKv3::GameConfig gameConfig;

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

    bool inEditor = true;
};

#endif // COMPILERV3_H
