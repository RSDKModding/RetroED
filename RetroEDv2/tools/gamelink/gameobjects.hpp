#ifndef GAMEOBJECTS_HPP
#define GAMEOBJECTS_HPP
namespace FunctionTable {
    void registerObject(GameObject **structPtr, const char *name, uint entitySize, uint objectSize,
                    void (*update)(void), void (*lateUpdate)(void), void (*staticUpdate)(void),
                    void (*draw)(void), void (*create)(void *), void (*stageLoad)(void),
                    void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void));

    void setEditableVar(byte type, const char* name, byte obkect, int offset);
}
#endif