#ifndef GAMEOBJECTS_HPP
#define GAMEOBJECTS_HPP

#define v5_OBJECT_COUNT (0x400)
// 0x800 scene objects, 0x40 reserved ones, and 0x100 spare slots for creation
#define v5_RESERVE_ENTITY_COUNT (0x40)
#define v5_TEMPENTITY_COUNT     (0x100)
#define v5_SCENEENTITY_COUNT    (0x800)
#define v5_ENTITY_COUNT         (v5_RESERVE_ENTITY_COUNT + v5_SCENEENTITY_COUNT + v5_TEMPENTITY_COUNT)
#define v5_TEMPENTITY_START     (v5_ENTITY_COUNT - v5_TEMPENTITY_COUNT)

#define TYPE_COUNT      (0x100)
#define TYPEGROUP_COUNT (0x104)

#define FOREACH_STACK_COUNT (0x400)

enum ActiveFlags {
    ACTIVE_NEVER,   // never update
    ACTIVE_ALWAYS,  // always update (even if paused/frozen)
    ACTIVE_NORMAL,  // always update (unless paused/frozen)
    ACTIVE_PAUSED,  // update if paused
    ACTIVE_BOUNDS,  // update if in x & y bounds
    ACTIVE_XBOUNDS, // update only if in x bounds (y bounds dont matter)
    ACTIVE_YBOUNDS, // update only if in y bounds (x bounds dont matter)
    ACTIVE_RBOUNDS, // update based on radius boundaries (updateRange.x = radius)
};

struct ForeachStackInfo {
    int id;
};

struct TypeGroupList {
    int entryCount;
    ushort entries[v5_ENTITY_COUNT];
};

extern int gameObjectCount;
extern GameObjectInfo gameObjectList[v5_OBJECT_COUNT];

extern GameObject *blankObject;

namespace FunctionTable
{
void registerObject(GameObject **structPtr, const char *name, uint entitySize, uint objectSize,
                    void (*update)(void), void (*lateUpdate)(void), void (*staticUpdate)(void),
                    void (*draw)(void), void (*create)(void *), void (*stageLoad)(void),
                    void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void));

void registerObjectContainer(GameObject **structPtr, const char *name, uint objectSize);

void setEditableVar(byte type, const char *name, byte obkect, int offset);

void setActiveVariable(int objectID, const char *name);
void addEnumVar(const char *name);

ushort getObjectByName(const char *name);
GameEntity *getObjectByID(ushort objectID);

int getEntityID(GameEntityBase *entityPtr);

int getEntityCount(ushort type, bool32 isActive);

void resetEntityPtr(GameEntity *entity, ushort type, void *data);
void resetEntitySlot(ushort slotID, ushort type, void *data);
void createEntity(ushort type, void *data, int x, int y);

inline void copyEntity(void *destEntity, void *srcEntity, bool32 clearSrcEntity)
{
    if (destEntity && srcEntity) {
        memcpy(destEntity, srcEntity, sizeof(GameEntityBase));
        if (clearSrcEntity)
            memset(srcEntity, 0, sizeof(GameEntityBase));
    }
}

bool32 getActiveEntities(ushort group, GameEntity **entity);
bool32 getEntities(ushort type, GameEntity **entity);

void breakForeachLoop();

bool32 checkOnScreen(GameEntity *entity, Vector2<int> *range);
bool32 checkPosOnScreen(Vector2<int> *position, Vector2<int> *range);
} // namespace FunctionTable
#endif
