#include "includes.hpp"

int gameObjectCount = 0;
GameObjectInfo gameObjectList[v5_OBJECT_COUNT];

GameObject *blankObject = NULL;

void FunctionTable::registerObject(GameObject **structPtr, const char *name, uint entitySize,
                                   uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                                   void (*staticUpdate)(void), void (*draw)(void),
                                   void (*create)(void *), void (*stageLoad)(void),
                                   void (*editorDraw)(void), void (*editorLoad)(void),
                                   void (*serialize)(void))
{
    if (gameObjectCount < v5_OBJECT_COUNT) {
        GameObjectInfo *info = &gameObjectList[gameObjectCount];
        if (entitySize > sizeof(GameEntityBase))
            qDebug() << "Class exceeds max entity memory: " + QString(name);
        QByteArray hashData = Utils::getMd5HashByteArray(QString(name));
        byte data[0x10];
        for (int i = 0; i < 0x10; ++i) data[i] = hashData[i];

        memcpy(info->hash, data, 0x10 * sizeof(byte));
        info->type         = structPtr;
        info->entitySize   = entitySize;
        info->objectSize   = objectSize;
        info->update       = update;
        info->lateUpdate   = lateUpdate;
        info->staticUpdate = staticUpdate;
        info->draw         = draw;
        info->create       = create;
        info->stageLoad    = stageLoad;
        info->editorDraw   = editorDraw;
        info->editorLoad   = editorLoad;
        info->serialize    = serialize;
        ++gameObjectCount;
    }
}

void FunctionTable::registerObjectContainer(GameObject **structPtr, const char *name, uint objectSize)
{
    if (!v5Editor)
        return;
    QByteArray hashData = Utils::getMd5HashByteArray(QString(name));
    byte data[0x10];
    for (int i = 0; i < 0x10; ++i) data[i] = hashData[i];
    uint hash[4];
    memcpy(hash, data, 0x10 * sizeof(byte));

    AllocateStorage(v5Editor->dataStorage, objectSize, (void **)structPtr, DATASET_STG, true);
    // LoadStaticObject((byte *)*structPtr, hash, 0);
}

void FunctionTable::setEditableVar(byte type, const char *name, byte object, int offset)
{
    if (!v5Editor)
        return;

    QByteArray hash = Utils::getMd5HashByteArray(QString(name));
    for (int i = 0; i < v5Editor->viewer->objects[object].variables.count(); ++i) {
        if (hash == v5Editor->viewer->objects[object].variables[i].hash) {
            v5Editor->viewer->objects[object].variables[i] = VariableInfo(name, type, offset);
            return;
        }
    }

    v5Editor->viewer->objects[object].variables.append(VariableInfo(name, type, offset));
}

void FunctionTable::setActiveVariable(int objectID, const char *name)
{
    if (!v5Editor)
        return;
    // SetActiveVariable() called
}
void FunctionTable::addEnumVar(const char *name)
{
    if (!v5Editor)
        return;
    // AddEnumVar() called
}

ushort FunctionTable::getObjectByName(const char *name)
{
    if (!v5Editor)
        return 0;

    QByteArray hashData = Utils::getMd5HashByteArray(QString(name));
    byte data[0x10];
    for (int i = 0; i < 0x10; ++i) data[i] = hashData[i];
    uint hash[4];
    memcpy(hash, data, 0x10 * sizeof(byte));

    for (int o = 0; o < v5Editor->viewer->objects.count(); ++o) {
        GameObjectInfo *info = gameLink.GetObjectInfo(name);
        if (info) {
            if (info->type && *info->type) {
                return o;
            }
        }
    }
    return 0;
}

GameEntity *FunctionTable::getObjectByID(ushort objectID)
{
    if (!v5Editor)
        return NULL;

    return &v5Editor->viewer->gameEntityList[objectID < ENTITY_COUNT ? objectID : (ENTITY_COUNT - 1)];
}

int FunctionTable::getEntityID(GameEntityBase *entityPtr)
{
    if (!v5Editor)
        return v5_ENTITY_COUNT - 1;

    return (int)(entityPtr - v5Editor->viewer->gameEntityList < v5_ENTITY_COUNT
                     ? entityPtr - v5Editor->viewer->gameEntityList
                     : 0);
}

int FunctionTable::getEntityCount(ushort type, bool32 isActive)
{
    if (!v5Editor)
        return 0;

    if (type >= TYPE_COUNT)
        return 0;
    if (isActive)
        return v5Editor->viewer->typeGroups[type].entryCount;

    int cnt = 0;
    for (int i = 0; i < ENTITY_COUNT; ++i) {
        if (v5Editor->viewer->gameEntityList[i].objectID == type)
            cnt++;
    }
    return cnt;
}

void FunctionTable::resetEntityPtr(GameEntity *entity, ushort type, void *data)
{
    if (!v5Editor)
        return;

    if (entity) {
        GameObjectInfo *info = gameLink.GetObjectInfo(v5Editor->viewer->objects[type].name);
        if (!info)
            return;
        memset(entity, 0, info->entitySize);
        if (info->create) {
            GameEntity *curEnt            = sceneInfo.entity;
            sceneInfo.entity              = entity;
            sceneInfo.entity->interaction = true;
            info->create(data);
            sceneInfo.entity->objectID = type;
            sceneInfo.entity           = curEnt;
        }
        entity->objectID = type;
    }
}

void FunctionTable::resetEntitySlot(ushort slotID, ushort type, void *data)
{
    if (!v5Editor)
        return;

    short slot              = ENTITY_COUNT - 1;
    GameObjectInfo *objInfo = gameLink.GetObjectInfo(v5Editor->viewer->objects[type].name);
    if (!objInfo)
        return;
    if (slotID < ENTITY_COUNT)
        slot = slotID;
    GameEntity *entityPtr = &v5Editor->viewer->gameEntityList[slot];
    memset(&v5Editor->viewer->gameEntityList[slot], 0, objInfo->entitySize);
    if (objInfo->create) {
        GameEntity *curEnt     = sceneInfo.entity;
        sceneInfo.entity       = entityPtr;
        entityPtr->interaction = true;
        objInfo->create(data);
        sceneInfo.entity    = curEnt;
        entityPtr->objectID = type;
    }
    else {
        entityPtr->objectID = type;
    }
}

void FunctionTable::createEntity(ushort type, void *data, int x, int y)
{
    if (!v5Editor)
        return;

    GameObjectInfo *objInfo = gameLink.GetObjectInfo(v5Editor->viewer->objects[type].name);
    if (!objInfo)
        return;
    GameEntity *entityPtr = &v5Editor->viewer->gameEntityList[sceneInfo.createSlot];

    int permCnt = 0, loopCnt = 0;
    while (entityPtr->objectID) {
        // after 16 loops, the game says fuck it and will start overwriting non-temp objects
        if (!entityPtr->isPermanent && loopCnt >= 16)
            break;
        if (entityPtr->isPermanent)
            ++permCnt;
        sceneInfo.createSlot++;
        if (sceneInfo.createSlot == ENTITY_COUNT) {
            sceneInfo.createSlot = TEMPENTITY_START;
            entityPtr            = &v5Editor->viewer->gameEntityList[sceneInfo.createSlot];
        }
        else {
            entityPtr = &v5Editor->viewer->gameEntityList[sceneInfo.createSlot];
        }
        if (permCnt >= v5_TEMPENTITY_COUNT)
            break;
        ++loopCnt;
    }

    memset(entityPtr, 0, objInfo->entitySize);
    entityPtr->position.x  = x;
    entityPtr->position.y  = y;
    entityPtr->interaction = true;

    if (objInfo->create) {
        GameEntity *curEnt = sceneInfo.entity;
        sceneInfo.entity   = entityPtr;
        objInfo->create(data);
        sceneInfo.entity    = curEnt;
        entityPtr->objectID = type;
    }
    else {
        entityPtr->objectID = type;
        entityPtr->active   = ACTIVE_NORMAL;
        entityPtr->visible  = true;
    }
}

bool32 FunctionTable::getActiveEntities(ushort group, GameEntity **entity)
{
    if (!v5Editor)
        return false;

    if (group >= TYPEGROUP_COUNT)
        return false;
    if (!entity)
        return false;

    if (*entity) {
        ++v5Editor->viewer->foreachStackPtr->id;
    }
    else {
        v5Editor->viewer->foreachStackPtr++;
        v5Editor->viewer->foreachStackPtr->id = 0;
    }

    auto *entityList = v5Editor->viewer->gameEntityList;
    auto *typeGroups = v5Editor->viewer->typeGroups;
    auto *stackPtr   = v5Editor->viewer->foreachStackPtr;

    for (GameEntity *nextEnt = &entityList[v5Editor->viewer->typeGroups[group].entries[stackPtr->id]];
         stackPtr->id < typeGroups[group].entryCount;
         ++stackPtr->id, nextEnt = &entityList[typeGroups[group].entries[stackPtr->id]]) {
        if (nextEnt->objectID == group) {
            *entity = nextEnt;
            return true;
        }
    }
    v5Editor->viewer->foreachStackPtr--;
    return false;
}
bool32 FunctionTable::getEntities(ushort type, GameEntity **entity)
{
    if (!v5Editor)
        return false;

    if (type >= OBJECT_COUNT)
        return false;
    if (!entity)
        return false;

    if (*entity) {
        ++v5Editor->viewer->foreachStackPtr->id;
    }
    else {
        v5Editor->viewer->foreachStackPtr++;
        v5Editor->viewer->foreachStackPtr->id = 0;
    }

    auto *entityList = v5Editor->viewer->gameEntityList;
    auto *stackPtr   = v5Editor->viewer->foreachStackPtr;

    for (GameEntity *nextEnt = &entityList[stackPtr->id]; stackPtr->id < ENTITY_COUNT;
         ++stackPtr->id, nextEnt = &entityList[stackPtr->id]) {
        if (nextEnt->objectID == type) {
            *entity = nextEnt;
            return true;
        }
    }

    v5Editor->viewer->foreachStackPtr--;
    return false;
}

void FunctionTable::breakForeachLoop()
{
    if (!v5Editor)
        return;
    --v5Editor->viewer->foreachStackPtr;
}

bool32 FunctionTable::checkOnScreen(GameEntity *entity, Vector2<int> *range)
{
    if (!entity)
        return false;

    // TODO!
    /*if (range) {
        for (int s = 0; s < cameraCount; ++s) {
            int sx = abs(entity->position.x - cameras[s].position.x);
            int sy = abs(entity->position.y - cameras[s].position.y);
            if (sx <= range->x + cameras[s].offset.x && sy <= range->y + cameras[s].offset.y) {
                return true;
            }
        }
    }
    else {
        for (int s = 0; s < cameraCount; ++s) {
            int sx = abs(entity->position.x - cameras[s].position.x);
            int sy = abs(entity->position.y - cameras[s].position.y);
            if (sx <= entity->updateRange.x + cameras[s].offset.x && sy <= entity->updateRange.y +
    cameras[s].offset.y) { return true;
            }
        }
    }*/

    return false;
}
bool32 FunctionTable::checkPosOnScreen(Vector2<int> *position, Vector2<int> *range)
{
    if (!position || !range)
        return false;

    // TODO!
    /*for (int s = 0; s < cameraCount; ++s) {
        int sx = abs(position->x - cameras[s].position.x);
        int sy = abs(position->y - cameras[s].position.y);
        if (sx <= range->x + cameras[s].offset.x && sy <= range->y + cameras[s].offset.y) {
            return true;
        }
    }*/

    return false;
}
