#ifndef GAMELINK_HPP
#define GAMELINK_HPP

typedef uint bool32;

struct GameObject {
    short objectID;
    byte active;
};

struct GameEntity {
    Vector2<int> position;
    Vector2<int> scale;
    Vector2<int> velocity;
    Vector2<int> updateRange;
    int angle;
    int alpha;
    int rotation;
    int groundVel;
    int depth;
    ushort group;
    ushort objectID;
    bool32 inBounds;
    bool32 isPermanent;
    bool32 tileCollisions;
    bool32 interaction;
    bool32 onGround;
    byte active;
    byte filter;
    byte direction;
    byte drawOrder;
    byte collisionLayers;
    byte collisionPlane;
    byte collisionMode;
    byte drawFX;
    byte inkEffect;
    byte visible;
    byte activeScreens;
};

struct GameEntityBase : GameEntity {
    void *data[0x100];
};

struct GameObjectInfo {
    uint hash[4];
    void (*update)(void);
    void (*lateUpdate)(void);
    void (*staticUpdate)(void);
    void (*draw)(void);
    void (*create)(void *data);
    void (*stageLoad)(void);
    void (*editorDraw)(void);
    void (*editorLoad)(void);
    void (*serialize)(void);
    GameObject **type;
    int entitySize;
    int objectSize;
};

struct SceneInfo {
    GameEntity *entity;
    void *listData;
    void *listCategory;
    int timeCounter;
    int currentDrawGroup;
    int currentScreenID;
    ushort listPos;
    ushort entitySlot;
    ushort createSlot;
    ushort classCount;
    bool32 inEditor;
    bool32 effectGizmo;
    bool32 debugMode;
    bool32 useGlobalScripts;
    bool32 timeEnabled;
    byte activeCategory;
    byte categoryCount;
    byte state;
    byte filter;
    byte milliseconds;
    byte seconds;
    byte minutes;
};

struct EngineInfo {
    char engineInfo[0x40];
    char gameSubname[0x100];
    char version[0x10];
};

typedef struct {
    int platform;
    int language;
    int region;
} SKUInfo;

struct InputState {
    bool32 down;
    bool32 press;
    int keyMap;
};

struct ControllerState {
    InputState keyUp;
    InputState keyDown;
    InputState keyLeft;
    InputState keyRight;
    InputState keyA;
    InputState keyB;
    InputState keyC;
    InputState keyX;
    InputState keyY;
    InputState keyZ;
    InputState keyStart;
    InputState keySelect;
};

struct AnalogState {
    InputState keyUp;
    InputState keyDown;
    InputState keyLeft;
    InputState keyRight;
    InputState keyStick;
    float deadzone;
    float hDelta;
    float vDelta;
};

struct TriggerState {
    InputState key1;
    InputState key2;
    float unknown1;
    float unknown2;
};

struct TouchMouseData {
    float x[0x10];
    float y[0x10];
    bool32 down[0x10];
    byte count;
};

struct UnknownInfo {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
};

#define SCREEN_XMAX  (1280)
#define SCREEN_YSIZE (240)
struct ScreenInfo {
    ushort frameBuffer[SCREEN_XMAX * SCREEN_YSIZE];
    Vector2<int> position;
    int width;
    int height;
    int centerX;
    int centerY;
    int pitch;
    int clipBound_X1;
    int clipBound_Y1;
    int clipBound_X2;
    int clipBound_Y2;
    int waterDrawPos;
};

struct GameInfo {
    void *functionPtrs;
    void *APIPtrs;
    SKUInfo *currentSKU;
    EngineInfo *engineInfo;
    SceneInfo *sceneInfo;
    ControllerState *controller;
    AnalogState *stickL;
    AnalogState *stickR;
    TriggerState *triggerL;
    TriggerState *triggerR;
    TouchMouseData *touchMouse;
    UnknownInfo *unknown;
    ScreenInfo *screenInfo;
};
#define v5_OBJECT_COUNT (0x400)
// 0x800 scene objects, 0x40 reserved ones, and 0x100 spare slots for creation
#define v5_RESERVE_ENTITY_COUNT (0x40)
#define v5_TEMPENTITY_COUNT     (0x100)
#define v5_SCENEENTITY_COUNT    (0x800)
#define v5_ENTITY_COUNT         (v5_RESERVE_ENTITY_COUNT + v5_SCENEENTITY_COUNT + v5_TEMPENTITY_COUNT)
#define v5_TEMPENTITY_START     (v5_ENTITY_COUNT - v5_TEMPENTITY_COUNT)

extern int objectCount;
extern GameObjectInfo objectList[v5_OBJECT_COUNT];
extern GameEntityBase objectEntityList[v5_ENTITY_COUNT];

extern SceneInfo sceneInfo;
extern EngineInfo engineInfo;
extern SKUInfo skuInfo;
extern ControllerState controller[5];
extern AnalogState stickL[5];
extern AnalogState stickR[5];
extern TriggerState triggerL[5];
extern TriggerState triggerR[5];
extern TouchMouseData touchMouse;
extern UnknownInfo unknownInfo;
extern ScreenInfo screens[4];

class GameLink
{
public:
    GameLink();

    void LinkGameObjects();
};

extern GameLink gameLink;

#endif // GAMELINK_HPP
