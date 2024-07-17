#pragma once

#define OBJECT_COUNT_v5 (0x400)
// 0x800 scene objects, 0x40 reserved ones, and 0x100 spare slots for creation
#define RESERVE_ENTITY_COUNT_v5 (0x40)
#define TEMPENTITY_COUNT_v5     (0x100)
#define SCENEENTITY_COUNT_v5    (0x800)
#define ENTITY_COUNT_v5         (RESERVE_ENTITY_COUNT_v5 + SCENEENTITY_COUNT_v5 + TEMPENTITY_COUNT_v5)
#define TEMPENTITY_START_v5     (ENTITY_COUNT_v5 - TEMPENTITY_COUNT_v5)

#define TYPE_COUNT      (0x100)
#define TYPEGROUP_COUNT_v5 (0x104)

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
    QList<ushort> entries;
};

namespace FunctionTable
{
void RegisterObjectv5(GameObject **sVars, const char *name, uint entityClassSize, uint staticClassSize,
                      void (*update)(void), void (*lateUpdate)(void), void (*staticUpdate)(void),
                      void (*draw)(void), void (*create)(void *), void (*stageLoad)(void),
                      void (*editorLoad)(void), void (*editorDraw)(void), void (*serialize)(void));
void RegisterObjectv5U(GameObject **sVars, const char *name, uint entityClassSize, uint staticClassSize,
                       void (*update)(void), void (*lateUpdate)(void), void (*staticUpdate)(void),
                       void (*draw)(void), void (*create)(void *), void (*stageLoad)(void),
                       void (*editorLoad)(void), void (*editorDraw)(void), void (*serialize)(void),
                       void (*staticLoad)(void *sVars));

void RegisterStaticVariables(GameObject **sVars, const char *name, uint classSize);

void SetEditableVar(byte type, const char *name, byte obkect, int offset);

void SetActiveVariable(int objectID, const char *name);
void AddEnumVariable(const char *name);

ushort FindObject(const char *name);
void *GetEntity(ushort objectID);

int GetEntitySlot(void *entityPtr);

int GetEntityCount(ushort type, bool32 isActive);

bool32 GetActiveEntities(ushort group, void **entity);
bool32 GetAllEntities(ushort type, void **entity);

void BreakForeachLoop();

bool32 CheckOnScreen(void *entity, Vector2<int> *range);
bool32 CheckPosOnScreen(Vector2<int> *position, Vector2<int> *range);
} // namespace FunctionTable
