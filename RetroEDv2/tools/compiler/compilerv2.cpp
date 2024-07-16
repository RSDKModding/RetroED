#include "includes.hpp"
#include "compilerv2.hpp"

#include "tools/sceneeditor.hpp"


struct AliasInfov2 {
    AliasInfov2()
    {
        name  = "";
        value = "";
    }
    AliasInfov2(QString aliasName, QString aliasVal)
    {
        name  = aliasName;
        value = aliasVal;
    }

    QString name  = "";
    QString value = "";
};

struct FunctionInfov2 {
    FunctionInfov2()
    {
        name       = "";
        opcodeSize = 0;
    }
    FunctionInfov2(QString functionName, int opSize)
    {
        name       = functionName;
        opcodeSize = opSize;
    }

    QString name   = "";
    int opcodeSize = 0;
};

const QString variableNames[] = {
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
    "Object.Frame",
    "Object.Value0",
    "Object.Value1",
    "Object.Value2",
    "Object.Value3",
    "Object.Value4",
    "Object.Value5",
    "Object.Value6",
    "Object.Value7",
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
    "Stage.ActiveList",
    "Stage.ListPos",
    "XScrollOffset",
    "YScrollOffset",
    "Global",
    "Stage.TimeEnabled",
    "Stage.MilliSeconds",
    "Stage.Seconds",
    "Stage.Minutes",
    "Stage.ActNo",
    "Object.EntityNo",
    "Player.Type",
    "Player.State",
    "Player.ControlMode",
    "Player.CollisionMode",
    "Player.CollisionPlane",
    "Player.XPos",
    "Player.YPos",
    "Player.ScreenXPos",
    "Player.ScreenYPos",
    "Player.Speed",
    "Player.XVelocity",
    "Player.YVelocity",
    "Player.Gravity",
    "Player.Angle",
    "Player.Rotation",
    "Player.Direction",
    "Player.Animation",
    "Player.Frame",
    "Player.Skidding",
    "Player.Pushing",
    "Player.FrictionLoss",
    "Player.WalkingSpeed",
    "Player.RunningSpeed",
    "Player.JumpingSpeed",
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
    "Player.RollingAcceleration",
    "Player.RollingDeceleration",
    "Player.EntityNo",
    "Player.CollisionLeft",
    "Player.CollisionTop",
    "Player.CollisionRight",
    "Player.CollisionBottom",
    "Player.Flailing",
    "Stage.PauseEnabled",
    "Stage.ListSize",
    "Player.Timer",
    "Player.AnimationSpeed",
    "Player.TileCollisions",
    "Player.ObjectInteraction",
    "Stage.CameraEnabled",
    "Stage.CameraStyle",
    "Music.Volume",
    "Music.CurrentTrack",
    "Player.Visible",
    "Stage.NewXBoundary1",
    "Stage.NewXBoundary2",
    "Stage.NewYBoundary1",
    "Stage.NewYBoundary2",
    "Stage.XBoundary1",
    "Stage.XBoundary2",
    "Stage.YBoundary1",
    "Stage.YBoundary2",
    "Object.OutOfBounds",
};

const FunctionInfov2 functions[] = { FunctionInfov2("End", 0),
                                     FunctionInfov2("Equal", 2),
                                     FunctionInfov2("Add", 2),
                                     FunctionInfov2("Sub", 2),
                                     FunctionInfov2("Inc", 1),
                                     FunctionInfov2("Dec", 1),
                                     FunctionInfov2("Mul", 2),
                                     FunctionInfov2("Div", 2),
                                     FunctionInfov2("ShR", 2),
                                     FunctionInfov2("ShL", 2),
                                     FunctionInfov2("And", 2),
                                     FunctionInfov2("Or", 2),
                                     FunctionInfov2("Xor", 2),
                                     FunctionInfov2("Not", 1),
                                     FunctionInfov2("FlipSign", 1),
                                     FunctionInfov2("CheckEqual", 2),
                                     FunctionInfov2("CheckGreater", 2),
                                     FunctionInfov2("CheckLower", 2),
                                     FunctionInfov2("CheckNotEqual", 2),
                                     FunctionInfov2("IfEqual", 3),
                                     FunctionInfov2("IfGreater", 3),
                                     FunctionInfov2("IfGreaterOrEqual", 3),
                                     FunctionInfov2("IfLower", 3),
                                     FunctionInfov2("IfLowerOrEqual", 3),
                                     FunctionInfov2("IfNotEqual", 3),
                                     FunctionInfov2("else", 0),
                                     FunctionInfov2("endif", 0),
                                     FunctionInfov2("WEqual", 3),
                                     FunctionInfov2("WGreater", 3),
                                     FunctionInfov2("WGreaterOrEqual", 3),
                                     FunctionInfov2("WLower", 3),
                                     FunctionInfov2("WLowerOrEqual", 3),
                                     FunctionInfov2("WNotEqual", 3),
                                     FunctionInfov2("loop", 0),
                                     FunctionInfov2("switch", 2),
                                     FunctionInfov2("break", 0),
                                     FunctionInfov2("endswitch", 0),
                                     FunctionInfov2("Rand", 2),
                                     FunctionInfov2("Sin", 2),
                                     FunctionInfov2("Cos", 2),
                                     FunctionInfov2("Sin256", 2),
                                     FunctionInfov2("Cos256", 2),
                                     FunctionInfov2("SinChange", 5),
                                     FunctionInfov2("CosChange", 5),
                                     FunctionInfov2("ATan2", 3),
                                     FunctionInfov2("Interpolate", 4),
                                     FunctionInfov2("InterpolateXY", 7),
                                     FunctionInfov2("LoadSpriteSheet", 1),
                                     FunctionInfov2("RemoveSpriteSheet", 1),
                                     FunctionInfov2("DrawSprite", 1),
                                     FunctionInfov2("DrawSpriteXY", 3),
                                     FunctionInfov2("DrawSpriteScreenXY", 3),
                                     FunctionInfov2("DrawSprite3D", 1),
                                     FunctionInfov2("DrawNumbers", 7),
                                     FunctionInfov2("DrawActName", 7),
                                     FunctionInfov2("DrawMenu", 3),
                                     FunctionInfov2("SpriteFrame", 6),
                                     FunctionInfov2("SetDebugIcon", 6),
                                     FunctionInfov2("LoadPalette", 3),
                                     FunctionInfov2("RotatePalette", 3),
                                     FunctionInfov2("SetFade", 6),
                                     FunctionInfov2("SetWaterColor", 4),
                                     FunctionInfov2("SetBlendTable", 4),
                                     FunctionInfov2("SetTintTable", 6),
                                     FunctionInfov2("ClearScreen", 1),
                                     FunctionInfov2("DrawSpriteFX", 4),
                                     FunctionInfov2("DrawSpriteScreenFX", 4),
                                     FunctionInfov2("DrawLifeIcon", 2),
                                     FunctionInfov2("SetupMenu", 4),
                                     FunctionInfov2("AddMenuEntry", 3),
                                     FunctionInfov2("EditMenuEntry", 4),
                                     FunctionInfov2("LoadStage", 0),
                                     FunctionInfov2("DrawTintRect", 5),
                                     FunctionInfov2("ResetObjectEntity", 5),
                                     FunctionInfov2("PlayerObjectCollision", 5),
                                     FunctionInfov2("CreateTempObject", 4),
                                     FunctionInfov2("DefaultGroundMovement", 0),
                                     FunctionInfov2("DefaultAirMovement", 0),
                                     FunctionInfov2("DefaultRollingMovement", 0),
                                     FunctionInfov2("DefaultGravityTrue", 0),
                                     FunctionInfov2("DefaultGravityFalse", 0),
                                     FunctionInfov2("DefaultJumpAction", 0),
                                     FunctionInfov2("SetMusicTrack", 3),
                                     FunctionInfov2("PlayMusic", 1),
                                     FunctionInfov2("StopMusic", 0),
                                     FunctionInfov2("PlaySfx", 2),
                                     FunctionInfov2("StopSfx", 1),
                                     FunctionInfov2("SetSfxAttributes", 3),
                                     FunctionInfov2("ObjectTileCollision", 4),
                                     FunctionInfov2("ObjectTileGrip", 4),
                                     FunctionInfov2("LoadVideo", 1),
                                     FunctionInfov2("NextVideoFrame", 0),
                                     FunctionInfov2("PlayStageSfx", 2),
                                     FunctionInfov2("StopStageSfx", 1) };

AliasInfov2 aliasesv2[0x80] = {
    AliasInfov2("true", "1"),          AliasInfov2("false", "0"),
    AliasInfov2("FX_SCALE", "0"),      AliasInfov2("FX_ROTATE", "1"),
    AliasInfov2("FX_INK", "2"),        AliasInfov2("PRESENTATION_STAGE", "0"),
    AliasInfov2("REGULAR_STAGE", "1"), AliasInfov2("BONUS_STAGE", "2"),
    AliasInfov2("SPECIAL_STAGE", "3"), AliasInfov2("MENU_1", "0"),
    AliasInfov2("MENU_2", "1"),        AliasInfov2("C_TOUCH", "0"),
    AliasInfov2("C_BOX", "1"),         AliasInfov2("C_PLATFORM", "2"),
    AliasInfov2("INK_NONE", "0"),      AliasInfov2("INK_BLEND", "1"),
    AliasInfov2("INK_TINT", "2"),      AliasInfov2("FX_SCALE", "0"),
    AliasInfov2("FX_ROTATE", "1"),     AliasInfov2("FX_INK", "2"),
    AliasInfov2("FX_TINT", "3"),       AliasInfov2("FLIP_NONE", "0"),
    AliasInfov2("FLIP_X", "1"),        AliasInfov2("FLIP_Y", "2"),
    AliasInfov2("FLIP_XY", "3"),
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
    PARSEMODE_SCOPELESS  = 0,
    PARSEMODE_FUNCTION   = 1,
    PARSEMODE_SWITCHREAD = 2,
    PARSEMODE_ERROR      = 0xFF
};

enum ScriptVarTypes { SCRIPTVAR_VAR = 1, SCRIPTVAR_INTCONST = 2, SCRIPTVAR_STRCONST = 3 };
enum ScriptVarArrTypes {
    VARARR_NONE        = 0,
    VARARR_ARRAY       = 1,
    VARARR_ENTNOPLUS1  = 2,
    VARARR_ENTNOMINUS1 = 3
};

enum ScrVariable {
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
    VAR_OBJECTFRAME,
    VAR_OBJECTVALUE0,
    VAR_OBJECTVALUE1,
    VAR_OBJECTVALUE2,
    VAR_OBJECTVALUE3,
    VAR_OBJECTVALUE4,
    VAR_OBJECTVALUE5,
    VAR_OBJECTVALUE6,
    VAR_OBJECTVALUE7,
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
    VAR_STAGEACTIVELIST,
    VAR_STAGELISTPOS,
    VAR_XSCROLLOFFSET,
    VAR_YSCROLLOFFSET,
    VAR_GLOBAL,
    VAR_STAGETIMEENABLED,
    VAR_STAGEMILLISECONDS,
    VAR_STAGESECONDS,
    VAR_STAGEMINUTES,
    VAR_STAGEACTNO,
    VAR_OBJECTENTITYNO,
    VAR_PLAYERTYPE,
    VAR_PLAYERSTATE,
    VAR_PLAYERCONTROLMODE,
    VAR_PLAYERCOLLISIONMODE,
    VAR_PLAYERCOLLISIONPLANE,
    VAR_PLAYERXPOS,
    VAR_PLAYERYPOS,
    VAR_PLAYERSCREENXPOS,
    VAR_PLAYERSCREENYPOS,
    VAR_PLAYERSPEED,
    VAR_PLAYERXVELOCITY,
    VAR_PLAYERYVELOCITY,
    VAR_PLAYERGRAVITY,
    VAR_PLAYERANGLE,
    VAR_PLAYERROTATION,
    VAR_PLAYERDIRECTION,
    VAR_PLAYERANIMATION,
    VAR_PLAYERFRAME,
    VAR_PLAYERSKIDDING,
    VAR_PLAYERPUSHING,
    VAR_PLAYERFRICTIONLOSS,
    VAR_PLAYERWALKINGSPEED,
    VAR_PLAYERRUNNINGSPEED,
    VAR_PLAYERJUMPINGSPEED,
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
    VAR_PLAYERROLLINGACCELERATION,
    VAR_PLAYERROLLINGDECELERATION,
    VAR_PLAYERENTITYNO,
    VAR_PLAYERCOLLISIONLEFT,
    VAR_PLAYERCOLLISIONTOP,
    VAR_PLAYERCOLLISIONRIGHT,
    VAR_PLAYERCOLLISIONBOTTOM,
    VAR_PLAYERFLAILING,
    VAR_STAGEPAUSEENABLED,
    VAR_STAGELISTSIZE,
    VAR_PLAYERTIMER,
    VAR_PLAYERANIMATIONSPEED,
    VAR_PLAYERTILECOLLISIONS,
    VAR_PLAYEROBJECTINTERACTION,
    VAR_SCREENCAMERAENABLED,
    VAR_SCREENCAMERASTYLE,
    VAR_MUSICVOLUME,
    VAR_MUSICCURRENTTRACK,
    VAR_PLAYERVISIBLE,
    VAR_STAGENEWXBOUNDARY1,
    VAR_STAGENEWXBOUNDARY2,
    VAR_STAGENEWYBOUNDARY1,
    VAR_STAGENEWYBOUNDARY2,
    VAR_STAGEXBOUNDARY1,
    VAR_STAGEXBOUNDARY2,
    VAR_STAGEYBOUNDARY1,
    VAR_STAGEYBOUNDARY2,
    VAR_OBJECTOUTOFBOUNDS,
    VAR_MAX_CNT,
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
    FUNC_NOT,
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
    FUNC_DRAWSPRITE3D,
    FUNC_DRAWNUMBERS,
    FUNC_DRAWACTNAME,
    FUNC_DRAWMENU,
    FUNC_SPRITEFRAME,
    FUNC_SETDEBUGICON,
    FUNC_LOADPALETTE,
    FUNC_ROTATEPALETTE,
    FUNC_SETFADE,
    FUNC_SETWATERCOLOR,
    FUNC_SETBLENDTABLE,
    FUNC_SETTINTTABLE,
    FUNC_CLEARSCREEN,
    FUNC_DRAWSPRITEFX,
    FUNC_DRAWSPRITESCREENFX,
    FUNC_DRAWLIFEICON,
    FUNC_SETUPMENU,
    FUNC_ADDMENUENTRY,
    FUNC_EDITMENUENTRY,
    FUNC_LOADSTAGE,
    FUNC_DRAWTINTRECT,
    FUNC_RESETOBJECTENTITY,
    FUNC_PLAYEROBJECTCOLLISION,
    FUNC_CREATETEMPOBJECT,
    FUNC_DEFAULTGROUNDMOVEMENT,
    FUNC_DEFAULTAIRMOVEMENT,
    FUNC_DEFAULTROLLINGMOVEMENT,
    FUNC_DEFAULTGRAVITYTRUE,
    FUNC_DEFAULTGRAVITYFALSE,
    FUNC_DEFAULTJUMPACTION,
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
    FUNC_MAX_CNT
};

Compilerv2::Compilerv2()
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

int Compilerv2::FindStringToken(QString &string, QString token, char stopID)
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

void Compilerv2::CheckAliasText(QString &text)
{
    if (FindStringToken(text, "#alias", 1))
        return;
    int textPos    = 6;
    int aliasMatch = 0;
    while (aliasMatch < 2) {
        if (aliasMatch) {
            if (aliasMatch == 1) {
                aliasesv2[aliasCount].name += text[textPos];
                if (textPos + 1 >= text.length())
                    ++aliasMatch;
            }
        }
        else if (text[textPos] == ':') {
            aliasMatch = 1;
        }
        else {
            aliasesv2[aliasCount].value += text[textPos];
        }
        ++textPos;
    }
    ++aliasCount;
}
void Compilerv2::ConvertArithmaticSyntax(QString &text)
{
    int token    = 0;
    int offset   = 0;
    int findID   = 0;
    QString dest = "";

    for (int i = FUNC_EQUAL; i < FUNC_NOT; ++i) {
        findID = FindStringToken(text, scriptEvaluationTokens[i - 1], 1);
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
void Compilerv2::ConvertIfWhileStatement(QString &text)
{
    QString dest  = "";
    int compareOp = -1;
    int strPos    = 0;
    if (FindStringToken(text, "if", 1)) {
        if (!FindStringToken(text, "while", 1)) {
            for (int i = 0; i < 6; ++i) {
                int tPos = FindStringToken(text, scriptEvaluationTokens[i + (FUNC_NOT - 1)], 1);
                if (tPos > -1) {
                    strPos    = tPos;
                    compareOp = i;
                }
            }
            if (compareOp > -1) {
                text.replace(strPos, 1, ',');
                dest = functions[compareOp + FUNC_WEQUAL].name + "(";
                AppendIntegerToSting(dest, jumpTablePos - jumpTableOffset);
                dest += ",";
                for (int i = 5; i < text.length(); ++i) {
                    if (text[i] != '=' && text[i] != '(' && text[i] != ')')
                        dest += text[i];
                }
                dest += ")";
                text                                = dest;
                jumpTableStack[++jumpTableStackPos] = jumpTablePos;
                jumpTable[jumpTablePos++]           = scriptCodePos - scriptCodeOffset;
                jumpTable[jumpTablePos++]           = 0;
            }
        }
    }
    else {
        for (int i = 0; i < 6; ++i) {
            int tPos = FindStringToken(text, scriptEvaluationTokens[i + (FUNC_NOT - 1)], 1);
            if (tPos > -1) {
                strPos    = tPos;
                compareOp = i;
            }
        }
        if (compareOp > -1) {
            text.replace(strPos, 1, ',');
            dest = functions[compareOp + FUNC_IFEQUAL].name + "(";
            AppendIntegerToSting(dest, jumpTablePos - jumpTableOffset);
            dest += ",";
            for (int i = 2; i < text.length(); ++i) {
                if (text[i] != '=' && text[i] != '(' && text[i] != ')')
                    dest += text[i];
            }
            dest += ")";
            text                                = dest;
            jumpTableStack[++jumpTableStackPos] = jumpTablePos;
            jumpTable[jumpTablePos++]           = -1;
            jumpTable[jumpTablePos++]           = 0;
        }
    }
}
bool Compilerv2::ConvertSwitchStatement(QString &text)
{
    if (FindStringToken(text, "switch", 1))
        return false;
    QString switchText = "";
    switchText         = "switch(";
    AppendIntegerToSting(switchText, jumpTablePos - jumpTableOffset);
    switchText += ",";
    for (int i = 6; i < text.length(); ++i) {
        if (text[i] != '=' && text[i] != '(' && text[i] != ')')
            switchText += text[i];
    }
    switchText += ")";
    text                                = switchText;
    jumpTableStack[++jumpTableStackPos] = jumpTablePos;
    jumpTable[jumpTablePos++]           = 0x10000;
    jumpTable[jumpTablePos++]           = -0x10000;
    jumpTable[jumpTablePos++]           = -1;
    jumpTable[jumpTablePos++]           = 0;
    return true;
}
void Compilerv2::ConvertFunctionText(QString &text)
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
        scriptCode[scriptCodePos++] = opcode;
        if (functions[opcode].name == "else")
            jumpTable[jumpTableStack[jumpTableStackPos]] = scriptCodePos - scriptCodeOffset;

        if (functions[opcode].name == "endif") {
            int jPos            = jumpTableStack[jumpTableStackPos];
            jumpTable[jPos + 1] = scriptCodePos - scriptCodeOffset;
            if (jumpTable[jPos] == -1)
                jumpTable[jPos] = (scriptCodePos - scriptCodeOffset) - 1;
            --jumpTableStackPos;
        }

        if (functions[opcode].name == "endswitch") {
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

        if (functions[opcode].name == "loop") {
            jumpTable[jumpTableStack[jumpTableStackPos--] + 1] = scriptCodePos - scriptCodeOffset;
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
            // Eg: TempValue0 = FX_SCALE
            for (int a = 0; a < aliasCount; ++a) {
                if (funcName == aliasesv2[a].name) {
                    CopyAliasStr(funcName, aliasesv2[a].value, 0);
                    if (FindStringToken(aliasesv2[a].value, "[", 1) > -1)
                        CopyAliasStr(arrayStr, aliasesv2[a].value, 1);
                }
            }

            // Aliases (array value)
            char prefix = 0;
            if (arrayStr.length() > 0) {
                if (arrayStr[0] == '+' || arrayStr[0] == '-') {
                    prefix = arrayStr[0].toLatin1();
                    arrayStr.remove(0, 1);
                }

                // Eg: TempValue0 = FX_SCALE
                for (int a = 0; a < aliasCount; ++a) {
                    if (arrayStr == aliasesv2[a].name)
                        CopyAliasStr(arrayStr, aliasesv2[a].value, 0);
                }
            }

            // Eg: TempValue0 = Game.Variable
            for (int v = 0; v < globalVariables.count(); ++v) {
                if (funcName == globalVariables[v]) {
                    funcName = "Global";
                    arrayStr = "";
                    AppendIntegerToSting(arrayStr, v);
                }
            }

            if (ConvertStringToInteger(funcName, &value)) {
                scriptCode[scriptCodePos++] = SCRIPTVAR_INTCONST;
                scriptCode[scriptCodePos++] = value;
            }
            else if (funcName[0] == '"') {
                scriptCode[scriptCodePos++] = SCRIPTVAR_STRCONST;
                scriptCode[scriptCodePos++] = funcName.length() - 2;
                int scriptTextPos           = 1;
                int scriptTextByteID        = 0;
                while (scriptTextPos > -1) {
                    switch (scriptTextByteID) {
                        case 0:
                            scriptCode[scriptCodePos] = funcName[scriptTextPos].toLatin1() << 24;
                            ++scriptTextByteID;
                            break;
                        case 1:
                            scriptCode[scriptCodePos] += funcName[scriptTextPos].toLatin1() << 16;
                            ++scriptTextByteID;
                            break;
                        case 2:
                            scriptCode[scriptCodePos] += funcName[scriptTextPos].toLatin1() << 8;
                            ++scriptTextByteID;
                            break;
                        case 3:
                            scriptCode[scriptCodePos++] += funcName[scriptTextPos].toLatin1();
                            scriptTextByteID = 0;
                            break;
                        default: break;
                    }
                    if (funcName[scriptTextPos] == '"') {
                        if (scriptTextByteID > 0)
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
                if (arrayStr.length()) {
                    scriptCode[scriptCodePos] = VARARR_ARRAY;
                    if (prefix == '+')
                        scriptCode[scriptCodePos] = VARARR_ENTNOPLUS1;
                    if (prefix == '-')
                        scriptCode[scriptCodePos] = VARARR_ENTNOMINUS1;
                    ++scriptCodePos;

                    if (ConvertStringToInteger(arrayStr, &value) == 1) {
                        scriptCode[scriptCodePos++] = 0;
                        scriptCode[scriptCodePos++] = value;
                    }
                    else {
                        if (arrayStr == "ArrayPos0")
                            value = 0;
                        if (arrayStr == "ArrayPos1")
                            value = 1;
                        if (arrayStr == "TempObjectPos")
                            value = 2;
                        scriptCode[scriptCodePos++] = VARARR_ARRAY;
                        scriptCode[scriptCodePos++] = value;
                    }
                }
                else {
                    scriptCode[scriptCodePos++] = VARARR_NONE;
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
                scriptCode[scriptCodePos++] = value;
            }
        }
    }
}
void Compilerv2::CheckCaseNumber(QString &text)
{
    if (FindStringToken(text, "case", 1))
        return;

    QString caseString;
    // int caseStrPos = 0;
    char caseChar = text[4].toLatin1();
    if (text.length() >= 4) {
        int textPos = 5;
        do {
            if (caseChar != ':')
                caseString += caseChar;
            caseChar = text[textPos++].toLatin1();
        } while (caseChar);
    }
    else {
        // caseStrPos = 0;
    }

    for (int a = 0; a < aliasCount; ++a) {
        if (caseString == aliasesv2[a].name) {
            caseString = aliasesv2[a].value;
            break;
        }
    }

    int caseID = 0;
    if (ConvertStringToInteger(caseString, &caseID)) {
        int stackValue = jumpTableStack[jumpTableStackPos];
        if (caseID < jumpTable[stackValue])
            jumpTable[stackValue] = caseID;
        stackValue++;
        if (caseID > jumpTable[stackValue])
            jumpTable[stackValue] = caseID;
    }
    else {
        // printLog("WARNING: unable to convert case string \"%s\" to int, on line %d", caseString,
        //          lineID);
    }
}
bool Compilerv2::ReadSwitchCase(QString &text)
{
    QString caseText = "";
    if (FindStringToken(text, "case", 1)) {
        if (FindStringToken(text, "default", 1)) {
            return false;
        }
        else {
            int jumpTablepos            = jumpTableStack[jumpTableStackPos];
            jumpTable[jumpTablepos + 2] = scriptCodePos - scriptCodeOffset;
            int cnt = abs(jumpTable[jumpTablepos + 1] - jumpTable[jumpTablepos]) + 1;

            int jOffset = jumpTablepos + 4;
            for (int i = 0; i < cnt; ++i) {
                if (jumpTable[jOffset + i] < 0)
                    jumpTable[jOffset + i] = scriptCodePos - scriptCodeOffset;
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

        for (int a = 0; a < aliasCount; ++a) {
            if (caseText == aliasesv2[a].name)
                caseText = aliasesv2[a].value;
        }

        int val = 0;

        int jPos    = jumpTableStack[jumpTableStackPos];
        int jOffset = jPos + 4;
        if (ConvertStringToInteger(caseText, &val))
            jumpTable[val - jumpTable[jPos] + jOffset] = scriptCodePos - scriptCodeOffset;
        return true;
    }
    return false;
}
void Compilerv2::AppendIntegerToSting(QString &text, int value) { text += QString::number(value); }
bool Compilerv2::ConvertStringToInteger(QString &text, int *value)
{
    *value  = 0;
    bool ok = false;

    if (text.startsWith("0x") || text.startsWith("0X")) {
        *value = text.toLongLong(&ok, 0x10);
    }
    else if (text.startsWith("0b") || text.startsWith("0B")) {
        *value = text.toLongLong(&ok, 0b10);
    }
    else if (text.startsWith("0o") || text.startsWith("0O")) {
        *value = text.toLongLong(&ok, 0010);
    }
    else {
        *value = text.toLongLong(&ok, 10);
    }

    return ok;
}
void Compilerv2::CopyAliasStr(QString &dest, QString text, bool arrayIndex)
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

void Compilerv2::ParseScriptFile(QString scriptName, int scriptID)
{
    jumpTableStackPos = 0;
    lineID            = 0;
    aliasCount        = COMMONALIAS_COUNT_v2;
    for (int i = COMMONALIAS_COUNT_v2; i < ALIAS_COUNT_v2; ++i) {
        aliasesv2[i].name  = "";
        aliasesv2[i].value = "";
    }

    scriptError = false;
    fileFound   = true;

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
                    CheckAliasText(scriptText);
                    if (scriptText == "subRSDK") {
                        parseMode                                        = PARSEMODE_FUNCTION;
                        objectScriptList[scriptID].subRSDK.scriptCodePtr = scriptCodePos;
                        objectScriptList[scriptID].subRSDK.jumpTablePtr  = jumpTablePos;
                        scriptCodeOffset                                 = scriptCodePos;
                        jumpTableOffset                                  = jumpTablePos;
                    }
                    break;
                case PARSEMODE_FUNCTION:
                    if (!semiFlag)
                        ++lineID;
                    if (scriptText.length()) {
                        if (scriptText == "endsub") {
                            scriptCode[scriptCodePos++] = FUNC_END;
                            parseMode                   = PARSEMODE_SCOPELESS;
                        }
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
                    break;
                case PARSEMODE_SWITCHREAD:
                    if (!FindStringToken(scriptText, "switch", 1))
                        ++switchDeep;
                    if (switchDeep) {
                        if (!FindStringToken(scriptText, "endswitch", 1))
                            --switchDeep;
                    }
                    else if (FindStringToken(scriptText, "endswitch", 1)) {
                        CheckCaseNumber(scriptText);
                    }
                    else {
                        reader.seek(storePos);
                        parseMode  = PARSEMODE_FUNCTION;
                        int jPos   = jumpTableStack[jumpTableStackPos];
                        switchDeep = abs(jumpTable[jPos + 1] - jumpTable[jPos]) + 1;
                        for (textPos = 0; textPos < switchDeep; ++textPos)
                            jumpTable[jumpTablePos++] = -1;
                    }
                    break;
                default: break;
            }
        }

        reader.close();
    } else {
        fileFound = false;
    }
}

void Compilerv2::ClearScriptData()
{
    memset(scriptCode, 0, SCRIPTDATA_COUNT_v2 * sizeof(int));
    memset(jumpTable, 0, JUMPTABLE_COUNT_v2 * sizeof(int));

    globalVariables.clear();

    jumpTableStackPos = 0;

    scriptCodePos    = 0;
    scriptCodeOffset = 0;
    jumpTablePos     = 0;
    jumpTableOffset  = 0;

    for (int o = 0; o < OBJECT_COUNT_v2; ++o) {
        ObjectScript *scriptInfo          = &objectScriptList[o];
        scriptInfo->subRSDK.scriptCodePtr = SCRIPTDATA_COUNT_v2 - 1;
        scriptInfo->subRSDK.jumpTablePtr  = JUMPTABLE_COUNT_v2 - 1;
    }
}

void Compilerv2::WriteBytecode(QString path)
{
    bytecode = RSDKv2::Bytecode();

    bytecode.scriptCode.clear();
    for (int i = globalScriptCodeCount; i < scriptCodePos; ++i) {
        bytecode.scriptCode.append(scriptCode[i]);
    }

    bytecode.jumpTable.clear();
    for (int i = globalJumpTableCount; i < jumpTablePos; ++i) {
        bytecode.jumpTable.append(jumpTable[i]);
    }

    bytecode.globalScriptCount = 0;
    bytecode.scriptList.clear();
    for (int i = globalScriptCount; i < scriptCount; ++i) {
        RSDKv2::Bytecode::ObjectScript scr;
        scr.mainScript    = objectScriptList[i].subRSDK.scriptCodePtr;
        scr.mainJumpTable = objectScriptList[i].subRSDK.jumpTablePtr;

        bytecode.scriptList.append(scr);
    }

    bytecode.write(path);
}

void Compilerv2::ProcessScript(int scriptCodeStart, int jumpTableStart)
{
    bool running        = true;
    int scriptCodePtr   = scriptCodeStart;
    jumpTableStackPos   = 0;
    SceneEditor *editor = (SceneEditor *)this->editor;

    while (running) {
        int opcode           = scriptCode[scriptCodePtr++];
        int opcodeSize       = functions[opcode].opcodeSize;
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
                    case VAR_OBJECTFRAME: {
                        scriptEng.operands[i] = entityPtr->frame;
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
                    case VAR_PLAYERFRAME: {
                        break;
                    }
                    case VAR_PLAYERANIMATION: {
                        break;
                    }
                    case VAR_PLAYERANIMATIONSPEED: {
                        break;
                    }
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
                    case VAR_SCREENCAMERAENABLED: break;
                    case VAR_SCREENCAMERASTYLE: break;
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
                }
            }
            else if (opcodeType == SCRIPTVAR_INTCONST) { // int constant
                scriptEng.operands[i] = scriptCode[scriptCodePtr++];
            }
            else if (opcodeType == SCRIPTVAR_STRCONST) { // string constant
                int strLen = scriptCode[scriptCodePtr++];
                scriptText = "";
                for (int c = 0; c < strLen; ++c) {
                    switch (c % 4) {
                        case 0: {
                            scriptText += (char)(scriptCode[scriptCodePtr] >> 24);
                            break;
                        }
                        case 1: {
                            scriptText += (char)((0xFFFFFF & scriptCode[scriptCodePtr]) >> 16);
                            break;
                        }
                        case 2: {
                            scriptText += (char)((0xFFFF & scriptCode[scriptCodePtr]) >> 8);
                            break;
                        }
                        case 3: {
                            scriptText += (char)scriptCode[scriptCodePtr++];
                            break;
                        }
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
                editor->viewer->drawSpriteFlipped((entity->XPos >> 16) + spriteFrame->pivotX,
                                                  (entity->YPos >> 16) + spriteFrame->pivotY,
                                                  spriteFrame->width, spriteFrame->height,
                                                  spriteFrame->sprX, spriteFrame->sprY, FLIP_NONE,
                                                  INK_NONE, 0xFF, scriptInfo->spriteSheetID);
                break;
            case FUNC_DRAWSPRITEXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->viewer->drawSpriteFlipped((scriptEng.operands[1] >> 16) + spriteFrame->pivotX,
                                                  (scriptEng.operands[2] >> 16) + spriteFrame->pivotY,
                                                  spriteFrame->width, spriteFrame->height,
                                                  spriteFrame->sprX, spriteFrame->sprY, FLIP_NONE,
                                                  INK_NONE, 0xFF, scriptInfo->spriteSheetID);
                break;
            case FUNC_DRAWSPRITESCREENXY:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                editor->viewer->drawSpriteFlipped(scriptEng.operands[1] + spriteFrame->pivotX,
                                                  scriptEng.operands[2] + spriteFrame->pivotY,
                                                  spriteFrame->width, spriteFrame->height,
                                                  spriteFrame->sprX, spriteFrame->sprY, FLIP_NONE,
                                                  INK_NONE, 0xFF, scriptInfo->spriteSheetID);
                break;
            case FUNC_DRAWTINTRECT: opcodeSize = 0; break;
            case FUNC_DRAWNUMBERS: {
                opcodeSize = 0;
                int i      = 10;
                if (scriptEng.operands[6]) {
                    while (scriptEng.operands[4] > 0) {
                        int frameID = scriptEng.operands[3] % i / (i / 10) + scriptEng.operands[0];
                        spriteFrame = &scriptFrames[scriptInfo->frameListOffset + frameID];
                        editor->viewer->drawSpriteFlipped(
                            spriteFrame->pivotX + scriptEng.operands[1],
                            spriteFrame->pivotY + scriptEng.operands[2], spriteFrame->width,
                            spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_NONE,
                            INK_NONE, 0xFF, scriptInfo->spriteSheetID);
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
                            editor->viewer->drawSpriteFlipped(
                                spriteFrame->pivotX + scriptEng.operands[1],
                                spriteFrame->pivotY + scriptEng.operands[2], spriteFrame->width,
                                spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY, FLIP_NONE,
                                INK_NONE, 0xFF, scriptInfo->spriteSheetID);
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
                if (scriptFrameCount < SPRITEFRAME_COUNT_v2) {
                    scriptFrames[scriptFrameCount].pivotX = scriptEng.operands[0];
                    scriptFrames[scriptFrameCount].pivotY = scriptEng.operands[1];
                    scriptFrames[scriptFrameCount].width  = scriptEng.operands[2];
                    scriptFrames[scriptFrameCount].height = scriptEng.operands[3];
                    scriptFrames[scriptFrameCount].sprX   = scriptEng.operands[4];
                    scriptFrames[scriptFrameCount].sprY   = scriptEng.operands[5];
                    ++scriptFrameCount;
                }
                break;
            case FUNC_LOADPALETTE: opcodeSize = 0; break;
            case FUNC_ROTATEPALETTE: opcodeSize = 0; break;
            case FUNC_CLEARSCREEN: opcodeSize = 0; break;
            case FUNC_DRAWSPRITEFX:
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                switch (scriptEng.operands[1]) {
                    default: break;
                    case FX_SCALE:
                        editor->viewer->drawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16) - xScrollOffset,
                            (scriptEng.operands[3] >> 16) - yScrollOffset, -spriteFrame->pivotX,
                            -spriteFrame->pivotY, spriteFrame->width, spriteFrame->height,
                            spriteFrame->sprX, spriteFrame->sprY, entity->scale, entity->scale,
                            entity->direction, 0, INK_NONE, 0xFF, scriptInfo->spriteSheetID);
                        break;
                    case FX_ROTATE:
                        editor->viewer->drawSpriteRotozoom(
                            (scriptEng.operands[2] >> 16) - xScrollOffset,
                            (scriptEng.operands[3] >> 16) - yScrollOffset, -spriteFrame->pivotX,
                            -spriteFrame->pivotY, spriteFrame->width, spriteFrame->height,
                            spriteFrame->sprX, spriteFrame->sprY, 0x200, 0x200, entity->direction,
                            entity->rotation, INK_NONE, 0xFF, scriptInfo->spriteSheetID);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                editor->viewer->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_NONE, 0xFF,
                                    scriptInfo->spriteSheetID);
                                break;
                            case INK_BLEND:
                                editor->viewer->drawSpriteFlipped(
                                    (scriptEng.operands[2] >> 16) + spriteFrame->pivotX,
                                    (scriptEng.operands[3] >> 16) + spriteFrame->pivotY,
                                    spriteFrame->width, spriteFrame->height, spriteFrame->sprX,
                                    spriteFrame->sprY, FLIP_NONE, INK_BLEND, 0xFF,
                                    scriptInfo->spriteSheetID);
                                break;
                        }
                        break;
                    case FX_TINT: break;
                }
                break;
            case FUNC_DRAWSPRITESCREENFX: {
                opcodeSize  = 0;
                spriteFrame = &scriptFrames[scriptInfo->frameListOffset + scriptEng.operands[0]];
                switch (scriptEng.operands[1]) {
                    default: break;
                    case FX_SCALE:
                        editor->viewer->drawSpriteRotozoom(
                            scriptEng.operands[2] - xScrollOffset,
                            scriptEng.operands[3] - yScrollOffset, -spriteFrame->pivotX,
                            -spriteFrame->pivotY, spriteFrame->width, spriteFrame->height,
                            spriteFrame->sprX, spriteFrame->sprY, entity->scale, entity->scale,
                            entity->direction, 0, INK_NONE, 0xFF, scriptInfo->spriteSheetID);
                        break;
                    case FX_ROTATE:
                        editor->viewer->drawSpriteRotozoom(
                            scriptEng.operands[2] - xScrollOffset,
                            scriptEng.operands[3] - yScrollOffset, -spriteFrame->pivotX,
                            -spriteFrame->pivotY, spriteFrame->width, spriteFrame->height,
                            spriteFrame->sprX, spriteFrame->sprY, 0x200, 0x200, entity->direction,
                            entity->rotation, INK_NONE, 0xFF, scriptInfo->spriteSheetID);
                        break;
                    case FX_INK:
                        switch (entity->inkEffect) {
                            case INK_NONE:
                                editor->viewer->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_NONE, 0xFF, scriptInfo->spriteSheetID);
                                break;
                            case INK_BLEND:
                                editor->viewer->drawSpriteFlipped(
                                    scriptEng.operands[2] + spriteFrame->pivotX,
                                    scriptEng.operands[3] + spriteFrame->pivotY, spriteFrame->width,
                                    spriteFrame->height, spriteFrame->sprX, spriteFrame->sprY,
                                    FLIP_NONE, INK_BLEND, 0xFF, scriptInfo->spriteSheetID);
                                break;
                        }
                        break;
                    case FX_TINT: break;
                }
                break;
            }
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
            case FUNC_RESETOBJECTENTITY: {
                opcodeSize = 0;
                break;
            }
            case FUNC_PLAYEROBJECTCOLLISION: opcodeSize = 0; break;
            case FUNC_CREATETEMPOBJECT: {
                opcodeSize = 0;
                break;
            }
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
                    case VAR_OBJECTFRAME: {
                        entityPtr->frame = scriptEng.operands[i];
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

                    case VAR_PLAYERFRAME: {
                        break;
                    }
                    case VAR_PLAYERANIMATION: {
                        break;
                    }

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
                    case VAR_SCREENCAMERAENABLED: break;
                    case VAR_SCREENCAMERASTYLE: break;
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

byte Compilerv2::ArcTanLookup(int X, int Y)
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
