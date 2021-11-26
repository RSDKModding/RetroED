#include "includes.hpp"

namespace RSDKv3
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
    "TempValue0", // 0x00
    "TempValue1",
    "TempValue2",
    "TempValue3",
    "TempValue4",
    "TempValue5",
    "TempValue6",
    "TempValue7",
    "CheckResult", // 0x08
    "ArrayPos0",   // 0x09
    "ArrayPos1",
    "Global", // 0x0B
    "Object.EntityNo",
    "Object.Type",
    "Object.PropertyValue", // 0x0E
    "Object.XPos",
    "Object.YPos", // 0x10
    "Object.iXPos",
    "Object.iYPos",
    "Object.State",
    "Object.Rotation",
    "Object.Scale",
    "Object.Priority",
    "Object.DrawOrder",
    "Object.Direction",
    "Object.InkEffect", // None,Blend,Alpha,Add,Sub
    "Object.Alpha",
    "Object.Frame",
    "Object.Animation",
    "Object.PrevAnimation",
    "Object.AnimationSpeed",
    "Object.AnimationTimer",
    "Object.Value0", // 0x20
    "Object.Value1",
    "Object.Value2", // 0x22
    "Object.Value3",
    "Object.Value4",
    "Object.Value5",
    "Object.Value6",
    "Object.Value7",
    "Object.OutOfBounds",
    "Player.State",
    "Player.ControlMode",
    "Player.ControlLock",
    "Player.CollisionMode",
    "Player.CollisionPlane",
    "Player.XPos",       // 0x2E
    "Player.YPos",       // 0x2F
    "Player.iXPos",      // 0x30
    "Player.iYPos",      // 0x31
    "Player.ScreenXPos", // 0x32
    "Player.ScreenYPos", // 0x33
    "Player.Speed",      // 0x34
    "Player.XVelocity",  // 0x35
    "Player.YVelocity",  // 0x36
    "Player.Gravity",    // 0x37
    "Player.Angle",
    "Player.Skidding",
    "Player.Pushing",
    "Player.TrackScroll",
    "Player.Up", // 0x3C
    "Player.Down",
    "Player.Left",
    "Player.Right",
    "Player.JumpPress", // 0x40
    "Player.JumpHold",
    "Player.FollowPlayer1",
    "Player.LookPos",
    "Player.Water",
    "Player.TopSpeed",
    "Player.Acceleration", // 0x46
    "Player.Deceleration",
    "Player.AirAcceleration",
    "Player.AirDeceleration",
    "Player.GravityStrength",
    "Player.JumpStrength",
    "Player.JumpCap",
    "Player.RollingAcceleration",
    "Player.RollingDeceleration",
    "Player.EntityNo",
    "Player.CollisionLeft", // 0x50
    "Player.CollisionTop",
    "Player.CollisionRight",
    "Player.CollisionBottom",
    "Player.Flailing",
    "Player.Timer",
    "Player.TileCollisions",
    "Player.ObjectInteraction",
    "Player.Visible",
    "Player.Rotation",
    "Player.Scale",
    "Player.Priority",
    "Player.DrawOrder", // 0x5C
    "Player.Direction",
    "Player.InkEffect",
    "Player.Alpha",
    "Player.Frame", // 0x60
    "Player.Animation",
    "Player.PrevAnimation",
    "Player.AnimationSpeed",
    "Player.AnimationTimer",
    "Player.Value0",
    "Player.Value1",
    "Player.Value2",
    "Player.Value3",
    "Player.Value4",
    "Player.Value5",
    "Player.Value6",
    "Player.Value7",
    "Player.Value8",
    "Player.Value9",
    "Player.Value10",
    "Player.Value11",
    "Player.Value12",
    "Player.Value13",
    "Player.Value14",
    "Player.Value15",
    "Player.OutOfBounds",
    "Stage.State",
    "Stage.ActiveList",
    "Stage.ListPos", // 0x78
    "Stage.TimeEnabled",
    "Stage.MilliSeconds", // 0x7A
    "Stage.Seconds",
    "Stage.Minutes", // 0x7C
    "Stage.ActNo",
    "Stage.PauseEnabled", // 0x7E
    "Stage.ListSize",
    "Stage.NewXBoundary1", // 0x80
    "Stage.NewXBoundary2",
    "Stage.NewYBoundary1", // 0x82
    "Stage.NewYBoundary2",
    "Stage.XBoundary1", // 0x84
    "Stage.XBoundary2",
    "Stage.YBoundary1", // 0x86
    "Stage.YBoundary2",
    "Stage.DeformationData0", // 0x88
    "Stage.DeformationData1",
    "Stage.DeformationData2",
    "Stage.DeformationData3",
    "Stage.WaterLevel", // 0x8C
    "Stage.ActiveLayer",
    "Stage.MidPoint",      // 0x8E
    "Stage.PlayerListPos", // 0x8F
    "Stage.ActivePlayer",  // 0x90
    "Screen.CameraEnabled",
    "Screen.CameraTarget",
    "Screen.CameraStyle", // 0x93
    "Screen.DrawListSize",
    "Screen.CenterX",
    "Screen.CenterY", // 0x96
    "Screen.XSize",
    "Screen.YSize", // 0x98
    "Screen.XOffset",
    "Screen.YOffset",
    "Screen.ShakeX",        // 0x9B
    "Screen.ShakeY",        // 0x9C
    "Screen.AdjustCameraY", // 0x9D
    "TouchScreen.Down",
    "TouchScreen.XPos",
    "TouchScreen.YPos",   // 0xA0
    "Music.Volume",       // 0xA1
    "Music.CurrentTrack", // 0xA2
    "KeyDown.Up",
    "KeyDown.Down",
    "KeyDown.Left",
    "KeyDown.Right",
    "KeyDown.ButtonA",
    "KeyDown.ButtonB", // 0xA8
    "KeyDown.ButtonC",
    "KeyDown.Start",
    "KeyPress.Up",
    "KeyPress.Down",
    "KeyPress.Left",
    "KeyPress.Right",
    "KeyPress.ButtonA",
    "KeyPress.ButtonB", // 0xB0
    "KeyPress.ButtonC",
    "KeyPress.Start",
    "Menu1.Selection",
    "Menu2.Selection",
    "TileLayer.XSize",
    "TileLayer.YSize",
    "TileLayer.Type",
    "TileLayer.Angle", // 0xB8
    "TileLayer.XPos",
    "TileLayer.YPos",
    "TileLayer.ZPos",
    "TileLayer.ParallaxFactor",
    "TileLayer.ScrollSpeed",
    "TileLayer.ScrollPos",
    "TileLayer.DeformationOffset",
    "TileLayer.DeformationOffsetW", // 0xC0
    "HParallax.ParallaxFactor",
    "HParallax.ScrollSpeed",
    "HParallax.ScrollPos",
    "VParallax.ParallaxFactor",
    "VParallax.ScrollSpeed",
    "VParallax.ScrollPos",
    "3DScene.NoVertices",
    "3DScene.NoFaces", // 0xC8
    "VertexBuffer.x",
    "VertexBuffer.y",
    "VertexBuffer.z",
    "VertexBuffer.u",
    "VertexBuffer.v",
    "FaceBuffer.a",
    "FaceBuffer.b",
    "FaceBuffer.c", // 0xD0
    "FaceBuffer.d",
    "FaceBuffer.Flag",
    "FaceBuffer.Color",
    "3DScene.ProjectionX",
    "3DScene.ProjectionY",     // 0xD5
    "Engine.State",            // 0xD6
    "Stage.DebugMode",         // 0xD7
    "Engine.Message",          // 0xD8
    "SaveRAM",                 // 0xD9
    "Engine.Language",         // 0xDA
    "Object.SpriteSheet",      // 0xDB
    "Engine.OnlineActive",     // 0xDC
    "Engine.FrameSkipTimer",   // 0xDD
    "Engine.FrameSkipSetting", // 0xDE
    "Engine.SFXVolume",        // 0xDF
    "Engine.BGMVolume",        // 0xE0
    "Engine.PlatformID",       // 0xE1
    "Engine.TrialMode",        // 0xE2
    "KeyPress.AnyStart",       // 0xE3
    "Engine.HapticsEnabled",
};

enum ScrVariable {
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
    VAR_GLOBAL,
    VAR_OBJECTENTITYNO,
    VAR_OBJECTTYPE,
    VAR_OBJECTPROPERTYVALUE,
    VAR_OBJECTXPOS,
    VAR_OBJECTYPOS,
    VAR_OBJECTIXPOS,
    VAR_OBJECTIYPOS,
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
    VAR_OBJECTVALUE0,
    VAR_OBJECTVALUE1,
    VAR_OBJECTVALUE2,
    VAR_OBJECTVALUE3,
    VAR_OBJECTVALUE4,
    VAR_OBJECTVALUE5,
    VAR_OBJECTVALUE6,
    VAR_OBJECTVALUE7,
    VAR_OBJECTOUTOFBOUNDS,
    VAR_PLAYERSTATE,
    VAR_PLAYERCONTROLMODE,
    VAR_PLAYERCONTROLLOCK,
    VAR_PLAYERCOLLISIONMODE,
    VAR_PLAYERCOLLISIONPLANE,
    VAR_PLAYERXPOS,
    VAR_PLAYERYPOS,
    VAR_PLAYERIXPOS,
    VAR_PLAYERIYPOS,
    VAR_PLAYERSCREENXPOS,
    VAR_PLAYERSCREENYPOS,
    VAR_PLAYERSPEED,
    VAR_PLAYERXVELOCITY,
    VAR_PLAYERYVELOCITY,
    VAR_PLAYERGRAVITY,
    VAR_PLAYERANGLE,
    VAR_PLAYERSKIDDING,
    VAR_PLAYERPUSHING,
    VAR_PLAYERTRACKSCROLL,
    VAR_PLAYERUP,
    VAR_PLAYERDOWN,
    VAR_PLAYERLEFT,
    VAR_PLAYERRIGHT,
    VAR_PLAYERJUMPPRESS,
    VAR_PLAYERJUMPHOLD,
    VAR_PLAYERFOLLOWPLAYER1,
    VAR_PLAYERLOOKPOS,
    VAR_PLAYERWATER,
    VAR_PLAYERTOPSPEED,
    VAR_PLAYERACCELERATION,
    VAR_PLAYERDECELERATION,
    VAR_PLAYERAIRACCELERATION,
    VAR_PLAYERAIRDECELERATION,
    VAR_PLAYERGRAVITYSTRENGTH,
    VAR_PLAYERJUMPSTRENGTH,
    VAR_PLAYERJUMPCAP,
    VAR_PLAYERROLLINGACCELERATION,
    VAR_PLAYERROLLINGDECELERATION,
    VAR_PLAYERENTITYNO,
    VAR_PLAYERCOLLISIONLEFT,
    VAR_PLAYERCOLLISIONTOP,
    VAR_PLAYERCOLLISIONRIGHT,
    VAR_PLAYERCOLLISIONBOTTOM,
    VAR_PLAYERFLAILING,
    VAR_PLAYERTIMER,
    VAR_PLAYERTILECOLLISIONS,
    VAR_PLAYEROBJECTINTERACTION,
    VAR_PLAYERVISIBLE,
    VAR_PLAYERROTATION,
    VAR_PLAYERSCALE,
    VAR_PLAYERPRIORITY,
    VAR_PLAYERDRAWORDER,
    VAR_PLAYERDIRECTION,
    VAR_PLAYERINKEFFECT,
    VAR_PLAYERALPHA,
    VAR_PLAYERFRAME,
    VAR_PLAYERANIMATION,
    VAR_PLAYERPREVANIMATION,
    VAR_PLAYERANIMATIONSPEED,
    VAR_PLAYERANIMATIONTIMER,
    VAR_PLAYERVALUE0,
    VAR_PLAYERVALUE1,
    VAR_PLAYERVALUE2,
    VAR_PLAYERVALUE3,
    VAR_PLAYERVALUE4,
    VAR_PLAYERVALUE5,
    VAR_PLAYERVALUE6,
    VAR_PLAYERVALUE7,
    VAR_PLAYERVALUE8,
    VAR_PLAYERVALUE9,
    VAR_PLAYERVALUE10,
    VAR_PLAYERVALUE11,
    VAR_PLAYERVALUE12,
    VAR_PLAYERVALUE13,
    VAR_PLAYERVALUE14,
    VAR_PLAYERVALUE15,
    VAR_PLAYEROUTOFBOUNDS,
    VAR_STAGESTATE,
    VAR_STAGEACTIVELIST,
    VAR_STAGELISTPOS,
    VAR_STAGETIMEENABLED,
    VAR_STAGEMILLISECONDS,
    VAR_STAGESECONDS,
    VAR_STAGEMINUTES,
    VAR_STAGEACTNO,
    VAR_STAGEPAUSEENABLED,
    VAR_STAGELISTSIZE,
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
    VAR_STAGEACTIVEPLAYER,
    VAR_SCREENCAMERAENABLED,
    VAR_SCREENCAMERATARGET,
    VAR_SCREENCAMERASTYLE,
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
    VAR_KEYDOWNUP,
    VAR_KEYDOWNDOWN,
    VAR_KEYDOWNLEFT,
    VAR_KEYDOWNRIGHT,
    VAR_KEYDOWNBUTTONA,
    VAR_KEYDOWNBUTTONB,
    VAR_KEYDOWNBUTTONC,
    VAR_KEYDOWNSTART,
    VAR_KEYPRESSUP,
    VAR_KEYPRESSDOWN,
    VAR_KEYPRESSLEFT,
    VAR_KEYPRESSRIGHT,
    VAR_KEYPRESSBUTTONA,
    VAR_KEYPRESSBUTTONB,
    VAR_KEYPRESSBUTTONC,
    VAR_KEYPRESSSTART,
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
    VAR_3DSCENEPROJECTIONX,
    VAR_3DSCENEPROJECTIONY,
    VAR_ENGINESTATE,
    VAR_STAGEDEBUGMODE,
    VAR_ENGINEMESSAGE,
    VAR_SAVERAM,
    VAR_ENGINELANGUAGE,
    VAR_OBJECTSPRITESHEET,
    VAR_ENGINEONLINEACTIVE,
    VAR_ENGINEFRAMESKIPTIMER,
    VAR_ENGINEFRAMESKIPSETTING,
    VAR_ENGINESFXVOLUME,
    VAR_ENGINEBGMVOLUME,
    VAR_ENGINEPLATFORMID,
    VAR_ENGINETRIALMODE,
    VAR_KEYPRESSANYSTART,
    VAR_ENGINEHAPTICSENABLED,
    VAR_MAX_CNT
};

enum ScrFunction {
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
    FUNC_SWITCH,
    FUNC_BREAK,
    FUNC_ENDSWITCH,
    FUNC_RAND,
    FUNC_SIN,
    FUNC_COS,
    FUNC_SIN256,
    FUNC_COS256,
    FUNC_SINCHANGE,
    FUNC_COSCHANGE,
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
    FUNC_BINDPLAYERTOOBJECT,
    FUNC_PLAYERTILECOLLISION,
    FUNC_PROCESSPLAYERCONTROL,
    FUNC_PROCESSANIMATION,
    FUNC_DRAWOBJECTANIMATION,
    FUNC_DRAWPLAYERANIMATION,
    FUNC_SETMUSICTRACK,
    FUNC_PLAYMUSIC,
    FUNC_STOPMUSIC,
    FUNC_PLAYSFX,
    FUNC_STOPSFX,
    FUNC_SETSFXATTRIBUTES,
    FUNC_OBJECTTILECOLLISION,
    FUNC_OBJECTTILEGRIP,
    FUNC_LOADVIDEO,
    FUNC_NEXTVIDEOFRAME,
    FUNC_PLAYSTAGESFX,
    FUNC_STOPSTAGESFX,
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
    FUNC_TRANSFORMVERTICES,
    FUNC_CALLFUNCTION,
    FUNC_ENDFUNCTION,
    FUNC_SETLAYERDEFORMATION,
    FUNC_CHECKTOUCHRECT,
    FUNC_GETTILELAYERENTRY,
    FUNC_SETTILELAYERENTRY,
    FUNC_GETBIT,
    FUNC_SETBIT,
    FUNC_PAUSEMUSIC,
    FUNC_RESUMEMUSIC,
    FUNC_CLEARDRAWLIST,
    FUNC_ADDDRAWLISTENTITYREF,
    FUNC_GETDRAWLISTENTITYREF,
    FUNC_SETDRAWLISTENTITYREF,
    FUNC_GET16X16TILEINFO,
    FUNC_COPY16X16TILE,
    FUNC_SET16X16TILEINFO,
    FUNC_GETANIMATIONBYNAME,
    FUNC_READSAVERAM,
    FUNC_WRITESAVERAM,
    FUNC_LOADTEXTFONT,
    FUNC_LOADTEXTFILE,
    FUNC_DRAWTEXT,
    FUNC_GETTEXTINFO,
    FUNC_GETVERSIONNUMBER,
    FUNC_SETACHIEVEMENT,
    FUNC_SETLEADERBOARD,
    FUNC_LOADONLINEMENU,
    FUNC_ENGINECALLBACK,
    FUNC_HAPTICEFFECT,
    FUNC_MAX_CNT
};

static QList<QString> functionNames = { "End",
                                        "Equal",
                                        "Add",
                                        "Sub",
                                        "Inc",
                                        "Dec",
                                        "Mul",
                                        "Div",
                                        "ShR",
                                        "ShL",
                                        "And",
                                        "Or",
                                        "Xor",
                                        "Mod",
                                        "FlipSign",
                                        "CheckEqual",
                                        "CheckGreater",
                                        "CheckLower",
                                        "CheckNotEqual",
                                        "IfEqual",
                                        "IfGreater",
                                        "IfGreaterOrEqual",
                                        "IfLower",
                                        "IfLowerOrEqual",
                                        "IfNotEqual",
                                        "else",
                                        "endif",
                                        "WEqual",
                                        "WGreater",
                                        "WGreaterOrEqual",
                                        "WLower",
                                        "WLowerOrEqual",
                                        "WNotEqual",
                                        "loop",
                                        "switch",
                                        "break",
                                        "endswitch",
                                        "Rand",
                                        "Sin",
                                        "Cos",
                                        "Sin256",
                                        "Cos256",
                                        "SinChange",
                                        "CosChange",
                                        "ATan2",
                                        "Interpolate",
                                        "InterpolateXY",
                                        "LoadSpriteSheet",
                                        "RemoveSpriteSheet",
                                        "DrawSprite",
                                        "DrawSpriteXY",
                                        "DrawSpriteScreenXY",
                                        "DrawTintRect",
                                        "DrawNumbers",
                                        "DrawActName",
                                        "DrawMenu",
                                        "SpriteFrame",
                                        "SetEditorIcon",
                                        "LoadPalette",
                                        "RotatePalette",
                                        "SetScreenFade",
                                        "SetActivePalette",
                                        "SetPaletteFade",
                                        "CopyPalette",
                                        "ClearScreen",
                                        "DrawSpriteFX",
                                        "DrawSpriteScreenFX",
                                        "LoadAnimation",
                                        "SetupMenu",
                                        "AddMenuEntry",
                                        "EditMenuEntry",
                                        "LoadStage",
                                        "DrawRect",
                                        "ResetObjectEntity",
                                        "PlayerObjectCollision",
                                        "CreateTempObject",
                                        "BindPlayerToObject",
                                        "PlayerTileCollision",
                                        "ProcessPlayerControl",
                                        "ProcessAnimation",
                                        "DrawObjectAnimation",
                                        "DrawPlayerAnimation",
                                        "SetMusicTrack",
                                        "PlayMusic",
                                        "StopMusic",
                                        "PlaySfx",
                                        "StopSfx",
                                        "SetSfxAttributes",
                                        "ObjectTileCollision",
                                        "ObjectTileGrip",
                                        "LoadVideo",
                                        "NextVideoFrame",
                                        "PlayStageSfx",
                                        "StopStageSfx",
                                        "Not",
                                        "Draw3DScene",
                                        "SetIdentityMatrix",
                                        "MatrixMultiply",
                                        "MatrixTranslateXYZ",
                                        "MatrixScaleXYZ",
                                        "MatrixRotateX",
                                        "MatrixRotateY",
                                        "MatrixRotateZ",
                                        "MatrixRotateXYZ",
                                        "TransformVertices",
                                        "CallFunction",
                                        "EndFunction",
                                        "SetLayerDeformation",
                                        "CheckTouchRect",
                                        "GetTileLayerEntry",
                                        "SetTileLayerEntry",
                                        "GetBit",
                                        "SetBit",
                                        "PauseMusic",
                                        "ResumeMusic",
                                        "ClearDrawList",
                                        "AddDrawListEntityRef",
                                        "GetDrawListEntityRef",
                                        "SetDrawListEntityRef",
                                        "Get16x16TileInfo",
                                        "Copy16x16Tile",
                                        "Set16x16TileInfo",
                                        "GetAnimationByName",
                                        "ReadSaveRAM",
                                        "WriteSaveRAM",
                                        "LoadTextFont",
                                        "LoadTextFile",
                                        "DrawText",
                                        "GetTextInfo",
                                        "GetVersionNumber",
                                        "SetAchievement",
                                        "SetLeaderboard",
                                        "LoadOnlineMenu",
                                        "EngineCallback",
                                        "HapticEffect" };

static byte scriptOpcodeSizes[] = {
    0, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 0, 0, 3,
    3, 3, 3, 3, 3, 0, 2, 0, 0, 2, 2, 2, 2, 2, 5, 5, 3, 4, 7, 1, 1, 1, 3, 3, 4, 7, 7, 3,
    6, 7, 5, 3, 4, 3, 7, 2, 1, 4, 4, 1, 4, 3, 4, 0, 8, 5, 5, 4, 2, 0, 0, 0, 0, 0, 3, 1,
    0, 2, 1, 3, 4, 4, 1, 0, 2, 1, 1, 0, 1, 2, 4, 4, 2, 2, 2, 4, 3, 1, 0, 6, 4, 4, 4, 3,
    3, 0, 0, 1, 2, 3, 3, 4, 2, 4, 2, 0, 0, 1, 3, 7, 5, 2, 2, 2, 1, 1, 4, 0,
};

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
    "C_PLATFORM",
};

static QList<QString> collision2Aliases = {
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
    "FACING_RIGHT",
    "FACING_LEFT",
};

static QList<QString> direction2Aliases = {
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
    "RETRO_WIN", "RETRO_OSX", "RETRO_XBOX_360", "RETRO_PS3", "RETRO_IOS", "RETRO_ANDROID", "RETRO_WP7",
};

// Custom aliases (wont compile, but makes reading easier)

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
    "FACE_TEXTURED_3D",
    "FACE_TEXTURED_2D",
    "FACE_COLOURED_3D",
    "FACE_COLOURED_2D",
};

static QList<QString> priorityAliases = {
    "PRIORITY_ACTIVE_BOUNDS",   "PRIORITY_ACTIVE",   "PRIORITY_ACTIVE_PAUSED", "PRIORITY_XBOUNDS",
    "PRIORITY_XBOUNDS_DESTROY", "PRIORITY_INACTIVE", "PRIORITY_BOUNDS_SMALL",  "PRIORITY_XBOUNDS_SMALL",
};

static QList<QString> callbackAliases = {
    "CALLBACK_DISPLAYLOGOS",
    "CALLBACK_PRESS_START",
    "CALLBACK_TIMEATTACK_NOTIFY_ENTER",
    "CALLBACK_TIMEATTACK_NOTIFY_EXIT",
    "CALLBACK_FINISHGAME_NOTIFY",
    "CALLBACK_RETURNSTORE_SELECTED",
    "CALLBACK_RESTART_SELECTED",
    "CALLBACK_EXIT_SELECTED",
    "CALLBACK_BUY_FULL_GAME_SELECTED",
    "CALLBACK_TERMS_SELECTED",
    "CALLBACK_PRIVACY_SELECTED",
    "CALLBACK_TRIAL_ENDED",
    "CALLBACK_SETTINGS_SELECTED",
    "CALLBACK_PAUSE_REQUESTED",
    "CALLBACK_FULL_VERSION_ONLY",
    "CALLBACK_STAFF_CREDITS",
    "CALLBACK_16",
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

struct HapInfo {
    QString m_name = "";
    int m_val      = 0;
    HapInfo() {}
    HapInfo(QString n, int v) { m_name = n, m_val = v; }
};

static QList<HapInfo> hapticEffects = {
    HapInfo("HAPTIC_ALERT1", 63),
    HapInfo("HAPTIC_ALERT10", 72),
    HapInfo("HAPTIC_ALERT2", 64),
    HapInfo("HAPTIC_ALERT3", 65),
    HapInfo("HAPTIC_ALERT4", 66),
    HapInfo("HAPTIC_ALERT5", 67),
    HapInfo("HAPTIC_ALERT6", 68),
    HapInfo("HAPTIC_ALERT7", 69),
    HapInfo("HAPTIC_ALERT8", 70),
    HapInfo("HAPTIC_ALERT9", 71),
    HapInfo("HAPTIC_BOUNCE_100", 9),
    HapInfo("HAPTIC_BOUNCE_33", 11),
    HapInfo("HAPTIC_BOUNCE_66", 10),
    HapInfo("HAPTIC_BUMP_100", 6),
    HapInfo("HAPTIC_BUMP_33", 8),
    HapInfo("HAPTIC_BUMP_66", 7),
    HapInfo("HAPTIC_DOUBLE_BUMP_100", 18),
    HapInfo("HAPTIC_DOUBLE_BUMP_33", 20),
    HapInfo("HAPTIC_DOUBLE_BUMP_66", 19),
    HapInfo("HAPTIC_DOUBLE_SHARP_CLICK_100", 12),
    HapInfo("HAPTIC_DOUBLE_SHARP_CLICK_33", 14),
    HapInfo("HAPTIC_DOUBLE_SHARP_CLICK_66", 13),
    HapInfo("HAPTIC_DOUBLE_STRONG_CLICK_100", 15),
    HapInfo("HAPTIC_DOUBLE_STRONG_CLICK_33", 17),
    HapInfo("HAPTIC_DOUBLE_STRONG_CLICK_66", 16),
    HapInfo("HAPTIC_ENGINE1_100", 112),
    HapInfo("HAPTIC_ENGINE1_33", 114),
    HapInfo("HAPTIC_ENGINE1_66", 113),
    HapInfo("HAPTIC_ENGINE2_100", 115),
    HapInfo("HAPTIC_ENGINE2_33", 117),
    HapInfo("HAPTIC_ENGINE2_66", 116),
    HapInfo("HAPTIC_ENGINE3_100", 118),
    HapInfo("HAPTIC_ENGINE3_33", 120),
    HapInfo("HAPTIC_ENGINE3_66", 119),
    HapInfo("HAPTIC_ENGINE4_100", 121),
    HapInfo("HAPTIC_ENGINE4_33", 123),
    HapInfo("HAPTIC_ENGINE4_66", 122),
    HapInfo("HAPTIC_EXPLOSION1", 73),
    HapInfo("HAPTIC_EXPLOSION10", 82),
    HapInfo("HAPTIC_EXPLOSION2", 74),
    HapInfo("HAPTIC_EXPLOSION3", 75),
    HapInfo("HAPTIC_EXPLOSION4", 76),
    HapInfo("HAPTIC_EXPLOSION5", 77),
    HapInfo("HAPTIC_EXPLOSION6", 78),
    HapInfo("HAPTIC_EXPLOSION7", 79),
    HapInfo("HAPTIC_EXPLOSION8", 80),
    HapInfo("HAPTIC_EXPLOSION9", 81),
    HapInfo("HAPTIC_FAST_PULSE_100", 45),
    HapInfo("HAPTIC_FAST_PULSE_33", 47),
    HapInfo("HAPTIC_FAST_PULSE_66", 46),
    HapInfo("HAPTIC_FAST_PULSING_100", 48),
    HapInfo("HAPTIC_FAST_PULSING_33", 50),
    HapInfo("HAPTIC_FAST_PULSING_66", 49),
    HapInfo("HAPTIC_IMPACT_METAL_100", 96),
    HapInfo("HAPTIC_IMPACT_METAL_33", 98),
    HapInfo("HAPTIC_IMPACT_METAL_66", 97),
    HapInfo("HAPTIC_IMPACT_RUBBER_100", 99),
    HapInfo("HAPTIC_IMPACT_RUBBER_33", 101),
    HapInfo("HAPTIC_IMPACT_RUBBER_66", 100),
    HapInfo("HAPTIC_IMPACT_WOOD_100", 93),
    HapInfo("HAPTIC_IMPACT_WOOD_33", 95),
    HapInfo("HAPTIC_IMPACT_WOOD_66", 94),
    HapInfo("HAPTIC_LONG_BUZZ_100", 27),
    HapInfo("HAPTIC_LONG_BUZZ_33", 29),
    HapInfo("HAPTIC_LONG_BUZZ_66", 28),
    HapInfo("HAPTIC_LONG_TRANSITION_RAMP_DOWN_100", 39),
    HapInfo("HAPTIC_LONG_TRANSITION_RAMP_DOWN_33", 41),
    HapInfo("HAPTIC_LONG_TRANSITION_RAMP_DOWN_66", 40),
    HapInfo("HAPTIC_LONG_TRANSITION_RAMP_UP_100", 33),
    HapInfo("HAPTIC_LONG_TRANSITION_RAMP_UP_33", 35),
    HapInfo("HAPTIC_LONG_TRANSITION_RAMP_UP_66", 34),
    HapInfo("HAPTIC_SHARP_CLICK_100", 0),
    HapInfo("HAPTIC_SHARP_CLICK_33", 2),
    HapInfo("HAPTIC_SHARP_CLICK_66", 1),
    HapInfo("HAPTIC_SHORT_BUZZ_100", 30),
    HapInfo("HAPTIC_SHORT_BUZZ_33", 32),
    HapInfo("HAPTIC_SHORT_BUZZ_66", 31),
    HapInfo("HAPTIC_SHORT_TRANSITION_RAMP_DOWN_100", 42),
    HapInfo("HAPTIC_SHORT_TRANSITION_RAMP_DOWN_33", 44),
    HapInfo("HAPTIC_SHORT_TRANSITION_RAMP_DOWN_66", 43),
    HapInfo("HAPTIC_SHORT_TRANSITION_RAMP_UP_100", 36),
    HapInfo("HAPTIC_SHORT_TRANSITION_RAMP_UP_33", 38),
    HapInfo("HAPTIC_SHORT_TRANSITION_RAMP_UP_66", 37),
    HapInfo("HAPTIC_SLOW_PULSE_100", 51),
    HapInfo("HAPTIC_SLOW_PULSE_33", 53),
    HapInfo("HAPTIC_SLOW_PULSE_66", 52),
    HapInfo("HAPTIC_SLOW_PULSING_100", 54),
    HapInfo("HAPTIC_SLOW_PULSING_33", 56),
    HapInfo("HAPTIC_SLOW_PULSING_66", 55),
    HapInfo("HAPTIC_STRONG_CLICK_100", 3),
    HapInfo("HAPTIC_STRONG_CLICK_33", 5),
    HapInfo("HAPTIC_STRONG_CLICK_66", 4),
    HapInfo("HAPTIC_TEXTURE1", 102),
    HapInfo("HAPTIC_TEXTURE10", 111),
    HapInfo("HAPTIC_TEXTURE2", 103),
    HapInfo("HAPTIC_TEXTURE3", 104),
    HapInfo("HAPTIC_TEXTURE4", 105),
    HapInfo("HAPTIC_TEXTURE5", 106),
    HapInfo("HAPTIC_TEXTURE6", 107),
    HapInfo("HAPTIC_TEXTURE7", 108),
    HapInfo("HAPTIC_TEXTURE8", 109),
    HapInfo("HAPTIC_TEXTURE9", 110),
    HapInfo("HAPTIC_TICK_100", 24),
    HapInfo("HAPTIC_TICK_33", 26),
    HapInfo("HAPTIC_TICK_66", 25),
    HapInfo("HAPTIC_TRANSITION_BOUNCE_100", 60),
    HapInfo("HAPTIC_TRANSITION_BOUNCE_33", 62),
    HapInfo("HAPTIC_TRANSITION_BOUNCE_66", 61),
    HapInfo("HAPTIC_TRANSITION_BUMP_100", 57),
    HapInfo("HAPTIC_TRANSITION_BUMP_33", 59),
    HapInfo("HAPTIC_TRANSITION_BUMP_66", 58),
    HapInfo("HAPTIC_TRIPLE_STRONG_CLICK_100", 21),
    HapInfo("HAPTIC_TRIPLE_STRONG_CLICK_33", 23),
    HapInfo("HAPTIC_TRIPLE_STRONG_CLICK_66", 22),
    HapInfo("HAPTIC_WEAPON1", 83),
    HapInfo("HAPTIC_WEAPON10", 92),
    HapInfo("HAPTIC_WEAPON2", 84),
    HapInfo("HAPTIC_WEAPON3", 85),
    HapInfo("HAPTIC_WEAPON4", 86),
    HapInfo("HAPTIC_WEAPON5", 87),
    HapInfo("HAPTIC_WEAPON6", 88),
    HapInfo("HAPTIC_WEAPON7", 89),
    HapInfo("HAPTIC_WEAPON8", 90),
    HapInfo("HAPTIC_WEAPON9", 91),
}; // namespace RSDKv3

static QList<QString> tempValue = {
    "TempValue0", "TempValue1", "TempValue2", "TempValue3",
    "TempValue4", "TempValue5", "TempValue6", "TempValue7",
};
static QList<QString> arrayPosition = {
    "ArrayPos0",
    "ArrayPos1",
    "TempObjectPos",
};
}; // namespace RSDKv3

void RSDKv3::Decompiler::clearScriptData()
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

QString RSDKv3::Decompiler::setArrayValue(QString strIn, QString index)
{
    QString strOut = strIn;
    int point      = -1;

    if (strIn == "Global") {
        strOut = variableNames[index.toInt()];
        if (strOut == "")
            return strIn;
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

class ScriptPtr
{
public:
    QString m_name  = "";
    int m_startPtr  = 0;
    int m_jumpStart = 0;
    bool m_function = false;
    ScriptPtr() {}
    ScriptPtr(QString n, int s, int j, bool f)
    {
        m_name      = n;
        m_startPtr  = s;
        m_jumpStart = j;
        m_function  = f;
    }
};

int funcIDv3 = 0;
void RSDKv3::Decompiler::decompile(RSDKv3::Bytecode bytecode, QString destPath)
{
    clearScriptData();

    funcIDv3 = globalFunctionCount; // what function to use
    for (int i = globalScriptCount; i < sourceNames.count(); ++i) {
        RSDKv3::Bytecode::ObjectScript &objectScript = bytecode.scriptList[i];
        int scriptCodePtrs[4]    = { objectScript.mainScript, objectScript.playerScript,
                                  objectScript.drawScript, objectScript.startupScript };
        int lowestScriptCodePtr  = 0x3FFFF;
        int highestScriptCodePtr = 0;

        for (int i = 0; i < 4; ++i) {
            if (scriptCodePtrs[i] < 0x3FFFF && scriptCodePtrs[i] > highestScriptCodePtr)
                highestScriptCodePtr = scriptCodePtrs[i];
        }

        for (int i = 0; i < 4; ++i) {
            if (scriptCodePtrs[i] < 0x3FFFF && scriptCodePtrs[i] < lowestScriptCodePtr)
                lowestScriptCodePtr = scriptCodePtrs[i];
        }

        for (int f = funcIDv3; f < bytecode.functionList.count(); ++f) {
            auto &func = bytecode.functionList[f];
            if (func.mainScript < lowestScriptCodePtr && lowestScriptCodePtr < 0x3FFFF) {
                QString tn = typeNames[i];
                functionNames.append(tn.replace(" ", "") + "_Function" + QString::number(f));
                funcIDv3++;
            }
        }
    }

    for (int i = globalScriptCount; i < sourceNames.count(); ++i) {
        // check to give proper names
    }

    funcIDv3 = globalFunctionCount; // what function to use
    for (int i = globalScriptCount; i < sourceNames.count(); ++i) {
        if (i == 0)
            continue;
        QString path       = "";
        QString scriptPath = sourceNames[i];
        QString baseDir    = destPath + "/Scripts/";
        if (useCustomAliases || seperateFolders) {
            baseDir = destPath + "/Scripts/" + QFileInfo(bytecode.filePath).baseName() + "/";
        }
        QString dir = baseDir + scriptPath.replace(QFileInfo(scriptPath).fileName(), "");
        scriptPath  = sourceNames[i];

        if (!QDir(QDir::tempPath()).exists(dir)) {
            QDir(QDir::tempPath()).mkpath(dir);
        }

        // qDebug() << baseDir << scriptPath;
        path = baseDir + scriptPath;

        if (QFile::exists(path))
            QFile::remove(path);
        Writer writer(path);

        RSDKv3::Bytecode::ObjectScript &objectScript = bytecode.scriptList[i];

        int scriptCodePtrs[4]    = { objectScript.mainScript, objectScript.playerScript,
                                  objectScript.drawScript, objectScript.startupScript };
        int lowestScriptCodePtr  = 0x3FFFF;
        int highestScriptCodePtr = 0;

        for (int i = 0; i < 4; ++i) {
            if (scriptCodePtrs[i] < 0x3FFFF && scriptCodePtrs[i] > highestScriptCodePtr)
                highestScriptCodePtr = scriptCodePtrs[i];
        }

        for (int i = 0; i < 4; ++i) {
            if (scriptCodePtrs[i] < 0x3FFFF && scriptCodePtrs[i] < lowestScriptCodePtr)
                lowestScriptCodePtr = scriptCodePtrs[i];
        }

        writer.writeLine("//------------Sonic CD " + typeNames[i] + " Script-------------//",
                         LINE_CRLF);
        writer.writeLine("//--------Scripted by Christian Whitehead 'The Taxman'--------//", LINE_CRLF);
        writer.writeLine("//-------Unpacked By Rubberduckycooly's Script Unpacker-------//", LINE_CRLF);

        writer.writeLine("", LINE_CRLF);

        writer.writeLine("//-------Aliases-------//", LINE_CRLF);

        writer.writeLine("#alias " + QString::number(i) + ": TYPE_"
                             + typeNames[i].toUpper().replace(" ", ""),
                         LINE_CRLF);

        writer.writeLine("", LINE_CRLF);

        QList<ScriptPtr> scriptPtrs;

        if (objectScript.mainScript < 0x3FFFF)
            scriptPtrs.append(
                ScriptPtr("ObjectMain", objectScript.mainScript, objectScript.mainJumpTable, false));

        if (objectScript.playerScript < 0x3FFFF)
            scriptPtrs.append(ScriptPtr("ObjectPlayerInteraction", objectScript.playerScript,
                                        objectScript.playerJumpTable, false));

        if (objectScript.drawScript < 0x3FFFF)
            scriptPtrs.append(
                ScriptPtr("ObjectDraw", objectScript.drawScript, objectScript.drawJumpTable, false));

        if (objectScript.startupScript < 0x3FFFF)
            scriptPtrs.append(ScriptPtr("ObjectStartup", objectScript.startupScript,
                                        objectScript.startupJumpTable, false));
        std::sort(
            scriptPtrs.begin(), scriptPtrs.end(),
            [](const ScriptPtr &a, const ScriptPtr &b) -> bool { return a.m_startPtr < b.m_startPtr; });

        int lastPtr = scriptPtrs.count() > 0 ? scriptPtrs[scriptPtrs.count() - 1].m_startPtr : 0x3FFFF;
        scriptPtrs.clear(); // temp, used to sort

        bool flag = false;
        for (int f = funcIDv3; f < bytecode.functionList.count(); ++f) {
            int fs = bytecode.functionList[f].mainScript;
            if (fs < lastPtr) {
                scriptPtrs.append(ScriptPtr(functionNames[f], bytecode.functionList[f].mainScript,
                                            bytecode.functionList[f].mainJumpTable, true));
            }
            else if (!flag) {
                flag     = true;
                funcIDv3 = f;
            }
        }
        if (!flag) {
            funcIDv3 = bytecode.functionList.count();
        }

        if (objectScript.mainScript < 0x3FFFF)
            scriptPtrs.append(
                ScriptPtr("ObjectMain", objectScript.mainScript, objectScript.mainJumpTable, false));

        if (objectScript.playerScript < 0x3FFFF)
            scriptPtrs.append(ScriptPtr("ObjectPlayerInteraction", objectScript.playerScript,
                                        objectScript.playerJumpTable, false));

        if (objectScript.drawScript < 0x3FFFF)
            scriptPtrs.append(
                ScriptPtr("ObjectDraw", objectScript.drawScript, objectScript.drawJumpTable, false));

        if (objectScript.startupScript < 0x3FFFF)
            scriptPtrs.append(ScriptPtr("ObjectStartup", objectScript.startupScript,
                                        objectScript.startupJumpTable, false));

        // Causes issues with player.state function calling
        std::sort(
            scriptPtrs.begin(), scriptPtrs.end(),
            [](const ScriptPtr &a, const ScriptPtr &b) -> bool { return a.m_startPtr < b.m_startPtr; });

        int fCnt = 0;
        for (int s = 0; s < scriptPtrs.count(); ++s) {
            if (scriptPtrs[s].m_name != "ObjectMain"
                && scriptPtrs[s].m_name != "ObjectPlayerInteraction"
                && scriptPtrs[s].m_name != "ObjectDraw" && scriptPtrs[s].m_name != "ObjectStartup") {
                fCnt++;
            }
        }

        if (fCnt)
            writer.writeLine("// Function declarations", LINE_CRLF);

        for (int s = 0; s < scriptPtrs.count(); ++s) {
            if (scriptPtrs[s].m_name != "ObjectMain"
                && scriptPtrs[s].m_name != "ObjectPlayerInteraction"
                && scriptPtrs[s].m_name != "ObjectDraw" && scriptPtrs[s].m_name != "ObjectStartup") {
                writer.writeLine("#function " + scriptPtrs[s].m_name, LINE_CRLF);
            }
        }

        for (int s = 0; s < scriptPtrs.count(); ++s) {
            writer.writeLine("", LINE_CRLF);
            decompileScript(bytecode, writer, scriptPtrs[s].m_startPtr, scriptPtrs[s].m_jumpStart,
                            scriptPtrs[s].m_name, scriptPtrs[s].m_function);
        }

        writer.writeLine("sub RSDK", LINE_CRLF);
        writer.writeLine(QString("\tLoadSpriteSheet(") + "\"Global/Display.gif\"" + ")", LINE_CRLF);
        writer.writeLine("\tSetEditorIcon(Icon0,SingleIcon,-16,-16,32,32,1,143)", LINE_CRLF);
        writer.writeLine("endsub", LINE_CRLF);

        writer.flush();
    }
}

void RSDKv3::Decompiler::decompileScript(RSDKv3::Bytecode &bytecode, Writer &writer, int scriptCodePtr,
                                         int jumpTablePtr, QString subName, bool isFunction)
{
    QList<QString> funcNames = { "ObjectMain", "ObjectPlayerInteraction", "ObjectDraw", "ObjectStartup",
                                 "RSDK" };

    if (!isFunction)
        writer.writeLine("sub " + subName, LINE_CRLF);
    else
        writer.writeLine("function " + subName, LINE_CRLF);

    StateScriptEngine state;
    state.m_scriptCodePtr = state.m_scriptCodeOffset = scriptCodePtr;
    state.m_jumpTablePtr = state.m_jumpTableOffset = jumpTablePtr;
    state.m_deep                                   = 1;
    state.m_switchDeep                             = -1;

    decompileSub(bytecode, writer, state, isFunction);
    writer.writeLine("", LINE_CRLF);
}

void RSDKv3::Decompiler::decompileSub(RSDKv3::Bytecode &bytecode, Writer writer,
                                      RSDKv3::Decompiler::StateScriptEngine &state, bool isFunction)
{
    state.m_endFlag      = false;
    RSDKv3::Bytecode &bc = bytecode;

    while (!state.m_endFlag) {

        if (state.m_switchDeep >= 0) {
            SwitchState &sw = state.m_switchState[state.m_switchDeep];
            for (int j = 0; j < sw.m_jumpPtr.count(); ++j) {
                if (sw.m_jumpPtr[j].m_jump == state.m_scriptCodePtr) {
                    if (sw.m_jumpPtr[j].m_jump != sw.m_endJmp + state.m_scriptCodeOffset - 1
                        && sw.m_jumpPtr[j].m_jump != sw.m_endJmp + state.m_scriptCodeOffset
                        && sw.m_jumpPtr[j].m_jump != sw.m_defaultJmp + state.m_scriptCodeOffset) {
                        for (auto &c : sw.m_jumpPtr[j].m_cases) {
                            for (int i = 0; i < state.m_deep - 1; ++i) writer.writeText("\t");
                            writer.writeLine("case " + QString::number(c), LINE_CRLF);
                        }
                    }
                }
            }

            if (sw.m_defaultJmp + state.m_scriptCodeOffset == state.m_scriptCodePtr) {
                if (sw.m_defaultJmp != sw.m_endJmp - 1) {
                    for (int i = 0; i < state.m_deep - 1; ++i) writer.writeText("\t");
                    writer.writeLine("default", LINE_CRLF);
                }
            }
        }

        int opcode      = bc.scriptData[state.m_scriptCodePtr++];
        int paramsCount = scriptOpcodeSizes[opcode];

        QList<QString> variableName;

        for (int i = 0; i < 0x10; ++i) variableName.append("UNKNOWN VARIABLE");

        for (int i = 0; i < paramsCount; ++i) {
            int paramId = bc.scriptData[state.m_scriptCodePtr++];
            switch (paramId) {
                case 0: // Unused
                    break;
                case 1: // Read value from RSDK
                    switch (bc.scriptData[state.m_scriptCodePtr++]) {
                        case 0: // Read Const Variable
                            variableName[i] = variableNames[bc.scriptData[state.m_scriptCodePtr++]];
                            break;
                        case 1:                                              // ARRAY
                            if (bc.scriptData[state.m_scriptCodePtr++] == 1) // Variable
                            {
                                QString value   = arrayPosition[bc.scriptData[state.m_scriptCodePtr++]];
                                variableName[i] = setArrayValue(
                                    variableNames[bc.scriptData[state.m_scriptCodePtr++]], value);
                            }
                            else // Value
                            {
                                QString value = QString::number(bc.scriptData[state.m_scriptCodePtr++]);
                                variableName[i] = setArrayValue(
                                    variableNames[bc.scriptData[state.m_scriptCodePtr++]], value);
                            }
                            break;
                        case 2:
                            // ObjectLoop +
                            if (bc.scriptData[state.m_scriptCodePtr++] == 1) {
                                QString value =
                                    "+" + arrayPosition[bc.scriptData[state.m_scriptCodePtr++]];
                                variableName[i] = setArrayValue(
                                    variableNames[bc.scriptData[state.m_scriptCodePtr++]], value);
                            }
                            else {
                                QString value =
                                    "+" + QString::number(bc.scriptData[state.m_scriptCodePtr++]);
                                variableName[i] = setArrayValue(
                                    variableNames[bc.scriptData[state.m_scriptCodePtr++]], value);
                            }
                            break;
                        case 3:
                            // ObjectLoop -
                            if (bc.scriptData[state.m_scriptCodePtr++] == 1) {
                                QString value =
                                    "-" + arrayPosition[bc.scriptData[state.m_scriptCodePtr++]];
                                variableName[i] = setArrayValue(
                                    variableNames[bc.scriptData[state.m_scriptCodePtr++]], value);
                            }
                            else {
                                QString value =
                                    "-" + QString::number(bc.scriptData[state.m_scriptCodePtr++]);
                                variableName[i] = setArrayValue(
                                    variableNames[bc.scriptData[state.m_scriptCodePtr++]], value);
                            }
                            break;
                    }
                    break;
                case 2: // Read constant value from bytecode
                    variableName[i] = QString::number(bc.scriptData[state.m_scriptCodePtr++]);
                    break;
                case 3: // Read string
                    QString tmp = "";
                    int strLen  = bc.scriptData[state.m_scriptCodePtr];
                    for (int j = 0; j < strLen;) {
                        state.m_scriptCodePtr++;
                        if (j < strLen) {
                            int val = bc.scriptData[state.m_scriptCodePtr] >> 24;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;

                        if (j < strLen) {
                            int val = (bc.scriptData[state.m_scriptCodePtr] & 0x00FFFFFF) >> 16;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;

                        if (j < strLen) {
                            int val = (bc.scriptData[state.m_scriptCodePtr] & 0x0000FFFF) >> 8;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;

                        if (j < strLen) {
                            int val = bc.scriptData[state.m_scriptCodePtr] & 0x000000FF;
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

        QString operand = functionNames[opcode];

        if (operand == "End" || operand == "EndFunction") {
            if (isFunction)
                writer.writeText("endfunction");
            else
                writer.writeText("endsub");
            state.m_endFlag = true;
            state.m_deep    = 0;
        }

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
                    state.m_deep--;
                    for (int i = 0; i < state.m_deep; ++i) writer.writeText("\t");
                    break;
                case FUNC_ENDSWITCH:
                    state.m_deep--;
                    for (int i = 0; i < state.m_deep; ++i) writer.writeText("\t");
                    break;
                default:
                    for (int i = 0; i < state.m_deep; ++i) writer.writeText("\t");
                    break;
            }

            if (opcode >= functionNames.count()) {
                writer.writeText("ERROR AT: " + QString::number(state.m_scriptCodePtr) + " : "
                                 + opcode);
                printLog("OPCODE ABOVE THE MAX OPCODES");
                return;
            }

            for (int i = 0; i < variableName.length(); ++i) {
                if (variableName[i] == "") {
                    variableName[i] = "Object.Value0";
                }
            }

            // aliases
            {
                if (variableName[0].contains("Object") && variableName[0].contains(".Type")) {
                    bool ok = false;
                    int id  = variableName[1].toInt(&ok);
                    if (ok && id < typeNames.count())
                        variableName[1] =
                            "TypeName["
                            + typeNames[id].replace(" ", "").replace("TouchControls", "DebugMode")
                            + "]";
                }

                if (variableName[0].contains("TileLayer") && variableName[0].contains(".Type")) {
                    bool ok = false;
                    int id  = variableName[1].toInt(&ok);
                    if (ok && useCustomAliases && id < tileLayerTypeAliases.count())
                        variableName[1] = "TypeName[" + tileLayerTypeAliases[id] + "]";
                }

                if (variableName[0].contains("_Type")) {
                    bool ok = false;
                    int id  = variableName[1].toInt(&ok);
                    if (ok && id < typeNames.count())
                        variableName[1] =
                            "TypeName["
                            + typeNames[id].replace(" ", "").replace("TouchControls", "DebugMode")
                            + "]";
                }

                if (variableName[1].contains("Object") && variableName[1].contains(".Type")) {
                    bool ok = false;
                    int id  = variableName[2].toInt(&ok);
                    if (ok && id < typeNames.count())
                        variableName[2] =
                            "TypeName["
                            + typeNames[id].replace(" ", "").replace("TouchControls", "DebugMode")
                            + "]";
                }

                if (variableName[1].contains("TileLayer") && variableName[1].contains(".Type")) {
                    bool ok = false;
                    int id  = variableName[2].toInt(&ok);
                    if (ok && useCustomAliases && id < tileLayerTypeAliases.count())
                        variableName[2] = "TypeName[" + tileLayerTypeAliases[id] + "]";
                }

                // Special Aliases for some functions
                QList<QString> operandSwitchList = {
                    "DrawSpriteFX",     "DrawSpriteScreenFX",  "PlayerObjectCollision",
                    "CreateTempObject", "ResetObjectEntity",   "PlaySfx",
                    "StopSfx",          "PlayStageSfx",        "StopStageSfx",
                    "SetSfxAttributes", "ObjectTileCollision", "ObjectTileGrip",
                    "MatrixMultiply",   "SetIdentityMatrix",   "MatrixTranslateXYZ",
                    "MatrixScaleXYZ",   "MatrixRotateX",       "MatrixRotateY",
                    "MatrixRotateZ",    "MatrixRotateXYZ",     "TransformVertices",
                    "DrawMenu",         "SetupMenu",           "AddMenuEntry",
                    "EditMenuEntry",    "LoadTextFile",        "DrawText",
                    "GetVersionNumber", "GetTextInfo",         "LoadOnlineMenu",
                    "Get16x16TileInfo", "Set16x16TileInfo",    "EngineCallback",
                    "HapticEffect",
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
                    case 2: //"PlayerObjectCollision"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok) {
                            if (!m_mobileVer && id < collisionAliases.count())
                                variableName[0] = collisionAliases[id];
                            else if (m_mobileVer && id < collision2Aliases.count())
                                variableName[0] = collision2Aliases[id];
                        }
                        break;
                    }
                    case 3: //"CreateTempObject"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok && id < typeNames.count())
                            variableName[0] =
                                "TypeName["
                                + typeNames[id].replace(" ", "").replace("TouchControls", "DebugMode")
                                + "]";
                        break;
                    }
                    case 4: //"ResetObjectEntity"
                    {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok && id < typeNames.count())
                            variableName[1] =
                                "TypeName["
                                + typeNames[id].replace(" ", "").replace("TouchControls", "DebugMode")
                                + "]";
                        break;
                    }
                    case 5: //"PlaySfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < sfxNames.count())
                                variableName[0] = "SFXName[" + sfxNames[id].replace(" ", "") + "]";
                        break;
                    }
                    case 6: //"StopSfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < sfxNames.count())
                                variableName[0] = "SFXName[" + sfxNames[id].replace(" ", "") + "]";
                        break;
                    }
                    case 7: //"PlayStageSfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id + globalSFXCount < sfxNames.count())
                                variableName[0] =
                                    "SFXName[" + sfxNames[id + globalSFXCount].replace(" ", "") + "]";
                        break;
                    }
                    case 8: //"StopStageSfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id + globalSFXCount < sfxNames.count())
                                variableName[0] =
                                    "SFXName[" + sfxNames[id + globalSFXCount].replace(" ", "") + "]";
                        break;
                    }
                    case 9: //"SetSfxAttributes"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < sfxNames.count())
                                variableName[0] = "SFXName[" + sfxNames[id].replace(" ", "") + "]";
                        break;
                    }
                    case 10: //"ObjectTileCollision"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < cSideAliases.count())
                                variableName[0] = cSideAliases[id];
                        break;
                    }
                    case 11: //"ObjectTileGrip"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < cSideAliases.count())
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
                        if (useCustomAliases) {
                            ok = false;
                            id = variableName[2].toInt(&ok);
                            if (ok && id < textInfoAliases.count())
                                variableName[2] = textInfoAliases[id];
                        }
                        break;
                    }
                    case 29: //"LoadOnlineMenu"
                    {
                        if (useCustomAliases) {
                            bool ok = false;
                            int id  = variableName[0].toInt(&ok);
                            if (ok && id < onlineMenuAliases.count())
                                variableName[0] = onlineMenuAliases[id];
                        }
                        break;
                    }
                    case 30: //"Get16x16TileInfo"
                    case 31: //"Set16x16TileInfo"
                    {
                        if (useCustomAliases) {
                            bool ok = false;
                            int id  = variableName[3].toInt(&ok);
                            if (ok && id < tileInfoAliases.count())
                                variableName[3] = tileInfoAliases[id];
                        }
                        break;
                    }
                    case 32: //"EngineCallback"
                    {
                        if (useCustomAliases) {
                            bool ok = false;
                            int id  = variableName[0].toInt(&ok);
                            if (ok) {
                                if (id < 0 || id >= callbackAliases.count())
                                    variableName[0] = "CALLBACK_" + QString::number(id);
                                else
                                    variableName[0] = callbackAliases[id];
                            }
                        }
                    } break;
                    case 33: //"HapticEffect"
                    {
                        if (useCustomAliases) {
                            bool ok = false;
                            int id  = variableName[0].toInt(&ok);
                            if (ok) {
                                if (id == -1)
                                    variableName[0] = "HAPTIC_STOP";
                                else if (id >= 0 && id < hapticEffects.count()) {
                                    for (int h = 0; h < hapticEffects.count(); ++h) {
                                        if (hapticEffects[h].m_val == id) {
                                            variableName[0] = hapticEffects[h].m_name;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                }

                if (opcode < FUNC_LOOP) {
                    operandSwitchList = { "Engine.PlatformID", "Stage.ActiveList", "Stage.State",
                                          "Engine.State", "Engine.Message" };
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

                    if (variableName[0].contains(".Direction")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok) {
                            if (!useCustomAliases && id < 2)
                                variableName[1] = directionAliases[id];
                            else if (useCustomAliases && id < direction2Aliases.count()) {
                                variableName[1] = direction2Aliases[id];
                            }
                        }
                    }

                    if (variableName[0].contains(".InkEffect")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < inkAliases.count())
                                variableName[1] = inkAliases[id];
                    }

                    if (variableName[0].contains(".Flag")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < faceFlagAliases.count())
                                variableName[1] = faceFlagAliases[id];
                    }

                    if (variableName[0].contains(".Priority")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < priorityAliases.count())
                                variableName[1] = priorityAliases[id];
                    }

                    if (variableName[0].contains(".Gravity")
                        && !variableName[0].contains(".GravityStrength")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < gravityAliases.count())
                                variableName[1] = gravityAliases[id];
                    }

                    if (variableName[0].contains(".CollisionMode")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < cModeAliases.count())
                                variableName[1] = cModeAliases[id];
                    }

                    if (variableName[0].contains(".CollisionPlane")) {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok)
                            if (useCustomAliases && id < cPathAliases.count())
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

                    if (variableName[1].contains(".Direction")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (!useCustomAliases) {
                            if (ok && id < directionAliases.count())
                                variableName[2] = directionAliases[id];
                        }
                        else {
                            if (ok && id < direction2Aliases.count())
                                variableName[2] = direction2Aliases[id];
                        }
                    }

                    if (variableName[1].contains(".InkEffect")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (useCustomAliases && id < inkAliases.count())
                                variableName[2] = inkAliases[id];
                        }
                    }

                    if (variableName[1].contains(".Gravity")
                        && !variableName[1].contains(".GravityStrength")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (useCustomAliases && id < gravityAliases.count())
                                variableName[2] = gravityAliases[id];
                        }
                    }

                    if (variableName[1].contains(".Flag")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (useCustomAliases && id < faceFlagAliases.count())
                                variableName[2] = faceFlagAliases[id];
                        }
                    }

                    if (variableName[1].contains(".Priority")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (useCustomAliases && id < priorityAliases.count())
                                variableName[2] = priorityAliases[id];
                        }
                    }

                    if (variableName[1].contains(".CollisionMode")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (useCustomAliases && id < cModeAliases.count())
                                variableName[2] = cModeAliases[id];
                        }
                    }

                    if (variableName[1].contains(".CollisionPlane")) {
                        bool ok = false;
                        int id  = variableName[2].toInt(&ok);
                        if (ok) {
                            if (useCustomAliases && id < cPathAliases.count())
                                variableName[2] = cPathAliases[id];
                        }
                    }
                }
            }

            switch (opcode) {
                case FUNC_END:
                    if (isFunction)
                        writer.writeText("endfunction");
                    else
                        writer.writeText("endsub");
                    state.m_endFlag = true;
                    state.m_deep    = 0;
                    break;
                case FUNC_EQUAL:
                    writer.writeText(toHexString(variableName[0]) + "=" + toHexString(variableName[1]));
                    break;
                case FUNC_ADD:
                    writer.writeText(toHexString(variableName[0])
                                     + "+=" + toHexString(variableName[1]));
                    break;
                case FUNC_SUB:
                    writer.writeText(toHexString(variableName[0])
                                     + "-=" + toHexString(variableName[1]));
                    break;
                case FUNC_INC: writer.writeText(toHexString(variableName[0]) + "++"); break;
                case FUNC_DEC: writer.writeText(toHexString(variableName[0]) + "--"); break;
                case FUNC_MUL:
                    writer.writeText(toHexString(variableName[0])
                                     + "*=" + toHexString(variableName[1]));
                    break;
                case FUNC_DIV:
                    writer.writeText(toHexString(variableName[0])
                                     + "/=" + toHexString(variableName[1]));
                    break;
                case FUNC_SHR:
                    writer.writeText(toHexString(variableName[0])
                                     + ">>=" + toHexString(variableName[1]));
                    break;
                case FUNC_SHL:
                    writer.writeText(toHexString(variableName[0])
                                     + "<<=" + toHexString(variableName[1]));
                    break;
                case FUNC_AND:
                    writer.writeText(toHexString(variableName[0])
                                     + "&=" + toHexString(variableName[1]));
                    break;
                case FUNC_OR:
                    writer.writeText(toHexString(variableName[0])
                                     + "|=" + toHexString(variableName[1]));
                    break;
                case FUNC_XOR:
                    writer.writeText(toHexString(variableName[0])
                                     + "^=" + toHexString(variableName[1]));
                    break;
                case FUNC_MOD:
                    writer.writeText(toHexString(variableName[0])
                                     + "%=" + toHexString(variableName[1]));
                    break;
                case FUNC_IFEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + "==" + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_IFGREATER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1]) + ">"
                                     + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_IFGREATEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + ">=" + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_IFLOWER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1]) + "<"
                                     + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_IFLOWEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + "<=" + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_IFNOTEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + "!=" + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_ELSE: writer.writeText("else"); break;
                case FUNC_ENDIF: writer.writeText("endif"); break;
                case FUNC_WEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + "==" + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_WGREATER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1]) + ">"
                                     + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_WGREATEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + ">=" + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_WLOWER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1]) + "<"
                                     + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_WLOWEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + "<=" + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_WNOTEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + "!=" + toHexString(variableName[2]));
                    ++state.m_deep;
                    break;
                case FUNC_LOOP: writer.writeText("loop"); break;
                case FUNC_SWITCH: {
                    writer.writeText("switch " + toHexString(variableName[1]));

                    ++state.m_switchDeep;
                    ++state.m_deep;
                    SwitchState &sw = state.m_switchState[state.m_switchDeep];
                    bool ok         = false;
                    int jmpOffset   = variableName[0].toInt(&ok) + state.m_jumpTableOffset;
                    if (!ok) {
                        printLog("uh oh");
                    }

                    sw.m_jumpTableOffset = jmpOffset;

                    int lowCase  = bc.jumpTableData[jmpOffset + 0];
                    int highCase = bc.jumpTableData[jmpOffset + 1];
                    int defJump  = bc.jumpTableData[jmpOffset + 2];
                    int endJmp   = bc.jumpTableData[jmpOffset + 3];

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
                            int jumpPtr    = state.m_scriptCodeOffset + bc.jumpTableData[jumpTblPtr];
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
                    writer.writeText("endswitch");
                    state.m_switchDeep--;
                    break;
                default:
                    if (operand == "CallFunction") {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok) {
                            QString funcName = functionNames[id];

                            writer.writeText(QString("CallFunction(%1)").arg(funcName));
                        }
                        else {
                            writer.writeText(QString("CallFunction(%1)").arg(variableName[0]));
                        }
                    }
                    else {
                        if (operand == "SetEditorIcon" && m_mobileVer)
                            writer.writeText("EditFrame(");
                        else
                            writer.writeText(operand + "(");
                        for (int i = 0; i < paramsCount; i++) {
                            writer.writeText(toHexString(variableName[i]));
                            if (i + 1 < paramsCount)
                                writer.writeText(",");
                        }
                        writer.writeText(")");
                    }
                    break;
            }
        }
        writer.writeLine("", LINE_CRLF);
    }
}
