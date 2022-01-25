#include "includes.hpp"

#define ALIAS_COUNT       (COMMONALIAS_COUNT + 0x60)
#define COMMONALIAS_COUNT (0x22)

struct AliasInfo {
    AliasInfo()
    {
        name  = "";
        value = "";
    }
    AliasInfo(QString aliasName, QString aliasVal)
    {
        name  = aliasName;
        value = aliasVal;
    }

    QString name  = "";
    QString value = "";
};

struct FunctionInfo {
    FunctionInfo()
    {
        name       = "";
        opcodeSize = 0;
    }
    FunctionInfo(QString functionName, int opSize)
    {
        name       = functionName;
        opcodeSize = opSize;
    }

    QString name   = "";
    int opcodeSize = 0;
};

const QString variableNames[] = {
    "TempValue0",
    "TempValue1",
    "TempValue2",
    "TempValue3",
    "TempValue4",
    "TempValue5",
    "TempValue6",
    "TempValue7",
    "CheckResult",
    "ArrayPos0",
    "ArrayPos1",
    "Global",
    "Object.EntityNo",
    "Object.Type",
    "Object.PropertyValue",
    "Object.XPos",
    "Object.YPos",
    "Object.iXPos",
    "Object.iYPos",
    "Object.State",
    "Object.Rotation",
    "Object.Scale",
    "Object.Priority",
    "Object.DrawOrder",
    "Object.Direction",
    "Object.InkEffect",
    "Object.Alpha",
    "Object.Frame",
    "Object.Animation",
    "Object.PrevAnimation",
    "Object.AnimationSpeed",
    "Object.AnimationTimer",
    "Object.Value0",
    "Object.Value1",
    "Object.Value2",
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
    "Player.XPos",
    "Player.YPos",
    "Player.iXPos",
    "Player.iYPos",
    "Player.ScreenXPos",
    "Player.ScreenYPos",
    "Player.Speed",
    "Player.XVelocity",
    "Player.YVelocity",
    "Player.Gravity",
    "Player.Angle",
    "Player.Skidding",
    "Player.Pushing",
    "Player.TrackScroll",
    "Player.Up",
    "Player.Down",
    "Player.Left",
    "Player.Right",
    "Player.JumpPress",
    "Player.JumpHold",
    "Player.FollowPlayer1",
    "Player.LookPos",
    "Player.Water",
    "Player.TopSpeed",
    "Player.Acceleration",
    "Player.Deceleration",
    "Player.AirAcceleration",
    "Player.AirDeceleration",
    "Player.GravityStrength",
    "Player.JumpStrength",
    "Player.JumpCap",
    "Player.RollingAcceleration",
    "Player.RollingDeceleration",
    "Player.EntityNo",
    "Player.CollisionLeft",
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
    "Player.DrawOrder",
    "Player.Direction",
    "Player.InkEffect",
    "Player.Alpha",
    "Player.Frame",
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
    "Stage.ListPos",
    "Stage.TimeEnabled",
    "Stage.MilliSeconds",
    "Stage.Seconds",
    "Stage.Minutes",
    "Stage.ActNo",
    "Stage.PauseEnabled",
    "Stage.ListSize",
    "Stage.NewXBoundary1",
    "Stage.NewXBoundary2",
    "Stage.NewYBoundary1",
    "Stage.NewYBoundary2",
    "Stage.XBoundary1",
    "Stage.XBoundary2",
    "Stage.YBoundary1",
    "Stage.YBoundary2",
    "Stage.DeformationData0",
    "Stage.DeformationData1",
    "Stage.DeformationData2",
    "Stage.DeformationData3",
    "Stage.WaterLevel",
    "Stage.ActiveLayer",
    "Stage.MidPoint",
    "Stage.PlayerListPos",
    "Stage.ActivePlayer",
    "Screen.CameraEnabled",
    "Screen.CameraTarget",
    "Screen.CameraStyle",
    "Screen.DrawListSize",
    "Screen.CenterX",
    "Screen.CenterY",
    "Screen.XSize",
    "Screen.YSize",
    "Screen.XOffset",
    "Screen.YOffset",
    "Screen.ShakeX",
    "Screen.ShakeY",
    "Screen.AdjustCameraY",
    "TouchScreen.Down",
    "TouchScreen.XPos",
    "TouchScreen.YPos",
    "Music.Volume",
    "Music.CurrentTrack",
    "KeyDown.Up",
    "KeyDown.Down",
    "KeyDown.Left",
    "KeyDown.Right",
    "KeyDown.ButtonA",
    "KeyDown.ButtonB",
    "KeyDown.ButtonC",
    "KeyDown.Start",
    "KeyPress.Up",
    "KeyPress.Down",
    "KeyPress.Left",
    "KeyPress.Right",
    "KeyPress.ButtonA",
    "KeyPress.ButtonB",
    "KeyPress.ButtonC",
    "KeyPress.Start",
    "Menu1.Selection",
    "Menu2.Selection",
    "TileLayer.XSize",
    "TileLayer.YSize",
    "TileLayer.Type",
    "TileLayer.Angle",
    "TileLayer.XPos",
    "TileLayer.YPos",
    "TileLayer.ZPos",
    "TileLayer.ParallaxFactor",
    "TileLayer.ScrollSpeed",
    "TileLayer.ScrollPos",
    "TileLayer.DeformationOffset",
    "TileLayer.DeformationOffsetW",
    "HParallax.ParallaxFactor",
    "HParallax.ScrollSpeed",
    "HParallax.ScrollPos",
    "VParallax.ParallaxFactor",
    "VParallax.ScrollSpeed",
    "VParallax.ScrollPos",
    "3DScene.NoVertices",
    "3DScene.NoFaces",
    "VertexBuffer.x",
    "VertexBuffer.y",
    "VertexBuffer.z",
    "VertexBuffer.u",
    "VertexBuffer.v",
    "FaceBuffer.a",
    "FaceBuffer.b",
    "FaceBuffer.c",
    "FaceBuffer.d",
    "FaceBuffer.Flag",
    "FaceBuffer.Color",
    "3DScene.ProjectionX",
    "3DScene.ProjectionY",
    "Engine.State",
    "Stage.DebugMode",
    "Engine.Message",
    "SaveRAM",
    "Engine.Language",
    "Object.SpriteSheet",
    "Engine.OnlineActive",
    "Engine.FrameSkipTimer",
    "Engine.FrameSkipSetting",
    "Engine.SFXVolume",
    "Engine.BGMVolume",
    "Engine.PlatformID",
    "Engine.TrialMode",
    "KeyPress.AnyStart",
    "Engine.HapticsEnabled",
    // EDITOR ONLY
    "Editor.VariableID",
    "Editor.VariableValue",
    "Editor.ReturnVariable",
};

const FunctionInfo functions[] = {
    FunctionInfo("End", 0),
    FunctionInfo("Equal", 2),
    FunctionInfo("Add", 2),
    FunctionInfo("Sub", 2),
    FunctionInfo("Inc", 1),
    FunctionInfo("Dec", 1),
    FunctionInfo("Mul", 2),
    FunctionInfo("Div", 2),
    FunctionInfo("ShR", 2),
    FunctionInfo("ShL", 2),
    FunctionInfo("And", 2),
    FunctionInfo("Or", 2),
    FunctionInfo("Xor", 2),
    FunctionInfo("Mod", 2),
    FunctionInfo("FlipSign", 1),
    FunctionInfo("CheckEqual", 2),
    FunctionInfo("CheckGreater", 2),
    FunctionInfo("CheckLower", 2),
    FunctionInfo("CheckNotEqual", 2),
    FunctionInfo("IfEqual", 3),
    FunctionInfo("IfGreater", 3),
    FunctionInfo("IfGreaterOrEqual", 3),
    FunctionInfo("IfLower", 3),
    FunctionInfo("IfLowerOrEqual", 3),
    FunctionInfo("IfNotEqual", 3),
    FunctionInfo("else", 0),
    FunctionInfo("endif", 0),
    FunctionInfo("WEqual", 3),
    FunctionInfo("WGreater", 3),
    FunctionInfo("WGreaterOrEqual", 3),
    FunctionInfo("WLower", 3),
    FunctionInfo("WLowerOrEqual", 3),
    FunctionInfo("WNotEqual", 3),
    FunctionInfo("loop", 0),
    FunctionInfo("switch", 2),
    FunctionInfo("break", 0),
    FunctionInfo("endswitch", 0),
    FunctionInfo("Rand", 2),
    FunctionInfo("Sin", 2),
    FunctionInfo("Cos", 2),
    FunctionInfo("Sin256", 2),
    FunctionInfo("Cos256", 2),
    FunctionInfo("SinChange", 5),
    FunctionInfo("CosChange", 5),
    FunctionInfo("ATan2", 3),
    FunctionInfo("Interpolate", 4),
    FunctionInfo("InterpolateXY", 7),
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
    FunctionInfo("RotatePalette", 3),
    FunctionInfo("SetScreenFade", 4),
    FunctionInfo("SetActivePalette", 3),
    FunctionInfo("SetPaletteFade", 7),
    FunctionInfo("CopyPalette", 2),
    FunctionInfo("ClearScreen", 1),
    FunctionInfo("DrawSpriteFX", 4),
    FunctionInfo("DrawSpriteScreenFX", 4),
    FunctionInfo("LoadAnimation", 1),
    FunctionInfo("SetupMenu", 4),
    FunctionInfo("AddMenuEntry", 3),
    FunctionInfo("EditMenuEntry", 4),
    FunctionInfo("LoadStage", 0),
    FunctionInfo("DrawRect", 8),
    FunctionInfo("ResetObjectEntity", 5),
    FunctionInfo("PlayerObjectCollision", 5),
    FunctionInfo("CreateTempObject", 4),
    FunctionInfo("BindPlayerToObject", 2),
    FunctionInfo("PlayerTileCollision", 0),
    FunctionInfo("ProcessPlayerControl", 0),
    FunctionInfo("ProcessAnimation", 0),
    FunctionInfo("DrawObjectAnimation", 0),
    FunctionInfo("DrawPlayerAnimation", 0),
    FunctionInfo("SetMusicTrack", 3),
    FunctionInfo("PlayMusic", 1),
    FunctionInfo("StopMusic", 0),
    FunctionInfo("PlaySfx", 2),
    FunctionInfo("StopSfx", 1),
    FunctionInfo("SetSfxAttributes", 3),
    FunctionInfo("ObjectTileCollision", 4),
    FunctionInfo("ObjectTileGrip", 4),
    FunctionInfo("LoadVideo", 1),
    FunctionInfo("NextVideoFrame", 0),
    FunctionInfo("PlayStageSfx", 2),
    FunctionInfo("StopStageSfx", 1),
    FunctionInfo("Not", 1),
    FunctionInfo("Draw3DScene", 0),
    FunctionInfo("SetIdentityMatrix", 1),
    FunctionInfo("MatrixMultiply", 2),
    FunctionInfo("MatrixTranslateXYZ", 4),
    FunctionInfo("MatrixScaleXYZ", 4),
    FunctionInfo("MatrixRotateX", 2),
    FunctionInfo("MatrixRotateY", 2),
    FunctionInfo("MatrixRotateZ", 2),
    FunctionInfo("MatrixRotateXYZ", 4),
    FunctionInfo("TransformVertices", 3),
    FunctionInfo("CallFunction", 1),
    FunctionInfo("EndFunction", 0),
    FunctionInfo("SetLayerDeformation", 6),
    FunctionInfo("CheckTouchRect", 4),
    FunctionInfo("GetTileLayerEntry", 4),
    FunctionInfo("SetTileLayerEntry", 4),
    FunctionInfo("GetBit", 3),
    FunctionInfo("SetBit", 3),
    FunctionInfo("PauseMusic", 0),
    FunctionInfo("ResumeMusic", 0),
    FunctionInfo("ClearDrawList", 1),
    FunctionInfo("AddDrawListEntityRef", 2),
    FunctionInfo("GetDrawListEntityRef", 3),
    FunctionInfo("SetDrawListEntityRef", 3),
    FunctionInfo("Get16x16TileInfo", 4),
    FunctionInfo("Copy16x16Tile", 2),
    FunctionInfo("Set16x16TileInfo", 4),
    FunctionInfo("GetAnimationByName", 2),
    FunctionInfo("ReadSaveRAM", 0),
    FunctionInfo("WriteSaveRAM", 0),
    FunctionInfo("LoadTextFont", 1),
    FunctionInfo("LoadTextFile", 3),
    FunctionInfo("DrawText", 7),
    FunctionInfo("GetTextInfo", 5),
    FunctionInfo("GetVersionNumber", 2),
    FunctionInfo("SetAchievement", 2),
    FunctionInfo("SetLeaderboard", 2),
    FunctionInfo("LoadOnlineMenu", 1),
    FunctionInfo("EngineCallback", 1),
    FunctionInfo("HapticEffect", 4),
    // EDITOR-ONLY
    FunctionInfo("AddEditorVariable", 1),
    FunctionInfo("SetActiveVariable", 1),
    FunctionInfo("AddEnumVariable", 2),
};

AliasInfo aliases[ALIAS_COUNT] = {
    AliasInfo("true", "1"),
    AliasInfo("false", "0"),
    AliasInfo("FX_SCALE", "0"),
    AliasInfo("FX_ROTATE", "1"),
    AliasInfo("FX_ROTOZOOM", "2"),
    AliasInfo("FX_INK", "3"),
    AliasInfo("PRESENTATION_STAGE", "0"),
    AliasInfo("REGULAR_STAGE", "1"),
    AliasInfo("BONUS_STAGE", "2"),
    AliasInfo("SPECIAL_STAGE", "3"),
    AliasInfo("MENU_1", "0"),
    AliasInfo("MENU_2", "1"),
    AliasInfo("C_TOUCH", "0"),
    AliasInfo("C_BOX", "1"),
    AliasInfo("C_BOX2", "2"),
    AliasInfo("C_PLATFORM", "3"),
    AliasInfo("MAT_WORLD", "0"),
    AliasInfo("MAT_VIEW", "1"),
    AliasInfo("MAT_TEMP", "2"),
    AliasInfo("FX_FLIP", "5"),
    AliasInfo("FACING_LEFT", "1"),
    AliasInfo("FACING_RIGHT", "0"),
    AliasInfo("STAGE_PAUSED", "2"),
    AliasInfo("STAGE_RUNNING", "1"),
    AliasInfo("RESET_GAME", "2"),
    AliasInfo("RETRO_WIN", "0"),
    AliasInfo("RETRO_OSX", "1"),
    AliasInfo("RETRO_XBOX_360", "2"),
    AliasInfo("RETRO_PS3", "3"),
    AliasInfo("RETRO_iOS", "4"),
    AliasInfo("RETRO_ANDROID", "5"),
    AliasInfo("RETRO_WP7", "6"),
    // EDITOR-ONLY
    AliasInfo("EDIT_VAR_PROPVAL", "-1"),
    AliasInfo("ALIAS_VAR_PROPVAL", "0"),
};

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
    PARSEMODE_ERROR        = 0xFF
};

enum ScriptVarTypes { SCRIPTVAR_VAR = 1, SCRIPTVAR_INTCONST = 2, SCRIPTVAR_STRCONST = 3 };
enum ScriptVarArrTypes {
    VARARR_NONE        = 0,
    VARARR_ARRAY       = 1,
    VARARR_ENTNOPLUS1  = 2,
    VARARR_ENTNOMINUS1 = 3
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
    // EDITOR-ONLY
    VAR_EDITORVARIABLEID,
    VAR_EDITORVARIABLEVAL,
    VAR_EDITORRETURNVAR,
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
    // EDITOR-ONLY
    FUNC_ADDEDITORVAR,
    FUNC_SETACTIVEVAR,
    FUNC_ADDENUMVAR,
    FUNC_MAX_CNT
};

Compilerv3::Compilerv3()
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

int Compilerv3::findStringToken(QString &string, QString token, char stopID)
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

void Compilerv3::checkAliasText(QString &text)
{
    if (findStringToken(text, "#alias", 1))
        return;
    int textPos    = 6;
    int aliasMatch = 0;
    while (aliasMatch < 2) {
        if (aliasMatch) {
            if (aliasMatch == 1) {
                aliases[aliasCount].name += text[textPos];
                if (textPos + 1 >= text.length())
                    ++aliasMatch;
            }
        }
        else if (text[textPos] == ':') {
            aliasMatch = 1;
        }
        else {
            aliases[aliasCount].value += text[textPos];
        }
        ++textPos;
    }
    ++aliasCount;
}
void Compilerv3::convertArithmaticSyntax(QString &text)
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
        dest = functions[token].name + "(";
        for (int i = 0; i < offset; ++i) dest += text[i];
        if (functions[token].opcodeSize > 1) {
            dest += ',';
            offset += scriptEvaluationTokens[token - 1].length();
            ++findID;
            while (offset < text.length()) dest += text[offset++];
        }

        dest = dest + ")";
        text = dest;
    }
}
void Compilerv3::convertIfWhileStatement(QString &text)
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
                dest = functions[compareOp + FUNC_WEQUAL].name + "(";
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
            dest = functions[compareOp + FUNC_IFEQUAL].name + "(";
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
bool Compilerv3::convertSwitchStatement(QString &text)
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
void Compilerv3::convertFunctionText(QString &text)
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

    for (int i = 0; i < FUNC_MAX_CNT; ++i) {
        if (funcName == functions[i].name) {
            opcode     = i;
            opcodeSize = functions[i].opcodeSize;
            textPos    = functions[i].name.length();
            i          = FUNC_MAX_CNT;
        }
    }

    if (opcode <= 0) {
        scriptError = true;
        errorMsg    = "OPCODE NOT FOUND";
        errorPos    = funcName;
    }
    else {
        scriptData[scriptDataPos++] = opcode;
        if (functions[opcode].name == "else")
            jumpTableData[jumpTableStack[jumpTableStackPos]] = scriptDataPos - scriptDataOffset;

        if (functions[opcode].name == "endif") {
            int jPos                = jumpTableStack[jumpTableStackPos];
            jumpTableData[jPos + 1] = scriptDataPos - scriptDataOffset;
            if (jumpTableData[jPos] == -1)
                jumpTableData[jPos] = (scriptDataPos - scriptDataOffset) - 1;
            --jumpTableStackPos;
        }

        if (functions[opcode].name == "endswitch") {
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

        if (functions[opcode].name == "loop") {
            jumpTableData[jumpTableStack[jumpTableStackPos--] + 1] = scriptDataPos - scriptDataOffset;
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
            for (int a = 0; a < aliasCount; ++a) {
                if (funcName == aliases[a].name) {
                    copyAliasStr(funcName, aliases[a].value, 0);
                    if (findStringToken(aliases[a].value, "[", 1) > -1)
                        copyAliasStr(strBuffer, aliases[a].value, 1);
                }
            }
            // Eg: TempValue0 = Game.Variable
            for (int v = 0; v < globalVariables.count(); ++v) {
                if (funcName == globalVariables[v]) {
                    funcName  = "Global";
                    strBuffer = "";
                    appendIntegerToString(strBuffer, v);
                }
            }
            // Eg: TempValue0 = Function1
            for (int f = 0; f < functionCount; ++f) {
                if (funcName == functionNames[f]) {
                    funcName = "";
                    appendIntegerToString(funcName, f);
                }
            }
            // Eg: TempValue0 = TypeName[PlayerObject]
            if (funcName == "TypeName") {
                funcName = "";
                appendIntegerToString(funcName, 0);
                for (int o = 0; o < OBJECT_COUNT; ++o) {
                    if (strBuffer == typeNames[o]) {
                        funcName = "";
                        appendIntegerToString(funcName, o);
                    }
                }
            }

            // Eg: TempValue0 = SfxName[Jump]
            if (funcName == "SfxName") {
                funcName = "";
                appendIntegerToString(funcName, 0);
                int s = 0;
                for (; s < globalSfxNames.count(); ++s) {
                    if (strBuffer == globalSfxNames[s]) {
                        funcName = "";
                        appendIntegerToString(funcName, s);
                        break;
                    }
                }

                if (s == globalSfxNames.count()) {
                    for (; s < stageSfxNames.count(); ++s) {
                        if (strBuffer == stageSfxNames[s]) {
                            funcName = "";
                            appendIntegerToString(funcName, s);
                            break;
                        }
                    }

                    if (s == stageSfxNames.count()) {
                        printLog(QString("WARNING: Unknown sfxName \"%1\", on line %2")
                                     .arg(strBuffer)
                                     .arg(lineID));
                    }
                }
            }

            // Eg: TempValue0 = AchievementName[Ring King]
            if (funcName == "AchievementName") {
                funcName = "";
                appendIntegerToString(funcName, 0);
                // default to 0, we don't know what these are
            }

            // Eg: TempValue0 = PlayerName[SONIC]
            if (funcName == "PlayerName") {
                funcName = "";
                appendIntegerToString(funcName, 0);
                int p = 0;
                if (viewer) {
                    for (; p < ((SceneViewer *)viewer)->gameConfig.players.count(); ++p) {
                        QString name = ((SceneViewer *)viewer)->gameConfig.players[p].m_name;
                        name         = name.replace(" ", "");

                        if (strBuffer == name) {
                            funcName = "";
                            appendIntegerToString(funcName, p);
                            break;
                        }
                    }

                    if (p == ((SceneViewer *)viewer)->gameConfig.players.count()) {
                        // printLog("WARNING: Unknown PlayerName \"%s\", on line %d", arrayStr, lineID);
                    }
                }
                else {
                    for (; p < gameConfig.players.count(); ++p) {
                        QString name = gameConfig.players[p];
                        name         = name.replace(" ", "");

                        if (strBuffer == name) {
                            funcName = "";
                            appendIntegerToString(funcName, p);
                            break;
                        }
                    }

                    if (p == gameConfig.players.count()) {
                        printLog(QString("WARNING: Unknown PlayerName \"%1\", on line %2")
                                     .arg(strBuffer)
                                     .arg(lineID));
                    }
                }
            }

            // Eg: TempValue0 = StageName[GREEN HILL ZONE 1]
            if (funcName == "StageName") {
                funcName = "";
                int s    = -1;
                if (strBuffer.length() >= 2) {
                    char list = strBuffer[0].toLatin1();
                    switch (list) {
                        default: list = 0xFF;
                        case 'P': list = 0; break;
                        case 'R': list = 1; break;
                        case 'S': list = 2; break;
                        case 'B': list = 3; break;
                    }

                    s = -1;
                    if (list <= 3) {
                        QString scnName = strBuffer;
                        scnName.replace(" ", "");
                        scnName = scnName.mid(2, scnName.length() - 2);

                        for (; s < gameConfig.stageLists[list].scenes.count(); ++s) {
                            QString name = gameConfig.stageLists[list].scenes[s].name;
                            name         = name.replace(" ", "");
                            if (scnName == name)
                                break;
                        }

                        if (s == gameConfig.stageLists[list].scenes.count())
                            s = -1;
                    }
                }

                if (s == -1) {
                    printLog(QString("WARNING: Unknown StageName \"%1\", on line %2")
                                 .arg(strBuffer)
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
                scriptTextByteID            = 0;
                while (scriptTextPos > -1) {
                    switch (scriptTextByteID) {
                        case 0:
                            scriptData[scriptDataPos] = funcName[scriptTextPos].toLatin1() << 24;
                            ++scriptTextByteID;
                            break;
                        case 1:
                            scriptData[scriptDataPos] += funcName[scriptTextPos].toLatin1() << 16;
                            ++scriptTextByteID;
                            break;
                        case 2:
                            scriptData[scriptDataPos] += funcName[scriptTextPos].toLatin1() << 8;
                            ++scriptTextByteID;
                            break;
                        case 3:
                            scriptData[scriptDataPos++] += funcName[scriptTextPos].toLatin1();
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
                if (strBuffer.length()) {
                    scriptData[scriptDataPos] = VARARR_ARRAY;
                    if (strBuffer[0] == '+')
                        scriptData[scriptDataPos] = VARARR_ENTNOPLUS1;
                    if (strBuffer[0] == '-')
                        scriptData[scriptDataPos] = VARARR_ENTNOMINUS1;
                    ++scriptDataPos;
                    if (strBuffer[0] == '-' || strBuffer[0] == '+') {
                        strBuffer.remove(0, 1);
                    }
                    if (convertStringToInteger(strBuffer, &value) == 1) {
                        scriptData[scriptDataPos++] = 0;
                        scriptData[scriptDataPos++] = value;
                    }
                    else {
                        if (strBuffer == "ArrayPos0")
                            value = 0;
                        if (strBuffer == "ArrayPos1")
                            value = 1;
                        if (strBuffer == "TempObjectPos")
                            value = 2;
                        scriptData[scriptDataPos++] = VARARR_ARRAY;
                        scriptData[scriptDataPos++] = value;
                    }
                }
                else {
                    scriptData[scriptDataPos++] = VARARR_NONE;
                }
                value = -1;
                for (int i = 0; i < VAR_MAX_CNT; ++i) {
                    if (funcName == variableNames[i])
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
void Compilerv3::checkCaseNumber(QString &text)
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

        while (caseString[textPos] != ':' && textPos < caseString.length()) {
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

        // Eg: TempValue0 = TypeName[PlayerObject]
        if (caseValue == "TypeName") {
            caseValue = "";
            appendIntegerToString(caseValue, 0);
            for (int o = 0; o < OBJECT_COUNT; ++o) {
                if (arrayStr == typeNames[o]) {
                    caseValue = "";
                    appendIntegerToString(caseValue, o);
                }
            }
        }

        // Eg: TempValue0 = SfxName[Jump]
        if (caseValue == "SfxName") {
            caseValue = "";
            appendIntegerToString(caseValue, 0);
            int s = 0;
            for (; s < globalSfxNames.count(); ++s) {
                if (arrayStr == globalSfxNames[s]) {
                    caseValue = "";
                    appendIntegerToString(caseValue, s);
                    break;
                }
            }

            if (s == globalSfxNames.count()) {
                for (; s < stageSfxNames.count(); ++s) {
                    if (arrayStr == stageSfxNames[s]) {
                        caseValue = "";
                        appendIntegerToString(caseValue, s);
                        break;
                    }
                }

                if (s == stageSfxNames.count()) {
                    printLog(QString("WARNING: Unknown sfxName \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
                }
            }
        }

        // Eg: TempValue0 = AchievementName[Ring King]
        if (caseValue == "AchievementName") {
            caseValue = "";
            appendIntegerToString(caseValue, 0);
            // default to 0, we don't know what these are
        }

        // Eg: TempValue0 = PlayerName[SONIC]
        if (caseValue == "PlayerName") {
            caseValue = "";
            appendIntegerToString(caseValue, 0);
            int p = 0;
            if (viewer) {
                for (; p < ((SceneViewer *)viewer)->gameConfig.players.count(); ++p) {
                    QString name = ((SceneViewer *)viewer)->gameConfig.players[p].m_name;
                    name         = name.replace(" ", "");

                    if (arrayStr == name) {
                        caseValue = "";
                        appendIntegerToString(caseValue, p);
                        break;
                    }
                }

                if (p == ((SceneViewer *)viewer)->gameConfig.players.count()) {
                    // printLog("WARNING: Unknown PlayerName \"%s\", on line %d", arrayStr, lineID);
                }
            }
            else {
                for (; p < gameConfig.players.count(); ++p) {
                    QString name = gameConfig.players[p];
                    name         = name.replace(" ", "");

                    if (arrayStr == name) {
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

        // Eg: TempValue0 = StageName[GREEN HILL ZONE 1]
        if (caseValue == "StageName") {
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
                        if (scnName == name)
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

    for (int a = 0; a < aliasCount && !flag; ++a) {
        if (caseString == aliases[a].name) {
            caseString = aliases[a].value;
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
bool Compilerv3::readSwitchCase(QString &text)
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

            while (caseText[textPos] != ':' && textPos < caseText.length()) {
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

            // Eg: TempValue0 = TypeName[PlayerObject]
            if (caseValue == "TypeName") {
                caseValue = "";
                appendIntegerToString(caseValue, 0);
                for (int o = 0; o < OBJECT_COUNT; ++o) {
                    if (arrayStr == typeNames[o]) {
                        caseValue = "";
                        appendIntegerToString(caseValue, o);
                    }
                }
            }

            // Eg: TempValue0 = SfxName[Jump]
            if (caseValue == "SfxName") {
                caseValue = "";
                appendIntegerToString(caseValue, 0);
                int s = 0;
                for (; s < globalSfxNames.count(); ++s) {
                    if (arrayStr == globalSfxNames[s]) {
                        caseValue = "";
                        appendIntegerToString(caseValue, s);
                        break;
                    }
                }

                if (s == globalSfxNames.count()) {
                    for (; s < stageSfxNames.count(); ++s) {
                        if (arrayStr == stageSfxNames[s]) {
                            caseValue = "";
                            appendIntegerToString(caseValue, s);
                            break;
                        }
                    }

                    if (s == stageSfxNames.count()) {
                        printLog(QString("WARNING: Unknown sfxName \"%1\", on line %2")
                                     .arg(arrayStr)
                                     .arg(lineID));
                    }
                }
            }

            // Eg: TempValue0 = AchievementName[Ring King]
            if (caseValue == "AchievementName") {
                caseValue = "";
                appendIntegerToString(caseValue, 0);
                // default to 0, we don't know what these are
            }

            // Eg: TempValue0 = PlayerName[SONIC]
            if (caseValue == "PlayerName") {
                caseValue = "";
                appendIntegerToString(caseValue, 0);
                int p = 0;
                if (viewer) {
                    for (; p < ((SceneViewer *)viewer)->gameConfig.players.count(); ++p) {
                        QString name = ((SceneViewer *)viewer)->gameConfig.players[p].m_name;
                        name         = name.replace(" ", "");

                        if (arrayStr == name) {
                            caseValue = "";
                            appendIntegerToString(caseValue, p);
                            break;
                        }
                    }

                    if (p == ((SceneViewer *)viewer)->gameConfig.players.count()) {
                        // printLog("WARNING: Unknown PlayerName \"%s\", on line %d", arrayStr, lineID);
                    }
                }
                else {
                    for (; p < gameConfig.players.count(); ++p) {
                        QString name = gameConfig.players[p];
                        name         = name.replace(" ", "");

                        if (arrayStr == name) {
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

            // Eg: TempValue0 = StageName[GREEN HILL ZONE 1]
            if (caseValue == "StageName") {
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
                            if (scnName == name)
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

        for (int a = 0; a < aliasCount && !flag; ++a) {
            if (caseText == aliases[a].name)
                caseText = aliases[a].value;
        }

        int val = 0;

        int jPos    = jumpTableStack[jumpTableStackPos];
        int jOffset = jPos + 4;
        if (convertStringToInteger(caseText, &val))
            jumpTableData[val - jumpTableData[jPos] + jOffset] = scriptDataPos - scriptDataOffset;
        return true;
    }
    return false;
}
void Compilerv3::appendIntegerToString(QString &text, int value) { text += QString::number(value); }
bool Compilerv3::convertStringToInteger(QString &text, int *value)
{
    *value  = 0;
    bool ok = false;

    bool negative = false;
    if (text.startsWith("-")) {
        negative = true;
        text.remove(0, 1);
    }

    if (text.startsWith("0x") || text.startsWith("0X")) {
        text.remove(0, 2);
        *value = text.toInt(&ok, 0x10);
    }
    else if (text.startsWith("0b") || text.startsWith("0B")) {
        text.remove(0, 2);
        *value = text.toInt(&ok, 0b10);
    }
    else if (text.startsWith("0o") || text.startsWith("0O")) {
        text.remove(0, 2);
        *value = text.toInt(&ok, 0010);
    }
    else {
        *value = text.toInt(&ok, 10);
    }

    if (negative)
        *value = -*value;

    return ok;
}
void Compilerv3::copyAliasStr(QString &dest, QString text, bool arrayIndex)
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

void Compilerv3::parseScriptFile(QString scriptName, int scriptID, bool inEditor)
{
    this->inEditor = inEditor;

    jumpTableStackPos = 0;
    lineID            = 0;
    aliasCount        = COMMONALIAS_COUNT;
    for (int i = COMMONALIAS_COUNT; i < ALIAS_COUNT; ++i) {
        aliases[i].name  = "";
        aliases[i].value = "";
    }

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
                        if ((curChar == '\n' && prevChar != '\r')
                            || (curChar == '\n' && prevChar == '\r')) {
                            readMode = READMODE_ENDLINE;
                            if (curChar == ';')
                                semiFlag = true;
                        }
                    }
                    else if (curChar != '/' || textPos <= 0) {
                        scriptText += curChar;
                        if (curChar == '"')
                            readMode = READMODE_NORMAL;
                    }
                    else if (curChar == '/' && prevChar == '/') {
                        readMode = READMODE_COMMENTLINE;
                        scriptText.remove(--textPos, 1);
                    }
                    else {
                        scriptText += curChar;
                    }
                }
                else if (curChar == ' ' || curChar == '\t' || curChar == '\r' || curChar == '\n'
                         || curChar == ';' || readMode >= READMODE_COMMENTLINE) {
                    if ((curChar == '\n' && prevChar != '\r')
                        || (curChar == '\n' && prevChar == '\r')) {
                        readMode = READMODE_ENDLINE;
                        if (curChar == ';')
                            semiFlag = true;
                    }
                }
                else if (curChar != '/' || textPos <= 0) {
                    scriptText += curChar;
                    if (curChar == '"' && !readMode)
                        readMode = READMODE_STRING;
                }
                else if (curChar == '/' && prevChar == '/') {
                    readMode = READMODE_COMMENTLINE;
                    scriptText.remove(--textPos, 1);
                }
                else {
                    scriptText += curChar;
                }
                if (reader.isEOF()) {
                    readMode = READMODE_EOF;
                }
            }

            switch (parseMode) {
                case PARSEMODE_SCOPELESS:
                    if (!semiFlag)
                        ++lineID;
                    checkAliasText(scriptText);
                    if (inEditor) {
                        if (scriptText == "subRSDKDraw") {
                            parseMode                                            = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subRSDKDraw.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].subRSDKDraw.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                     = scriptDataPos;
                            jumpTableDataOffset                                  = jumpTableDataPos;
                        }
                        if (scriptText == "subRSDKLoad") {
                            parseMode                                            = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subRSDKLoad.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].subRSDKLoad.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                     = scriptDataPos;
                            jumpTableDataOffset                                  = jumpTableDataPos;
                        }
                        if (scriptText == "subRSDKEdit") {
                            parseMode                                            = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subRSDKEdit.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].subRSDKEdit.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                     = scriptDataPos;
                            jumpTableDataOffset                                  = jumpTableDataPos;
                        }
                    }
                    else {
                        if (scriptText == "subObjectMain") {
                            parseMode                                        = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subMain.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].subMain.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                 = scriptDataPos;
                            jumpTableDataOffset                              = jumpTableDataPos;
                        }
                        if (scriptText == "subObjectPlayerInteraction") {
                            parseMode = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subPlayerInteraction.scriptCodePtr =
                                scriptDataPos;
                            objectScriptList[scriptID].subPlayerInteraction.jumpTablePtr =
                                jumpTableDataPos;
                            scriptDataOffset    = scriptDataPos;
                            jumpTableDataOffset = jumpTableDataPos;
                        }
                        if (scriptText == "subObjectDraw") {
                            parseMode                                        = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subDraw.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].subDraw.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                 = scriptDataPos;
                            jumpTableDataOffset                              = jumpTableDataPos;
                        }
                        if (scriptText == "subObjectStartup") {
                            parseMode                                           = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subStartup.scriptCodePtr = scriptDataPos;
                            objectScriptList[scriptID].subStartup.jumpTablePtr  = jumpTableDataPos;
                            scriptDataOffset                                    = scriptDataPos;
                            jumpTableDataOffset                                 = jumpTableDataPos;
                        }
                    }

                    if (findStringToken(scriptText, "function", 1)) {
                        if (findStringToken(scriptText, "function", 1) == 1) {
                            QString funcName = "";
                            for (textPos = 9; textPos < scriptText.length(); ++textPos)
                                funcName += scriptText[textPos];

                            int funcID = -1;
                            for (int f = 0; f < functionCount; ++f) {
                                if (funcName == functionNames[f])
                                    funcID = f;
                            }
                            if (functionCount < FUNCTION_COUNT && funcID == -1) {
                                functionNames[functionCount++] = funcName;
                            }
                            parseMode = PARSEMODE_SCOPELESS;
                        }
                    }
                    else {
                        QString funcName = "";
                        for (textPos = 8; textPos < scriptText.length(); ++textPos)
                            funcName += scriptText[textPos];

                        int funcID = -1;
                        for (int f = 0; f < functionCount; ++f) {
                            if (funcName == functionNames[f])
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
                        if (scriptText == "endsub") {
                            scriptData[scriptDataPos++] = FUNC_END;
                            parseMode                   = PARSEMODE_SCOPELESS;
                        }
                        else if (scriptText == "endfunction") {
                            scriptData[scriptDataPos++] = FUNC_ENDFUNCTION;
                            parseMode                   = PARSEMODE_SCOPELESS;
                        }
                        else if (findStringToken(scriptText, "#platform:", 1)) {
                            // layed out like ass, but this means "if we did not find "#platform:"
                            if (findStringToken(scriptText, "#endplatform", 1) == -1) {
                                // if we did NOT find "#endplatform"
                                convertIfWhileStatement(scriptText);
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
                                 && findStringToken(scriptText, "Use_Decomp", 1) == -1
                                 && findStringToken(scriptText, "Use_Editor", 1) == -1) {
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
                default: break;
            }
        }

        reader.close();
    }
}

void Compilerv3::clearScriptData()
{
    memset(scriptData, 0, SCRIPTDATA_COUNT * sizeof(int));
    memset(jumpTableData, 0, JUMPTABLE_COUNT * sizeof(int));
    memset(functionStack, 0, FUNCSTACK_COUNT * sizeof(int));

    globalVariables.clear();

    jumpTableStackPos = 0;

    scriptDataPos       = 0;
    scriptDataOffset    = 0;
    jumpTableDataPos    = 0;
    jumpTableDataOffset = 0;

    functionCount = 0;

    for (int o = 0; o < OBJECT_COUNT; ++o) {
        ObjectScript *scriptInfo = &objectScriptList[o];

        scriptInfo->subMain.scriptCodePtr              = SCRIPTDATA_COUNT - 1;
        scriptInfo->subMain.jumpTablePtr               = JUMPTABLE_COUNT - 1;
        scriptInfo->subPlayerInteraction.scriptCodePtr = SCRIPTDATA_COUNT - 1;
        scriptInfo->subPlayerInteraction.jumpTablePtr  = JUMPTABLE_COUNT - 1;
        scriptInfo->subDraw.scriptCodePtr              = SCRIPTDATA_COUNT - 1;
        scriptInfo->subDraw.jumpTablePtr               = JUMPTABLE_COUNT - 1;
        scriptInfo->subStartup.scriptCodePtr           = SCRIPTDATA_COUNT - 1;
        scriptInfo->subStartup.jumpTablePtr            = JUMPTABLE_COUNT - 1;

        scriptInfo->subRSDKDraw.scriptCodePtr = SCRIPTDATA_COUNT - 1;
        scriptInfo->subRSDKDraw.jumpTablePtr  = JUMPTABLE_COUNT - 1;
        scriptInfo->subRSDKLoad.scriptCodePtr = SCRIPTDATA_COUNT - 1;
        scriptInfo->subRSDKLoad.jumpTablePtr  = JUMPTABLE_COUNT - 1;
        scriptInfo->subRSDKEdit.scriptCodePtr = SCRIPTDATA_COUNT - 1;
        scriptInfo->subRSDKEdit.jumpTablePtr  = JUMPTABLE_COUNT - 1;
        typeNames[o]                          = "";
    }

    for (int f = 0; f < FUNCTION_COUNT; ++f) {
        functionList[f].scriptCodePtr = SCRIPTDATA_COUNT - 1;
        functionList[f].jumpTablePtr  = JUMPTABLE_COUNT - 1;
    }
    globalSfxNames.clear();
    stageSfxNames.clear();

    typeNames[0] = "BlankObject";
}

void Compilerv3::writeBytecode(QString path)
{
    bytecode = RSDKv3::Bytecode();

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
        RSDKv3::Bytecode::ScriptInfo scr;

        scr.main.scriptCodePos = objectScriptList[i].subMain.scriptCodePtr;
        scr.main.jumpTablePos  = objectScriptList[i].subMain.jumpTablePtr;

        scr.playerInteraction.scriptCodePos = objectScriptList[i].subPlayerInteraction.scriptCodePtr;
        scr.playerInteraction.jumpTablePos  = objectScriptList[i].subPlayerInteraction.jumpTablePtr;

        scr.draw.scriptCodePos = objectScriptList[i].subDraw.scriptCodePtr;
        scr.draw.jumpTablePos  = objectScriptList[i].subDraw.jumpTablePtr;

        scr.startup.scriptCodePos = objectScriptList[i].subStartup.scriptCodePtr;
        scr.startup.jumpTablePos  = objectScriptList[i].subStartup.jumpTablePtr;

        bytecode.scriptList.append(scr);
    }

    bytecode.functionList.clear();
    for (int f = 0; f < functionCount; ++f) {
        RSDKv3::Bytecode::FunctionInfo func;
        func.scriptCodePos = functionList[f].scriptCodePtr;
        func.jumpTablePos  = functionList[f].jumpTablePtr;
        bytecode.functionList.append(func);
    }

    bytecode.write(path);
}

void Compilerv3::processScript(int scriptCodePtr, int jumpTablePtr, byte scriptSub)
{
    bool running        = true;
    int scriptDataPtr   = scriptCodePtr;
    jumpTableStackPos   = 0;
    functionStackPos    = 0;
    SceneViewer *viewer = (SceneViewer *)this->viewer;

    while (running) {
        int opcode           = scriptData[scriptDataPtr++];
        int opcodeSize       = functions[opcode].opcodeSize;
        int scriptCodeOffset = scriptDataPtr;
        // Get Values
        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = scriptData[scriptDataPtr++];

            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (scriptData[scriptDataPtr++]) {
                    case VARARR_NONE: arrayVal = objectLoop; break;
                    case VARARR_ARRAY:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal = scriptEng.arrayPosition[scriptData[scriptDataPtr++]];
                        else
                            arrayVal = scriptData[scriptDataPtr++];
                        break;
                    case VARARR_ENTNOPLUS1:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                scriptEng.arrayPosition[scriptData[scriptDataPtr++]] + objectLoop;
                        else
                            arrayVal = scriptData[scriptDataPtr++] + objectLoop;
                        break;
                    case VARARR_ENTNOMINUS1:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                objectLoop - scriptEng.arrayPosition[scriptData[scriptDataPtr++]];
                        else
                            arrayVal = objectLoop - scriptData[scriptDataPtr++];
                        break;
                    default: break;
                }

                // Variables
                switch (scriptData[scriptDataPtr++]) {
                    default: break;
                    case VAR_TEMPVALUE0: scriptEng.operands[i] = scriptEng.tempValue[0]; break;
                    case VAR_TEMPVALUE1: scriptEng.operands[i] = scriptEng.tempValue[1]; break;
                    case VAR_TEMPVALUE2: scriptEng.operands[i] = scriptEng.tempValue[2]; break;
                    case VAR_TEMPVALUE3: scriptEng.operands[i] = scriptEng.tempValue[3]; break;
                    case VAR_TEMPVALUE4: scriptEng.operands[i] = scriptEng.tempValue[4]; break;
                    case VAR_TEMPVALUE5: scriptEng.operands[i] = scriptEng.tempValue[5]; break;
                    case VAR_TEMPVALUE6: scriptEng.operands[i] = scriptEng.tempValue[6]; break;
                    case VAR_TEMPVALUE7: scriptEng.operands[i] = scriptEng.tempValue[7]; break;
                    case VAR_CHECKRESULT: scriptEng.operands[i] = scriptEng.checkResult; break;
                    case VAR_ARRAYPOS0: scriptEng.operands[i] = scriptEng.arrayPosition[0]; break;
                    case VAR_ARRAYPOS1: scriptEng.operands[i] = scriptEng.arrayPosition[1]; break;
                    case VAR_GLOBAL: /*scriptEng.operands[i] = globalVariables[arrayVal];*/ break;
                    case VAR_OBJECTENTITYNO: scriptEng.operands[i] = arrayVal; break;
                    case VAR_OBJECTTYPE: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].type;
                        break;
                    }
                    case VAR_OBJECTPROPERTYVALUE: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].propertyValue;
                        break;
                    }
                    case VAR_OBJECTXPOS: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].XPos;
                        break;
                    }
                    case VAR_OBJECTYPOS: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].YPos;
                        break;
                    }
                    case VAR_OBJECTIXPOS: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].XPos >> 16;
                        break;
                    }
                    case VAR_OBJECTIYPOS: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].YPos >> 16;
                        break;
                    }
                    case VAR_OBJECTSTATE: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].state;
                        break;
                    }
                    case VAR_OBJECTROTATION: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].rotation;
                        break;
                    }
                    case VAR_OBJECTSCALE: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].scale;
                        break;
                    }
                    case VAR_OBJECTPRIORITY: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].priority;
                        break;
                    }
                    case VAR_OBJECTDRAWORDER: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].drawOrder;
                        break;
                    }
                    case VAR_OBJECTDIRECTION: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].direction;
                        break;
                    }
                    case VAR_OBJECTINKEFFECT: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].inkEffect;
                        break;
                    }
                    case VAR_OBJECTALPHA: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].alpha;
                        break;
                    }
                    case VAR_OBJECTFRAME: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].frame;
                        break;
                    }
                    case VAR_OBJECTANIMATION: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].animation;
                        break;
                    }
                    case VAR_OBJECTPREVANIMATION: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].prevAnimation;
                        break;
                    }
                    case VAR_OBJECTANIMATIONSPEED: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].animationSpeed;
                        break;
                    }
                    case VAR_OBJECTANIMATIONTIMER: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].animationTimer;
                        break;
                    }
                    case VAR_OBJECTVALUE0: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].values[0];
                        break;
                    }
                    case VAR_OBJECTVALUE1: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].values[1];
                        break;
                    }
                    case VAR_OBJECTVALUE2: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].values[2];
                        break;
                    }
                    case VAR_OBJECTVALUE3: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].values[3];
                        break;
                    }
                    case VAR_OBJECTVALUE4: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].values[4];
                        break;
                    }
                    case VAR_OBJECTVALUE5: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].values[5];
                        break;
                    }
                    case VAR_OBJECTVALUE6: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].values[6];
                        break;
                    }
                    case VAR_OBJECTVALUE7: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].values[7];
                        break;
                    }
                    case VAR_OBJECTOUTOFBOUNDS: {
                        /*int pos = objectEntityList[arrayVal].XPos >> 16;
                        if (pos <= xScrollOffset - OBJECT_BORDER_X1 || pos >= OBJECT_BORDER_X2 +
                        xScrollOffset) { scriptEng.operands[i] = 1;
                        }
                        else {
                            int pos               = objectEntityList[arrayVal].YPos >> 16;
                            scriptEng.operands[i] = pos <= yScrollOffset - OBJECT_BORDER_Y1 || pos >=
                        yScrollOffset + OBJECT_BORDER_Y2;
                        }*/
                        break;
                    }
                    case VAR_PLAYERSTATE: {
                        break;
                    }
                    case VAR_PLAYERCONTROLMODE: {
                        break;
                    }
                    case VAR_PLAYERCONTROLLOCK: {
                        break;
                    }
                    case VAR_PLAYERCOLLISIONMODE: {
                        break;
                    }
                    case VAR_PLAYERCOLLISIONPLANE: {
                        break;
                    }
                    case VAR_PLAYERXPOS: {
                        break;
                    }
                    case VAR_PLAYERYPOS: {
                        break;
                    }
                    case VAR_PLAYERIXPOS: {
                        break;
                    }
                    case VAR_PLAYERIYPOS: {
                        break;
                    }
                    case VAR_PLAYERSCREENXPOS: {
                        break;
                    }
                    case VAR_PLAYERSCREENYPOS: {
                        break;
                    }
                    case VAR_PLAYERSPEED: {
                        break;
                    }
                    case VAR_PLAYERXVELOCITY: {
                        break;
                    }
                    case VAR_PLAYERYVELOCITY: {
                        break;
                    }
                    case VAR_PLAYERGRAVITY: {
                        break;
                    }
                    case VAR_PLAYERANGLE: {
                        break;
                    }
                    case VAR_PLAYERSKIDDING: {
                        break;
                    }
                    case VAR_PLAYERPUSHING: {
                        break;
                    }
                    case VAR_PLAYERTRACKSCROLL: {
                        break;
                    }
                    case VAR_PLAYERUP: {
                        break;
                    }
                    case VAR_PLAYERDOWN: {
                        break;
                    }
                    case VAR_PLAYERLEFT: {
                        break;
                    }
                    case VAR_PLAYERRIGHT: {
                        break;
                    }
                    case VAR_PLAYERJUMPPRESS: {
                        break;
                    }
                    case VAR_PLAYERJUMPHOLD: {
                        break;
                    }
                    case VAR_PLAYERFOLLOWPLAYER1: {
                        break;
                    }
                    case VAR_PLAYERLOOKPOS: {
                        break;
                    }
                    case VAR_PLAYERWATER: {
                        break;
                    }
                    case VAR_PLAYERTOPSPEED: {
                        break;
                    }
                    case VAR_PLAYERACCELERATION: {
                        break;
                    }
                    case VAR_PLAYERDECELERATION: {
                        break;
                    }
                    case VAR_PLAYERAIRACCELERATION: {
                        break;
                    }
                    case VAR_PLAYERAIRDECELERATION: {
                        break;
                    }
                    case VAR_PLAYERGRAVITYSTRENGTH: {
                        break;
                    }
                    case VAR_PLAYERJUMPSTRENGTH: {
                        break;
                    }
                    case VAR_PLAYERJUMPCAP: {
                        break;
                    }
                    case VAR_PLAYERROLLINGACCELERATION: {
                        break;
                    }
                    case VAR_PLAYERROLLINGDECELERATION: {
                        break;
                    }
                    case VAR_PLAYERENTITYNO: {
                        break;
                    }
                    case VAR_PLAYERCOLLISIONLEFT: {
                        break;
                    }
                    case VAR_PLAYERCOLLISIONTOP: {
                        break;
                    }
                    case VAR_PLAYERCOLLISIONRIGHT: {
                        break;
                    }
                    case VAR_PLAYERCOLLISIONBOTTOM: {
                        break;
                    }
                    case VAR_PLAYERFLAILING: {
                        break;
                    }
                    case VAR_PLAYERTIMER: {
                        break;
                    }
                    case VAR_PLAYERTILECOLLISIONS: {
                        break;
                    }
                    case VAR_PLAYEROBJECTINTERACTION: {
                        break;
                    }
                    case VAR_PLAYERVISIBLE: {
                        break;
                    }
                    case VAR_PLAYERROTATION: {
                        break;
                    }
                    case VAR_PLAYERSCALE: {
                        break;
                    }
                    case VAR_PLAYERPRIORITY: {
                        break;
                    }
                    case VAR_PLAYERDRAWORDER: {
                        break;
                    }
                    case VAR_PLAYERDIRECTION: {
                        break;
                    }
                    case VAR_PLAYERINKEFFECT: {
                        break;
                    }
                    case VAR_PLAYERALPHA: {
                        break;
                    }
                    case VAR_PLAYERFRAME: {
                        break;
                    }
                    case VAR_PLAYERANIMATION: {
                        break;
                    }
                    case VAR_PLAYERPREVANIMATION: {
                        break;
                    }
                    case VAR_PLAYERANIMATIONSPEED: {
                        break;
                    }
                    case VAR_PLAYERANIMATIONTIMER: {
                        break;
                    }
                    case VAR_PLAYERVALUE0: {
                        break;
                    }
                    case VAR_PLAYERVALUE1: {
                        break;
                    }
                    case VAR_PLAYERVALUE2: {
                        break;
                    }
                    case VAR_PLAYERVALUE3: {
                        break;
                    }
                    case VAR_PLAYERVALUE4: {
                        break;
                    }
                    case VAR_PLAYERVALUE5: {
                        break;
                    }
                    case VAR_PLAYERVALUE6: {
                        break;
                    }
                    case VAR_PLAYERVALUE7: {
                        break;
                    }
                    case VAR_PLAYERVALUE8: {
                        break;
                    }
                    case VAR_PLAYERVALUE9: {
                        break;
                    }
                    case VAR_PLAYERVALUE10: {
                        break;
                    }
                    case VAR_PLAYERVALUE11: {
                        break;
                    }
                    case VAR_PLAYERVALUE12: {
                        break;
                    }
                    case VAR_PLAYERVALUE13: {
                        break;
                    }
                    case VAR_PLAYERVALUE14: {
                        break;
                    }
                    case VAR_PLAYERVALUE15: {
                        break;
                    }
                    case VAR_PLAYEROUTOFBOUNDS: {
                        break;
                    }
                    case VAR_STAGESTATE: break;
                    case VAR_STAGEACTIVELIST: break;
                    case VAR_STAGELISTPOS: break;
                    case VAR_STAGETIMEENABLED: break;
                    case VAR_STAGEMILLISECONDS: break;
                    case VAR_STAGESECONDS: break;
                    case VAR_STAGEMINUTES: break;
                    case VAR_STAGEACTNO: break;
                    case VAR_STAGEPAUSEENABLED: break;
                    case VAR_STAGELISTSIZE: break;
                    case VAR_STAGENEWXBOUNDARY1: break;
                    case VAR_STAGENEWXBOUNDARY2: break;
                    case VAR_STAGENEWYBOUNDARY1: break;
                    case VAR_STAGENEWYBOUNDARY2: break;
                    case VAR_STAGEXBOUNDARY1: break;
                    case VAR_STAGEXBOUNDARY2: break;
                    case VAR_STAGEYBOUNDARY1: break;
                    case VAR_STAGEYBOUNDARY2: break;
                    case VAR_STAGEDEFORMATIONDATA0: break;
                    case VAR_STAGEDEFORMATIONDATA1: break;
                    case VAR_STAGEDEFORMATIONDATA2: break;
                    case VAR_STAGEDEFORMATIONDATA3: break;
                    case VAR_STAGEWATERLEVEL: break;
                    case VAR_STAGEACTIVELAYER: break;
                    case VAR_STAGEMIDPOINT: break;
                    case VAR_STAGEPLAYERLISTPOS: break;
                    case VAR_STAGEACTIVEPLAYER: break;
                    case VAR_SCREENCAMERAENABLED: break;
                    case VAR_SCREENCAMERATARGET: break;
                    case VAR_SCREENCAMERASTYLE: break;
                    case VAR_SCREENDRAWLISTSIZE: break;
                    case VAR_SCREENCENTERX: break;
                    case VAR_SCREENCENTERY: break;
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
                    case VAR_KEYDOWNUP: break;
                    case VAR_KEYDOWNDOWN: break;
                    case VAR_KEYDOWNLEFT: break;
                    case VAR_KEYDOWNRIGHT: break;
                    case VAR_KEYDOWNBUTTONA: break;
                    case VAR_KEYDOWNBUTTONB: break;
                    case VAR_KEYDOWNBUTTONC: break;
                    case VAR_KEYDOWNSTART: break;
                    case VAR_KEYPRESSUP: break;
                    case VAR_KEYPRESSDOWN: break;
                    case VAR_KEYPRESSLEFT: break;
                    case VAR_KEYPRESSRIGHT: break;
                    case VAR_KEYPRESSBUTTONA: break;
                    case VAR_KEYPRESSBUTTONB: break;
                    case VAR_KEYPRESSBUTTONC: break;
                    case VAR_KEYPRESSSTART: break;
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
                    case VAR_3DSCENENOVERTICES: break;
                    case VAR_3DSCENENOFACES: break;
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
                    case VAR_3DSCENEPROJECTIONX: break;
                    case VAR_3DSCENEPROJECTIONY: break;
                    case VAR_ENGINESTATE: break;
                    case VAR_STAGEDEBUGMODE: break;
                    case VAR_ENGINEMESSAGE: break;
                    case VAR_SAVERAM: break;
                    case VAR_ENGINELANGUAGE: break;
                    case VAR_OBJECTSPRITESHEET: {
                        scriptEng.operands[i] =
                            objectScriptList[objectEntityList[arrayVal].type].spriteSheetID;
                        break;
                    }
                    case VAR_ENGINEONLINEACTIVE: break;
                    case VAR_ENGINEFRAMESKIPTIMER: break;
                    case VAR_ENGINEFRAMESKIPSETTING: break;
                    case VAR_ENGINESFXVOLUME: break;
                    case VAR_ENGINEBGMVOLUME: break;
                    case VAR_ENGINEPLATFORMID: break;
                    case VAR_ENGINETRIALMODE: break;
                    case VAR_KEYPRESSANYSTART:
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

        ObjectScript *scriptInfo = &objectScriptList[objectEntityList[objectLoop].type];
        Entity *entity           = &objectEntityList[objectLoop];
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
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
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
            case FUNC_SINCHANGE: {
                scriptEng.operands[0] = scriptEng.operands[3]
                                        + (sin512(scriptEng.operands[1]) >> scriptEng.operands[2])
                                        - scriptEng.operands[4];
                break;
            }
            case FUNC_COSCHANGE: {
                scriptEng.operands[0] = scriptEng.operands[3]
                                        + (cos512(scriptEng.operands[1]) >> scriptEng.operands[2])
                                        - scriptEng.operands[4];
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
                scriptInfo->spriteSheetID = viewer->addGraphicsFile(scriptText);
                break;
            case FUNC_REMOVESPRITESHEET:
                opcodeSize = 0;
                viewer->removeGraphicsFile(scriptText, -1);
                break;
            case FUNC_DRAWSPRITE:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                viewer->drawSprite((entity->XPos >> 16) + spriteFrame->pivotX,
                                   (entity->YPos >> 16) + spriteFrame->pivotY, spriteFrame->width,
                                   spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                   scriptInfo->spriteSheetID);
                break;
            case FUNC_DRAWSPRITEXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                viewer->drawSprite((scriptEng.operands[1] >> 16) + spriteFrame->pivotX,
                                   (scriptEng.operands[2] >> 16) + spriteFrame->pivotY,
                                   spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                   spriteFrame->sprY, scriptInfo->spriteSheetID);
                break;
            case FUNC_DRAWSPRITESCREENXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                viewer->drawSprite(scriptEng.operands[1] + spriteFrame->pivotX,
                                   scriptEng.operands[2] + spriteFrame->pivotY, spriteFrame->width,
                                   spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                   scriptInfo->spriteSheetID);
                break;
            case FUNC_DRAWTINTRECT: opcodeSize = 0; break;
            case FUNC_DRAWNUMBERS: {
                opcodeSize = 0;
                int i      = 10;
                if (scriptEng.operands[6]) {
                    while (scriptEng.operands[4] > 0) {
                        int frameID = scriptEng.operands[3] % i / (i / 10) + scriptEng.operands[0];
                        spriteFrame = &scriptFrames[scriptInfo->frameListOffset + frameID];
                        viewer->drawSprite(spriteFrame->pivotX + scriptEng.operands[1],
                                           spriteFrame->pivotY + scriptEng.operands[2],
                                           spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                           spriteFrame->sprY, scriptInfo->spriteSheetID);
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
                            viewer->drawSprite(spriteFrame->pivotX + scriptEng.operands[1],
                                               spriteFrame->pivotY + scriptEng.operands[2],
                                               spriteFrame->width, spriteFrame->height,
                                               spriteFrame->sprX, spriteFrame->sprY,
                                               scriptInfo->spriteSheetID);
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
                if (scriptSub == SUB_RSDKLOAD && scriptFrameCount < SPRITEFRAME_COUNT) {
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
            case FUNC_SETPALETTEFADE: opcodeSize = 0; break;
            case FUNC_COPYPALETTE: opcodeSize = 0; break;
            case FUNC_CLEARSCREEN: opcodeSize = 0; break;
            case FUNC_DRAWSPRITEFX:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                switch (scriptEng.operands[1]) {
                    default: break;
                    case FX_SCALE:
                        viewer->drawSpriteScaled(
                            entity->direction, scriptEng.operands[2] >> 16, scriptEng.operands[3] >> 16,
                            -spriteFrame->pivotX, -spriteFrame->pivotY, entity->scale, entity->scale,
                            spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                            spriteFrame->sprY, scriptInfo->spriteSheetID);
                        break;
                    case FX_ROTATE:
                        viewer->drawSpriteRotated(
                            entity->direction, scriptEng.operands[2] >> 16, scriptEng.operands[3] >> 16,
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->sprX,
                            spriteFrame->sprY, spriteFrame->width, spriteFrame->height,
                            entity->rotation, scriptInfo->spriteSheetID);
                        break;
                    case FX_ROTOZOOM:
                        viewer->drawSpriteRotozoom(
                            entity->direction, scriptEng.operands[2] >> 16, scriptEng.operands[3] >> 16,
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->sprX,
                            spriteFrame->sprY, spriteFrame->width, spriteFrame->height,
                            entity->rotation, entity->scale, scriptInfo->spriteSheetID);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                viewer->drawSprite((scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                                   (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                                   spriteFrame->width, spriteFrame->height,
                                                   spriteFrame->sprX, spriteFrame->sprY,
                                                   scriptInfo->spriteSheetID);
                                break;
                            case INK_BLEND:
                                viewer->drawBlendedSprite(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, scriptInfo->spriteSheetID);
                                break;
                            case INK_ALPHA:
                                viewer->drawAlphaBlendedSprite(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, entity->alpha, scriptInfo->spriteSheetID);
                                break;
                            case INK_ADD:
                                viewer->drawAdditiveBlendedSprite(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, entity->alpha, scriptInfo->spriteSheetID);
                                break;
                            case INK_SUB:
                                viewer->drawSubtractiveBlendedSprite(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, entity->alpha, scriptInfo->spriteSheetID);
                                break;
                        }
                        break;
                    case FX_TINT: break;
                    case FX_FLIP:
                        switch (entity->direction) {
                            default:
                            case FLIP_NONE:
                                viewer->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, scriptInfo->spriteSheetID);
                                break;
                            case FLIP_X:
                                viewer->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) - spriteFrame->width
                                        - spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_X, scriptInfo->spriteSheetID);
                                break;
                            case FLIP_Y:
                                viewer->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) - spriteFrame->height
                                        - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_Y, scriptInfo->spriteSheetID);
                                break;
                            case FLIP_XY:
                                viewer->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) - spriteFrame->width
                                        - spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) - spriteFrame->height
                                        - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_XY, scriptInfo->spriteSheetID);
                                break;
                        }
                        break;
                }
                break;
            case FUNC_DRAWSPRITESCREENFX: {
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                switch (scriptEng.operands[1]) {
                    default: break;
                    case FX_SCALE:
                        viewer->drawSpriteScaled(
                            entity->direction, scriptEng.operands[2], scriptEng.operands[3],
                            -spriteFrame->pivotX, -spriteFrame->pivotY, entity->scale, entity->scale,
                            spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                            spriteFrame->sprY, scriptInfo->spriteSheetID);
                        break;
                    case FX_ROTATE:
                        viewer->drawSpriteRotated(
                            entity->direction, scriptEng.operands[2], scriptEng.operands[3],
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->sprX,
                            spriteFrame->sprY, spriteFrame->width, spriteFrame->height,
                            entity->rotation, scriptInfo->spriteSheetID);
                        break;
                    case FX_ROTOZOOM:
                        viewer->drawSpriteRotozoom(
                            entity->direction, scriptEng.operands[2], scriptEng.operands[3],
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->sprX,
                            spriteFrame->sprY, spriteFrame->width, spriteFrame->height,
                            entity->rotation, entity->scale, scriptInfo->spriteSheetID);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                viewer->drawSprite(scriptEng.operands[2] + spriteFrame->pivotX,
                                                   scriptEng.operands[3] + spriteFrame->pivotY,
                                                   spriteFrame->width, spriteFrame->height,
                                                   spriteFrame->sprX, spriteFrame->sprY,
                                                   scriptInfo->spriteSheetID);
                                break;
                            case INK_BLEND:
                                viewer->drawBlendedSprite(scriptEng.operands[2] + spriteFrame->pivotX,
                                                          scriptEng.operands[3] + spriteFrame->pivotY,
                                                          spriteFrame->width, spriteFrame->height,
                                                          spriteFrame->sprX, spriteFrame->sprY,
                                                          scriptInfo->spriteSheetID);
                                break;
                            case INK_ALPHA:
                                viewer->drawAlphaBlendedSprite(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    entity->alpha, scriptInfo->spriteSheetID);
                                break;
                            case INK_ADD:
                                viewer->drawAdditiveBlendedSprite(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    entity->alpha, scriptInfo->spriteSheetID);
                                break;
                            case INK_SUB:
                                viewer->drawSubtractiveBlendedSprite(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    entity->alpha, scriptInfo->spriteSheetID);
                                break;
                        }
                        break;
                    case FX_TINT: break;
                    case FX_FLIP:
                        switch (entity->direction) {
                            default:
                            case FLIP_NONE:
                                viewer->drawSpriteFlipped(scriptEng.operands[2] + spriteFrame->pivotX,
                                                          scriptEng.operands[3] + spriteFrame->pivotY,
                                                          spriteFrame->width, spriteFrame->height,
                                                          spriteFrame->sprX, spriteFrame->sprY,
                                                          FLIP_NONE, scriptInfo->spriteSheetID);
                                break;
                            case FLIP_X:
                                viewer->drawSpriteFlipped(
                                    scriptEng.operands[2] - spriteFrame->width - spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_X,
                                    scriptInfo->spriteSheetID);
                                break;
                            case FLIP_Y:
                                viewer->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] - spriteFrame->height - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_Y, scriptInfo->spriteSheetID);
                                break;
                            case FLIP_XY:
                                viewer->drawSpriteFlipped(
                                    scriptEng.operands[2] - spriteFrame->width - spriteFrame->pivotX,
                                    scriptEng.operands[3] - spriteFrame->height - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_XY, scriptInfo->spriteSheetID);
                                break;
                        }
                        break;
                }
                break;
            }
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
                viewer->drawRectangle(scriptEng.operands[0], scriptEng.operands[1],
                                      scriptEng.operands[2], scriptEng.operands[3],
                                      scriptEng.operands[4], scriptEng.operands[5],
                                      scriptEng.operands[6], scriptEng.operands[7]);
                break;
            case FUNC_RESETOBJECTENTITY: {
                opcodeSize = 0;
                break;
            }
            case FUNC_PLAYEROBJECTCOLLISION: opcodeSize = 0; break;
            case FUNC_CREATETEMPOBJECT: {
                opcodeSize = 0;
                break;
            }
            case FUNC_BINDPLAYERTOOBJECT: {
                opcodeSize = 0;
                break;
            }
            case FUNC_PLAYERTILECOLLISION: opcodeSize = 0; break;
            case FUNC_PROCESSPLAYERCONTROL: opcodeSize = 0; break;
            case FUNC_PROCESSANIMATION: opcodeSize = 0; break;
            case FUNC_DRAWOBJECTANIMATION: opcodeSize = 0; break;
            case FUNC_DRAWPLAYERANIMATION: opcodeSize = 0; break;
            case FUNC_SETMUSICTRACK: opcodeSize = 0; break;
            case FUNC_PLAYMUSIC: opcodeSize = 0; break;
            case FUNC_STOPMUSIC: opcodeSize = 0; break;
            case FUNC_PLAYSFX: opcodeSize = 0; break;
            case FUNC_STOPSFX: opcodeSize = 0; break;
            case FUNC_SETSFXATTRIBUTES: opcodeSize = 0; break;
            case FUNC_OBJECTTILECOLLISION: opcodeSize = 0; break;
            case FUNC_OBJECTTILEGRIP: opcodeSize = 0; break;
            case FUNC_LOADVIDEO: opcodeSize = 0; break;
            case FUNC_NEXTVIDEOFRAME: opcodeSize = 0; break;
            case FUNC_PLAYSTAGESFX: opcodeSize = 0; break;
            case FUNC_STOPSTAGESFX: opcodeSize = 0; break;
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
                break;
            case FUNC_ENDFUNCTION:
                opcodeSize    = 0;
                scriptCodePtr = functionStack[--functionStackPos];
                jumpTablePtr  = functionStack[--functionStackPos];
                scriptDataPtr = functionStack[--functionStackPos];
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
            case FUNC_PAUSEMUSIC: opcodeSize = 0; break;
            case FUNC_RESUMEMUSIC: opcodeSize = 0; break;
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
            case FUNC_COPY16X16TILE: {
                opcodeSize = 0;
                break;
            }
            case FUNC_GETANIMATIONBYNAME: {
                opcodeSize = 0;
                break;
            }
            case FUNC_READSAVERAM: opcodeSize = 0; break;
            case FUNC_WRITESAVERAM: opcodeSize = 0; break;
            case FUNC_LOADTEXTFONT: opcodeSize = 0; break;
            case FUNC_LOADTEXTFILE: {
                opcodeSize = 0;
                break;
            }
            case FUNC_DRAWTEXT: {
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
            case FUNC_SETACHIEVEMENT: opcodeSize = 0; break;
            case FUNC_SETLEADERBOARD: opcodeSize = 0; break;
            case FUNC_LOADONLINEMENU: opcodeSize = 0; break;
            case FUNC_ENGINECALLBACK: opcodeSize = 0; break;
            case FUNC_HAPTICEFFECT: opcodeSize = 0; break;
            case FUNC_ADDEDITORVAR: {
                opcodeSize = 0;
                if (scriptSub == SUB_RSDKLOAD) {
                    viewer->addEditorVariable(scriptText);
                }
                break;
            }
            case FUNC_SETACTIVEVAR: {
                opcodeSize = 0;
                if (scriptSub == SUB_RSDKLOAD) {
                    viewer->setActiveVariable(scriptText);
                }
                break;
            }
            case FUNC_ADDENUMVAR: {
                opcodeSize = 0;
                if (scriptSub == SUB_RSDKLOAD) {
                    viewer->addEnumVariable(scriptText, scriptEng.operands[1]);
                }
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
                    case VARARR_NONE: arrayVal = objectLoop; break;
                    case VARARR_ARRAY:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal = scriptEng.arrayPosition[scriptData[scriptDataPtr++]];
                        else
                            arrayVal = scriptData[scriptDataPtr++];
                        break;
                    case VARARR_ENTNOPLUS1:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                scriptEng.arrayPosition[scriptData[scriptDataPtr++]] + objectLoop;
                        else
                            arrayVal = scriptData[scriptDataPtr++] + objectLoop;
                        break;
                    case VARARR_ENTNOMINUS1:
                        if (scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                objectLoop - scriptEng.arrayPosition[scriptData[scriptDataPtr++]];
                        else
                            arrayVal = objectLoop - scriptData[scriptDataPtr++];
                        break;
                    default: break;
                }

                // Variables
                switch (scriptData[scriptDataPtr++]) {
                    default: break;
                    case VAR_TEMPVALUE0: scriptEng.tempValue[0] = scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE1: scriptEng.tempValue[1] = scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE2: scriptEng.tempValue[2] = scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE3: scriptEng.tempValue[3] = scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE4: scriptEng.tempValue[4] = scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE5: scriptEng.tempValue[5] = scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE6: scriptEng.tempValue[6] = scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE7: scriptEng.tempValue[7] = scriptEng.operands[i]; break;
                    case VAR_CHECKRESULT: scriptEng.checkResult = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS0: scriptEng.arrayPosition[0] = scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS1: scriptEng.arrayPosition[1] = scriptEng.operands[i]; break;
                    case VAR_GLOBAL: /*globalVariables[arrayVal] = scriptEng.operands[i];*/ break;
                    case VAR_OBJECTENTITYNO: objectLoop = scriptEng.operands[i]; break;
                    case VAR_OBJECTTYPE: {
                        objectEntityList[arrayVal].type = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPROPERTYVALUE: {
                        objectEntityList[arrayVal].propertyValue = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTXPOS: {
                        objectEntityList[arrayVal].XPos = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTYPOS: {
                        objectEntityList[arrayVal].YPos = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTIXPOS: {
                        objectEntityList[arrayVal].XPos = scriptEng.operands[i] << 16;
                        break;
                    }
                    case VAR_OBJECTIYPOS: {
                        objectEntityList[arrayVal].YPos = scriptEng.operands[i] << 16;
                        break;
                    }
                    case VAR_OBJECTSTATE: {
                        objectEntityList[arrayVal].state = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTROTATION: {
                        objectEntityList[arrayVal].rotation = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTSCALE: {
                        objectEntityList[arrayVal].scale = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPRIORITY: {
                        objectEntityList[arrayVal].priority = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDRAWORDER: {
                        objectEntityList[arrayVal].drawOrder = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTDIRECTION: {
                        objectEntityList[arrayVal].direction = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTINKEFFECT: {
                        objectEntityList[arrayVal].inkEffect = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTALPHA: {
                        objectEntityList[arrayVal].alpha = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTFRAME: {
                        objectEntityList[arrayVal].frame = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATION: {
                        objectEntityList[arrayVal].animation = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPREVANIMATION: {
                        objectEntityList[arrayVal].prevAnimation = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATIONSPEED: {
                        objectEntityList[arrayVal].animationSpeed = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTANIMATIONTIMER: {
                        objectEntityList[arrayVal].animationTimer = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE0: {
                        objectEntityList[arrayVal].values[0] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE1: {
                        objectEntityList[arrayVal].values[1] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE2: {
                        objectEntityList[arrayVal].values[2] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE3: {
                        objectEntityList[arrayVal].values[3] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE4: {
                        objectEntityList[arrayVal].values[4] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE5: {
                        objectEntityList[arrayVal].values[5] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE6: {
                        objectEntityList[arrayVal].values[6] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTVALUE7: {
                        objectEntityList[arrayVal].values[7] = scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTOUTOFBOUNDS: break;
                    case VAR_PLAYERSTATE: {
                        break;
                    }
                    case VAR_PLAYERCONTROLMODE: {
                        break;
                    }
                    case VAR_PLAYERCONTROLLOCK: {
                        break;
                    }
                    case VAR_PLAYERCOLLISIONMODE: {
                        break;
                    }
                    case VAR_PLAYERCOLLISIONPLANE: {
                        break;
                    }
                    case VAR_PLAYERXPOS: {
                        break;
                    }
                    case VAR_PLAYERYPOS: {
                        break;
                    }
                    case VAR_PLAYERIXPOS: {
                        break;
                    }
                    case VAR_PLAYERIYPOS: {
                        break;
                    }
                    case VAR_PLAYERSCREENXPOS: {
                        break;
                    }
                    case VAR_PLAYERSCREENYPOS: {
                        break;
                    }
                    case VAR_PLAYERSPEED: {
                        break;
                    }
                    case VAR_PLAYERXVELOCITY: {
                        break;
                    }
                    case VAR_PLAYERYVELOCITY: {
                        break;
                    }
                    case VAR_PLAYERGRAVITY: {
                        break;
                    }
                    case VAR_PLAYERANGLE: {
                        break;
                    }
                    case VAR_PLAYERSKIDDING: {
                        break;
                    }
                    case VAR_PLAYERPUSHING: {
                        break;
                    }
                    case VAR_PLAYERTRACKSCROLL: {
                        break;
                    }
                    case VAR_PLAYERUP: {
                        break;
                    }
                    case VAR_PLAYERDOWN: {
                        break;
                    }
                    case VAR_PLAYERLEFT: {
                        break;
                    }
                    case VAR_PLAYERRIGHT: {
                        break;
                    }
                    case VAR_PLAYERJUMPPRESS: {
                        break;
                    }
                    case VAR_PLAYERJUMPHOLD: {
                        break;
                    }
                    case VAR_PLAYERFOLLOWPLAYER1: {
                        break;
                    }
                    case VAR_PLAYERLOOKPOS: {
                        break;
                    }
                    case VAR_PLAYERWATER: {
                        break;
                    }
                    case VAR_PLAYERTOPSPEED: {
                        break;
                    }
                    case VAR_PLAYERACCELERATION: {
                        break;
                    }
                    case VAR_PLAYERDECELERATION: {
                        break;
                    }
                    case VAR_PLAYERAIRACCELERATION: {
                        break;
                    }
                    case VAR_PLAYERAIRDECELERATION: {
                        break;
                    }
                    case VAR_PLAYERGRAVITYSTRENGTH: {
                        break;
                    }
                    case VAR_PLAYERJUMPSTRENGTH: {
                        break;
                    }
                    case VAR_PLAYERJUMPCAP: {
                        break;
                    }
                    case VAR_PLAYERROLLINGACCELERATION: {
                        break;
                    }
                    case VAR_PLAYERROLLINGDECELERATION: {
                        break;
                    }
                    case VAR_PLAYERENTITYNO: break;
                    case VAR_PLAYERCOLLISIONLEFT: break;
                    case VAR_PLAYERCOLLISIONTOP: break;
                    case VAR_PLAYERCOLLISIONRIGHT: break;
                    case VAR_PLAYERCOLLISIONBOTTOM: break;
                    case VAR_PLAYERFLAILING: {
                        break;
                    }
                    case VAR_PLAYERTIMER: {
                        break;
                    }
                    case VAR_PLAYERTILECOLLISIONS: {
                        break;
                    }
                    case VAR_PLAYEROBJECTINTERACTION: {
                        break;
                    }
                    case VAR_PLAYERVISIBLE: {
                        break;
                    }
                    case VAR_PLAYERROTATION: {
                        break;
                    }
                    case VAR_PLAYERSCALE: {
                        break;
                    }
                    case VAR_PLAYERPRIORITY: {
                        break;
                    }
                    case VAR_PLAYERDRAWORDER: {
                        break;
                    }
                    case VAR_PLAYERDIRECTION: {
                        break;
                    }
                    case VAR_PLAYERINKEFFECT: {
                        break;
                    }
                    case VAR_PLAYERALPHA: {
                        break;
                    }
                    case VAR_PLAYERFRAME: {
                        break;
                    }
                    case VAR_PLAYERANIMATION: {
                        break;
                    }
                    case VAR_PLAYERPREVANIMATION: {
                        break;
                    }
                    case VAR_PLAYERANIMATIONSPEED: {
                        break;
                    }
                    case VAR_PLAYERANIMATIONTIMER: {
                        break;
                    }
                    case VAR_PLAYERVALUE0: {
                        break;
                    }
                    case VAR_PLAYERVALUE1: {
                        break;
                    }
                    case VAR_PLAYERVALUE2: {
                        break;
                    }
                    case VAR_PLAYERVALUE3: {
                        break;
                    }
                    case VAR_PLAYERVALUE4: {
                        break;
                    }
                    case VAR_PLAYERVALUE5: {
                        break;
                    }
                    case VAR_PLAYERVALUE6: {
                        break;
                    }
                    case VAR_PLAYERVALUE7: {
                        break;
                    }
                    case VAR_PLAYERVALUE8: {
                        break;
                    }
                    case VAR_PLAYERVALUE9: {
                        break;
                    }
                    case VAR_PLAYERVALUE10: {
                        break;
                    }
                    case VAR_PLAYERVALUE11: {
                        break;
                    }
                    case VAR_PLAYERVALUE12: {
                        break;
                    }
                    case VAR_PLAYERVALUE13: {
                        break;
                    }
                    case VAR_PLAYERVALUE14: {
                        break;
                    }
                    case VAR_PLAYERVALUE15: {
                        break;
                    }
                    case VAR_PLAYEROUTOFBOUNDS: break;
                    case VAR_STAGESTATE: break;
                    case VAR_STAGEACTIVELIST: break;
                    case VAR_STAGELISTPOS: break;
                    case VAR_STAGETIMEENABLED: break;
                    case VAR_STAGEMILLISECONDS: break;
                    case VAR_STAGESECONDS: break;
                    case VAR_STAGEMINUTES: break;
                    case VAR_STAGEACTNO: break;
                    case VAR_STAGEPAUSEENABLED: break;
                    case VAR_STAGELISTSIZE: break;
                    case VAR_STAGENEWXBOUNDARY1: break;
                    case VAR_STAGENEWXBOUNDARY2: break;
                    case VAR_STAGENEWYBOUNDARY1: break;
                    case VAR_STAGENEWYBOUNDARY2: break;
                    case VAR_STAGEXBOUNDARY1: break;
                    case VAR_STAGEXBOUNDARY2: break;
                    case VAR_STAGEYBOUNDARY1: break;
                    case VAR_STAGEYBOUNDARY2: break;
                    case VAR_STAGEDEFORMATIONDATA0: break;
                    case VAR_STAGEDEFORMATIONDATA1: break;
                    case VAR_STAGEDEFORMATIONDATA2: break;
                    case VAR_STAGEDEFORMATIONDATA3: break;
                    case VAR_STAGEWATERLEVEL: break;
                    case VAR_STAGEACTIVELAYER: break;
                    case VAR_STAGEMIDPOINT: break;
                    case VAR_STAGEPLAYERLISTPOS: break;
                    case VAR_STAGEACTIVEPLAYER: break;
                    case VAR_SCREENCAMERAENABLED: break;
                    case VAR_SCREENCAMERATARGET: break;
                    case VAR_SCREENCAMERASTYLE: break;
                    case VAR_SCREENDRAWLISTSIZE: break;
                    case VAR_SCREENCENTERX: break;
                    case VAR_SCREENCENTERY: break;
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
                    case VAR_KEYDOWNUP: break;
                    case VAR_KEYDOWNDOWN: break;
                    case VAR_KEYDOWNLEFT: break;
                    case VAR_KEYDOWNRIGHT: break;
                    case VAR_KEYDOWNBUTTONA: break;
                    case VAR_KEYDOWNBUTTONB: break;
                    case VAR_KEYDOWNBUTTONC: break;
                    case VAR_KEYDOWNSTART: break;
                    case VAR_KEYPRESSUP: break;
                    case VAR_KEYPRESSDOWN: break;
                    case VAR_KEYPRESSLEFT: break;
                    case VAR_KEYPRESSRIGHT: break;
                    case VAR_KEYPRESSBUTTONA: break;
                    case VAR_KEYPRESSBUTTONB: break;
                    case VAR_KEYPRESSBUTTONC: break;
                    case VAR_KEYPRESSSTART: break;
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
                    case VAR_3DSCENENOVERTICES: break;
                    case VAR_3DSCENENOFACES: break;
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
                    case VAR_3DSCENEPROJECTIONX: break;
                    case VAR_3DSCENEPROJECTIONY: break;
                    case VAR_ENGINESTATE: break;
                    case VAR_STAGEDEBUGMODE: break;
                    case VAR_ENGINEMESSAGE: break;
                    case VAR_SAVERAM: break;
                    case VAR_ENGINELANGUAGE: break;
                    case VAR_OBJECTSPRITESHEET: {
                        objectScriptList[objectEntityList[arrayVal].type].spriteSheetID =
                            scriptEng.operands[i];
                        break;
                    }
                    case VAR_ENGINEONLINEACTIVE: break;
                    case VAR_ENGINEFRAMESKIPTIMER: break;
                    case VAR_ENGINEFRAMESKIPSETTING: break;
                    case VAR_ENGINESFXVOLUME: break;
                    case VAR_ENGINEBGMVOLUME: break;
                    case VAR_ENGINEPLATFORMID: break;
                    case VAR_ENGINETRIALMODE: break;
                    case VAR_KEYPRESSANYSTART: break;
                    case VAR_ENGINEHAPTICSENABLED:
                        break;
                        // EDITOR-ONLY
                    case VAR_EDITORVARIABLEID: break;
                    case VAR_EDITORVARIABLEVAL: break;
                    case VAR_EDITORRETURNVAR: break;
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

byte Compilerv3::ArcTanLookup(int X, int Y)
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
