#include "includes.hpp"

#define COMMONALIAS_COUNT_v4 (0x6A + 7)
#define ALIAS_COUNT_TRIM_v4  (0xE0)
#define ALIAS_COUNT_v4       (COMMONALIAS_COUNT_v4 + ALIAS_COUNT_TRIM_v4)

struct AliasInfov4 {
    AliasInfov4()
    {
        name  = "";
        value = "";
    }
    AliasInfov4(QString aliasName, QString aliasVal)
    {
        name  = aliasName;
        value = aliasVal;
    }

    QString name  = "";
    QString value = "";
};

struct FunctionInfov4 {
    FunctionInfov4()
    {
        name       = "";
        opcodeSize = 0;
    }
    FunctionInfov4(QString functionName, int opSize)
    {
        name       = functionName;
        opcodeSize = opSize;
    }

    QString name   = "";
    int opcodeSize = 0;
};

const char variableNamesv4[][0x20] = {
    // Internal Script Values
    "temp0",
    "temp1",
    "temp2",
    "temp3",
    "temp4",
    "temp5",
    "temp6",
    "temp7",
    "checkResult",
    "arrayPos0",
    "arrayPos1",
    "arrayPos2",
    "arrayPos3",
    "arrayPos4",
    "arrayPos5",
    "arrayPos6",
    "arrayPos7",
    "global",
    "local",

    // Object Properties
    "object.entityPos",
    "object.groupID",
    "object.type",
    "object.propertyValue",
    "object.xpos",
    "object.ypos",
    "object.ixpos",
    "object.iypos",
    "object.xvel",
    "object.yvel",
    "object.speed",
    "object.state",
    "object.rotation",
    "object.scale",
    "object.priority",
    "object.drawOrder",
    "object.direction",
    "object.inkEffect",
    "object.alpha",
    "object.frame",
    "object.animation",
    "object.prevAnimation",
    "object.animationSpeed",
    "object.animationTimer",
    "object.angle",
    "object.lookPosX",
    "object.lookPosY",
    "object.collisionMode",
    "object.collisionPlane",
    "object.controlMode",
    "object.controlLock",
    "object.pushing",
    "object.visible",
    "object.tileCollisions",
    "object.interaction",
    "object.gravity",
    "object.up",
    "object.down",
    "object.left",
    "object.right",
    "object.jumpPress",
    "object.jumpHold",
    "object.scrollTracking",
    "object.floorSensorL",
    "object.floorSensorC",
    "object.floorSensorR",
    "object.floorSensorLC",
    "object.floorSensorRC",
    "object.collisionLeft",
    "object.collisionTop",
    "object.collisionRight",
    "object.collisionBottom",
    "object.outOfBounds",
    "object.spriteSheet",

    // Object Values
    "object.value0",
    "object.value1",
    "object.value2",
    "object.value3",
    "object.value4",
    "object.value5",
    "object.value6",
    "object.value7",
    "object.value8",
    "object.value9",
    "object.value10",
    "object.value11",
    "object.value12",
    "object.value13",
    "object.value14",
    "object.value15",
    "object.value16",
    "object.value17",
    "object.value18",
    "object.value19",
    "object.value20",
    "object.value21",
    "object.value22",
    "object.value23",
    "object.value24",
    "object.value25",
    "object.value26",
    "object.value27",
    "object.value28",
    "object.value29",
    "object.value30",
    "object.value31",
    "object.value32",
    "object.value33",
    "object.value34",
    "object.value35",
    "object.value36",
    "object.value37",
    "object.value38",
    "object.value39",
    "object.value40",
    "object.value41",
    "object.value42",
    "object.value43",
    "object.value44",
    "object.value45",
    "object.value46",
    "object.value47",

    // Stage Properties
    "stage.state",
    "stage.activeList",
    "stage.listPos",
    "stage.timeEnabled",
    "stage.milliSeconds",
    "stage.seconds",
    "stage.minutes",
    "stage.actNum",
    "stage.pauseEnabled",
    "stage.listSize",
    "stage.newXBoundary1",
    "stage.newXBoundary2",
    "stage.newYBoundary1",
    "stage.newYBoundary2",
    "stage.curXBoundary1",
    "stage.curXBoundary2",
    "stage.curYBoundary1",
    "stage.curYBoundary2",
    "stage.deformationData0",
    "stage.deformationData1",
    "stage.deformationData2",
    "stage.deformationData3",
    "stage.waterLevel",
    "stage.activeLayer",
    "stage.midPoint",
    "stage.playerListPos",
    "stage.debugMode",
    "stage.entityPos",

    // Screen Properties
    "screen.cameraEnabled",
    "screen.cameraTarget",
    "screen.cameraStyle",
    "screen.cameraX",
    "screen.cameraY",
    "screen.drawListSize",
    "screen.xcenter",
    "screen.ycenter",
    "screen.xsize",
    "screen.ysize",
    "screen.xoffset",
    "screen.yoffset",
    "screen.shakeX",
    "screen.shakeY",
    "screen.adjustCameraY",

    "touchscreen.down",
    "touchscreen.xpos",
    "touchscreen.ypos",

    // Sound Properties
    "music.volume",
    "music.currentTrack",
    "music.position",

    // Input Properties
    "inputDown.up",
    "inputDown.down",
    "inputDown.left",
    "inputDown.right",
    "inputDown.buttonA",
    "inputDown.buttonB",
    "inputDown.buttonC",
    "inputDown.buttonX",
    "inputDown.buttonY",
    "inputDown.buttonZ",
    "inputDown.buttonL",
    "inputDown.buttonR",
    "inputDown.start",
    "inputDown.select",
    "inputPress.up",
    "inputPress.down",
    "inputPress.left",
    "inputPress.right",
    "inputPress.buttonA",
    "inputPress.buttonB",
    "inputPress.buttonC",
    "inputPress.buttonX",
    "inputPress.buttonY",
    "inputPress.buttonZ",
    "inputPress.buttonL",
    "inputPress.buttonR",
    "inputPress.start",
    "inputPress.select",

    // Menu Properties
    "menu1.selection",
    "menu2.selection",

    // Tile Layer Properties
    "tileLayer.xsize",
    "tileLayer.ysize",
    "tileLayer.type",
    "tileLayer.angle",
    "tileLayer.xpos",
    "tileLayer.ypos",
    "tileLayer.zpos",
    "tileLayer.parallaxFactor",
    "tileLayer.scrollSpeed",
    "tileLayer.scrollPos",
    "tileLayer.deformationOffset",
    "tileLayer.deformationOffsetW",
    "hParallax.parallaxFactor",
    "hParallax.scrollSpeed",
    "hParallax.scrollPos",
    "vParallax.parallaxFactor",
    "vParallax.scrollSpeed",
    "vParallax.scrollPos",

    // 3D Scene Properties
    "scene3D.vertexCount",
    "scene3D.faceCount",
    "scene3D.projectionX",
    "scene3D.projectionY",
    "scene3D.fogColor",
    "scene3D.fogStrength",

    "vertexBuffer.x",
    "vertexBuffer.y",
    "vertexBuffer.z",
    "vertexBuffer.u",
    "vertexBuffer.v",

    "faceBuffer.a",
    "faceBuffer.b",
    "faceBuffer.c",
    "faceBuffer.d",
    "faceBuffer.flag",
    "faceBuffer.color",

    "saveRAM",
    "engine.state",
    "engine.language",
    "engine.onlineActive",
    "engine.sfxVolume",
    "engine.bgmVolume",
    "engine.trialMode",
    "engine.deviceType",

    // EDITOR-ONLY
    "editor.variableID",
    "editor.variableValue",
    "editor.returnVariable",
    "editor.showGizmos",
};

const FunctionInfov4 functionsv4[] = {
    FunctionInfov4("End", 0),      // End of Script
    FunctionInfov4("Equal", 2),    // Equal
    FunctionInfov4("Add", 2),      // Add
    FunctionInfov4("Sub", 2),      // Subtract
    FunctionInfov4("Inc", 1),      // Increment
    FunctionInfov4("Dec", 1),      // Decrement
    FunctionInfov4("Mul", 2),      // Multiply
    FunctionInfov4("Div", 2),      // Divide
    FunctionInfov4("ShR", 2),      // Bit Shift Right
    FunctionInfov4("ShL", 2),      // Bit Shift Left
    FunctionInfov4("And", 2),      // Bitwise And
    FunctionInfov4("Or", 2),       // Bitwise Or
    FunctionInfov4("Xor", 2),      // Bitwise Xor
    FunctionInfov4("Mod", 2),      // Mod
    FunctionInfov4("FlipSign", 1), // Flips the Sign of the value

    FunctionInfov4("CheckEqual", 2),    // compare a=b, return result in CheckResult Variable
    FunctionInfov4("CheckGreater", 2),  // compare a>b, return result in CheckResult Variable
    FunctionInfov4("CheckLower", 2),    // compare a<b, return result in CheckResult Variable
    FunctionInfov4("CheckNotEqual", 2), // compare a!=b, return result in CheckResult Variable

    FunctionInfov4("IfEqual", 3),          // compare a=b, jump if condition met
    FunctionInfov4("IfGreater", 3),        // compare a>b, jump if condition met
    FunctionInfov4("IfGreaterOrEqual", 3), // compare a>=b, jump if condition met
    FunctionInfov4("IfLower", 3),          // compare a<b, jump if condition met
    FunctionInfov4("IfLowerOrEqual", 3),   // compare a<=b, jump if condition met
    FunctionInfov4("IfNotEqual", 3),       // compare a!=b, jump if condition met
    FunctionInfov4("else", 0),             // The else for an if statement
    FunctionInfov4("endif", 0),            // The end if

    FunctionInfov4("WEqual", 3),          // compare a=b, loop if condition met
    FunctionInfov4("WGreater", 3),        // compare a>b, loop if condition met
    FunctionInfov4("WGreaterOrEqual", 3), // compare a>=b, loop if condition met
    FunctionInfov4("WLower", 3),          // compare a<b, loop if condition met
    FunctionInfov4("WLowerOrEqual", 3),   // compare a<=b, loop if condition met
    FunctionInfov4("WNotEqual", 3),       // compare a!=b, loop if condition met
    FunctionInfov4("loop", 0),            // While Loop marker

    FunctionInfov4("ForEachActive", 3), // foreach loop, iterates through object group lists only if
                                        // they are active and interaction is true
    FunctionInfov4("ForEachAll", 3),    // foreach loop, iterates through objects matching type
    FunctionInfov4("next", 0),          // foreach loop, next marker

    FunctionInfov4("switch", 2),    // Switch Statement
    FunctionInfov4("break", 0),     // break
    FunctionInfov4("endswitch", 0), // endswitch

    // Math functions
    FunctionInfov4("Rand", 2),
    FunctionInfov4("Sin", 2),
    FunctionInfov4("Cos", 2),
    FunctionInfov4("Sin256", 2),
    FunctionInfov4("Cos256", 2),
    FunctionInfov4("ATan2", 3),
    FunctionInfov4("Interpolate", 4),
    FunctionInfov4("InterpolateXY", 7),

    // Graphics functions
    FunctionInfov4("LoadSpriteSheet", 1),
    FunctionInfov4("RemoveSpriteSheet", 1),
    FunctionInfov4("DrawSprite", 1),
    FunctionInfov4("DrawSpriteXY", 3),
    FunctionInfov4("DrawSpriteScreenXY", 3),
    FunctionInfov4("DrawTintRect", 4),
    FunctionInfov4("DrawNumbers", 7),
    FunctionInfov4("DrawActName", 7),
    FunctionInfov4("DrawMenu", 3),
    FunctionInfov4("SpriteFrame", 6),
    FunctionInfov4("EditFrame", 7),
    FunctionInfov4("LoadPalette", 5),
    FunctionInfov4("RotatePalette", 4),
    FunctionInfov4("SetScreenFade", 4),
    FunctionInfov4("SetActivePalette", 3),
    FunctionInfov4("SetPaletteFade", 6),
    FunctionInfov4("SetPaletteEntry", 3),
    FunctionInfov4("GetPaletteEntry", 3),
    FunctionInfov4("CopyPalette", 5),
    FunctionInfov4("ClearScreen", 1),
    FunctionInfov4("DrawSpriteFX", 4),
    FunctionInfov4("DrawSpriteScreenFX", 4),

    // More Useful Stuff
    FunctionInfov4("LoadAnimation", 1),
    FunctionInfov4("SetupMenu", 4),
    FunctionInfov4("AddMenuEntry", 3),
    FunctionInfov4("EditMenuEntry", 4),
    FunctionInfov4("LoadStage", 0),
    FunctionInfov4("DrawRect", 8),
    FunctionInfov4("ResetObjectEntity", 5),
    FunctionInfov4("BoxCollisionTest", 11),
    FunctionInfov4("CreateTempObject", 4),

    // Player and Animation functions
    FunctionInfov4("ProcessObjectMovement", 0),
    FunctionInfov4("ProcessObjectControl", 0),
    FunctionInfov4("ProcessAnimation", 0),
    FunctionInfov4("DrawObjectAnimation", 0),

    // Music
    FunctionInfov4("SetMusicTrack", 3),
    FunctionInfov4("PlayMusic", 1),
    FunctionInfov4("StopMusic", 0),
    FunctionInfov4("PauseMusic", 0),
    FunctionInfov4("ResumeMusic", 0),
    FunctionInfov4("SwapMusicTrack", 4),

    // Sound FX
    FunctionInfov4("PlaySfx", 2),
    FunctionInfov4("StopSfx", 1),
    FunctionInfov4("SetSfxAttributes", 3),

    // More Collision Stuff
    FunctionInfov4("ObjectTileCollision", 4),
    FunctionInfov4("ObjectTileGrip", 4),

    // Bitwise Not
    FunctionInfov4("Not", 1),

    // 3D Stuff
    FunctionInfov4("Draw3DScene", 0),
    FunctionInfov4("SetIdentityMatrix", 1),
    FunctionInfov4("MatrixMultiply", 2),
    FunctionInfov4("MatrixTranslateXYZ", 4),
    FunctionInfov4("MatrixScaleXYZ", 4),
    FunctionInfov4("MatrixRotateX", 2),
    FunctionInfov4("MatrixRotateY", 2),
    FunctionInfov4("MatrixRotateZ", 2),
    FunctionInfov4("MatrixRotateXYZ", 4),
    FunctionInfov4("MatrixInverse", 1),
    FunctionInfov4("TransformVertices", 3),

    FunctionInfov4("CallFunction", 1),
    FunctionInfov4("return", 0),

    FunctionInfov4("SetLayerDeformation", 6),
    FunctionInfov4("CheckTouchRect", 4),
    FunctionInfov4("GetTileLayerEntry", 4),
    FunctionInfov4("SetTileLayerEntry", 4),

    FunctionInfov4("GetBit", 3),
    FunctionInfov4("SetBit", 3),

    FunctionInfov4("ClearDrawList", 1),
    FunctionInfov4("AddDrawListEntityRef", 2),
    FunctionInfov4("GetDrawListEntityRef", 3),
    FunctionInfov4("SetDrawListEntityRef", 3),

    FunctionInfov4("Get16x16TileInfo", 4),
    FunctionInfov4("Set16x16TileInfo", 4),
    FunctionInfov4("Copy16x16Tile", 2),
    FunctionInfov4("GetAnimationByName", 2),
    FunctionInfov4("ReadSaveRAM", 0),
    FunctionInfov4("WriteSaveRAM", 0),

    FunctionInfov4("LoadTextFile", 2),
    FunctionInfov4("GetTextInfo", 5),
    FunctionInfov4("GetVersionNumber", 2),

    FunctionInfov4("GetTableValue", 3),
    FunctionInfov4("SetTableValue", 3),

    FunctionInfov4("CheckCurrentStageFolder", 1),
    FunctionInfov4("Abs", 1),

    FunctionInfov4("CallNativeFunction", 1),
    FunctionInfov4("CallNativeFunction2", 3),
    FunctionInfov4("CallNativeFunction4", 5),

    FunctionInfov4("SetObjectRange", 1),
    FunctionInfov4("GetObjectValue", 3),
    FunctionInfov4("SetObjectValue", 3),
    FunctionInfov4("CopyObject", 3),
    FunctionInfov4("Print", 3),

    // EDITOR-ONLY
    FunctionInfov4("AddEditorVariable", 1),
    FunctionInfov4("SetActiveVariable", 1),
    FunctionInfov4("AddEnumVariable", 2),
    FunctionInfov4("SetVariableAlias", 2),
    FunctionInfov4("DrawLine", 7),
    FunctionInfov4("DrawArrow", 7),
};

AliasInfov4 publicAliases[ALIAS_COUNT_v4] = {
    AliasInfov4("true", "1"),
    AliasInfov4("false", "0"),
    AliasInfov4("FX_SCALE", "0"),
    AliasInfov4("FX_ROTATE", "1"),
    AliasInfov4("FX_ROTOZOOM", "2"),
    AliasInfov4("FX_INK", "3"),
    AliasInfov4("PRESENTATION_STAGE", "0"),
    AliasInfov4("REGULAR_STAGE", "1"),
    AliasInfov4("BONUS_STAGE", "2"),
    AliasInfov4("SPECIAL_STAGE", "3"),
    AliasInfov4("MENU_1", "0"),
    AliasInfov4("MENU_2", "1"),
    AliasInfov4("C_TOUCH", "0"),
    AliasInfov4("C_BOX", "1"),
    AliasInfov4("C_BOX2", "2"),
    AliasInfov4("C_PLATFORM", "3"),
    AliasInfov4("MAT_WORLD", "0"),
    AliasInfov4("MAT_VIEW", "1"),
    AliasInfov4("MAT_TEMP", "2"),
    AliasInfov4("FX_FLIP", "5"),
    AliasInfov4("FACING_LEFT", "1"),
    AliasInfov4("FACING_RIGHT", "0"),
    AliasInfov4("FLIP_NONE", "0"),
    AliasInfov4("FLIP_X", "1"),
    AliasInfov4("FLIP_Y", "2"),
    AliasInfov4("FLIP_XY", "3"),
    AliasInfov4("STAGE_RUNNING", "1"),
    AliasInfov4("STAGE_PAUSED", "2"),
    AliasInfov4("STAGE_FROZEN", "3"),
    AliasInfov4("STAGE_2P", "4"),
    AliasInfov4("ENGINE_DEVMENU", "0"),
    AliasInfov4("ENGINE_MAINGAME", "1"),
    AliasInfov4("ENGINE_INITDEVMENU", "2"),
    AliasInfov4("ENGINE_WAIT", "3"),
    AliasInfov4("ENGINE_SCRIPTERROR", "4"),
    AliasInfov4("ENGINE_INITPAUSE", "5"),
    AliasInfov4("ENGINE_EXITPAUSE", "6"),
    AliasInfov4("ENGINE_ENDGAME", "7"),
    AliasInfov4("ENGINE_RESETGAME", "8"),
    AliasInfov4("RESET_GAME", "2"),
    AliasInfov4("RETRO_STANDARD", "0"),
    AliasInfov4("RETRO_MOBILE", "1"),
    AliasInfov4("INK_NONE", "0"),
    AliasInfov4("INK_BLEND", "1"),
    AliasInfov4("INK_ALPHA", "2"),
    AliasInfov4("INK_ADD", "3"),
    AliasInfov4("INK_SUB", "4"),
    AliasInfov4("CSIDE_FLOOR", "0"),
    AliasInfov4("CSIDE_LWALL", "1"),
    AliasInfov4("CSIDE_RWALL", "2"),
    AliasInfov4("CSIDE_ROOF", "3"),
    AliasInfov4("CMODE_FLOOR", "0"),
    AliasInfov4("CMODE_LWALL", "1"),
    AliasInfov4("CMODE_ROOF", "2"),
    AliasInfov4("CMODE_RWALL", "3"),
    AliasInfov4("COL_NONE", "0"),
    AliasInfov4("COL_TOP", "1"),
    AliasInfov4("COL_LEFT", "2"),
    AliasInfov4("COL_RIGHT", "3"),
    AliasInfov4("COL_BOTTOM", "4"),
    AliasInfov4("PATH_A", "0"),
    AliasInfov4("PATH_B", "1"),
    AliasInfov4("GRAVITY_GROUND", "0"),
    AliasInfov4("GRAVITY_AIR", "1"),
    AliasInfov4("FACE_TEXTURED_3D", "0"),
    AliasInfov4("FACE_TEXTURED_2D", "1"),
    AliasInfov4("FACE_COLOURED_3D", "2"),
    AliasInfov4("FACE_COLOURED_2D", "3"),
    AliasInfov4("FACE_FADED", "4"),
    AliasInfov4("FACE_TEXTURED_C", "5"),
    AliasInfov4("FACE_TEXTURED_C_BLEND", "6"),
    AliasInfov4("FACE_SPRITE_3D", "7"),
    AliasInfov4("PRIORITY_ACTIVE_BOUNDS", "0"),
    AliasInfov4("PRIORITY_ACTIVE", "1"),
    AliasInfov4("PRIORITY_ACTIVE_PAUSED", "2"),
    AliasInfov4("PRIORITY_XBOUNDS", "3"),
    AliasInfov4("PRIORITY_XBOUNDS_DESTROY", "4"),
    AliasInfov4("PRIORITY_INACTIVE", "5"),
    AliasInfov4("PRIORITY_BOUNDS_SMALL", "6"),
    AliasInfov4("PRIORITY_ACTIVE_SMALL", "7"),
    AliasInfov4("CONTROLMODE_NONE", "-1"),
    AliasInfov4("CONTROLMODE_NORMAL", "0"),
    AliasInfov4("CAMERASTYLE_FOLLOW", "0"),
    AliasInfov4("CAMERASTYLE_EXTENDED", "1"),
    AliasInfov4("CAMERASTYLE_EXTENDED_OFFSET_L", "2"),
    AliasInfov4("CAMERASTYLE_EXTENDED_OFFSET_R", "3"),
    AliasInfov4("CAMERASTYLE_HLOCKED", "4"),
    AliasInfov4("TILEINFO_INDEX", "0"),
    AliasInfov4("TILEINFO_DIRECTION", "1"),
    AliasInfov4("TILEINFO_VISUALPLANE", "2"),
    AliasInfov4("TILEINFO_SOLIDITYA", "3"),
    AliasInfov4("TILEINFO_SOLIDITYB", "4"),
    AliasInfov4("TILEINFO_FLAGSA", "5"),
    AliasInfov4("TILEINFO_ANGLEA", "6"),
    AliasInfov4("TILEINFO_FLAGSB", "7"),
    AliasInfov4("TILEINFO_ANGLEB", "8"),
    AliasInfov4("TEXTINFO_TEXTDATA", "0"),
    AliasInfov4("TEXTINFO_TEXTSIZE", "1"),
    AliasInfov4("TEXTINFO_ROWCOUNT", "2"),
    AliasInfov4("TILELAYER_NOSCROLL", "0"),
    AliasInfov4("TILELAYER_HSCROLL", "1"),
    AliasInfov4("TILELAYER_VSCROLL", "2"),
    AliasInfov4("TILELAYER_3DFLOOR", "3"),
    AliasInfov4("TILELAYER_3DSKY", "4"),
    AliasInfov4("GROUP_ALL", "0"),
    // EDITOR-ONLY
    AliasInfov4("EDIT_VAR_PROPVAL", "-1"),
    AliasInfov4("ALIAS_VAR_PROPVAL", "0"),
    AliasInfov4("ALIAS_VAR_VAL0", "1"),
    AliasInfov4("ALIAS_VAR_VAL1", "2"),
    AliasInfov4("ALIAS_VAR_VAL2", "3"),
    AliasInfov4("ALIAS_VAR_VAL3", "4"),
};
AliasInfov4 privateAliases[ALIAS_COUNT_TRIM_v4];

#define TABLE_COUNT       (0x200)
#define TABLE_ENTRY_COUNT (0x400)
#define STATICVAR_COUNT   (0x200)

struct StaticInfo {
    StaticInfo()
    {
        name    = "";
        value   = 0;
        dataPos = SCRIPTDATA_COUNT_v4 - 1;
    }

    QString name;
    int value   = 0;
    int dataPos = 0;
};

struct TableValueInfo {
    TableValueInfo()
    {
        value   = 0;
        dataPos = SCRIPTDATA_COUNT_v4 - 1;
    }

    int value   = 0;
    int dataPos = 0;
};

struct TableInfo {
    TableInfo()
    {
        name       = "";
        valueCount = 0;
        dataPos    = SCRIPTDATA_COUNT_v4 - 1;
    }

    QString name;
    int valueCount;
    TableValueInfo values[TABLE_ENTRY_COUNT];
    int dataPos;
};

StaticInfo publicStaticVariables[STATICVAR_COUNT];
StaticInfo privateStaticVariables[STATICVAR_COUNT];

TableInfo *currentTable = NULL;
bool curTablePublic     = false;

TableInfo publicTables[TABLE_COUNT];
TableInfo privateTables[TABLE_COUNT];

const QString scriptEvaluationTokens[] = { "=",  "+=", "-=", "++", "--", "*=", "/=", ">>=", "<<=", "&=",
                                           "|=", "^=", "%=", "==", ">",  ">=", "<",  "<=",  "!=" };

enum ScriptReadModes {
    READMODE_NORMAL      = 0,
    READMODE_STRING      = 1,
    READMODE_COMMENTLINE = 2,
    READMODE_ENDLINE     = 3,
    READMODE_EOF         = 4
};
enum ScriptParseModes {
    PARSEMODE_SCOPELESS    = 0,
    PARSEMODE_PLATFORMSKIP = 1,
    PARSEMODE_FUNCTION     = 2,
    PARSEMODE_SWITCHREAD   = 3,
    PARSEMODE_TABLEREAD    = 4,
    PARSEMODE_ERROR        = 0xFF
};

enum ScriptVarTypes { SCRIPTVAR_VAR = 1, SCRIPTVAR_INTCONST = 2, SCRIPTVAR_STRCONST = 3 };
enum ScriptVarArrTypes {
    VARARR_NONE        = 0,
    VARARR_ARRAY       = 1,
    VARARR_ENTNOPLUS1  = 2,
    VARARR_ENTNOMINUS1 = 3
};

enum ScrVar {
    VAR_TEMP0,
    VAR_TEMP1,
    VAR_TEMP2,
    VAR_TEMP3,
    VAR_TEMP4,
    VAR_TEMP5,
    VAR_TEMP6,
    VAR_TEMP7,
    VAR_CHECKRESULT,
    VAR_ARRAYPOS0,
    VAR_ARRAYPOS1,
    VAR_ARRAYPOS2,
    VAR_ARRAYPOS3,
    VAR_ARRAYPOS4,
    VAR_ARRAYPOS5,
    VAR_ARRAYPOS6,
    VAR_ARRAYPOS7,
    VAR_GLOBAL,
    VAR_LOCAL,
    VAR_OBJECTENTITYPOS,
    VAR_OBJECTGROUPID,
    VAR_OBJECTTYPE,
    VAR_OBJECTPROPERTYVALUE,
    VAR_OBJECTXPOS,
    VAR_OBJECTYPOS,
    VAR_OBJECTIXPOS,
    VAR_OBJECTIYPOS,
    VAR_OBJECTXVEL,
    VAR_OBJECTYVEL,
    VAR_OBJECTSPEED,
    VAR_OBJECTSTATE,
    VAR_OBJECTROTATION,
    VAR_OBJECTSCALE,
    VAR_OBJECTPRIORITY,
    VAR_OBJECTDRAWORDER,
    VAR_OBJECTDIRECTION,
    VAR_OBJECTINKEFFECT,
    VAR_OBJECTALPHA,
    VAR_OBJECTFRAME,
    VAR_OBJECTANIMATION,
    VAR_OBJECTPREVANIMATION,
    VAR_OBJECTANIMATIONSPEED,
    VAR_OBJECTANIMATIONTIMER,
    VAR_OBJECTANGLE,
    VAR_OBJECTLOOKPOSX,
    VAR_OBJECTLOOKPOSY,
    VAR_OBJECTCOLLISIONMODE,
    VAR_OBJECTCOLLISIONPLANE,
    VAR_OBJECTCONTROLMODE,
    VAR_OBJECTCONTROLLOCK,
    VAR_OBJECTPUSHING,
    VAR_OBJECTVISIBLE,
    VAR_OBJECTTILECOLLISIONS,
    VAR_OBJECTINTERACTION,
    VAR_OBJECTGRAVITY,
    VAR_OBJECTUP,
    VAR_OBJECTDOWN,
    VAR_OBJECTLEFT,
    VAR_OBJECTRIGHT,
    VAR_OBJECTJUMPPRESS,
    VAR_OBJECTJUMPHOLD,
    VAR_OBJECTSCROLLTRACKING,
    VAR_OBJECTFLOORSENSORL,
    VAR_OBJECTFLOORSENSORC,
    VAR_OBJECTFLOORSENSORR,
    VAR_OBJECTFLOORSENSORLC,
    VAR_OBJECTFLOORSENSORRC,
    VAR_OBJECTCOLLISIONLEFT,
    VAR_OBJECTCOLLISIONTOP,
    VAR_OBJECTCOLLISIONRIGHT,
    VAR_OBJECTCOLLISIONBOTTOM,
    VAR_OBJECTOUTOFBOUNDS,
    VAR_OBJECTSPRITESHEET,
    VAR_OBJECTVALUE0,
    VAR_OBJECTVALUE1,
    VAR_OBJECTVALUE2,
    VAR_OBJECTVALUE3,
    VAR_OBJECTVALUE4,
    VAR_OBJECTVALUE5,
    VAR_OBJECTVALUE6,
    VAR_OBJECTVALUE7,
    VAR_OBJECTVALUE8,
    VAR_OBJECTVALUE9,
    VAR_OBJECTVALUE10,
    VAR_OBJECTVALUE11,
    VAR_OBJECTVALUE12,
    VAR_OBJECTVALUE13,
    VAR_OBJECTVALUE14,
    VAR_OBJECTVALUE15,
    VAR_OBJECTVALUE16,
    VAR_OBJECTVALUE17,
    VAR_OBJECTVALUE18,
    VAR_OBJECTVALUE19,
    VAR_OBJECTVALUE20,
    VAR_OBJECTVALUE21,
    VAR_OBJECTVALUE22,
    VAR_OBJECTVALUE23,
    VAR_OBJECTVALUE24,
    VAR_OBJECTVALUE25,
    VAR_OBJECTVALUE26,
    VAR_OBJECTVALUE27,
    VAR_OBJECTVALUE28,
    VAR_OBJECTVALUE29,
    VAR_OBJECTVALUE30,
    VAR_OBJECTVALUE31,
    VAR_OBJECTVALUE32,
    VAR_OBJECTVALUE33,
    VAR_OBJECTVALUE34,
    VAR_OBJECTVALUE35,
    VAR_OBJECTVALUE36,
    VAR_OBJECTVALUE37,
    VAR_OBJECTVALUE38,
    VAR_OBJECTVALUE39,
    VAR_OBJECTVALUE40,
    VAR_OBJECTVALUE41,
    VAR_OBJECTVALUE42,
    VAR_OBJECTVALUE43,
    VAR_OBJECTVALUE44,
    VAR_OBJECTVALUE45,
    VAR_OBJECTVALUE46,
    VAR_OBJECTVALUE47,
    VAR_STAGESTATE,
    VAR_STAGEACTIVELIST,
    VAR_STAGELISTPOS,
    VAR_STAGETIMEENABLED,
    VAR_STAGEMILLISECONDS,
    VAR_STAGESECONDS,
    VAR_STAGEMINUTES,
    VAR_STAGEACTNUM,
    VAR_STAGEPAUSEENABLED,
    VAR_STAGELISTSIZE,
    VAR_STAGENEWXBOUNDARY1,
    VAR_STAGENEWXBOUNDARY2,
    VAR_STAGENEWYBOUNDARY1,
    VAR_STAGENEWYBOUNDARY2,
    VAR_STAGECURXBOUNDARY1,
    VAR_STAGECURXBOUNDARY2,
    VAR_STAGECURYBOUNDARY1,
    VAR_STAGECURYBOUNDARY2,
    VAR_STAGEDEFORMATIONDATA0,
    VAR_STAGEDEFORMATIONDATA1,
    VAR_STAGEDEFORMATIONDATA2,
    VAR_STAGEDEFORMATIONDATA3,
    VAR_STAGEWATERLEVEL,
    VAR_STAGEACTIVELAYER,
    VAR_STAGEMIDPOINT,
    VAR_STAGEPLAYERLISTPOS,
    VAR_STAGEDEBUGMODE,
    VAR_STAGEENTITYPOS,
    VAR_SCREENCAMERAENABLED,
    VAR_SCREENCAMERATARGET,
    VAR_SCREENCAMERASTYLE,
    VAR_SCREENCAMERAX,
    VAR_SCREENCAMERAY,
    VAR_SCREENDRAWLISTSIZE,
    VAR_SCREENXCENTER,
    VAR_SCREENYCENTER,
    VAR_SCREENXSIZE,
    VAR_SCREENYSIZE,
    VAR_SCREENXOFFSET,
    VAR_SCREENYOFFSET,
    VAR_SCREENSHAKEX,
    VAR_SCREENSHAKEY,
    VAR_SCREENADJUSTCAMERAY,
    VAR_TOUCHSCREENDOWN,
    VAR_TOUCHSCREENXPOS,
    VAR_TOUCHSCREENYPOS,
    VAR_MUSICVOLUME,
    VAR_MUSICCURRENTTRACK,
    VAR_MUSICPOSITION,
    VAR_INPUTDOWNUP,
    VAR_INPUTDOWNDOWN,
    VAR_INPUTDOWNLEFT,
    VAR_INPUTDOWNRIGHT,
    VAR_INPUTDOWNBUTTONA,
    VAR_INPUTDOWNBUTTONB,
    VAR_INPUTDOWNBUTTONC,
    VAR_INPUTDOWNBUTTONX,
    VAR_INPUTDOWNBUTTONY,
    VAR_INPUTDOWNBUTTONZ,
    VAR_INPUTDOWNBUTTONL,
    VAR_INPUTDOWNBUTTONR,
    VAR_INPUTDOWNSTART,
    VAR_INPUTDOWNSELECT,
    VAR_INPUTPRESSUP,
    VAR_INPUTPRESSDOWN,
    VAR_INPUTPRESSLEFT,
    VAR_INPUTPRESSRIGHT,
    VAR_INPUTPRESSBUTTONA,
    VAR_INPUTPRESSBUTTONB,
    VAR_INPUTPRESSBUTTONC,
    VAR_INPUTPRESSBUTTONX,
    VAR_INPUTPRESSBUTTONY,
    VAR_INPUTPRESSBUTTONZ,
    VAR_INPUTPRESSBUTTONL,
    VAR_INPUTPRESSBUTTONR,
    VAR_INPUTPRESSSTART,
    VAR_INPUTPRESSSELECT,
    VAR_MENU1SELECTION,
    VAR_MENU2SELECTION,
    VAR_TILELAYERXSIZE,
    VAR_TILELAYERYSIZE,
    VAR_TILELAYERTYPE,
    VAR_TILELAYERANGLE,
    VAR_TILELAYERXPOS,
    VAR_TILELAYERYPOS,
    VAR_TILELAYERZPOS,
    VAR_TILELAYERPARALLAXFACTOR,
    VAR_TILELAYERSCROLLSPEED,
    VAR_TILELAYERSCROLLPOS,
    VAR_TILELAYERDEFORMATIONOFFSET,
    VAR_TILELAYERDEFORMATIONOFFSETW,
    VAR_HPARALLAXPARALLAXFACTOR,
    VAR_HPARALLAXSCROLLSPEED,
    VAR_HPARALLAXSCROLLPOS,
    VAR_VPARALLAXPARALLAXFACTOR,
    VAR_VPARALLAXSCROLLSPEED,
    VAR_VPARALLAXSCROLLPOS,
    VAR_SCENE3DVERTEXCOUNT,
    VAR_SCENE3DFACECOUNT,
    VAR_SCENE3DPROJECTIONX,
    VAR_SCENE3DPROJECTIONY,
    VAR_SCENE3DFOGCOLOR,
    VAR_SCENE3DFOGSTRENGTH,
    VAR_VERTEXBUFFERX,
    VAR_VERTEXBUFFERY,
    VAR_VERTEXBUFFERZ,
    VAR_VERTEXBUFFERU,
    VAR_VERTEXBUFFERV,
    VAR_FACEBUFFERA,
    VAR_FACEBUFFERB,
    VAR_FACEBUFFERC,
    VAR_FACEBUFFERD,
    VAR_FACEBUFFERFLAG,
    VAR_FACEBUFFERCOLOR,
    VAR_SAVERAM,
    VAR_ENGINESTATE,
    VAR_ENGINELANGUAGE,
    VAR_ENGINEONLINEACTIVE,
    VAR_ENGINESFXVOLUME,
    VAR_ENGINEBGMVOLUME,
    VAR_ENGINETRIALMODE,
    VAR_ENGINEDEVICETYPE,
    // EDITOR-ONLY
    VAR_EDITORVARIABLEID,
    VAR_EDITORVARIABLEVAL,
    VAR_EDITORRETURNVAR,
    VAR_EDITORSHOWGIZMOS,
    VAR_MAX_CNT
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
    FUNC_AND,
    FUNC_OR,
    FUNC_XOR,
    FUNC_MOD,
    FUNC_FLIPSIGN,
    FUNC_CHECKEQUAL,
    FUNC_CHECKGREATER,
    FUNC_CHECKLOWER,
    FUNC_CHECKNOTEQUAL,
    FUNC_IFEQUAL,
    FUNC_IFGREATER,
    FUNC_IFGREATEROREQUAL,
    FUNC_IFLOWER,
    FUNC_IFLOWEROREQUAL,
    FUNC_IFNOTEQUAL,
    FUNC_ELSE,
    FUNC_ENDIF,
    FUNC_WEQUAL,
    FUNC_WGREATER,
    FUNC_WGREATEROREQUAL,
    FUNC_WLOWER,
    FUNC_WLOWEROREQUAL,
    FUNC_WNOTEQUAL,
    FUNC_LOOP,
    FUNC_FOREACHACTIVE,
    FUNC_FOREACHALL,
    FUNC_NEXT,
    FUNC_SWITCH,
    FUNC_BREAK,
    FUNC_ENDSWITCH,
    FUNC_RAND,
    FUNC_SIN,
    FUNC_COS,
    FUNC_SIN256,
    FUNC_COS256,
    FUNC_ATAN2,
    FUNC_INTERPOLATE,
    FUNC_INTERPOLATEXY,
    FUNC_LOADSPRITESHEET,
    FUNC_REMOVESPRITESHEET,
    FUNC_DRAWSPRITE,
    FUNC_DRAWSPRITEXY,
    FUNC_DRAWSPRITESCREENXY,
    FUNC_DRAWTINTRECT,
    FUNC_DRAWNUMBERS,
    FUNC_DRAWACTNAME,
    FUNC_DRAWMENU,
    FUNC_SPRITEFRAME,
    FUNC_EDITFRAME,
    FUNC_LOADPALETTE,
    FUNC_ROTATEPALETTE,
    FUNC_SETSCREENFADE,
    FUNC_SETACTIVEPALETTE,
    FUNC_SETPALETTEFADE,
    FUNC_SETPALETTEENTRY,
    FUNC_GETPALETTEENTRY,
    FUNC_COPYPALETTE,
    FUNC_CLEARSCREEN,
    FUNC_DRAWSPRITEFX,
    FUNC_DRAWSPRITESCREENFX,
    FUNC_LOADANIMATION,
    FUNC_SETUPMENU,
    FUNC_ADDMENUENTRY,
    FUNC_EDITMENUENTRY,
    FUNC_LOADSTAGE,
    FUNC_DRAWRECT,
    FUNC_RESETOBJECTENTITY,
    FUNC_BOXCOLLISIONTEST,
    FUNC_CREATETEMPOBJECT,
    FUNC_PROCESSOBJECTMOVEMENT,
    FUNC_PROCESSOBJECTCONTROL,
    FUNC_PROCESSANIMATION,
    FUNC_DRAWOBJECTANIMATION,
    FUNC_SETMUSICTRACK,
    FUNC_PLAYMUSIC,
    FUNC_STOPMUSIC,
    FUNC_PAUSEMUSIC,
    FUNC_RESUMEMUSIC,
    FUNC_SWAPMUSICTRACK,
    FUNC_PLAYSFX,
    FUNC_STOPSFX,
    FUNC_SETSFXATTRIBUTES,
    FUNC_OBJECTTILECOLLISION,
    FUNC_OBJECTTILEGRIP,
    FUNC_NOT,
    FUNC_DRAW3DSCENE,
    FUNC_SETIDENTITYMATRIX,
    FUNC_MATRIXMULTIPLY,
    FUNC_MATRIXTRANSLATEXYZ,
    FUNC_MATRIXSCALEXYZ,
    FUNC_MATRIXROTATEX,
    FUNC_MATRIXROTATEY,
    FUNC_MATRIXROTATEZ,
    FUNC_MATRIXROTATEXYZ,
    FUNC_MATRIXINVERSE,
    FUNC_TRANSFORMVERTICES,
    FUNC_CALLFUNCTION,
    FUNC_RETURN,
    FUNC_SETLAYERDEFORMATION,
    FUNC_CHECKTOUCHRECT,
    FUNC_GETTILELAYERENTRY,
    FUNC_SETTILELAYERENTRY,
    FUNC_GETBIT,
    FUNC_SETBIT,
    FUNC_CLEARDRAWLIST,
    FUNC_ADDDRAWLISTENTITYREF,
    FUNC_GETDRAWLISTENTITYREF,
    FUNC_SETDRAWLISTENTITYREF,
    FUNC_GET16X16TILEINFO,
    FUNC_SET16X16TILEINFO,
    FUNC_COPY16X16TILE,
    FUNC_GETANIMATIONBYNAME,
    FUNC_READSAVERAM,
    FUNC_WRITESAVERAM,
    FUNC_LOADTEXTFILE,
    FUNC_GETTEXTINFO,
    FUNC_GETVERSIONNUMBER,
    FUNC_GETTABLEVALUE,
    FUNC_SETTABLEVALUE,
    FUNC_CHECKCURRENTSTAGEFOLDER,
    FUNC_ABS,
    FUNC_CALLNATIVEFUNCTION,
    FUNC_CALLNATIVEFUNCTION2,
    FUNC_CALLNATIVEFUNCTION4,
    FUNC_SETOBJECTRANGE,
    FUNC_GETOBJECTVALUE,
    FUNC_SETOBJECTVALUE,
    FUNC_COPYOBJECT,
    FUNC_PRINT,
    // EDITOR-ONLY
    FUNC_ADDEDITORVAR,
    FUNC_SETACTIVEVAR,
    FUNC_ADDENUMVAR,
    FUNC_SETVARALIAS,
    FUNC_DRAWLINE,
    FUNC_DRAWARROW,
    FUNC_MAX_CNT
};

Compilerv4::Compilerv4()
{
    for (int i = 0; i < 0x200; ++i) {
        float Val    = sinf(((float)i / 256) * M_PI);
        sinVal512[i] = (signed int)(Val * 512.0);
        Val          = cosf(((float)i / 256) * M_PI);
        cosVal512[i] = (signed int)(Val * 512.0);
    }

    cosVal512[0]   = 0x200;
    cosVal512[128] = 0;
    cosVal512[256] = -0x200;
    cosVal512[384] = 0;
    sinVal512[0]   = 0;
    sinVal512[128] = 0x200;
    sinVal512[256] = 0;
    sinVal512[384] = -0x200;

    for (int i = 0; i < 0x100; i++) {
        sinVal256[i] = (sinVal512[i * 2] >> 1);
        cosVal256[i] = (cosVal512[i * 2] >> 1);
    }

    for (int Y = 0; Y < 0x100; ++Y) {
        byte *ATan = (byte *)&atanVal256[Y];
        for (int X = 0; X < 0x100; ++X) {
            float angle = atan2f(Y, X);
            *ATan       = (signed int)(angle * 40.743664f);
            ATan += 0x100;
        }
    }
}

bool Compilerv4::strComp(QString strA, QString strB)
{
    if (strA.length() == 0 && strB.length() == 0)
        return true;
    if (strA.length() == 0 || strB.length() == 0 || strA.length() != strB.length())
        return false;

    bool match  = true;
    int strPosA = 0, strPosB = 0;

    while (strPosA < strA.length() && strPosB < strB.length()) {
        if (strA[strPosA] == strB[strPosB] || strA[strPosA] == strB[strPosB].toLatin1() + ' '
            || strA[strPosA] == strB[strPosB].toLatin1() - ' ') {
            ++strPosA;
            ++strPosB;
        }
        else {
            match = false;
            break;
        }
    }
    return match;
}

int Compilerv4::findStringToken(QString &string, QString token, char stopID)
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

void Compilerv4::checkAliasText(QString &text)
{
    if (findStringToken(text, "publicalias", 1) && findStringToken(text, "privatealias", 1))
        return;
    int textPos     = 11;
    int aliasStrPos = 0;
    int aliasMatch  = 0;

    if (publicAliasCount >= ALIAS_COUNT_v4) {
        if (findStringToken(text, "privatealias", 1)) // public alias & we reached the cap
            return;
    }

    AliasInfov4 *a = &publicAliases[publicAliasCount];
    int *cnt       = &publicAliasCount;
    if (findStringToken(text, "privatealias", 1) == 0) {
        a       = &privateAliases[privateAliasCount];
        cnt     = &privateAliasCount;
        textPos = 12;
        if (privateAliasCount >= ALIAS_COUNT_v4) // private alias & we reached the cap
            return;
    }
    a->name  = "";
    a->value = "";

    while (aliasMatch < 2) {
        if (aliasMatch) {
            if (aliasMatch == 1) {
                if (textPos < text.length()) {
                    a->name += text[textPos];
                    aliasStrPos++;
                }
                else {
                    aliasStrPos = 0;
                    ++aliasMatch;
                }
            }
        }
        else if (text[textPos] == ':') {
            aliasStrPos = 0;
            aliasMatch  = 1;
        }
        else {
            a->value += text[textPos];
        }
        ++textPos;
    }
    ++*cnt;
}
void Compilerv4::checkStaticText(QString &text)
{
    if (findStringToken(text, "publicvalue", 1) && findStringToken(text, "privatevalue", 1))
        return;
    int textPos       = 11;
    int staticStrPos  = 0;
    int staticMatch   = 0;
    QString strBuffer = "";

    if (publicStaticVarCount >= STATICVAR_COUNT) {
        if (findStringToken(text, "privatevalue", 1)) // public value & we reached the cap
            return;
    }

    StaticInfo *var = &publicStaticVariables[publicStaticVarCount];
    int *cnt        = &publicStaticVarCount;
    if (findStringToken(text, "privatevalue", 1) == 0) {
        if (privateStaticVarCount >= STATICVAR_COUNT) // private value and we reached the cap
            return;
        var     = &privateStaticVariables[privateStaticVarCount];
        cnt     = &privateStaticVarCount;
        textPos = 12;
    }
    var->name    = "";
    var->value   = 0;
    var->dataPos = 0;

    while (staticMatch < 2) {
        if (staticMatch == 1) {
            if (text[staticStrPos] != ';' && textPos < text.length()) {
                strBuffer += text[textPos];
            }
            else {
                convertStringToInteger(strBuffer, &var->value);

                var->dataPos                = scriptDataPos;
                scriptData[scriptDataPos++] = var->value;
                staticStrPos                = 0;
                ++staticMatch;
            }
        }
        else if (text[textPos] == '=') {
            staticStrPos = 0;
            staticMatch  = 1;
        }
        else {
            var->name += text[textPos];
        }
        ++textPos;
    }
    ++*cnt;
}
void *Compilerv4::checkTableText(QString &text)
{
    if (findStringToken(text, "publictable", 1) && findStringToken(text, "privatetable", 1))
        return NULL;

    if (publicTableCount >= TABLE_COUNT) {
        if (findStringToken(text, "privatetable", 1)) // public table & we reached the cap
            return NULL;
    }

    TableInfo *table = &publicTables[publicTableCount];
    int strPos       = 11;
    curTablePublic   = true;
    if (findStringToken(text, "privatetable", 1) == 0) {
        if (privateTableCount >= TABLE_COUNT) // private table and we reached the cap
            return NULL;
        table          = &privateTables[privateTableCount];
        strPos         = 12;
        curTablePublic = false;
    }
    table->name       = "";
    table->valueCount = 0;
    table->dataPos    = 0;

    while (strPos < text.length()) {
        table->name += text[strPos++];
    }
    return table;
}
void Compilerv4::convertArithmaticSyntax(QString &text)
{
    int token    = 0;
    int offset   = 0;
    int findID   = 0;
    QString dest = "";

    for (int i = FUNC_EQUAL; i <= FUNC_MOD; ++i) {
        findID = findStringToken(text, scriptEvaluationTokens[i - 1], 1);
        if (findID > -1) {
            offset = findID;
            token  = i;
        }
    }

    if (token > 0) {
        dest = functionsv4[token].name + "(";
        for (int i = 0; i < offset; ++i) dest += text[i];
        if (functionsv4[token].opcodeSize > 1) {
            dest += ',';
            offset += scriptEvaluationTokens[token - 1].length();
            ++findID;
            while (offset < text.length()) dest += text[offset++];
        }

        dest = dest + ")";
        text = dest;
    }
}
void Compilerv4::convertIfWhileStatement(QString &text)
{
    QString dest  = "";
    int compareOp = -1;
    int strPos    = 0;
    if (findStringToken(text, "if", 1)) {
        if (!findStringToken(text, "while", 1)) {
            for (int i = 0; i < 6; ++i) {
                int tPos = findStringToken(text, scriptEvaluationTokens[i + FUNC_MOD], 1);
                if (tPos > -1) {
                    strPos    = tPos;
                    compareOp = i;
                }
            }
            if (compareOp > -1) {
                text.replace(strPos, 1, ',');
                dest = functionsv4[compareOp + FUNC_WEQUAL].name + "(";
                appendIntegerToString(dest, jumpTableDataPos - jumpTableDataOffset);
                dest += ",";
                for (int i = 5; i < text.length(); ++i) {
                    if (text[i] != '=' && text[i] != '(' && text[i] != ')')
                        dest += text[i];
                }
                dest += ")";
                text                                = dest;
                jumpTableStack[++jumpTableStackPos] = jumpTableDataPos;
                jumpTableData[jumpTableDataPos++]   = scriptDataPos - scriptDataOffset;
                jumpTableData[jumpTableDataPos++]   = 0;
            }
        }
    }
    else {
        for (int i = 0; i < 6; ++i) {
            int tPos = findStringToken(text, scriptEvaluationTokens[i + FUNC_MOD], 1);
            if (tPos > -1) {
                strPos    = tPos;
                compareOp = i;
            }
        }
        if (compareOp > -1) {
            text.replace(strPos, 1, ',');
            dest = functionsv4[compareOp + FUNC_IFEQUAL].name + "(";
            appendIntegerToString(dest, jumpTableDataPos - jumpTableDataOffset);
            dest += ",";
            for (int i = 2; i < text.length(); ++i) {
                if (text[i] != '=' && text[i] != '(' && text[i] != ')')
                    dest += text[i];
            }
            dest += ")";
            text                                = dest;
            jumpTableStack[++jumpTableStackPos] = jumpTableDataPos;
            jumpTableData[jumpTableDataPos++]   = -1;
            jumpTableData[jumpTableDataPos++]   = 0;
        }
    }
}
void Compilerv4::convertForeachStatement(QString &text)
{
    if (findStringToken(text, "foreach", 1))
        return;
    QString dest   = "";
    int destStrPos = 0;

    if (findStringToken(text, "ACTIVE_ENTITIES", 1)
        > 0) { // foreach (just actively interacting entities)
        dest = functionsv4[FUNC_FOREACHACTIVE].name;
        dest += "(";
        dest += QString::number(jumpTableDataPos - jumpTableDataOffset);
        dest += ",";
        destStrPos = dest.length();
        int cnt    = 0;
        for (int i = 8; i < text.length(); ++i) {
            if (text[i] != '(' && text[i] != ')' && text[i] != ',') {
                dest += text[i];
            }
            else if (text[i] == ',') {
                if (!cnt)
                    dest += text[i];
                else
                    break;
                ++cnt;
            }
        }
        dest += ")";
        text                                = dest;
        jumpTableStack[++jumpTableStackPos] = jumpTableDataPos;
        jumpTableData[jumpTableDataPos++]   = scriptDataPos - scriptDataOffset;
        jumpTableData[jumpTableDataPos++]   = 0;
    }
    else if (findStringToken(text, "ALL_ENTITIES", 1) > 0) { // foreach (all entities)
        dest = functionsv4[FUNC_FOREACHALL].name;
        dest += "(";
        dest += QString::number(jumpTableDataPos - jumpTableDataOffset);
        dest += ",";
        destStrPos = dest.length();
        int cnt    = 0;
        for (int i = 8; i < text.length(); ++i) {
            if (text[i] != '(' && text[i] != ')' && text[i] != ',') {
                dest += text[i];
            }
            else if (text[i] == ',') {
                if (!cnt)
                    dest += text[i];
                else
                    break;
                ++cnt;
            }
        }
        dest += ")";
        text                                = dest;
        jumpTableStack[++jumpTableStackPos] = jumpTableDataPos;
        jumpTableData[jumpTableDataPos++]   = scriptDataPos - scriptDataOffset;
        jumpTableData[jumpTableDataPos++]   = 0;
    }
}
bool Compilerv4::convertSwitchStatement(QString &text)
{
    if (findStringToken(text, "switch", 1))
        return false;
    QString switchText = "";
    switchText         = "switch(";
    appendIntegerToString(switchText, jumpTableDataPos - jumpTableDataOffset);
    switchText += ",";
    for (int i = 6; i < text.length(); ++i) {
        if (text[i] != '=' && text[i] != '(' && text[i] != ')')
            switchText += text[i];
    }
    switchText += ")";
    text                                = switchText;
    jumpTableStack[++jumpTableStackPos] = jumpTableDataPos;
    jumpTableData[jumpTableDataPos++]   = 0x10000;
    jumpTableData[jumpTableDataPos++]   = -0x10000;
    jumpTableData[jumpTableDataPos++]   = -1;
    jumpTableData[jumpTableDataPos++]   = 0;
    return true;
}
void Compilerv4::convertFunctionText(QString &text)
{
    QString arrayStr = "";
    QString funcName = "";
    int opcode       = 0;
    int opcodeSize   = 0;
    int textPos      = 0;
    int namePos      = 0;
    for (namePos = 0; namePos < text.length(); ++namePos) {
        if (text[namePos] == '(')
            break;

        funcName += text[namePos];
    }

    for (int i = 0; i < ScrFunc::FUNC_MAX_CNT; ++i) {
        if (strComp(funcName, functionsv4[i].name)) {
            opcode     = i;
            opcodeSize = functionsv4[i].opcodeSize;
            textPos    = functionsv4[i].name.length();
            i          = FUNC_MAX_CNT;
        }
    }

    if (opcode <= 0) {
        scriptError = true;
        errorMsg    = "OPCODE NOT FOUND";
        errorPos    = funcName;
        errorLine   = lineID;
    }
    else {
        scriptData[scriptDataPos++] = opcode;
        if (strComp(functionsv4[opcode].name, "else"))
            jumpTableData[jumpTableStack[jumpTableStackPos]] = scriptDataPos - scriptDataOffset;

        if (strComp(functionsv4[opcode].name, "endif")) {
            int jPos                = jumpTableStack[jumpTableStackPos];
            jumpTableData[jPos + 1] = scriptDataPos - scriptDataOffset;
            if (jumpTableData[jPos] == -1)
                jumpTableData[jPos] = (scriptDataPos - scriptDataOffset) - 1;
            --jumpTableStackPos;
        }

        if (strComp(functionsv4[opcode].name, "endswitch")) {
            int jPos                = jumpTableStack[jumpTableStackPos];
            jumpTableData[jPos + 3] = scriptDataPos - scriptDataOffset;
            if (jumpTableData[jPos + 2] == -1) {
                jumpTableData[jPos + 2] = (scriptDataPos - scriptDataOffset) - 1;
                int caseCnt             = abs(jumpTableData[jPos + 1] - jumpTableData[jPos]) + 1;

                int jOffset = jPos + 4;
                for (int c = 0; c < caseCnt; ++c) {
                    if (jumpTableData[jOffset + c] < 0)
                        jumpTableData[jOffset + c] = jumpTableData[jPos + 2];
                }
            }
            --jumpTableStackPos;
        }

        if (strComp(functionsv4[opcode].name, "loop") || strComp(functionsv4[opcode].name, "next")) {
            jumpTableData[jumpTableStack[jumpTableStackPos--] + 1] = scriptDataPos - scriptDataOffset;
        }

        for (int i = 0; i < opcodeSize; ++i) {
            ++textPos;
            int mode     = 0;
            int prevMode = 0;
            funcName     = "";
            arrayStr     = "";
            while (((text[textPos] != ',' && text[textPos] != ')') || mode == 2)
                   && textPos < text.length()) {
                switch (mode) {
                    case 0: // normal
                        if (text[textPos] == '[')
                            mode = 1;
                        else if (text[textPos] == '"') {
                            prevMode = mode;
                            mode     = 2;
                            funcName += '"';
                        }
                        else
                            funcName += text[textPos];
                        ++textPos;
                        break;
                    case 1: // array val
                        if (text[textPos] == ']')
                            mode = 0;
                        else if (text[textPos] == '"') {
                            prevMode = mode;
                            mode     = 2;
                        }
                        else
                            arrayStr += text[textPos];
                        ++textPos;
                        break;
                    case 2: // string
                        if (text[textPos] == '"') {
                            mode = prevMode;
                            funcName += '"';
                        }
                        else
                            funcName += text[textPos];
                        ++textPos;
                        break;
                }
            }

            int value = 0;
            // Eg: temp0 = FX_SCALE
            // Private (this script only)
            for (int a = 0; a < privateAliasCount; ++a) {
                if (strComp(funcName, privateAliases[a].name)) {
                    copyAliasStr(funcName, privateAliases[a].value, 0);
                    if (findStringToken(privateAliases[a].value, "[", 1) > -1)
                        copyAliasStr(arrayStr, privateAliases[a].value, 1);
                }
            }
            // Private (this script & all following scripts)
            for (int a = 0; a < publicAliasCount; ++a) {
                if (strComp(funcName, publicAliases[a].name)) {
                    copyAliasStr(funcName, publicAliases[a].value, 0);
                    if (findStringToken(publicAliases[a].value, "[", 1) > -1)
                        copyAliasStr(arrayStr, publicAliases[a].value, 1);
                }
            }

            // Aliases (array value)
            char prefix = 0;
            if (arrayStr.length() > 0) {
                if (arrayStr[0] == '+' || arrayStr[0] == '-') {
                    prefix = arrayStr[0].toLatin1();
                    arrayStr.remove(0, 1);
                }

                // Private (this script only)
                for (int a = 0; a < privateAliasCount; ++a) {
                    if (strComp(arrayStr, privateAliases[a].name)) {
                        copyAliasStr(arrayStr, privateAliases[a].value, 0);
                    }
                }
                // Private (this script & all following scripts)
                for (int a = 0; a < publicAliasCount; ++a) {
                    if (strComp(arrayStr, publicAliases[a].name)) {
                        copyAliasStr(arrayStr, publicAliases[a].value, 0);
                    }
                }
            }

            // Eg: temp0 = value0
            // Private (this script only)
            for (int s = 0; s < privateStaticVarCount; ++s) {
                if (strComp(funcName, privateStaticVariables[s].name)) {
                    funcName = "local";
                    arrayStr = QString::number(privateStaticVariables[s].dataPos);
                }
            }
            // Private (this script & all following scripts)
            for (int s = 0; s < publicStaticVarCount; ++s) {
                if (strComp(funcName, publicStaticVariables[s].name)) {
                    funcName = "local";
                    arrayStr = QString::number(publicStaticVariables[s].dataPos);
                }
            }

            // Eg: GetTableValue(TempValue0, 1, array0)
            // Private (this script only)
            for (int a = 0; a < privateTableCount; ++a) {
                if (strComp(funcName, privateTables[a].name)) {
                    funcName = QString::number(privateTables[a].dataPos);
                    arrayStr = "";
                }
            }
            // Private (this script & all following scripts)
            for (int a = 0; a < publicTableCount; ++a) {
                if (strComp(funcName, publicTables[a].name)) {
                    funcName = QString::number(publicTables[a].dataPos);
                    arrayStr = "";
                }
            }

            // Eg: temp0 = Game.Variable
            for (int v = 0; v < globalVariables.count(); ++v) {
                if (strComp(funcName, globalVariables[v])) {
                    funcName = "global";
                    arrayStr = "";
                    appendIntegerToString(arrayStr, v);
                }
            }
            // Eg: temp0 = Function1
            for (int f = 0; f < functionCount; ++f) {
                if (strComp(funcName, functionNames[f])) {
                    funcName = "";
                    appendIntegerToString(funcName, f);
                }
            }
            // Eg: temp0 = TypeName[PlayerObject]
            if (strComp(funcName, "TypeName")) {
                funcName = "";
                appendIntegerToString(funcName, 0);
                for (int o = 0; o < OBJECT_COUNT; ++o) {
                    if (strComp(arrayStr, typeNames[o])) {
                        funcName = "";
                        appendIntegerToString(funcName, o);
                    }
                }
            }

            // Eg: temp0 = SfxName[Jump]
            if (strComp(funcName, "SfxName")) {
                funcName = "";
                appendIntegerToString(funcName, 0);
                int s = 0;
                for (; s < SFX_COUNT; ++s) {
                    if (strComp(arrayStr, sfxNames[s])) {
                        funcName = "";
                        appendIntegerToString(funcName, s);
                        break;
                    }
                }

                if (s == SFX_COUNT) {
                    // printLog("WARNING: Unknown sfxName \"%s\", on line %d", arrayStr, lineID);
                }
            }

            // Eg: temp0 = VarName[player.lives]
            if (strComp(funcName, "VarName")) {
                funcName = "";
                appendIntegerToString(funcName, 0);
                int v = 0;
                for (; v < globalVariables.count(); ++v) {
                    if (strComp(arrayStr, globalVariables[v])) {
                        funcName = "";
                        appendIntegerToString(funcName, v);
                        break;
                    }
                }

                if (v == globalVariables.count()) {
                    // printLog("WARNING: Unknown varName \"%s\", on line %d", arrayStr, lineID);
                }
            }

            // Eg: temp0 = AchievementName[Ring King]
            if (strComp(funcName, "AchievementName")) {
                funcName = "";
                appendIntegerToString(funcName, 0);
                // default to 0, we don't know what these are
            }

            // Eg: temp0 = PlayerName[SONIC]
            if (strComp(funcName, "PlayerName")) {
                funcName = "";
                appendIntegerToString(funcName, 0);
                int p = 0;
                if (editor) {
                    for (; p < ((SceneEditor *)editor)->gameConfig.players.count(); ++p) {
                        QString name = ((SceneEditor *)editor)->gameConfig.players[p].name;
                        name         = name.replace(" ", "");

                        if (strComp(arrayStr, name)) {
                            funcName = "";
                            appendIntegerToString(funcName, p);
                            break;
                        }
                    }

                    if (p == ((SceneEditor *)editor)->gameConfig.players.count()) {
                        // printLog("WARNING: Unknown PlayerName \"%s\", on line %d", arrayStr, lineID);
                    }
                }
                else {
                    for (; p < gameConfig.players.count(); ++p) {
                        QString name = gameConfig.players[p];
                        name         = name.replace(" ", "");

                        if (strComp(arrayStr, name)) {
                            funcName = "";
                            appendIntegerToString(funcName, p);
                            break;
                        }
                    }

                    if (p == gameConfig.players.count()) {
                        printLog(QString("WARNING: Unknown PlayerName \"%1\", on line %2")
                                     .arg(arrayStr)
                                     .arg(lineID));
                    }
                }
            }

            // Eg: temp0 = StageName[GREEN HILL ZONE 1]
            if (strComp(funcName, "StageName")) {
                funcName = "";
                int s    = -1;
                if (arrayStr.length() >= 2) {
                    char list = arrayStr[0].toLatin1();
                    switch (list) {
                        default: list = 0xFF; break;
                        case 'P': list = 0; break;
                        case 'R': list = 1; break;
                        case 'S': list = 2; break;
                        case 'B': list = 3; break;
                    }

                    s = -1;
                    if (list <= 3) {
                        QString scnName = arrayStr;
                        scnName.replace(" ", "");
                        scnName = scnName.mid(2, scnName.length() - 2);

                        for (; s < gameConfig.stageLists[list].scenes.count(); ++s) {
                            QString name = gameConfig.stageLists[list].scenes[s].name;
                            name         = name.replace(" ", "");
                            if (strComp(scnName, name))
                                break;
                        }

                        if (s == gameConfig.stageLists[list].scenes.count())
                            s = -1;
                    }
                }

                if (s == -1) {
                    printLog(QString("WARNING: Unknown StageName \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
                    s = 0;
                }
                funcName = "";
                appendIntegerToString(funcName, s);
            }

            if (convertStringToInteger(funcName, &value)) {
                scriptData[scriptDataPos++] = SCRIPTVAR_INTCONST;
                scriptData[scriptDataPos++] = value;
            }
            else if (funcName[0] == '"') {
                scriptData[scriptDataPos++] = SCRIPTVAR_STRCONST;
                scriptData[scriptDataPos++] = funcName.length() - 2;
                int scriptTextPos           = 1;
                int scriptTextByteID        = 0;
                while (scriptTextPos > -1) {
                    switch (scriptTextByteID) {
                        case 0:
                            scriptData[scriptDataPos] = (byte)funcName[scriptTextPos].toLatin1() << 24;
                            ++scriptTextByteID;
                            break;
                        case 1:
                            scriptData[scriptDataPos] |= (byte)funcName[scriptTextPos].toLatin1() << 16;
                            ++scriptTextByteID;
                            break;
                        case 2:
                            scriptData[scriptDataPos] |= (byte)funcName[scriptTextPos].toLatin1() << 8;
                            ++scriptTextByteID;
                            break;
                        case 3:
                            scriptData[scriptDataPos++] |= (byte)funcName[scriptTextPos].toLatin1();
                            scriptTextByteID = 0;
                            break;
                        default: break;
                    }
                    if (funcName[scriptTextPos] == '"') {
                        if (scriptTextByteID > 0)
                            ++scriptDataPos;
                        scriptTextPos = -1;
                    }
                    else {
                        scriptTextPos++;
                    }
                }
            }
            else {
                scriptData[scriptDataPos++] = SCRIPTVAR_VAR;
                if (arrayStr.length()) {
                    scriptData[scriptDataPos] = VARARR_ARRAY;
                    if (prefix == '+')
                        scriptData[scriptDataPos] = VARARR_ENTNOPLUS1;
                    if (prefix == '-')
                        scriptData[scriptDataPos] = VARARR_ENTNOMINUS1;
                    ++scriptDataPos;

                    if (convertStringToInteger(arrayStr, &value) == 1) {
                        scriptData[scriptDataPos++] = 0;
                        scriptData[scriptDataPos++] = value;
                    }
                    else {
                        if (strComp(arrayStr, "arrayPos0"))
                            value = 0;
                        if (strComp(arrayStr, "arrayPos1"))
                            value = 1;
                        if (strComp(arrayStr, "arrayPos2"))
                            value = 2;
                        if (strComp(arrayStr, "arrayPos3"))
                            value = 3;
                        if (strComp(arrayStr, "arrayPos4"))
                            value = 4;
                        if (strComp(arrayStr, "arrayPos5"))
                            value = 5;
                        if (strComp(arrayStr, "arrayPos6"))
                            value = 6;
                        if (strComp(arrayStr, "arrayPos7"))
                            value = 7;
                        if (strComp(arrayStr, "tempObjectPos"))
                            value = 8;
                        scriptData[scriptDataPos++] = VARARR_ARRAY;
                        scriptData[scriptDataPos++] = value;
                    }
                }
                else {
                    scriptData[scriptDataPos++] = VARARR_NONE;
                }
                value = -1;
                for (int i = 0; i < VAR_MAX_CNT; ++i) {
                    if (strComp(funcName, variableNamesv4[i]))
                        value = i;
                }

                if (value == -1 && !scriptError) {
                    scriptError = true;
                    errorMsg    = "OPERAND NOT FOUND";
                    errorPos    = funcName;
                    errorLine   = lineID;
                    value       = 0;
                }
                scriptData[scriptDataPos++] = value;
            }
        }
    }
}
void Compilerv4::checkCaseNumber(QString &text)
{
    if (findStringToken(text, "case", 1))
        return;

    QString caseString = "";
    if (text.length() > 4) {
        int textPos = 4;
        do {
            if (text[textPos] != ':')
                caseString += text[textPos];
        } while (++textPos < text.length());
    }

    bool flag = false;

    if (findStringToken(caseString, "[", 1) >= 0) {
        QString caseValue = "";
        QString arrayStr  = "";

        int textPos = 0;
        int mode    = 0;

        while (textPos < caseString.length() && caseString[textPos] != ':') {
            switch (mode) {
                case 0: // normal
                    if (caseString[textPos] == '[')
                        mode = 1;
                    else
                        caseValue += caseString[textPos];
                    ++textPos;
                    break;
                case 1: // array val
                    if (caseString[textPos] == ']')
                        mode = 0;
                    else
                        arrayStr += caseString[textPos];
                    ++textPos;
                    break;
            }
        }

        // Eg: temp0 = TypeName[PlayerObject]
        if (strComp(caseValue, "TypeName")) {
            caseValue = "";
            appendIntegerToString(caseValue, 0);
            for (int o = 0; o < OBJECT_COUNT; ++o) {
                if (strComp(arrayStr, typeNames[o])) {
                    caseValue = "";
                    appendIntegerToString(caseValue, o);
                }
            }
        }

        // Eg: temp0 = SfxName[Jump]
        if (strComp(caseValue, "SfxName")) {
            caseValue = "";
            appendIntegerToString(caseValue, 0);
            int s = 0;
            for (; s < SFX_COUNT; ++s) {
                if (strComp(arrayStr, sfxNames[s])) {
                    caseValue = "";
                    appendIntegerToString(caseValue, s);
                    break;
                }
            }

            if (s == SFX_COUNT) {
                // printLog("WARNING: Unknown sfxName \"%s\", on line %d", arrayStr, lineID);
            }
        }

        // Eg: temp0 = VarName[player.lives]
        if (strComp(caseValue, "VarName")) {
            caseValue = "";
            appendIntegerToString(caseValue, 0);
            int v = 0;
            for (; v < globalVariables.count(); ++v) {
                if (strComp(arrayStr, globalVariables[v])) {
                    caseValue = "";
                    appendIntegerToString(caseValue, v);
                    break;
                }
            }

            if (v == globalVariables.count()) {
                // printLog("WARNING: Unknown varName \"%s\", on line %d", arrayStr, lineID);
            }
        }

        // Eg: temp0 = AchievementName[Ring King]
        if (strComp(caseValue, "AchievementName")) {
            caseValue = "";
            appendIntegerToString(caseValue, 0);
            // default to 0, we don't know what these are
        }

        // Eg: temp0 = PlayerName[SONIC]
        if (strComp(caseValue, "PlayerName")) {
            caseValue = "";
            appendIntegerToString(caseValue, 0);
            int p = 0;
            if (editor) {
                for (; p < ((SceneEditor *)editor)->gameConfig.players.count(); ++p) {
                    QString name = ((SceneEditor *)editor)->gameConfig.players[p].name;
                    name         = name.replace(" ", "");

                    if (strComp(arrayStr, name)) {
                        caseValue = "";
                        appendIntegerToString(caseValue, p);
                        break;
                    }
                }

                if (p == ((SceneEditor *)editor)->gameConfig.players.count()) {
                    // printLog("WARNING: Unknown PlayerName \"%s\", on line %d", arrayStr, lineID);
                }
            }
            else {
                for (; p < gameConfig.players.count(); ++p) {
                    QString name = gameConfig.players[p];
                    name         = name.replace(" ", "");

                    if (strComp(arrayStr, name)) {
                        caseValue = "";
                        appendIntegerToString(caseValue, p);
                        break;
                    }
                }

                if (p == gameConfig.players.count()) {
                    printLog(QString("WARNING: Unknown PlayerName \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
                }
            }
        }

        // Eg: temp0 = StageName[GREEN HILL ZONE 1]
        if (strComp(caseValue, "StageName")) {
            caseValue = "";
            int s     = -1;
            if (arrayStr.length() >= 2) {
                char list = arrayStr[0].toLatin1();
                switch (list) {
                    default: list = 0xFF;
                    case 'P': list = 0; break;
                    case 'R': list = 1; break;
                    case 'S': list = 2; break;
                    case 'B': list = 3; break;
                }

                s = -1;
                if (list <= 3) {
                    QString scnName = arrayStr;
                    scnName.replace(" ", "");
                    scnName = scnName.mid(2, scnName.length() - 2);

                    for (; s < gameConfig.stageLists[list].scenes.count(); ++s) {
                        QString name = gameConfig.stageLists[list].scenes[s].name;
                        name         = name.replace(" ", "");
                        if (strComp(scnName, name))
                            break;
                    }

                    if (s == gameConfig.stageLists[list].scenes.count())
                        s = -1;
                }
            }

            if (s == -1) {
                printLog(
                    QString("WARNING: Unknown StageName \"%1\", on line %2").arg(arrayStr).arg(lineID));
                s = 0;
            }
            caseValue = "";
            appendIntegerToString(caseValue, s);
        }

        caseString = caseValue;
        flag       = true;
    }

    for (int a = 0; a < privateAliasCount && !flag; ++a) {
        if (strComp(caseString, privateAliases[a].name)) {
            caseString = privateAliases[a].value;
            flag       = true;
            break;
        }
    }

    for (int a = 0; a < publicAliasCount && !flag; ++a) {
        if (strComp(caseString, publicAliases[a].name)) {
            caseString = publicAliases[a].value;
            break;
        }
    }

    int caseID = 0;
    if (convertStringToInteger(caseString, &caseID)) {
        int stackValue = jumpTableStack[jumpTableStackPos];
        if (caseID < jumpTableData[stackValue])
            jumpTableData[stackValue] = caseID;
        stackValue++;
        if (caseID > jumpTableData[stackValue])
            jumpTableData[stackValue] = caseID;
    }
    else {
        printLog(QString("WARNING: unable to convert case string \"%1\" to int, on line %2")
                     .arg(caseString)
                     .arg(lineID));
    }
}
bool Compilerv4::readSwitchCase(QString &text)
{
    QString caseText = "";
    if (findStringToken(text, "case", 1)) {
        if (findStringToken(text, "default", 1)) {
            return false;
        }
        else {
            int jumpTablepos                = jumpTableStack[jumpTableStackPos];
            jumpTableData[jumpTablepos + 2] = scriptDataPos - scriptDataOffset;
            int cnt = abs(jumpTableData[jumpTablepos + 1] - jumpTableData[jumpTablepos]) + 1;

            int jOffset = jumpTablepos + 4;
            for (int i = 0; i < cnt; ++i) {
                if (jumpTableData[jOffset + i] < 0)
                    jumpTableData[jOffset + i] = scriptDataPos - scriptDataOffset;
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

        bool flag = false;

        if (findStringToken(caseText, "[", 1) >= 0) {
            QString caseValue = "";
            QString arrayStr  = "";

            int textPos = 0;
            int mode    = 0;

            while (textPos < caseText.length() && caseText[textPos] != ':') {
                switch (mode) {
                    case 0: // normal
                        if (caseText[textPos] == '[')
                            mode = 1;
                        else
                            caseValue += caseText[textPos];
                        ++textPos;
                        break;
                    case 1: // array val
                        if (caseText[textPos] == ']')
                            mode = 0;
                        else
                            arrayStr += caseText[textPos];
                        ++textPos;
                        break;
                }
            }

            // Eg: temp0 = TypeName[PlayerObject]
            if (strComp(caseValue, "TypeName")) {
                caseValue = "";
                appendIntegerToString(caseValue, 0);
                for (int o = 0; o < OBJECT_COUNT; ++o) {
                    if (strComp(arrayStr, typeNames[o])) {
                        caseValue = "";
                        appendIntegerToString(caseValue, o);
                    }
                }
            }

            // Eg: temp0 = SfxName[Jump]
            if (strComp(caseValue, "SfxName")) {
                caseValue = "";
                appendIntegerToString(caseValue, 0);
                int s = 0;
                for (; s < SFX_COUNT; ++s) {
                    if (strComp(arrayStr, sfxNames[s])) {
                        caseValue = "";
                        appendIntegerToString(caseValue, s);
                        break;
                    }
                }

                if (s == SFX_COUNT) {
                    // printLog("WARNING: Unknown sfxName \"%s\", on line %d", arrayStr, lineID);
                }
            }

            // Eg: temp0 = VarName[player.lives]
            if (strComp(caseValue, "VarName")) {
                caseValue = "";
                appendIntegerToString(caseValue, 0);
                int v = 0;
                for (; v < globalVariables.count(); ++v) {
                    if (strComp(arrayStr, globalVariables[v])) {
                        caseValue = "";
                        appendIntegerToString(caseValue, v);
                        break;
                    }
                }

                if (v == globalVariables.count()) {
                    // printLog("WARNING: Unknown varName \"%s\", on line %d", arrayStr, lineID);
                }
            }

            // Eg: temp0 = AchievementName[Ring King]
            if (strComp(caseValue, "AchievementName")) {
                caseValue = "";
                appendIntegerToString(caseValue, 0);
                // default to 0, we don't know what these are
            }

            // Eg: temp0 = PlayerName[SONIC]
            if (strComp(caseValue, "PlayerName")) {
                caseValue = "";
                appendIntegerToString(caseValue, 0);
                int p = 0;
                if (editor) {
                    for (; p < ((SceneEditor *)editor)->gameConfig.players.count(); ++p) {
                        QString name = ((SceneEditor *)editor)->gameConfig.players[p].name;
                        name         = name.replace(" ", "");

                        if (strComp(arrayStr, name)) {
                            caseValue = "";
                            appendIntegerToString(caseValue, p);
                            break;
                        }
                    }

                    if (p == ((SceneEditor *)editor)->gameConfig.players.count()) {
                        // printLog("WARNING: Unknown PlayerName \"%s\", on line %d", arrayStr, lineID);
                    }
                }
                else {
                    for (; p < gameConfig.players.count(); ++p) {
                        QString name = gameConfig.players[p];
                        name         = name.replace(" ", "");

                        if (strComp(arrayStr, name)) {
                            caseValue = "";
                            appendIntegerToString(caseValue, p);
                            break;
                        }
                    }

                    if (p == gameConfig.players.count()) {
                        printLog(QString("WARNING: Unknown PlayerName \"%1\", on line %2")
                                     .arg(arrayStr)
                                     .arg(lineID));
                    }
                }
            }

            // Eg: temp0 = StageName[GREEN HILL ZONE 1]
            if (strComp(caseValue, "StageName")) {
                caseValue = "";
                int s     = -1;
                if (arrayStr.length() >= 2) {
                    char list = arrayStr[0].toLatin1();
                    switch (list) {
                        default: list = 0xFF;
                        case 'P': list = 0; break;
                        case 'R': list = 1; break;
                        case 'S': list = 2; break;
                        case 'B': list = 3; break;
                    }

                    s = -1;
                    if (list <= 3) {
                        QString scnName = arrayStr;
                        scnName.replace(" ", "");
                        scnName = scnName.mid(2, scnName.length() - 2);

                        for (; s < gameConfig.stageLists[list].scenes.count(); ++s) {
                            QString name = gameConfig.stageLists[list].scenes[s].name;
                            name         = name.replace(" ", "");
                            if (strComp(scnName, name))
                                break;
                        }

                        if (s == gameConfig.stageLists[list].scenes.count())
                            s = -1;
                    }
                }

                if (s == -1) {
                    printLog(QString("WARNING: Unknown StageName \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
                    s = 0;
                }
                caseValue = "";
                appendIntegerToString(caseValue, s);
            }

            caseText = caseValue;
            flag     = true;
        }

        for (int a = 0; a < privateAliasCount && !flag; ++a) {
            if (strComp(caseText, privateAliases[a].name)) {
                caseText = privateAliases[a].value;
                flag     = true;
                break;
            }
        }

        for (int a = 0; a < publicAliasCount && !flag; ++a) {
            if (strComp(caseText, publicAliases[a].name)) {
                caseText = publicAliases[a].value;
                break;
            }
        }

        int val = 0;

        int jPos    = jumpTableStack[jumpTableStackPos];
        int jOffset = jPos + 4;
        if (convertStringToInteger(caseText, &val))
            jumpTableData[val - jumpTableData[jPos] + jOffset] = scriptDataPos - scriptDataOffset;
        // else
        //    printLog("WARNING: unable to read case string \"%s\" as an int, on line %d", caseText,
        //    lineID);
        return true;
    }
    return false;
}
void Compilerv4::readTableValues(QString &text)
{
    int textPos       = 0;
    QString strBuffer = "";
    while (true) {
        if (textPos >= text.length() || text[textPos] == ',') {
            if (strBuffer.length()) { // only try if something exists
                int cnt = currentTable->valueCount;

                if (!convertStringToInteger(strBuffer, &currentTable->values[cnt].value)) {
                    bool flag = false;
                    for (int a = 0; a < privateAliasCount; ++a) {
                        if (strComp(privateAliases[a].name, strBuffer)) {
                            strBuffer = publicAliases[a].value;
                            flag      = true;
                            break;
                        }
                    }

                    for (int a = 0; a < publicAliasCount && !flag; ++a) {
                        if (strComp(publicAliases[a].name, strBuffer)) {
                            strBuffer = publicAliases[a].value;
                            break;
                        }
                    }

                    if (!convertStringToInteger(strBuffer, &currentTable->values[cnt].value)) {
                        // printLog(
                        //     "WARNING: unable to convert table var %d value \"%s\" to int, on line
                        //     %d", cnt, strBuffer, lineID);
                    }
                }
                currentTable->valueCount++;
            }

            strBuffer = "";

            if (textPos >= text.length())
                break;
        }
        else {
            strBuffer += text[textPos];
        }
        textPos++;
    }
}
void Compilerv4::appendIntegerToString(QString &text, int value) { text += QString::number(value); }
bool Compilerv4::convertStringToInteger(QString &text, int *value)
{
    *value  = 0;
    bool ok = false;

    bool negative = false;
    if (text.startsWith("-")) {
        negative = true;
        text.remove(0, 1);
    }

    // using toLongLong will handle uint values (they'll be negatives, but the bits will be correct)
    if (text.startsWith("0x") || text.startsWith("0X")) {
        text.remove(0, 2);
        *value = text.toLongLong(&ok, 0x10);
    }
    else if (text.startsWith("0b") || text.startsWith("0B")) {
        text.remove(0, 2);
        *value = text.toLongLong(&ok, 0b10);
    }
    else if (text.startsWith("0o") || text.startsWith("0O")) {
        text.remove(0, 2);
        *value = text.toLongLong(&ok, 0010);
    }
    else {
        *value = text.toLongLong(&ok, 10);
    }

    if (negative)
        *value = -*value;

    return ok;
}
void Compilerv4::copyAliasStr(QString &dest, QString text, bool arrayIndex)
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

void Compilerv4::parseScriptFile(QString scriptName, int scriptID, bool inEditor)
{
    this->inEditor = inEditor;

    jumpTableStackPos = 0;
    lineID            = 0;

    privateAliasCount     = 0;
    privateStaticVarCount = 0;
    privateTableCount     = 0;

    scriptError = false;

    Reader reader(scriptName);
    if (reader.initialised) {
        int readMode   = READMODE_NORMAL;
        int parseMode  = PARSEMODE_SCOPELESS;
        char prevChar  = 0;
        char curChar   = 0;
        int switchDeep = 0;
        int storePos   = 0;

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
                            if (curChar == ';')
                                semiFlag = true;
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
                        if (curChar == ';')
                            semiFlag = true;
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
                        ++lineID;
                    checkAliasText(scriptText);
                    checkStaticText(scriptText);

                    currentTable = (TableInfo *)checkTableText(scriptText);
                    if (currentTable) {
                        parseMode  = PARSEMODE_TABLEREAD;
                        scriptText = "";
                    }

                    if (inEditor) {
                        if (strComp(scriptText, "eventRSDKDraw")) {
                            parseMode                                              = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventRSDKDraw.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].eventRSDKDraw.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                       = scriptDataPos;
                            jumpTableDataOffset                                    = jumpTableDataPos;
                        }
                        if (strComp(scriptText, "eventRSDKLoad")) {
                            parseMode                                              = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventRSDKLoad.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].eventRSDKLoad.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                       = scriptDataPos;
                            jumpTableDataOffset                                    = jumpTableDataPos;
                        }
                        if (strComp(scriptText, "eventRSDKEdit")) {
                            parseMode                                              = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventRSDKEdit.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].eventRSDKEdit.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                       = scriptDataPos;
                            jumpTableDataOffset                                    = jumpTableDataPos;
                        }
                    }
                    else {
                        if (strComp(scriptText, "eventObjectMain")) {
                            parseMode                                          = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventMain.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].eventMain.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                   = scriptDataPos;
                            jumpTableDataOffset                                = jumpTableDataPos;
                        }
                        if (strComp(scriptText, "eventObjectDraw")) {
                            parseMode                                          = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventDraw.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].eventDraw.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                   = scriptDataPos;
                            jumpTableDataOffset                                = jumpTableDataPos;
                        }
                        if (strComp(scriptText, "eventObjectStartup")) {
                            parseMode                                             = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].eventStartup.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].eventStartup.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                      = scriptDataPos;
                            jumpTableDataOffset                                   = jumpTableDataPos;
                        }
                    }

                    if (!findStringToken(scriptText, "reservefunction", 1)) { // forward decl
                        QString funcName = "";
                        for (textPos = 15; textPos < scriptText.length(); ++textPos)
                            funcName += scriptText[textPos];

                        int funcID = -1;
                        for (int f = 0; f < functionCount; ++f) {
                            if (strComp(funcName, functionNames[f]))
                                funcID = f;
                        }

                        if (functionCount < FUNCTION_COUNT && funcID == -1) {
                            functionNames[functionCount++] = funcName;
                        }
                        else {
                            printLog(QString("Warning: Function %1 has already been reserved!")
                                         .arg(funcName));
                        }
                        parseMode = PARSEMODE_SCOPELESS;
                    }
                    else if (!findStringToken(scriptText, "function", 1)) { // regular decl
                        QString funcName = "";
                        for (textPos = 8; textPos < scriptText.length(); ++textPos)
                            funcName += scriptText[textPos];

                        int funcID = -1;
                        for (int f = 0; f < functionCount; ++f) {
                            if (strComp(funcName, functionNames[f]))
                                funcID = f;
                        }
                        if (funcID <= -1) {
                            if (functionCount >= FUNCTION_COUNT) {
                                parseMode = PARSEMODE_SCOPELESS;
                            }
                            else {
                                functionNames[functionCount]              = funcName;
                                functionList[functionCount].scriptCodePtr = scriptDataPos;
                                functionList[functionCount].jumpTablePtr  = jumpTableDataPos;
                                scriptDataOffset                          = scriptDataPos;
                                jumpTableDataOffset                       = jumpTableDataPos;
                                parseMode                                 = PARSEMODE_FUNCTION;
                                ++functionCount;
                            }
                        }
                        else {
                            functionNames[funcID]              = funcName;
                            functionList[funcID].scriptCodePtr = scriptDataPos;
                            functionList[funcID].jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                   = scriptDataPos;
                            jumpTableDataOffset                = jumpTableDataPos;
                            parseMode                          = PARSEMODE_FUNCTION;
                        }
                    }
                    break;
                case PARSEMODE_PLATFORMSKIP:
                    if (!semiFlag)
                        ++lineID;
                    if (!findStringToken(scriptText, "#endplatform", 1))
                        parseMode = PARSEMODE_FUNCTION;
                    break;
                case PARSEMODE_FUNCTION:
                    if (!semiFlag)
                        ++lineID;
                    if (scriptText.length()) {
                        if (strComp(scriptText, "endevent")) {
                            scriptData[scriptDataPos++] = FUNC_END;
                            parseMode                   = PARSEMODE_SCOPELESS;
                        }
                        else if (strComp(scriptText, "endfunction")) {
                            scriptData[scriptDataPos++] = FUNC_RETURN;
                            parseMode                   = PARSEMODE_SCOPELESS;
                        }
                        else if (findStringToken(scriptText, "#platform:", 1)) {
                            // layed out like ass, but this means "if we did not find "#platform:"
                            if (findStringToken(scriptText, "#endplatform", 1) == -1) {
                                // if we did NOT find "#endplatform"
                                convertIfWhileStatement(scriptText);
                                convertForeachStatement(scriptText);
                                if (convertSwitchStatement(scriptText)) {
                                    parseMode  = PARSEMODE_SWITCHREAD;
                                    storePos   = (int)reader.tell();
                                    switchDeep = 0;
                                }
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
                        else if (findStringToken(scriptText, gamePlatform, 1) == -1
                                 && findStringToken(scriptText, gameRenderType, 1) == -1
                                 && findStringToken(scriptText, gameHapticSetting, 1) == -1
                                 && findStringToken(scriptText, "USE_DECOMP", 1) == -1
                                 && findStringToken(scriptText, "USE_EDITOR", 1) == -1) {
                            // if NONE of these checks succeeded, then we skip everything until "end
                            // platform"
                            parseMode = PARSEMODE_PLATFORMSKIP;
                        }
                    }
                    break;
                case PARSEMODE_SWITCHREAD:
                    if (!findStringToken(scriptText, "switch", 1))
                        ++switchDeep;
                    if (switchDeep) {
                        if (!findStringToken(scriptText, "endswitch", 1))
                            --switchDeep;
                    }
                    else if (findStringToken(scriptText, "endswitch", 1)) {
                        checkCaseNumber(scriptText);
                    }
                    else {
                        reader.seek(storePos);
                        parseMode  = PARSEMODE_FUNCTION;
                        int jPos   = jumpTableStack[jumpTableStackPos];
                        switchDeep = abs(jumpTableData[jPos + 1] - jumpTableData[jPos]) + 1;
                        for (textPos = 0; textPos < switchDeep; ++textPos)
                            jumpTableData[jumpTableDataPos++] = -1;
                    }
                    break;
                case PARSEMODE_TABLEREAD:
                    if (!semiFlag)
                        ++lineID;
                    if (findStringToken(scriptText, "endtable", 1)) {
                        readTableValues(scriptText);
                    }
                    else {
                        currentTable->dataPos       = scriptDataPos;
                        scriptData[scriptDataPos++] = currentTable->valueCount;
                        for (int v = 0; v < currentTable->valueCount; ++v) {
                            currentTable->values[v].dataPos = scriptDataPos;
                            scriptData[scriptDataPos++]     = currentTable->values[v].value;
                        }

                        if (curTablePublic) {
                            publicTables[publicTableCount] = *currentTable;
                            ++publicTableCount;
                        }
                        else {
                            privateTables[privateTableCount] = *currentTable;
                            ++privateTableCount;
                        }

                        currentTable = NULL;
                        parseMode    = PARSEMODE_SCOPELESS;
                    }
                    break;
                default: break;
            }
        }

        reader.close();
    }
}

void Compilerv4::clearScriptData()
{
    memset(scriptData, 0, SCRIPTDATA_COUNT_v4 * sizeof(int));
    memset(jumpTableData, 0, JUMPTABLE_COUNT_v4 * sizeof(int));

    memset(foreachStack, -1, FORSTACK_COUNT_v4 * sizeof(int));
    memset(jumpTableStack, 0, JUMPSTACK_COUNT_v4 * sizeof(int));
    memset(functionStack, 0, FUNCSTACK_COUNT_v4 * sizeof(int));

    globalVariables.clear();

    scriptFrameCount = 0;

    // scriptCodePos     = 0;
    // jumpTablePos      = 0;
    jumpTableStackPos = 0;
    functionStackPos  = 0;

    scriptDataPos       = 0;
    scriptDataOffset    = 0;
    jumpTableDataPos    = 0;
    jumpTableDataOffset = 0;

    functionCount = 0;

    lineID = 0;

    publicAliasCount  = COMMONALIAS_COUNT_v4;
    privateAliasCount = 0;

    publicStaticVarCount  = 0;
    privateStaticVarCount = 0;

    publicTableCount  = 0;
    privateTableCount = 0;

    for (int o = 0; o < OBJECT_COUNT; ++o) {
        ObjectScript *scriptInfo = &objectScriptList[o];

        scriptInfo->eventMain.scriptCodePtr    = SCRIPTDATA_COUNT_v4 - 1;
        scriptInfo->eventMain.jumpTablePtr     = JUMPTABLE_COUNT_v4 - 1;
        scriptInfo->eventDraw.scriptCodePtr    = SCRIPTDATA_COUNT_v4 - 1;
        scriptInfo->eventDraw.jumpTablePtr     = JUMPTABLE_COUNT_v4 - 1;
        scriptInfo->eventStartup.scriptCodePtr = SCRIPTDATA_COUNT_v4 - 1;
        scriptInfo->eventStartup.jumpTablePtr  = JUMPTABLE_COUNT_v4 - 1;

        scriptInfo->eventRSDKDraw.scriptCodePtr = SCRIPTDATA_COUNT_v4 - 1;
        scriptInfo->eventRSDKDraw.jumpTablePtr  = JUMPTABLE_COUNT_v4 - 1;
        scriptInfo->eventRSDKLoad.scriptCodePtr = SCRIPTDATA_COUNT_v4 - 1;
        scriptInfo->eventRSDKLoad.jumpTablePtr  = JUMPTABLE_COUNT_v4 - 1;
        scriptInfo->eventRSDKEdit.scriptCodePtr = SCRIPTDATA_COUNT_v4 - 1;
        scriptInfo->eventRSDKEdit.jumpTablePtr  = JUMPTABLE_COUNT_v4 - 1;

        typeNames[o] = "";
    }

    for (int f = 0; f < FUNCTION_COUNT; ++f) {
        functionList[f].scriptCodePtr = SCRIPTDATA_COUNT_v4 - 1;
        functionList[f].jumpTablePtr  = JUMPTABLE_COUNT_v4 - 1;
    }

    typeNames[0] = "Blank Object";
}

void Compilerv4::writeBytecode(QString path)
{
    bytecode = RSDKv4::Bytecode();

    bytecode.scriptCode.clear();
    for (int i = globalScriptDataCount; i < scriptDataPos; ++i) {
        bytecode.scriptCode.append(scriptData[i]);
    }

    bytecode.jumpTable.clear();
    for (int i = globalJumpTableCount; i < jumpTableDataPos; ++i) {
        bytecode.jumpTable.append(jumpTableData[i]);
    }

    bytecode.scriptList.clear();
    for (int i = globalScriptCount; i < scriptCount; ++i) {
        RSDKv4::Bytecode::ScriptInfo scr;

        scr.main.scriptCodePos = objectScriptList[i].eventMain.scriptCodePtr;
        scr.main.jumpTablePos  = objectScriptList[i].eventMain.jumpTablePtr;

        scr.draw.scriptCodePos = objectScriptList[i].eventDraw.scriptCodePtr;
        scr.draw.jumpTablePos  = objectScriptList[i].eventDraw.jumpTablePtr;

        scr.startup.scriptCodePos = objectScriptList[i].eventStartup.scriptCodePtr;
        scr.startup.jumpTablePos  = objectScriptList[i].eventStartup.jumpTablePtr;

        bytecode.scriptList.append(scr);
    }

    bytecode.functionList.clear();
    for (int f = 0; f < functionCount; ++f) {
        RSDKv4::Bytecode::FunctionInfo func;
        func.scriptCodePos = functionList[f].scriptCodePtr;
        func.jumpTablePos  = functionList[f].jumpTablePtr;
        bytecode.functionList.append(func);
    }

    bytecode.write(path);
}

void Compilerv4::processScript(int scriptCodePtr, int jumpTablePtr, byte scriptEvent)
{
    bool running        = true;
    int scriptDataPtr   = scriptCodePtr;
    jumpTableStackPos   = 0;
    functionStackPos    = 0;
    foreachStackPos     = 0;
    SceneEditor *editor = (SceneEditor *)this->editor;

    while (running) {
        int opcode           = scriptData[scriptDataPtr++];
        int opcodeSize       = functionsv4[opcode].opcodeSize;
        int scriptCodeOffset = scriptDataPtr;

        scriptText = "";

        // Get Values
        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = scriptData[scriptDataPtr++];

            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (scriptData[scriptDataPtr++]) {
                    case VARARR_NONE: arrayVal = objectEntityPos; break;
                    case VARARR_ARRAY:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal = scriptEng.arrayPosition[scriptData[scriptDataPtr++]];
                        else
                            arrayVal = scriptData[scriptDataPtr++];
                        break;
                    case VARARR_ENTNOPLUS1:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                scriptEng.arrayPosition[scriptData[scriptDataPtr++]] + objectEntityPos;
                        else
                            arrayVal = scriptData[scriptDataPtr++] + objectEntityPos;
                        break;
                    case VARARR_ENTNOMINUS1:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                objectEntityPos - scriptEng.arrayPosition[scriptData[scriptDataPtr++]];
                        else
                            arrayVal = objectEntityPos - scriptData[scriptDataPtr++];
                        break;
                    default: break;
                }

                // Allows the game to link to the editor properly
                Entity *entityPtr = &objectEntityList[arrayVal];
                if (editor) {
                    for (int e = 0; e < editor->viewer->entities.count(); ++e) {
                        if (arrayVal == editor->viewer->entities[e].gameEntitySlot) {
                            entityPtr = &objectEntityList[e];
                        }
                    }
                }

                // Variables
                switch (scriptData[scriptDataPtr++]) {
                    default: break;
                    case VAR_TEMP0: scriptEng.operands[i] = scriptEng.tempValue[0]; break;
                    case VAR_TEMP1: scriptEng.operands[i] = scriptEng.tempValue[1]; break;
                    case VAR_TEMP2: scriptEng.operands[i] = scriptEng.tempValue[2]; break;
                    case VAR_TEMP3: scriptEng.operands[i] = scriptEng.tempValue[3]; break;
                    case VAR_TEMP4: scriptEng.operands[i] = scriptEng.tempValue[4]; break;
                    case VAR_TEMP5: scriptEng.operands[i] = scriptEng.tempValue[5]; break;
                    case VAR_TEMP6: scriptEng.operands[i] = scriptEng.tempValue[6]; break;
                    case VAR_TEMP7: scriptEng.operands[i] = scriptEng.tempValue[7]; break;
                    case VAR_CHECKRESULT: scriptEng.operands[i] = scriptEng.checkResult; break;
                    case VAR_ARRAYPOS0: scriptEng.operands[i] = scriptEng.arrayPosition[0]; break;
                    case VAR_ARRAYPOS1: scriptEng.operands[i] = scriptEng.arrayPosition[1]; break;
                    case VAR_ARRAYPOS2: scriptEng.operands[i] = scriptEng.arrayPosition[2]; break;
                    case VAR_ARRAYPOS3: scriptEng.operands[i] = scriptEng.arrayPosition[3]; break;
                    case VAR_ARRAYPOS4: scriptEng.operands[i] = scriptEng.arrayPosition[4]; break;
                    case VAR_ARRAYPOS5: scriptEng.operands[i] = scriptEng.arrayPosition[5]; break;
                    case VAR_ARRAYPOS6: scriptEng.operands[i] = scriptEng.arrayPosition[6]; break;
                    case VAR_ARRAYPOS7: scriptEng.operands[i] = scriptEng.arrayPosition[7]; break;
                    case VAR_GLOBAL: /*scriptEng.operands[i] = globalVariables[arrayVal];*/ break;
                    case VAR_LOCAL: scriptEng.operands[i] = scriptData[arrayVal]; break;
                    case VAR_OBJECTENTITYPOS: scriptEng.operands[i] = arrayVal; break;
                    case VAR_OBJECTGROUPID: {
                        scriptEng.operands[i] = entityPtr->typeGroup;
                        break;
                    }
                    case VAR_OBJECTTYPE: {
                        scriptEng.operands[i] = entityPtr->type;
                        break;
                    }
                    case VAR_OBJECTPROPERTYVALUE: {
                        scriptEng.operands[i] = entityPtr->propertyValue;
                        break;
                    }
                    case VAR_OBJECTXPOS: {
                        scriptEng.operands[i] = entityPtr->XPos;
                        break;
                    }
                    case VAR_OBJECTYPOS: {
                        scriptEng.operands[i] = entityPtr->YPos;
                        break;
                    }
                    case VAR_OBJECTIXPOS: {
                        scriptEng.operands[i] = entityPtr->XPos >> 16;
                        break;
                    }
                    case VAR_OBJECTIYPOS: {
                        scriptEng.operands[i] = entityPtr->YPos >> 16;
                        break;
                    }
                    case VAR_OBJECTXVEL: {
                        scriptEng.operands[i] = entityPtr->XVelocity;
                        break;
                    }
                    case VAR_OBJECTYVEL: {
                        scriptEng.operands[i] = entityPtr->YVelocity;
                        break;
                    }
                    case VAR_OBJECTSPEED: {
                        scriptEng.operands[i] = entityPtr->speed;
                        break;
                    }
                    case VAR_OBJECTSTATE: {
                        scriptEng.operands[i] = entityPtr->state;
                        break;
                    }
                    case VAR_OBJECTROTATION: {
                        scriptEng.operands[i] = entityPtr->rotation;
                        break;
                    }
                    case VAR_OBJECTSCALE: {
                        scriptEng.operands[i] = entityPtr->scale;
                        break;
                    }
                    case VAR_OBJECTPRIORITY: {
                        scriptEng.operands[i] = entityPtr->priority;
                        break;
                    }
                    case VAR_OBJECTDRAWORDER: {
                        scriptEng.operands[i] = entityPtr->drawOrder;
                        break;
                    }
                    case VAR_OBJECTDIRECTION: {
                        scriptEng.operands[i] = entityPtr->direction;
                        break;
                    }
                    case VAR_OBJECTINKEFFECT: {
                        scriptEng.operands[i] = entityPtr->inkEffect;
                        break;
                    }
                    case VAR_OBJECTALPHA: {
                        scriptEng.operands[i] = entityPtr->alpha;
                        break;
                    }
                    case VAR_OBJECTFRAME: {
                        scriptEng.operands[i] = entityPtr->frame;
                        break;
                    }
                    case VAR_OBJECTANIMATION: {
                        scriptEng.operands[i] = entityPtr->animation;
                        break;
                    }
                    case VAR_OBJECTPREVANIMATION: {
                        scriptEng.operands[i] = entityPtr->prevAnimation;
                        break;
                    }
                    case VAR_OBJECTANIMATIONSPEED: {
                        scriptEng.operands[i] = entityPtr->animationSpeed;
                        break;
                    }
                    case VAR_OBJECTANIMATIONTIMER: {
                        scriptEng.operands[i] = entityPtr->animationTimer;
                        break;
                    }
                    case VAR_OBJECTANGLE: {
                        scriptEng.operands[i] = entityPtr->angle;
                        break;
                    }
                    case VAR_OBJECTLOOKPOSX: {
                        scriptEng.operands[i] = entityPtr->lookPosX;
                        break;
                    }
                    case VAR_OBJECTLOOKPOSY: {
                        scriptEng.operands[i] = entityPtr->lookPosY;
                        break;
                    }
                    case VAR_OBJECTCOLLISIONMODE: {
                        scriptEng.operands[i] = entityPtr->collisionMode;
                        break;
                    }
                    case VAR_OBJECTCOLLISIONPLANE: {
                        scriptEng.operands[i] = entityPtr->collisionPlane;
                        break;
                    }
                    case VAR_OBJECTCONTROLMODE: {
                        scriptEng.operands[i] = entityPtr->controlMode;
                        break;
                    }
                    case VAR_OBJECTCONTROLLOCK: {
                        scriptEng.operands[i] = entityPtr->controlLock;
                        break;
                    }
                    case VAR_OBJECTPUSHING: {
                        scriptEng.operands[i] = entityPtr->pushing;
                        break;
                    }
                    case VAR_OBJECTVISIBLE: {
                        scriptEng.operands[i] = entityPtr->visible;
                        break;
                    }
                    case VAR_OBJECTTILECOLLISIONS: {
                        scriptEng.operands[i] = entityPtr->tileCollisions;
                        break;
                    }
                    case VAR_OBJECTINTERACTION: {
                        scriptEng.operands[i] = entityPtr->objectInteractions;
                        break;
                    }
                    case VAR_OBJECTGRAVITY: {
                        scriptEng.operands[i] = entityPtr->gravity;
                        break;
                    }
                    case VAR_OBJECTUP: {
                        scriptEng.operands[i] = entityPtr->up;
                        break;
                    }
                    case VAR_OBJECTDOWN: {
                        scriptEng.operands[i] = entityPtr->down;
                        break;
                    }
                    case VAR_OBJECTLEFT: {
                        scriptEng.operands[i] = entityPtr->left;
                        break;
                    }
                    case VAR_OBJECTRIGHT: {
                        scriptEng.operands[i] = entityPtr->right;
                        break;
                    }
                    case VAR_OBJECTJUMPPRESS: {
                        scriptEng.operands[i] = entityPtr->jumpPress;
                        break;
                    }
                    case VAR_OBJECTJUMPHOLD: {
                        scriptEng.operands[i] = entityPtr->jumpHold;
                        break;
                    }
                    case VAR_OBJECTSCROLLTRACKING: {
                        scriptEng.operands[i] = entityPtr->trackScroll;
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORL: {
                        scriptEng.operands[i] = entityPtr->flailing[0];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORC: {
                        scriptEng.operands[i] = entityPtr->flailing[1];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORR: {
                        scriptEng.operands[i] = entityPtr->flailing[2];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORLC: {
                        scriptEng.operands[i] = entityPtr->flailing[3];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORRC: {
                        scriptEng.operands[i] = entityPtr->flailing[4];
                        break;
                    }
                    case VAR_OBJECTCOLLISIONLEFT: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONTOP: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONRIGHT: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONBOTTOM: {
                        break;
                    }
                    case VAR_OBJECTOUTOFBOUNDS: {
                        break;
                    }
                    case VAR_OBJECTSPRITESHEET: {
                        scriptEng.operands[i] = objectScriptList[entityPtr->type].spriteSheetID;
                        break;
                    }
                    case VAR_OBJECTVALUE0: {
                        scriptEng.operands[i] = entityPtr->values[0];
                        break;
                    }
                    case VAR_OBJECTVALUE1: {
                        scriptEng.operands[i] = entityPtr->values[1];
                        break;
                    }
                    case VAR_OBJECTVALUE2: {
                        scriptEng.operands[i] = entityPtr->values[2];
                        break;
                    }
                    case VAR_OBJECTVALUE3: {
                        scriptEng.operands[i] = entityPtr->values[3];
                        break;
                    }
                    case VAR_OBJECTVALUE4: {
                        scriptEng.operands[i] = entityPtr->values[4];
                        break;
                    }
                    case VAR_OBJECTVALUE5: {
                        scriptEng.operands[i] = entityPtr->values[5];
                        break;
                    }
                    case VAR_OBJECTVALUE6: {
                        scriptEng.operands[i] = entityPtr->values[6];
                        break;
                    }
                    case VAR_OBJECTVALUE7: {
                        scriptEng.operands[i] = entityPtr->values[7];
                        break;
                    }
                    case VAR_OBJECTVALUE8: {
                        scriptEng.operands[i] = entityPtr->values[8];
                        break;
                    }
                    case VAR_OBJECTVALUE9: {
                        scriptEng.operands[i] = entityPtr->values[9];
                        break;
                    }
                    case VAR_OBJECTVALUE10: {
                        scriptEng.operands[i] = entityPtr->values[10];
                        break;
                    }
                    case VAR_OBJECTVALUE11: {
                        scriptEng.operands[i] = entityPtr->values[11];
                        break;
                    }
                    case VAR_OBJECTVALUE12: {
                        scriptEng.operands[i] = entityPtr->values[12];
                        break;
                    }
                    case VAR_OBJECTVALUE13: {
                        scriptEng.operands[i] = entityPtr->values[13];
                        break;
                    }
                    case VAR_OBJECTVALUE14: {
                        scriptEng.operands[i] = entityPtr->values[14];
                        break;
                    }
                    case VAR_OBJECTVALUE15: {
                        scriptEng.operands[i] = entityPtr->values[15];
                        break;
                    }
                    case VAR_OBJECTVALUE16: {
                        scriptEng.operands[i] = entityPtr->values[16];
                        break;
                    }
                    case VAR_OBJECTVALUE17: {
                        scriptEng.operands[i] = entityPtr->values[17];
                        break;
                    }
                    case VAR_OBJECTVALUE18: {
                        scriptEng.operands[i] = entityPtr->values[18];
                        break;
                    }
                    case VAR_OBJECTVALUE19: {
                        scriptEng.operands[i] = entityPtr->values[19];
                        break;
                    }
                    case VAR_OBJECTVALUE20: {
                        scriptEng.operands[i] = entityPtr->values[20];
                        break;
                    }
                    case VAR_OBJECTVALUE21: {
                        scriptEng.operands[i] = entityPtr->values[21];
                        break;
                    }
                    case VAR_OBJECTVALUE22: {
                        scriptEng.operands[i] = entityPtr->values[22];
                        break;
                    }
                    case VAR_OBJECTVALUE23: {
                        scriptEng.operands[i] = entityPtr->values[23];
                        break;
                    }
                    case VAR_OBJECTVALUE24: {
                        scriptEng.operands[i] = entityPtr->values[24];
                        break;
                    }
                    case VAR_OBJECTVALUE25: {
                        scriptEng.operands[i] = entityPtr->values[25];
                        break;
                    }
                    case VAR_OBJECTVALUE26: {
                        scriptEng.operands[i] = entityPtr->values[26];
                        break;
                    }
                    case VAR_OBJECTVALUE27: {
                        scriptEng.operands[i] = entityPtr->values[27];
                        break;
                    }
                    case VAR_OBJECTVALUE28: {
                        scriptEng.operands[i] = entityPtr->values[28];
                        break;
                    }
                    case VAR_OBJECTVALUE29: {
                        scriptEng.operands[i] = entityPtr->values[29];
                        break;
                    }
                    case VAR_OBJECTVALUE30: {
                        scriptEng.operands[i] = entityPtr->values[30];
                        break;
                    }
                    case VAR_OBJECTVALUE31: {
                        scriptEng.operands[i] = entityPtr->values[31];
                        break;
                    }
                    case VAR_OBJECTVALUE32: {
                        scriptEng.operands[i] = entityPtr->values[32];
                        break;
                    }
                    case VAR_OBJECTVALUE33: {
                        scriptEng.operands[i] = entityPtr->values[33];
                        break;
                    }
                    case VAR_OBJECTVALUE34: {
                        scriptEng.operands[i] = entityPtr->values[34];
                        break;
                    }
                    case VAR_OBJECTVALUE35: {
                        scriptEng.operands[i] = entityPtr->values[35];
                        break;
                    }
                    case VAR_OBJECTVALUE36: {
                        scriptEng.operands[i] = entityPtr->values[36];
                        break;
                    }
                    case VAR_OBJECTVALUE37: {
                        scriptEng.operands[i] = entityPtr->values[37];
                        break;
                    }
                    case VAR_OBJECTVALUE38: {
                        scriptEng.operands[i] = entityPtr->values[38];
                        break;
                    }
                    case VAR_OBJECTVALUE39: {
                        scriptEng.operands[i] = entityPtr->values[39];
                        break;
                    }
                    case VAR_OBJECTVALUE40: {
                        scriptEng.operands[i] = entityPtr->values[40];
                        break;
                    }
                    case VAR_OBJECTVALUE41: {
                        scriptEng.operands[i] = entityPtr->values[41];
                        break;
                    }
                    case VAR_OBJECTVALUE42: {
                        scriptEng.operands[i] = entityPtr->values[42];
                        break;
                    }
                    case VAR_OBJECTVALUE43: {
                        scriptEng.operands[i] = entityPtr->values[43];
                        break;
                    }
                    case VAR_OBJECTVALUE44: {
                        scriptEng.operands[i] = entityPtr->values[44];
                        break;
                    }
                    case VAR_OBJECTVALUE45: {
                        scriptEng.operands[i] = entityPtr->values[45];
                        break;
                    }
                    case VAR_OBJECTVALUE46: {
                        scriptEng.operands[i] = entityPtr->values[46];
                        break;
                    }
                    case VAR_OBJECTVALUE47: {
                        scriptEng.operands[i] = entityPtr->values[47];
                        break;
                    }
                    case VAR_STAGESTATE: break;
                    case VAR_STAGEACTIVELIST: break;
                    case VAR_STAGELISTPOS: break;
                    case VAR_STAGETIMEENABLED: break;
                    case VAR_STAGEMILLISECONDS: break;
                    case VAR_STAGESECONDS: break;
                    case VAR_STAGEMINUTES: break;
                    case VAR_STAGEACTNUM: break;
                    case VAR_STAGEPAUSEENABLED: break;
                    case VAR_STAGELISTSIZE: break;
                    case VAR_STAGENEWXBOUNDARY1: break;
                    case VAR_STAGENEWXBOUNDARY2: break;
                    case VAR_STAGENEWYBOUNDARY1: break;
                    case VAR_STAGENEWYBOUNDARY2: break;
                    case VAR_STAGECURXBOUNDARY1: break;
                    case VAR_STAGECURXBOUNDARY2: break;
                    case VAR_STAGECURYBOUNDARY1: break;
                    case VAR_STAGECURYBOUNDARY2: break;
                    case VAR_STAGEDEFORMATIONDATA0: break;
                    case VAR_STAGEDEFORMATIONDATA1: break;
                    case VAR_STAGEDEFORMATIONDATA2: break;
                    case VAR_STAGEDEFORMATIONDATA3: break;
                    case VAR_STAGEWATERLEVEL: break;
                    case VAR_STAGEACTIVELAYER: break;
                    case VAR_STAGEMIDPOINT: break;
                    case VAR_STAGEPLAYERLISTPOS: break;
                    case VAR_STAGEDEBUGMODE: break;
                    case VAR_STAGEENTITYPOS: break;
                    case VAR_SCREENCAMERAENABLED: break;
                    case VAR_SCREENCAMERATARGET: break;
                    case VAR_SCREENCAMERASTYLE: break;
                    case VAR_SCREENCAMERAX: break;
                    case VAR_SCREENCAMERAY: break;
                    case VAR_SCREENDRAWLISTSIZE: break;
                    case VAR_SCREENXCENTER: break;
                    case VAR_SCREENYCENTER: break;
                    case VAR_SCREENXSIZE: break;
                    case VAR_SCREENYSIZE: break;
                    case VAR_SCREENXOFFSET: break;
                    case VAR_SCREENYOFFSET: break;
                    case VAR_SCREENSHAKEX: break;
                    case VAR_SCREENSHAKEY: break;
                    case VAR_SCREENADJUSTCAMERAY: break;
                    case VAR_TOUCHSCREENDOWN: break;
                    case VAR_TOUCHSCREENXPOS: break;
                    case VAR_TOUCHSCREENYPOS: break;
                    case VAR_MUSICVOLUME: break;
                    case VAR_MUSICCURRENTTRACK: break;
                    case VAR_MUSICPOSITION: break;
                    case VAR_INPUTDOWNUP: break;
                    case VAR_INPUTDOWNDOWN: break;
                    case VAR_INPUTDOWNLEFT: break;
                    case VAR_INPUTDOWNRIGHT: break;
                    case VAR_INPUTDOWNBUTTONA: break;
                    case VAR_INPUTDOWNBUTTONB: break;
                    case VAR_INPUTDOWNBUTTONC: break;
                    case VAR_INPUTDOWNBUTTONX: break;
                    case VAR_INPUTDOWNBUTTONY: break;
                    case VAR_INPUTDOWNBUTTONZ: break;
                    case VAR_INPUTDOWNBUTTONL: break;
                    case VAR_INPUTDOWNBUTTONR: break;
                    case VAR_INPUTDOWNSTART: break;
                    case VAR_INPUTDOWNSELECT: break;
                    case VAR_INPUTPRESSUP: break;
                    case VAR_INPUTPRESSDOWN: break;
                    case VAR_INPUTPRESSLEFT: break;
                    case VAR_INPUTPRESSRIGHT: break;
                    case VAR_INPUTPRESSBUTTONA: break;
                    case VAR_INPUTPRESSBUTTONB: break;
                    case VAR_INPUTPRESSBUTTONC: break;
                    case VAR_INPUTPRESSBUTTONX: break;
                    case VAR_INPUTPRESSBUTTONY: break;
                    case VAR_INPUTPRESSBUTTONZ: break;
                    case VAR_INPUTPRESSBUTTONL: break;
                    case VAR_INPUTPRESSBUTTONR: break;
                    case VAR_INPUTPRESSSTART: break;
                    case VAR_INPUTPRESSSELECT: break;
                    case VAR_MENU1SELECTION: break;
                    case VAR_MENU2SELECTION: break;
                    case VAR_TILELAYERXSIZE: break;
                    case VAR_TILELAYERYSIZE: break;
                    case VAR_TILELAYERTYPE: break;
                    case VAR_TILELAYERANGLE: break;
                    case VAR_TILELAYERXPOS: break;
                    case VAR_TILELAYERYPOS: break;
                    case VAR_TILELAYERZPOS: break;
                    case VAR_TILELAYERPARALLAXFACTOR: break;
                    case VAR_TILELAYERSCROLLSPEED: break;
                    case VAR_TILELAYERSCROLLPOS: break;
                    case VAR_TILELAYERDEFORMATIONOFFSET: break;
                    case VAR_TILELAYERDEFORMATIONOFFSETW: break;
                    case VAR_HPARALLAXPARALLAXFACTOR: break;
                    case VAR_HPARALLAXSCROLLSPEED: break;
                    case VAR_HPARALLAXSCROLLPOS: break;
                    case VAR_VPARALLAXPARALLAXFACTOR: break;
                    case VAR_VPARALLAXSCROLLSPEED: break;
                    case VAR_VPARALLAXSCROLLPOS: break;
                    case VAR_SCENE3DVERTEXCOUNT: break;
                    case VAR_SCENE3DFACECOUNT: break;
                    case VAR_SCENE3DPROJECTIONX: break;
                    case VAR_SCENE3DPROJECTIONY: break;
                    case VAR_SCENE3DFOGCOLOR: break;
                    case VAR_SCENE3DFOGSTRENGTH: break;
                    case VAR_VERTEXBUFFERX: break;
                    case VAR_VERTEXBUFFERY: break;
                    case VAR_VERTEXBUFFERZ: break;
                    case VAR_VERTEXBUFFERU: break;
                    case VAR_VERTEXBUFFERV: break;
                    case VAR_FACEBUFFERA: break;
                    case VAR_FACEBUFFERB: break;
                    case VAR_FACEBUFFERC: break;
                    case VAR_FACEBUFFERD: break;
                    case VAR_FACEBUFFERFLAG: break;
                    case VAR_FACEBUFFERCOLOR: break;
                    case VAR_SAVERAM: break;
                    case VAR_ENGINESTATE: break;
                    case VAR_ENGINELANGUAGE: break;
                    case VAR_ENGINEONLINEACTIVE: break;
                    case VAR_ENGINESFXVOLUME: break;
                    case VAR_ENGINEBGMVOLUME: break;
                    case VAR_ENGINETRIALMODE: break;
                    case VAR_ENGINEDEVICETYPE:
                        break;
                        // EDITOR-ONLY
                    case VAR_EDITORVARIABLEID:
                        scriptEng.operands[i] = scnEditor->viewer->variableID;
                        break;
                    case VAR_EDITORVARIABLEVAL:
                        scriptEng.operands[i] = scnEditor->viewer->variableValue;
                        break;
                    case VAR_EDITORRETURNVAR:
                        scriptEng.operands[i] = scnEditor->viewer->returnVariable;
                        break;
                    case VAR_EDITORSHOWGIZMOS:
                        scriptEng.operands[i] = scnEditor->viewer->sceneInfo.effectGizmo
                                                || scnEditor->viewer->selectedEntity == objectEntityPos;
                        break;
                }
            }
            else if (opcodeType == SCRIPTVAR_INTCONST) { // int constant
                scriptEng.operands[i] = scriptData[scriptDataPtr++];
            }
            else if (opcodeType == SCRIPTVAR_STRCONST) { // string constant
                int strLen = scriptData[scriptDataPtr++];
                scriptText = "";
                for (int c = 0; c < strLen; ++c) {
                    switch (c % 4) {
                        case 0: {
                            scriptText += (char)(scriptData[scriptDataPtr] >> 24);
                            break;
                        }
                        case 1: {
                            scriptText += (char)((0xFFFFFF & scriptData[scriptDataPtr]) >> 16);
                            break;
                        }
                        case 2: {
                            scriptText += (char)((0xFFFF & scriptData[scriptDataPtr]) >> 8);
                            break;
                        }
                        case 3: {
                            scriptText += (char)scriptData[scriptDataPtr++];
                            break;
                        }
                        default: break;
                    }
                }
                scriptDataPtr++;
            }
        }

        ObjectScript *scriptInfo = &objectScriptList[objectEntityList[objectEntityPos].type];
        Entity *entity           = &objectEntityList[objectEntityPos];
        SpriteFrame *spriteFrame = nullptr;

        // Functions
        switch (opcode) {
            default: break;
            case FUNC_END: running = false; break;
            case FUNC_EQUAL: scriptEng.operands[0] = scriptEng.operands[1]; break;
            case FUNC_ADD: scriptEng.operands[0] += scriptEng.operands[1]; break;
            case FUNC_SUB: scriptEng.operands[0] -= scriptEng.operands[1]; break;
            case FUNC_INC: ++scriptEng.operands[0]; break;
            case FUNC_DEC: --scriptEng.operands[0]; break;
            case FUNC_MUL: scriptEng.operands[0] *= scriptEng.operands[1]; break;
            case FUNC_DIV: scriptEng.operands[0] /= scriptEng.operands[1]; break;
            case FUNC_SHR: scriptEng.operands[0] >>= scriptEng.operands[1]; break;
            case FUNC_SHL: scriptEng.operands[0] <<= scriptEng.operands[1]; break;
            case FUNC_AND: scriptEng.operands[0] &= scriptEng.operands[1]; break;
            case FUNC_OR: scriptEng.operands[0] |= scriptEng.operands[1]; break;
            case FUNC_XOR: scriptEng.operands[0] ^= scriptEng.operands[1]; break;
            case FUNC_MOD: scriptEng.operands[0] %= scriptEng.operands[1]; break;
            case FUNC_FLIPSIGN: scriptEng.operands[0] = -scriptEng.operands[0]; break;
            case FUNC_CHECKEQUAL:
                scriptEng.checkResult = scriptEng.operands[0] == scriptEng.operands[1];
                opcodeSize            = 0;
                break;
            case FUNC_CHECKGREATER:
                scriptEng.checkResult = scriptEng.operands[0] > scriptEng.operands[1];
                opcodeSize            = 0;
                break;
            case FUNC_CHECKLOWER:
                scriptEng.checkResult = scriptEng.operands[0] < scriptEng.operands[1];
                opcodeSize            = 0;
                break;
            case FUNC_CHECKNOTEQUAL:
                scriptEng.checkResult = scriptEng.operands[0] != scriptEng.operands[1];
                opcodeSize            = 0;
                break;
            case FUNC_IFEQUAL:
                if (scriptEng.operands[1] != scriptEng.operands[2])
                    scriptDataPtr = scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFGREATER:
                if (scriptEng.operands[1] <= scriptEng.operands[2])
                    scriptDataPtr = scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFGREATEROREQUAL:
                if (scriptEng.operands[1] < scriptEng.operands[2])
                    scriptDataPtr = scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0]];
                jumpTableStack[+jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                         = 0;
                break;
            case FUNC_IFLOWER:
                if (scriptEng.operands[1] >= scriptEng.operands[2])
                    scriptDataPtr = scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFLOWEROREQUAL:
                if (scriptEng.operands[1] > scriptEng.operands[2])
                    scriptDataPtr = scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFNOTEQUAL:
                if (scriptEng.operands[1] == scriptEng.operands[2])
                    scriptDataPtr = scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_ELSE:
                opcodeSize    = 0;
                scriptDataPtr = scriptCodePtr
                                + jumpTableData[jumpTablePtr + jumpTableStack[jumpTableStackPos--] + 1];
                break;
            case FUNC_ENDIF:
                opcodeSize = 0;
                --jumpTableStackPos;
                break;
            case FUNC_WEQUAL:
                if (scriptEng.operands[1] != scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WGREATER:
                if (scriptEng.operands[1] <= scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WGREATEROREQUAL:
                if (scriptEng.operands[1] < scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WLOWER:
                if (scriptEng.operands[1] >= scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WLOWEROREQUAL:
                if (scriptEng.operands[1] > scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WNOTEQUAL:
                if (scriptEng.operands[1] == scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_LOOP:
                opcodeSize = 0;
                scriptDataPtr =
                    scriptCodePtr + jumpTableData[jumpTablePtr + jumpTableStack[jumpTableStackPos--]];
                break;
            case FUNC_FOREACHACTIVE: {
                int typeGroup = scriptEng.operands[1];
                if (typeGroup < TYPEGROUP_COUNT) {
                    int loop                      = foreachStack[++foreachStackPos] + 1;
                    foreachStack[foreachStackPos] = loop;
                    if (loop >= objectTypeGroupList[typeGroup].listSize) {
                        opcodeSize                      = 0;
                        foreachStack[foreachStackPos--] = -1;
                        scriptDataPtr =
                            scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                        break;
                    }
                    else {
                        scriptEng.operands[2] = objectTypeGroupList[typeGroup].entityRefs[loop];
                        jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                    }
                }
                else {
                    opcodeSize = 0;
                    scriptDataPtr =
                        scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                }
                break;
            }
            case FUNC_FOREACHALL: {
                int objType = scriptEng.operands[1];
                if (objType < OBJECT_COUNT) {
                    int loop                      = foreachStack[++foreachStackPos] + 1;
                    foreachStack[foreachStackPos] = loop;

                    if (scriptEvent == EVENT_RSDKLOAD) {
                        while (true) {
                            if (loop >= TEMPENTITY_START) {
                                opcodeSize                      = 0;
                                foreachStack[foreachStackPos--] = -1;
                                int off       = jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                                scriptDataPtr = scriptCodePtr + off;
                                break;
                            }
                            else if (objType == objectEntityList[loop].type) {
                                scriptEng.operands[2]               = loop;
                                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                                break;
                            }
                            else {
                                foreachStack[foreachStackPos] = ++loop;
                            }
                        }
                    }
                    else {
                        while (true) {
                            if (loop >= ENTITY_COUNT) {
                                opcodeSize                      = 0;
                                foreachStack[foreachStackPos--] = -1;
                                scriptDataPtr =
                                    scriptCodePtr
                                    + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                                break;
                            }
                            else if (objType == objectEntityList[loop].type) {
                                scriptEng.operands[2]               = loop;
                                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                                break;
                            }
                            else {
                                foreachStack[foreachStackPos] = ++loop;
                            }
                        }
                    }
                }
                else {
                    opcodeSize = 0;
                    scriptDataPtr =
                        scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1];
                }
                break;
            }
            case FUNC_NEXT:
                opcodeSize = 0;
                scriptDataPtr =
                    scriptCodePtr + jumpTableData[jumpTablePtr + jumpTableStack[jumpTableStackPos--]];
                --foreachStackPos;
                break;
            case FUNC_SWITCH:
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                if (scriptEng.operands[1] < jumpTableData[jumpTablePtr + scriptEng.operands[0]]
                    || scriptEng.operands[1] > jumpTableData[jumpTablePtr + scriptEng.operands[0] + 1])
                    scriptDataPtr =
                        scriptCodePtr + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 2];
                else
                    scriptDataPtr =
                        scriptCodePtr
                        + jumpTableData[jumpTablePtr + scriptEng.operands[0] + 4
                                        + (scriptEng.operands[1]
                                           - jumpTableData[jumpTablePtr + scriptEng.operands[0]])];
                opcodeSize = 0;
                break;
            case FUNC_BREAK:
                opcodeSize    = 0;
                scriptDataPtr = scriptCodePtr
                                + jumpTableData[jumpTablePtr + jumpTableStack[jumpTableStackPos--] + 3];
                break;
            case FUNC_ENDSWITCH:
                opcodeSize = 0;
                --jumpTableStackPos;
                break;
            case FUNC_RAND: scriptEng.operands[0] = rand() % scriptEng.operands[1]; break;
            case FUNC_SIN: {
                scriptEng.operands[0] = sin512(scriptEng.operands[1]);
                break;
            }
            case FUNC_COS: {
                scriptEng.operands[0] = cos512(scriptEng.operands[1]);
                break;
            }
            case FUNC_SIN256: {
                scriptEng.operands[0] = sin256(scriptEng.operands[1]);
                break;
            }
            case FUNC_COS256: {
                scriptEng.operands[0] = cos256(scriptEng.operands[1]);
                break;
            }
            case FUNC_ATAN2: {
                scriptEng.operands[0] = ArcTanLookup(scriptEng.operands[1], scriptEng.operands[2]);
                break;
            }
            case FUNC_INTERPOLATE:
                scriptEng.operands[0] = (scriptEng.operands[2] * (0x100 - scriptEng.operands[3])
                                         + scriptEng.operands[3] * scriptEng.operands[1])
                                        >> 8;
                break;
            case FUNC_INTERPOLATEXY:
                scriptEng.operands[0] = (scriptEng.operands[3] * (0x100 - scriptEng.operands[6]) >> 8)
                                        + ((scriptEng.operands[6] * scriptEng.operands[2]) >> 8);
                scriptEng.operands[1] = (scriptEng.operands[5] * (0x100 - scriptEng.operands[6]) >> 8)
                                        + (scriptEng.operands[6] * scriptEng.operands[4] >> 8);
                break;
            case FUNC_LOADSPRITESHEET:
                opcodeSize                = 0;
                scriptInfo->spriteSheetID = editor->loadSpriteSheet(scriptText);
                break;
            case FUNC_REMOVESPRITESHEET:
                opcodeSize = 0;
                editor->viewer->removeGraphicsFile(scriptText, -1);
                break;
            case FUNC_DRAWSPRITE:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->drawSpriteFlipped((entity->XPos >> 16) + spriteFrame->pivotX,
                                          (entity->YPos >> 16) + spriteFrame->pivotY,
                                          spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                          spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                          scriptInfo->spriteSheetID, false);
                break;
            case FUNC_DRAWSPRITEXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->drawSpriteFlipped((scriptEng.operands[1] >> 16) + spriteFrame->pivotX,
                                          (scriptEng.operands[2] >> 16) + spriteFrame->pivotY,
                                          spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                          spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                          scriptInfo->spriteSheetID, false);
                break;
            case FUNC_DRAWSPRITESCREENXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->drawSpriteFlipped(scriptEng.operands[1] + spriteFrame->pivotX,
                                          scriptEng.operands[2] + spriteFrame->pivotY,
                                          spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                          spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                          scriptInfo->spriteSheetID, true);
                break;
            case FUNC_DRAWTINTRECT: opcodeSize = 0; break;
            case FUNC_DRAWNUMBERS: {
                opcodeSize = 0;
                int i      = 10;
                if (scriptEng.operands[6]) {
                    while (scriptEng.operands[4] > 0) {
                        int frameID = scriptEng.operands[3] % i / (i / 10) + scriptEng.operands[0];
                        spriteFrame = &scriptFrames[scriptInfo->frameListOffset + frameID];
                        editor->drawSpriteFlipped(spriteFrame->pivotX + scriptEng.operands[1],
                                                  spriteFrame->pivotY + scriptEng.operands[2],
                                                  spriteFrame->width, spriteFrame->height,
                                                  spriteFrame->sprX, spriteFrame->sprY, FLIP_NONE,
                                                  INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        scriptEng.operands[1] -= scriptEng.operands[5];
                        i *= 10;
                        --scriptEng.operands[4];
                    }
                }
                else {
                    int extra = 10;
                    if (scriptEng.operands[3])
                        extra = 10 * scriptEng.operands[3];
                    while (scriptEng.operands[4] > 0) {
                        if (extra >= i) {
                            int frameID = scriptEng.operands[3] % i / (i / 10) + scriptEng.operands[0];
                            spriteFrame = &scriptFrames[scriptInfo->frameListOffset + frameID];
                            editor->drawSpriteFlipped(spriteFrame->pivotX + scriptEng.operands[1],
                                                      spriteFrame->pivotY + scriptEng.operands[2],
                                                      spriteFrame->width, spriteFrame->height,
                                                      spriteFrame->sprX, spriteFrame->sprY, FLIP_NONE,
                                                      INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        }
                        scriptEng.operands[1] -= scriptEng.operands[5];
                        i *= 10;
                        --scriptEng.operands[4];
                    }
                }
                break;
            }
            case FUNC_DRAWACTNAME: {
                opcodeSize = 0;
                break;
            }
            case FUNC_DRAWMENU: opcodeSize = 0; break;
            case FUNC_SPRITEFRAME:
                opcodeSize = 0;
                if (scriptEvent == EVENT_RSDKLOAD && scriptFrameCount < SPRITEFRAME_COUNT) {
                    scriptFrames[scriptFrameCount].pivotX = scriptEng.operands[0];
                    scriptFrames[scriptFrameCount].pivotY = scriptEng.operands[1];
                    scriptFrames[scriptFrameCount].width  = scriptEng.operands[2];
                    scriptFrames[scriptFrameCount].height = scriptEng.operands[3];
                    scriptFrames[scriptFrameCount].sprX   = scriptEng.operands[4];
                    scriptFrames[scriptFrameCount].sprY   = scriptEng.operands[5];
                    ++scriptFrameCount;
                }
                break;
            case FUNC_EDITFRAME: {
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];

                spriteFrame->pivotX = scriptEng.operands[1];
                spriteFrame->pivotY = scriptEng.operands[2];
                spriteFrame->width  = scriptEng.operands[3];
                spriteFrame->height = scriptEng.operands[4];
                spriteFrame->sprX   = scriptEng.operands[5];
                spriteFrame->sprY   = scriptEng.operands[6];
                break;
            }
            case FUNC_LOADPALETTE: opcodeSize = 0; break;
            case FUNC_ROTATEPALETTE: opcodeSize = 0; break;
            case FUNC_SETSCREENFADE: opcodeSize = 0; break;
            case FUNC_SETACTIVEPALETTE: opcodeSize = 0; break;
            case FUNC_SETPALETTEFADE: break;
            case FUNC_SETPALETTEENTRY: opcodeSize = 0; break;
            case FUNC_GETPALETTEENTRY: opcodeSize = 0; break;
            case FUNC_COPYPALETTE: opcodeSize = 0; break;
            case FUNC_CLEARSCREEN: opcodeSize = 0; break;
            case FUNC_DRAWSPRITEFX:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                switch (scriptEng.operands[1]) {
                    default: break;
                    case FX_SCALE:
                        editor->drawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16), (scriptEng.operands[3] >> 16),
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->width,
                            spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, entity->scale,
                            entity->scale, entity->direction, 0, INK_NONE, 0xFF,
                            scriptInfo->spriteSheetID, false);
                        break;
                    case FX_ROTATE:
                        editor->drawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16), (scriptEng.operands[3] >> 16),
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->width,
                            spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, 0x200, 0x200,
                            entity->direction, entity->rotation, INK_NONE, 0xFF,
                            scriptInfo->spriteSheetID, false);
                        break;
                    case FX_ROTOZOOM:
                        editor->drawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16), (scriptEng.operands[3] >> 16),
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->width,
                            spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, entity->scale,
                            entity->scale, entity->direction, entity->rotation, INK_NONE, 0xFF,
                            scriptInfo->spriteSheetID, false);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_BLEND:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_BLEND, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_ALPHA:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_ALPHA, entity->alpha,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_ADD:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_ADD, entity->alpha,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_SUB:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_SUB, entity->alpha,
                                    scriptInfo->spriteSheetID, false);
                                break;
                        }
                        break;
                    case FX_TINT: break;
                    case FX_FLIP:
                        switch (entity->direction) {
                            default:
                            case FLIP_NONE:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case FLIP_X:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) - spriteFrame->width
                                        - spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_X, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case FLIP_Y:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) - spriteFrame->height
                                        - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_Y, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case FLIP_XY:
                                editor->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) - spriteFrame->width
                                        - spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) - spriteFrame->height
                                        - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_XY, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                        }
                        break;
                }
                break;
            case FUNC_DRAWSPRITESCREENFX:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                switch (scriptEng.operands[1]) {
                    default: break;
                    case FX_SCALE:
                        editor->drawSpriteRotozoom(
                            scriptEng.operands[2], scriptEng.operands[3], -spriteFrame->pivotX,
                            -spriteFrame->pivotY, spriteFrame->width, spriteFrame->height,
                            spriteFrame->sprX, spriteFrame->sprY, entity->scale, entity->scale,
                            entity->direction, 0, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        break;
                    case FX_ROTATE:
                        editor->drawSpriteRotozoom(
                            scriptEng.operands[2], scriptEng.operands[3], -spriteFrame->pivotX,
                            -spriteFrame->pivotY, spriteFrame->width, spriteFrame->height,
                            spriteFrame->sprX, spriteFrame->sprY, 0x200, 0x200, entity->direction,
                            entity->rotation, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        break;
                    case FX_ROTOZOOM:
                        editor->drawSpriteRotozoom(scriptEng.operands[2], scriptEng.operands[3],
                                                   -spriteFrame->pivotX, -spriteFrame->pivotY,
                                                   spriteFrame->width, spriteFrame->height,
                                                   spriteFrame->sprX, spriteFrame->sprY, entity->scale,
                                                   entity->scale, entity->direction, entity->rotation,
                                                   INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case INK_BLEND:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_BLEND, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case INK_ALPHA:
                                editor->drawSpriteFlipped(scriptEng.operands[2] + spriteFrame->pivotX,
                                                          scriptEng.operands[3] + spriteFrame->pivotY,
                                                          spriteFrame->width, spriteFrame->height,
                                                          spriteFrame->sprX, spriteFrame->sprY,
                                                          FLIP_NONE, INK_ALPHA, entity->alpha,
                                                          scriptInfo->spriteSheetID, true);
                                break;
                            case INK_ADD:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_ADD, entity->alpha, scriptInfo->spriteSheetID, true);
                                break;
                            case INK_SUB:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_SUB, entity->alpha, scriptInfo->spriteSheetID, true);
                                break;
                        }
                        break;
                    case FX_TINT: break;
                    case FX_FLIP:
                        switch (entity->direction) {
                            default:
                            case FLIP_NONE:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case FLIP_X:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_X,
                                    INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case FLIP_Y:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_Y,
                                    INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case FLIP_XY:
                                editor->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_XY,
                                    INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                        }
                        break;
                }
                break;
            case FUNC_LOADANIMATION: opcodeSize = 0; break;
            case FUNC_SETUPMENU: {
                opcodeSize = 0;
                break;
            }
            case FUNC_ADDMENUENTRY: {
                opcodeSize = 0;
                break;
            }
            case FUNC_EDITMENUENTRY: {
                opcodeSize = 0;
                break;
            }
            case FUNC_LOADSTAGE: opcodeSize = 0; break;
            case FUNC_DRAWRECT:
                opcodeSize = 0;
                editor->viewer->drawRect(
                    scriptEng.operands[0], scriptEng.operands[1], scriptEng.operands[2],
                    scriptEng.operands[3],
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, scriptEng.operands[7] / 255.0f),
                    false, 0xFF, INK_NONE);
                break;
            case FUNC_RESETOBJECTENTITY: {
                opcodeSize = 0;
                break;
            }
            case FUNC_BOXCOLLISIONTEST: opcodeSize = 0; break;
            case FUNC_CREATETEMPOBJECT: {
                opcodeSize = 0;
                break;
            }
            case FUNC_PROCESSOBJECTMOVEMENT: opcodeSize = 0; break;
            case FUNC_PROCESSOBJECTCONTROL: opcodeSize = 0; break;
            case FUNC_PROCESSANIMATION: opcodeSize = 0; break;
            case FUNC_DRAWOBJECTANIMATION: opcodeSize = 0; break;
            case FUNC_SETMUSICTRACK: opcodeSize = 0; break;
            case FUNC_PLAYMUSIC: opcodeSize = 0; break;
            case FUNC_STOPMUSIC: opcodeSize = 0; break;
            case FUNC_PAUSEMUSIC: opcodeSize = 0; break;
            case FUNC_RESUMEMUSIC: opcodeSize = 0; break;
            case FUNC_SWAPMUSICTRACK: opcodeSize = 0; break;
            case FUNC_PLAYSFX: opcodeSize = 0; break;
            case FUNC_STOPSFX: opcodeSize = 0; break;
            case FUNC_SETSFXATTRIBUTES: opcodeSize = 0; break;
            case FUNC_OBJECTTILECOLLISION: opcodeSize = 0; break;
            case FUNC_OBJECTTILEGRIP: opcodeSize = 0; break;
            case FUNC_NOT: scriptEng.operands[0] = ~scriptEng.operands[0]; break;
            case FUNC_DRAW3DSCENE: opcodeSize = 0; break;
            case FUNC_SETIDENTITYMATRIX: opcodeSize = 0; break;
            case FUNC_MATRIXMULTIPLY: opcodeSize = 0; break;
            case FUNC_MATRIXTRANSLATEXYZ: opcodeSize = 0; break;
            case FUNC_MATRIXSCALEXYZ: opcodeSize = 0; break;
            case FUNC_MATRIXROTATEX: opcodeSize = 0; break;
            case FUNC_MATRIXROTATEY: opcodeSize = 0; break;
            case FUNC_MATRIXROTATEZ: opcodeSize = 0; break;
            case FUNC_MATRIXROTATEXYZ: opcodeSize = 0; break;
            case FUNC_MATRIXINVERSE: opcodeSize = 0; break;
            case FUNC_TRANSFORMVERTICES: opcodeSize = 0; break;
            case FUNC_CALLFUNCTION:
                opcodeSize                        = 0;
                functionStack[functionStackPos++] = scriptDataPtr;
                functionStack[functionStackPos++] = jumpTablePtr;
                functionStack[functionStackPos++] = scriptCodePtr;
                scriptCodePtr                     = functionList[scriptEng.operands[0]].scriptCodePtr;
                jumpTablePtr                      = functionList[scriptEng.operands[0]].jumpTablePtr;
                scriptDataPtr                     = scriptCodePtr;
                break;
            case FUNC_RETURN:
                opcodeSize = 0;
                if (!functionStackPos) { // event, stop running
                    running = false;
                }
                else { // function, jump out
                    scriptCodePtr = functionStack[--functionStackPos];
                    jumpTablePtr  = functionStack[--functionStackPos];
                    scriptDataPtr = functionStack[--functionStackPos];
                }
                break;
            case FUNC_SETLAYERDEFORMATION: opcodeSize = 0; break;
            case FUNC_CHECKTOUCHRECT: opcodeSize = 0; break;
            case FUNC_GETTILELAYERENTRY: opcodeSize = 0; break;
            case FUNC_SETTILELAYERENTRY: opcodeSize = 0; break;
            case FUNC_GETBIT:
                scriptEng.operands[0] =
                    (scriptEng.operands[1] & (1 << scriptEng.operands[2])) >> scriptEng.operands[2];
                break;
            case FUNC_SETBIT:
                if (scriptEng.operands[2] <= 0)
                    scriptEng.operands[0] &= ~(1 << scriptEng.operands[1]);
                else
                    scriptEng.operands[0] |= 1 << scriptEng.operands[1];
                break;
            case FUNC_CLEARDRAWLIST: opcodeSize = 0; break;
            case FUNC_ADDDRAWLISTENTITYREF: {
                opcodeSize = 0;
                // drawListEntries[scriptEng.operands[0]].entityRefs[drawListEntries[scriptEng.operands[0]].listSize++]
                // = scriptEng.operands[1];
                break;
            }
            case FUNC_GETDRAWLISTENTITYREF:
                // scriptEng.operands[0] =
                // drawListEntries[scriptEng.operands[1]].entityRefs[scriptEng.operands[2]];
                break;
            case FUNC_SETDRAWLISTENTITYREF:
                opcodeSize = 0;
                // drawListEntries[scriptEng.operands[1]].entityRefs[scriptEng.operands[2]] =
                // scriptEng.operands[0];
                break;
            case FUNC_GET16X16TILEINFO: {
                opcodeSize = 0;
                break;
            }
            case FUNC_SET16X16TILEINFO: {
                opcodeSize = 0;
                break;
            }
            case FUNC_COPY16X16TILE: opcodeSize = 0; break;
            case FUNC_GETANIMATIONBYNAME: {
                opcodeSize = 0;
                break;
            }
            case FUNC_READSAVERAM: opcodeSize = 0; break;
            case FUNC_WRITESAVERAM: opcodeSize = 0; break;
            case FUNC_LOADTEXTFILE: {
                opcodeSize = 0;
                break;
            }
            case FUNC_GETTEXTINFO: {
                opcodeSize = 0;
                break;
            }
            case FUNC_GETVERSIONNUMBER: {
                opcodeSize = 0;
                break;
            }
            case FUNC_GETTABLEVALUE: {
                int arrPos = scriptEng.operands[1];
                if (arrPos >= 0) {
                    int pos     = scriptEng.operands[2];
                    int arrSize = scriptData[pos];
                    if (arrPos < arrSize)
                        scriptEng.operands[0] = scriptData[pos + arrPos + 1];
                }
                break;
            }
            case FUNC_SETTABLEVALUE: {
                opcodeSize = 0;
                int arrPos = scriptEng.operands[1];
                if (arrPos >= 0) {
                    int pos     = scriptEng.operands[2];
                    int arrSize = scriptData[pos];
                    if (arrPos < arrSize)
                        scriptData[pos + arrPos + 1] = scriptEng.operands[0];
                }
                break;
            }
            case FUNC_CHECKCURRENTSTAGEFOLDER:
                opcodeSize            = 0;
                scriptEng.checkResult = editor->viewer->currentFolder == scriptText;
                break;
            case FUNC_ABS: {
                scriptEng.operands[0] = abs(scriptEng.operands[0]);
                break;
            }
            case FUNC_CALLNATIVEFUNCTION: opcodeSize = 0; break;
            case FUNC_CALLNATIVEFUNCTION2: opcodeSize = 0; break;
            case FUNC_CALLNATIVEFUNCTION4: opcodeSize = 0; break;
            case FUNC_SETOBJECTRANGE: {
                opcodeSize = 0;
                break;
            }
            case FUNC_GETOBJECTVALUE: {
                int valID = scriptEng.operands[1];
                if (valID <= 47)
                    scriptEng.operands[0] = objectEntityList[scriptEng.operands[2]].values[valID];
                break;
            }
            case FUNC_SETOBJECTVALUE: {
                opcodeSize = 0;
                int valID  = scriptEng.operands[1];
                if (valID <= 47)
                    objectEntityList[scriptEng.operands[2]].values[valID] = scriptEng.operands[0];
                break;
            }
            case FUNC_COPYOBJECT: {
                opcodeSize = 0;
                // start index, copy offset, count
                Entity *src = &objectEntityList[scriptEng.operands[0]];
                for (int e = 0; e < scriptEng.operands[2]; ++e) {
                    Entity *dst = &src[scriptEng.operands[1]];
                    ++src;
                    memcpy(dst, src, sizeof(Entity));
                }
                break;
            }
            case FUNC_PRINT: {
                if (scriptEng.operands[1])
                    printLog(QString::number(scriptEng.operands[0]));
                else
                    printLog(scriptText);

                if (scriptEng.operands[2])
                    printLog("\n");
                break;
            }
            case FUNC_ADDEDITORVAR: {
                opcodeSize = 0;
                if (scriptEvent == EVENT_RSDKLOAD) {
                    editor->viewer->addEditorVariable(scriptText);
                }
                break;
            }
            case FUNC_SETACTIVEVAR: {
                opcodeSize = 0;
                if (scriptEvent == EVENT_RSDKLOAD) {
                    editor->viewer->setActiveVariable(scriptText);
                }
                break;
            }
            case FUNC_ADDENUMVAR: {
                opcodeSize = 0;
                if (scriptEvent == EVENT_RSDKLOAD) {
                    editor->viewer->addEnumVariable(scriptText, scriptEng.operands[1]);
                }
                break;
            }
            case FUNC_SETVARALIAS: {
                opcodeSize = 0;
                if (scriptEvent == EVENT_RSDKLOAD) {
                    editor->viewer->setVariableAlias(scriptEng.operands[0], scriptText);
                }
                break;
            }
            case FUNC_DRAWLINE: {
                opcodeSize = 0;

                Vector4<float> colour =
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, 1.0);

                editor->viewer->drawLine((scriptEng.operands[0] >> 16) - editor->viewer->cameraPos.x,
                                         (scriptEng.operands[1] >> 16) - editor->viewer->cameraPos.y,
                                         (scriptEng.operands[2] >> 16) - editor->viewer->cameraPos.x,
                                         (scriptEng.operands[3] >> 16) - editor->viewer->cameraPos.y,
                                         colour, entity->alpha, (InkEffects)entity->inkEffect);
                break;
            }
            case FUNC_DRAWARROW: {
                opcodeSize = 0;

                int angle = ArcTanLookup(scriptEng.operands[0] - scriptEng.operands[2],
                                         scriptEng.operands[1] - scriptEng.operands[3]);
                Vector4<float> colour =
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, 1.0);
                InkEffects ink = (InkEffects)entity->inkEffect;

                int x1 = (scriptEng.operands[0] >> 16) - editor->viewer->cameraPos.x;
                int y1 = (scriptEng.operands[1] >> 16) - editor->viewer->cameraPos.y;
                int x2 = (scriptEng.operands[2] >> 16) - editor->viewer->cameraPos.x;
                int y2 = (scriptEng.operands[3] >> 16) - editor->viewer->cameraPos.y;

                editor->viewer->drawLine(x1, y1, x2, y2, colour, entity->alpha, ink);
                editor->viewer->drawLine(x2, y2, x2 + ((cos256(angle + 12) << 12) >> 16),
                                         y2 + ((sin256(angle + 12) << 12) >> 16), colour, entity->alpha,
                                         ink);
                editor->viewer->drawLine(x2, y2, x2 + ((cos256(angle - 12) << 12) >> 16),
                                         y2 + ((sin256(angle - 12) << 12) >> 16), colour, entity->alpha,
                                         ink);
                break;
            }
        }

        // Set Values
        if (opcodeSize > 0)
            scriptDataPtr -= scriptDataPtr - scriptCodeOffset;
        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = scriptData[scriptDataPtr++];
            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (scriptData[scriptDataPtr++]) { // variable
                    case VARARR_NONE: arrayVal = objectEntityPos; break;
                    case VARARR_ARRAY:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal = scriptEng.arrayPosition[scriptData[scriptDataPtr++]];
                        else
                            arrayVal = scriptData[scriptDataPtr++];
                        break;
                    case VARARR_ENTNOPLUS1:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                objectEntityPos + scriptEng.arrayPosition[scriptData[scriptDataPtr++]];
                        else
                            arrayVal = objectEntityPos + scriptData[scriptDataPtr++];
                        break;
                    case VARARR_ENTNOMINUS1:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                objectEntityPos - scriptEng.arrayPosition[scriptData[scriptDataPtr++]];
                        else
                            arrayVal = objectEntityPos - scriptData[scriptDataPtr++];
                        break;
                    default: break;
                }

                // Allows the game to link to the editor properly
                Entity *entityPtr = &objectEntityList[arrayVal];
                if (editor) {
                    for (int e = 0; e < editor->viewer->entities.count(); ++e) {
                        if (arrayVal == editor->viewer->entities[e].gameEntitySlot) {
                            entityPtr = &objectEntityList[e];
                        }
                    }
                }

                // Variables
                switch (scriptData[scriptDataPtr++]) {
                    default: break;
                    case VAR_TEMP0: scriptEng.tempValue[0] = scriptEng.operands[i]; break;
                    case VAR_TEMP1: scriptEng.tempValue[1] = scriptEng.operands[i]; break;
                    case VAR_TEMP2: scriptEng.tempValue[2] = scriptEng.operands[i]; break;
                    case VAR_TEMP3: scriptEng.tempValue[3] = scriptEng.operands[i]; break;
                    case VAR_TEMP4: scriptEng.tempValue[4] = scriptEng.operands[i]; break;
                    case VAR_TEMP5: scriptEng.tempValue[5] = scriptEng.operands[i]; break;
                    case VAR_TEMP6: scriptEng.tempValue[6] = scriptEng.operands[i]; break;
                    case VAR_TEMP7: scriptEng.tempValue[7] = scriptEng.operands[i]; break;
                    case VAR_CHECKRESULT: scriptEng.checkResult = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS0: scriptEng.arrayPosition[0] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS1: scriptEng.arrayPosition[1] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS2: scriptEng.arrayPosition[2] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS3: scriptEng.arrayPosition[3] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS4: scriptEng.arrayPosition[4] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS5: scriptEng.arrayPosition[5] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS6: scriptEng.arrayPosition[6] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS7: scriptEng.arrayPosition[7] = scriptEng.operands[i]; break;
                    case VAR_GLOBAL: /*globalVariables[arrayVal] = scriptEng.operands[i];*/ break;
                    case VAR_LOCAL: scriptData[arrayVal] = scriptEng.operands[i]; break;
                    case VAR_OBJECTENTITYPOS: objectEntityPos = scriptEng.operands[i]; break;
                    case VAR_OBJECTGROUPID: {
                        entityPtr->typeGroup = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTTYPE: {
                        entityPtr->type = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPROPERTYVALUE: {
                        entityPtr->propertyValue = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTXPOS: {
                        entityPtr->XPos = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTYPOS: {
                        entityPtr->YPos = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTIXPOS: {
                        entityPtr->XPos = scriptEng.operands[i] << 16;
                        break;
                    }
                    case VAR_OBJECTIYPOS: {
                        entityPtr->YPos = scriptEng.operands[i] << 16;
                        break;
                    }
                    case VAR_OBJECTXVEL: {
                        entityPtr->XVelocity = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTYVEL: {
                        entityPtr->YVelocity = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSPEED: {
                        entityPtr->speed = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSTATE: {
                        entityPtr->state = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTROTATION: {
                        entityPtr->rotation = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSCALE: {
                        entityPtr->scale = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPRIORITY: {
                        entityPtr->priority = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDRAWORDER: {
                        entityPtr->drawOrder = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDIRECTION: {
                        entityPtr->direction = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTINKEFFECT: {
                        entityPtr->inkEffect = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTALPHA: {
                        entityPtr->alpha = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFRAME: {
                        entityPtr->frame = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATION: {
                        entityPtr->animation = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPREVANIMATION: {
                        entityPtr->prevAnimation = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATIONSPEED: {
                        entityPtr->animationSpeed = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATIONTIMER: {
                        entityPtr->animationTimer = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANGLE: {
                        entityPtr->angle = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTLOOKPOSX: {
                        entityPtr->lookPosX = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTLOOKPOSY: {
                        entityPtr->lookPosY = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCOLLISIONMODE: {
                        entityPtr->collisionMode = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCOLLISIONPLANE: {
                        entityPtr->collisionPlane = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCONTROLMODE: {
                        entityPtr->controlMode = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCONTROLLOCK: {
                        entityPtr->controlLock = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPUSHING: {
                        entityPtr->pushing = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVISIBLE: {
                        entityPtr->visible = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTTILECOLLISIONS: {
                        entityPtr->tileCollisions = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTINTERACTION: {
                        entityPtr->objectInteractions = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTGRAVITY: {
                        entityPtr->gravity = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTUP: {
                        entityPtr->up = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDOWN: {
                        entityPtr->down = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTLEFT: {
                        entityPtr->left = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTRIGHT: {
                        entityPtr->right = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTJUMPPRESS: {
                        entityPtr->jumpPress = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTJUMPHOLD: {
                        entityPtr->jumpHold = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSCROLLTRACKING: {
                        entityPtr->trackScroll = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORL: {
                        entityPtr->flailing[0] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORC: {
                        entityPtr->flailing[1] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORR: {
                        entityPtr->flailing[2] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORLC: {
                        entityPtr->flailing[3] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORRC: {
                        entityPtr->flailing[4] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCOLLISIONLEFT: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONTOP: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONRIGHT: {
                        break;
                    }
                    case VAR_OBJECTCOLLISIONBOTTOM: {
                        break;
                    }
                    case VAR_OBJECTOUTOFBOUNDS: {
                        break;
                    }
                    case VAR_OBJECTSPRITESHEET: {
                        objectScriptList[entityPtr->type].spriteSheetID = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE0: {
                        entityPtr->values[0] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE1: {
                        entityPtr->values[1] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE2: {
                        entityPtr->values[2] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE3: {
                        entityPtr->values[3] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE4: {
                        entityPtr->values[4] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE5: {
                        entityPtr->values[5] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE6: {
                        entityPtr->values[6] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE7: {
                        entityPtr->values[7] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE8: {
                        entityPtr->values[8] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE9: {
                        entityPtr->values[9] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE10: {
                        entityPtr->values[10] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE11: {
                        entityPtr->values[11] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE12: {
                        entityPtr->values[12] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE13: {
                        entityPtr->values[13] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE14: {
                        entityPtr->values[14] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE15: {
                        entityPtr->values[15] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE16: {
                        entityPtr->values[16] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE17: {
                        entityPtr->values[17] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE18: {
                        entityPtr->values[18] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE19: {
                        entityPtr->values[19] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE20: {
                        entityPtr->values[20] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE21: {
                        entityPtr->values[21] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE22: {
                        entityPtr->values[22] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE23: {
                        entityPtr->values[23] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE24: {
                        entityPtr->values[24] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE25: {
                        entityPtr->values[25] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE26: {
                        entityPtr->values[26] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE27: {
                        entityPtr->values[27] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE28: {
                        entityPtr->values[28] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE29: {
                        entityPtr->values[29] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE30: {
                        entityPtr->values[30] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE31: {
                        entityPtr->values[31] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE32: {
                        entityPtr->values[32] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE33: {
                        entityPtr->values[33] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE34: {
                        entityPtr->values[34] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE35: {
                        entityPtr->values[35] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE36: {
                        entityPtr->values[36] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE37: {
                        entityPtr->values[37] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE38: {
                        entityPtr->values[38] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE39: {
                        entityPtr->values[39] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE40: {
                        entityPtr->values[40] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE41: {
                        entityPtr->values[41] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE42: {
                        entityPtr->values[42] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE43: {
                        entityPtr->values[43] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE44: {
                        entityPtr->values[44] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE45: {
                        entityPtr->values[45] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE46: {
                        entityPtr->values[46] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE47: {
                        entityPtr->values[47] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_STAGESTATE: break;
                    case VAR_STAGEACTIVELIST: break;
                    case VAR_STAGELISTPOS: break;
                    case VAR_STAGETIMEENABLED: break;
                    case VAR_STAGEMILLISECONDS: break;
                    case VAR_STAGESECONDS: break;
                    case VAR_STAGEMINUTES: break;
                    case VAR_STAGEACTNUM: break;
                    case VAR_STAGEPAUSEENABLED: break;
                    case VAR_STAGELISTSIZE: break;
                    case VAR_STAGENEWXBOUNDARY1: break;
                    case VAR_STAGENEWXBOUNDARY2: break;
                    case VAR_STAGENEWYBOUNDARY1: break;
                    case VAR_STAGENEWYBOUNDARY2: break;
                    case VAR_STAGECURXBOUNDARY1: break;
                    case VAR_STAGECURXBOUNDARY2: break;
                    case VAR_STAGECURYBOUNDARY1: break;
                    case VAR_STAGECURYBOUNDARY2: break;
                    case VAR_STAGEDEFORMATIONDATA0: break;
                    case VAR_STAGEDEFORMATIONDATA1: break;
                    case VAR_STAGEDEFORMATIONDATA2: break;
                    case VAR_STAGEDEFORMATIONDATA3: break;
                    case VAR_STAGEWATERLEVEL: break;
                    case VAR_STAGEACTIVELAYER: break;
                    case VAR_STAGEMIDPOINT: break;
                    case VAR_STAGEPLAYERLISTPOS: break;
                    case VAR_STAGEDEBUGMODE: break;
                    case VAR_STAGEENTITYPOS: break;
                    case VAR_SCREENCAMERAENABLED: break;
                    case VAR_SCREENCAMERATARGET: break;
                    case VAR_SCREENCAMERASTYLE: break;
                    case VAR_SCREENCAMERAX: break;
                    case VAR_SCREENCAMERAY: break;
                    case VAR_SCREENDRAWLISTSIZE: break;
                    case VAR_SCREENXCENTER: break;
                    case VAR_SCREENYCENTER: break;
                    case VAR_SCREENXSIZE: break;
                    case VAR_SCREENYSIZE: break;
                    case VAR_SCREENXOFFSET: break;
                    case VAR_SCREENYOFFSET: break;
                    case VAR_SCREENSHAKEX: break;
                    case VAR_SCREENSHAKEY: break;
                    case VAR_SCREENADJUSTCAMERAY: break;
                    case VAR_TOUCHSCREENDOWN: break;
                    case VAR_TOUCHSCREENXPOS: break;
                    case VAR_TOUCHSCREENYPOS: break;
                    case VAR_MUSICVOLUME: break;
                    case VAR_MUSICCURRENTTRACK: break;
                    case VAR_MUSICPOSITION: break;
                    case VAR_INPUTDOWNUP: break;
                    case VAR_INPUTDOWNDOWN: break;
                    case VAR_INPUTDOWNLEFT: break;
                    case VAR_INPUTDOWNRIGHT: break;
                    case VAR_INPUTDOWNBUTTONA: break;
                    case VAR_INPUTDOWNBUTTONB: break;
                    case VAR_INPUTDOWNBUTTONC: break;
                    case VAR_INPUTDOWNBUTTONX: break;
                    case VAR_INPUTDOWNBUTTONY: break;
                    case VAR_INPUTDOWNBUTTONZ: break;
                    case VAR_INPUTDOWNBUTTONL: break;
                    case VAR_INPUTDOWNBUTTONR: break;
                    case VAR_INPUTDOWNSTART: break;
                    case VAR_INPUTDOWNSELECT: break;
                    case VAR_INPUTPRESSUP: break;
                    case VAR_INPUTPRESSDOWN: break;
                    case VAR_INPUTPRESSLEFT: break;
                    case VAR_INPUTPRESSRIGHT: break;
                    case VAR_INPUTPRESSBUTTONA: break;
                    case VAR_INPUTPRESSBUTTONB: break;
                    case VAR_INPUTPRESSBUTTONC: break;
                    case VAR_INPUTPRESSBUTTONX: break;
                    case VAR_INPUTPRESSBUTTONY: break;
                    case VAR_INPUTPRESSBUTTONZ: break;
                    case VAR_INPUTPRESSBUTTONL: break;
                    case VAR_INPUTPRESSBUTTONR: break;
                    case VAR_INPUTPRESSSTART: break;
                    case VAR_INPUTPRESSSELECT: break;
                    case VAR_MENU1SELECTION: break;
                    case VAR_MENU2SELECTION: break;
                    case VAR_TILELAYERXSIZE: break;
                    case VAR_TILELAYERYSIZE: break;
                    case VAR_TILELAYERTYPE: break;
                    case VAR_TILELAYERANGLE: {
                        break;
                    }
                    case VAR_TILELAYERXPOS: break;
                    case VAR_TILELAYERYPOS: break;
                    case VAR_TILELAYERZPOS: break;
                    case VAR_TILELAYERPARALLAXFACTOR: break;
                    case VAR_TILELAYERSCROLLSPEED: break;
                    case VAR_TILELAYERSCROLLPOS: break;
                    case VAR_TILELAYERDEFORMATIONOFFSET: break;
                    case VAR_TILELAYERDEFORMATIONOFFSETW: break;
                    case VAR_HPARALLAXPARALLAXFACTOR: break;
                    case VAR_HPARALLAXSCROLLSPEED: break;
                    case VAR_HPARALLAXSCROLLPOS: break;
                    case VAR_VPARALLAXPARALLAXFACTOR: break;
                    case VAR_VPARALLAXSCROLLSPEED: break;
                    case VAR_VPARALLAXSCROLLPOS: break;
                    case VAR_SCENE3DVERTEXCOUNT: break;
                    case VAR_SCENE3DFACECOUNT: break;
                    case VAR_SCENE3DPROJECTIONX: break;
                    case VAR_SCENE3DPROJECTIONY: break;
                    case VAR_SCENE3DFOGCOLOR: break;
                    case VAR_SCENE3DFOGSTRENGTH: break;
                    case VAR_VERTEXBUFFERX: break;
                    case VAR_VERTEXBUFFERY: break;
                    case VAR_VERTEXBUFFERZ: break;
                    case VAR_VERTEXBUFFERU: break;
                    case VAR_VERTEXBUFFERV: break;
                    case VAR_FACEBUFFERA: break;
                    case VAR_FACEBUFFERB: break;
                    case VAR_FACEBUFFERC: break;
                    case VAR_FACEBUFFERD: break;
                    case VAR_FACEBUFFERFLAG: break;
                    case VAR_FACEBUFFERCOLOR: break;
                    case VAR_SAVERAM: break;
                    case VAR_ENGINESTATE:; break;
                    case VAR_ENGINELANGUAGE: break;
                    case VAR_ENGINEONLINEACTIVE: break;
                    case VAR_ENGINESFXVOLUME: break;
                    case VAR_ENGINEBGMVOLUME: break;
                    case VAR_ENGINETRIALMODE:; break;
                    case VAR_ENGINEDEVICETYPE:
                        break;
                        // EDITOR-ONLY
                    case VAR_EDITORVARIABLEID: break;
                    case VAR_EDITORVARIABLEVAL:
                        scnEditor->viewer->variableValue = scriptEng.operands[i];
                        break;
                    case VAR_EDITORRETURNVAR: break;
                    case VAR_EDITORSHOWGIZMOS: break;
                }
            }
            else if (opcodeType == SCRIPTVAR_INTCONST) { // int constant
                scriptDataPtr++;
            }
            else if (opcodeType == SCRIPTVAR_STRCONST) { // string constant
                int strLen = scriptData[scriptDataPtr++];
                for (int c = 0; c < strLen; ++c) {
                    switch (c % 4) {
                        case 0: break;
                        case 1: break;
                        case 2: break;
                        case 3: ++scriptDataPtr; break;
                        default: break;
                    }
                }
                scriptDataPtr++;
            }
        }
    }
}

byte Compilerv4::ArcTanLookup(int X, int Y)
{
    int XVal;
    byte result = 0;
    int YVal;

    if (X >= 0)
        XVal = X;
    else
        XVal = -X;

    if (Y >= 0)
        YVal = Y;
    else
        YVal = -Y;

    if (XVal <= YVal) {
        while (YVal > 0xFF) {
            XVal >>= 4;
            YVal >>= 4;
        }
    }
    else {
        while (XVal > 0xFF) {
            XVal >>= 4;
            YVal >>= 4;
        }
    }
    if (X <= 0) {
        if (Y <= 0)
            result = atanVal256[0x100 * XVal + YVal] + -0x80;
        else
            result = -0x80 - atanVal256[0x100 * XVal + YVal];
    }
    else if (Y <= 0)
        result = -atanVal256[0x100 * XVal + YVal];
    else
        result = atanVal256[0x100 * XVal + YVal];
    return result;
}
