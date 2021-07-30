#include "includes.hpp"

void FunctionTable::registerObject(GameObject **structPtr, const char *name, uint entitySize, uint objectSize,
                    void (*update)(void), void (*lateUpdate)(void), void (*staticUpdate)(void),
                    void (*draw)(void), void (*create)(void *), void (*stageLoad)(void),
                    void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void))
{
    if (gameObjectCount < OBJECT_COUNT) {
        GameObjectInfo *info = &gameObjectList[gameObjectCount];
        if (entitySize > sizeof(GameEntityBase))
            printf("Class exceeds max entity memory: %s \n", name);
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

void FunctionTable::setEditableVar(byte type, const char *name, byte object, int offset) {
    if (!v5Editor) return;
    v5Editor->viewer->objects[object].variables.append(VariableInfo(name, type, offset));
}
