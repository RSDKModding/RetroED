#pragma once

#define OBJECT_COUNT_v3     (0x100)
#define SFX_COUNT_v3        (0x100)
#define ENTITY_COUNT_v3     (0x4A0)
#define TEMPENTITY_START_v3 (ENTITY_COUNT_v3 - 0x80)

#define SCRIPTCODE_COUNT_v3 (0x40000)
#define JUMPTABLE_COUNT_v3  (0x4000)
#define FUNCTION_COUNT_v3   (0x200)

#define JUMPSTACK_COUNT_v3 (0x400)
#define FUNCSTACK_COUNT_v3 (0x400)

#define SPRITEFRAME_COUNT_v3 (0x1000)

#define COMMONALIAS_COUNT_v3 (0x22)
#define ALIAS_COUNT_v3       (COMMONALIAS_COUNT_v3 + 0x60)

#include <RSDKv3/bytecodev3.hpp>
#include <RSDKv3/gameconfigv3.hpp>

class Compilerv3
{
public:
    Compilerv3();

public:
    struct AliasInfo {
        AliasInfo()
        {
            StrCopy(name, "");
            StrCopy(value, "");
        }
        AliasInfo(const char *aliasName, const char *aliasVal)
        {
            StrCopy(name, aliasName);
            StrCopy(value, aliasVal);
        }

        char name[0x20];
        char value[0x20];
    };

    struct FunctionInfo {
        FunctionInfo()
        {
            name       = "";
            opcodeSize = 0;
        }
        FunctionInfo(const char *functionName, int opSize)
        {
            name       = functionName;
            opcodeSize = opSize;
        }

        const char *name = "";
        int opcodeSize   = 0;
    };

    struct VariableInfo {
        VariableInfo() { name = ""; }
        VariableInfo(const char *functionName) { name = functionName; }

        const char *name = "";
    };

    struct ScriptPtr {
        int scriptCodePtr;
        int jumpTablePtr;
    };

    struct ScriptFunction {
        char name[0x20];
        ScriptPtr ptr;
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

    ObjectScript objectScriptList[OBJECT_COUNT_v3];

    ScriptFunction functionList[FUNCTION_COUNT_v3];
    int functionCount = 0;

    int scriptCode[SCRIPTCODE_COUNT_v3];
    int jumpTable[JUMPTABLE_COUNT_v3];

    int jumpTableStack[JUMPSTACK_COUNT_v3];

    int jumpTableStackPos = 0;

    char scriptText[0x100];

    int scriptCodePos    = 0;
    int scriptCodeOffset = 0;
    int jumpTablePos     = 0;
    int jumpTableOffset  = 0;

    int functionStack[FUNCSTACK_COUNT_v3];
    int functionStackPos = 0;

    static FunctionInfo opcodeFunctionList[];
    static VariableInfo variableList[];

    static AliasInfo aliases[ALIAS_COUNT_v3];
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

    const char *gamePlatform      = "Editor";
    const char *gameRenderType    = "Editor_Rendering";
    const char *gameHapticSetting = "No_Haptics";
    const char *releaseType       = "Use_Standalone";

    QList<QString> globalVariables;

    char typeNames[OBJECT_COUNT_v3][0x40];

    QList<QString> globalSfxNames;
    QList<QString> stageSfxNames;

    void CheckAliasText(char *text);
    void ConvertArithmaticSyntax(char *text);
    void ConvertIfWhileStatement(char *text);
    bool ConvertSwitchStatement(char *text);
    void ConvertFunctionText(char *text);
    void CheckCaseNumber(char *text);
    bool ReadSwitchCase(char *text);
    void CopyAliasStr(char *dest, char *text, bool arrayIndex);

    void ParseScriptFile(QString scriptName, int scriptID, bool inEditor = true);

    void ClearScriptData();

    void WriteBytecode(QString path);

    int objectLoop = 0;

    ScriptEngine scriptEng;
    Entity objectEntityList[ENTITY_COUNT_v3];
    SpriteFrame scriptFrames[SPRITEFRAME_COUNT_v3];
    int scriptFrameCount = 0;

    int xScrollOffset = 0;
    int yScrollOffset = 0;

    void *editor = nullptr;

    void ProcessScript(int scriptCodePtr, int jumpTablePtr, byte scriptSub);

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


