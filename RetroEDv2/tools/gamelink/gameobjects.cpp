#include "includes.hpp"
#include "tools/sceneeditorv5.hpp"

#include <RSDKv5/scenev5.hpp>

void FunctionTable::RegisterObjectv5(GameObject **sVars, const char *name, uint entityClassSize,
                                     uint staticClassSize, void (*update)(void),
                                     void (*lateUpdate)(void), void (*staticUpdate)(void),
                                     void (*draw)(void), void (*create)(void *),
                                     void (*stageLoad)(void), void (*editorDraw)(void),
                                     void (*editorLoad)(void), void (*serialize)(void))
{
    RegisterObjectv5U(sVars, name, entityClassSize, staticClassSize, update, lateUpdate, staticUpdate,
                      draw, create, stageLoad, editorDraw, editorLoad, serialize, NULL);
}
void FunctionTable::RegisterObjectv5U(GameObject **sVars, const char *name, uint entityClassSize,
                                      uint staticClassSize, void (*update)(void),
                                      void (*lateUpdate)(void), void (*staticUpdate)(void),
                                      void (*draw)(void), void (*create)(void *),
                                      void (*stageLoad)(void), void (*editorDraw)(void),
                                      void (*editorLoad)(void), void (*serialize)(void),
                                      void (*staticLoad)(void *sVars))
{
    GameObjectInfo info{};
    if (entityClassSize > sizeof(GameEntityBasev1))
        PrintLog("Class exceeds max entity memory: " + QString(name));
    QByteArray hashData = Utils::getMd5HashByteArray(QString(name));
    byte data[0x10];
    for (int i = 0; i < 0x10; ++i) data[i] = hashData[i];

    memcpy(info.hash, data, 0x10 * sizeof(byte));
    info.sVars           = sVars;
    info.entityClassSize = entityClassSize;
    info.staticClassSize = staticClassSize;
    info.update          = update;
    info.lateUpdate      = lateUpdate;
    info.staticUpdate    = staticUpdate;
    info.draw            = draw;
    info.create          = create;
    info.stageLoad       = stageLoad;
    info.editorDraw      = editorDraw;
    info.editorLoad      = editorLoad;
    info.serialize       = serialize;
    info.staticLoad      = staticLoad;
    info.name            = name;
    v5Editor->gameLinks.last().gameObjectList.append(info);
}

void FunctionTable::RegisterStaticVariables(GameObject **sVars, const char *name, uint classSize)
{
    if (!v5Editor)
        return;
    QByteArray hashData = Utils::getMd5HashByteArray(QString(name));
    byte data[0x10];
    for (int i = 0; i < 0x10; ++i) data[i] = hashData[i];
    uint hash[4];
    memcpy(hash, data, 0x10 * sizeof(byte));

    allocateStorage(v5Editor->dataStorage, classSize, (void **)sVars, DATASET_STG, true);

    GameObjectInfo info{};
    memcpy(info.hash, data, 0x10 * sizeof(byte));
    info.sVars           = sVars;
    info.entityClassSize = 0;
    info.staticClassSize = classSize;
    info.update          = NULL;
    info.lateUpdate      = NULL;
    info.staticUpdate    = NULL;
    info.draw            = NULL;
    info.create          = NULL;
    info.stageLoad       = NULL;
    info.editorDraw      = NULL;
    info.editorLoad      = NULL;
    info.serialize       = NULL;
    info.staticLoad      = NULL;
    info.name            = name;
    v5Editor->gameLinks.last().gameObjectList.append(info);
}

void FunctionTable::SetEditableVar(byte type, const char *name, byte object, int offset)
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

void FunctionTable::SetActiveVariable(int objectID, const char *name)
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
void FunctionTable::AddEnumVariable(const char *name)
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

ushort FunctionTable::FindObject(const char *name)
{
    if (!v5Editor)
        return 0;

    QByteArray hashData = Utils::getMd5HashByteArray(QString(name));
    byte data[0x10];
    for (int i = 0; i < 0x10; ++i) data[i] = hashData[i];
    uint hash[4];
    memcpy(hash, data, 0x10 * sizeof(byte));

    for (int o = 0; o < v5Editor->viewer->objects.count(); ++o) {
        GameObjectInfo *info = v5Editor->GetObjectInfo(name);
        if (info) {
            if (info->sVars && *info->sVars) {
                return o;
            }
        }
    }
    return 0;
}

void *FunctionTable::GetEntity(ushort entityID)
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

int FunctionTable::GetEntitySlot(void *entityPtr)
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

int FunctionTable::GetEntityCount(ushort type, bool32 isActive)
{
    if (!v5Editor)
        return 0;

    if (type >= TYPE_COUNT)
        return 0;
    if (isActive)
        return v5Editor->viewer->typeGroups[type].entries.count();

    int cnt = 0;
    switch (v5Editor->viewer->engineRevision) {
        case 1:
            for (int i = 0; i < ENTITY_COUNT_v5; ++i) {
                if (AS_ENTITY(v5Editor->viewer->gameEntityList, GameEntityv1)[i].classID == type)
                    cnt++;
            }
            break;

        case 2:
            for (int i = 0; i < ENTITY_COUNT_v5; ++i) {
                if (AS_ENTITY(v5Editor->viewer->gameEntityList, GameEntityv2)[i].classID == type)
                    cnt++;
            }
            break;

        default:
        case 3:
            for (int i = 0; i < ENTITY_COUNT_v5; ++i) {
                if (AS_ENTITY(v5Editor->viewer->gameEntityList, GameEntityvU)[i].classID == type)
                    cnt++;
            }
            break;
    }

    return cnt;
}

bool32 FunctionTable::GetActiveEntities(ushort group, void **entity)
{
    if (!v5Editor)
        return false;

    if (group >= TYPEGROUP_COUNT_v5)
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

    void *entityList = v5Editor->viewer->gameEntityList;
    auto *typeGroups = v5Editor->viewer->typeGroups;
    auto *stackPtr   = v5Editor->viewer->foreachStackPtr;

    switch (v5Editor->viewer->engineRevision) {
        case 1:
            for (GameEntityv1 *nextEnt = &AS_ENTITY(
                     entityList,
                     GameEntityv1)[v5Editor->viewer->typeGroups[group].entries[stackPtr->id]];
                 stackPtr->id < typeGroups[group].entries.count(); ++stackPtr->id,
                              nextEnt = &AS_ENTITY(
                                  entityList, GameEntityv1)[typeGroups[group].entries[stackPtr->id]]) {
                if (nextEnt->classID == group) {
                    *entity = nextEnt;
                    return true;
                }
            }
            break;

        case 2:
            for (GameEntityv2 *nextEnt = &AS_ENTITY(
                     entityList,
                     GameEntityv2)[v5Editor->viewer->typeGroups[group].entries[stackPtr->id]];
                 stackPtr->id < typeGroups[group].entries.count(); ++stackPtr->id,
                              nextEnt = &AS_ENTITY(
                                  entityList, GameEntityv2)[typeGroups[group].entries[stackPtr->id]]) {
                if (nextEnt->classID == group) {
                    *entity = nextEnt;
                    return true;
                }
            }
            break;

        default:
        case 3:
            for (GameEntityvU *nextEnt = &AS_ENTITY(
                     entityList,
                     GameEntityvU)[v5Editor->viewer->typeGroups[group].entries[stackPtr->id]];
                 stackPtr->id < typeGroups[group].entries.count(); ++stackPtr->id,
                              nextEnt = &AS_ENTITY(
                                  entityList, GameEntityvU)[typeGroups[group].entries[stackPtr->id]]) {
                if (nextEnt->classID == group) {
                    *entity = nextEnt;
                    return true;
                }
            }
            break;
    }

    v5Editor->viewer->foreachStackPtr--;
    return false;
}
bool32 FunctionTable::GetAllEntities(ushort type, void **entity)
{
    if (!v5Editor)
        return false;

    if (type >= OBJECT_COUNT_v5)
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

    void *entityList = v5Editor->viewer->gameEntityList;
    auto *stackPtr   = v5Editor->viewer->foreachStackPtr;

    switch (v5Editor->viewer->engineRevision) {
        case 1:
            for (GameEntityv1 *nextEnt = &AS_ENTITY(entityList, GameEntityv1)[stackPtr->id];
                 stackPtr->id < ENTITY_COUNT_v5;
                 ++stackPtr->id, nextEnt = &AS_ENTITY(entityList, GameEntityv1)[stackPtr->id]) {
                if (nextEnt->classID == type) {
                    *entity = nextEnt;
                    return true;
                }
            }
            break;

        case 2:
            for (GameEntityv2 *nextEnt = &AS_ENTITY(entityList, GameEntityv2)[stackPtr->id];
                 stackPtr->id < ENTITY_COUNT_v5;
                 ++stackPtr->id, nextEnt = &AS_ENTITY(entityList, GameEntityv2)[stackPtr->id]) {
                if (nextEnt->classID == type) {
                    *entity = nextEnt;
                    return true;
                }
            }
            break;

        default:
        case 3:
            for (GameEntityvU *nextEnt = &AS_ENTITY(entityList, GameEntityvU)[stackPtr->id];
                 stackPtr->id < ENTITY_COUNT_v5;
                 ++stackPtr->id, nextEnt = &AS_ENTITY(entityList, GameEntityvU)[stackPtr->id]) {
                if (nextEnt->classID == type) {
                    *entity = nextEnt;
                    return true;
                }
            }
            break;
    }

    v5Editor->viewer->foreachStackPtr--;
    return false;
}

void FunctionTable::BreakForeachLoop()
{
    if (!v5Editor)
        return;
    --v5Editor->viewer->foreachStackPtr;
}

bool32 FunctionTable::CheckOnScreen(void *entity, Vector2<int> *range)
{
    if (!entity)
        return false;

    // TODO!
    if (range) { /*
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
         }//*/
    }

    return false;
}
bool32 FunctionTable::CheckPosOnScreen(Vector2<int> *position, Vector2<int> *range)
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
