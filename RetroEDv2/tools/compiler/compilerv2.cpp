#include "includes.hpp"

#define ALIAS_COUNT       (0x80)
#define COMMONALIAS_COUNT (25)

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

int Compilerv2::findStringToken(QString &string, QString token, char stopID)
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

void Compilerv2::checkAliasText(QString &text)
{
    if (findStringToken(text, "#alias", 1))
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
void Compilerv2::convertArithmaticSyntax(QString &text)
{
    int token    = 0;
    int offset   = 0;
    int findID   = 0;
    QString dest = "";

    for (int i = FUNC_EQUAL; i < FUNC_NOT; ++i) {
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
void Compilerv2::convertIfWhileStatement(QString &text)
{
    QString dest  = "";
    int compareOp = -1;
    int strPos    = 0;
    if (findStringToken(text, "if", 1)) {
        if (!findStringToken(text, "while", 1)) {
            for (int i = 0; i < 6; ++i) {
                int tPos = findStringToken(text, scriptEvaluationTokens[i + (FUNC_NOT - 1)], 1);
                if (tPos > -1) {
                    strPos    = tPos;
                    compareOp = i;
                }
            }
            if (compareOp > -1) {
                text.replace(strPos, 1, ',');
                dest = functions[compareOp + FUNC_WEQUAL].name + "(";
                appendIntegerToSting(dest, jumpTableDataPos - jumpTableDataOffset);
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
            int tPos = findStringToken(text, scriptEvaluationTokens[i + (FUNC_NOT - 1)], 1);
            if (tPos > -1) {
                strPos    = tPos;
                compareOp = i;
            }
        }
        if (compareOp > -1) {
            text.replace(strPos, 1, ',');
            dest = functions[compareOp + FUNC_IFEQUAL].name + "(";
            appendIntegerToSting(dest, jumpTableDataPos - jumpTableDataOffset);
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
bool Compilerv2::convertSwitchStatement(QString &text)
{
    if (findStringToken(text, "switch", 1))
        return false;
    QString switchText = "";
    switchText         = "switch(";
    appendIntegerToSting(switchText, jumpTableDataPos - jumpTableDataOffset);
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
void Compilerv2::convertFunctionText(QString &text)
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
                if (funcName == aliasesv2[a].name) {
                    copyAliasStr(funcName, aliasesv2[a].value, 0);
                    if (findStringToken(aliasesv2[a].value, "[", 1) > -1)
                        copyAliasStr(strBuffer, aliasesv2[a].value, 1);
                }
            }
            // Eg: TempValue0 = Game.Variable
            for (int v = 0; v < globalVariables.count(); ++v) {
                if (funcName == globalVariables[v]) {
                    funcName  = "Global";
                    strBuffer = "";
                    appendIntegerToSting(strBuffer, v);
                }
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
void Compilerv2::checkCaseNumber(QString &text)
{
    if (findStringToken(text, "case", 1))
        return;

    QString caseString;
    int caseStrPos = 0;
    char caseChar  = text[4].toLatin1();
    if (text.length() >= 4) {
        int textPos = 5;
        do {
            if (caseChar != ':')
                caseString += caseChar;
            caseChar = text[textPos++].toLatin1();
        } while (caseChar);
    }
    else {
        caseStrPos = 0;
    }

    for (int a = 0; a < aliasCount; ++a) {
        if (caseString == aliasesv2[a].name) {
            caseString = aliasesv2[a].value;
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
        // printLog("WARNING: unable to convert case string \"%s\" to int, on line %d", caseString,
        //          lineID);
    }
}
bool Compilerv2::readSwitchCase(QString &text)
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

        for (int a = 0; a < aliasCount; ++a) {
            if (caseText == aliasesv2[a].name)
                caseText = aliasesv2[a].value;
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
void Compilerv2::appendIntegerToSting(QString &text, int value) { text += QString::number(value); }
bool Compilerv2::convertStringToInteger(QString &text, int *value)
{
    *value  = 0;
    bool ok = false;

    if (text.startsWith("0x") || text.startsWith("0X")) {
        *value = text.toInt(&ok, 0x10);
    }
    else if (text.startsWith("0b") || text.startsWith("0B")) {
        *value = text.toInt(&ok, 0b10);
    }
    else if (text.startsWith("0o") || text.startsWith("0O")) {
        *value = text.toInt(&ok, 0010);
    }
    else {
        *value = text.toInt(&ok, 10);
    }

    return ok;
}
void Compilerv2::copyAliasStr(QString &dest, QString text, bool arrayIndex)
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

void Compilerv2::parseScriptFile(QString scriptName, int scriptID)
{
    jumpTableStackPos = 0;
    lineID            = 0;
    aliasCount        = COMMONALIAS_COUNT;
    for (int i = COMMONALIAS_COUNT; i < ALIAS_COUNT; ++i) {
        aliasesv2[i].name  = "";
        aliasesv2[i].value = "";
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
            scriptText  = "";
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
                            readMode = READMODE_ENDLINE;
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
                    ++lineID;
                    checkAliasText(scriptText);
                    if (scriptText == "subRSDK") {
                        parseMode                                        = PARSEMODE_FUNCTION;
                        objectScriptList[scriptID].subRSDK.scriptCodePtr = scriptDataPos;
                        objectScriptList[scriptID].subRSDK.jumpTablePtr  = jumpTableDataPos;
                        scriptDataOffset                                 = scriptDataPos;
                        jumpTableDataOffset                              = jumpTableDataPos;
                    }
                    break;
                case PARSEMODE_FUNCTION:
                    ++lineID;
                    if (scriptText.length()) {
                        if (scriptText == "endsub") {
                            scriptData[scriptDataPos++] = FUNC_END;
                            parseMode                   = PARSEMODE_SCOPELESS;
                        }
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

void Compilerv2::clearScriptData()
{
    memset(scriptData, 0, SCRIPTDATA_COUNT * sizeof(int));
    memset(jumpTableData, 0, JUMPTABLE_COUNT * sizeof(int));

    globalVariables.clear();

    jumpTableStackPos = 0;

    scriptDataPos       = 0;
    scriptDataOffset    = 0;
    jumpTableDataPos    = 0;
    jumpTableDataOffset = 0;

    functionCount = 0;

    for (int o = 0; o < OBJECT_COUNT; ++o) {
        ObjectScript *scriptInfo          = &objectScriptList[o];
        scriptInfo->subRSDK.scriptCodePtr = SCRIPTDATA_COUNT - 1;
        scriptInfo->subRSDK.jumpTablePtr  = JUMPTABLE_COUNT - 1;
    }
}

void Compilerv2::writeBytecode(QString path)
{
    bytecode = RSDKv2::Bytecode();

    bytecode.scriptData.clear();
    for (int i = globalScriptDataCount; i < scriptDataPos; ++i) {
        bytecode.scriptData.append(scriptData[i]);
    }

    bytecode.jumpTableData.clear();
    for (int i = globalJumpTableCount; i < jumpTableDataPos; ++i) {
        bytecode.jumpTableData.append(jumpTableData[i]);
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

void Compilerv2::processScript(int scriptCodePtr, int jumpTablePtr, byte scriptSub)
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
                    case VAR_OBJECTFRAME: {
                        scriptEng.operands[i] = objectEntityList[arrayVal].frame;
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
                if (scriptFrameCount < SPRITEFRAME_COUNT) {
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
                    case VAR_OBJECTFRAME: {
                        objectEntityList[arrayVal].frame = scriptEng.operands[i];
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
