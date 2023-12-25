#include "includes.hpp"
#include "compilerv3.hpp"

#include "tools/sceneeditor.hpp"

Compilerv3::VariableInfo Compilerv3::variableList[] = {
    VariableInfo("TempValue0"),
    VariableInfo("TempValue1"),
    VariableInfo("TempValue2"),
    VariableInfo("TempValue3"),
    VariableInfo("TempValue4"),
    VariableInfo("TempValue5"),
    VariableInfo("TempValue6"),
    VariableInfo("TempValue7"),
    VariableInfo("CheckResult"),
    VariableInfo("ArrayPos0"),
    VariableInfo("ArrayPos1"),
    VariableInfo("Global"),
    VariableInfo("Object.EntityNo"),
    VariableInfo("Object.Type"),
    VariableInfo("Object.PropertyValue"),
    VariableInfo("Object.XPos"),
    VariableInfo("Object.YPos"),
    VariableInfo("Object.iXPos"),
    VariableInfo("Object.iYPos"),
    VariableInfo("Object.State"),
    VariableInfo("Object.Rotation"),
    VariableInfo("Object.Scale"),
    VariableInfo("Object.Priority"),
    VariableInfo("Object.DrawOrder"),
    VariableInfo("Object.Direction"),
    VariableInfo("Object.InkEffect"),
    VariableInfo("Object.Alpha"),
    VariableInfo("Object.Frame"),
    VariableInfo("Object.Animation"),
    VariableInfo("Object.PrevAnimation"),
    VariableInfo("Object.AnimationSpeed"),
    VariableInfo("Object.AnimationTimer"),
    VariableInfo("Object.Value0"),
    VariableInfo("Object.Value1"),
    VariableInfo("Object.Value2"),
    VariableInfo("Object.Value3"),
    VariableInfo("Object.Value4"),
    VariableInfo("Object.Value5"),
    VariableInfo("Object.Value6"),
    VariableInfo("Object.Value7"),
    VariableInfo("Object.OutOfBounds"),
    VariableInfo("Player.State"),
    VariableInfo("Player.ControlMode"),
    VariableInfo("Player.ControlLock"),
    VariableInfo("Player.CollisionMode"),
    VariableInfo("Player.CollisionPlane"),
    VariableInfo("Player.XPos"),
    VariableInfo("Player.YPos"),
    VariableInfo("Player.iXPos"),
    VariableInfo("Player.iYPos"),
    VariableInfo("Player.ScreenXPos"),
    VariableInfo("Player.ScreenYPos"),
    VariableInfo("Player.Speed"),
    VariableInfo("Player.XVelocity"),
    VariableInfo("Player.YVelocity"),
    VariableInfo("Player.Gravity"),
    VariableInfo("Player.Angle"),
    VariableInfo("Player.Skidding"),
    VariableInfo("Player.Pushing"),
    VariableInfo("Player.TrackScroll"),
    VariableInfo("Player.Up"),
    VariableInfo("Player.Down"),
    VariableInfo("Player.Left"),
    VariableInfo("Player.Right"),
    VariableInfo("Player.JumpPress"),
    VariableInfo("Player.JumpHold"),
    VariableInfo("Player.FollowPlayer1"),
    VariableInfo("Player.LookPos"),
    VariableInfo("Player.Water"),
    VariableInfo("Player.TopSpeed"),
    VariableInfo("Player.Acceleration"),
    VariableInfo("Player.Deceleration"),
    VariableInfo("Player.AirAcceleration"),
    VariableInfo("Player.AirDeceleration"),
    VariableInfo("Player.GravityStrength"),
    VariableInfo("Player.JumpStrength"),
    VariableInfo("Player.JumpCap"),
    VariableInfo("Player.RollingAcceleration"),
    VariableInfo("Player.RollingDeceleration"),
    VariableInfo("Player.EntityNo"),
    VariableInfo("Player.CollisionLeft"),
    VariableInfo("Player.CollisionTop"),
    VariableInfo("Player.CollisionRight"),
    VariableInfo("Player.CollisionBottom"),
    VariableInfo("Player.Flailing"),
    VariableInfo("Player.Timer"),
    VariableInfo("Player.TileCollisions"),
    VariableInfo("Player.ObjectInteraction"),
    VariableInfo("Player.Visible"),
    VariableInfo("Player.Rotation"),
    VariableInfo("Player.Scale"),
    VariableInfo("Player.Priority"),
    VariableInfo("Player.DrawOrder"),
    VariableInfo("Player.Direction"),
    VariableInfo("Player.InkEffect"),
    VariableInfo("Player.Alpha"),
    VariableInfo("Player.Frame"),
    VariableInfo("Player.Animation"),
    VariableInfo("Player.PrevAnimation"),
    VariableInfo("Player.AnimationSpeed"),
    VariableInfo("Player.AnimationTimer"),
    VariableInfo("Player.Value0"),
    VariableInfo("Player.Value1"),
    VariableInfo("Player.Value2"),
    VariableInfo("Player.Value3"),
    VariableInfo("Player.Value4"),
    VariableInfo("Player.Value5"),
    VariableInfo("Player.Value6"),
    VariableInfo("Player.Value7"),
    VariableInfo("Player.Value8"),
    VariableInfo("Player.Value9"),
    VariableInfo("Player.Value10"),
    VariableInfo("Player.Value11"),
    VariableInfo("Player.Value12"),
    VariableInfo("Player.Value13"),
    VariableInfo("Player.Value14"),
    VariableInfo("Player.Value15"),
    VariableInfo("Player.OutOfBounds"),
    VariableInfo("Stage.State"),
    VariableInfo("Stage.ActiveList"),
    VariableInfo("Stage.ListPos"),
    VariableInfo("Stage.TimeEnabled"),
    VariableInfo("Stage.MilliSeconds"),
    VariableInfo("Stage.Seconds"),
    VariableInfo("Stage.Minutes"),
    VariableInfo("Stage.ActNo"),
    VariableInfo("Stage.PauseEnabled"),
    VariableInfo("Stage.ListSize"),
    VariableInfo("Stage.NewXBoundary1"),
    VariableInfo("Stage.NewXBoundary2"),
    VariableInfo("Stage.NewYBoundary1"),
    VariableInfo("Stage.NewYBoundary2"),
    VariableInfo("Stage.XBoundary1"),
    VariableInfo("Stage.XBoundary2"),
    VariableInfo("Stage.YBoundary1"),
    VariableInfo("Stage.YBoundary2"),
    VariableInfo("Stage.DeformationData0"),
    VariableInfo("Stage.DeformationData1"),
    VariableInfo("Stage.DeformationData2"),
    VariableInfo("Stage.DeformationData3"),
    VariableInfo("Stage.WaterLevel"),
    VariableInfo("Stage.ActiveLayer"),
    VariableInfo("Stage.MidPoint"),
    VariableInfo("Stage.PlayerListPos"),
    VariableInfo("Stage.ActivePlayer"),
    VariableInfo("Screen.CameraEnabled"),
    VariableInfo("Screen.CameraTarget"),
    VariableInfo("Screen.CameraStyle"),
    VariableInfo("Screen.DrawListSize"),
    VariableInfo("Screen.CenterX"),
    VariableInfo("Screen.CenterY"),
    VariableInfo("Screen.XSize"),
    VariableInfo("Screen.YSize"),
    VariableInfo("Screen.XOffset"),
    VariableInfo("Screen.YOffset"),
    VariableInfo("Screen.ShakeX"),
    VariableInfo("Screen.ShakeY"),
    VariableInfo("Screen.AdjustCameraY"),
    VariableInfo("TouchScreen.Down"),
    VariableInfo("TouchScreen.XPos"),
    VariableInfo("TouchScreen.YPos"),
    VariableInfo("Music.Volume"),
    VariableInfo("Music.CurrentTrack"),
    VariableInfo("KeyDown.Up"),
    VariableInfo("KeyDown.Down"),
    VariableInfo("KeyDown.Left"),
    VariableInfo("KeyDown.Right"),
    VariableInfo("KeyDown.ButtonA"),
    VariableInfo("KeyDown.ButtonB"),
    VariableInfo("KeyDown.ButtonC"),
    VariableInfo("KeyDown.Start"),
    VariableInfo("KeyPress.Up"),
    VariableInfo("KeyPress.Down"),
    VariableInfo("KeyPress.Left"),
    VariableInfo("KeyPress.Right"),
    VariableInfo("KeyPress.ButtonA"),
    VariableInfo("KeyPress.ButtonB"),
    VariableInfo("KeyPress.ButtonC"),
    VariableInfo("KeyPress.Start"),
    VariableInfo("Menu1.Selection"),
    VariableInfo("Menu2.Selection"),
    VariableInfo("TileLayer.XSize"),
    VariableInfo("TileLayer.YSize"),
    VariableInfo("TileLayer.Type"),
    VariableInfo("TileLayer.Angle"),
    VariableInfo("TileLayer.XPos"),
    VariableInfo("TileLayer.YPos"),
    VariableInfo("TileLayer.ZPos"),
    VariableInfo("TileLayer.ParallaxFactor"),
    VariableInfo("TileLayer.ScrollSpeed"),
    VariableInfo("TileLayer.ScrollPos"),
    VariableInfo("TileLayer.DeformationOffset"),
    VariableInfo("TileLayer.DeformationOffsetW"),
    VariableInfo("HParallax.ParallaxFactor"),
    VariableInfo("HParallax.ScrollSpeed"),
    VariableInfo("HParallax.ScrollPos"),
    VariableInfo("VParallax.ParallaxFactor"),
    VariableInfo("VParallax.ScrollSpeed"),
    VariableInfo("VParallax.ScrollPos"),
    VariableInfo("3DScene.NoVertices"),
    VariableInfo("3DScene.NoFaces"),
    VariableInfo("VertexBuffer.x"),
    VariableInfo("VertexBuffer.y"),
    VariableInfo("VertexBuffer.z"),
    VariableInfo("VertexBuffer.u"),
    VariableInfo("VertexBuffer.v"),
    VariableInfo("FaceBuffer.a"),
    VariableInfo("FaceBuffer.b"),
    VariableInfo("FaceBuffer.c"),
    VariableInfo("FaceBuffer.d"),
    VariableInfo("FaceBuffer.Flag"),
    VariableInfo("FaceBuffer.Color"),
    VariableInfo("3DScene.ProjectionX"),
    VariableInfo("3DScene.ProjectionY"),
    VariableInfo("Engine.State"),
    VariableInfo("Stage.DebugMode"),
    VariableInfo("Engine.Message"),
    VariableInfo("SaveRAM"),
    VariableInfo("Engine.Language"),
    VariableInfo("Object.SpriteSheet"),
    VariableInfo("Engine.OnlineActive"),
    VariableInfo("Engine.FrameSkipTimer"),
    VariableInfo("Engine.FrameSkipSetting"),
    VariableInfo("Engine.SFXVolume"),
    VariableInfo("Engine.BGMVolume"),
    VariableInfo("Engine.PlatformID"),
    VariableInfo("Engine.TrialMode"),
    VariableInfo("KeyPress.AnyStart"),
    VariableInfo("Engine.HapticsEnabled"),

    // EDITOR ONLY
    VariableInfo("Editor.VariableID"),
    VariableInfo("Editor.VariableValue"),
    VariableInfo("Editor.ReturnVariable"),
    VariableInfo("Editor.ShowGizmos"),
    VariableInfo("Editor.DrawingOverlay"),
};

Compilerv3::FunctionInfo Compilerv3::opcodeFunctionList[] = {
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
    FunctionInfo("SetVariableAlias", 2),
    FunctionInfo("DrawLine", 7),
    FunctionInfo("DrawArrow", 7),
    FunctionInfo("DrawRectWorld", 8),
    FunctionInfo("DrawRectOutline", 8),
    FunctionInfo("GetObjectType", 2),
    FunctionInfo("CheckCurrentStageFolder", 1),
};

Compilerv3::AliasInfo Compilerv3::aliases[ALIAS_COUNT_v3] = {
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

const char scriptEvaluationTokens[][0x4] = { "=",   "+=",  "-=", "++", "--", "*=", "/=",
                                             ">>=", "<<=", "&=", "|=", "^=", "%=", "==",
                                             ">",   ">=",  "<",  "<=", "!=" };

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
    VAR_EDITORSHOWGIZMOS,
    VAR_EDITORDRAWINGOVERLAY,
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
    FUNC_SETVARALIAS,
    FUNC_DRAWLINE,
    FUNC_DRAWARROW,
    FUNC_DRAWRECTWORLD,
    FUNC_DRAWRECTOUTLINE,
    FUNC_GETOBJECTTYPE,
    FUNC_CHECKCURRENTSTAGEFOLDER,
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

void Compilerv3::CheckAliasText(char *text)
{
    if (FindStringToken(text, "#alias", 1) != 0)
        return;

    int textPos     = 6;
    int aliasStrPos = 0;
    int parseMode   = 0;

    while (parseMode < 2) {
        if (parseMode) {
            if (parseMode == 1) {
                aliases[aliasCount].name[aliasStrPos] = text[textPos];
                if (text[textPos]) {
                    aliasStrPos++;
                }
                else {
                    aliasStrPos = 0;
                    ++parseMode;
                }
            }
        }
        else if (text[textPos] == ':') {
            aliases[aliasCount].value[aliasStrPos] = 0;
            aliasStrPos                            = 0;
            parseMode                              = 1;
        }
        else {
            aliases[aliasCount].value[aliasStrPos++] = text[textPos];
        }
        ++textPos;
    }

    ++aliasCount;
}
void Compilerv3::ConvertArithmaticSyntax(char *text)
{
    int token  = 0;
    int offset = 0;
    int findID = 0;
    char dest[260];

    for (int i = FUNC_EQUAL; i <= FUNC_MOD; ++i) {
        findID = FindStringToken(text, scriptEvaluationTokens[i - 1], 1);
        if (findID > -1) {
            offset = findID;
            token  = i;
        }
    }

    if (token > 0) {
        StrCopy(dest, opcodeFunctionList[token].name);
        StrAdd(dest, "(");
        findID = StrLength(dest);
        for (int i = 0; i < offset; ++i) dest[findID++] = text[i];
        if (opcodeFunctionList[token].opcodeSize > 1) {
            dest[findID] = ',';
            int len      = StrLength(scriptEvaluationTokens[token - 1]);
            offset += len;
            ++findID;
            while (text[offset]) dest[findID++] = text[offset++];
        }
        dest[findID] = 0;
        StrAdd(dest, ")");
        StrCopy(text, dest);
    }
}
void Compilerv3::ConvertIfWhileStatement(char *text)
{
    char dest[260];
    int compareOp  = -1;
    int strPos     = 0;
    int destStrPos = 0;

    if (FindStringToken(text, "if", 1) == 0) {
        for (int i = 0; i < 6; ++i) {
            destStrPos = FindStringToken(text, scriptEvaluationTokens[i + FUNC_MOD], 1);
            if (destStrPos > -1) {
                strPos    = destStrPos;
                compareOp = i;
            }
        }

        if (compareOp > -1) {
            text[strPos] = ',';

            StrCopy(dest, opcodeFunctionList[compareOp + FUNC_IFEQUAL].name);
            StrAdd(dest, "(");
            AppendIntegerToString(dest, jumpTablePos - jumpTableOffset);
            StrAdd(dest, ",");

            destStrPos = StrLength(dest);
            for (int i = 2; text[i]; ++i) {
                if (text[i] != '=' && text[i] != '(' && text[i] != ')')
                    dest[destStrPos++] = text[i];
            }
            dest[destStrPos] = 0;

            StrAdd(dest, ")");
            StrCopy(text, dest);

            jumpTableStack[++jumpTableStackPos] = jumpTablePos;
            jumpTable[jumpTablePos++]           = -1;
            jumpTable[jumpTablePos++]           = 0;
        }
    }
    else if (FindStringToken(text, "while", 1) == 0) {
        for (int i = 0; i < 6; ++i) {
            destStrPos = FindStringToken(text, scriptEvaluationTokens[i + FUNC_MOD], 1);
            if (destStrPos > -1) {
                strPos    = destStrPos;
                compareOp = i;
            }
        }

        if (compareOp > -1) {
            text[strPos] = ',';

            StrCopy(dest, opcodeFunctionList[compareOp + FUNC_WEQUAL].name);
            StrAdd(dest, "(");
            AppendIntegerToString(dest, jumpTablePos - jumpTableOffset);
            StrAdd(dest, ",");

            destStrPos = StrLength(dest);
            for (int i = 5; text[i]; ++i) {
                if (text[i] != '=' && text[i] != '(' && text[i] != ')')
                    dest[destStrPos++] = text[i];
            }
            dest[destStrPos] = 0;

            StrAdd(dest, ")");
            StrCopy(text, dest);

            jumpTableStack[++jumpTableStackPos] = jumpTablePos;
            jumpTable[jumpTablePos++]           = scriptCodePos - scriptCodeOffset;
            jumpTable[jumpTablePos++]           = 0;
        }
    }
}
bool Compilerv3::ConvertSwitchStatement(char *text)
{
    if (FindStringToken(text, "switch", 1))
        return false;

    char switchText[260];
    StrCopy(switchText, "switch");
    StrAdd(switchText, "(");
    AppendIntegerToString(switchText, jumpTablePos - jumpTableOffset);
    StrAdd(switchText, ",");
    int pos = StrLength(switchText);
    for (int i = 6; text[i]; ++i) {
        if (text[i] != '=' && text[i] != '(' && text[i] != ')')
            switchText[pos++] = text[i];
    }
    switchText[pos] = 0;
    StrAdd(switchText, ")");
    StrCopy(text, switchText);

    jumpTableStack[++jumpTableStackPos] = jumpTablePos;
    jumpTable[jumpTablePos++]           = 0x10000;
    jumpTable[jumpTablePos++]           = -0x10000;
    jumpTable[jumpTablePos++]           = -1;
    jumpTable[jumpTablePos++]           = 0;

    return true;
}
void Compilerv3::ConvertFunctionText(char *text)
{
    char arrayStr[128];
    char funcName[132];
    int opcode     = 0;
    int opcodeSize = 0;
    int textPos    = 0;
    int namePos    = 0;
    for (namePos = 0; text[namePos] != '(' && text[namePos]; ++namePos)
        funcName[namePos] = text[namePos];
    funcName[namePos] = 0;
    for (int i = 0; i < FUNC_MAX_CNT; ++i) {
        if (StrComp(funcName, opcodeFunctionList[i].name)) {
            opcode     = i;
            opcodeSize = opcodeFunctionList[i].opcodeSize;
            textPos    = StrLength(opcodeFunctionList[i].name);
            i          = FUNC_MAX_CNT;
        }
    }

    if (opcode <= 0) {
        scriptError = true;
        errorMsg    = "OPCODE NOT FOUND";
        errorPos    = funcName;
    }
    else {
        scriptCode[scriptCodePos++] = opcode;
        if (StrComp("else", opcodeFunctionList[opcode].name))
            jumpTable[jumpTableStack[jumpTableStackPos]] = scriptCodePos - scriptCodeOffset;

        if (StrComp("endif", opcodeFunctionList[opcode].name) == 1) {
            int jPos            = jumpTableStack[jumpTableStackPos];
            jumpTable[jPos + 1] = scriptCodePos - scriptCodeOffset;
            if (jumpTable[jPos] == -1)
                jumpTable[jPos] = (scriptCodePos - scriptCodeOffset) - 1;
            --jumpTableStackPos;
        }

        if (StrComp("endswitch", opcodeFunctionList[opcode].name)) {
            int jPos            = jumpTableStack[jumpTableStackPos];
            jumpTable[jPos + 3] = scriptCodePos - scriptCodeOffset;
            if (jumpTable[jPos + 2] == -1) {
                jumpTable[jPos + 2] = (scriptCodePos - scriptCodeOffset) - 1;
                int caseCnt         = abs(jumpTable[jPos + 1] - jumpTable[jPos]) + 1;

                int jOffset = jPos + 4;
                for (int c = 0; c < caseCnt; ++c) {
                    if (jumpTable[jOffset + c] < 0)
                        jumpTable[jOffset + c] = jumpTable[jPos + 2];
                }
            }
            --jumpTableStackPos;
        }

        if (StrComp("loop", opcodeFunctionList[opcode].name)) {
            jumpTable[jumpTableStack[jumpTableStackPos--] + 1] = scriptCodePos - scriptCodeOffset;
        }

        for (int i = 0; i < opcodeSize; ++i) {
            ++textPos;
            int funcNamePos = 0;
            int mode        = 0;
            int prevMode    = 0;
            int arrayStrPos = 0;
            while (((text[textPos] != ',' && text[textPos] != ')') || mode == 2) && text[textPos]) {
                switch (mode) {
                    case 0: // normal
                        if (text[textPos] == '[')
                            mode = 1;
                        else if (text[textPos] == '"') {
                            prevMode                = mode;
                            mode                    = 2;
                            funcName[funcNamePos++] = '"';
                        }
                        else
                            funcName[funcNamePos++] = text[textPos];
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
                            arrayStr[arrayStrPos++] = text[textPos];
                        ++textPos;
                        break;

                    case 2: // string
                        if (text[textPos] == '"') {
                            mode                    = prevMode;
                            funcName[funcNamePos++] = '"';
                        }
                        else
                            funcName[funcNamePos++] = text[textPos];
                        ++textPos;
                        break;
                }
            }
            funcName[funcNamePos] = 0;
            arrayStr[arrayStrPos] = 0;

            // Eg: TempValue0 = FX_SCALE
            for (int a = 0; a < aliasCount; ++a) {
                if (StrComp(funcName, aliases[a].name)) {
                    CopyAliasStr(funcName, aliases[a].value, 0);
                    if (FindStringToken(aliases[a].value, "[", 1) > -1)
                        CopyAliasStr(arrayStr, aliases[a].value, 1);
                }
            }

            // Eg: TempValue0 = Game.Variable
            for (int v = 0; v < globalVariables.count(); ++v) {
                if (StrComp(funcName, globalVariables[v].toStdString().c_str())) {
                    StrCopy(funcName, "Global");
                    arrayStr[0] = 0;
                    AppendIntegerToString(arrayStr, v);
                }
            }

            // Eg: TempValue0 = Function1
            for (int f = 0; f < functionCount; ++f) {
                if (StrComp(funcName, functionList[f].name)) {
                    funcName[0] = 0;
                    AppendIntegerToString(funcName, f);
                }
            }

            // Eg: TempValue0 = TypeName[PlayerObject]
            if (StrComp(funcName, "TypeName")) {
                funcName[0] = '0';
                funcName[1] = 0;

                int o = 0;
                for (; o < OBJECT_COUNT_v3; ++o) {
                    if (StrComp(arrayStr, typeNames[o])) {
                        funcName[0] = 0;
                        AppendIntegerToString(funcName, o);
                        break;
                    }
                }

                if (o == OBJECT_COUNT_v3)
                    PrintLog(QString("WARNING: Unknown typename \"%1\", on line %2")
                                 .arg(arrayStr)
                                 .arg(lineID));
            }

            // Eg: TempValue0 = SfxName[Jump]
            if (StrComp(funcName, "SfxName")) {
                funcName[0] = '0';
                funcName[1] = 0;
            }

            // Eg: TempValue0 = AchievementName[88 Miles Per Hour]
            if (StrComp(funcName, "AchievementName")) {
                funcName[0] = '0';
                funcName[1] = 0;
            }

            // Eg: TempValue0 = PlayerName[SONIC]
            if (StrComp(funcName, "PlayerName")) {
                funcName[0] = '0';
                funcName[1] = 0;
            }

            // Eg: TempValue0 = StageName[R - PALMTREE PANIC ZONE 1 A]
            if (StrComp(funcName, "StageName")) {
                funcName[0] = '0';
                funcName[1] = 0;
            }

            int constant = 0;
            if (ConvertStringToInteger(funcName, &constant)) {
                scriptCode[scriptCodePos++] = SCRIPTVAR_INTCONST;
                scriptCode[scriptCodePos++] = constant;
            }
            else if (funcName[0] == '"') {
                scriptCode[scriptCodePos++] = SCRIPTVAR_STRCONST;
                scriptCode[scriptCodePos++] = StrLength(funcName) - 2;

                int scriptTextPos = 1;
                arrayStrPos       = 0;
                while (scriptTextPos > -1) {
                    switch (arrayStrPos) {
                        case 0:
                            scriptCode[scriptCodePos] = funcName[scriptTextPos] << 24;
                            ++arrayStrPos;
                            break;

                        case 1:
                            scriptCode[scriptCodePos] += funcName[scriptTextPos] << 16;
                            ++arrayStrPos;
                            break;

                        case 2:
                            scriptCode[scriptCodePos] += funcName[scriptTextPos] << 8;
                            ++arrayStrPos;
                            break;

                        case 3:
                            scriptCode[scriptCodePos++] += funcName[scriptTextPos];
                            arrayStrPos = 0;
                            break;

                        default: break;
                    }

                    if (funcName[scriptTextPos] == '"') {
                        if (arrayStrPos > 0)
                            ++scriptCodePos;
                        scriptTextPos = -1;
                    }
                    else {
                        scriptTextPos++;
                    }
                }
            }
            else {
                scriptCode[scriptCodePos++] = SCRIPTVAR_VAR;
                if (arrayStr[0]) {
                    scriptCode[scriptCodePos] = VARARR_ARRAY;

                    if (arrayStr[0] == '+')
                        scriptCode[scriptCodePos] = VARARR_ENTNOPLUS1;

                    if (arrayStr[0] == '-')
                        scriptCode[scriptCodePos] = VARARR_ENTNOMINUS1;

                    ++scriptCodePos;

                    if (arrayStr[0] == '-' || arrayStr[0] == '+') {
                        for (int i = 0; i < StrLength(arrayStr); ++i) arrayStr[i] = arrayStr[i + 1];
                    }

                    if (ConvertStringToInteger(arrayStr, &constant)) {
                        scriptCode[scriptCodePos++] = 0;
                        scriptCode[scriptCodePos++] = constant;
                    }
                    else {
                        if (StrComp(arrayStr, "ArrayPos0"))
                            constant = 0;
                        if (StrComp(arrayStr, "ArrayPos1"))
                            constant = 1;
                        if (StrComp(arrayStr, "TempObjectPos"))
                            constant = 2;

                        scriptCode[scriptCodePos++] = 1;
                        scriptCode[scriptCodePos++] = constant;
                    }
                }
                else {
                    scriptCode[scriptCodePos++] = VARARR_NONE;
                }

                constant = -1;
                for (int i = 0; i < VAR_MAX_CNT; ++i) {
                    if (StrComp(funcName, variableList[i].name))
                        constant = i;
                }

                if (constant == -1 && !scriptError) {
                    scriptError = true;
                    errorMsg    = "OPERAND NOT FOUND";
                    errorPos    = funcName;
                    errorLine   = lineID;
                    constant    = 0;
                }

                scriptCode[scriptCodePos++] = constant;
            }
        }
    }
}
void Compilerv3::CheckCaseNumber(char *text)
{
    if (FindStringToken(text, "case", 1))
        return;

    char dest[128];
    int destStrPos = 0;
    char caseChar  = text[4];
    if (text[4]) {
        int textPos = 5;
        do {
            if (caseChar != ':')
                dest[destStrPos++] = caseChar;
            caseChar = text[textPos++];
        } while (caseChar);
    }
    else {
        destStrPos = 0;
    }
    dest[destStrPos] = 0;
    int aliasVarID   = 0;
    if (aliasCount) {
        aliasVarID = 0;
        do {
            while (!StrComp(dest, aliases[aliasVarID].name)) {
                if (aliasCount <= ++aliasVarID)
                    goto CONV_VAL;
            }
            StrCopy(dest, aliases[aliasVarID++].value);
        } while (aliasCount > aliasVarID);
    }

CONV_VAL:
    if (ConvertStringToInteger(dest, &aliasVarID) != 1)
        return;
    int stackValue = jumpTableStack[jumpTableStackPos];
    if (aliasVarID < jumpTable[stackValue])
        jumpTable[stackValue] = aliasVarID;
    stackValue++;
    if (aliasVarID > jumpTable[stackValue])
        jumpTable[stackValue] = aliasVarID;
}
bool Compilerv3::ReadSwitchCase(char *text)
{
    char caseText[0x80];
    if (FindStringToken(text, "case", 1) == 0) {
        int textPos       = 4;
        int caseStringPos = 0;
        while (text[textPos]) {
            if (text[textPos] != ':')
                caseText[caseStringPos++] = text[textPos];
            ++textPos;
        }
        caseText[caseStringPos] = 0;
        for (int a = 0; a < aliasCount; ++a) {
            if (StrComp(caseText, aliases[a].name))
                StrCopy(caseText, aliases[a].value);
        }

        int val = 0;

        int jPos    = jumpTableStack[jumpTableStackPos];
        int jOffset = jPos + 4;
        if (ConvertStringToInteger(caseText, &val))
            jumpTable[val - jumpTable[jPos] + jOffset] = scriptCodePos - scriptCodeOffset;
        return true;
    }
    else if (FindStringToken(text, "default", 1) == 0) {
        int jumpTablepos            = jumpTableStack[jumpTableStackPos];
        jumpTable[jumpTablepos + 2] = scriptCodePos - scriptCodeOffset;
        int cnt                     = abs(jumpTable[jumpTablepos + 1] - jumpTable[jumpTablepos]) + 1;

        int jOffset = jumpTablepos + 4;
        for (int i = 0; i < cnt; ++i) {
            if (jumpTable[jOffset + i] < 0)
                jumpTable[jOffset + i] = scriptCodePos - scriptCodeOffset;
        }
        return true;
    }

    return false;
}
void Compilerv3::CopyAliasStr(char *dest, char *text, bool arrayIndex)
{
    int textPos     = 0;
    int destPos     = 0;
    bool arrayValue = false;
    if (arrayIndex) {
        while (text[textPos]) {
            if (arrayValue) {
                if (text[textPos] == ']')
                    arrayValue = false;
                else
                    dest[destPos++] = text[textPos];
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
        while (text[textPos]) {
            if (arrayValue) {
                if (text[textPos] == ']')
                    arrayValue = false;
                ++textPos;
            }
            else {
                if (text[textPos] == '[')
                    arrayValue = true;
                else
                    dest[destPos++] = text[textPos];
                ++textPos;
            }
        }
    }
    dest[destPos] = 0;
}

void Compilerv3::ParseScriptFile(QString scriptName, int scriptID, bool inEditor)
{
    this->inEditor = inEditor;

    jumpTableStackPos = 0;
    lineID            = 0;
    aliasCount        = COMMONALIAS_COUNT_v3;
    for (int i = COMMONALIAS_COUNT_v3; i < ALIAS_COUNT_v3; ++i) {
        StrCopy(aliases[i].name, "");
        StrCopy(aliases[i].value, "");
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
            int textPos = 0;
            readMode    = READMODE_NORMAL;

            while (readMode < READMODE_ENDLINE) {
                prevChar = curChar;
                curChar  = reader.read<char>();

                if (readMode == READMODE_STRING) {
                    if (curChar == '\t' || curChar == '\r' || curChar == '\n' || curChar == ';'
                        || readMode >= READMODE_COMMENTLINE) {
                        if ((curChar == '\n' && prevChar != '\r')
                            || (curChar == '\n' && prevChar == '\r')) {
                            readMode            = READMODE_ENDLINE;
                            scriptText[textPos] = 0;
                        }
                    }
                    else if (curChar != '/' || textPos <= 0) {
                        scriptText[textPos++] = curChar;
                        if (curChar == '"')
                            readMode = READMODE_NORMAL;
                    }
                    else if (curChar == '/' && prevChar == '/') {
                        readMode              = READMODE_COMMENTLINE;
                        scriptText[--textPos] = 0;
                    }
                    else {
                        scriptText[textPos++] = curChar;
                    }
                }
                else if (curChar == ' ' || curChar == '\t' || curChar == '\r' || curChar == '\n'
                         || curChar == ';' || readMode >= READMODE_COMMENTLINE) {
                    if ((curChar == '\n' && prevChar != '\r') || (curChar == '\n' && prevChar == '\r')
                        || curChar == ';') {
                        // don't read commas as endline in comments
                        if (readMode < READMODE_COMMENTLINE || curChar != ';') {
                            readMode            = READMODE_ENDLINE;
                            scriptText[textPos] = 0;
                        }
                    }
                }
                else if (curChar != '/' || textPos <= 0) {
                    scriptText[textPos++] = curChar;
                    if (curChar == '"' && !readMode)
                        readMode = READMODE_STRING;
                }
                else if (curChar == '/' && prevChar == '/') {
                    readMode              = READMODE_COMMENTLINE;
                    scriptText[--textPos] = 0;
                }
                else {
                    scriptText[textPos++] = curChar;
                }

                if (reader.isEOF()) {
                    scriptText[textPos] = 0;
                    readMode            = READMODE_EOF;
                }
            }

            switch (parseMode) {
                case PARSEMODE_SCOPELESS:
                    ++lineID;
                    CheckAliasText(scriptText);

                    if (inEditor) {
                        if (StrComp(scriptText, "subRSDKDraw")) {
                            parseMode                                            = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subRSDKDraw.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].subRSDKDraw.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                     = scriptCodePos;
                            jumpTableOffset                                      = jumpTablePos;
                        }

                        if (StrComp(scriptText, "subRSDKLoad")) {
                            parseMode                                            = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subRSDKLoad.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].subRSDKLoad.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                     = scriptCodePos;
                            jumpTableOffset                                      = jumpTablePos;
                        }

                        if (StrComp(scriptText, "subRSDKEdit")) {
                            parseMode                                            = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subRSDKEdit.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].subRSDKEdit.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                     = scriptCodePos;
                            jumpTableOffset                                      = jumpTablePos;
                        }
                    }
                    else {
                        if (StrComp(scriptText, "subObjectMain")) {
                            parseMode                                        = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subMain.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].subMain.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                 = scriptCodePos;
                            jumpTableOffset                                  = jumpTablePos;
                        }

                        if (StrComp(scriptText, "subObjectPlayerInteraction")) {
                            parseMode = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subPlayerInteraction.scriptCodePtr =
                                scriptCodePos;
                            objectScriptList[scriptID].subPlayerInteraction.jumpTablePtr = jumpTablePos;
                            scriptCodeOffset = scriptCodePos;
                            jumpTableOffset  = jumpTablePos;
                        }

                        if (StrComp(scriptText, "subObjectDraw")) {
                            parseMode                                        = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subDraw.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].subDraw.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                 = scriptCodePos;
                            jumpTableOffset                                  = jumpTablePos;
                        }

                        if (StrComp(scriptText, "subObjectStartup")) {
                            parseMode                                           = PARSEMODE_FUNCTION;
                            objectScriptList[scriptID].subStartup.scriptCodePtr = scriptCodePos;
                            objectScriptList[scriptID].subStartup.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                                    = scriptCodePos;
                            jumpTableOffset                                     = jumpTablePos;
                        }
                    }

                    if (FindStringToken(scriptText, "function", 1) == 0) { // regular decl.
                        char funcName[0x20];
                        for (textPos = 8; scriptText[textPos]; ++textPos)
                            funcName[textPos - 8] = scriptText[textPos];
                        funcName[textPos - 8] = 0;

                        int funcID = -1;
                        for (int f = 0; f < functionCount; ++f) {
                            if (StrComp(funcName, functionList[f].name))
                                funcID = f;
                        }

                        if (funcID <= -1) {
                            if (functionCount >= FUNCTION_COUNT_v3) {
                                parseMode = PARSEMODE_SCOPELESS;
                            }
                            else {
                                StrCopy(functionList[functionCount].name, funcName);
                                functionList[functionCount].ptr.scriptCodePtr = scriptCodePos;
                                functionList[functionCount].ptr.jumpTablePtr  = jumpTablePos;
                                scriptCodeOffset                              = scriptCodePos;
                                jumpTableOffset                               = jumpTablePos;
                                parseMode                                     = PARSEMODE_FUNCTION;
                                ++functionCount;
                            }
                        }
                        else {
                            StrCopy(functionList[funcID].name, funcName);
                            functionList[funcID].ptr.scriptCodePtr = scriptCodePos;
                            functionList[funcID].ptr.jumpTablePtr  = jumpTablePos;
                            scriptCodeOffset                       = scriptCodePos;
                            jumpTableOffset                        = jumpTablePos;
                            parseMode                              = PARSEMODE_FUNCTION;
                        }
                    }
                    else if (FindStringToken(scriptText, "function", 1) == 1) { // forward decl.
                        char funcName[0x20];
                        for (textPos = 9; scriptText[textPos]; ++textPos)
                            funcName[textPos - 9] = scriptText[textPos];
                        funcName[textPos - 9] = 0;

                        int funcID = -1;
                        for (int f = 0; f < functionCount; ++f) {
                            if (StrComp(funcName, functionList[f].name))
                                funcID = f;
                        }

                        if (functionCount < FUNCTION_COUNT_v3 && funcID == -1) {
                            StrCopy(functionList[functionCount++].name, funcName);
                        }

                        parseMode = PARSEMODE_SCOPELESS;
                    }
                    break;

                case PARSEMODE_PLATFORMSKIP:
                    ++lineID;

                    if (FindStringToken(scriptText, "#endplatform", 1) == 0)
                        parseMode = PARSEMODE_FUNCTION;
                    break;

                case PARSEMODE_FUNCTION:
                    ++lineID;

                    if (scriptText[0]) {
                        if (StrComp(scriptText, "endsub")) {
                            scriptCode[scriptCodePos++] = FUNC_END;
                            parseMode                   = PARSEMODE_SCOPELESS;
                        }
                        else if (StrComp(scriptText, "endfunction")) {
                            scriptCode[scriptCodePos++] = FUNC_ENDFUNCTION;
                            parseMode                   = PARSEMODE_SCOPELESS;
                        }
                        else if (FindStringToken(scriptText, "#platform:", 1) == 0) {
                            if (FindStringToken(scriptText, gamePlatform, 1) == -1
                                && FindStringToken(scriptText, gameRenderType, 1) == -1
                                && FindStringToken(scriptText, gameHapticSetting, 1) == -1
                                && FindStringToken(scriptText, releaseType, 1) == -1
                                && FindStringToken(scriptText, "Use_Decomp", 1)
                                       == -1) { // if NONE of these checks succeeded, then we skip
                                                // everything until "end
                                // platform"
                                parseMode = PARSEMODE_PLATFORMSKIP;
                            }
                        }
                        else if (FindStringToken(scriptText, "#endplatform", 1) == -1) {
                            ConvertIfWhileStatement(scriptText);

                            if (ConvertSwitchStatement(scriptText)) {
                                parseMode  = PARSEMODE_SWITCHREAD;
                                storePos   = (int)reader.tell();
                                switchDeep = 0;
                            }

                            ConvertArithmaticSyntax(scriptText);

                            if (!ReadSwitchCase(scriptText)) {
                                ConvertFunctionText(scriptText);

                                if (scriptError) {
                                    errorScr  = scriptName;
                                    parseMode = PARSEMODE_ERROR;
                                }
                            }
                        }
                    }
                    break;

                case PARSEMODE_SWITCHREAD:
                    if (FindStringToken(scriptText, "switch", 1) == 0)
                        ++switchDeep;

                    if (switchDeep) {
                        if (FindStringToken(scriptText, "endswitch", 1) == 0)
                            --switchDeep;
                    }
                    else if (FindStringToken(scriptText, "endswitch", 1) == 0) {
                        reader.seek(storePos);
                        parseMode  = PARSEMODE_FUNCTION;
                        int jPos   = jumpTableStack[jumpTableStackPos];
                        switchDeep = abs(jumpTable[jPos + 1] - jumpTable[jPos]) + 1;
                        for (textPos = 0; textPos < switchDeep; ++textPos)
                            jumpTable[jumpTablePos++] = -1;
                    }
                    else {
                        CheckCaseNumber(scriptText);
                    }
                    break;

                default: break;
            }
        }

        reader.close();
    }
}

void Compilerv3::ClearScriptData()
{
    memset(scriptCode, 0, SCRIPTCODE_COUNT_v3 * sizeof(int));
    memset(jumpTable, 0, JUMPTABLE_COUNT_v3 * sizeof(int));
    memset(functionStack, 0, FUNCSTACK_COUNT_v3 * sizeof(int));

    globalVariables.clear();

    jumpTableStackPos = 0;

    scriptCodePos    = 0;
    scriptCodeOffset = 0;
    jumpTablePos     = 0;
    jumpTableOffset  = 0;

    functionCount = 0;

    for (int o = 0; o < OBJECT_COUNT_v3; ++o) {
        ObjectScript *scriptInfo = &objectScriptList[o];

        scriptInfo->subMain.scriptCodePtr              = SCRIPTCODE_COUNT_v3 - 1;
        scriptInfo->subMain.jumpTablePtr               = JUMPTABLE_COUNT_v3 - 1;
        scriptInfo->subPlayerInteraction.scriptCodePtr = SCRIPTCODE_COUNT_v3 - 1;
        scriptInfo->subPlayerInteraction.jumpTablePtr  = JUMPTABLE_COUNT_v3 - 1;
        scriptInfo->subDraw.scriptCodePtr              = SCRIPTCODE_COUNT_v3 - 1;
        scriptInfo->subDraw.jumpTablePtr               = JUMPTABLE_COUNT_v3 - 1;
        scriptInfo->subStartup.scriptCodePtr           = SCRIPTCODE_COUNT_v3 - 1;
        scriptInfo->subStartup.jumpTablePtr            = JUMPTABLE_COUNT_v3 - 1;

        scriptInfo->subRSDKDraw.scriptCodePtr = SCRIPTCODE_COUNT_v3 - 1;
        scriptInfo->subRSDKDraw.jumpTablePtr  = JUMPTABLE_COUNT_v3 - 1;
        scriptInfo->subRSDKLoad.scriptCodePtr = SCRIPTCODE_COUNT_v3 - 1;
        scriptInfo->subRSDKLoad.jumpTablePtr  = JUMPTABLE_COUNT_v3 - 1;
        scriptInfo->subRSDKEdit.scriptCodePtr = SCRIPTCODE_COUNT_v3 - 1;
        scriptInfo->subRSDKEdit.jumpTablePtr  = JUMPTABLE_COUNT_v3 - 1;

        typeNames[o][0] = 0;
    }

    for (int f = 0; f < FUNCTION_COUNT_v3; ++f) {
        functionList[f].ptr.scriptCodePtr = SCRIPTCODE_COUNT_v3 - 1;
        functionList[f].ptr.jumpTablePtr  = JUMPTABLE_COUNT_v3 - 1;
    }
    globalSfxNames.clear();
    stageSfxNames.clear();

    SetScriptTypeName("Blank Object", typeNames[0]);
}

void Compilerv3::WriteBytecode(QString path)
{
    bytecode = RSDKv3::Bytecode();

    bytecode.scriptCode.clear();
    for (int i = globalScriptCodeCount; i < scriptCodePos; ++i) {
        bytecode.scriptCode.append(scriptCode[i]);
    }

    bytecode.jumpTable.clear();
    for (int i = globalJumpTableCount; i < jumpTablePos; ++i) {
        bytecode.jumpTable.append(jumpTable[i]);
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
        func.scriptCodePos = functionList[f].ptr.scriptCodePtr;
        func.jumpTablePos  = functionList[f].ptr.jumpTablePtr;
        bytecode.functionList.append(func);
    }

    bytecode.write(path);
}

void Compilerv3::ProcessScript(int scriptCodeStart, int jumpTableStart, byte scriptSub)
{
    bool running        = true;
    int scriptCodePtr   = scriptCodeStart;
    jumpTableStackPos   = 0;
    functionStackPos    = 0;
    SceneEditor *editor = (SceneEditor *)this->editor;

    while (running) {
        int opcode           = scriptCode[scriptCodePtr++];
        int opcodeSize       = opcodeFunctionList[opcode].opcodeSize;
        int scriptCodeOffset = scriptCodePtr;
        // Get Values
        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = scriptCode[scriptCodePtr++];

            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (scriptCode[scriptCodePtr++]) {
                    case VARARR_NONE: arrayVal = objectLoop; break;

                    case VARARR_ARRAY:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal = scriptEng.arrayPosition[scriptCode[scriptCodePtr++]];
                        else
                            arrayVal = scriptCode[scriptCodePtr++];
                        break;

                    case VARARR_ENTNOPLUS1:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal =
                                scriptEng.arrayPosition[scriptCode[scriptCodePtr++]] + objectLoop;
                        else
                            arrayVal = scriptCode[scriptCodePtr++] + objectLoop;
                        break;

                    case VARARR_ENTNOMINUS1:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal =
                                objectLoop - scriptEng.arrayPosition[scriptCode[scriptCodePtr++]];
                        else
                            arrayVal = objectLoop - scriptCode[scriptCodePtr++];
                        break;

                    default: break;
                }

                // Allows the game to link to the editor properly
                Entity *entityPtr = &objectEntityList[arrayVal];
                if (editor) {
                    for (int e = 0; e < editor->viewer->entities.count(); ++e) {
                        if (arrayVal == editor->viewer->entities[e].slotID) {
                            entityPtr = &objectEntityList[editor->viewer->entities[e].gameEntitySlot];
                        }
                    }
                }

                // Variables
                switch (scriptCode[scriptCodePtr++]) {
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
                    case VAR_OBJECTOUTOFBOUNDS: {
                        /*int pos = entityPtr->XPos >> 16;
                        if (pos <= xScrollOffset - OBJECT_BORDER_X1 || pos >= OBJECT_BORDER_X2 +
                        xScrollOffset) { scriptEng.operands[i] = 1;
                        }
                        else {
                            int pos               = entityPtr->YPos >> 16;
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
                        scriptEng.operands[i] = objectScriptList[entityPtr->type].spriteSheetID;
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
                    case VAR_EDITORSHOWGIZMOS:
                        scriptEng.operands[i] = scnEditor->viewer->sceneInfo.effectGizmo
                                                || scnEditor->viewer->selectedEntity == objectLoop;
                        break;
                    case VAR_EDITORDRAWINGOVERLAY:
                        scriptEng.operands[i] = scnEditor->viewer->sceneInfo.debugMode;
                        break;
                }
            }
            else if (opcodeType == SCRIPTVAR_INTCONST) { // int constant
                scriptEng.operands[i] = scriptCode[scriptCodePtr++];
            }
            else if (opcodeType == SCRIPTVAR_STRCONST) { // string constant
                int strLen         = scriptCode[scriptCodePtr++];
                scriptText[strLen] = 0;
                for (int c = 0; c < strLen; ++c) {
                    switch (c % 4) {
                        case 0: scriptText[c] = scriptCode[scriptCodePtr] >> 24; break;

                        case 1: scriptText[c] = (0xFFFFFF & scriptCode[scriptCodePtr]) >> 16; break;

                        case 2: scriptText[c] = (0xFFFF & scriptCode[scriptCodePtr]) >> 8; break;

                        case 3: scriptText[c] = scriptCode[scriptCodePtr++]; break;

                        default: break;
                    }
                }
                scriptCodePtr++;
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
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFGREATER:
                if (scriptEng.operands[1] <= scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFGREATEROREQUAL:
                if (scriptEng.operands[1] < scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFLOWER:
                if (scriptEng.operands[1] >= scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFLOWEROREQUAL:
                if (scriptEng.operands[1] > scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_IFNOTEQUAL:
                if (scriptEng.operands[1] == scriptEng.operands[2])
                    scriptCodePtr = scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0]];
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize                          = 0;
                break;
            case FUNC_ELSE:
                opcodeSize    = 0;
                scriptCodePtr = scriptCodeStart
                                + jumpTable[jumpTableStart + jumpTableStack[jumpTableStackPos--] + 1];
                break;
            case FUNC_ENDIF:
                opcodeSize = 0;
                --jumpTableStackPos;
                break;
            case FUNC_WEQUAL:
                if (scriptEng.operands[1] != scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WGREATER:
                if (scriptEng.operands[1] <= scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WGREATEROREQUAL:
                if (scriptEng.operands[1] < scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WLOWER:
                if (scriptEng.operands[1] >= scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WLOWEROREQUAL:
                if (scriptEng.operands[1] > scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_WNOTEQUAL:
                if (scriptEng.operands[1] == scriptEng.operands[2])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 1];
                else
                    jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                opcodeSize = 0;
                break;
            case FUNC_LOOP:
                opcodeSize = 0;
                scriptCodePtr =
                    scriptCodeStart + jumpTable[jumpTableStart + jumpTableStack[jumpTableStackPos--]];
                break;
            case FUNC_SWITCH:
                jumpTableStack[++jumpTableStackPos] = scriptEng.operands[0];
                if (scriptEng.operands[1] < jumpTable[jumpTableStart + scriptEng.operands[0]]
                    || scriptEng.operands[1] > jumpTable[jumpTableStart + scriptEng.operands[0] + 1])
                    scriptCodePtr =
                        scriptCodeStart + jumpTable[jumpTableStart + scriptEng.operands[0] + 2];
                else
                    scriptCodePtr =
                        scriptCodeStart
                        + jumpTable[jumpTableStart + scriptEng.operands[0] + 4
                                    + (scriptEng.operands[1]
                                       - jumpTable[jumpTableStart + scriptEng.operands[0]])];
                opcodeSize = 0;
                break;
            case FUNC_BREAK:
                opcodeSize    = 0;
                scriptCodePtr = scriptCodeStart
                                + jumpTable[jumpTableStart + jumpTableStack[jumpTableStackPos--] + 3];
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
                scriptInfo->spriteSheetID = editor->LoadSpriteSheet(scriptText);
                break;
            case FUNC_REMOVESPRITESHEET:
                opcodeSize = 0;
                editor->viewer->removeGraphicsFile(scriptText, -1);
                break;
            case FUNC_DRAWSPRITE:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->DrawSpriteFlipped((entity->XPos >> 16) + spriteFrame->pivotX,
                                          (entity->YPos >> 16) + spriteFrame->pivotY,
                                          spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                          spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                          scriptInfo->spriteSheetID, false);
                break;
            case FUNC_DRAWSPRITEXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->DrawSpriteFlipped((scriptEng.operands[1] >> 16) + spriteFrame->pivotX,
                                          (scriptEng.operands[2] >> 16) + spriteFrame->pivotY,
                                          spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                          spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                          scriptInfo->spriteSheetID, false);
                break;
            case FUNC_DRAWSPRITESCREENXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->DrawSpriteFlipped(scriptEng.operands[1] + spriteFrame->pivotX,
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
                        editor->DrawSpriteFlipped(spriteFrame->pivotX + scriptEng.operands[1],
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
                            editor->DrawSpriteFlipped(spriteFrame->pivotX + scriptEng.operands[1],
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
                if (scriptSub == SUB_RSDKLOAD && scriptFrameCount < SPRITEFRAME_COUNT_v3) {
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
                        editor->DrawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16), (scriptEng.operands[3] >> 16),
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->width,
                            spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, entity->scale,
                            entity->scale, entity->direction, 0, INK_NONE, 0xFF,
                            scriptInfo->spriteSheetID, false);
                        break;
                    case FX_ROTATE:
                        editor->DrawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16), (scriptEng.operands[3] >> 16),
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->width,
                            spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, 0x200, 0x200,
                            entity->direction, entity->rotation, INK_NONE, 0xFF,
                            scriptInfo->spriteSheetID, false);
                        break;
                    case FX_ROTOZOOM:
                        editor->DrawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16), (scriptEng.operands[3] >> 16),
                            -spriteFrame->pivotX, -spriteFrame->pivotY, spriteFrame->width,
                            spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, entity->scale,
                            entity->scale, entity->direction, entity->rotation, INK_NONE, 0xFF,
                            scriptInfo->spriteSheetID, false);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                editor->DrawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_BLEND:
                                editor->DrawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_BLEND, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_ALPHA:
                                editor->DrawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_ALPHA, entity->alpha,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_ADD:
                                editor->DrawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_ADD, entity->alpha,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case INK_SUB:
                                editor->DrawSpriteFlipped(
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
                                editor->DrawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case FLIP_X:
                                editor->DrawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) - spriteFrame->width
                                        - spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_X, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case FLIP_Y:
                                editor->DrawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) - spriteFrame->height
                                        - spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_Y, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID, false);
                                break;
                            case FLIP_XY:
                                editor->DrawSpriteFlipped(
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
                        editor->DrawSpriteRotozoom(
                            scriptEng.operands[2], scriptEng.operands[3], -spriteFrame->pivotX,
                            -spriteFrame->pivotY, spriteFrame->width, spriteFrame->height,
                            spriteFrame->sprX, spriteFrame->sprY, entity->scale, entity->scale,
                            entity->direction, 0, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        break;
                    case FX_ROTATE:
                        editor->DrawSpriteRotozoom(
                            scriptEng.operands[2], scriptEng.operands[3], -spriteFrame->pivotX,
                            -spriteFrame->pivotY, spriteFrame->width, spriteFrame->height,
                            spriteFrame->sprX, spriteFrame->sprY, 0x200, 0x200, entity->direction,
                            entity->rotation, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        break;
                    case FX_ROTOZOOM:
                        editor->DrawSpriteRotozoom(scriptEng.operands[2], scriptEng.operands[3],
                                                   -spriteFrame->pivotX, -spriteFrame->pivotY,
                                                   spriteFrame->width, spriteFrame->height,
                                                   spriteFrame->sprX, spriteFrame->sprY, entity->scale,
                                                   entity->scale, entity->direction, entity->rotation,
                                                   INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                editor->DrawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case INK_BLEND:
                                editor->DrawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_BLEND, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case INK_ALPHA:
                                editor->DrawSpriteFlipped(scriptEng.operands[2] + spriteFrame->pivotX,
                                                          scriptEng.operands[3] + spriteFrame->pivotY,
                                                          spriteFrame->width, spriteFrame->height,
                                                          spriteFrame->sprX, spriteFrame->sprY,
                                                          FLIP_NONE, INK_ALPHA, entity->alpha,
                                                          scriptInfo->spriteSheetID, true);
                                break;
                            case INK_ADD:
                                editor->DrawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_ADD, entity->alpha, scriptInfo->spriteSheetID, true);
                                break;
                            case INK_SUB:
                                editor->DrawSpriteFlipped(
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
                                editor->DrawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case FLIP_X:
                                editor->DrawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_X,
                                    INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case FLIP_Y:
                                editor->DrawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_Y,
                                    INK_NONE, 0xFF, scriptInfo->spriteSheetID, true);
                                break;
                            case FLIP_XY:
                                editor->DrawSpriteFlipped(
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
                functionStack[functionStackPos++] = scriptCodePtr;
                functionStack[functionStackPos++] = jumpTableStart;
                functionStack[functionStackPos++] = scriptCodeStart;
                scriptCodeStart = functionList[scriptEng.operands[0]].ptr.scriptCodePtr;
                jumpTableStart  = functionList[scriptEng.operands[0]].ptr.jumpTablePtr;
                scriptCodePtr   = scriptCodeStart;
                break;
                break;
            case FUNC_ENDFUNCTION:
                opcodeSize      = 0;
                scriptCodeStart = functionStack[--functionStackPos];
                jumpTableStart  = functionStack[--functionStackPos];
                scriptCodePtr   = functionStack[--functionStackPos];
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
                    editor->viewer->addEditorVariable(scriptText);
                }
                break;
            }
            case FUNC_SETACTIVEVAR: {
                opcodeSize = 0;
                if (scriptSub == SUB_RSDKLOAD) {
                    editor->viewer->setActiveVariable(scriptText);
                }
                break;
            }
            case FUNC_ADDENUMVAR: {
                opcodeSize = 0;
                if (scriptSub == SUB_RSDKLOAD) {
                    editor->viewer->addEnumVariable(scriptText, scriptEng.operands[1]);
                }
                break;
            }
            case FUNC_SETVARALIAS: {
                opcodeSize = 0;
                if (scriptSub == SUB_RSDKLOAD) {
                    editor->viewer->setVariableAlias(scriptEng.operands[0], scriptText);
                }
                break;
            }
            case FUNC_DRAWLINE: {
                opcodeSize = 0;

                Vector4<float> color =
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, 1.0);

                editor->viewer->drawLine((scriptEng.operands[0] >> 16) - editor->viewer->cameraPos.x,
                                         (scriptEng.operands[1] >> 16) - editor->viewer->cameraPos.y,
                                         (scriptEng.operands[2] >> 16) - editor->viewer->cameraPos.x,
                                         (scriptEng.operands[3] >> 16) - editor->viewer->cameraPos.y,
                                         color, entity->alpha, (InkEffects)entity->inkEffect);
                break;
            }
            case FUNC_DRAWARROW: {
                opcodeSize = 0;

                int angle = ArcTanLookup(scriptEng.operands[0] - scriptEng.operands[2],
                                         scriptEng.operands[1] - scriptEng.operands[3]);
                Vector4<float> color =
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, 1.0);
                InkEffects ink = (InkEffects)entity->inkEffect;

                int x1 = (scriptEng.operands[0] >> 16) - editor->viewer->cameraPos.x;
                int y1 = (scriptEng.operands[1] >> 16) - editor->viewer->cameraPos.y;
                int x2 = (scriptEng.operands[2] >> 16) - editor->viewer->cameraPos.x;
                int y2 = (scriptEng.operands[3] >> 16) - editor->viewer->cameraPos.y;

                editor->viewer->drawLine(x1, y1, x2, y2, color, entity->alpha, ink);
                editor->viewer->drawLine(x2, y2, x2 + ((cos256(angle + 12) << 12) >> 16),
                                         y2 + ((sin256(angle + 12) << 12) >> 16), color, entity->alpha,
                                         ink);
                editor->viewer->drawLine(x2, y2, x2 + ((cos256(angle - 12) << 12) >> 16),
                                         y2 + ((sin256(angle - 12) << 12) >> 16), color, entity->alpha,
                                         ink);
                break;
            }
            case FUNC_DRAWRECTWORLD: {
                opcodeSize = 0;
                int x      = (scriptEng.operands[0] >> 16) - editor->viewer->cameraPos.x;
                int y      = (scriptEng.operands[1] >> 16) - editor->viewer->cameraPos.y;

                editor->viewer->drawRect(
                    x, y, scriptEng.operands[2], scriptEng.operands[3],
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, scriptEng.operands[7] / 255.0f),
                    false, 0xFF, INK_NONE);
                break;
            }
            case FUNC_DRAWRECTOUTLINE: {
                opcodeSize = 0;

                int x = (scriptEng.operands[0] >> 16) - editor->viewer->cameraPos.x;
                int y = (scriptEng.operands[1] >> 16) - editor->viewer->cameraPos.y;

                editor->viewer->drawRect(
                    x, y, scriptEng.operands[2], scriptEng.operands[3],
                    Vector4<float>(scriptEng.operands[4] / 255.0f, scriptEng.operands[5] / 255.0f,
                                   scriptEng.operands[6] / 255.0f, scriptEng.operands[7] / 255.0f),
                    true, 0xFF, INK_NONE);
                break;
            }
            case FUNC_GETOBJECTTYPE: {
                char typeName[0x40];
                int objPos  = 0;
                int typePos = 0;
                while (scriptText[objPos]) {
                    if (scriptText[objPos] != ' ')
                        typeName[typePos++] = scriptText[objPos];
                    ++objPos;
                }
                typeName[typePos] = 0;

                scriptEng.operands[0] = -1;
                for (int o = 0; o < OBJECT_COUNT_v3; ++o) {
                    if (StrComp(typeName, typeNames[o])) {
                        scriptEng.operands[0] = o;
                    }
                }
                break;
            }
            case FUNC_CHECKCURRENTSTAGEFOLDER:
                opcodeSize            = 0;
                scriptEng.checkResult = editor->viewer->currentFolder == scriptText;

                if (!scriptEng.checkResult) {
                    int targetSize  = editor->viewer->currentFolder.size();
                    int currentSize = strlen(scriptText);
                    if (targetSize > currentSize) {
                        scriptEng.checkResult = editor->viewer->currentFolder.endsWith(scriptText);
                    }
                }
                break;
        }

        // Set Values
        if (opcodeSize > 0)
            scriptCodePtr -= scriptCodePtr - scriptCodeOffset;
        for (int i = 0; i < opcodeSize; ++i) {
            int opcodeType = scriptCode[scriptCodePtr++];
            if (opcodeType == SCRIPTVAR_VAR) {
                int arrayVal = 0;
                switch (scriptCode[scriptCodePtr++]) { // variable
                    case VARARR_NONE: arrayVal = objectLoop; break;
                    case VARARR_ARRAY:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal = scriptEng.arrayPosition[scriptCode[scriptCodePtr++]];
                        else
                            arrayVal = scriptCode[scriptCodePtr++];
                        break;

                    case VARARR_ENTNOPLUS1:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal =
                                scriptEng.arrayPosition[scriptCode[scriptCodePtr++]] + objectLoop;
                        else
                            arrayVal = scriptCode[scriptCodePtr++] + objectLoop;
                        break;

                    case VARARR_ENTNOMINUS1:
                        if (scriptCode[scriptCodePtr++] == 1)
                            arrayVal =
                                objectLoop - scriptEng.arrayPosition[scriptCode[scriptCodePtr++]];
                        else
                            arrayVal = objectLoop - scriptCode[scriptCodePtr++];
                        break;

                    default: break;
                }

                // Allows the game to link to the editor properly
                Entity *entityPtr = &objectEntityList[arrayVal];
                if (editor) {
                    for (int e = 0; e < editor->viewer->entities.count(); ++e) {
                        if (arrayVal == editor->viewer->entities[e].slotID) {
                            entityPtr = &objectEntityList[editor->viewer->entities[e].gameEntitySlot];
                        }
                    }
                }

                // Variables
                switch (scriptCode[scriptCodePtr++]) {
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
                        objectScriptList[entityPtr->type].spriteSheetID = scriptEng.operands[i];
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
                    case VAR_EDITORVARIABLEVAL:
                        scnEditor->viewer->variableValue = scriptEng.operands[i];
                        break;
                    case VAR_EDITORRETURNVAR: break;
                    case VAR_EDITORSHOWGIZMOS: break;
                    case VAR_EDITORDRAWINGOVERLAY:
                        scnEditor->viewer->sceneInfo.debugMode = scriptEng.operands[i];
                        break;
                }
            }
            else if (opcodeType == SCRIPTVAR_INTCONST) { // int constant
                scriptCodePtr++;
            }
            else if (opcodeType == SCRIPTVAR_STRCONST) { // string constant
                int strLen = scriptCode[scriptCodePtr++];
                for (int c = 0; c < strLen; ++c) {
                    switch (c % 4) {
                        case 0: break;
                        case 1: break;
                        case 2: break;
                        case 3: ++scriptCodePtr; break;
                        default: break;
                    }
                }
                scriptCodePtr++;
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
