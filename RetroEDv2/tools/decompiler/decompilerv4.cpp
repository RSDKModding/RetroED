#include "includes.hpp"

namespace RSDKv4
{

enum lineFormats {
    LINE_LF,
    LINE_CRLF,
    LINE_CR,
};

struct AliasInfo {
    AliasInfo() {}
    AliasInfo(QString aliasName, QString aliasVal)
    {
        name  = aliasName;
        value = aliasVal;
    }

    QString name  = "";
    QString value = "";
};

struct FunctionInfo {
    FunctionInfo() {}
    FunctionInfo(QString functionName, int opSize)
    {
        name       = functionName;
        opcodeSize = opSize;
    }

    QString name   = "";
    int opcodeSize = 0;
};

static QList<QString> variableNames = {
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

enum ScrVar {
    VAR_TEMPVALUE0,
    VAR_TEMPVALUE1,
    VAR_TEMPVALUE2,
    VAR_TEMPVALUE3,
    VAR_TEMPVALUE4,
    VAR_TEMPVALUE5,
    VAR_TEMPVALUE6,
    VAR_TEMPVALUE7,
    VAR_CHECKRESULT,
    VAR_ARRAYPOS0,
    VAR_ARRAYPOS1,
    VAR_ARRAYPOS2,
    VAR_ARRAYPOS3,
    VAR_ARRAYPOS4,
    VAR_ARRAYPOS5,
    VAR_PLAYEROBJECTPOS,
    VAR_PLAYEROBJECTCOUNT,
    VAR_GLOBAL,
    VAR_SCRIPTDATA,
    VAR_OBJECTENTITYNO,
    VAR_OBJECTTYPEGROUP,
    VAR_OBJECTTYPE,
    VAR_OBJECTPROPERTYVALUE,
    VAR_OBJECTXPOS,
    VAR_OBJECTYPOS,
    VAR_OBJECTIXPOS,
    VAR_OBJECTIYPOS,
    VAR_OBJECTXVELOCITY,
    VAR_OBJECTYVELOCITY,
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
    VAR_OBJECTVALUEF0,
    VAR_OBJECTLOOKPOS,
    VAR_OBJECTCOLLISIONMODE,
    VAR_OBJECTCOLLISIONPLANE,
    VAR_OBJECTCONTROLMODE,
    VAR_OBJECTCONTROLLOCK,
    VAR_OBJECTPUSHING,
    VAR_OBJECTVISIBLE,
    VAR_OBJECTTILECOLLISIONS,
    VAR_OBJECTOBJECTINTERACTIONS,
    VAR_OBJECTGRAVITY,
    VAR_OBJECTUP,
    VAR_OBJECTDOWN,
    VAR_OBJECTLEFT,
    VAR_OBJECTRIGHT,
    VAR_OBJECTJUMPPRESS,
    VAR_OBJECTJUMPHOLD,
    VAR_OBJECTTRACKSCROLL,
    VAR_OBJECTFLAILING0,
    VAR_OBJECTFLAILING1,
    VAR_OBJECTFLAILING2,
    VAR_OBJECTFLAILING3,
    VAR_OBJECTFLAILING4,
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
    VAR_STAGELIST_STPOS,
    VAR_STAGETIMEENABLED,
    VAR_STAGEMILLISECONDS,
    VAR_STAGESECONDS,
    VAR_STAGEMINUTES,
    VAR_STAGEACTNO,
    VAR_STAGEPAUSEENABLED,
    VAR_STAGELIST_STSIZE,
    VAR_STAGENEWXBOUNDARY1,
    VAR_STAGENEWXBOUNDARY2,
    VAR_STAGENEWYBOUNDARY1,
    VAR_STAGENEWYBOUNDARY2,
    VAR_STAGEXBOUNDARY1,
    VAR_STAGEXBOUNDARY2,
    VAR_STAGEYBOUNDARY1,
    VAR_STAGEYBOUNDARY2,
    VAR_STAGEDEFORMATIONDATA0,
    VAR_STAGEDEFORMATIONDATA1,
    VAR_STAGEDEFORMATIONDATA2,
    VAR_STAGEDEFORMATIONDATA3,
    VAR_STAGEWATERLEVEL,
    VAR_STAGEACTIVELAYER,
    VAR_STAGEMIDPOINT,
    VAR_STAGEPLAYERLISTPOS,
    VAR_STAGEDEBUGMODE,
    VAR_STAGEOBJECTENTITYPOS,
    VAR_SCREENCAMERAENABLED,
    VAR_SCREENCAMERATARGET,
    VAR_SCREENCAMERASTYLE,
    VAR_SCREENCAMERAXPOS,
    VAR_SCREENCAMERAYPOS,
    VAR_SCREENDRAWLISTSIZE,
    VAR_SCREENCENTERX,
    VAR_SCREENCENTERY,
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
    VAR_UNKNOWN,
    VAR_KEYDOWNUP,
    VAR_KEYDOWNDOWN,
    VAR_KEYDOWNLEFT,
    VAR_KEYDOWNRIGHT,
    VAR_KEYDOWNBUTTONA,
    VAR_KEYDOWNBUTTONB,
    VAR_KEYDOWNBUTTONC,
    VAR_KEYDOWNBUTTONA2,
    VAR_KEYDOWNBUTTONB2,
    VAR_KEYDOWNBUTTONC2,
    VAR_KEYDOWNBUTTONA3,
    VAR_KEYDOWNBUTTONB3,
    VAR_KEYDOWNSTART,
    VAR_KEYDOWNSTART2,
    VAR_KEYPRESSUP,
    VAR_KEYPRESSDOWN,
    VAR_KEYPRESSLEFT,
    VAR_KEYPRESSRIGHT,
    VAR_KEYPRESSBUTTONA,
    VAR_KEYPRESSBUTTONB,
    VAR_KEYPRESSBUTTONC,
    VAR_KEYPRESSBUTTONA2,
    VAR_KEYPRESSBUTTONB2,
    VAR_KEYPRESSBUTTONC2,
    VAR_KEYPRESSBUTTONA3,
    VAR_KEYPRESSBUTTONB3,
    VAR_KEYPRESSSTART,
    VAR_KEYPRESSSTART2,
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
    VAR_3DSCENENOVERTICES,
    VAR_3DSCENENOFACES,
    VAR_3DSCENEPROJECTIONX,
    VAR_3DSCENEPROJECTIONY,
    VAR_3DSCENEFOGCOLOR,
    VAR_3DSCENEFOGSTRENGTH,
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
    VAR_ENGINEPLATFORMID,
    VAR_ENGINEHAPTICSENABLED,
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
    FUNC_PLAYEROBJECTCOLLISION,
    FUNC_CREATETEMPOBJECT,
    FUNC_PLAYERTILECOLLISION,
    FUNC_PROCESSPLAYERCONTROL,
    FUNC_PROCESSOBJECTANIMATION,
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
    FUNC_GETARRAYVALUE,
    FUNC_SETARRAYVALUE,
    FUNC_CHECKSTAGEFOLDER,
    FUNC_ABSOLUTE,
    FUNC_ENGINECALLBACK,
    FUNC_CALLENGINEFUNCTION,
    FUNC_CALLENGINEFUNCTION2,
    FUNC_SETOBJECTBORDERX,
    FUNC_GETOBJECTVALUE,
    FUNC_SETOBJECTVALUE,
    FUNC_COPYOBJECT,
    FUNC_HAPTICEFFECT,
    FUNC_MAX_CNT
};

// clang-format off
const QList<FunctionInfo> functions = {
    FunctionInfo("End",	0),         // End of Script
    FunctionInfo("Equal",2),		// Equal
    FunctionInfo("Add",2),			// Add
    FunctionInfo("Sub",2),			// Subtract
    FunctionInfo("Inc",1),			// Increment
    FunctionInfo("Dec",1),			// Decrement
    FunctionInfo("Mul",2),			// Multiply
    FunctionInfo("Div",2),			// Divide
    FunctionInfo("ShR",2),			// Bit Shift Right
    FunctionInfo("ShL",2),			// Bit Shift Left
    FunctionInfo("And",2),			// Bitwise And
    FunctionInfo("Or", 2),          // Bitwise Or
    FunctionInfo("Xor",2),			// Bitwise Xor
    FunctionInfo("Mod",2),			// Mod
    FunctionInfo("FlipSign",1),		// Flips the Sign of the value

    FunctionInfo("CheckEqual",2),		// compare a=b, return result in CheckResult Variable
    FunctionInfo("CheckGreater",2),		// compare a>b, return result in CheckResult Variable
    FunctionInfo("CheckLower",2),		// compare a<b, return result in CheckResult Variable
    FunctionInfo("CheckNotEqual",2),	// compare a!=b, return result in CheckResult Variable

    FunctionInfo("IfEqual",3),          // compare a=b, jump if condition met
    FunctionInfo("IfGreater",3),		// compare a>b, jump if condition met
    FunctionInfo("IfGreaterOrEqual",3),	// compare a>=b, jump if condition met
    FunctionInfo("IfLower",3),          // compare a<b, jump if condition met
    FunctionInfo("IfLowerOrEqual",3),	// compare a<=b, jump if condition met
    FunctionInfo("IfNotEqual",3),		// compare a!=b, jump if condition met
    FunctionInfo("else",0),             // The else for an if statement
    FunctionInfo("endif",0),            // The end if

    FunctionInfo("WEqual",3),           // compare a=b, loop if condition met
    FunctionInfo("WGreater",3),         // compare a>b, loop if condition met
    FunctionInfo("WGreaterOrEqual",3),	// compare a>=b, loop if condition met
    FunctionInfo("WLower",3),           // compare a<b, loop if condition met
    FunctionInfo("WLowerOrEqual",3),	// compare a<=b, loop if condition met
    FunctionInfo("WNotEqual",3),		// compare a!=b, loop if condition met
    FunctionInfo("loop",0),             // While Loop marker

    FunctionInfo("ForEachActive", 3),   // foreach loop, iterates through object group lists only if they are active and interaction is true
    FunctionInfo("ForEachAll", 3),      // foreach loop, iterates through objects matching type
    FunctionInfo("next", 0),            // foreach loop, next marker

    FunctionInfo("switch", 2),          // Switch Statement
    FunctionInfo("break", 0),           //break
    FunctionInfo("endswitch", 0),       //endswitch

    // Math Functions
    FunctionInfo("Rand", 2),
    FunctionInfo("Sin", 2),
    FunctionInfo("Cos", 2),
    FunctionInfo("Sin256", 2),
    FunctionInfo("Cos256", 2),
    FunctionInfo("ATan2", 3),
    FunctionInfo("Interpolate", 4),
    FunctionInfo("InterpolateXY", 7),

    // Graphics Functions
    FunctionInfo("LoadSpriteSheet", 1),
    FunctionInfo("RemoveSpriteSheet", 1),
    FunctionInfo("DrawSprite", 1),
    FunctionInfo("DrawSpriteXY", 3),
    FunctionInfo("DrawSpriteScreenXY", 3),
    FunctionInfo("DrawTintRect", 4),
    FunctionInfo("DrawNumbers", 7),
    FunctionInfo("DrawActName", 7),
    FunctionInfo("DrawMenu", 3),
    FunctionInfo("SpriteFrame", 6),
    FunctionInfo("EditFrame", 7),
    FunctionInfo("LoadPalette", 5),
    FunctionInfo("RotatePalette", 4),
    FunctionInfo("SetScreenFade", 4),
    FunctionInfo("SetActivePalette", 3),
    FunctionInfo("SetPaletteFade", 6),
    FunctionInfo("SetPaletteEntry", 3),
    FunctionInfo("GetPaletteEntry", 3),
    FunctionInfo("CopyPalette", 5),
    FunctionInfo("ClearScreen", 1),
    FunctionInfo("DrawSpriteFX", 4),
    FunctionInfo("DrawSpriteScreenFX", 4),

    // More Useful Stuff
    FunctionInfo("LoadAnimation", 1),
    FunctionInfo("SetupMenu", 4),
    FunctionInfo("AddMenuEntry", 3),
    FunctionInfo("EditMenuEntry", 4),
    FunctionInfo("LoadStage", 0),
    FunctionInfo("DrawRect", 8),
    FunctionInfo("ResetObjectEntity", 5),
    FunctionInfo("BoxCollisionTest", 11),
    FunctionInfo("CreateTempObject", 4),

    // Player and Animation Functions
    FunctionInfo("ProcessObjectMovement", 0),
    FunctionInfo("ProcessObjectControl", 0),
    FunctionInfo("ProcessAnimation", 0),
    FunctionInfo("DrawObjectAnimation", 0),

    // Music
    FunctionInfo("SetMusicTrack", 3),
    FunctionInfo("PlayMusic", 1),
    FunctionInfo("StopMusic", 0),
    FunctionInfo("PauseMusic", 0),
    FunctionInfo("ResumeMusic", 0),
    FunctionInfo("SwapMusicTrack", 4),

    // Sound FX
    FunctionInfo("PlaySfx", 2),
    FunctionInfo("StopSfx", 1),
    FunctionInfo("SetSfxAttributes", 3),

    // More Collision Stuff
    FunctionInfo("ObjectTileCollision", 4),
    FunctionInfo("ObjectTileGrip", 4),

    // Bitwise Not
    FunctionInfo("Not", 1),

    // 3D Stuff
    FunctionInfo("Draw3DScene", 0),
    FunctionInfo("SetIdentityMatrix", 1),
    FunctionInfo("MatrixMultiply", 2),
    FunctionInfo("MatrixTranslateXYZ", 4),
    FunctionInfo("MatrixScaleXYZ", 4),
    FunctionInfo("MatrixRotateX", 2),
    FunctionInfo("MatrixRotateY", 2),
    FunctionInfo("MatrixRotateZ", 2),
    FunctionInfo("MatrixRotateXYZ", 4),
    FunctionInfo("MatrixInverse", 1),
    FunctionInfo("TransformVertices", 3),

    FunctionInfo("CallFunction", 1),
    FunctionInfo("return", 0),

    FunctionInfo("SetLayerDeformation", 6),
    FunctionInfo("CheckTouchRect", 4),
    FunctionInfo("GetTileLayerEntry", 4),
    FunctionInfo("SetTileLayerEntry", 4),

    FunctionInfo("GetBit", 3),
    FunctionInfo("SetBit", 3),

    FunctionInfo("ClearDrawList", 1),
    FunctionInfo("AddDrawListEntityRef", 2),
    FunctionInfo("GetDrawListEntityRef", 3),
    FunctionInfo("SetDrawListEntityRef", 3),

    FunctionInfo("Get16x16TileInfo", 4),
    FunctionInfo("Set16x16TileInfo", 4),
    FunctionInfo("Copy16x16Tile", 2),
    FunctionInfo("GetAnimationByName", 2),
    FunctionInfo("ReadSaveRAM", 0),
    FunctionInfo("WriteSaveRAM", 0),

    FunctionInfo("LoadTextFile", 2),
    FunctionInfo("GetTextInfo", 5),
    FunctionInfo("GetVersionNumber", 2),

    FunctionInfo("GetTableValue", 3),
    FunctionInfo("SetTableValue", 3),

    FunctionInfo("CheckCurrentStageFolder", 1),
    FunctionInfo("Abs", 1),

    FunctionInfo("CallNativeFunction", 1),
    FunctionInfo("CallNativeFunction2", 3),
    FunctionInfo("CallNativeFunction4", 5),

    FunctionInfo("SetObjectRange", 1),
    FunctionInfo("GetObjectValue", 3),
    FunctionInfo("SetObjectValue", 3),
    FunctionInfo("CopyObject", 3),
    FunctionInfo("Print", 3),
};
// clang-format on

// Aliases
static QList<QString> boolAliases = {
    "false",
    "true",
};

static QList<QString> FXAliases = {
    "FX_SCALE", "FX_ROTATE", "FX_ROTOZOOM", "FX_INK", "FX_TINT", "FX_FLIP",
};

static QList<QString> stagesAliases = {
    "PRESENTATION_STAGE",
    "REGULAR_STAGE",
    "BONUS_STAGE",
    "SPECIAL_STAGE",
};

static QList<QString> menuAliases = {
    "MENU_1",
    "MENU_2",
};

static QList<QString> collisionAliases = {
    "C_TOUCH",
    "C_BOX",
    "C_BOX2",
    "C_PLATFORM",
};

static QList<QString> matAliases = {
    "MAT_WORLD",
    "MAT_VIEW",
    "MAT_TEMP",
};

static QList<QString> directionAliases = {
    "FLIP_NONE",
    "FLIP_X",
    "FLIP_Y",
    "FLIP_XY",
};

static QList<QString> engineStateAliases = {
    "RESET_GAME",
};

static QList<QString> stageStateAliases = {
    "STAGE_RUNNING",
    "STAGE_PAUSED",
};

static QList<QString> platformAliases = {
    //"RETRO_WIN", "RETRO_OSX", "RETRO_XBOX_360", "RETRO_PS3", "RETRO_IOS", "RETRO_ANDROID",
    //"RETRO_WP7",
    "RETRO_STANDARD", "RETRO_MOBILE"
};

static QList<QString> inkAliases = {
    "INK_NONE", "INK_BLEND", "INK_ALPHA", "INK_ADD", "INK_SUB",
};

static QList<QString> cSideAliases = {
    "CMODE_FLOOR",
    "CMODE_LWALL",
    "CMODE_RWALL",
    "CMODE_ROOF",
};

static QList<QString> cModeAliases = {
    "CMODE_FLOOR",
    "CMODE_LWALL",
    "CMODE_ROOF",
    "CMODE_RWALL",
};

static QList<QString> cPathAliases = {
    "PATH_A",
    "PATH_B",
};

static QList<QString> gravityAliases = {
    "GRAVITY_GROUND",
    "GRAVITY_AIR",
};

static QList<QString> faceFlagAliases = {
    "FACE_TEXTURED_3D", "FACE_TEXTURED_2D", "FACE_COLOURED_3D", "FACE_COLOURED_2D",
    "FACE_FADED",       "FACE_TEXTURED_C",  "FACE_TEXTURED_D",  "FACE_SPRITE_3D",
};

static QList<QString> priorityAliases = {
    "PRIORITY_ACTIVE_BOUNDS",   "PRIORITY_ACTIVE",   "PRIORITY_ACTIVE_PAUSED", "PRIORITY_XBOUNDS",
    "PRIORITY_XBOUNDS_DESTROY", "PRIORITY_INACTIVE", "PRIORITY_BOUNDS_SMALL",  "PRIORITY_UNKNOWN",
};

static QList<QString> callbackAliases = {
    "CALLBACK_0",
};

static QList<QString> tileInfoAliases = {
    "TILEINFO_INDEX",     "TILEINFO_DIRECTION", "TILEINFO_VISUALPLANE",
    "TILEINFO_SOLIDITYA", "TILEINFO_SOLIDITYB", "TILEINFO_FLAGSA",
    "TILEINFO_ANGLEA",    "TILEINFO_FLAGSB",    "TILEINFO_ANGLEB",
};

static QList<QString> textInfoAliases = {
    "TEXTINFO_TEXTDATA",
    "TEXTINFO_TEXTSIZE",
    "TEXTINFO_ROWCOUNT",
};

static QList<QString> onlineMenuAliases = {
    "ONLINEMENU_ACHIEVEMENTS",
    "ONLINEMENU_LEADERBOARDS",
};

static QList<QString> tileLayerTypeAliases = {
    "TILELAYER_NOSCROLL", "TILELAYER_HSCROLL", "TILELAYER_VSCROLL",
    "TILELAYER_3DFLOOR",  "TILELAYER_3DSKY",
};

static QList<QString> arrayPosition = {
    "arrayPos0", "arrayPos1", "arrayPos2", "arrayPos3",     "arrayPos4",
    "arrayPos5", "arrayPos6", "arrayPos7", "tempObjectPos",
};
}; // namespace RSDKv4

void RSDKv4::Decompiler::clearScriptData()
{
    // m_typeNames.clear();
    //
    // m_functionNames.clear();
    // m_variableNames.clear();
    //
    // m_globalSFXCount = 0;
    // m_sfxNames.clear();
    //
    // m_globalScriptCount = 0;
    // m_useHex            = false;

    m_scriptDataPos    = 0;
    m_jumpTableDataPos = 0;

    m_state         = StateScriptEngine();
    m_functionCount = 0;
    // m_globalfunctionCount = 0;
}

QString curTypeNamev4 = "Blank Object";
QString RSDKv4::Decompiler::setArrayValue(QString strIn, QString index)
{
    QString strOut = strIn;
    int point      = -1;

    if (strIn == "global") {
        strOut = m_variableNames[index.toInt()];
        if (strOut == "")
            return strIn;
        return strOut;
    }
    else if (strIn == "local") {
        bool ok = false;
        int idx = index.toInt(&ok);
        if (ok) {
            StaticVarInfo c(QString("%1_value%2").arg(curTypeNamev4).arg(m_staticVars.count()), idx);

            if (!m_staticVars.contains(c)) {
                strOut = c.m_name;
                m_staticVars.append(c);
            }
            else if (m_staticVars.indexOf(c) >= 0) {
                strOut = m_staticVars[m_staticVars.indexOf(c)].m_name;
            }
        }
        else {
            strOut = QString("local[%1]").arg(index);
        }

        return strOut;
    }
    else {
        for (int i = 0; i < strIn.length(); ++i) {
            if (strIn[i] == '.') {
                point = i;
                break;
            }
        }
        if (point >= 0) {
            QStringList split = strIn.split('.');
            strOut            = split[0] + "[" + index + "]." + split[1];
            return strOut;
        }
        else {
            strOut = strOut + "[" + index + "]";
            return strOut;
        }
    }
}

struct ScriptPtrv4 {
    QString m_name  = "";
    int m_startPtr  = 0;
    int m_endPtr    = 0;
    int m_jumpStart = 0;
    bool m_function = false;
    ScriptPtrv4() {}
    ScriptPtrv4(QString n, int s, int j, bool f)
    {
        m_name      = n;
        m_startPtr  = s;
        m_endPtr    = 0;
        m_jumpStart = j;
        m_function  = f;
    }
    ScriptPtrv4(const ScriptPtrv4 &other)
    {
        m_name      = other.m_name;
        m_startPtr  = other.m_startPtr;
        m_endPtr    = other.m_endPtr;
        m_jumpStart = other.m_jumpStart;
        m_function  = other.m_function;
    }
};

int funcIDv4       = 0;
byte debugNameFlag = 0;
QList<ScriptPtrv4> scriptPtrs;

void RSDKv4::Decompiler::decompile(RSDKv4::Bytecode bytecode, QString destPath)
{
    clearScriptData();

    funcIDv4 = m_globalFunctionCount; // what function to use
    for (int i = m_globalScriptCount; i < m_sourceNames.count(); ++i) {
        RSDKv4::Bytecode::ObjectScript &objectScript = bytecode.m_scriptList[i];
        int scriptCodePtrs[3]    = { objectScript.m_mainScript, objectScript.m_drawScript,
                                  objectScript.m_startupScript };
        int lowestScriptCodePtr  = 0x3FFFF;
        int highestScriptCodePtr = 0;

        for (int i = 0; i < 3; ++i) {
            if (scriptCodePtrs[i] < 0x3FFFF && scriptCodePtrs[i] > highestScriptCodePtr)
                highestScriptCodePtr = scriptCodePtrs[i];
        }

        for (int i = 0; i < 3; ++i) {
            if (scriptCodePtrs[i] < 0x3FFFF && scriptCodePtrs[i] < lowestScriptCodePtr)
                lowestScriptCodePtr = scriptCodePtrs[i];
        }

        for (int f = funcIDv4; f < bytecode.m_functionList.count(); ++f) {
            auto &func = bytecode.m_functionList[f];
            if (func.m_mainScript < lowestScriptCodePtr && lowestScriptCodePtr < 0x3FFFF) {
                QString tn = m_typeNames[i];
                m_functionNames.append(tn.replace(" ", "") + "_Function" + QString::number(f));
                funcIDv4++;
            }
        }
    }

    for (int i = m_globalScriptCount; i < m_sourceNames.count(); ++i) {
        // check to give proper names
    }

    funcIDv4      = m_globalFunctionCount; // what function to use
    curTypeNamev4 = "Blank Object";

    for (int i = m_globalScriptCount; i < m_sourceNames.count(); ++i) {
        if (i == 0)
            continue;
        QString path       = "";
        QString scriptPath = m_sourceNames[i];
        QString baseDir    = destPath + "/Scripts/";
        if (m_useCustomAliases || m_seperateFolders) {
            baseDir = destPath + "/Scripts/" + QFileInfo(bytecode.m_filename).baseName() + "/";
        }
        QString dir = baseDir + scriptPath.replace(QFileInfo(scriptPath).fileName(), "");
        scriptPath  = m_sourceNames[i];

        if (!QDir(QDir::tempPath()).exists(dir)) {
            QDir(QDir::tempPath()).mkpath(dir);
        }

        // qDebug() << baseDir << scriptPath;
        path = baseDir + scriptPath;

        if (QFile::exists(path))
            QFile::remove(path);
        Writer writer(path);

        RSDKv4::Bytecode::ObjectScript &objectScript = bytecode.m_scriptList[i];

        int scriptCodePtrs[3]    = { objectScript.m_mainScript, objectScript.m_drawScript,
                                  objectScript.m_startupScript };
        int lowestScriptCodePtr  = 0x3FFFF;
        int highestScriptCodePtr = 0;

        for (int i = 0; i < 3; ++i) {
            if (scriptCodePtrs[i] < 0x3FFFF && scriptCodePtrs[i] > highestScriptCodePtr)
                highestScriptCodePtr = scriptCodePtrs[i];
        }

        for (int i = 0; i < 3; ++i) {
            if (scriptCodePtrs[i] < 0x3FFFF && scriptCodePtrs[i] < lowestScriptCodePtr)
                lowestScriptCodePtr = scriptCodePtrs[i];
        }

        curTypeNamev4 = m_typeNames[i];
        curTypeNamev4 = curTypeNamev4.replace(" ", "");

        scriptPtrs.clear();

        if (objectScript.m_mainScript < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectMain", objectScript.m_mainScript,
                                          objectScript.m_mainJumpTable, false));

        if (objectScript.m_drawScript < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectDraw", objectScript.m_drawScript,
                                          objectScript.m_drawJumpTable, false));

        if (objectScript.m_startupScript < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectStartup", objectScript.m_startupScript,
                                          objectScript.m_startupJumpTable, false));
        std::sort(scriptPtrs.begin(), scriptPtrs.end(),
                  [](const ScriptPtrv4 &a, const ScriptPtrv4 &b) -> bool {
                      return a.m_startPtr < b.m_startPtr;
                  });

        int lastPtr = scriptPtrs.count() > 0 ? scriptPtrs[scriptPtrs.count() - 1].m_startPtr : 0x3FFFF;
        scriptPtrs.clear(); // temp, used to sort

        bool flag = false;
        for (int f = funcIDv4; f < bytecode.m_functionList.count(); ++f) {
            int fs = bytecode.m_functionList[f].m_mainScript;
            if (fs < lastPtr) {
                scriptPtrs.append(ScriptPtrv4(m_functionNames[f],
                                              bytecode.m_functionList[f].m_mainScript,
                                              bytecode.m_functionList[f].m_mainJumpTable, true));
            }
            else if (!flag) {
                flag     = true;
                funcIDv4 = f;
            }
        }
        if (!flag) {
            funcIDv4 = bytecode.m_functionList.count();
        }

        if (objectScript.m_mainScript < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectMain", objectScript.m_mainScript,
                                          objectScript.m_mainJumpTable, false));

        if (objectScript.m_drawScript < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectDraw", objectScript.m_drawScript,
                                          objectScript.m_drawJumpTable, false));

        if (objectScript.m_startupScript < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectStartup", objectScript.m_startupScript,
                                          objectScript.m_startupJumpTable, false));

        QByteArray buf;
        QBuffer buffer(&buf);
        buffer.open(QIODevice::Append);
        QDataStream *cmem = new QDataStream(&buffer);
        Writer bufWriter(cmem);

        int fCnt      = 0;
        debugNameFlag = 0;
        for (int s = 0; s < scriptPtrs.count(); ++s) {
            if (scriptPtrs[s].m_name != "ObjectMain" && scriptPtrs[s].m_name != "ObjectDraw"
                && scriptPtrs[s].m_name != "ObjectStartup") {
                fCnt++;
            }
            else if (scriptPtrs[s].m_name == "ObjectStartup" && curTypeNamev4 != "DebugMode") {
                QByteArray buf2;
                QBuffer buffer2(&buf2);
                buffer2.open(QIODevice::Append);
                QDataStream *cmem2 = new QDataStream(&buffer2);
                Writer bufWriter2(cmem2);

                // do this to get debugMode func names
                debugNameFlag = 1;
                decompileScript(bytecode, bufWriter2, scriptPtrs[s].m_startPtr,
                                scriptPtrs[s].m_jumpStart, scriptPtrs[s].m_name,
                                scriptPtrs[s].m_function);
                debugNameFlag = 0;
            }
        }

        QList<ScriptPtrv4> unsortedPtrs = scriptPtrs;
        std::sort(scriptPtrs.begin(), scriptPtrs.end(),
                  [](const ScriptPtrv4 &a, const ScriptPtrv4 &b) -> bool {
                      return a.m_startPtr < b.m_startPtr;
                  });

        for (int s = 0; s < scriptPtrs.count(); ++s) {
            bufWriter.writeLine("", LINE_CRLF);
            debugNameFlag = scriptPtrs[s].m_name == "ObjectStartup" && curTypeNamev4 != "DebugMode";
            if (debugNameFlag)
                debugNameFlag++;

            int end                = decompileScript(bytecode, bufWriter, scriptPtrs[s].m_startPtr,
                                      scriptPtrs[s].m_jumpStart, scriptPtrs[s].m_name,
                                      scriptPtrs[s].m_function);
            scriptPtrs[s].m_endPtr = end;
            debugNameFlag          = 0;
        }

        writer.writeLine("// ----------------------------------");
        writer.writeLine("// RSDK Project: Sonic 1/Sonic 2");
        writer.writeLine("// Script Description: " + m_typeNames[i] + " Object");
        writer.writeLine("// Script Author: Christan Whitehead/Simon Thomley");
        writer.writeLine("// Unpacked by Rubberduckycooly's script unpacker");
        writer.writeLine("// ----------------------------------");
        writer.writeLine("", LINE_CRLF);

        writer.writeLine("//-------Aliases-------//", LINE_CRLF);

        writer.writeLine("private alias " + QString::number(i) + " : TYPE_"
                             + m_typeNames[i].toUpper().replace(" ", ""),
                         LINE_CRLF);
        if (i == 1) {
            writer.writeLine("public alias 256 : GROUP_PLAYERS", LINE_CRLF);
            writer.writeLine("public alias 65536 : HITBOX_AUTO", LINE_CRLF);
            writer.writeLine("public alias arrayPos6 : currentPlayer", LINE_CRLF);
            writer.writeLine("public alias arrayPos7 : playerCount", LINE_CRLF);
        }

        writer.writeLine("", LINE_CRLF);

        if (fCnt)
            writer.writeLine("// Function declarations", LINE_CRLF);

        for (int s = 0; s < unsortedPtrs.count(); ++s) {
            if (unsortedPtrs[s].m_name != "ObjectMain" && unsortedPtrs[s].m_name != "ObjectDraw"
                && unsortedPtrs[s].m_name != "ObjectStartup") {
                writer.writeLine("reserve function " + unsortedPtrs[s].m_name, LINE_CRLF);
            }
        }

        bufWriter.writeLine("event RSDKDraw", LINE_CRLF);
        bufWriter.writeLine("\tDrawSprite(0)", LINE_CRLF);
        bufWriter.writeLine("end event", LINE_CRLF);
        bufWriter.writeLine("", LINE_CRLF);
        bufWriter.writeLine("event RSDKLoad", LINE_CRLF);
        bufWriter.writeLine("\tLoadSpriteSheet(\"Global/Display.gif\")", LINE_CRLF);
        bufWriter.writeLine("\tSpriteFrame(-16, -16, 32, 32, 1, 143)", LINE_CRLF);
        bufWriter.writeLine("end event", LINE_CRLF);

        if (m_staticVars.count() > 0) {
            writer.writeLine("", LINE_CRLF);
            writer.writeLine("// Static Values", LINE_CRLF);
        }
        QList<int> registeredPos;

        int firstOffset = m_lastOffset;
        int endOffset   = scriptPtrs[scriptPtrs.count() - 1].m_endPtr;

        std::sort(m_tables.begin(), m_tables.end(),
                  [](const StaticVarInfo &a, const StaticVarInfo &b) -> bool {
                      return a.m_dataPos < b.m_dataPos;
                  });

        std::sort(m_staticVars.begin(), m_staticVars.end(),
                  [](const StaticVarInfo &a, const StaticVarInfo &b) -> bool {
                      return a.m_dataPos < b.m_dataPos;
                  });

        for (int c = m_globalStaticCount; c < m_staticVars.count(); ++c) {
            int index = m_staticVars[c].m_dataPos;
            if (index >= firstOffset && index < endOffset) {
                writer.writeLine(QString("public %2 = %1;")
                                     .arg(toHexString(QString::number(bytecode.m_scriptData[index])))
                                     .arg(m_staticVars[c].m_name),
                                 LINE_CRLF);
                registeredPos.append(index);
            }
        }

        if (m_tables.count() > 0) {
            writer.writeLine("", LINE_CRLF);
            writer.writeLine("// Tables", LINE_CRLF);
        }
        for (int a = m_globalArrayCount; a < m_tables.count(); ++a) {
            int index = m_tables[a].m_dataPos;
            if (index >= firstOffset && index < endOffset) {
                writer.writeLine(QString("public table %1").arg(m_tables[a].m_name), LINE_CRLF);
                registeredPos.append(index);
                writer.writeText("\t");
                for (int v = 1; v < bytecode.m_scriptData[index] + 1; ++v) {
                    writer.writeText(toHexString(QString::number(bytecode.m_scriptData[index + v])));
                    registeredPos.append(index + v);
                    if ((v % 0x10) == 0) {
                        writer.writeLine("", LINE_CRLF);
                        writer.writeText("\t");
                    }
                    else if (v + 1 < bytecode.m_scriptData[index] + 1) {
                        writer.writeText(", ");
                    }
                }
                writer.writeLine("", LINE_CRLF);
                writer.writeLine("end table", LINE_CRLF);
                writer.writeLine("", LINE_CRLF);
            }
        }

        // Unknowns
        int constCnt = 0;
        if (scriptPtrs[0].m_startPtr - m_lastOffset > 0) {
            for (int b = m_lastOffset; b < scriptPtrs[0].m_startPtr; ++b) {
                if (!registeredPos.contains(b) && (b >= firstOffset && b < endOffset))
                    constCnt++;
            }
        }

        for (int s = 0; s < scriptPtrs.count() - 1; ++s) {
            int dif = scriptPtrs[s + 1].m_startPtr - scriptPtrs[s].m_endPtr;
            if (dif > 0) {
                for (int b = scriptPtrs[s].m_endPtr; b < scriptPtrs[s].m_endPtr + dif; ++b) {
                    if (!registeredPos.contains(b) && (b >= firstOffset && b < endOffset))
                        constCnt++;
                }
            }
        }

        if (constCnt > 0) {
            writer.writeLine("", LINE_CRLF);
            writer.writeLine("//Unknown Variables", LINE_CRLF);

            if (scriptPtrs[0].m_startPtr - m_lastOffset > 0) {
                for (int b = m_lastOffset; b < scriptPtrs[0].m_startPtr; ++b) {
                    if (!registeredPos.contains(b) && (b >= firstOffset && b < endOffset))
                        writer.writeLine(
                            QString("private value value%2 = %1;")
                                .arg(toHexString(QString::number(bytecode.m_scriptData[b])))
                                .arg(toHexString(QString::number(b))),
                            LINE_CRLF);
                }
            }

            for (int s = 0; s < scriptPtrs.count() - 1; ++s) {
                int dif = scriptPtrs[s + 1].m_startPtr - scriptPtrs[s].m_endPtr;
                if (dif > 0) {
                    for (int b = scriptPtrs[s].m_endPtr; b < scriptPtrs[s].m_endPtr + dif; ++b) {
                        if (!registeredPos.contains(b) && (b >= firstOffset && b < endOffset))
                            writer.writeLine(
                                QString("private value value%2 = %1;")
                                    .arg(toHexString(QString::number(bytecode.m_scriptData[b])))
                                    .arg(toHexString(QString::number(b))),
                                LINE_CRLF);
                    }
                }
            }
        }
        m_lastOffset = scriptPtrs[scriptPtrs.count() - 1].m_endPtr;

        bufWriter.flush();
        QString str = QString::fromLatin1(buf);
        writer.writeText(buf);
        writer.flush();
    }
}

int RSDKv4::Decompiler::decompileScript(RSDKv4::Bytecode &bytecode, Writer &writer, int scriptCodePtr,
                                        int jumpTablePtr, QString subName, bool isFunction)
{
    if (!isFunction)
        writer.writeLine("event " + subName, LINE_CRLF);
    else
        writer.writeLine("function " + subName, LINE_CRLF);

    StateScriptEngine state;
    state.m_scriptCodePtr = state.m_scriptCodeOffset = scriptCodePtr;
    state.m_jumpTablePtr = state.m_jumpTableOffset = jumpTablePtr;
    state.m_deep                                   = 1;
    state.m_switchDeep                             = -1;

    decompileSub(bytecode, writer, state, isFunction);
    writer.writeLine("", LINE_CRLF);
    return state.m_scriptCodePtr;
}

void RSDKv4::Decompiler::decompileSub(RSDKv4::Bytecode &bytecode, Writer writer,
                                      RSDKv4::Decompiler::StateScriptEngine &state, bool isFunction)
{
    state.m_endFlag      = false;
    RSDKv4::Bytecode &bc = bytecode;

    while (!state.m_endFlag) {
        if (state.m_switchDeep >= 0) {
            SwitchState &sw = state.m_switchState[state.m_switchDeep];
            for (int j = 0; j < sw.m_jumpPtr.count(); ++j) {
                if (sw.m_jumpPtr[j].m_jump == state.m_scriptCodePtr) {
                    if (sw.m_jumpPtr[j].m_jump != sw.m_endJmp - 1
                        && sw.m_jumpPtr[j].m_jump != sw.m_endJmp
                        && sw.m_jumpPtr[j].m_jump != sw.m_defaultJmp) {
                        for (auto &c : sw.m_jumpPtr[j].m_cases) {
                            for (int i = 0; i < state.m_deep - 1; ++i) writer.writeText("\t");
                            writer.writeLine("case " + QString::number(c), LINE_CRLF);
                        }
                    }
                }
            }

            if (sw.m_defaultJmp + state.m_scriptCodeOffset == state.m_scriptCodePtr) {
                if (sw.m_defaultJmp != sw.m_endJmp - 1 && sw.m_defaultJmp != sw.m_endJmp) {
                    for (int i = 0; i < state.m_deep - 1; ++i) writer.writeText("\t");
                    writer.writeLine("default", LINE_CRLF);
                }
            }
        }

        int opcode      = bc.m_scriptData[state.m_scriptCodePtr++];
        int paramsCount = functions[opcode].opcodeSize;

        QList<QString> variableName;

        for (int i = 0; i < 0x10; ++i) variableName.append("UNKNOWN VARIABLE");

        for (int i = 0; i < paramsCount; ++i) {
            int paramId = bc.m_scriptData[state.m_scriptCodePtr++];
            switch (paramId) {
                case 0: // Unused
                    break;
                case 1: // Read value from RSDK
                    switch (bc.m_scriptData[state.m_scriptCodePtr++]) {
                        case 0: // Read Const Variable
                            variableName[i] = variableNames[bc.m_scriptData[state.m_scriptCodePtr++]];
                            break;
                        case 1:                                                // ARRAY
                            if (bc.m_scriptData[state.m_scriptCodePtr++] == 1) // Variable
                            {
                                QString value = arrayPosition[bc.m_scriptData[state.m_scriptCodePtr++]];
                                variableName[i] = setArrayValue(
                                    variableNames[bc.m_scriptData[state.m_scriptCodePtr++]], value);
                            }
                            else // Value
                            {
                                QString value =
                                    QString::number(bc.m_scriptData[state.m_scriptCodePtr++]);
                                variableName[i] = setArrayValue(
                                    variableNames[bc.m_scriptData[state.m_scriptCodePtr++]], value);
                            }
                            break;
                        case 2:
                            // ObjectLoop +
                            if (bc.m_scriptData[state.m_scriptCodePtr++] == 1) {
                                QString value =
                                    "+" + arrayPosition[bc.m_scriptData[state.m_scriptCodePtr++]];
                                variableName[i] = setArrayValue(
                                    variableNames[bc.m_scriptData[state.m_scriptCodePtr++]], value);
                            }
                            else {
                                QString value =
                                    "+" + QString::number(bc.m_scriptData[state.m_scriptCodePtr++]);
                                variableName[i] = setArrayValue(
                                    variableNames[bc.m_scriptData[state.m_scriptCodePtr++]], value);
                            }
                            break;
                        case 3:
                            // ObjectLoop -
                            if (bc.m_scriptData[state.m_scriptCodePtr++] == 1) {
                                QString value =
                                    "-" + arrayPosition[bc.m_scriptData[state.m_scriptCodePtr++]];
                                variableName[i] = setArrayValue(
                                    variableNames[bc.m_scriptData[state.m_scriptCodePtr++]], value);
                            }
                            else {
                                QString value =
                                    "-" + QString::number(bc.m_scriptData[state.m_scriptCodePtr++]);
                                variableName[i] = setArrayValue(
                                    variableNames[bc.m_scriptData[state.m_scriptCodePtr++]], value);
                            }
                            break;
                    }
                    break;
                case 2: // Read constant value from bytecode
                    variableName[i] = QString::number(bc.m_scriptData[state.m_scriptCodePtr++]);
                    break;
                case 3: // Read string
                    QString tmp = "";
                    int strLen  = bc.m_scriptData[state.m_scriptCodePtr];
                    for (int j = 0; j < strLen;) {
                        state.m_scriptCodePtr++;
                        if (j < strLen) {
                            int val = bc.m_scriptData[state.m_scriptCodePtr] >> 24;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;

                        if (j < strLen) {
                            int val = (bc.m_scriptData[state.m_scriptCodePtr] & 0x00FFFFFF) >> 16;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;

                        if (j < strLen) {
                            int val = (bc.m_scriptData[state.m_scriptCodePtr] & 0x0000FFFF) >> 8;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;

                        if (j < strLen) {
                            int val = bc.m_scriptData[state.m_scriptCodePtr] & 0x000000FF;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;
                    }
                    variableName[i] = '"' + tmp + '"';
                    if ((strLen & 3) == 0) {
                        state.m_scriptCodePtr += 2;
                        break;
                    }
                    state.m_scriptCodePtr++;
                    break;
            }
        }

        QString operand = functions[opcode].name;

        /*if (operand == "End" || operand == "EndFunction") {
            if (isFunction)
                writer.writeText("end function");
            else
                writer.writeText("end event");
            state.m_endFlag = true;
            state.m_deep    = 0;
        }*/

        if (!state.m_endFlag) {
            // Check what opcodes terminates a statement
            switch (opcode) {
                case FUNC_END: break;
                case FUNC_ELSE:
                    for (int i = 0; i < state.m_deep - 1; ++i) writer.writeText("\t");
                    break;
                case FUNC_ENDIF:
                    state.m_deep--;
                    for (int i = 0; i < state.m_deep; ++i) writer.writeText("\t");
                    break;
                case FUNC_LOOP:
                case FUNC_NEXT:
                    state.m_deep--;
                    for (int i = 0; i < state.m_deep; ++i) writer.writeText("\t");
                    break;
                case FUNC_ENDSWITCH:
                    state.m_deep--;
                    for (int i = 0; i < state.m_deep; ++i) writer.writeText("\t");
                    break;
                case FUNC_RETURN:
                    if (state.m_deep <= 1)
                        break;
                    [[fallthrough]];
                default:
                    for (int i = 0; i < state.m_deep; ++i) writer.writeText("\t");
                    break;
            }

            for (int i = 0; i < 0x10; ++i) {
                if (variableName[i].contains("arrayPos6"))
                    variableName[i].replace("arrayPos6", "currentPlayer");
                if (variableName[i].contains("arrayPos7"))
                    variableName[i].replace("arrayPos7", "playerCount");
            }

            // aliases
            {
                if (opcode >= functions.count()) {
                    writer.writeText("ERROR AT: " + QString::number(state.m_scriptCodePtr) + " : "
                                     + opcode);
                    qDebug() << "OPCODE ABOVE THE MAX OPCODES";
                    return;
                }

                for (int i = 0; i < variableName.length(); ++i) {
                    if (variableName[i] == "") {
                        variableName[i] = "object.value0";
                    }
                }

                if (variableName[0].contains("object") && variableName[0].contains(".type")) {
                    bool ok = false;
                    int id  = variableName[1].toInt(&ok);
                    if (ok && id < m_typeNames.count())
                        variableName[1] = "TypeName[" + m_typeNames[id] + "]";
                }

                if (variableName[0].contains("animalType")) {
                    bool ok = false;
                    int id  = variableName[1].toInt(&ok);
                    if (ok && id < m_typeNames.count())
                        variableName[1] = "TypeName[" + m_typeNames[id] + "]";
                }

                if (variableName[0].contains("tileLayer") && variableName[0].contains(".type")) {
                    bool ok = false;
                    int id  = variableName[1].toInt(&ok);
                    if (ok && id < tileLayerTypeAliases.count())
                        variableName[1] = tileLayerTypeAliases[id];
                }

                if (variableName[0].contains("_type")) {
                    bool ok = false;
                    int id  = variableName[1].toInt(&ok);
                    if (ok && id < m_typeNames.count())
                        variableName[1] = "TypeName[" + m_typeNames[id] + "]";
                }

                if (variableName[1].contains("object") && variableName[1].contains(".type")) {
                    bool ok = false;
                    int id  = variableName[2].toInt(&ok);
                    if (ok && id < m_typeNames.count())
                        variableName[2] = "TypeName[" + m_typeNames[id] + "]";
                }

                if (variableName[1].contains("animalType")) {
                    bool ok = false;
                    int id  = variableName[2].toInt(&ok);
                    if (ok && id < m_typeNames.count())
                        variableName[2] = "TypeName[" + m_typeNames[id] + "]";
                }

                if (variableName[1].contains("tileLayer") && variableName[1].contains(".type")) {
                    bool ok = false;
                    int id  = variableName[2].toInt(&ok);
                    if (ok && id < tileLayerTypeAliases.count())
                        variableName[2] = tileLayerTypeAliases[id];
                }

                if (variableName[0] == "SpeedUpMusic" || variableName[0] == "SlowDownMusic") {
                    bool ok = false;
                    int id  = variableName[1].toInt(&ok);
                    if (ok) {
                        variableName[1] = m_functionNames[id];
                    }
                }

                // Special Aliases for some functions
                QList<QString> operandSwitchList = {
                    "DrawSpriteFX",     "DrawSpriteScreenFX",  "BoxCollisionTest",
                    "CreateTempObject", "ResetObjectEntity",   "PlaySfx",
                    "StopSfx",          "PlayStageSfx",        "StopStageSfx",
                    "SetSfxAttributes", "ObjectTileCollision", "ObjectTileGrip",
                    "MatrixMultiply",   "SetIdentityMatrix",   "MatrixTranslateXYZ",
                    "MatrixScaleXYZ",   "MatrixRotateX",       "MatrixRotateY",
                    "MatrixRotateZ",    "MatrixRotateXYZ",     "TransformVertices",
                    "DrawMenu",         "SetupMenu",           "AddMenuEntry",
                    "EditMenuEntry",    "LoadTextFile",        "DrawText",
                    "GetVersionNumber", "GetTextInfo",         "LoadOnlineMenu",
                    "Get16x16TileInfo", "Set16x16TileInfo",
                };
                int opID = operandSwitchList.indexOf(operand);
                switch (opID) {
                    default: break;
                    case 0: //"DrawSpriteFX"
                    {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok && id < FXAliases.count())
                            variableName[1] = FXAliases[id];
                        break;
                    }
                    case 1: //"DrawSpriteScreenFX"
                    {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok && id < FXAliases.count())
                            variableName[1] = FXAliases[id];
                        break;
                    }
                    case 2: //"BoxCollisionTest"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok) {
                            if (id < collisionAliases.count())
                                variableName[0] = collisionAliases[id];
                        }

                        for (int h = 2; h < 6; ++h) {
                            ok = false;
                            id = variableName[h].toInt(&ok);
                            if (ok) {
                                if (id == 0x10000)
                                    variableName[h] = "HITBOX_AUTO";
                            }
                        }

                        for (int h = 7; h < 11; ++h) {
                            ok = false;
                            id = variableName[h].toInt(&ok);
                            if (ok) {
                                if (id == 0x10000)
                                    variableName[h] = "HITBOX_AUTO";
                            }
                        }
                        break;
                    }
                    case 3: //"CreateTempObject"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok && id < m_typeNames.count())
                            variableName[0] = "TypeName[" + m_typeNames[id] + "]";
                        break;
                    }
                    case 4: //"ResetObjectEntity"
                    {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok && id < m_typeNames.count())
                            variableName[1] = "TypeName[" + m_typeNames[id] + "]";
                        break;
                    }
                    case 5: //"PlaySfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id < m_sfxNames.count())
                                variableName[0] = "SfxName[" + m_sfxNames[id] + "]";
                        break;
                    }
                    case 6: //"StopSfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id < m_sfxNames.count())
                                variableName[0] = "SfxName[" + m_sfxNames[id] + "]";
                        break;
                    }
                    case 7: //"PlayStageSfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id + m_globalSFXCount < m_sfxNames.count())
                                variableName[0] = "SfxName[" + m_sfxNames[id + m_globalSFXCount] + "]";
                        break;
                    }
                    case 8: //"StopStageSfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id + m_globalSFXCount < m_sfxNames.count())
                                variableName[0] = "SfxName[" + m_sfxNames[id + m_globalSFXCount] + "]";
                        break;
                    }
                    case 9: //"SetSfxAttributes"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id < m_sfxNames.count())
                                variableName[0] = "SfxName[" + m_sfxNames[id] + "]";
                        break;
                    }
                    case 10: //"ObjectTileCollision"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id < cSideAliases.count())
                                variableName[0] = cSideAliases[id];
                        break;
                    }
                    case 11: //"ObjectTileGrip"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id < cSideAliases.count())
                                variableName[0] = cSideAliases[id];
                        break;
                    }
                    case 12: //"MatrixMultiply"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok) {
                            if (id < matAliases.count())
                                variableName[0] = matAliases[id];
                        }

                        ok = false;
                        id = variableName[1].toInt(&ok);
                        if (ok) {
                            if (id < matAliases.count())
                                variableName[1] = matAliases[id];
                        }
                        break;
                    }
                    case 13: // "SetIdentityMatrix"
                    case 14: // "MatrixTranslateXYZ"
                    case 15: // "MatrixScaleXYZ"
                    case 16: // "MatrixRotateX"
                    case 17: // "MatrixRotateY"
                    case 18: // "MatrixRotateZ"
                    case 19: // "MatrixRotateXYZ"
                    case 20: // "TransformVertices"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok) {
                            if (id < matAliases.count())
                                variableName[0] = matAliases[id];
                        }
                        break;
                    }
                    case 21: //"DrawMenu"
                    case 22: //"SetupMenu"
                    case 23: //"AddMenuEntry"
                    case 24: //"EditMenuEntry"
                    case 25: //"LoadTextFile"
                    case 26: //"DrawText"
                    case 27: //"GetVersionNumber"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok && id < menuAliases.count())
                            variableName[0] = menuAliases[id];
                        break;
                    }
                    case 28: //"GetTextInfo"
                    {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok && id < menuAliases.count())
                            variableName[1] = menuAliases[id];
                        ok = false;
                        id = variableName[2].toInt(&ok);
                        if (ok && id < textInfoAliases.count())
                            variableName[2] = textInfoAliases[id];

                        break;
                    }
                    case 29: //"LoadOnlineMenu"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok && id < onlineMenuAliases.count())
                            variableName[0] = onlineMenuAliases[id];

                        break;
                    }
                    case 30: //"Get16x16TileInfo"
                    case 31: //"Set16x16TileInfo"
                    {
                        bool ok = false;
                        int id  = variableName[3].toInt(&ok);
                        if (ok && id < tileInfoAliases.count())
                            variableName[3] = tileInfoAliases[id];

                        break;
                    }
                }

                if (opcode < FUNC_LOOP) {
                    operandSwitchList = { "engine.deviceType", "stage.activeList", "stage.state",
                                          "engine.state" };
                    opID              = operandSwitchList.indexOf(variableName[0]);
                    switch (opID) {
                        case 0: //"Engine.PlatformID":
                        {
                            bool ok = false;
                            int id  = variableName[1].toInt(&ok);
                            if (ok && id < platformAliases.count())
                                variableName[1] = platformAliases[id];
                            break;
                        }
                        case 1: //"Stage.ActiveList":
                        {
                            bool ok = false;
                            int id  = variableName[1].toInt(&ok);
                            if (ok && id < stagesAliases.count())
                                variableName[1] = stagesAliases[id];
                            break;
                        }
                        case 2: //"Stage.State":
                        {
                            bool ok = false;
                            int id  = variableName[1].toInt(&ok);
                            if (ok && id - 1 < stageStateAliases.count())
                                variableName[1] = stageStateAliases[id - 1];
                            break;
                        }
                        case 3: // Engine.State
                        {
                            bool ok = false;
                            int id  = variableName[1].toInt(&ok);
                            if (ok) {
                                if (id == 2)
                                    variableName[1] = engineStateAliases[0];
                            }
                            break;
                        }
                        case 4: // Engine.Message
                        {
                            // bool ok = false;
                            // int id  = variableName[1].toInt(&ok);
                            // if (ok) {
                            //    if (id < 0 || id > messageAliases.count())
                            //        variableName[1] = "MESSAGE_" + QString::number(id);
                            //    else
                            //        variableName[1] = messageAliases[id];
                            //}
                            break;
                        }
                    }

                    if (variableName[0].contains(".direction")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok) {
                            if (id < directionAliases.count()) {
                                variableName[1] = directionAliases[id];
                            }
                        }
                    }

                    if (variableName[0].contains(".inkEffect")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (id < inkAliases.count())
                                variableName[1] = inkAliases[id];
                    }

                    if (variableName[0].contains(".flag")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (id < faceFlagAliases.count())
                                variableName[1] = faceFlagAliases[id];
                    }

                    if (variableName[0].contains(".priority")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (id < priorityAliases.count())
                                variableName[1] = priorityAliases[id];
                    }

                    if (variableName[0].contains(".gravity")
                        && !variableName[0].contains(".gravityStrength")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (id < gravityAliases.count())
                                variableName[1] = gravityAliases[id];
                    }

                    if (variableName[0].contains(".collisionMode")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (id < cModeAliases.count())
                                variableName[1] = cModeAliases[id];
                    }

                    if (variableName[0].contains(".collisionPlane")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (id < cPathAliases.count())
                                variableName[1] = cPathAliases[id];
                    }

                    opID = operandSwitchList.indexOf(variableName[1]);
                    switch (opID) {
                        case 0: //"Engine.PlatformID":
                        {
                            bool ok = false;
                            int id  = variableName[2].toInt(&ok);
                            if (ok && id < platformAliases.count())
                                variableName[2] = platformAliases[id];
                            break;
                        }
                        case 1: //"Stage.ActiveList":
                        {
                            bool ok = false;
                            int id  = variableName[2].toInt(&ok);
                            if (ok && id < stagesAliases.count())
                                variableName[2] = stagesAliases[id];
                            break;
                        }
                        case 2: //"Stage.State":
                        {
                            bool ok = false;
                            int id  = variableName[2].toInt(&ok);
                            if (ok && id - 1 < stageStateAliases.count())
                                variableName[2] = stageStateAliases[id - 1];
                            break;
                        }
                        case 3: // Engine.State
                        {
                            bool ok = false;
                            int id  = variableName[2].toInt(&ok);
                            if (ok) {
                                if (id == 2)
                                    variableName[2] = engineStateAliases[0];
                            }
                            break;
                        }
                        case 4: // Engine.Message
                        {
                            // bool ok = false;
                            // int id  = variableName[2].toInt(&ok);
                            // if (ok) {
                            //    if (id < 0 || id > messageAliases.count())
                            //        variableName[2] = "MESSAGE_" + QString::number(id);
                            //    else
                            //        variableName[2] = messageAliases[id];
                            //}
                            break;
                        }
                    }

                    if (variableName[1].contains(".direction")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok && id < directionAliases.count())
                            variableName[2] = directionAliases[id];
                    }

                    if (variableName[1].contains(".inkEffect")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (id < inkAliases.count())
                                variableName[2] = inkAliases[id];
                        }
                    }

                    if (variableName[1].contains(".gravity")
                        && !variableName[1].contains(".gravityStrength")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (id < gravityAliases.count())
                                variableName[2] = gravityAliases[id];
                        }
                    }

                    if (variableName[1].contains(".flag")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (id < faceFlagAliases.count())
                                variableName[2] = faceFlagAliases[id];
                        }
                    }

                    if (variableName[1].contains(".priority")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (id < priorityAliases.count())
                                variableName[2] = priorityAliases[id];
                        }
                    }

                    if (variableName[1].contains(".collisionMode")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (id < cModeAliases.count())
                                variableName[2] = cModeAliases[id];
                        }
                    }

                    if (variableName[1].contains(".collisionPlane")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (id < cPathAliases.count())
                                variableName[2] = cPathAliases[id];
                        }
                    }
                }
            }

            switch (opcode) {
                case FUNC_END:
                    if (isFunction)
                        writer.writeText("end function");
                    else
                        writer.writeText("end event");
                    state.m_endFlag = true;
                    state.m_deep    = 0;
                    break;
                case FUNC_EQUAL:
                    writer.writeText(toHexString(variableName[0]) + " = "
                                     + toHexString(variableName[1]));
                    break;
                case FUNC_ADD:
                    writer.writeText(toHexString(variableName[0])
                                     + " += " + toHexString(variableName[1]));
                    break;
                case FUNC_SUB:
                    writer.writeText(toHexString(variableName[0])
                                     + " -= " + toHexString(variableName[1]));
                    break;
                case FUNC_INC: writer.writeText(toHexString(variableName[0]) + "++"); break;
                case FUNC_DEC: writer.writeText(toHexString(variableName[0]) + "--"); break;
                case FUNC_MUL:
                    writer.writeText(toHexString(variableName[0])
                                     + " *= " + toHexString(variableName[1]));
                    break;
                case FUNC_DIV:
                    writer.writeText(toHexString(variableName[0])
                                     + " /= " + toHexString(variableName[1]));
                    break;
                case FUNC_SHR:
                    writer.writeText(toHexString(variableName[0])
                                     + " >>= " + toHexString(variableName[1]));
                    break;
                case FUNC_SHL:
                    writer.writeText(toHexString(variableName[0])
                                     + " <<= " + toHexString(variableName[1]));
                    break;
                case FUNC_AND:
                    writer.writeText(toHexString(variableName[0])
                                     + " &= " + toHexString(variableName[1]));
                    break;
                case FUNC_OR:
                    writer.writeText(toHexString(variableName[0])
                                     + " |= " + toHexString(variableName[1]));
                    break;
                case FUNC_XOR:
                    writer.writeText(toHexString(variableName[0])
                                     + " ^= " + toHexString(variableName[1]));
                    break;
                case FUNC_MOD:
                    writer.writeText(toHexString(variableName[0])
                                     + " %= " + toHexString(variableName[1]));
                    break;
                case FUNC_IFEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + " == " + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_IFGREATER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1]) + " > "
                                     + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_IFGREATEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + " >= " + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_IFLOWER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1]) + " < "
                                     + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_IFLOWEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + " <= " + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_IFNOTEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + " != " + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_ELSE: writer.writeText("else"); break;
                case FUNC_ENDIF: writer.writeText("end if"); break;
                case FUNC_WEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + " == " + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_WGREATER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1]) + " > "
                                     + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_WGREATEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + " >= " + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_WLOWER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1]) + " < "
                                     + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_WLOWEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + " <= " + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_WNOTEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + " != " + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_LOOP: writer.writeText("loop"); break;
                case FUNC_FOREACHACTIVE: {
                    QString type = "TypeName[";
                    bool ok      = false;
                    int t        = variableName[1].toInt(&ok);
                    QString tn   = "";
                    if (ok) {
                        if (t < m_typeNames.count() && t >= 0) {
                            type = m_typeNames[t];
                            type = "TypeName[" + type + "]";
                        }
                        else {
                            if (t == 256)
                                type = "GROUP_PLAYERS";
                            else
                                type = QString::number(t);
                        }
                    }
                    else {
                        type = variableName[1];
                    }
                    writer.writeText(QString("foreach (%1, %2, ACTIVE_ENTITIES)")
                                         .arg(type)
                                         .arg(toHexString(variableName[2])));
                    ++state.m_deep;
                    break;
                }
                case FUNC_FOREACHALL: {
                    QString type = "TypeName[";
                    bool ok      = false;
                    int t        = variableName[1].toInt(&ok);
                    QString tn   = "";
                    if (ok) {
                        if (t < m_typeNames.count() && t >= 0) {
                            type = m_typeNames[t];
                            type = "TypeName[" + type + "]";
                        }
                        else {
                            type = QString::number(t);
                        }
                    }
                    else {
                        type = variableName[1];
                    }
                    writer.writeText(QString("foreach (%1, %2, ALL_ENTITIES)")
                                         .arg(type)
                                         .arg(toHexString(variableName[2])));
                    ++state.m_deep;
                    break;
                }
                case FUNC_NEXT: writer.writeText("next"); break;
                case FUNC_SWITCH: {
                    writer.writeText("switch " + toHexString(variableName[1]));

                    ++state.m_switchDeep;
                    ++state.m_deep;
                    SwitchState &sw = state.m_switchState[state.m_switchDeep];
                    bool ok         = false;
                    int jmpOffset   = variableName[0].toInt(&ok) + state.m_jumpTableOffset;
                    if (!ok) {
                        qDebug() << "uh oh";
                    }

                    sw.m_jumpTableOffset = jmpOffset;

                    int lowCase  = bc.m_jumpTableData[jmpOffset + 0];
                    int highCase = bc.m_jumpTableData[jmpOffset + 1];
                    int defJump  = bc.m_jumpTableData[jmpOffset + 2];
                    int endJmp   = bc.m_jumpTableData[jmpOffset + 3];

                    sw.m_lowCase    = lowCase;
                    sw.m_highCase   = highCase;
                    sw.m_defaultJmp = defJump;
                    sw.m_endJmp     = endJmp;
                    sw.m_case       = 0; // Case ID

                    sw.m_jumpPtr.clear();
                    int caseID = 0;

                    // Load Data for Switches
                    for (int c = lowCase; c <= highCase + 1; c++) {
                        if (c == highCase + 1) {                     // Check if default
                            if (sw.m_endJmp - sw.m_defaultJmp > 1) { // Default
                                int jumpPtr = state.m_scriptCodeOffset + sw.m_defaultJmp;
                                int match   = -1;

                                for (int i = 0; i < sw.m_jumpPtr.count(); ++i) {
                                    if (jumpPtr == sw.m_jumpPtr[i].m_jump) {
                                        match = i;
                                        break;
                                    }
                                }

                                if (match > -1) // fallthrough case
                                    sw.m_jumpPtr[match].m_cases.append(c);
                                else // new case
                                {
                                    sw.m_jumpPtr.append(SwitchJumpPtr());
                                    sw.m_jumpPtr[caseID].m_cases.append(c);

                                    sw.m_jumpPtr[caseID].m_jump = jumpPtr;
                                    ++caseID;
                                }
                            }
                        }
                        else {
                            int jumpTblPtr = jmpOffset + (c - lowCase) + 4;
                            int jumpPtr    = state.m_scriptCodeOffset + bc.m_jumpTableData[jumpTblPtr];
                            int match      = -1;

                            for (int i = 0; i < sw.m_jumpPtr.count(); ++i) {
                                if (jumpPtr == sw.m_jumpPtr[i].m_jump) {
                                    match = i;
                                    break;
                                }
                            }

                            if (match > -1) // fallthrough case
                                sw.m_jumpPtr[match].m_cases.append(c);
                            else // new case
                            {
                                sw.m_jumpPtr.append(SwitchJumpPtr());
                                sw.m_jumpPtr[caseID].m_cases.append(c);

                                sw.m_jumpPtr[caseID].m_jump = jumpPtr;
                                ++caseID;
                            }
                        }
                    }

                    // Sort Switches by JumpPtr
                    if (sw.m_jumpPtr.count() > 0) {
                        std::sort(sw.m_jumpPtr.begin(), sw.m_jumpPtr.end(),
                                  [](const SwitchJumpPtr &a, const SwitchJumpPtr &b) -> bool {
                                      return a.m_jump < b.m_jump;
                                  });
                    }
                    break;
                }
                case FUNC_BREAK: writer.writeText("break"); break;
                case FUNC_ENDSWITCH:
                    writer.writeText("end switch");
                    state.m_switchDeep--;
                    break;
                case FUNC_RETURN:
                    if (state.m_deep <= 1) {
                        writer.writeText("end function");
                        state.m_endFlag = true;
                        state.m_deep    = 0;
                    }
                    else {
                        writer.writeText("return");
                    }
                    break;
                default:
                    if (operand == "GetTableValue") {
                        bool ok   = false;
                        int index = variableName[2].toInt(&ok);
                        if (ok) {
                            StaticVarInfo c(
                                QString("%1_table%2").arg(curTypeNamev4).arg(m_tables.count()), index);

                            if (!m_tables.contains(c)) {
                                m_tables.append(c);
                                variableName[2] = c.m_name;
                            }
                            else {
                                variableName[2] = m_tables[m_tables.indexOf(c)].m_name;
                            }
                        }
                    }
                    if (operand == "SetTableValue") {
                        bool ok   = false;
                        int index = variableName[2].toInt(&ok);
                        if (ok) {
                            StaticVarInfo c(
                                QString("%1_table%2").arg(curTypeNamev4).arg(m_tables.count()), index);

                            if (!m_tables.contains(c)) {
                                m_tables.append(c);
                                variableName[2] = c.m_name;
                            }
                            else {
                                variableName[2] = m_tables[m_tables.indexOf(c)].m_name;
                            }
                        }

                        if (debugNameFlag) {
                            if (variableName[2] == "DebugMode_table12") {
                                bool ok = false;
                                int id  = variableName[0].toInt(&ok);
                                if (ok && id < m_typeNames.count())
                                    variableName[0] = "TypeName[" + m_typeNames[id] + "]";
                            }
                            if (variableName[2] == "DebugMode_table13") {
                                bool ok = false;
                                int id  = variableName[0].toInt(&ok);
                                if (ok) {
                                    if (debugNameFlag == 1 && id < m_functionNames.count()) {
                                        variableName[0] = curTypeNamev4 + "_DebugDraw";

                                        for (int i = 0; i < scriptPtrs.count(); ++i) {
                                            if (scriptPtrs[i].m_name == m_functionNames[id]) {
                                                scriptPtrs[i].m_name = curTypeNamev4 + "_DebugDraw";
                                            }
                                        }
                                    }
                                    else {
                                        variableName[0] = curTypeNamev4 + "_DebugDraw";
                                    }
                                }
                            }
                            if (variableName[2] == "DebugMode_table11") {
                                bool ok = false;
                                int id  = variableName[0].toInt(&ok);
                                if (ok) {
                                    if (debugNameFlag == 1 && id < m_functionNames.count()) {
                                        variableName[0] = curTypeNamev4 + "_DebugSpawn";

                                        for (int i = 0; i < scriptPtrs.count(); ++i) {
                                            if (scriptPtrs[i].m_name == m_functionNames[id]) {
                                                scriptPtrs[i].m_name = curTypeNamev4 + "_DebugSpawn";
                                            }
                                        }
                                    }
                                    else {
                                        variableName[0] = curTypeNamev4 + "_DebugSpawn";
                                    }
                                }
                            }
                        }
                    }

                    if (operand == "CallFunction") {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok) {
                            QString funcName = m_functionNames[id];

                            writer.writeText(QString("CallFunction(%1)").arg(funcName));
                        }
                        else {
                            writer.writeText(QString("CallFunction(%1)").arg(variableName[0]));
                        }
                    }
                    else {

                        writer.writeText(operand + "(");
                        for (int i = 0; i < paramsCount; i++) {
                            writer.writeText(toHexString(variableName[i]));
                            if (i + 1 < paramsCount)
                                writer.writeText(", ");
                        }
                        writer.writeText(")");
                    }
                    break;
            }
        }
        writer.writeLine("", LINE_CRLF);
    }
}
