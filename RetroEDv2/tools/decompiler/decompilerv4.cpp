#include "includes.hpp"

namespace RSDKv4
{

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
    "FACE_TEXTURED_3D", "FACE_TEXTURED_2D", "FACE_COLOURED_3D",      "FACE_COLOURED_2D",
    "FACE_FADED",       "FACE_TEXTURED_C",  "FACE_TEXTURED_C_BLEND", "FACE_SPRITE_3D",
};

static QList<QString> priorityAliases = {
    "PRIORITY_ACTIVE_BOUNDS",   "PRIORITY_ACTIVE",   "PRIORITY_ACTIVE_PAUSED", "PRIORITY_XBOUNDS",
    "PRIORITY_XBOUNDS_DESTROY", "PRIORITY_INACTIVE", "PRIORITY_BOUNDS_SMALL",  "PRIORITY_ACTIVE_SMALL",
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
    // typeNames.clear();
    //
    // functionNames.clear();
    // variableNames.clear();
    //
    // globalSFXCount = 0;
    // sfxNames.clear();
    //
    // globalScriptCount = 0;
    // useHex            = false;

    scriptDataPos    = 0;
    jumpTableDataPos = 0;

    state         = StateScriptEngine();
    functionCount = 0;
    // globalfunctionCount = 0;
}

QString curTypeNamev4 = "Blank Object";
QString RSDKv4::Decompiler::setArrayValue(QString strIn, QString index)
{
    QString strOut = strIn;
    int point      = -1;

    if (strIn == "global") {
        strOut = globalVarNames[index.toInt()];
        if (strOut == "")
            return strIn;
        return strOut;
    }
    else if (strIn == "local") {
        bool ok = false;
        int idx = index.toInt(&ok);
        if (ok) {
            StaticVarInfo c(QString("%1_value%2").arg(curTypeNamev4).arg(staticVars.count()), idx);

            if (!staticVars.contains(c)) {
                strOut = c.name;
                staticVars.append(c);
            }
            else if (staticVars.indexOf(c) >= 0) {
                strOut = staticVars[staticVars.indexOf(c)].name;
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
    QString name  = "";
    int startPtr  = 0;
    int endPtr    = 0;
    int jumpStart = 0;
    bool function = false;
    ScriptPtrv4() {}
    ScriptPtrv4(QString n, int s, int j, bool f)
    {
        name      = n;
        startPtr  = s;
        endPtr    = 0;
        jumpStart = j;
        function  = f;
    }
    ScriptPtrv4(const ScriptPtrv4 &other)
    {
        name      = other.name;
        startPtr  = other.startPtr;
        endPtr    = other.endPtr;
        jumpStart = other.jumpStart;
        function  = other.function;
    }
};

int funcIDv4       = 0;
byte debugNameFlag = 0;
QList<ScriptPtrv4> scriptPtrs;

RSDKv4::Decompiler::Decompiler()
{
    variableList_rev00 = {
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
        "engine.message",
        "engine.language",
        "engine.onlineActive",
        "engine.sfxVolume",
        "engine.bgmVolume",
        "engine.platformID",
        "engine.trialMode",
        "engine.hapticsEnabled",
    };

    variableList = {
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
        "engine.hapticsEnabled",
    };

    // clang-format off
    functionList_rev02 = {
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

    functionList_rev01 = {
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

        FunctionInfo("LoadFontFile", 1),
        FunctionInfo("LoadTextFile", 2),
        FunctionInfo("GetTextInfo", 5),
        FunctionInfo("DrawText", 7),
        FunctionInfo("GetVersionNumber", 2),

        FunctionInfo("GetTableValue", 3),
        FunctionInfo("SetTableValue", 3),

        FunctionInfo("CheckCurrentStageFolder", 1),
        FunctionInfo("Abs", 1),

        FunctionInfo("CallNativeFunction", 1),
        FunctionInfo("CallNativeFunction2", 3),
        FunctionInfo("CallNativeFunction4", 5),

        FunctionInfo("SetObjectRange", 1),
        FunctionInfo("Print", 3),
    };

    functionList_rev00 = {
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
        FunctionInfo("SetPaletteFade", 7),
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

        FunctionInfo("LoadFontFile", 1),
        FunctionInfo("LoadTextFile", 3),
        FunctionInfo("DrawText", 7),
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
        FunctionInfo("Print", 3),
    };

}

void RSDKv4::Decompiler::decompile(QString destPath)
{
    clearScriptData();

    funcIDv4 = globalFunctionCount; // what function to use
    for (int i = globalScriptCount; i < sourceNames.count(); ++i) {
        RSDKv4::Bytecode::ScriptInfo &objectScript = scriptList[i];
        int scriptCodePtrs[3]    = { objectScript.main.scriptCodePos, objectScript.draw.scriptCodePos,
                                  objectScript.startup.scriptCodePos };
        int lowestScriptCodePtr  = 0x3FFFF;
        int highestScriptCodePtr = 0;

        for (int e = 0; e < 3; ++e) {
            if (scriptCodePtrs[e] < 0x3FFFF && scriptCodePtrs[e] > highestScriptCodePtr)
                highestScriptCodePtr = scriptCodePtrs[e];
        }

        for (int e = 0; e < 3; ++e) {
            if (scriptCodePtrs[e] < 0x3FFFF && scriptCodePtrs[e] < lowestScriptCodePtr)
                lowestScriptCodePtr = scriptCodePtrs[e];
        }

        for (int f = funcIDv4; f < functionList.count(); ++f) {
            auto &func = functionList[f];
            if (func.scriptCodePos < lowestScriptCodePtr && lowestScriptCodePtr < 0x3FFFF) {
                QString tn = typeNames[i];
                functionNames.append(tn.replace(" ", "") + "_Function" + QString::number(f));
                funcIDv4++;
            }
        }
    }

    for (int o = globalScriptCount; o < sourceNames.count(); ++o) {
        // check to give proper names
    }

    funcIDv4      = globalFunctionCount; // what function to use
    curTypeNamev4 = "Blank Object";

    for (int o = globalScriptCount; o < sourceNames.count(); ++o) {
        if (o == 0)
            continue;
        QString path       = "";
        QString scriptPath = sourceNames[o];
        QString baseDir    = destPath + "/Scripts/";
        if (useCustomAliases || seperateFolders)
            baseDir = destPath + "/Scripts/" + QFileInfo(bytecodePath).baseName() + "/";

        QString dir = baseDir + scriptPath.replace(QFileInfo(scriptPath).fileName(), "");
        scriptPath  = sourceNames[o];

        if (!QDir(QDir::tempPath()).exists(dir)) {
            QDir(QDir::tempPath()).mkpath(dir);
        }

        path = baseDir + scriptPath;

        if (QFile::exists(path))
            QFile::remove(path);
        Writer writer(path);

        RSDKv4::Bytecode::ScriptInfo &objectScript = scriptList[o];

        int scriptCodePtrs[3]    = { objectScript.main.scriptCodePos, objectScript.draw.scriptCodePos,
                                  objectScript.startup.scriptCodePos };
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

        curTypeNamev4 = typeNames[o];
        curTypeNamev4 = curTypeNamev4.replace(" ", "");

        scriptPtrs.clear();

        if (objectScript.main.scriptCodePos < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectMain", objectScript.main.scriptCodePos,
                                          objectScript.main.jumpTablePos, false));

        if (objectScript.draw.scriptCodePos < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectDraw", objectScript.draw.scriptCodePos,
                                          objectScript.draw.jumpTablePos, false));

        if (objectScript.startup.scriptCodePos < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectStartup", objectScript.startup.scriptCodePos,
                                          objectScript.startup.jumpTablePos, false));
        std::sort(
            scriptPtrs.begin(), scriptPtrs.end(),
            [](const ScriptPtrv4 &a, const ScriptPtrv4 &b) -> bool { return a.startPtr < b.startPtr; });

        int lastPtr = scriptPtrs.count() > 0 ? scriptPtrs[scriptPtrs.count() - 1].startPtr : 0x3FFFF;
        scriptPtrs.clear(); // temp, used to sort

        bool flag = false;
        for (int f = funcIDv4; f < functionList.count(); ++f) {
            int fs = functionList[f].scriptCodePos;
            if (fs < lastPtr) {
                scriptPtrs.append(ScriptPtrv4(functionNames[f], functionList[f].scriptCodePos,
                                              functionList[f].jumpTablePos, true));
            }
            else if (!flag) {
                flag     = true;
                funcIDv4 = f;
            }
        }
        if (!flag) {
            funcIDv4 = functionList.count();
        }

        if (objectScript.main.scriptCodePos < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectMain", objectScript.main.scriptCodePos,
                                          objectScript.main.jumpTablePos, false));

        if (objectScript.draw.scriptCodePos < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectDraw", objectScript.draw.scriptCodePos,
                                          objectScript.draw.jumpTablePos, false));

        if (objectScript.startup.scriptCodePos < 0x3FFFF)
            scriptPtrs.append(ScriptPtrv4("ObjectStartup", objectScript.startup.scriptCodePos,
                                          objectScript.startup.jumpTablePos, false));

        QByteArray buf;
        QBuffer buffer(&buf);
        buffer.open(QIODevice::Append);
        QDataStream *cmem = new QDataStream(&buffer);
        Writer bufWriter(cmem);

        int fCnt      = 0;
        debugNameFlag = 0;
        for (int s = 0; s < scriptPtrs.count(); ++s) {
            if (scriptPtrs[s].name != "ObjectMain" && scriptPtrs[s].name != "ObjectDraw"
                && scriptPtrs[s].name != "ObjectStartup") {
                fCnt++;
            }
            else if (scriptPtrs[s].name == "ObjectStartup" && curTypeNamev4 != "DebugMode") {
                QByteArray buf2;
                QBuffer buffer2(&buf2);
                buffer2.open(QIODevice::Append);
                QDataStream *cmem2 = new QDataStream(&buffer2);
                Writer bufWriter2(cmem2);

                // do this to get debugMode func names
                debugNameFlag = 1;
                decompileScript(bufWriter2, scriptPtrs[s].startPtr, scriptPtrs[s].jumpStart,
                                scriptPtrs[s].name, scriptPtrs[s].function);
                debugNameFlag = 0;
            }
        }

        QList<ScriptPtrv4> unsortedPtrs = scriptPtrs;
        std::sort(
            scriptPtrs.begin(), scriptPtrs.end(),
            [](const ScriptPtrv4 &a, const ScriptPtrv4 &b) -> bool { return a.startPtr < b.startPtr; });

        for (int s = 0; s < scriptPtrs.count(); ++s) {
            bufWriter.writeLine("", LINE_CRLF);
            debugNameFlag = scriptPtrs[s].name == "ObjectStartup" && curTypeNamev4 != "DebugMode";
            if (debugNameFlag)
                debugNameFlag++;

            int end = decompileScript(bufWriter, scriptPtrs[s].startPtr, scriptPtrs[s].jumpStart,
                                      scriptPtrs[s].name, scriptPtrs[s].function);
            scriptPtrs[s].endPtr = end;
            debugNameFlag        = 0;
        }

        writer.writeLine("// ----------------------------------");
        writer.writeLine("// RSDK Project: Sonic 1/Sonic 2");
        writer.writeLine("// Script Description: " + typeNames[o] + " Object");
        writer.writeLine("// Script Author: Christian Whitehead/Simon Thomley");
        writer.writeLine("// Unpacked by Rubberduckycooly's script unpacker");
        writer.writeLine("// ----------------------------------");
        writer.writeLine("", LINE_CRLF);

        writer.writeLine("//-------Aliases-------//", LINE_CRLF);

        writer.writeLine("private alias " + QString::number(o) + " : TYPE_"
                             + typeNames[o].toUpper().replace(" ", ""),
                         LINE_CRLF);
        if (o == 1) {
            writer.writeLine("public alias 256 : GROUP_PLAYERS", LINE_CRLF);
            writer.writeLine("public alias 65536 : HITBOX_AUTO", LINE_CRLF);
            writer.writeLine("public alias arrayPos6 : currentPlayer", LINE_CRLF);
            writer.writeLine("public alias arrayPos7 : playerCount", LINE_CRLF);
        }

        writer.writeLine("", LINE_CRLF);

        if (fCnt)
            writer.writeLine("// Function declarations", LINE_CRLF);

        for (int s = 0; s < unsortedPtrs.count(); ++s) {
            if (unsortedPtrs[s].name != "ObjectMain" && unsortedPtrs[s].name != "ObjectDraw"
                && unsortedPtrs[s].name != "ObjectStartup") {
                writer.writeLine("reserve function " + unsortedPtrs[s].name, LINE_CRLF);
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

        if (staticVars.count() > 0) {
            writer.writeLine("", LINE_CRLF);
            writer.writeLine("// Static Values", LINE_CRLF);
        }
        QList<int> registeredPos;

        int firstOffset = lastOffset;
        int endOffset   = scriptPtrs[scriptPtrs.count() - 1].endPtr;

        std::sort(tables.begin(), tables.end(),
                  [](const StaticVarInfo &a, const StaticVarInfo &b) -> bool {
                      return a.dataPos < b.dataPos;
                  });

        std::sort(staticVars.begin(), staticVars.end(),
                  [](const StaticVarInfo &a, const StaticVarInfo &b) -> bool {
                      return a.dataPos < b.dataPos;
                  });

        for (int c = globalStaticCount; c < staticVars.count(); ++c) {
            int index = staticVars[c].dataPos;
            if (index >= firstOffset && index < endOffset) {
                writer.writeLine(QString("public value %2 = %1;")
                                     .arg(toHexString(QString::number(scriptCode[index])))
                                     .arg(staticVars[c].name),
                                 LINE_CRLF);
                registeredPos.append(index);
            }
        }

        if (tables.count() > 0) {
            writer.writeLine("", LINE_CRLF);
            writer.writeLine("// Tables", LINE_CRLF);
        }
        for (int a = globalArrayCount; a < tables.count(); ++a) {
            int index = tables[a].dataPos;
            if (index >= firstOffset && index < endOffset) {
                writer.writeLine(QString("public table %1").arg(tables[a].name), LINE_CRLF);
                registeredPos.append(index);
                writer.writeText("\t");
                for (int v = 1; v < scriptCode[index] + 1; ++v) {
                    writer.writeText(toHexString(QString::number(scriptCode[index + v])));
                    registeredPos.append(index + v);
                    if ((v % 0x10) == 0) {
                        writer.writeLine("", LINE_CRLF);
                        writer.writeText("\t");
                    }
                    else if (v + 1 < scriptCode[index] + 1) {
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
        if (scriptPtrs[0].startPtr - lastOffset > 0) {
            for (int b = lastOffset; b < scriptPtrs[0].startPtr; ++b) {
                if (!registeredPos.contains(b) && (b >= firstOffset && b < endOffset))
                    constCnt++;
            }
        }

        for (int s = 0; s < scriptPtrs.count() - 1; ++s) {
            int dif = scriptPtrs[s + 1].startPtr - scriptPtrs[s].endPtr;
            if (dif > 0) {
                for (int b = scriptPtrs[s].endPtr; b < scriptPtrs[s].endPtr + dif; ++b) {
                    if (!registeredPos.contains(b) && (b >= firstOffset && b < endOffset))
                        constCnt++;
                }
            }
        }

        if (constCnt > 0) {
            writer.writeLine("", LINE_CRLF);
            writer.writeLine("//Unknown Variables", LINE_CRLF);

            if (scriptPtrs[0].startPtr - lastOffset > 0) {
                for (int b = lastOffset; b < scriptPtrs[0].startPtr; ++b) {
                    if (!registeredPos.contains(b) && (b >= firstOffset && b < endOffset))
                        writer.writeLine(QString("private value value%2 = %1;")
                                             .arg(toHexString(QString::number(scriptCode[b])))
                                             .arg(toHexString(QString::number(b))),
                                         LINE_CRLF);
                }
            }

            for (int s = 0; s < scriptPtrs.count() - 1; ++s) {
                int dif = scriptPtrs[s + 1].startPtr - scriptPtrs[s].endPtr;
                if (dif > 0) {
                    for (int b = scriptPtrs[s].endPtr; b < scriptPtrs[s].endPtr + dif; ++b) {
                        if (!registeredPos.contains(b) && (b >= firstOffset && b < endOffset))
                            writer.writeLine(QString("private value value%2 = %1;")
                                                 .arg(toHexString(QString::number(scriptCode[b])))
                                                 .arg(toHexString(QString::number(b))),
                                             LINE_CRLF);
                    }
                }
            }
        }
        lastOffset = scriptPtrs[scriptPtrs.count() - 1].endPtr;

        bufWriter.flush();
        QString str = QString::fromLatin1(buf);
        writer.writeText(buf);
        writer.flush();
    }
}

int RSDKv4::Decompiler::decompileScript(Writer &writer, int scriptCodePtr, int jumpTablePtr,
                                        QString subName, bool isFunction)
{
    if (!isFunction)
        writer.writeLine("event " + subName, LINE_CRLF);
    else
        writer.writeLine("function " + subName, LINE_CRLF);

    StateScriptEngine state;
    state.scriptCodePtr = state.scriptCodeOffset = scriptCodePtr;
    state.jumpTablePtr = state.jumpTableOffset = jumpTablePtr;
    state.deep                                 = 1;
    state.switchDeep                           = -1;

    decompileSub(writer, state, isFunction);
    writer.writeLine("", LINE_CRLF);
    return state.scriptCodePtr;
}

void RSDKv4::Decompiler::decompileSub(Writer writer, RSDKv4::Decompiler::StateScriptEngine &state,
                                      bool isFunction)
{
    state.endFlag = false;

    const QList<FunctionInfo> *functionPtrs = nullptr;
    const QList<QString> *variablePtrs      = nullptr;
    switch (engineRevision) {
        case 0:
            functionPtrs = &functionList_rev00;
            variablePtrs = &variableList_rev00;
            break;
        case 1:
            functionPtrs = &functionList_rev01;
            variablePtrs = &variableList;
            break;
        case 2:
            functionPtrs = &functionList_rev02;
            variablePtrs = &variableList;
            break;
    }

    while (!state.endFlag) {
        if (state.switchDeep >= 0) {
            SwitchState &sw = state.switchState[state.switchDeep];
            for (int j = 0; j < sw.jumpPtr.count(); ++j) {
                if (sw.jumpPtr[j].jump == state.scriptCodePtr) {
                    if (sw.jumpPtr[j].jump != sw.endJmp + state.scriptCodeOffset - 1
                        && sw.jumpPtr[j].jump != sw.endJmp + state.scriptCodeOffset
                        && sw.jumpPtr[j].jump != sw.defaultJmp + state.scriptCodeOffset) {
                        for (auto &c : sw.jumpPtr[j].cases) {
                            for (int i = 0; i < state.deep - 1; ++i) writer.writeText("\t");
                            writer.writeLine("case " + QString::number(c), LINE_CRLF);
                        }
                    }
                }
            }

            if (sw.defaultJmp + state.scriptCodeOffset == state.scriptCodePtr) {
                if (sw.defaultJmp != sw.endJmp - 1 && sw.defaultJmp != sw.endJmp) {
                    for (int i = 0; i < state.deep - 1; ++i) writer.writeText("\t");
                    writer.writeLine("default", LINE_CRLF);
                }
            }
        }
        int opcode      = 0;
        int paramsCount = 0;

        opcode      = scriptCode[state.scriptCodePtr++];
        paramsCount = functionPtrs->at(opcode).opcodeSize;

        QList<QString> variableName;
        for (int i = 0; i < 0x10; ++i) variableName.append("UNKNOWN VARIABLE");

        for (int i = 0; i < paramsCount; ++i) {
            int paramId = scriptCode[state.scriptCodePtr++];
            switch (paramId) {
                case 0: // Unused
                    break;
                case 1: // Read value from RSDK
                    switch (scriptCode[state.scriptCodePtr++]) {
                        case 0: // Read Const Variable
                            variableName[i] = variablePtrs->at(scriptCode[state.scriptCodePtr++]);
                            break;
                        case 1:                                         // ARRAY
                            if (scriptCode[state.scriptCodePtr++] == 1) // Variable
                            {
                                QString value   = arrayPosition[scriptCode[state.scriptCodePtr++]];
                                variableName[i] = setArrayValue(
                                    variablePtrs->at(scriptCode[state.scriptCodePtr++]), value);
                            }
                            else // Value
                            {
                                QString value   = QString::number(scriptCode[state.scriptCodePtr++]);
                                variableName[i] = setArrayValue(
                                    variablePtrs->at(scriptCode[state.scriptCodePtr++]), value);
                            }
                            break;
                        case 2:
                            // ObjectLoop +
                            if (scriptCode[state.scriptCodePtr++] == 1) {
                                QString value = "+" + arrayPosition[scriptCode[state.scriptCodePtr++]];
                                variableName[i] = setArrayValue(
                                    variablePtrs->at(scriptCode[state.scriptCodePtr++]), value);
                            }
                            else {
                                QString value =
                                    "+" + QString::number(scriptCode[state.scriptCodePtr++]);
                                variableName[i] = setArrayValue(
                                    variablePtrs->at(scriptCode[state.scriptCodePtr++]), value);
                            }
                            break;
                        case 3:
                            // ObjectLoop -
                            if (scriptCode[state.scriptCodePtr++] == 1) {
                                QString value = "-" + arrayPosition[scriptCode[state.scriptCodePtr++]];
                                variableName[i] = setArrayValue(
                                    variablePtrs->at(scriptCode[state.scriptCodePtr++]), value);
                            }
                            else {
                                QString value =
                                    "-" + QString::number(scriptCode[state.scriptCodePtr++]);
                                variableName[i] = setArrayValue(
                                    variablePtrs->at(scriptCode[state.scriptCodePtr++]), value);
                            }
                            break;
                    }
                    break;
                case 2: // Read constant value from bytecode
                    variableName[i] = QString::number(scriptCode[state.scriptCodePtr++]);
                    break;
                case 3: // Read string
                    QString tmp = "";
                    int strLen  = scriptCode[state.scriptCodePtr];
                    for (int j = 0; j < strLen;) {
                        state.scriptCodePtr++;
                        if (j < strLen) {
                            int val = scriptCode[state.scriptCodePtr] >> 24;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;

                        if (j < strLen) {
                            int val = (scriptCode[state.scriptCodePtr] & 0x00FFFFFF) >> 16;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;

                        if (j < strLen) {
                            int val = (scriptCode[state.scriptCodePtr] & 0x0000FFFF) >> 8;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;

                        if (j < strLen) {
                            int val = scriptCode[state.scriptCodePtr] & 0x000000FF;
                            if (val < 0)
                                val = 0;
                            tmp = tmp + (char)val;
                        }
                        j++;
                    }
                    variableName[i] = '"' + tmp + '"';
                    if ((strLen & 3) == 0) {
                        state.scriptCodePtr += 2;
                        break;
                    }
                    state.scriptCodePtr++;
                    break;
            }
        }

        QString operand = functionPtrs->at(opcode).name;
        if (operand == "return")
            opcode = FUNC_RETURN;

        /*if (operand == "End" || operand == "EndFunction") {
            if (isFunction)
                writer.writeText("end function");
            else
                writer.writeText("end event");
            state.endFlag = true;
            state.deep    = 0;
        }*/

        if (!state.endFlag) {
            // Check what opcodes terminates a statement
            switch (opcode) {
                case FUNC_END: break;
                case FUNC_ELSE:
                    for (int i = 0; i < state.deep - 1; ++i) writer.writeText("\t");
                    break;
                case FUNC_ENDIF:
                    state.deep--;
                    for (int i = 0; i < state.deep; ++i) writer.writeText("\t");
                    break;
                case FUNC_LOOP:
                case FUNC_NEXT:
                    state.deep--;
                    for (int i = 0; i < state.deep; ++i) writer.writeText("\t");
                    break;
                case FUNC_ENDSWITCH:
                    state.deep--;
                    for (int i = 0; i < state.deep; ++i) writer.writeText("\t");
                    break;
                case FUNC_RETURN:
                    if (state.deep <= 1)
                        break;
                    [[fallthrough]];
                default:
                    for (int i = 0; i < state.deep; ++i) writer.writeText("\t");
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
                if (opcode >= functionPtrs->count()) {
                    writer.writeText("ERROR AT: " + QString::number(state.scriptCodePtr) + " : "
                                     + opcode);
                    PrintLog("OPCODE ABOVE THE MAX OPCODES");
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
                    if (ok && id < typeNames.count())
                        variableName[1] = "TypeName[" + typeNames[id] + "]";
                }

                if (variableName[0].contains("animalType")) {
                    bool ok = false;
                    int id  = variableName[1].toInt(&ok);
                    if (ok && id < typeNames.count())
                        variableName[1] = "TypeName[" + typeNames[id] + "]";
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
                    if (ok && id < typeNames.count())
                        variableName[1] = "TypeName[" + typeNames[id] + "]";
                }

                if (variableName[1].contains("object") && variableName[1].contains(".type")) {
                    bool ok = false;
                    int id  = variableName[2].toInt(&ok);
                    if (ok && id < typeNames.count())
                        variableName[2] = "TypeName[" + typeNames[id] + "]";
                }

                if (variableName[1].contains("animalType")) {
                    bool ok = false;
                    int id  = variableName[2].toInt(&ok);
                    if (ok && id < typeNames.count())
                        variableName[2] = "TypeName[" + typeNames[id] + "]";
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
                        variableName[1] = functionNames[id];
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
                        if (ok && id < typeNames.count())
                            variableName[0] = "TypeName[" + typeNames[id] + "]";
                        break;
                    }
                    case 4: //"ResetObjectEntity"
                    {
                        bool ok = false;
                        int id  = variableName[1].toInt(&ok);
                        if (ok && id < typeNames.count())
                            variableName[1] = "TypeName[" + typeNames[id] + "]";
                        break;
                    }
                    case 5: //"PlaySfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id < sfxNames.count())
                                variableName[0] = "SfxName[" + sfxNames[id] + "]";
                        break;
                    }
                    case 6: //"StopSfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id < sfxNames.count())
                                variableName[0] = "SfxName[" + sfxNames[id] + "]";
                        break;
                    }
                    case 7: //"PlayStageSfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id + globalSFXCount < sfxNames.count())
                                variableName[0] = "SfxName[" + sfxNames[id + globalSFXCount] + "]";
                        break;
                    }
                    case 8: //"StopStageSfx"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id + globalSFXCount < sfxNames.count())
                                variableName[0] = "SfxName[" + sfxNames[id + globalSFXCount] + "]";
                        break;
                    }
                    case 9: //"SetSfxAttributes"
                    {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok)
                            if (id < sfxNames.count())
                                variableName[0] = "SfxName[" + sfxNames[id] + "]";
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
                    state.endFlag = true;
                    state.deep    = 0;
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
                    ++state.deep;
                    break;
                case FUNC_IFGREATER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1]) + " > "
                                     + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_IFGREATEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + " >= " + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_IFLOWER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1]) + " < "
                                     + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_IFLOWEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + " <= " + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_IFNOTEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("if " + toHexString(variableName[1])
                                     + " != " + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_ELSE: writer.writeText("else"); break;
                case FUNC_ENDIF: writer.writeText("end if"); break;
                case FUNC_WEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + " == " + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_WGREATER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1]) + " > "
                                     + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_WGREATEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + " >= " + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_WLOWER:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1]) + " < "
                                     + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_WLOWEROREQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + " <= " + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_WNOTEQUAL:
                    // JumpTableOffset = Int32.Parse(variableName[0]);
                    writer.writeText("while " + toHexString(variableName[1])
                                     + " != " + toHexString(variableName[2]));
                    ++state.deep;
                    break;
                case FUNC_LOOP: writer.writeText("loop"); break;
                case FUNC_FOREACHACTIVE: {
                    QString type = "TypeName[";
                    bool ok      = false;
                    int t        = variableName[1].toInt(&ok);
                    QString tn   = "";
                    if (ok) {
                        if (t < typeNames.count() && t >= 0) {
                            type = typeNames[t];
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
                    ++state.deep;
                    break;
                }
                case FUNC_FOREACHALL: {
                    QString type = "TypeName[";
                    bool ok      = false;
                    int t        = variableName[1].toInt(&ok);
                    QString tn   = "";
                    if (ok) {
                        if (t < typeNames.count() && t >= 0) {
                            type = typeNames[t];
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
                    ++state.deep;
                    break;
                }
                case FUNC_NEXT: writer.writeText("next"); break;
                case FUNC_SWITCH: {
                    writer.writeText("switch " + toHexString(variableName[1]));

                    ++state.switchDeep;
                    ++state.deep;
                    SwitchState &sw = state.switchState[state.switchDeep];
                    bool ok         = false;
                    int jmpOffset   = variableName[0].toInt(&ok) + state.jumpTableOffset;
                    if (!ok) {
                        PrintLog("uh oh");
                    }

                    sw.jumpTableOffset = jmpOffset;

                    int lowCase  = jumpTable[jmpOffset + 0];
                    int highCase = jumpTable[jmpOffset + 1];
                    int defJump  = jumpTable[jmpOffset + 2];
                    int endJmp   = jumpTable[jmpOffset + 3];

                    sw.lowCase    = lowCase;
                    sw.highCase   = highCase;
                    sw.defaultJmp = defJump;
                    sw.endJmp     = endJmp;
                    sw.caseID     = 0;

                    sw.jumpPtr.clear();
                    int caseID = 0;

                    // Load Data for Switches
                    for (int c = lowCase; c <= highCase + 1; c++) {
                        if (c == highCase + 1) {                 // Check if default
                            if (sw.endJmp - sw.defaultJmp > 1) { // Default
                                int jumpPtr = state.scriptCodeOffset + sw.defaultJmp;
                                int match   = -1;

                                for (int i = 0; i < sw.jumpPtr.count(); ++i) {
                                    if (jumpPtr == sw.jumpPtr[i].jump) {
                                        match = i;
                                        break;
                                    }
                                }

                                if (match > -1) // fallthrough case
                                    sw.jumpPtr[match].cases.append(c);
                                else // new case
                                {
                                    sw.jumpPtr.append(SwitchJumpPtr());
                                    sw.jumpPtr[caseID].cases.append(c);

                                    sw.jumpPtr[caseID].jump = jumpPtr;
                                    ++caseID;
                                }
                            }
                        }
                        else {
                            int jumpTblPtr = jmpOffset + (c - lowCase) + 4;
                            int jumpPtr    = state.scriptCodeOffset + jumpTable[jumpTblPtr];
                            int match      = -1;

                            for (int i = 0; i < sw.jumpPtr.count(); ++i) {
                                if (jumpPtr == sw.jumpPtr[i].jump) {
                                    match = i;
                                    break;
                                }
                            }

                            if (match > -1) // fallthrough case
                                sw.jumpPtr[match].cases.append(c);
                            else // new case
                            {
                                sw.jumpPtr.append(SwitchJumpPtr());
                                sw.jumpPtr[caseID].cases.append(c);

                                sw.jumpPtr[caseID].jump = jumpPtr;
                                ++caseID;
                            }
                        }
                    }

                    // Sort Switches by JumpPtr
                    if (sw.jumpPtr.count() > 0) {
                        std::sort(sw.jumpPtr.begin(), sw.jumpPtr.end(),
                                  [](const SwitchJumpPtr &a, const SwitchJumpPtr &b) -> bool {
                                      return a.jump < b.jump;
                                  });
                    }
                    break;
                }
                case FUNC_BREAK: writer.writeText("break"); break;
                case FUNC_ENDSWITCH:
                    writer.writeText("end switch");
                    state.switchDeep--;
                    break;
                case FUNC_RETURN:
                    if (state.deep <= 1) {
                        writer.writeText("end function");
                        state.endFlag = true;
                        state.deep    = 0;
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
                                QString("%1_table%2").arg(curTypeNamev4).arg(tables.count()), index);

                            if (!tables.contains(c)) {
                                tables.append(c);
                                variableName[2] = c.name;
                            }
                            else {
                                variableName[2] = tables[tables.indexOf(c)].name;
                            }
                        }
                    }

                    if (operand == "SetTableValue") {
                        bool ok   = false;
                        int index = variableName[2].toInt(&ok);
                        if (ok) {
                            StaticVarInfo c(
                                QString("%1_table%2").arg(curTypeNamev4).arg(tables.count()), index);

                            if (!tables.contains(c)) {
                                tables.append(c);
                                variableName[2] = c.name;
                            }
                            else {
                                variableName[2] = tables[tables.indexOf(c)].name;
                            }
                        }

                        if (debugNameFlag) {
                            if (variableName[2] == "DebugMode_table12") {
                                bool ok = false;
                                int id  = variableName[0].toInt(&ok);
                                if (ok && id < typeNames.count())
                                    variableName[0] = "TypeName[" + typeNames[id] + "]";
                            }
                            if (variableName[2] == "DebugMode_table13") {
                                bool ok = false;
                                int id  = variableName[0].toInt(&ok);
                                if (ok) {
                                    if (debugNameFlag == 1 && id < functionNames.count()) {
                                        variableName[0] = curTypeNamev4 + "_DebugDraw";

                                        for (int i = 0; i < scriptPtrs.count(); ++i) {
                                            if (scriptPtrs[i].name == functionNames[id]) {
                                                scriptPtrs[i].name = curTypeNamev4 + "_DebugDraw";
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
                                    if (debugNameFlag == 1 && id < functionNames.count()) {
                                        variableName[0] = curTypeNamev4 + "_DebugSpawn";

                                        for (int i = 0; i < scriptPtrs.count(); ++i) {
                                            if (scriptPtrs[i].name == functionNames[id]) {
                                                scriptPtrs[i].name = curTypeNamev4 + "_DebugSpawn";
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

                    if (operand == "Print") {
                        bool ok = false;
                        int id  = variableName[0].toInt(&ok);
                        if (ok) {
                            if (id)
                                variableName[0] = "true";
                            else
                                variableName[0] = "false";
                        }

                        ok = false;
                        id = variableName[1].toInt(&ok);
                        if (ok) {
                            if (id)
                                variableName[1] = "true";
                            else
                                variableName[1] = "false";
                        }
                    }

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
