#include "includes.hpp"

#define COMMONALIAS_COUNT_v4 (0x4E)
#define ALIAS_COUNT_TRIM_v4  (0xE0)
#define ALIAS_COUNT_v4       (COMMONALIAS_COUNT_v4 + ALIAS_COUNT_TRIM_v4)

struct AliasInfov4 {
    AliasInfov4()
    {
        m_name  = "";
        m_value = "";
    }
    AliasInfov4(QString aliasName, QString aliasVal)
    {
        m_name  = aliasName;
        m_value = aliasVal;
    }

    QString m_name  = "";
    QString m_value = "";
};

struct FunctionInfov4 {
    FunctionInfov4()
    {
        m_name       = "";
        m_opcodeSize = 0;
    }
    FunctionInfov4(QString functionName, int opSize)
    {
        m_name       = functionName;
        m_opcodeSize = opSize;
    }

    QString m_name   = "";
    int m_opcodeSize = 0;
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

    // Math functionsv4
    FunctionInfov4("Rand", 2),
    FunctionInfov4("Sin", 2),
    FunctionInfov4("Cos", 2),
    FunctionInfov4("Sin256", 2),
    FunctionInfov4("Cos256", 2),
    FunctionInfov4("ATan2", 3),
    FunctionInfov4("Interpolate", 4),
    FunctionInfov4("InterpolateXY", 7),

    // Graphics functionsv4
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

    // Player and Animation functionsv4
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
};

AliasInfov4 publicAliases[ALIAS_COUNT_v4] = { AliasInfov4("true", "1"),
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
                                              AliasInfov4("STAGE_PAUSED", "2"),
                                              AliasInfov4("STAGE_RUNNING", "1"),
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
                                              AliasInfov4("FACE_TEXTURED_D", "6"),
                                              AliasInfov4("FACE_SPRITE_3D", "7"),
                                              AliasInfov4("PRIORITY_ACTIVE_BOUNDS", "0"),
                                              AliasInfov4("PRIORITY_ACTIVE", "1"),
                                              AliasInfov4("PRIORITY_ACTIVE_PAUSED", "2"),
                                              AliasInfov4("PRIORITY_XBOUNDS", "3"),
                                              AliasInfov4("PRIORITY_XBOUNDS_DESTROY", "4"),
                                              AliasInfov4("PRIORITY_INACTIVE", "5"),
                                              AliasInfov4("PRIORITY_BOUNDS_SMALL", "6"),
                                              AliasInfov4("PRIORITY_UNKNOWN", "7"),
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
                                              AliasInfov4("TEXTINFO_ROWCOUNT", "1"),
                                              AliasInfov4("ONLINEMENU_ACHIEVEMENTS", "0"),
                                              AliasInfov4("ONLINEMENU_LEADERBOARDS", "1"),
                                              AliasInfov4("TILELAYER_NOSCROLL", "0"),
                                              AliasInfov4("TILELAYER_HSCROLL", "1"),
                                              AliasInfov4("TILELAYER_VSCROLL", "2"),
                                              AliasInfov4("TILELAYER_3DFLOOR", "3"),
                                              AliasInfov4("TILELAYER_3DSKY", "4") };
AliasInfov4 privateAliases[ALIAS_COUNT_TRIM_v4];

#define TABLE_COUNT       (0x200)
#define TABLE_ENTRY_COUNT (0x400)
#define STATICVAR_COUNT   (0x200)

struct StaticInfo {
    StaticInfo()
    {
        name    = "";
        value   = 0;
        dataPos = SCRIPTDATA_COUNT - 1;
    }

    QString name;
    int value   = 0;
    int dataPos = 0;
};

struct TableInfo {
    TableInfo()
    {
        name       = "";
        valueCount = 0;
        dataPos    = SCRIPTDATA_COUNT - 1;
    }

    QString name;
    int valueCount;
    StaticInfo values[TABLE_ENTRY_COUNT];
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
    VAR_m_objectEntityPos,
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

    if (m_publicAliasCount >= ALIAS_COUNT_v4) {
        if (findStringToken(text, "privatealias", 1)) // public alias & we reached the cap
            return;
    }

    AliasInfov4 *a = &publicAliases[m_publicAliasCount];
    int *cnt       = &m_publicAliasCount;
    if (findStringToken(text, "privatealias", 1) == 0) {
        a       = &privateAliases[m_privateAliasCount];
        cnt     = &m_privateAliasCount;
        textPos = 12;
        if (m_privateAliasCount >= ALIAS_COUNT_v4) // private alias & we reached the cap
            return;
    }
    a->m_name  = "";
    a->m_value = "";

    while (aliasMatch < 2) {
        if (aliasMatch) {
            if (aliasMatch == 1) {
                if (textPos < text.length()) {
                    a->m_name += text[textPos];
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
            a->m_value += text[textPos];
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

    if (m_publicStaticVarCount >= STATICVAR_COUNT) {
        if (findStringToken(text, "privatevalue", 1)) // public value & we reached the cap
            return;
    }

    StaticInfo *var = &publicStaticVariables[m_publicStaticVarCount];
    int *cnt        = &m_publicStaticVarCount;
    if (findStringToken(text, "privatevalue", 1) == 0) {
        if (m_privateStaticVarCount >= STATICVAR_COUNT) // private value and we reached the cap
            return;
        var     = &privateStaticVariables[m_privateStaticVarCount];
        cnt     = &m_privateStaticVarCount;
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

                var->dataPos                    = m_scriptDataPos;
                m_scriptData[m_scriptDataPos++] = var->value;
                staticStrPos                    = 0;
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

    if (m_publicTableCount >= TABLE_COUNT) {
        if (findStringToken(text, "privatetable", 1)) // public table & we reached the cap
            return NULL;
    }

    TableInfo *table = &publicTables[m_publicTableCount];
    int strPos       = 11;
    curTablePublic   = true;
    if (findStringToken(text, "privatetable", 1) == 0) {
        if (m_privateTableCount >= TABLE_COUNT) // private table and we reached the cap
            return NULL;
        table          = &privateTables[m_privateTableCount];
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
        dest = functionsv4[token].m_name + "(";
        for (int i = 0; i < offset; ++i) dest += text[i];
        if (functionsv4[token].m_opcodeSize > 1) {
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
                dest = functionsv4[compareOp + FUNC_WEQUAL].m_name + "(";
                appendIntegerToSting(dest, m_jumpTableDataPos - m_jumpTableDataOffset);
                dest += ",";
                for (int i = 5; i < text.length(); ++i) {
                    if (text[i] != '=' && text[i] != '(' && text[i] != ')')
                        dest += text[i];
                }
                dest += ")";
                text                                    = dest;
                m_jumpTableStack[++m_jumpTableStackPos] = m_jumpTableDataPos;
                m_jumpTableData[m_jumpTableDataPos++]   = m_scriptDataPos - m_scriptDataOffset;
                m_jumpTableData[m_jumpTableDataPos++]   = 0;
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
            dest = functionsv4[compareOp + FUNC_IFEQUAL].m_name + "(";
            appendIntegerToSting(dest, m_jumpTableDataPos - m_jumpTableDataOffset);
            dest += ",";
            for (int i = 2; i < text.length(); ++i) {
                if (text[i] != '=' && text[i] != '(' && text[i] != ')')
                    dest += text[i];
            }
            dest += ")";
            text                                    = dest;
            m_jumpTableStack[++m_jumpTableStackPos] = m_jumpTableDataPos;
            m_jumpTableData[m_jumpTableDataPos++]   = -1;
            m_jumpTableData[m_jumpTableDataPos++]   = 0;
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
        dest = functionsv4[FUNC_FOREACHACTIVE].m_name;
        dest += "(";
        dest += QString::number(m_jumpTableDataPos - m_jumpTableDataOffset);
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
        text                                    = dest;
        m_jumpTableStack[++m_jumpTableStackPos] = m_jumpTableDataPos;
        m_jumpTableData[m_jumpTableDataPos++]   = m_scriptDataPos - m_scriptDataOffset;
        m_jumpTableData[m_jumpTableDataPos++]   = 0;
    }
    else if (findStringToken(text, "ALL_ENTITIES", 1) > 0) { // foreach (all entities)
        dest = functionsv4[FUNC_FOREACHALL].m_name;
        dest += "(";
        dest += QString::number(m_jumpTableDataPos - m_jumpTableDataOffset);
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
        text                                    = dest;
        m_jumpTableStack[++m_jumpTableStackPos] = m_jumpTableDataPos;
        m_jumpTableData[m_jumpTableDataPos++]   = m_scriptDataPos - m_scriptDataOffset;
        m_jumpTableData[m_jumpTableDataPos++]   = 0;
    }
}
bool Compilerv4::convertSwitchStatement(QString &text)
{
    if (findStringToken(text, "switch", 1))
        return false;
    QString switchText = "";
    switchText         = "switch(";
    appendIntegerToSting(switchText, m_jumpTableDataPos - m_jumpTableDataOffset);
    switchText += ",";
    for (int i = 6; i < text.length(); ++i) {
        if (text[i] != '=' && text[i] != '(' && text[i] != ')')
            switchText += text[i];
    }
    switchText += ")";
    text                                    = switchText;
    m_jumpTableStack[++m_jumpTableStackPos] = m_jumpTableDataPos;
    m_jumpTableData[m_jumpTableDataPos++]   = 0x10000;
    m_jumpTableData[m_jumpTableDataPos++]   = -0x10000;
    m_jumpTableData[m_jumpTableDataPos++]   = -1;
    m_jumpTableData[m_jumpTableDataPos++]   = 0;
    return true;
}
void Compilerv4::convertFunctionText(QString &text)
{
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

    for (int i = 0; i < ScrFunc::FUNC_MAX_CNT; ++i) {
        if (funcName == functionsv4[i].m_name) {
            opcode     = i;
            opcodeSize = functionsv4[i].m_opcodeSize;
            textPos    = functionsv4[i].m_name.length();
            i          = FUNC_MAX_CNT;
        }
    }

    if (opcode <= 0) {
        m_scriptError = true;
        errorMsg      = "OPCODE NOT FOUND";
        errorPos      = funcName;
    }
    else {
        m_scriptData[m_scriptDataPos++] = opcode;
        if (functionsv4[opcode].m_name == "else")
            m_jumpTableData[m_jumpTableStack[m_jumpTableStackPos]] =
                m_scriptDataPos - m_scriptDataOffset;

        if (functionsv4[opcode].m_name == "endif") {
            int jPos                  = m_jumpTableStack[m_jumpTableStackPos];
            m_jumpTableData[jPos + 1] = m_scriptDataPos - m_scriptDataOffset;
            if (m_jumpTableData[jPos] == -1)
                m_jumpTableData[jPos] = (m_scriptDataPos - m_scriptDataOffset) - 1;
            --m_jumpTableStackPos;
        }

        if (functionsv4[opcode].m_name == "endswitch") {
            int jPos                  = m_jumpTableStack[m_jumpTableStackPos];
            m_jumpTableData[jPos + 3] = m_scriptDataPos - m_scriptDataOffset;
            if (m_jumpTableData[jPos + 2] == -1) {
                m_jumpTableData[jPos + 2] = (m_scriptDataPos - m_scriptDataOffset) - 1;
                int caseCnt               = abs(m_jumpTableData[jPos + 1] - m_jumpTableData[jPos]) + 1;

                int jOffset = jPos + 4;
                for (int c = 0; c < caseCnt; ++c) {
                    if (m_jumpTableData[jOffset + c] < 0)
                        m_jumpTableData[jOffset + c] = m_jumpTableData[jPos + 2];
                }
            }
            --m_jumpTableStackPos;
        }

        if (functionsv4[opcode].m_name == "loop" || functionsv4[opcode].m_name == "next") {
            m_jumpTableData[m_jumpTableStack[m_jumpTableStackPos--] + 1] =
                m_scriptDataPos - m_scriptDataOffset;
        }

        for (int i = 0; i < opcodeSize; ++i) {
            ++textPos;
            int value            = 0;
            int scriptTextByteID = 0;
            funcName             = "";
            strBuffer            = "";
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
            // Private (this script only)
            for (int a = 0; a < m_privateAliasCount; ++a) {
                if (funcName == privateAliases[a].m_name) {
                    copyAliasStr(funcName, privateAliases[a].m_value, 0);
                    if (findStringToken(privateAliases[a].m_value, "[", 1) > -1)
                        copyAliasStr(strBuffer, privateAliases[a].m_value, 1);
                }
            }
            // Private (this script & all following scripts)
            for (int a = 0; a < m_publicAliasCount; ++a) {
                if (funcName == publicAliases[a].m_name) {
                    copyAliasStr(funcName, publicAliases[a].m_value, 0);
                    if (findStringToken(publicAliases[a].m_value, "[", 1) > -1)
                        copyAliasStr(strBuffer, publicAliases[a].m_value, 1);
                }
            }

            if (strBuffer.length() > 0) {
                // Private (this script only)
                for (int a = 0; a < m_privateAliasCount; ++a) {
                    if (strBuffer == privateAliases[a].m_name) {
                        copyAliasStr(strBuffer, privateAliases[a].m_value, 0);
                    }
                }
                // Private (this script & all following scripts)
                for (int a = 0; a < m_publicAliasCount; ++a) {
                    if (strBuffer == publicAliases[a].m_name) {
                        copyAliasStr(strBuffer, publicAliases[a].m_value, 0);
                    }
                }
            }

            // Eg: TempValue0 = value0
            // Private (this script only)
            for (int s = 0; s < m_privateStaticVarCount; ++s) {
                if (funcName == privateStaticVariables[s].name) {
                    funcName  = "local";
                    strBuffer = QString::number(privateStaticVariables[s].dataPos);
                }
            }
            // Private (this script & all following scripts)
            for (int s = 0; s < m_publicStaticVarCount; ++s) {
                if (funcName == publicStaticVariables[s].name) {
                    funcName  = "local";
                    strBuffer = QString::number(publicStaticVariables[s].dataPos);
                }
            }

            // Eg: GetTableValue(TempValue0, 1, array0)
            // Private (this script only)
            for (int a = 0; a < m_privateTableCount; ++a) {
                if (funcName == privateTables[a].name) {
                    funcName  = QString::number(privateTables[a].dataPos);
                    strBuffer = "";
                }
            }
            // Private (this script & all following scripts)
            for (int a = 0; a < m_publicTableCount; ++a) {
                if (funcName == publicTables[a].name) {
                    funcName  = QString::number(publicTables[a].dataPos);
                    strBuffer = "";
                }
            }

            // Eg: TempValue0 = Game.Variable
            for (int v = 0; v < m_globalVariables.count(); ++v) {
                if (funcName == m_globalVariables[v]) {
                    funcName  = "Global";
                    strBuffer = "";
                    appendIntegerToSting(strBuffer, v);
                }
            }
            // Eg: TempValue0 = Function1
            for (int f = 0; f < m_functionCount; ++f) {
                if (funcName == m_functionNames[f]) {
                    funcName = "";
                    appendIntegerToSting(funcName, f);
                }
            }
            // Eg: TempValue0 = TypeName[PlayerObject]
            if (funcName == "TypeName") {
                funcName = "";
                appendIntegerToSting(funcName, 0);
                for (int o = 0; o < OBJECT_COUNT; ++o) {
                    if (strBuffer == m_typeNames[o]) {
                        funcName = "";
                        appendIntegerToSting(funcName, o);
                    }
                }
            }
            if (convertStringToInteger(funcName, &value)) {
                m_scriptData[m_scriptDataPos++] = SCRIPTVAR_INTCONST;
                m_scriptData[m_scriptDataPos++] = value;
            }
            else if (funcName.contains('"')) {
                m_scriptData[m_scriptDataPos++] = SCRIPTVAR_STRCONST;
                m_scriptData[m_scriptDataPos++] = funcName.length() - 2;
                int scriptTextPos               = 1;
                scriptTextByteID                = 0;
                while (scriptTextPos > -1) {
                    switch (scriptTextByteID) {
                        case 0:
                            m_scriptData[m_scriptDataPos] = funcName[scriptTextPos].toLatin1() << 24;
                            ++scriptTextByteID;
                            break;
                        case 1:
                            m_scriptData[m_scriptDataPos] += funcName[scriptTextPos].toLatin1() << 16;
                            ++scriptTextByteID;
                            break;
                        case 2:
                            m_scriptData[m_scriptDataPos] += funcName[scriptTextPos].toLatin1() << 8;
                            ++scriptTextByteID;
                            break;
                        case 3:
                            m_scriptData[m_scriptDataPos++] += funcName[scriptTextPos].toLatin1();
                            scriptTextByteID = 0;
                            break;
                        default: break;
                    }
                    if (funcName[scriptTextPos] == '"') {
                        if (scriptTextByteID > 0)
                            ++m_scriptDataPos;
                        scriptTextPos = -1;
                    }
                    else {
                        scriptTextPos++;
                    }
                }
            }
            else {
                m_scriptData[m_scriptDataPos++] = SCRIPTVAR_VAR;
                if (strBuffer.length()) {
                    m_scriptData[m_scriptDataPos] = VARARR_ARRAY;
                    if (strBuffer[0] == '+')
                        m_scriptData[m_scriptDataPos] = VARARR_ENTNOPLUS1;
                    if (strBuffer[0] == '-')
                        m_scriptData[m_scriptDataPos] = VARARR_ENTNOMINUS1;
                    ++m_scriptDataPos;
                    if (strBuffer[0] == '-' || strBuffer[0] == '+') {
                        strBuffer.remove(0, 1);
                    }
                    if (convertStringToInteger(strBuffer, &value) == 1) {
                        m_scriptData[m_scriptDataPos++] = 0;
                        m_scriptData[m_scriptDataPos++] = value;
                    }
                    else {
                        if (strBuffer == "arrayPos0")
                            value = 0;
                        if (strBuffer == "arrayPos1")
                            value = 1;
                        if (strBuffer == "arrayPos2")
                            value = 2;
                        if (strBuffer == "arrayPos3")
                            value = 3;
                        if (strBuffer == "arrayPos4")
                            value = 4;
                        if (strBuffer == "arrayPos5")
                            value = 5;
                        if (strBuffer == "arrayPos6")
                            value = 6;
                        if (strBuffer == "arrayPos7")
                            value = 7;
                        if (strBuffer == "tempObjectPos")
                            value = 8;
                        m_scriptData[m_scriptDataPos++] = VARARR_ARRAY;
                        m_scriptData[m_scriptDataPos++] = value;
                    }
                }
                else {
                    m_scriptData[m_scriptDataPos++] = VARARR_NONE;
                }
                value = -1;
                for (int i = 0; i < VAR_MAX_CNT; ++i) {
                    if (funcName == variableNamesv4[i])
                        value = i;
                }

                if (value == -1 && !m_scriptError) {
                    m_scriptError = true;
                    errorMsg      = "OPERAND NOT FOUND";
                    errorPos      = funcName;
                    m_errorLine   = m_lineID;
                    value         = 0;
                }
                m_scriptData[m_scriptDataPos++] = value;
            }
        }
    }
}
void Compilerv4::checkCaseNumber(QString &text)
{
    if (findStringToken(text, "case", 1))
        return;

    QString dest = "";
    if (text.length() > 4) {
        int textPos = 4;
        do {
            if (text[textPos] != ':')
                dest += text[textPos];
        } while (++textPos < text.length());
    }

    for (int a = 0; a < m_privateAliasCount; ++a) {
        if (privateAliases[a].m_name == dest) {
            dest = privateAliases[a].m_value;
            goto CONV_VAL;
        }
    }

    for (int a = 0; a < m_publicAliasCount; ++a) {
        if (publicAliases[a].m_name == dest) {
            dest = publicAliases[a].m_value;
            goto CONV_VAL;
        }
    }

CONV_VAL:
    int aliasVarID = 0;
    if (!convertStringToInteger(dest, &aliasVarID))
        return;
    int stackValue = m_jumpTableStack[m_jumpTableStackPos];
    if (aliasVarID < m_jumpTableData[stackValue])
        m_jumpTableData[stackValue] = aliasVarID;
    stackValue++;
    if (aliasVarID > m_jumpTableData[stackValue])
        m_jumpTableData[stackValue] = aliasVarID;
}
bool Compilerv4::readSwitchCase(QString &text)
{
    QString caseText = "";
    if (findStringToken(text, "case", 1)) {
        if (findStringToken(text, "default", 1)) {
            return false;
        }
        else {
            int jumpTablepos                  = m_jumpTableStack[m_jumpTableStackPos];
            m_jumpTableData[jumpTablepos + 2] = m_scriptDataPos - m_scriptDataOffset;
            int cnt = abs(m_jumpTableData[jumpTablepos + 1] - m_jumpTableData[jumpTablepos]) + 1;

            int jOffset = jumpTablepos + 4;
            for (int i = 0; i < cnt; ++i) {
                if (m_jumpTableData[jOffset + i] < 0)
                    m_jumpTableData[jOffset + i] = m_scriptDataPos - m_scriptDataOffset;
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

        for (int a = 0; a < m_privateAliasCount; ++a) {
            if (caseText == privateAliases[a].m_name)
                caseText = privateAliases[a].m_value;
        }

        for (int a = 0; a < m_publicAliasCount; ++a) {
            if (caseText == publicAliases[a].m_name)
                caseText = publicAliases[a].m_value;
        }

        int val = 0;

        int jPos    = m_jumpTableStack[m_jumpTableStackPos];
        int jOffset = jPos + 4;
        if (convertStringToInteger(caseText, &val))
            m_jumpTableData[val - m_jumpTableData[jPos] + jOffset] =
                m_scriptDataPos - m_scriptDataOffset;
        return true;
    }
    return false;
}
void Compilerv4::readTableValues(QString &text)
{
    int textPos       = 0;
    QString strBuffer = "";
    int strPos        = 0;
    while (true) {
        if (textPos >= text.length()) {
            int cnt = currentTable->valueCount;
            convertStringToInteger(strBuffer, &currentTable->values[cnt].value);
            currentTable->valueCount++;
            strBuffer = "";

            strPos = 0;

            if (textPos >= text.length())
                break;
        }
        else if (text[textPos] == ',') {
            int cnt = currentTable->valueCount;
            convertStringToInteger(strBuffer, &currentTable->values[cnt].value);
            currentTable->valueCount++;
            strBuffer = "";

            strPos = 0;

            if (textPos >= text.length())
                break;
        }
        else {
            strBuffer += text[textPos];
        }
        textPos++;
    }
}
void Compilerv4::appendIntegerToSting(QString &text, int value) { text += QString::number(value); }
bool Compilerv4::convertStringToInteger(QString &text, int *value)
{
    *value  = 0;
    bool ok = false;

    if (text.startsWith("0x") || text.startsWith("0X")) {
        *value = text.toInt(&ok, 0x10);
    }
    else {
        *value = text.toInt(&ok, 10);
    }

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

void Compilerv4::parseScriptFile(QString scriptName, int scriptID)
{
    m_jumpTableStackPos = 0;
    m_lineID            = 0;

    m_privateAliasCount     = 0;
    m_privateStaticVarCount = 0;
    m_privateTableCount     = 0;

    Reader reader(scriptName);
    if (reader.m_initialised) {
        int readMode   = READMODE_NORMAL;
        int parseMode  = PARSEMODE_SCOPELESS;
        char prevChar  = 0;
        char curChar   = 0;
        int switchDeep = 0;
        int storePos   = 0;

        while (readMode < READMODE_EOF) {
            m_scriptText = "";
            int textPos  = 0;
            readMode     = READMODE_NORMAL;
            while (readMode < READMODE_ENDLINE) {
                prevChar = curChar;
                curChar  = reader.read<char>();
                if (readMode == READMODE_STRING) {
                    if (curChar == '\t' || curChar == '\r' || curChar == '\n' || curChar == ';'
                        || readMode >= READMODE_COMMENTLINE) {
                        if ((curChar == '\n' && prevChar != '\r')
                            || (curChar == '\n' && prevChar == '\r')) {
                            readMode = READMODE_ENDLINE;
                        }
                    }
                    else if (curChar != '/' || textPos <= 0) {
                        m_scriptText += curChar;
                        if (curChar == '"')
                            readMode = READMODE_NORMAL;
                    }
                    else if (curChar == '/' && prevChar == '/') {
                        readMode     = READMODE_COMMENTLINE;
                        m_scriptText = "";
                    }
                    else {
                        m_scriptText += curChar;
                    }
                }
                else if (curChar == ' ' || curChar == '\t' || curChar == '\r' || curChar == '\n'
                         || curChar == ';' || readMode >= READMODE_COMMENTLINE) {
                    if ((curChar == '\n' && prevChar != '\r')
                        || (curChar == '\n' && prevChar == '\r')) {
                        readMode = READMODE_ENDLINE;
                    }
                }
                else if (curChar != '/' || textPos <= 0) {
                    m_scriptText += curChar;
                    if (curChar == '"' && !readMode)
                        readMode = READMODE_STRING;
                }
                else if (curChar == '/' && prevChar == '/') {
                    readMode     = READMODE_COMMENTLINE;
                    m_scriptText = "";
                }
                else {
                    m_scriptText += curChar;
                }
                if (reader.isEOF()) {
                    readMode = READMODE_EOF;
                }
            }

            switch (parseMode) {
                case PARSEMODE_SCOPELESS:
                    ++m_lineID;
                    checkAliasText(m_scriptText);
                    checkStaticText(m_scriptText);

                    currentTable = (TableInfo *)checkTableText(m_scriptText);
                    if (currentTable) {
                        parseMode    = PARSEMODE_TABLEREAD;
                        m_scriptText = "";
                    }

                    if (m_scriptText == "eventRSDKDraw") {
                        parseMode                                                  = PARSEMODE_FUNCTION;
                        m_objectScriptList[scriptID].eventRSDKDraw.m_scriptCodePtr = m_scriptDataPos;
                        m_objectScriptList[scriptID].eventRSDKDraw.m_jumpTablePtr  = m_jumpTableDataPos;
                        m_scriptDataOffset                                         = m_scriptDataPos;
                        m_jumpTableDataOffset                                      = m_jumpTableDataPos;
                    }
                    if (m_scriptText == "eventRSDKLoad") {
                        parseMode                                                  = PARSEMODE_FUNCTION;
                        m_objectScriptList[scriptID].eventRSDKLoad.m_scriptCodePtr = m_scriptDataPos;
                        m_objectScriptList[scriptID].eventRSDKLoad.m_jumpTablePtr  = m_jumpTableDataPos;
                        m_scriptDataOffset                                         = m_scriptDataPos;
                        m_jumpTableDataOffset                                      = m_jumpTableDataPos;
                    }
                    break;
                case PARSEMODE_PLATFORMSKIP:
                    ++m_lineID;
                    if (!findStringToken(m_scriptText, "#endplatform", 1))
                        parseMode = PARSEMODE_FUNCTION;
                    break;
                case PARSEMODE_FUNCTION:
                    ++m_lineID;
                    if (m_scriptText.length()) {
                        if (m_scriptText == "endevent") {
                            m_scriptData[m_scriptDataPos++] = FUNC_END;
                            parseMode                       = PARSEMODE_SCOPELESS;
                        }
                        else if (m_scriptText == "endfunction") {
                            m_scriptData[m_scriptDataPos++] = FUNC_RETURN;
                            parseMode                       = PARSEMODE_SCOPELESS;
                        }
                        else if (findStringToken(m_scriptText, "#platform:", 1)) {
                            // layed out like ass, but this means "if we did not find "#platform:"
                            if (findStringToken(m_scriptText, "#endplatform", 1) == -1) {
                                // if we did NOT find "#endplatform"
                                convertIfWhileStatement(m_scriptText);
                                convertForeachStatement(m_scriptText);
                                if (convertSwitchStatement(m_scriptText)) {
                                    parseMode  = PARSEMODE_SWITCHREAD;
                                    storePos   = (int)reader.tell();
                                    switchDeep = 0;
                                }
                                convertArithmaticSyntax(m_scriptText);
                                if (!readSwitchCase(m_scriptText)) {
                                    convertFunctionText(m_scriptText);
                                    if (m_scriptError) {
                                        m_errorScr = scriptName;
                                        parseMode  = PARSEMODE_ERROR;
                                    }
                                }
                            }
                        }
                        else if (findStringToken(m_scriptText, m_gamePlatform, 1) == -1
                                 && findStringToken(m_scriptText, m_gameRenderType, 1) == -1
                                 && findStringToken(m_scriptText, m_gameHapticSetting, 1) == -1) {
                            // if NONE of these checks succeeded, then we skip everything until "end
                            // platform"
                            parseMode = PARSEMODE_PLATFORMSKIP;
                        }
                    }
                    break;
                case PARSEMODE_SWITCHREAD:
                    if (!findStringToken(m_scriptText, "switch", 1))
                        ++switchDeep;
                    if (switchDeep) {
                        if (!findStringToken(m_scriptText, "endswitch", 1))
                            --switchDeep;
                    }
                    else if (findStringToken(m_scriptText, "endswitch", 1)) {
                        checkCaseNumber(m_scriptText);
                    }
                    else {
                        reader.seek(storePos);
                        parseMode  = PARSEMODE_FUNCTION;
                        int jPos   = m_jumpTableStack[m_jumpTableStackPos];
                        switchDeep = abs(m_jumpTableData[jPos + 1] - m_jumpTableData[jPos]) + 1;
                        for (textPos = 0; textPos < switchDeep; ++textPos)
                            m_jumpTableData[m_jumpTableDataPos++] = -1;
                    }
                    break;
                case PARSEMODE_TABLEREAD:
                    if (findStringToken(m_scriptText, "endtable", 1)) {
                        readTableValues(m_scriptText);
                    }
                    else {
                        currentTable->dataPos           = m_scriptDataPos;
                        m_scriptData[m_scriptDataPos++] = currentTable->valueCount;
                        for (int v = 0; v < currentTable->valueCount; ++v) {
                            currentTable->values[v].dataPos = m_scriptDataPos;
                            m_scriptData[m_scriptDataPos++] = currentTable->values[v].value;
                        }

                        if (curTablePublic) {
                            publicTables[m_publicTableCount] = *currentTable;
                            ++m_publicTableCount;
                        }
                        else {
                            privateTables[m_privateTableCount] = *currentTable;
                            ++m_privateTableCount;
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
    memset(m_scriptData, 0, SCRIPTDATA_COUNT * sizeof(int));
    memset(m_jumpTableData, 0, JUMPTABLE_COUNT * sizeof(int));

    memset(m_foreachStack, -1, FORSTACK_COUNT_v4 * sizeof(int));
    memset(m_jumpTableStack, 0, JUMPSTACK_COUNT_v4 * sizeof(int));
    // memset(m_functionStack, 0, FUNCSTACK_COUNT_v4 * sizeof(int));

    scriptFrameCount = 0;

    // m_scriptCodePos     = 0;
    // m_jumpTablePos      = 0;
    m_jumpTableStackPos = 0;
    m_functionStackPos  = 0;

    m_scriptDataPos       = 0;
    m_scriptDataOffset    = 0;
    m_jumpTableDataPos    = 0;
    m_jumpTableDataOffset = 0;

    m_functionCount = 0;

    m_lineID = 0;

    m_publicAliasCount  = COMMONALIAS_COUNT_v4;
    m_privateAliasCount = 0;

    m_publicStaticVarCount  = 0;
    m_privateStaticVarCount = 0;

    m_publicTableCount  = 0;
    m_privateTableCount = 0;

    for (int o = 0; o < OBJECT_COUNT; ++o) {
        ObjectScript *scriptInfo                  = &m_objectScriptList[o];
        scriptInfo->eventRSDKDraw.m_scriptCodePtr = SCRIPTDATA_COUNT - 1;
        scriptInfo->eventRSDKDraw.m_jumpTablePtr  = JUMPTABLE_COUNT - 1;
        scriptInfo->eventRSDKLoad.m_scriptCodePtr = SCRIPTDATA_COUNT - 1;
        scriptInfo->eventRSDKLoad.m_jumpTablePtr  = JUMPTABLE_COUNT - 1;
        m_typeNames[o]                            = "";
    }

    for (int f = 0; f < FUNCTION_COUNT; ++f) {
        m_functionList[f].m_scriptCodePtr = SCRIPTDATA_COUNT - 1;
        m_functionList[f].m_jumpTablePtr  = JUMPTABLE_COUNT - 1;
    }

    m_typeNames[0] = "Blank Object";
}

void Compilerv4::writeBytecode(QString path)
{
    m_bytecode = RSDKv4::Bytecode();

    m_bytecode.m_scriptData.clear();
    for (int i = m_globalScriptDataCount; i < m_scriptDataPos; ++i) {
        m_bytecode.m_scriptData.append(m_scriptData[i]);
    }

    m_bytecode.m_jumpTableData.clear();
    for (int i = m_globalJumpTableCount; i < m_jumpTableDataPos; ++i) {
        m_bytecode.m_jumpTableData.append(m_jumpTableData[i]);
    }

    m_bytecode.m_globalScriptCount = 0;
    m_bytecode.m_scriptList.clear();
    for (int i = m_globalScriptCount; i < m_scriptCount; ++i) {
        RSDKv4::Bytecode::ObjectScript scr;
        scr.m_mainScript    = m_objectScriptList[i].eventRSDKDraw.m_scriptCodePtr;
        scr.m_mainJumpTable = m_objectScriptList[i].eventRSDKDraw.m_jumpTablePtr;

        scr.m_startupScript    = m_objectScriptList[i].eventRSDKLoad.m_scriptCodePtr;
        scr.m_startupJumpTable = m_objectScriptList[i].eventRSDKLoad.m_jumpTablePtr;

        m_bytecode.m_scriptList.append(scr);
    }

    m_bytecode.m_functionList.clear();
    for (int f = 0; f < m_functionCount; ++f) {
        RSDKv4::Bytecode::FunctionScript func;
        func.m_mainScript    = m_functionList[f].m_scriptCodePtr;
        func.m_mainJumpTable = m_functionList[f].m_jumpTablePtr;
        m_bytecode.m_functionList.append(func);
    }

    m_bytecode.write(path);
}

void Compilerv4::processScript(int scriptCodePtr, int jumpTablePtr, byte scriptEvent)
{
    bool running        = true;
    int scriptDataPtr   = scriptCodePtr;
    m_jumpTableStackPos = 0;
    m_functionStackPos  = 0;
    m_foreachStackPos   = 0;
    SceneViewer *viewer = (SceneViewer *)m_viewer;

    while (running) {
        int opcode           = m_scriptData[scriptDataPtr++];
        int opcodeSize       = functionsv4[opcode].m_opcodeSize;
        int scriptCodeOffset = scriptDataPtr;

        scriptText[0] = '\0';

        // Get Values
        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = m_scriptData[scriptDataPtr++];

            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (m_scriptData[scriptDataPtr++]) {
                    case VARARR_NONE: arrayVal = m_objectEntityPos; break;
                    case VARARR_ARRAY:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal = m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]];
                        else
                            arrayVal = m_scriptData[scriptDataPtr++];
                        break;
                    case VARARR_ENTNOPLUS1:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal = m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]]
                                       + m_objectEntityPos;
                        else
                            arrayVal = m_scriptData[scriptDataPtr++] + m_objectEntityPos;
                        break;
                    case VARARR_ENTNOMINUS1:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal = m_objectEntityPos
                                       - m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]];
                        else
                            arrayVal = m_objectEntityPos - m_scriptData[scriptDataPtr++];
                        break;
                    default: break;
                }

                // Variables
                switch (m_scriptData[scriptDataPtr++]) {
                    default: break;
                    case VAR_TEMP0: m_scriptEng.operands[i] = m_scriptEng.tempValue[0]; break;
                    case VAR_TEMP1: m_scriptEng.operands[i] = m_scriptEng.tempValue[1]; break;
                    case VAR_TEMP2: m_scriptEng.operands[i] = m_scriptEng.tempValue[2]; break;
                    case VAR_TEMP3: m_scriptEng.operands[i] = m_scriptEng.tempValue[3]; break;
                    case VAR_TEMP4: m_scriptEng.operands[i] = m_scriptEng.tempValue[4]; break;
                    case VAR_TEMP5: m_scriptEng.operands[i] = m_scriptEng.tempValue[5]; break;
                    case VAR_TEMP6: m_scriptEng.operands[i] = m_scriptEng.tempValue[6]; break;
                    case VAR_TEMP7: m_scriptEng.operands[i] = m_scriptEng.tempValue[7]; break;
                    case VAR_CHECKRESULT: m_scriptEng.operands[i] = m_scriptEng.checkResult; break;
                    case VAR_ARRAYPOS0: m_scriptEng.operands[i] = m_scriptEng.arrayPosition[0]; break;
                    case VAR_ARRAYPOS1: m_scriptEng.operands[i] = m_scriptEng.arrayPosition[1]; break;
                    case VAR_ARRAYPOS2: m_scriptEng.operands[i] = m_scriptEng.arrayPosition[2]; break;
                    case VAR_ARRAYPOS3: m_scriptEng.operands[i] = m_scriptEng.arrayPosition[3]; break;
                    case VAR_ARRAYPOS4: m_scriptEng.operands[i] = m_scriptEng.arrayPosition[4]; break;
                    case VAR_ARRAYPOS5: m_scriptEng.operands[i] = m_scriptEng.arrayPosition[5]; break;
                    case VAR_ARRAYPOS6: m_scriptEng.operands[i] = m_scriptEng.arrayPosition[6]; break;
                    case VAR_ARRAYPOS7: m_scriptEng.operands[i] = m_scriptEng.arrayPosition[7]; break;
                    case VAR_GLOBAL: /*m_scriptEng.operands[i] = globalVariables[arrayVal];*/ break;
                    case VAR_LOCAL: m_scriptEng.operands[i] = m_scriptData[arrayVal]; break;
                    case VAR_m_objectEntityPos: m_scriptEng.operands[i] = arrayVal; break;
                    case VAR_OBJECTGROUPID: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].typeGroup;
                        break;
                    }
                    case VAR_OBJECTTYPE: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].type;
                        break;
                    }
                    case VAR_OBJECTPROPERTYVALUE: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].propertyValue;
                        break;
                    }
                    case VAR_OBJECTXPOS: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].XPos;
                        break;
                    }
                    case VAR_OBJECTYPOS: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].YPos;
                        break;
                    }
                    case VAR_OBJECTIXPOS: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].XPos >> 16;
                        break;
                    }
                    case VAR_OBJECTIYPOS: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].YPos >> 16;
                        break;
                    }
                    case VAR_OBJECTXVEL: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].XVelocity;
                        break;
                    }
                    case VAR_OBJECTYVEL: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].YVelocity;
                        break;
                    }
                    case VAR_OBJECTSPEED: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].speed;
                        break;
                    }
                    case VAR_OBJECTSTATE: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].state;
                        break;
                    }
                    case VAR_OBJECTROTATION: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].rotation;
                        break;
                    }
                    case VAR_OBJECTSCALE: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].scale;
                        break;
                    }
                    case VAR_OBJECTPRIORITY: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].priority;
                        break;
                    }
                    case VAR_OBJECTDRAWORDER: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].drawOrder;
                        break;
                    }
                    case VAR_OBJECTDIRECTION: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].direction;
                        break;
                    }
                    case VAR_OBJECTINKEFFECT: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].inkEffect;
                        break;
                    }
                    case VAR_OBJECTALPHA: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].alpha;
                        break;
                    }
                    case VAR_OBJECTFRAME: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].frame;
                        break;
                    }
                    case VAR_OBJECTANIMATION: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].animation;
                        break;
                    }
                    case VAR_OBJECTPREVANIMATION: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].prevAnimation;
                        break;
                    }
                    case VAR_OBJECTANIMATIONSPEED: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].animationSpeed;
                        break;
                    }
                    case VAR_OBJECTANIMATIONTIMER: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].animationTimer;
                        break;
                    }
                    case VAR_OBJECTANGLE: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].angle;
                        break;
                    }
                    case VAR_OBJECTLOOKPOSX: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].camOffsetX;
                        break;
                    }
                    case VAR_OBJECTLOOKPOSY: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].lookPos;
                        break;
                    }
                    case VAR_OBJECTCOLLISIONMODE: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].collisionMode;
                        break;
                    }
                    case VAR_OBJECTCOLLISIONPLANE: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].collisionPlane;
                        break;
                    }
                    case VAR_OBJECTCONTROLMODE: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].controlMode;
                        break;
                    }
                    case VAR_OBJECTCONTROLLOCK: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].controlLock;
                        break;
                    }
                    case VAR_OBJECTPUSHING: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].pushing;
                        break;
                    }
                    case VAR_OBJECTVISIBLE: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].visible;
                        break;
                    }
                    case VAR_OBJECTTILECOLLISIONS: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].tileCollisions;
                        break;
                    }
                    case VAR_OBJECTINTERACTION: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].objectInteractions;
                        break;
                    }
                    case VAR_OBJECTGRAVITY: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].gravity;
                        break;
                    }
                    case VAR_OBJECTUP: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].up;
                        break;
                    }
                    case VAR_OBJECTDOWN: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].down;
                        break;
                    }
                    case VAR_OBJECTLEFT: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].left;
                        break;
                    }
                    case VAR_OBJECTRIGHT: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].right;
                        break;
                    }
                    case VAR_OBJECTJUMPPRESS: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].jumpPress;
                        break;
                    }
                    case VAR_OBJECTJUMPHOLD: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].jumpHold;
                        break;
                    }
                    case VAR_OBJECTSCROLLTRACKING: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].trackScroll;
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORL: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].flailing[0];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORC: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].flailing[1];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORR: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].flailing[2];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORLC: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].flailing[3];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORRC: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].flailing[4];
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
                        m_scriptEng.operands[i] =
                            m_objectScriptList[m_objectEntityList[arrayVal].type].spriteSheetID;
                        break;
                    }
                    case VAR_OBJECTVALUE0: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[0];
                        break;
                    }
                    case VAR_OBJECTVALUE1: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[1];
                        break;
                    }
                    case VAR_OBJECTVALUE2: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[2];
                        break;
                    }
                    case VAR_OBJECTVALUE3: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[3];
                        break;
                    }
                    case VAR_OBJECTVALUE4: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[4];
                        break;
                    }
                    case VAR_OBJECTVALUE5: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[5];
                        break;
                    }
                    case VAR_OBJECTVALUE6: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[6];
                        break;
                    }
                    case VAR_OBJECTVALUE7: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[7];
                        break;
                    }
                    case VAR_OBJECTVALUE8: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[8];
                        break;
                    }
                    case VAR_OBJECTVALUE9: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[9];
                        break;
                    }
                    case VAR_OBJECTVALUE10: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[10];
                        break;
                    }
                    case VAR_OBJECTVALUE11: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[11];
                        break;
                    }
                    case VAR_OBJECTVALUE12: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[12];
                        break;
                    }
                    case VAR_OBJECTVALUE13: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[13];
                        break;
                    }
                    case VAR_OBJECTVALUE14: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[14];
                        break;
                    }
                    case VAR_OBJECTVALUE15: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[15];
                        break;
                    }
                    case VAR_OBJECTVALUE16: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[16];
                        break;
                    }
                    case VAR_OBJECTVALUE17: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[17];
                        break;
                    }
                    case VAR_OBJECTVALUE18: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[18];
                        break;
                    }
                    case VAR_OBJECTVALUE19: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[19];
                        break;
                    }
                    case VAR_OBJECTVALUE20: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[20];
                        break;
                    }
                    case VAR_OBJECTVALUE21: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[21];
                        break;
                    }
                    case VAR_OBJECTVALUE22: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[22];
                        break;
                    }
                    case VAR_OBJECTVALUE23: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[23];
                        break;
                    }
                    case VAR_OBJECTVALUE24: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[24];
                        break;
                    }
                    case VAR_OBJECTVALUE25: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[25];
                        break;
                    }
                    case VAR_OBJECTVALUE26: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[26];
                        break;
                    }
                    case VAR_OBJECTVALUE27: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[27];
                        break;
                    }
                    case VAR_OBJECTVALUE28: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[28];
                        break;
                    }
                    case VAR_OBJECTVALUE29: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[29];
                        break;
                    }
                    case VAR_OBJECTVALUE30: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[30];
                        break;
                    }
                    case VAR_OBJECTVALUE31: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[31];
                        break;
                    }
                    case VAR_OBJECTVALUE32: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[32];
                        break;
                    }
                    case VAR_OBJECTVALUE33: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[33];
                        break;
                    }
                    case VAR_OBJECTVALUE34: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[34];
                        break;
                    }
                    case VAR_OBJECTVALUE35: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[35];
                        break;
                    }
                    case VAR_OBJECTVALUE36: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[36];
                        break;
                    }
                    case VAR_OBJECTVALUE37: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[37];
                        break;
                    }
                    case VAR_OBJECTVALUE38: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[38];
                        break;
                    }
                    case VAR_OBJECTVALUE39: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[39];
                        break;
                    }
                    case VAR_OBJECTVALUE40: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[40];
                        break;
                    }
                    case VAR_OBJECTVALUE41: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[41];
                        break;
                    }
                    case VAR_OBJECTVALUE42: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[42];
                        break;
                    }
                    case VAR_OBJECTVALUE43: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[43];
                        break;
                    }
                    case VAR_OBJECTVALUE44: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[44];
                        break;
                    }
                    case VAR_OBJECTVALUE45: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[45];
                        break;
                    }
                    case VAR_OBJECTVALUE46: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[46];
                        break;
                    }
                    case VAR_OBJECTVALUE47: {
                        m_scriptEng.operands[i] = m_objectEntityList[arrayVal].values[47];
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
                    case VAR_ENGINEDEVICETYPE: break;
                }
            }
            else if (opcodeType == SCRIPTVAR_INTCONST) { // int constant
                m_scriptEng.operands[i] = m_scriptData[scriptDataPtr++];
            }
            else if (opcodeType == SCRIPTVAR_STRCONST) { // string constant
                int strLen         = m_scriptData[scriptDataPtr++];
                scriptText[strLen] = 0;
                for (int c = 0; c < strLen; ++c) {
                    switch (c % 4) {
                        case 0: {
                            scriptText[c] = m_scriptData[scriptDataPtr] >> 24;
                            break;
                        }
                        case 1: {
                            scriptText[c] = (0xFFFFFF & m_scriptData[scriptDataPtr]) >> 16;
                            break;
                        }
                        case 2: {
                            scriptText[c] = (0xFFFF & m_scriptData[scriptDataPtr]) >> 8;
                            break;
                        }
                        case 3: {
                            scriptText[c] = m_scriptData[scriptDataPtr++];
                            break;
                        }
                        default: break;
                    }
                }
                scriptDataPtr++;
            }
        }

        ObjectScript *scriptInfo = &m_objectScriptList[m_objectEntityList[m_objectEntityPos].type];
        Entity *entity           = &m_objectEntityList[m_objectEntityPos];
        SpriteFrame *spriteFrame = nullptr;

        // Functions
        switch (opcode) {
            default: break;
            case FUNC_END: running = false; break;
            case FUNC_EQUAL: m_scriptEng.operands[0] = m_scriptEng.operands[1]; break;
            case FUNC_ADD: m_scriptEng.operands[0] += m_scriptEng.operands[1]; break;
            case FUNC_SUB: m_scriptEng.operands[0] -= m_scriptEng.operands[1]; break;
            case FUNC_INC: ++m_scriptEng.operands[0]; break;
            case FUNC_DEC: --m_scriptEng.operands[0]; break;
            case FUNC_MUL: m_scriptEng.operands[0] *= m_scriptEng.operands[1]; break;
            case FUNC_DIV: m_scriptEng.operands[0] /= m_scriptEng.operands[1]; break;
            case FUNC_SHR: m_scriptEng.operands[0] >>= m_scriptEng.operands[1]; break;
            case FUNC_SHL: m_scriptEng.operands[0] <<= m_scriptEng.operands[1]; break;
            case FUNC_AND: m_scriptEng.operands[0] &= m_scriptEng.operands[1]; break;
            case FUNC_OR: m_scriptEng.operands[0] |= m_scriptEng.operands[1]; break;
            case FUNC_XOR: m_scriptEng.operands[0] ^= m_scriptEng.operands[1]; break;
            case FUNC_MOD: m_scriptEng.operands[0] %= m_scriptEng.operands[1]; break;
            case FUNC_FLIPSIGN: m_scriptEng.operands[0] = -m_scriptEng.operands[0]; break;
            case FUNC_CHECKEQUAL:
                m_scriptEng.checkResult = m_scriptEng.operands[0] == m_scriptEng.operands[1];
                opcodeSize              = 0;
                break;
            case FUNC_CHECKGREATER:
                m_scriptEng.checkResult = m_scriptEng.operands[0] > m_scriptEng.operands[1];
                opcodeSize              = 0;
                break;
            case FUNC_CHECKLOWER:
                m_scriptEng.checkResult = m_scriptEng.operands[0] < m_scriptEng.operands[1];
                opcodeSize              = 0;
                break;
            case FUNC_CHECKNOTEQUAL:
                m_scriptEng.checkResult = m_scriptEng.operands[0] != m_scriptEng.operands[1];
                opcodeSize              = 0;
                break;
            case FUNC_IFEQUAL:
                if (m_scriptEng.operands[1] != m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0]];
                m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize                              = 0;
                break;
            case FUNC_IFGREATER:
                if (m_scriptEng.operands[1] <= m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0]];
                m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize                              = 0;
                break;
            case FUNC_IFGREATEROREQUAL:
                if (m_scriptEng.operands[1] < m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0]];
                m_jumpTableStack[+m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize                             = 0;
                break;
            case FUNC_IFLOWER:
                if (m_scriptEng.operands[1] >= m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0]];
                m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize                              = 0;
                break;
            case FUNC_IFLOWEROREQUAL:
                if (m_scriptEng.operands[1] > m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0]];
                m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize                              = 0;
                break;
            case FUNC_IFNOTEQUAL:
                if (m_scriptEng.operands[1] == m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0]];
                m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize                              = 0;
                break;
            case FUNC_ELSE:
                opcodeSize = 0;
                scriptDataPtr =
                    scriptCodePtr
                    + m_jumpTableData[jumpTablePtr + m_jumpTableStack[m_jumpTableStackPos--] + 1];
                break;
            case FUNC_ENDIF:
                opcodeSize = 0;
                --m_jumpTableStackPos;
                break;
            case FUNC_WEQUAL:
                if (m_scriptEng.operands[1] != m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                else
                    m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WGREATER:
                if (m_scriptEng.operands[1] <= m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                else
                    m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WGREATEROREQUAL:
                if (m_scriptEng.operands[1] < m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                else
                    m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WLOWER:
                if (m_scriptEng.operands[1] >= m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                else
                    m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WLOWEROREQUAL:
                if (m_scriptEng.operands[1] > m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                else
                    m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WNOTEQUAL:
                if (m_scriptEng.operands[1] == m_scriptEng.operands[2])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                else
                    m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_LOOP:
                opcodeSize = 0;
                scriptDataPtr =
                    scriptCodePtr
                    + m_jumpTableData[jumpTablePtr + m_jumpTableStack[m_jumpTableStackPos--]];
                break;
            case FUNC_FOREACHACTIVE: {
                int typeGroup = m_scriptEng.operands[1];
                if (typeGroup < TYPEGROUP_COUNT) {
                    // int loop                      = m_foreachStack[++m_foreachStackPos] + 1;
                    // m_foreachStack[m_foreachStackPos] = loop;
                    // if (loop >= m_objectTypeGroupList[typeGroup].listSize) {
                    opcodeSize                          = 0;
                    m_foreachStack[m_foreachStackPos--] = -1;
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                    break;
                    //}
                    // else {
                    //    m_scriptEng.operands[2]               =
                    //    m_objectTypeGroupList[typeGroup].entityRefs[loop];
                    //    m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                    //}
                }
                else {
                    opcodeSize = 0;
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                }
                break;
            }
            case FUNC_FOREACHALL: {
                int objType = m_scriptEng.operands[1];
                if (objType < OBJECT_COUNT) {
                    int loop                          = m_foreachStack[++m_foreachStackPos] + 1;
                    m_foreachStack[m_foreachStackPos] = loop;

                    if (scriptEvent == EVENT_RSDKLOAD) {
                        while (true) {
                            if (loop >= TEMPENTITY_START) {
                                opcodeSize                          = 0;
                                m_foreachStack[m_foreachStackPos--] = -1;
                                int off = m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                                scriptDataPtr = scriptCodePtr + off;
                                break;
                            }
                            else if (objType == m_objectEntityList[loop].type) {
                                m_scriptEng.operands[2]                 = loop;
                                m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                                break;
                            }
                            else {
                                m_foreachStack[m_foreachStackPos] = ++loop;
                            }
                        }
                    }
                    else {
                        while (true) {
                            if (loop >= ENTITY_COUNT) {
                                opcodeSize                          = 0;
                                m_foreachStack[m_foreachStackPos--] = -1;
                                scriptDataPtr =
                                    scriptCodePtr
                                    + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                                break;
                            }
                            else if (objType == m_objectEntityList[loop].type) {
                                m_scriptEng.operands[2]                 = loop;
                                m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                                break;
                            }
                            else {
                                m_foreachStack[m_foreachStackPos] = ++loop;
                            }
                        }
                    }
                }
                else {
                    opcodeSize = 0;
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1];
                }
                break;
            }
            case FUNC_NEXT:
                opcodeSize = 0;
                scriptDataPtr =
                    scriptCodePtr
                    + m_jumpTableData[jumpTablePtr + m_jumpTableStack[m_jumpTableStackPos--]];
                --m_foreachStackPos;
                break;
            case FUNC_SWITCH:
                m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                if (m_scriptEng.operands[1] < m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0]]
                    || m_scriptEng.operands[1]
                           > m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 1])
                    scriptDataPtr =
                        scriptCodePtr + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 2];
                else
                    scriptDataPtr = scriptCodePtr
                                    + m_jumpTableData[jumpTablePtr + m_scriptEng.operands[0] + 4
                                                      + (m_scriptEng.operands[1]
                                                         - m_jumpTableData[jumpTablePtr
                                                                           + m_scriptEng.operands[0]])];
                opcodeSize = 0;
                break;
            case FUNC_BREAK:
                opcodeSize = 0;
                scriptDataPtr =
                    scriptCodePtr
                    + m_jumpTableData[jumpTablePtr + m_jumpTableStack[m_jumpTableStackPos--] + 3];
                break;
            case FUNC_ENDSWITCH:
                opcodeSize = 0;
                --m_jumpTableStackPos;
                break;
            case FUNC_RAND: m_scriptEng.operands[0] = rand() % m_scriptEng.operands[1]; break;
            case FUNC_SIN: {
                m_scriptEng.operands[0] = sin512(m_scriptEng.operands[1]);
                break;
            }
            case FUNC_COS: {
                m_scriptEng.operands[0] = cos512(m_scriptEng.operands[1]);
                break;
            }
            case FUNC_SIN256: {
                m_scriptEng.operands[0] = sin256(m_scriptEng.operands[1]);
                break;
            }
            case FUNC_COS256: {
                m_scriptEng.operands[0] = cos256(m_scriptEng.operands[1]);
                break;
            }
            case FUNC_ATAN2: {
                m_scriptEng.operands[0] =
                    ArcTanLookup(m_scriptEng.operands[1], m_scriptEng.operands[2]);
                break;
            }
            case FUNC_INTERPOLATE:
                m_scriptEng.operands[0] = (m_scriptEng.operands[2] * (0x100 - m_scriptEng.operands[3])
                                           + m_scriptEng.operands[3] * m_scriptEng.operands[1])
                                          >> 8;
                break;
            case FUNC_INTERPOLATEXY:
                m_scriptEng.operands[0] =
                    (m_scriptEng.operands[3] * (0x100 - m_scriptEng.operands[6]) >> 8)
                    + ((m_scriptEng.operands[6] * m_scriptEng.operands[2]) >> 8);
                m_scriptEng.operands[1] =
                    (m_scriptEng.operands[5] * (0x100 - m_scriptEng.operands[6]) >> 8)
                    + (m_scriptEng.operands[6] * m_scriptEng.operands[4] >> 8);
                break;
            case FUNC_LOADSPRITESHEET:
                opcodeSize                = 0;
                scriptInfo->spriteSheetID = viewer->addGraphicsFile(scriptText);
                break;
            case FUNC_REMOVESPRITESHEET:
                opcodeSize = 0;
                viewer->removeGraphicsFile(scriptText, -1);
                break;
            case FUNC_DRAWSPRITE:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + m_scriptEng.operands[0]];
                viewer->drawSprite((entity->XPos >> 16) + spriteFrame->pivotX,
                                   (entity->YPos >> 16) + spriteFrame->pivotY, spriteFrame->width,
                                   spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                   scriptInfo->spriteSheetID);
                break;
            case FUNC_DRAWSPRITEXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + m_scriptEng.operands[0]];
                viewer->drawSprite((m_scriptEng.operands[1] >> 16) + spriteFrame->pivotX,
                                   (m_scriptEng.operands[2] >> 16) + spriteFrame->pivotY,
                                   spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                   spriteFrame->sprY, scriptInfo->spriteSheetID);
                break;
            case FUNC_DRAWSPRITESCREENXY: opcodeSize = 0; break;
            case FUNC_DRAWTINTRECT: opcodeSize = 0; break;
            case FUNC_DRAWNUMBERS: {
                opcodeSize = 0;
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
                    scriptFrames[scriptFrameCount].pivotX = m_scriptEng.operands[0];
                    scriptFrames[scriptFrameCount].pivotY = m_scriptEng.operands[1];
                    scriptFrames[scriptFrameCount].width  = m_scriptEng.operands[2];
                    scriptFrames[scriptFrameCount].height = m_scriptEng.operands[3];
                    scriptFrames[scriptFrameCount].sprX   = m_scriptEng.operands[4];
                    scriptFrames[scriptFrameCount].sprY   = m_scriptEng.operands[5];
                    ++scriptFrameCount;
                }
                break;
            case FUNC_EDITFRAME: {
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + m_scriptEng.operands[0]];

                spriteFrame->pivotX = m_scriptEng.operands[1];
                spriteFrame->pivotY = m_scriptEng.operands[2];
                spriteFrame->width  = m_scriptEng.operands[3];
                spriteFrame->height = m_scriptEng.operands[4];
                spriteFrame->sprX   = m_scriptEng.operands[5];
                spriteFrame->sprY   = m_scriptEng.operands[6];
            } break;
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
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + m_scriptEng.operands[0]];
                switch (m_scriptEng.operands[1]) {
                    default: break;
                    case FX_SCALE:
                        // DrawSpriteScaled(entity->direction, (m_scriptEng.operands[2] >> 16) -
                        // xScrollOffset,
                        //                 (m_scriptEng.operands[3] >> 16) - yScrollOffset,
                        //                 -spriteFrame->pivotX, -spriteFrame->pivotY, entity->scale,
                        //                 entity->scale, spriteFrame->width, spriteFrame->height,
                        //                 spriteFrame->sprX, spriteFrame->sprY,
                        //                 scriptInfo->spriteSheetID);
                        break;
                    case FX_ROTATE:
                        // DrawSpriteRotated(entity->direction, (m_scriptEng.operands[2] >> 16) -
                        // xScrollOffset,
                        //                  (m_scriptEng.operands[3] >> 16) - yScrollOffset,
                        //                  -spriteFrame->pivotX, -spriteFrame->pivotY,
                        //                  spriteFrame->sprX, spriteFrame->sprY, spriteFrame->width,
                        //                  spriteFrame->height, entity->rotation,
                        //                  scriptInfo->spriteSheetID);
                        break;
                    case FX_ROTOZOOM:
                        // DrawSpriteRotozoom(entity->direction, (m_scriptEng.operands[2] >> 16) -
                        // xScrollOffset,
                        //                   (m_scriptEng.operands[3] >> 16) - yScrollOffset,
                        //                   -spriteFrame->pivotX, -spriteFrame->pivotY,
                        //                   spriteFrame->sprX, spriteFrame->sprY, spriteFrame->width,
                        //                   spriteFrame->height, entity->rotation, entity->scale,
                        //                   scriptInfo->spriteSheetID);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                viewer->drawSprite(
                                    (m_scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (m_scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, scriptInfo->spriteSheetID);
                                break;
                            case INK_BLEND:
                                viewer->drawBlendedSprite(
                                    (m_scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (m_scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, scriptInfo->spriteSheetID);
                                break;
                            case INK_ALPHA:
                                viewer->drawAlphaBlendedSprite(
                                    (m_scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (m_scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, entity->alpha, scriptInfo->spriteSheetID);
                                break;
                            case INK_ADD:
                                // DrawAdditiveBlendedSprite((m_scriptEng.operands[2] >> 16)+
                                // spriteFrame->pivotX,
                                //                          (m_scriptEng.operands[3] >> 16)  +
                                //                          spriteFrame->pivotY, spriteFrame->width,
                                //                          spriteFrame->height, spriteFrame->sprX,
                                //                          spriteFrame->sprY, entity->alpha,
                                //                          scriptInfo->spriteSheetID);
                                break;
                            case INK_SUB:
                                // DrawSubtractiveBlendedSprite((m_scriptEng.operands[2] >> 16)  +
                                // spriteFrame->pivotX,
                                //                             (m_scriptEng.operands[3] >> 16) +
                                //                             spriteFrame->pivotY, spriteFrame->width,
                                //                             spriteFrame->height, spriteFrame->sprX,
                                //                             spriteFrame->sprY, entity->alpha,
                                //                             scriptInfo->spriteSheetID);
                                break;
                        }
                        break;
                    case FX_TINT: break;
                    case FX_FLIP:
                        switch (entity->direction) {
                            default:
                            case FLIP_NONE:
                                viewer->drawSpriteFlipped(
                                    (m_scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (m_scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, scriptInfo->spriteSheetID);
                                break;
                            case FLIP_X:
                                viewer->drawSpriteFlipped(
                                    (m_scriptEng.operands[2] >> 16) - spriteFrame->width
                                        - spriteFrame->pivotX,
                                    (m_scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_X, scriptInfo->spriteSheetID);
                                break;
                            case FLIP_Y:
                                viewer->drawSpriteFlipped(
                                    (m_scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (m_scriptEng.operands[3] >> 16) - spriteFrame->height
                                        - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_Y, scriptInfo->spriteSheetID);
                                break;
                            case FLIP_XY:
                                viewer->drawSpriteFlipped(
                                    (m_scriptEng.operands[2] >> 16) - spriteFrame->width
                                        - spriteFrame->pivotX,
                                    (m_scriptEng.operands[3] >> 16) - spriteFrame->height
                                        - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_XY, scriptInfo->spriteSheetID);
                                break;
                        }
                        break;
                }
                break;
            case FUNC_DRAWSPRITESCREENFX: opcodeSize = 0; break;
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
                // viewer->drawRectangle(m_scriptEng.operands[0], m_scriptEng.operands[1],
                // m_scriptEng.operands[2], m_scriptEng.operands[3], m_scriptEng.operands[4],
                //              m_scriptEng.operands[5], m_scriptEng.operands[6],
                //              m_scriptEng.operands[7]);
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
            case FUNC_NOT: m_scriptEng.operands[0] = ~m_scriptEng.operands[0]; break;
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
            case FUNC_CALLFUNCTION: {
                opcodeSize = 0;
            } break;
            case FUNC_RETURN: opcodeSize = 0; break;
            case FUNC_SETLAYERDEFORMATION: opcodeSize = 0; break;
            case FUNC_CHECKTOUCHRECT: opcodeSize = 0; break;
            case FUNC_GETTILELAYERENTRY: opcodeSize = 0; break;
            case FUNC_SETTILELAYERENTRY: opcodeSize = 0; break;
            case FUNC_GETBIT:
                m_scriptEng.operands[0] = (m_scriptEng.operands[1] & (1 << m_scriptEng.operands[2]))
                                          >> m_scriptEng.operands[2];
                break;
            case FUNC_SETBIT:
                if (m_scriptEng.operands[2] <= 0)
                    m_scriptEng.operands[0] &= ~(1 << m_scriptEng.operands[1]);
                else
                    m_scriptEng.operands[0] |= 1 << m_scriptEng.operands[1];
                break;
            case FUNC_CLEARDRAWLIST: opcodeSize = 0; break;
            case FUNC_ADDDRAWLISTENTITYREF: {
                opcodeSize = 0;
                break;
            }
            case FUNC_GETDRAWLISTENTITYREF: opcodeSize = 0; break;
            case FUNC_SETDRAWLISTENTITYREF: opcodeSize = 0; break;
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
                int arrPos = m_scriptEng.operands[1];
                if (arrPos >= 0) {
                    int pos     = m_scriptEng.operands[2];
                    int arrSize = m_scriptData[pos];
                    if (arrPos < arrSize)
                        m_scriptEng.operands[0] = m_scriptData[pos + arrPos + 1];
                }
                break;
            }
            case FUNC_SETTABLEVALUE: {
                opcodeSize = 0;
                int arrPos = m_scriptEng.operands[1];
                if (arrPos >= 0) {
                    int pos     = m_scriptEng.operands[2];
                    int arrSize = m_scriptData[pos];
                    if (arrPos < arrSize)
                        m_scriptData[pos + arrPos + 1] = m_scriptEng.operands[0];
                }
                break;
            }
            case FUNC_CHECKCURRENTSTAGEFOLDER:
                opcodeSize = 0;
                // m_scriptEng.checkResult =
                // StrComp(stageList[activeStageList][stageListPosition].folder, scriptText);
                break;
            case FUNC_ABS: {
                m_scriptEng.operands[0] = abs(m_scriptEng.operands[0]);
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
                int valID = m_scriptEng.operands[1];
                if (valID <= 47)
                    m_scriptEng.operands[0] = m_objectEntityList[m_scriptEng.operands[2]].values[valID];
                break;
            }
            case FUNC_SETOBJECTVALUE: {
                opcodeSize = 0;
                int valID  = m_scriptEng.operands[1];
                if (valID <= 47)
                    m_objectEntityList[m_scriptEng.operands[2]].values[valID] = m_scriptEng.operands[0];
                break;
            }
            case FUNC_COPYOBJECT: {
                opcodeSize = 0;
                // start index, copy offset, count
                Entity *src = &m_objectEntityList[m_scriptEng.operands[0]];
                for (int e = 0; e < m_scriptEng.operands[2]; ++e) {
                    Entity *dst = &src[m_scriptEng.operands[1]];
                    ++src;
                    memcpy(dst, src, sizeof(Entity));
                }
                break;
            }
            case FUNC_PRINT: {
                if (m_scriptEng.operands[1])
                    qDebug() << QString::number(m_scriptEng.operands[0]);
                else
                    qDebug() << scriptText;

                if (m_scriptEng.operands[2])
                    qDebug() << "\n";
                break;
            }
        }

        // Set Values
        if (opcodeSize > 0)
            scriptDataPtr -= scriptDataPtr - scriptCodeOffset;
        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = m_scriptData[scriptDataPtr++];
            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (m_scriptData[scriptDataPtr++]) { // variable
                    case VARARR_NONE: arrayVal = m_objectEntityPos; break;
                    case VARARR_ARRAY:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal = m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]];
                        else
                            arrayVal = m_scriptData[scriptDataPtr++];
                        break;
                    case VARARR_ENTNOPLUS1:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal = m_objectEntityPos
                                       + m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]];
                        else
                            arrayVal = m_objectEntityPos + m_scriptData[scriptDataPtr++];
                        break;
                    case VARARR_ENTNOMINUS1:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal = m_objectEntityPos
                                       - m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]];
                        else
                            arrayVal = m_objectEntityPos - m_scriptData[scriptDataPtr++];
                        break;
                    default: break;
                }

                // Variables
                switch (m_scriptData[scriptDataPtr++]) {
                    default: break;
                    case VAR_TEMP0: m_scriptEng.tempValue[0] = m_scriptEng.operands[i]; break;
                    case VAR_TEMP1: m_scriptEng.tempValue[1] = m_scriptEng.operands[i]; break;
                    case VAR_TEMP2: m_scriptEng.tempValue[2] = m_scriptEng.operands[i]; break;
                    case VAR_TEMP3: m_scriptEng.tempValue[3] = m_scriptEng.operands[i]; break;
                    case VAR_TEMP4: m_scriptEng.tempValue[4] = m_scriptEng.operands[i]; break;
                    case VAR_TEMP5: m_scriptEng.tempValue[5] = m_scriptEng.operands[i]; break;
                    case VAR_TEMP6: m_scriptEng.tempValue[6] = m_scriptEng.operands[i]; break;
                    case VAR_TEMP7: m_scriptEng.tempValue[7] = m_scriptEng.operands[i]; break;
                    case VAR_CHECKRESULT: m_scriptEng.checkResult = m_scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS0: m_scriptEng.arrayPosition[0] = m_scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS1: m_scriptEng.arrayPosition[1] = m_scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS2: m_scriptEng.arrayPosition[2] = m_scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS3: m_scriptEng.arrayPosition[3] = m_scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS4: m_scriptEng.arrayPosition[4] = m_scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS5: m_scriptEng.arrayPosition[5] = m_scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS6: m_scriptEng.arrayPosition[6] = m_scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS7: m_scriptEng.arrayPosition[7] = m_scriptEng.operands[i]; break;
                    case VAR_GLOBAL: /*globalVariables[arrayVal] = m_scriptEng.operands[i];*/ break;
                    case VAR_LOCAL: m_scriptData[arrayVal] = m_scriptEng.operands[i]; break;
                    case VAR_m_objectEntityPos: break;
                    case VAR_OBJECTGROUPID: {
                        m_objectEntityList[arrayVal].typeGroup = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTTYPE: {
                        break;
                    }
                    case VAR_OBJECTPROPERTYVALUE: {
                        break;
                    }
                    case VAR_OBJECTXPOS: {
                        break;
                    }
                    case VAR_OBJECTYPOS: {
                        break;
                    }
                    case VAR_OBJECTIXPOS: {
                        break;
                    }
                    case VAR_OBJECTIYPOS: {
                        break;
                    }
                    case VAR_OBJECTXVEL: {
                        m_objectEntityList[arrayVal].XVelocity = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTYVEL: {
                        m_objectEntityList[arrayVal].YVelocity = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSPEED: {
                        m_objectEntityList[arrayVal].speed = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSTATE: {
                        m_objectEntityList[arrayVal].state = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTROTATION: {
                        m_objectEntityList[arrayVal].rotation = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSCALE: {
                        m_objectEntityList[arrayVal].scale = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPRIORITY: {
                        m_objectEntityList[arrayVal].priority = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDRAWORDER: {
                        m_objectEntityList[arrayVal].drawOrder = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDIRECTION: {
                        m_objectEntityList[arrayVal].direction = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTINKEFFECT: {
                        m_objectEntityList[arrayVal].inkEffect = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTALPHA: {
                        m_objectEntityList[arrayVal].alpha = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFRAME: {
                        m_objectEntityList[arrayVal].frame = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATION: {
                        m_objectEntityList[arrayVal].animation = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPREVANIMATION: {
                        m_objectEntityList[arrayVal].prevAnimation = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATIONSPEED: {
                        m_objectEntityList[arrayVal].animationSpeed = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATIONTIMER: {
                        m_objectEntityList[arrayVal].animationTimer = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANGLE: {
                        m_objectEntityList[arrayVal].angle = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTLOOKPOSX: {
                        m_objectEntityList[arrayVal].camOffsetX = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTLOOKPOSY: {
                        m_objectEntityList[arrayVal].lookPos = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCOLLISIONMODE: {
                        m_objectEntityList[arrayVal].collisionMode = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCOLLISIONPLANE: {
                        m_objectEntityList[arrayVal].collisionPlane = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCONTROLMODE: {
                        m_objectEntityList[arrayVal].controlMode = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTCONTROLLOCK: {
                        m_objectEntityList[arrayVal].controlLock = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPUSHING: {
                        m_objectEntityList[arrayVal].pushing = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVISIBLE: {
                        m_objectEntityList[arrayVal].visible = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTTILECOLLISIONS: {
                        m_objectEntityList[arrayVal].tileCollisions = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTINTERACTION: {
                        m_objectEntityList[arrayVal].objectInteractions = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTGRAVITY: {
                        m_objectEntityList[arrayVal].gravity = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTUP: {
                        m_objectEntityList[arrayVal].up = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDOWN: {
                        m_objectEntityList[arrayVal].down = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTLEFT: {
                        m_objectEntityList[arrayVal].left = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTRIGHT: {
                        m_objectEntityList[arrayVal].right = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTJUMPPRESS: {
                        m_objectEntityList[arrayVal].jumpPress = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTJUMPHOLD: {
                        m_objectEntityList[arrayVal].jumpHold = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSCROLLTRACKING: {
                        m_objectEntityList[arrayVal].trackScroll = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORL: {
                        m_objectEntityList[arrayVal].flailing[0] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORC: {
                        m_objectEntityList[arrayVal].flailing[1] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORR: {
                        m_objectEntityList[arrayVal].flailing[2] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORLC: {
                        m_objectEntityList[arrayVal].flailing[3] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFLOORSENSORRC: {
                        m_objectEntityList[arrayVal].flailing[4] = m_scriptEng.operands[i];
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
                        m_objectScriptList[m_objectEntityList[arrayVal].type].spriteSheetID =
                            m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE0: {
                        m_objectEntityList[arrayVal].values[0] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE1: {
                        m_objectEntityList[arrayVal].values[1] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE2: {
                        m_objectEntityList[arrayVal].values[2] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE3: {
                        m_objectEntityList[arrayVal].values[3] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE4: {
                        m_objectEntityList[arrayVal].values[4] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE5: {
                        m_objectEntityList[arrayVal].values[5] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE6: {
                        m_objectEntityList[arrayVal].values[6] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE7: {
                        m_objectEntityList[arrayVal].values[7] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE8: {
                        m_objectEntityList[arrayVal].values[8] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE9: {
                        m_objectEntityList[arrayVal].values[9] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE10: {
                        m_objectEntityList[arrayVal].values[10] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE11: {
                        m_objectEntityList[arrayVal].values[11] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE12: {
                        m_objectEntityList[arrayVal].values[12] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE13: {
                        m_objectEntityList[arrayVal].values[13] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE14: {
                        m_objectEntityList[arrayVal].values[14] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE15: {
                        m_objectEntityList[arrayVal].values[15] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE16: {
                        m_objectEntityList[arrayVal].values[16] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE17: {
                        m_objectEntityList[arrayVal].values[17] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE18: {
                        m_objectEntityList[arrayVal].values[18] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE19: {
                        m_objectEntityList[arrayVal].values[19] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE20: {
                        m_objectEntityList[arrayVal].values[20] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE21: {
                        m_objectEntityList[arrayVal].values[21] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE22: {
                        m_objectEntityList[arrayVal].values[22] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE23: {
                        m_objectEntityList[arrayVal].values[23] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE24: {
                        m_objectEntityList[arrayVal].values[24] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE25: {
                        m_objectEntityList[arrayVal].values[25] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE26: {
                        m_objectEntityList[arrayVal].values[26] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE27: {
                        m_objectEntityList[arrayVal].values[27] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE28: {
                        m_objectEntityList[arrayVal].values[28] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE29: {
                        m_objectEntityList[arrayVal].values[29] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE30: {
                        m_objectEntityList[arrayVal].values[30] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE31: {
                        m_objectEntityList[arrayVal].values[31] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE32: {
                        m_objectEntityList[arrayVal].values[32] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE33: {
                        m_objectEntityList[arrayVal].values[33] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE34: {
                        m_objectEntityList[arrayVal].values[34] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE35: {
                        m_objectEntityList[arrayVal].values[35] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE36: {
                        m_objectEntityList[arrayVal].values[36] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE37: {
                        m_objectEntityList[arrayVal].values[37] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE38: {
                        m_objectEntityList[arrayVal].values[38] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE39: {
                        m_objectEntityList[arrayVal].values[39] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE40: {
                        m_objectEntityList[arrayVal].values[40] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE41: {
                        m_objectEntityList[arrayVal].values[41] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE42: {
                        m_objectEntityList[arrayVal].values[42] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE43: {
                        m_objectEntityList[arrayVal].values[43] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE44: {
                        m_objectEntityList[arrayVal].values[44] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE45: {
                        m_objectEntityList[arrayVal].values[45] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE46: {
                        m_objectEntityList[arrayVal].values[46] = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE47: {
                        m_objectEntityList[arrayVal].values[47] = m_scriptEng.operands[i];
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
                    case VAR_ENGINEDEVICETYPE: break;
                }
            }
            else if (opcodeType == SCRIPTVAR_INTCONST) { // int constant
                scriptDataPtr++;
            }
            else if (opcodeType == SCRIPTVAR_STRCONST) { // string constant
                int strLen = m_scriptData[scriptDataPtr++];
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
