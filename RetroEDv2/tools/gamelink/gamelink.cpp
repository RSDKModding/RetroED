#include "include.hpp"

int objectCount = 0;
GameObjectInfo objectList[v5_OBJECT_COUNT];
GameEntityBase objectEntityList[v5_ENTITY_COUNT];

SceneInfo sceneInfo;
EngineInfo engineInfo;
SKUInfo skuInfo;
ControllerState controller[5];
AnalogState stickL[5];
AnalogState stickR[5];
TriggerState triggerL[5];
TriggerState triggerR[5];
TouchMouseData touchMouse;
UnknownInfo unknownInfo;
ScreenInfo screens[4];

GameLink gameLink;

GameLink::GameLink() {}

void GameLink::LinkGameObjects()
{
    memset(&objectList, 0, v5_OBJECT_COUNT * sizeof(GameObjectInfo));
    sceneInfo.classCount     = 0;
    sceneInfo.activeCategory = 0;
    sceneInfo.listPos        = 0;
    sceneInfo.state          = 1;
    sceneInfo.inEditor       = true;
    sceneInfo.debugMode      = true;

    skuInfo.platform = 0xFF;

    GameInfo info;

    // info.functionPtrs = RSDKFunctionTable;
    info.APIPtrs    = NULL;
    info.currentSKU = &skuInfo;
    info.engineInfo = &engineInfo;
    info.sceneInfo  = &sceneInfo;
    info.controller = controller;
    info.stickL     = stickL;
    info.stickR     = stickR;
    info.triggerL   = triggerL;
    info.triggerR   = triggerR;
    info.touchMouse = &touchMouse;
    info.unknown    = &unknownInfo;
    info.screenInfo = screens;

    // LinkGameLogic(&info);
}
