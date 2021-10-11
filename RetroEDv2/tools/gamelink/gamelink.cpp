#include "includes.hpp"

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

enum FunctionTableIDs {
    FunctionTable_InitGameOptions,
    FunctionTable_RegisterObject,
    FunctionTable_RegisterObjectContainer,
    FunctionTable_GetActiveEntities,
    FunctionTable_GetEntities,
    FunctionTable_BreakForeachLoop,
    FunctionTable_SetEditableVar,
    FunctionTable_GetObjectByID,
    FunctionTable_GetEntityID,
    FunctionTable_GetEntityCount,
    FunctionTable_GetDrawListRef,
    FunctionTable_GetDrawListRefPtr,
    FunctionTable_ResetEntityPtr,
    FunctionTable_ResetEntitySlot,
    FunctionTable_CreateEntity,
    FunctionTable_CopyEntity,
    FunctionTable_CheckOnScreen,
    FunctionTable_CheckPosOnScreen,
    FunctionTable_AddDrawListRef,
    FunctionTable_SwapDrawLayers,
    FunctionTable_SetDrawLayerProperties,
    FunctionTable_LoadScene,
    FunctionTable_SetGameMode,
    FunctionTable_SetHardResetFlag,
    FunctionTable_CheckValidScene,
    FunctionTable_CheckSceneFolder,
    FunctionTable_InitSceneLoad,
    FunctionTable_GetObjectByName,
    FunctionTable_ClearScreens,
    FunctionTable_AddScreen,
    FunctionTable_GetSettingsValue,
    FunctionTable_SetSettingsValue,
    FunctionTable_UpdateWindow,
    FunctionTable_Sin1024,
    FunctionTable_Cos1024,
    FunctionTable_ATan1024,
    FunctionTable_ASin1024,
    FunctionTable_ACos1024,
    FunctionTable_Sin512,
    FunctionTable_Cos512,
    FunctionTable_ATan512,
    FunctionTable_ASin512,
    FunctionTable_ACos512,
    FunctionTable_Sin256,
    FunctionTable_Cos256,
    FunctionTable_ATan256,
    FunctionTable_ASin256,
    FunctionTable_ACos256,
    FunctionTable_Rand,
    FunctionTable_Random,
    FunctionTable_SetRandKey,
    FunctionTable_ATan2,
    FunctionTable_SetIdentityMatrix,
    FunctionTable_MatrixMultiply,
    FunctionTable_MatrixTranslateXYZ,
    FunctionTable_MatrixScaleXYZ,
    FunctionTable_MatrixRotateX,
    FunctionTable_MatrixRotateY,
    FunctionTable_MatrixRotateZ,
    FunctionTable_MatrixRotateXYZ,
    FunctionTable_MatrixInverse,
    FunctionTable_MatrixCopy,
    FunctionTable_SetText,
    FunctionTable_CopyString,
    FunctionTable_PrependText,
    FunctionTable_AppendString,
    FunctionTable_AppendText,
    FunctionTable_LoadStrings,
    FunctionTable_SplitStringList,
    FunctionTable_GetCString,
    FunctionTable_StringCompare,
    FunctionTable_GetDisplayInfo,
    FunctionTable_GetWindowSize,
    FunctionTable_SetScreenSize,
    FunctionTable_SetClipBounds,
    FunctionTable_SetScreenSplitVerticies,
    FunctionTable_LoadSpriteSheet,
    FunctionTable_SetLookupTable,
    FunctionTable_SetPaletteMask,
    FunctionTable_SetPaletteEntry,
    FunctionTable_GetPaletteEntry,
    FunctionTable_SetActivePalette,
    FunctionTable_CopyPalette,
    FunctionTable_LoadPalette,
    FunctionTable_RotatePalette,
    FunctionTable_SetLimitedFade,
    FunctionTable_BlendColours,
    FunctionTable_DrawRect,
    FunctionTable_DrawLine,
    FunctionTable_DrawCircle,
    FunctionTable_DrawCircleOutline,
    FunctionTable_DrawFace,
    FunctionTable_DrawBlendedFace,
    FunctionTable_DrawSprite,
    FunctionTable_DrawDeformed,
    FunctionTable_DrawText,
    FunctionTable_DrawTile,
    FunctionTable_CopyTile,
    FunctionTable_DrawAniTile,
    FunctionTable_FillScreen,
    FunctionTable_LoadMesh,
    FunctionTable_Create3DScene,
    FunctionTable_Prepare3DScene,
    FunctionTable_SetDiffuseColour,
    FunctionTable_SetDiffuseIntensity,
    FunctionTable_SetSpecularIntensity,
    FunctionTable_SetupMesh,
    FunctionTable_SetModelAnimation,
    FunctionTable_SetupMeshAnimation,
    FunctionTable_Draw3DScene,
    FunctionTable_LoadAnimation,
    FunctionTable_CreateAnimation,
    FunctionTable_SetSpriteAnimation,
    FunctionTable_EditAnimation,
    FunctionTable_SetSpriteString,
    FunctionTable_GetAnimation,
    FunctionTable_GetFrame,
    FunctionTable_GetHitbox,
    FunctionTable_GetFrameID,
    FunctionTable_GetStringWidth,
    FunctionTable_ProcessAnimation,
    FunctionTable_GetSceneLayerID,
    FunctionTable_GetSceneLayer,
    FunctionTable_GetLayerSize,
    FunctionTable_GetTileInfo,
    FunctionTable_SetTileInfo,
    FunctionTable_CopyTileLayer,
    FunctionTable_ProcessParallax,
    FunctionTable_GetScanlines,
    FunctionTable_CheckObjectCollisionTouch,
    FunctionTable_CheckObjectCollisionPoint,
    FunctionTable_CheckObjectCollisionBox,
    FunctionTable_CheckObjectCollisionPlatform,
    FunctionTable_ObjectTileCollision,
    FunctionTable_ObjectTileGrip,
    FunctionTable_ProcessTileCollisions,
    FunctionTable_GetTileAngle,
    FunctionTable_SetTileAngle,
    FunctionTable_GetTileBehaviour,
    FunctionTable_SetTileBehaviour,
    FunctionTable_GetSFX,
    FunctionTable_PlaySFX,
    FunctionTable_StopSFX,
    FunctionTable_PlayMusic,
    FunctionTable_SetChannelAttributes,
    FunctionTable_StopChannel,
    FunctionTable_PauseChannel,
    FunctionTable_ResumeChannel,
    FunctionTable_SfxPlaying,
    FunctionTable_ChannelActive,
    FunctionTable_GetChannelPos,
    FunctionTable_LoadVideo,
    FunctionTable_LoadImage,
    FunctionTable_ControllerIDForInputID,
    FunctionTable_MostRecentActiveControllerID,
    FunctionTable_Unknown100,
    FunctionTable_GetAssignedControllerID,
    FunctionTable_GetAssignedUnknown,
    FunctionTable_Missing22,
    FunctionTable_Missing23,
    FunctionTable_Missing24,
    FunctionTable_Missing25,
    FunctionTable_Missing26,
    FunctionTable_AssignControllerID,
    FunctionTable_InputIDIsDisconnected,
    FunctionTable_ResetControllerAssignments,
    FunctionTable_LoadUserFile,
    FunctionTable_SaveUserFile,
    FunctionTable_printLog,
    FunctionTable_printString,
    FunctionTable_printText,
    FunctionTable_printIntegerUnsigned,
    FunctionTable_printInteger,
    FunctionTable_printFloat,
    FunctionTable_printVector2,
    FunctionTable_printHitbox,
    FunctionTable_Unknown105,
    FunctionTable_Unknown106,
    FunctionTable_ClearDebugValues,
    FunctionTable_SetDebugValue,
    FunctionTable_Count,
};

void *RSDKFunctionTable[FunctionTable_Count];
void nullFunc() {}

#define addToRSDKFunctionTable(id, func) RSDKFunctionTable[id] = (void *)func;

byte *gameGlobalVariablesPtr = NULL;
void FunctionTable::initGameOptions(void **options, int size)
{
    gameGlobalVariablesPtr = NULL;
    if (!v5Editor)
        return;
    allocateStorage(v5Editor->dataStorage, size, options, DATASET_STG, true);
    gameGlobalVariablesPtr = (byte *)*options;
}

GameLink gameLink;

GameLink::GameLink() {}

void GameLink::Setup()
{
    using namespace FunctionTable;
    calculateTrigAngles();

    // Function Table
    addToRSDKFunctionTable(FunctionTable_InitGameOptions, initGameOptions);
    addToRSDKFunctionTable(FunctionTable_RegisterObject, registerObject);
    addToRSDKFunctionTable(FunctionTable_RegisterObjectContainer, registerObjectContainer);
    addToRSDKFunctionTable(FunctionTable_GetActiveEntities, getActiveEntities);
    addToRSDKFunctionTable(FunctionTable_GetEntities, getEntities);
    addToRSDKFunctionTable(FunctionTable_BreakForeachLoop, breakForeachLoop);
    addToRSDKFunctionTable(FunctionTable_SetEditableVar, setEditableVar);
    addToRSDKFunctionTable(FunctionTable_GetObjectByID, getObjectByID);
    addToRSDKFunctionTable(FunctionTable_GetEntityID, getEntityID);
    addToRSDKFunctionTable(FunctionTable_GetEntityCount, getEntityCount);
    addToRSDKFunctionTable(FunctionTable_GetDrawListRef, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetDrawListRefPtr, nullFunc);
    addToRSDKFunctionTable(FunctionTable_ResetEntityPtr, resetEntityPtr);
    addToRSDKFunctionTable(FunctionTable_ResetEntitySlot, resetEntitySlot);
    addToRSDKFunctionTable(FunctionTable_CreateEntity, createEntity);
    addToRSDKFunctionTable(FunctionTable_CopyEntity, copyEntity);
    addToRSDKFunctionTable(FunctionTable_CheckOnScreen, checkOnScreen);
    addToRSDKFunctionTable(FunctionTable_CheckPosOnScreen, checkPosOnScreen);
    addToRSDKFunctionTable(FunctionTable_AddDrawListRef, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SwapDrawLayers, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetDrawLayerProperties, nullFunc);
    addToRSDKFunctionTable(FunctionTable_LoadScene, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetGameMode, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetHardResetFlag, nullFunc);
    addToRSDKFunctionTable(FunctionTable_CheckValidScene, nullFunc);
    addToRSDKFunctionTable(FunctionTable_CheckSceneFolder, checkStageFolder);
    addToRSDKFunctionTable(FunctionTable_InitSceneLoad, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetObjectByName, nullFunc);
    addToRSDKFunctionTable(FunctionTable_ClearScreens, nullFunc);
    addToRSDKFunctionTable(FunctionTable_AddScreen, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetSettingsValue, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetSettingsValue, nullFunc);
    addToRSDKFunctionTable(FunctionTable_UpdateWindow, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Sin1024, sin1024);
    addToRSDKFunctionTable(FunctionTable_Cos1024, cos1024);
    addToRSDKFunctionTable(FunctionTable_ATan1024, tan1024);
    addToRSDKFunctionTable(FunctionTable_ASin1024, aSin1024);
    addToRSDKFunctionTable(FunctionTable_ACos1024, aCos1024);
    addToRSDKFunctionTable(FunctionTable_Sin512, sin512);
    addToRSDKFunctionTable(FunctionTable_Cos512, cos512);
    addToRSDKFunctionTable(FunctionTable_ATan512, tan512);
    addToRSDKFunctionTable(FunctionTable_ASin512, aSin512);
    addToRSDKFunctionTable(FunctionTable_ACos512, aCos512);
    addToRSDKFunctionTable(FunctionTable_Sin256, sin256);
    addToRSDKFunctionTable(FunctionTable_Cos256, cos256);
    addToRSDKFunctionTable(FunctionTable_ATan256, tan256);
    addToRSDKFunctionTable(FunctionTable_ASin256, aSin256);
    addToRSDKFunctionTable(FunctionTable_ACos256, aCos256);
    addToRSDKFunctionTable(FunctionTable_Rand, FunctionTable::random);
    addToRSDKFunctionTable(FunctionTable_Random, random2);
    addToRSDKFunctionTable(FunctionTable_SetRandKey, setRandKey);
    addToRSDKFunctionTable(FunctionTable_ATan2, arcTanLookup);
    addToRSDKFunctionTable(FunctionTable_SetIdentityMatrix, setIdentityMatrix);
    addToRSDKFunctionTable(FunctionTable_MatrixMultiply, matrixMultiply);
    addToRSDKFunctionTable(FunctionTable_MatrixTranslateXYZ, matrixTranslateXYZ);
    addToRSDKFunctionTable(FunctionTable_MatrixScaleXYZ, matrixScaleXYZ);
    addToRSDKFunctionTable(FunctionTable_MatrixRotateX, matrixRotateX);
    addToRSDKFunctionTable(FunctionTable_MatrixRotateY, matrixRotateY);
    addToRSDKFunctionTable(FunctionTable_MatrixRotateZ, matrixRotateZ);
    addToRSDKFunctionTable(FunctionTable_MatrixRotateXYZ, matrixRotateXYZ);
    addToRSDKFunctionTable(FunctionTable_MatrixInverse, matrixInverse);
    addToRSDKFunctionTable(FunctionTable_MatrixCopy, matrixCopy);
    addToRSDKFunctionTable(FunctionTable_SetText, setText);
    addToRSDKFunctionTable(FunctionTable_CopyString, copyString);
    addToRSDKFunctionTable(FunctionTable_PrependText, prependText);
    addToRSDKFunctionTable(FunctionTable_AppendString, appendString);
    addToRSDKFunctionTable(FunctionTable_AppendText, appendText);
    addToRSDKFunctionTable(FunctionTable_LoadStrings, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SplitStringList, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetCString, getCString);
    addToRSDKFunctionTable(FunctionTable_StringCompare, stringCompare);
    addToRSDKFunctionTable(FunctionTable_GetDisplayInfo, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetWindowSize, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetScreenSize, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetClipBounds, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetScreenSplitVerticies, nullFunc);
    addToRSDKFunctionTable(FunctionTable_LoadSpriteSheet, loadSpriteSheet);
    addToRSDKFunctionTable(FunctionTable_SetLookupTable, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetPaletteMask, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetPaletteEntry, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetPaletteEntry, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetActivePalette, nullFunc);
    addToRSDKFunctionTable(FunctionTable_CopyPalette, nullFunc);
    addToRSDKFunctionTable(FunctionTable_LoadPalette, nullFunc);
    addToRSDKFunctionTable(FunctionTable_RotatePalette, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetLimitedFade, nullFunc);
    addToRSDKFunctionTable(FunctionTable_BlendColours, nullFunc);
    addToRSDKFunctionTable(FunctionTable_DrawRect, drawRect);
    addToRSDKFunctionTable(FunctionTable_DrawLine, drawLine);
    addToRSDKFunctionTable(FunctionTable_DrawCircle, drawCircle);
    addToRSDKFunctionTable(FunctionTable_DrawCircleOutline, drawCircleOutline);
    addToRSDKFunctionTable(FunctionTable_DrawFace, drawFace);
    addToRSDKFunctionTable(FunctionTable_DrawBlendedFace, drawBlendedFace);
    addToRSDKFunctionTable(FunctionTable_DrawSprite, drawSprite);
    addToRSDKFunctionTable(FunctionTable_DrawDeformed, nullFunc);
    addToRSDKFunctionTable(FunctionTable_DrawText, drawText);
    addToRSDKFunctionTable(FunctionTable_DrawTile, drawTile);
    addToRSDKFunctionTable(FunctionTable_CopyTile, nullFunc);
    addToRSDKFunctionTable(FunctionTable_DrawAniTile, nullFunc);
    addToRSDKFunctionTable(FunctionTable_FillScreen, nullFunc);
    addToRSDKFunctionTable(FunctionTable_LoadMesh, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Create3DScene, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Prepare3DScene, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetDiffuseColour, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetDiffuseIntensity, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetSpecularIntensity, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetupMesh, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetModelAnimation, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetupMeshAnimation, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Draw3DScene, nullFunc);
    addToRSDKFunctionTable(FunctionTable_LoadAnimation, loadSpriteAnimation);
    addToRSDKFunctionTable(FunctionTable_CreateAnimation, createSpriteAnimation);
    addToRSDKFunctionTable(FunctionTable_SetSpriteAnimation, setSpriteAnimation);
    addToRSDKFunctionTable(FunctionTable_EditAnimation, editSpriteAnimation);
    addToRSDKFunctionTable(FunctionTable_SetSpriteString, setSpriteString);
    addToRSDKFunctionTable(FunctionTable_GetAnimation, getSpriteAnimation);
    addToRSDKFunctionTable(FunctionTable_GetFrame, getFrame);
    addToRSDKFunctionTable(FunctionTable_GetHitbox, getHitbox);
    addToRSDKFunctionTable(FunctionTable_GetFrameID, getFrameID);
    addToRSDKFunctionTable(FunctionTable_GetStringWidth, getStringWidth);
    addToRSDKFunctionTable(FunctionTable_ProcessAnimation, processAnimation);
    addToRSDKFunctionTable(FunctionTable_GetSceneLayer, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetSceneLayerID, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetLayerSize, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetTileInfo, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetTileInfo, nullFunc);
    addToRSDKFunctionTable(FunctionTable_CopyTileLayer, nullFunc);
    addToRSDKFunctionTable(FunctionTable_ProcessParallax, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetScanlines, nullFunc);
    addToRSDKFunctionTable(FunctionTable_CheckObjectCollisionTouch, nullFunc);
    addToRSDKFunctionTable(FunctionTable_CheckObjectCollisionPoint, nullFunc);
    addToRSDKFunctionTable(FunctionTable_CheckObjectCollisionBox, nullFunc);
    addToRSDKFunctionTable(FunctionTable_CheckObjectCollisionPlatform, nullFunc);
    addToRSDKFunctionTable(FunctionTable_ObjectTileCollision, nullFunc);
    addToRSDKFunctionTable(FunctionTable_ObjectTileGrip, nullFunc);
    addToRSDKFunctionTable(FunctionTable_ProcessTileCollisions, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetTileAngle, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetTileAngle, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetTileBehaviour, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetTileBehaviour, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetSFX, nullFunc);
    addToRSDKFunctionTable(FunctionTable_PlaySFX, nullFunc);
    addToRSDKFunctionTable(FunctionTable_StopSFX, nullFunc);
    addToRSDKFunctionTable(FunctionTable_PlayMusic, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetChannelAttributes, nullFunc);
    addToRSDKFunctionTable(FunctionTable_StopChannel, nullFunc);
    addToRSDKFunctionTable(FunctionTable_PauseChannel, nullFunc);
    addToRSDKFunctionTable(FunctionTable_ResumeChannel, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SfxPlaying, nullFunc);
    addToRSDKFunctionTable(FunctionTable_ChannelActive, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetChannelPos, nullFunc);
    addToRSDKFunctionTable(FunctionTable_LoadVideo, nullFunc);
    addToRSDKFunctionTable(FunctionTable_LoadImage, nullFunc);
    addToRSDKFunctionTable(FunctionTable_ControllerIDForInputID, nullFunc);
    addToRSDKFunctionTable(FunctionTable_MostRecentActiveControllerID, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Unknown100, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetAssignedControllerID, nullFunc);
    addToRSDKFunctionTable(FunctionTable_GetAssignedUnknown, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Missing22, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Missing23, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Missing24, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Missing25, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Missing26, nullFunc);
    addToRSDKFunctionTable(FunctionTable_AssignControllerID, nullFunc);
    addToRSDKFunctionTable(FunctionTable_InputIDIsDisconnected, nullFunc);
    addToRSDKFunctionTable(FunctionTable_ResetControllerAssignments, nullFunc);
    addToRSDKFunctionTable(FunctionTable_LoadUserFile, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SaveUserFile, nullFunc);
    addToRSDKFunctionTable(FunctionTable_printLog, nullFunc);
    addToRSDKFunctionTable(FunctionTable_printString, nullFunc);
    addToRSDKFunctionTable(FunctionTable_printText, nullFunc);
    addToRSDKFunctionTable(FunctionTable_printIntegerUnsigned, nullFunc);
    addToRSDKFunctionTable(FunctionTable_printInteger, nullFunc);
    addToRSDKFunctionTable(FunctionTable_printFloat, nullFunc);
    addToRSDKFunctionTable(FunctionTable_printVector2, nullFunc);
    addToRSDKFunctionTable(FunctionTable_printHitbox, nullFunc);
    addToRSDKFunctionTable(FunctionTable_Unknown105, setActiveVariable);
    addToRSDKFunctionTable(FunctionTable_Unknown106, addEnumVar);
    addToRSDKFunctionTable(FunctionTable_ClearDebugValues, nullFunc);
    addToRSDKFunctionTable(FunctionTable_SetDebugValue, nullFunc);
}

void GameLink::LinkGameObjects(QString gameName)
{
    gameObjectList.clear();
    sceneInfo.classCount     = 0;
    sceneInfo.activeCategory = 0;
    sceneInfo.listPos        = 0;
    sceneInfo.state          = 1;
    sceneInfo.inEditor       = true;
    sceneInfo.debugMode      = true;

    skuInfo.platform = 0xFF;

    GameInfo info;

    info.functionPtrs = &RSDKFunctionTable;
    info.APIPtrs      = NULL;
    info.currentSKU   = &skuInfo;
    info.engineInfo   = &engineInfo;
    info.sceneInfo    = &sceneInfo;
    info.controller   = controller;
    info.stickL       = stickL;
    info.stickR       = stickR;
    info.triggerL     = triggerL;
    info.triggerR     = triggerR;
    info.touchMouse   = &touchMouse;
    info.unknown      = &unknownInfo;
    info.screenInfo   = screens;
    info.modPtrs      = NULL;

    QLibrary logicLib;

    logicLib.setFileName(gameName);
    // logicLib.setFileName("E:/Github/Sonic-Mania-Decompilation/build/Win32/Debug/Game");
    logicLib.load();
    if (!logicLib.isLoaded())
        qDebug() << logicLib.errorString();

    void (*linkGameLogic)(GameInfo *) = NULL;

    // LinkGameLogic(&info)
    if (logicLib.isLoaded()) {
        linkGameLogic = (void (*)(GameInfo *))logicLib.resolve("LinkGameLogicDLL");
    }

    if (linkGameLogic) {
        // engine adds this obj no matter what game dll is
        FunctionTable::registerObject(&blankObject, "Blank Object", sizeof(GameEntity),
                                      sizeof(GameObject), NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                      NULL, NULL);

        linkGameLogic(&info);
        printf("sucessfully linked game logic!\n");
        printf("linked %d objects!\n", gameObjectList.count());
    }
    else {
        printf("failed to link game logic...\n");
    }

    for (int i = 0; i < gameObjectList.count(); ++i) {
        if (gameObjectList[i].type)
            *gameObjectList[i].type = NULL;
    }
}

GameObjectInfo *GameLink::GetObjectInfo(QString name)
{
    QByteArray hashData = Utils::getMd5HashByteArray(name);
    byte data[0x10];
    for (int i = 0; i < 0x10; ++i) data[i] = hashData[i];

    uint hash[4];
    memcpy(hash, data, 0x10 * sizeof(byte));

    for (int i = 0; i < gameObjectList.count(); ++i) {
        if (memcmp(hash, gameObjectList[i].hash, 0x10 * sizeof(byte)) == 0) {
            return &gameObjectList[i];
        }
    }
    return NULL;
}

GameObjectInfo *GameLink::GetObjectInfo(int type)
{
    for (int i = 0; i < gameObjectList.count(); ++i) {
        if (gameObjectList[i].type && *gameObjectList[i].type) {
            if ((*gameObjectList[i].type)->objectID == type)
                return &gameObjectList[i];
        }
    }
    return NULL;
}
