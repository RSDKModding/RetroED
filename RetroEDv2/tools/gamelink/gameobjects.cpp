#include "includes.hpp"

void FunctionTable::registerObject(GameObject **structPtr, const char *name, uint entitySize,
                                   uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                                   void (*staticUpdate)(void), void (*draw)(void),
                                   void (*create)(void *), void (*stageLoad)(void),
                                   void (*editorDraw)(void), void (*editorLoad)(void),
                                   void (*serialize)(void))
{

    GameObjectInfo info;
    if (entitySize > sizeof(GameEntityBase))
        printLog("Class exceeds max entity memory: " + QString(name));
    QByteArray hashData = Utils::getMd5HashByteArray(QString(name));
    byte data[0x10];
    for (int i = 0; i < 0x10; ++i) data[i] = hashData[i];

    memcpy(info.hash, data, 0x10 * sizeof(byte));
    info.type         = structPtr;
    info.entitySize   = entitySize;
    info.objectSize   = objectSize;
    info.update       = update;
    info.lateUpdate   = lateUpdate;
    info.staticUpdate = staticUpdate;
    info.draw         = draw;
    info.create       = create;
    info.stageLoad    = stageLoad;
    info.editorDraw   = editorDraw;
    info.editorLoad   = editorLoad;
    info.serialize    = serialize;
    info.name         = name;
    v5Editor->gameLinks.last().gameObjectList.append(info);
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

    allocateStorage(v5Editor->dataStorage, objectSize, (void **)structPtr, DATASET_STG, true);

    GameObjectInfo info;
    memcpy(info.hash, data, 0x10 * sizeof(byte));
    info.type         = structPtr;
    info.entitySize   = 0;
    info.objectSize   = objectSize;
    info.update       = NULL;
    info.lateUpdate   = NULL;
    info.staticUpdate = NULL;
    info.draw         = NULL;
    info.create       = NULL;
    info.stageLoad    = NULL;
    info.editorDraw   = NULL;
    info.editorLoad   = NULL;
    info.serialize    = NULL;
    info.name         = name;
    v5Editor->gameLinks.last().gameObjectList.append(info);
}

void FunctionTable::setEditableVar(byte type, const char *name, byte object, int offset)
{
    if (!v5Editor)
        return;

    QByteArray hash = Utils::getMd5HashByteArray(QString(name));
    for (int i = 0; i < v5Editor->viewer->objects[object].variables.count(); ++i) {
        if (hash == v5Editor->viewer->objects[object].variables[i].hash) {
            v5Editor->viewer->objects[object].variables[i] = VariableInfo(name, type, offset);
            for (auto &entity : v5Editor->viewer->entities) {
                if (entity.type == object) {
                    // if we can transfer values, we should
                    if (entity.variables[i].type <= VAR_BOOL && type <= VAR_BOOL) {
                        int value = 0;
                        switch (entity.variables[i].type) {
                            case VAR_UINT8: value = entity.variables[i].value_uint8; break;
                            case VAR_INT8: value = entity.variables[i].value_int8; break;
                            case VAR_UINT16: value = entity.variables[i].value_uint16; break;
                            case VAR_INT16: value = entity.variables[i].value_int16; break;
                            case VAR_UINT32: value = entity.variables[i].value_uint32; break;
                            case VAR_INT32: value = entity.variables[i].value_int32; break;
                            case VAR_ENUM: value = entity.variables[i].value_enum; break;
                            case VAR_BOOL: value = entity.variables[i].value_bool ? 1 : 0; break;
                        }

                        switch (type) {
                            case VAR_UINT8: entity.variables[i].value_uint8 = value; break;
                            case VAR_INT8: entity.variables[i].value_int8 = value; break;
                            case VAR_UINT16: entity.variables[i].value_uint16 = value; break;
                            case VAR_INT16: entity.variables[i].value_int16 = value; break;
                            case VAR_UINT32: entity.variables[i].value_uint32 = value; break;
                            case VAR_INT32: entity.variables[i].value_int32 = value; break;
                            case VAR_ENUM: entity.variables[i].value_enum = value; break;
                            case VAR_BOOL: entity.variables[i].value_bool = value != 0; break;
                        }
                    }

                    entity.variables[i].type = type;
                }
            }
            return;
        }
    }

    if (QString(name) == "filter") {
        v5Editor->viewer->objects[object].variables.insert(0, VariableInfo(name, type, offset));
        for (auto &entity : v5Editor->viewer->entities) {
            if (entity.type == object) {
                RSDKv5::Scene::VariableValue variable;
                variable.type        = type;
                variable.value_uint8 = 0xFF;
                entity.variables.insert(0, variable);
            }
        }
    }
    else {
        v5Editor->viewer->objects[object].variables.append(VariableInfo(name, type, offset));
        for (auto &entity : v5Editor->viewer->entities) {
            if (entity.type == object) {
                RSDKv5::Scene::VariableValue variable;
                variable.type = type;
                entity.variables.append(variable);
            }
        }
    }
}

void FunctionTable::setActiveVariable(int objectID, const char *name)
{
    if (!v5Editor)
        return;
    v5Editor->viewer->activeVar    = -1;
    v5Editor->viewer->activeVarObj = -1;

    // invalid obj
    if (objectID <= -1)
        return;

    int v = 0;
    for (auto &var : v5Editor->viewer->objects[objectID].variables) {
        if (var.name == name) {
            v5Editor->viewer->activeVar    = v;
            v5Editor->viewer->activeVarObj = objectID;
            break;
        }
        ++v;
    }
}
void FunctionTable::addEnumVar(const char *name)
{
    if (!v5Editor)
        return;

    if (v5Editor->viewer->activeVarObj >= 0) {
        int objectID = v5Editor->viewer->activeVarObj;
        int id       = v5Editor->viewer->activeVar;

        VariableValue value;
        value.name  = name;
        value.value = v5Editor->viewer->objects[objectID].variables[id].values.count();

        v5Editor->viewer->objects[objectID].variables[id].values.append(value);
    }
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
        GameObjectInfo *info = v5Editor->getObjectInfo(name);
        if (info) {
            if (info->type && *info->type) {
                return o;
            }
        }
    }
    return 0;
}

GameEntity *FunctionTable::getEntityByID(ushort entityID)
{
    if (!v5Editor)
        return NULL;

    for (auto &entity : v5Editor->viewer->entities) {
        if (entity.slotID == entityID) {
            return entity.gameEntity;
        }
    }

    return NULL;
}

int FunctionTable::getEntityID(GameEntityBase *entityPtr)
{
    if (!v5Editor)
        return 0;

    int id = 0;
    for (auto &entity : v5Editor->viewer->entities) {
        if (entity.gameEntity == entityPtr) {
            return id;
        }
        id++;
    }

    return 0;
}

int FunctionTable::getEntityCount(ushort type, bool32 isActive)
{
    if (!v5Editor)
        return 0;

    if (type >= TYPE_COUNT)
        return 0;
    if (isActive)
        return v5Editor->viewer->typeGroups[type].entries.count();

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
        GameObjectInfo *info = v5Editor->getObjectInfo(v5Editor->viewer->objects[type].name);
        if (!info)
            return;
        memset((void*)entity, 0, info->entitySize);
        if (info->create) {
            GameEntity *curEnt                              = v5Editor->viewer->sceneInfo.entity;
            v5Editor->viewer->sceneInfo.entity              = entity;
            v5Editor->viewer->sceneInfo.entity->interaction = true;
            info->create(data);
            v5Editor->viewer->sceneInfo.entity->objectID = type;
            v5Editor->viewer->sceneInfo.entity           = curEnt;
        }
        entity->objectID = type;
    }
}

void FunctionTable::resetEntitySlot(ushort slotID, ushort type, void *data)
{
    if (!v5Editor)
        return;

    short slot              = ENTITY_COUNT - 1;
    GameObjectInfo *objInfo = v5Editor->getObjectInfo(v5Editor->viewer->objects[type].name);
    if (!objInfo)
        return;
    if (slotID < ENTITY_COUNT)
        slot = slotID;
    GameEntity *entityPtr = &v5Editor->viewer->gameEntityList[slot];
    memset((void*)&v5Editor->viewer->gameEntityList[slot], 0, objInfo->entitySize);
    if (objInfo->create) {
        GameEntity *curEnt                 = v5Editor->viewer->sceneInfo.entity;
        v5Editor->viewer->sceneInfo.entity = entityPtr;
        entityPtr->interaction             = true;
        objInfo->create(data);
        v5Editor->viewer->sceneInfo.entity = curEnt;
        entityPtr->objectID                = type;
    }
    else {
        entityPtr->objectID = type;
    }
}

void FunctionTable::createEntity(ushort type, void *data, int x, int y)
{
    if (!v5Editor)
        return;

    GameObjectInfo *objInfo = v5Editor->getObjectInfo(v5Editor->viewer->objects[type].name);
    if (!objInfo)
        return;
    GameEntity *entityPtr = &v5Editor->viewer->gameEntityList[v5Editor->viewer->sceneInfo.createSlot];

    int permCnt = 0, loopCnt = 0;
    while (entityPtr->objectID) {
        // after 16 loops, the game says fuck it and will start overwriting non-temp objects
        if (!entityPtr->isPermanent && loopCnt >= 16)
            break;
        if (entityPtr->isPermanent)
            ++permCnt;
        v5Editor->viewer->sceneInfo.createSlot++;
        if (v5Editor->viewer->sceneInfo.createSlot == ENTITY_COUNT) {
            v5Editor->viewer->sceneInfo.createSlot = TEMPENTITY_START;
            entityPtr = &v5Editor->viewer->gameEntityList[v5Editor->viewer->sceneInfo.createSlot];
        }
        else {
            entityPtr = &v5Editor->viewer->gameEntityList[v5Editor->viewer->sceneInfo.createSlot];
        }
        if (permCnt >= v5_TEMPENTITY_COUNT)
            break;
        ++loopCnt;
    }

    memset((void*)entityPtr, 0, objInfo->entitySize);
    entityPtr->position.x  = x;
    entityPtr->position.y  = y;
    entityPtr->interaction = true;

    if (objInfo->create) {
        GameEntity *curEnt                 = v5Editor->viewer->sceneInfo.entity;
        v5Editor->viewer->sceneInfo.entity = entityPtr;
        objInfo->create(data);
        v5Editor->viewer->sceneInfo.entity = curEnt;
        entityPtr->objectID                = type;
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
         stackPtr->id < typeGroups[group].entries.count();
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
