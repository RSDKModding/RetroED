#include "includes.hpp"

#define ALIAS_COUNT       (0x80)
#define COMMONALIAS_COUNT (0x20)

struct AliasInfo {
    AliasInfo()
    {
        m_name  = "";
        m_value = "";
    }
    AliasInfo(QString aliasName, QString aliasVal)
    {
        m_name  = aliasName;
        m_value = aliasVal;
    }

    QString m_name  = "";
    QString m_value = "";
};

struct FunctionInfo {
    FunctionInfo()
    {
        m_name       = "";
        m_opcodeSize = 0;
    }
    FunctionInfo(QString functionName, int opSize)
    {
        m_name       = functionName;
        m_opcodeSize = opSize;
    }

    QString m_name   = "";
    int m_opcodeSize = 0;
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
};

const FunctionInfo functions[] = { FunctionInfo("End", 0),
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
                                   FunctionInfo("HapticEffect", 4) };

AliasInfo aliases[0x80] = { AliasInfo("true", "1"),
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
                            AliasInfo("RETRO_WP7", "6") };

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
                aliases[m_aliasCount].m_name += text[textPos];
                if (textPos + 1 >= text.length())
                    ++aliasMatch;
            }
        }
        else if (text[textPos] == ':') {
            aliasMatch = 1;
        }
        else {
            aliases[m_aliasCount].m_value += text[textPos];
        }
        ++textPos;
    }
    ++m_aliasCount;
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
        dest = functions[token].m_name + "(";
        for (int i = 0; i < offset; ++i) dest += text[i];
        if (functions[token].m_opcodeSize > 1) {
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
                dest = functions[compareOp + FUNC_WEQUAL].m_name + "(";
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
            dest = functions[compareOp + FUNC_IFEQUAL].m_name + "(";
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
bool Compilerv3::convertSwitchStatement(QString &text)
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
        if (funcName == functions[i].m_name) {
            opcode     = i;
            opcodeSize = functions[i].m_opcodeSize;
            textPos    = functions[i].m_name.length();
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
        if (functions[opcode].m_name == "else")
            m_jumpTableData[m_jumpTableStack[m_jumpTableStackPos]] =
                m_scriptDataPos - m_scriptDataOffset;

        if (functions[opcode].m_name == "endif") {
            int jPos                  = m_jumpTableStack[m_jumpTableStackPos];
            m_jumpTableData[jPos + 1] = m_scriptDataPos - m_scriptDataOffset;
            if (m_jumpTableData[jPos] == -1)
                m_jumpTableData[jPos] = (m_scriptDataPos - m_scriptDataOffset) - 1;
            --m_jumpTableStackPos;
        }

        if (functions[opcode].m_name == "endswitch") {
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

        if (functions[opcode].m_name == "loop") {
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
            for (int a = 0; a < m_aliasCount; ++a) {
                if (funcName == aliases[a].m_name) {
                    copyAliasStr(funcName, aliases[a].m_value, 0);
                    if (findStringToken(aliases[a].m_value, "[", 1) > -1)
                        copyAliasStr(strBuffer, aliases[a].m_value, 1);
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
            else if (funcName[0] == '"') {
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
                        if (strBuffer == "ArrayPos0")
                            value = 0;
                        if (strBuffer == "ArrayPos1")
                            value = 1;
                        if (strBuffer == "TempObjectPos")
                            value = 2;
                        m_scriptData[m_scriptDataPos++] = VARARR_ARRAY;
                        m_scriptData[m_scriptDataPos++] = value;
                    }
                }
                else {
                    m_scriptData[m_scriptDataPos++] = VARARR_NONE;
                }
                value = -1;
                for (int i = 0; i < VAR_MAX_CNT; ++i) {
                    if (funcName == variableNames[i])
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
void Compilerv3::checkCaseNumber(QString &text)
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

    int aliasVarID = 0;
    if (m_aliasCount) {
        aliasVarID = 0;
        do {
            while (dest != aliases[aliasVarID].m_name) {
                if (m_aliasCount <= ++aliasVarID)
                    goto CONV_VAL;
            }
            dest = aliases[aliasVarID++].m_value;
        } while (m_aliasCount > aliasVarID);
    }

CONV_VAL:
    if (!convertStringToInteger(dest, &aliasVarID))
        return;
    int stackValue = m_jumpTableStack[m_jumpTableStackPos];
    if (aliasVarID < m_jumpTableData[stackValue])
        m_jumpTableData[stackValue] = aliasVarID;
    stackValue++;
    if (aliasVarID > m_jumpTableData[stackValue])
        m_jumpTableData[stackValue] = aliasVarID;
}
bool Compilerv3::readSwitchCase(QString &text)
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

        for (int a = 0; a < m_aliasCount; ++a) {
            if (caseText == aliases[a].m_name)
                caseText = aliases[a].m_value;
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
void Compilerv3::appendIntegerToSting(QString &text, int value) { text += QString::number(value); }
bool Compilerv3::convertStringToInteger(QString &text, int *value)
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

void Compilerv3::parseScriptFile(QString scriptName, int scriptID)
{
    m_jumpTableStackPos = 0;
    m_lineID            = 0;
    m_aliasCount        = COMMONALIAS_COUNT;
    for (int i = COMMONALIAS_COUNT; i < ALIAS_COUNT; ++i) {
        aliases[i].m_name  = "";
        aliases[i].m_value = "";
    }

    Reader reader(scriptName);
    if (reader.initialised) {
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
                    if (m_scriptText == "subRSDKDraw") {
                        parseMode                                                = PARSEMODE_FUNCTION;
                        m_objectScriptList[scriptID].subRSDKDraw.m_scriptCodePtr = m_scriptDataPos;
                        m_objectScriptList[scriptID].subRSDKDraw.m_jumpTablePtr  = m_jumpTableDataPos;
                        m_scriptDataOffset                                       = m_scriptDataPos;
                        m_jumpTableDataOffset                                    = m_jumpTableDataPos;
                    }
                    if (m_scriptText == "subRSDKLoad") {
                        parseMode                                                = PARSEMODE_FUNCTION;
                        m_objectScriptList[scriptID].subRSDKLoad.m_scriptCodePtr = m_scriptDataPos;
                        m_objectScriptList[scriptID].subRSDKLoad.m_jumpTablePtr  = m_jumpTableDataPos;
                        m_scriptDataOffset                                       = m_scriptDataPos;
                        m_jumpTableDataOffset                                    = m_jumpTableDataPos;
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
                        if (m_scriptText == "endsub") {
                            m_scriptData[m_scriptDataPos++] = FUNC_END;
                            parseMode                       = PARSEMODE_SCOPELESS;
                        }
                        else if (m_scriptText == "endfunction") {
                            m_scriptData[m_scriptDataPos++] = FUNC_ENDFUNCTION;
                            parseMode                       = PARSEMODE_SCOPELESS;
                        }
                        else if (findStringToken(m_scriptText, "#platform:", 1)) {
                            // layed out like ass, but this means "if we did not find "#platform:"
                            if (findStringToken(m_scriptText, "#endplatform", 1) == -1) {
                                // if we did NOT find "#endplatform"
                                convertIfWhileStatement(m_scriptText);
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
                default: break;
            }
        }

        reader.close();
    }
}

void Compilerv3::clearScriptData()
{
    memset(m_scriptData, 0, SCRIPTDATA_COUNT * sizeof(int));
    memset(m_jumpTableData, 0, JUMPTABLE_COUNT * sizeof(int));

    m_jumpTableStackPos = 0;

    m_scriptDataPos       = 0;
    m_scriptDataOffset    = 0;
    m_jumpTableDataPos    = 0;
    m_jumpTableDataOffset = 0;

    m_functionCount = 0;

    for (int o = 0; o < OBJECT_COUNT; ++o) {
        ObjectScript *scriptInfo                = &m_objectScriptList[o];
        scriptInfo->subRSDKDraw.m_scriptCodePtr = SCRIPTDATA_COUNT - 1;
        scriptInfo->subRSDKDraw.m_jumpTablePtr  = JUMPTABLE_COUNT - 1;
        scriptInfo->subRSDKLoad.m_scriptCodePtr = SCRIPTDATA_COUNT - 1;
        scriptInfo->subRSDKLoad.m_jumpTablePtr  = JUMPTABLE_COUNT - 1;
        m_typeNames[o]                          = "";
    }

    for (int f = 0; f < FUNCTION_COUNT; ++f) {
        m_functionList[f].m_scriptCodePtr = SCRIPTDATA_COUNT - 1;
        m_functionList[f].m_jumpTablePtr  = JUMPTABLE_COUNT - 1;
    }

    m_typeNames[0] = "Blank Object";
}

void Compilerv3::writeBytecode(QString path)
{
    m_bytecode = RSDKv3::Bytecode();

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
        RSDKv3::Bytecode::ObjectScript scr;
        scr.m_mainScript    = m_objectScriptList[i].subRSDKDraw.m_scriptCodePtr;
        scr.m_mainJumpTable = m_objectScriptList[i].subRSDKDraw.m_jumpTablePtr;

        scr.m_playerScript    = m_objectScriptList[i].subRSDKLoad.m_scriptCodePtr;
        scr.m_playerJumpTable = m_objectScriptList[i].subRSDKLoad.m_jumpTablePtr;

        m_bytecode.m_scriptList.append(scr);
    }

    m_bytecode.functionList.clear();
    for (int f = 0; f < m_functionCount; ++f) {
        RSDKv3::Bytecode::FunctionScript func;
        func.m_mainScript    = m_functionList[f].m_scriptCodePtr;
        func.m_mainJumpTable = m_functionList[f].m_jumpTablePtr;
        m_bytecode.functionList.append(func);
    }

    m_bytecode.write(path);
}

void Compilerv3::processScript(int scriptCodePtr, int jumpTablePtr, byte scriptSub)
{
    bool running        = true;
    int scriptDataPtr   = scriptCodePtr;
    m_jumpTableStackPos = 0;
    m_functionStackPos  = 0;
    SceneViewer *viewer = (SceneViewer *)m_viewer;

    while (running) {
        int opcode           = m_scriptData[scriptDataPtr++];
        int opcodeSize       = functions[opcode].m_opcodeSize;
        int scriptCodeOffset = scriptDataPtr;
        // Get Valuess
        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = m_scriptData[scriptDataPtr++];

            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (m_scriptData[scriptDataPtr++]) {
                    case VARARR_NONE: arrayVal = m_objectLoop; break;
                    case VARARR_ARRAY:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal = m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]];
                        else
                            arrayVal = m_scriptData[scriptDataPtr++];
                        break;
                    case VARARR_ENTNOPLUS1:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]] + m_objectLoop;
                        else
                            arrayVal = m_scriptData[scriptDataPtr++] + m_objectLoop;
                        break;
                    case VARARR_ENTNOMINUS1:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                m_objectLoop - m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]];
                        else
                            arrayVal = m_objectLoop - m_scriptData[scriptDataPtr++];
                        break;
                    default: break;
                }

                // Variables
                switch (m_scriptData[scriptDataPtr++]) {
                    default: break;
                    case VAR_TEMPVALUE0: m_scriptEng.operands[i] = m_scriptEng.tempValue[0]; break;
                    case VAR_TEMPVALUE1: m_scriptEng.operands[i] = m_scriptEng.tempValue[1]; break;
                    case VAR_TEMPVALUE2: m_scriptEng.operands[i] = m_scriptEng.tempValue[2]; break;
                    case VAR_TEMPVALUE3: m_scriptEng.operands[i] = m_scriptEng.tempValue[3]; break;
                    case VAR_TEMPVALUE4: m_scriptEng.operands[i] = m_scriptEng.tempValue[4]; break;
                    case VAR_TEMPVALUE5: m_scriptEng.operands[i] = m_scriptEng.tempValue[5]; break;
                    case VAR_TEMPVALUE6: m_scriptEng.operands[i] = m_scriptEng.tempValue[6]; break;
                    case VAR_TEMPVALUE7: m_scriptEng.operands[i] = m_scriptEng.tempValue[7]; break;
                    case VAR_CHECKRESULT: m_scriptEng.operands[i] = m_scriptEng.checkResult; break;
                    case VAR_ARRAYPOS0: m_scriptEng.operands[i] = m_scriptEng.arrayPosition[0]; break;
                    case VAR_ARRAYPOS1: m_scriptEng.operands[i] = m_scriptEng.arrayPosition[1]; break;
                    case VAR_GLOBAL: /*m_scriptEng.operands[i] = globalVariables[arrayVal];*/ break;
                    case VAR_OBJECTENTITYNO: m_scriptEng.operands[i] = arrayVal; break;
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
                    case VAR_OBJECTOUTOFBOUNDS: {
                        /*int pos = m_objectEntityList[arrayVal].XPos >> 16;
                        if (pos <= xScrollOffset - OBJECT_BORDER_X1 || pos >= OBJECT_BORDER_X2 +
                        xScrollOffset) { m_scriptEng.operands[i] = 1;
                        }
                        else {
                            int pos               = m_objectEntityList[arrayVal].YPos >> 16;
                            m_scriptEng.operands[i] = pos <= yScrollOffset - OBJECT_BORDER_Y1 || pos >=
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
                        m_scriptEng.operands[i] =
                            m_objectScriptList[m_objectEntityList[arrayVal].type].spriteSheetID;
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
#if RETRO_USE_HAPTICS
                    case VAR_ENGINEHAPTICSENABLED:
                        m_scriptEng.operands[i] = Engine.hapticsEnabled;
                        break;
#endif
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

        ObjectScript *scriptInfo = &m_objectScriptList[m_objectEntityList[m_objectLoop].type];
        Entity *entity           = &m_objectEntityList[m_objectLoop];
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
                m_jumpTableStack[++m_jumpTableStackPos] = m_scriptEng.operands[0];
                opcodeSize                              = 0;
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
            case FUNC_SINCHANGE: {
                m_scriptEng.operands[0] = m_scriptEng.operands[3]
                                          + (sin512(m_scriptEng.operands[1]) >> m_scriptEng.operands[2])
                                          - m_scriptEng.operands[4];
                break;
            }
            case FUNC_COSCHANGE: {
                m_scriptEng.operands[0] = m_scriptEng.operands[3]
                                          + (cos512(m_scriptEng.operands[1]) >> m_scriptEng.operands[2])
                                          - m_scriptEng.operands[4];
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
                if (scriptSub == SUB_RSDKLOAD && scriptFrameCount < SPRITEFRAME_COUNT) {
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
            case FUNC_SETPALETTEFADE: opcodeSize = 0; break;
            case FUNC_COPYPALETTE: opcodeSize = 0; break;
            case FUNC_CLEARSCREEN: opcodeSize = 0; break;
            case FUNC_DRAWSPRITEFX:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + m_scriptEng.operands[0]];
                switch (m_scriptEng.operands[1]) {
                    default: break;
                    case FX_SCALE:
                        // DrawSpriteScaled(
                        //    entity->direction, (m_scriptEng.operands[2] >> 16) - xScrollOffset,
                        //    (m_scriptEng.operands[3] >> 16) - yScrollOffset, -spriteFrame->pivotX,
                        //    -spriteFrame->pivotY, entity->scale, entity->scale, spriteFrame->width,
                        //    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                        //    scriptInfo->spriteSheetID);
                        break;
                    case FX_ROTATE:
                        // DrawSpriteRotated(entity->direction,
                        //                  (m_scriptEng.operands[2] >> 16) - xScrollOffset,
                        //                  (m_scriptEng.operands[3] >> 16) - yScrollOffset,
                        //                  -spriteFrame->pivotX, -spriteFrame->pivotY,
                        //                  spriteFrame->sprX, spriteFrame->sprY, spriteFrame->width,
                        //                  spriteFrame->height, entity->rotation,
                        //                  scriptInfo->spriteSheetID);
                        break;
                    case FX_ROTOZOOM:
                        // DrawSpriteRotozoom(
                        //    entity->direction, (m_scriptEng.operands[2] >> 16) - xScrollOffset,
                        //    (m_scriptEng.operands[3] >> 16) - yScrollOffset, -spriteFrame->pivotX,
                        //    -spriteFrame->pivotY, spriteFrame->sprX, spriteFrame->sprY,
                        //    spriteFrame->width, spriteFrame->height, entity->rotation, entity->scale,
                        //    scriptInfo->spriteSheetID);
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
                                // DrawAdditiveBlendedSprite((m_scriptEng.operands[2] >> 16)
                                //                              - xScrollOffset + spriteFrame->pivotX,
                                //                          (m_scriptEng.operands[3] >> 16)
                                //                              - yScrollOffset + spriteFrame->pivotY,
                                //                          spriteFrame->width, spriteFrame->height,
                                //                          spriteFrame->sprX, spriteFrame->sprY,
                                //                          entity->alpha, scriptInfo->spriteSheetID);
                                break;
                            case INK_SUB:
                                // DrawSubtractiveBlendedSprite((m_scriptEng.operands[2] >> 16)
                                //                                 - xScrollOffset +
                                //                                 spriteFrame->pivotX,
                                //                             (m_scriptEng.operands[3] >> 16)
                                //                                 - yScrollOffset +
                                //                                 spriteFrame->pivotY,
                                //                             spriteFrame->width, spriteFrame->height,
                                //                             spriteFrame->sprX, spriteFrame->sprY,
                                //                             entity->alpha,
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
            case FUNC_DRAWSPRITESCREENFX: {
                opcodeSize = 0;
            } break;
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
                // DrawRectangle(m_scriptEng.operands[0], m_scriptEng.operands[1],
                // m_scriptEng.operands[2],
                //              m_scriptEng.operands[3], m_scriptEng.operands[4],
                //              m_scriptEng.operands[5], m_scriptEng.operands[6],
                //              m_scriptEng.operands[7]);
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
            case FUNC_TRANSFORMVERTICES: opcodeSize = 0; break;
            case FUNC_CALLFUNCTION: opcodeSize = 0; break;
            case FUNC_ENDFUNCTION: opcodeSize = 0; break;
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
        }

        // Set Values
        if (opcodeSize > 0)
            scriptDataPtr -= scriptDataPtr - scriptCodeOffset;
        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = m_scriptData[scriptDataPtr++];
            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (m_scriptData[scriptDataPtr++]) { // variable
                    case VARARR_NONE: arrayVal = m_objectLoop; break;
                    case VARARR_ARRAY:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal = m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]];
                        else
                            arrayVal = m_scriptData[scriptDataPtr++];
                        break;
                    case VARARR_ENTNOPLUS1:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]] + m_objectLoop;
                        else
                            arrayVal = m_scriptData[scriptDataPtr++] + m_objectLoop;
                        break;
                    case VARARR_ENTNOMINUS1:
                        if (m_scriptData[scriptDataPtr++] == 1)
                            arrayVal =
                                m_objectLoop - m_scriptEng.arrayPosition[m_scriptData[scriptDataPtr++]];
                        else
                            arrayVal = m_objectLoop - m_scriptData[scriptDataPtr++];
                        break;
                    default: break;
                }

                // Variables
                switch (m_scriptData[scriptDataPtr++]) {
                    default: break;
                    case VAR_TEMPVALUE0: m_scriptEng.tempValue[0] = m_scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE1: m_scriptEng.tempValue[1] = m_scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE2: m_scriptEng.tempValue[2] = m_scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE3: m_scriptEng.tempValue[3] = m_scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE4: m_scriptEng.tempValue[4] = m_scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE5: m_scriptEng.tempValue[5] = m_scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE6: m_scriptEng.tempValue[6] = m_scriptEng.operands[i]; break;
                    case VAR_TEMPVALUE7: m_scriptEng.tempValue[7] = m_scriptEng.operands[i]; break;
                    case VAR_CHECKRESULT: m_scriptEng.checkResult = m_scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS0: m_scriptEng.arrayPosition[0] = m_scriptEng.operands[i]; break;
                    case VAR_ARRAYPOS1: m_scriptEng.arrayPosition[1] = m_scriptEng.operands[i]; break;
                    case VAR_GLOBAL: /*globalVariables[arrayVal] = m_scriptEng.operands[i];*/ break;
                    case VAR_OBJECTENTITYNO: break;
                    case VAR_OBJECTTYPE: {
                        // m_objectEntityList[arrayVal].type = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTPROPERTYVALUE: {
                        // m_objectEntityList[arrayVal].propertyValue = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTXPOS: {
                        // m_objectEntityList[arrayVal].XPos = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTYPOS: {
                        // m_objectEntityList[arrayVal].YPos = m_scriptEng.operands[i];
                        break;
                    }
                    case VAR_OBJECTIXPOS: {
                        // m_objectEntityList[arrayVal].XPos = m_scriptEng.operands[i] << 16;
                        break;
                    }
                    case VAR_OBJECTIYPOS: {
                        // m_objectEntityList[arrayVal].YPos = m_scriptEng.operands[i] << 16;
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
                        m_objectScriptList[m_objectEntityList[arrayVal].type].spriteSheetID =
                            m_scriptEng.operands[i];
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
                    case VAR_ENGINEHAPTICSENABLED: break;
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
