#pragma once

#define ENTITY_COUNT_v4     (0x4A0)
#define TEMPENTITY_START_v4 (ENTITY_COUNT_v4 - 0x80)
#define OBJECT_COUNT_v4     (0x100)
#define TYPEGROUP_COUNT_v4  (0x103)
#define SFX_COUNT_v4   (0x100)

#define SCRIPTCODE_COUNT_v4 (0x40000)
#define JUMPTABLE_COUNT_v4  (0x4000)
#define FUNCTION_COUNT_v4   (0x200)

#define JUMPSTACK_COUNT_v4 (0x400)
#define FUNCSTACK_COUNT_v4 (0x400)
#define FORSTACK_COUNT_v4  (0x400)

#define SPRITEFRAME_COUNT_v4 (0x1000)

#define COMMON_SCRIPT_VAR_COUNT_v4 (110 + 6)
#define SCRIPT_VAR_COUNT_v4        (COMMON_SCRIPT_VAR_COUNT_v4 + 0x1DF)

#include <RSDKv4/bytecodev4.hpp>
#include <RSDKv4/gameconfigv4.hpp>

class Compilerv4
{
public:
    Compilerv4();

public:
    enum ScriptVarType { VAR_ALIAS, VAR_STATICVALUE, VAR_TABLE };
    enum ScriptVarAccessModifier { ACCESS_NONE, ACCESS_PUBLIC, ACCESS_PRIVATE };

    struct ScriptVariableInfo {
        ScriptVariableInfo()
        {
            type   = VAR_ALIAS;
            access = ACCESS_NONE;
            StrCopy(name, "");
            StrCopy(value, "");
        }

        ScriptVariableInfo(byte type, byte access, const char *name, const char *value)
        {
            this->type   = type;
            this->access = access;
            StrCopy(this->name, name);
            StrCopy(this->value, value);
        }

        byte type;
        byte access;
        char name[0x20];
        char value[0x20];
    };

    struct VariableInfo {
        VariableInfo() { name = ""; }
        VariableInfo(const char *functionName) { name = functionName; }

        const char *name = "";
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

    struct ScriptPtr {
        int scriptCodePtr;
        int jumpTablePtr;
    };

    struct ScriptFunction {

        byte access;
        char name[0x20];
        ScriptPtr ptr;
    };

    struct ObjectScript {
        // Game
        ScriptPtr eventDraw;
        ScriptPtr eventUpdate;
        ScriptPtr eventStartup;

        // Editor
        ScriptPtr eventRSDKDraw;
        ScriptPtr eventRSDKLoad;
        ScriptPtr eventRSDKEdit;
        byte spriteSheetID;
        int frameListOffset;
        byte spriteFrameCount;
    };

    struct ScriptEngine {
        int operands[10];
        int tempValue[8];
        int arrayPosition[9];
        int checkResult;
    };

    struct TypeGroupList {
        int entityRefs[ENTITY_COUNT_v4];
        int listSize = 0;
    };

    struct Entity {
        Entity()
        {
            memset(values, 0, 48 * sizeof(int));
            memset(flailing, 0, 5 * sizeof(byte));
        }
        int XPos      = 0;
        int YPos      = 0;
        int XVelocity = 0;
        int YVelocity = 0;
        int speed     = 0;
        int values[48];
        int state               = 0;
        int angle               = 0;
        int scale               = 0;
        int rotation            = 0;
        int alpha               = 0;
        int animationTimer      = 0;
        int animationSpeed      = 0;
        int lookPosX            = 0;
        int lookPosY            = 0;
        ushort typeGroup        = 0;
        byte type               = 0;
        byte propertyValue      = 0;
        byte priority           = 1;
        signed char drawOrder   = 3;
        byte direction          = 0;
        byte inkEffect          = 0;
        byte animation          = 0;
        byte prevAnimation      = 0;
        byte frame              = 0;
        byte collisionMode      = 0;
        byte collisionPlane     = 0;
        signed char controlMode = 0;
        byte controlLock        = 0;
        byte pushing            = 0;
        byte visible            = true;
        byte tileCollisions     = true;
        byte objectInteractions = true;
        byte gravity            = 0;
        byte left               = 0;
        byte right              = 0;
        byte up                 = 0;
        byte down               = 0;
        byte jumpPress          = 0;
        byte jumpHold           = 0;
        byte trackScroll        = 0;
        byte flailing[5];
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

    enum ScriptEvents { EVENT_MAIN = 0, EVENT_DRAW = 1, EVENT_STARTUP = 2 };
    enum ScriptEditorEvents { EVENT_RSDKDRAW = 0, EVENT_RSDKLOAD = 1, EVENT_RSDKEDIT = 2 };

    ObjectScript objectScriptList[OBJECT_COUNT_v4];
    ScriptFunction functionList[FUNCTION_COUNT_v4];

    int scriptCode[SCRIPTCODE_COUNT_v4];
    int jumpTable[JUMPTABLE_COUNT_v4];

    int jumpTableStack[JUMPSTACK_COUNT_v4];
    int jumpTableStackPos = 0;

    int foreachStack[FORSTACK_COUNT_v4];
    int foreachStackPos;

    TypeGroupList objectTypeGroupList[TYPEGROUP_COUNT_v4];

    char scriptText[0x1000];

    int scriptCodePos    = 0;
    int scriptCodeOffset = 0;
    int jumpTablePos     = 0;
    int jumpTableOffset  = 0;

    int functionStack[FUNCSTACK_COUNT_v4];
    int functionStackPos = 0;

    int functionCount = 0;
    QString functionNames[FUNCTION_COUNT_v4];

    int scriptValueListCount = 0;
    static ScriptVariableInfo scriptValueList[SCRIPT_VAR_COUNT_v4];

    static FunctionInfo functionList_rev00[];
    static FunctionInfo functionList_rev01[];
    static FunctionInfo functionList_rev02[];
    static FunctionInfo functionList_rev03[];

    FunctionInfo *opcodeFunctionList;

    static VariableInfo variableNames_rev00[];
    static VariableInfo variableNames_rev01[];
    static VariableInfo variableNames_rev02[];
    static VariableInfo variableNames_rev03[];

    VariableInfo *variableNames;

    int lineID = 0;

    int scriptCount = 0;

    int globalScriptCount     = 0;
    int globalScriptCodeCount = 0;
    int globalJumpTableCount  = 0;

    bool scriptError = false;
    QString errorMsg = "";
    QString errorPos = "";
    QString errorScr = "";
    int errorLine    = 0;

    const char *gamePlatform      = "EDITOR";
    const char *gameRenderType    = "EDITOR_RENDERING";
    const char *gameHapticSetting = "NO_F_FEEDBACK";
    const char *releaseType       = "USE_STANDALONE";

    QList<QString> globalVariables;

    char typeNames[OBJECT_COUNT_v4][0x40];
    char sfxNames[SFX_COUNT_v4][0x40];

    void CheckAliasText(char *text);
    void CheckStaticText(char *text);
    bool CheckTableText(char *text);
    void ConvertArithmaticSyntax(char *text);
    void ConvertConditionalStatement(char *text);
    bool ConvertSwitchStatement(char *text);
    void ConvertFunctionText(char *text);
    void CheckCaseNumber(char *text);
    bool ReadSwitchCase(char *text);
    void ReadTableValues(char *text);
    void CopyAliasStr(char *dest, char *text, bool arrayIndex);

    void ParseScriptFile(QString scriptName, int scriptID, bool inEditor = true);

    void ClearScriptData();

    void WriteBytecode(QString path);

    int objectEntityPos = 0;

    ScriptEngine scriptEng;
    Entity objectEntityList[ENTITY_COUNT_v4];
    SpriteFrame scriptFrames[SPRITEFRAME_COUNT_v4];
    int scriptFrameCount = 0;

    int xScrollOffset = 0;
    int yScrollOffset = 0;

    void *editor = nullptr;

    void ProcessScript(int scriptCodeStart, int jumpTableStart, byte scriptEvent);

    RSDKv4::Bytecode bytecode;

    RSDKv4::GameConfig gameConfig;

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
        angle &= 0x1FF;
        return sinVal512[angle];
    }

    inline int cos512(int angle)
    {
        if (angle < 0)
            angle = 0x200 - angle;
        angle &= 0x1FF;
        return cosVal512[angle];
    }

    inline int sin256(int angle)
    {
        if (angle < 0)
            angle = 0x100 - angle;
        angle &= 0xFF;
        return sinVal256[angle];
    }

    inline int cos256(int angle)
    {
        if (angle < 0)
            angle = 0x100 - angle;
        angle &= 0xFF;
        return cosVal256[angle];
    }

    // Get Arc Tan value
    byte ArcTanLookup(int X, int Y);

    bool inEditor = false;
};


