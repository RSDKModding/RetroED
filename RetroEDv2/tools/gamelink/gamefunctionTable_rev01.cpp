
namespace FunctionTable
{

namespace Rev01
{


struct GameInfo {
    void *rsdkFunctionTable;

    EngineInfoV1 *engineInfo;
    SceneInfoV1 *sceneInfo;

    ControllerStateV1 *controller;
    AnalogStateV1 *stickL;

    TouchMouseDataV1 *touchMouse;

    ScreenInfo *screenInfo;

    void *modFunctionTable;
};

enum FunctionTableIDs {
    FunctionTable_RegisterGlobalVariables,
    FunctionTable_RegisterObject,
    FunctionTable_GetActiveEntities,
    FunctionTable_GetAllEntities,
    FunctionTable_BreakForeachLoop,
    FunctionTable_SetEditableVar,
    FunctionTable_GetEntity,
    FunctionTable_GetEntitySlot,
    FunctionTable_GetEntityCount,
    FunctionTable_GetDrawListRefSlot,
    FunctionTable_GetDrawListRef,
    FunctionTable_ResetEntity,
    FunctionTable_ResetEntitySlot,
    FunctionTable_CreateEntity,
    FunctionTable_CopyEntity,
    FunctionTable_CheckOnScreen,
    FunctionTable_CheckPosOnScreen,
    FunctionTable_AddDrawListRef,
    FunctionTable_SwapDrawListEntries,
    FunctionTable_SetDrawGroupProperties,
    FunctionTable_SetScene,
    FunctionTable_SetEngineState,
    FunctionTable_CheckValidScene,
    FunctionTable_CheckSceneFolder,
    FunctionTable_LoadScene,
    FunctionTable_FindObject,
    FunctionTable_ClearCameras,
    FunctionTable_AddCamera,
    FunctionTable_GetAPIFunction,
    FunctionTable_GetVideoSetting,
    FunctionTable_SetVideoSetting,
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
    FunctionTable_RandSeeded,
    FunctionTable_SetRandSeed,
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
    FunctionTable_InitString,
    FunctionTable_CopyString,
    FunctionTable_SetString,
    FunctionTable_AppendString,
    FunctionTable_AppendText,
    FunctionTable_LoadStringList,
    FunctionTable_SplitStringList,
    FunctionTable_GetCString,
    FunctionTable_CompareStrings,
    FunctionTable_GetDisplayInfo,
    FunctionTable_GetWindowSize,
    FunctionTable_SetScreenSize,
    FunctionTable_SetClipBounds,
    FunctionTable_LoadSpriteSheet,
    FunctionTable_SetTintLookupTable,
    FunctionTable_SetPaletteMask,
    FunctionTable_SetPaletteEntry,
    FunctionTable_GetPaletteEntry,
    FunctionTable_SetActivePalette,
    FunctionTable_CopyPalette,
    FunctionTable_RotatePalette,
    FunctionTable_SetLimitedFade,
    FunctionTable_DrawRect,
    FunctionTable_DrawLine,
    FunctionTable_DrawCircle,
    FunctionTable_DrawCircleOutline,
    FunctionTable_DrawFace,
    FunctionTable_DrawBlendedFace,
    FunctionTable_DrawSprite,
    FunctionTable_DrawDeformedSprite,
    FunctionTable_DrawString,
    FunctionTable_DrawTile,
    FunctionTable_CopyTile,
    FunctionTable_DrawAniTile,
    FunctionTable_FillScreen,
    FunctionTable_LoadMesh,
    FunctionTable_Create3DScene,
    FunctionTable_Prepare3DScene,
    FunctionTable_SetDiffuseColor,
    FunctionTable_SetDiffuseIntensity,
    FunctionTable_SetSpecularIntensity,
    FunctionTable_AddModelToScene,
    FunctionTable_SetModelAnimation,
    FunctionTable_AddMeshFrameToScene,
    FunctionTable_Draw3DScene,
    FunctionTable_LoadSpriteAnimation,
    FunctionTable_CreateSpriteAnimation,
    FunctionTable_SetSpriteAnimation,
    FunctionTable_EditSpriteAnimation,
    FunctionTable_SetSpriteString,
    FunctionTable_FindSpriteAnimation,
    FunctionTable_GetFrame,
    FunctionTable_GetHitbox,
    FunctionTable_GetFrameID,
    FunctionTable_GetStringWidth,
    FunctionTable_ProcessAnimation,
    FunctionTable_GetTileLayerID,
    FunctionTable_GetTileLayer,
    FunctionTable_GetLayerSize,
    FunctionTable_GetTile,
    FunctionTable_SetTile,
    FunctionTable_CopyTileLayer,
    FunctionTable_ProcessParallax,
    FunctionTable_GetScanlines,
    FunctionTable_CheckObjectCollisionTouch,
    FunctionTable_CheckObjectCollisionCircle,
    FunctionTable_CheckObjectCollisionBox,
    FunctionTable_CheckObjectCollisionPlatform,
    FunctionTable_ObjectTileCollision,
    FunctionTable_ObjectTileGrip,
    FunctionTable_ProcessObjectMovement,
    FunctionTable_GetTileAngle,
    FunctionTable_SetTileAngle,
    FunctionTable_GetTileFlags,
    FunctionTable_SetTileFlags,
    FunctionTable_GetSfx,
    FunctionTable_PlaySfx,
    FunctionTable_StopSfx,
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
    FunctionTable_GetUnknownInputValue,
    FunctionTable_LoadUserFile,
    FunctionTable_SaveUserFile,
    FunctionTable_SetActiveVariable,
    FunctionTable_AddEnumVariable,
    FunctionTable_PrintMessage,
    FunctionTable_Count,
};

void *RSDKFunctionTable[FunctionTable_Count];

void RegisterGlobalVariables(void **globals, int size)
{
    gameGlobalVariablesPtr = NULL;
    gameGlobalVarsInitCB = NULL;
    if (!v5Editor)
        return;
    allocateStorage(v5Editor->dataStorage, size, globals, DATASET_STG, true);
    gameGlobalVariablesPtr = (byte *)*globals;
}

void InitFunctionTables()
{

    // ============================
    // RSDK Function Table
    // ============================

    // Registration
    ADD_RSDK_FUNCTION(FunctionTable_RegisterGlobalVariables, RegisterGlobalVariables);
    ADD_RSDK_FUNCTION(FunctionTable_RegisterObject, RegisterObjectv5);

    // Entities & Objects
    ADD_RSDK_FUNCTION(FunctionTable_GetActiveEntities, GetActiveEntities);
    ADD_RSDK_FUNCTION(FunctionTable_GetAllEntities, GetAllEntities);
    ADD_RSDK_FUNCTION(FunctionTable_BreakForeachLoop, BreakForeachLoop);
    ADD_RSDK_FUNCTION(FunctionTable_SetEditableVar, SetEditableVar);
    ADD_RSDK_FUNCTION(FunctionTable_GetEntity, GetEntity);
    ADD_RSDK_FUNCTION(FunctionTable_GetEntitySlot, GetEntitySlot);
    ADD_RSDK_FUNCTION(FunctionTable_GetEntityCount, GetEntityCount);
    ADD_RSDK_FUNCTION(FunctionTable_GetDrawListRefSlot, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetDrawListRef, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_ResetEntity, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_ResetEntitySlot, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CreateEntity, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CopyEntity, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CheckOnScreen, CheckOnScreen);
    ADD_RSDK_FUNCTION(FunctionTable_CheckPosOnScreen, CheckPosOnScreen);
    ADD_RSDK_FUNCTION(FunctionTable_AddDrawListRef, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SwapDrawListEntries, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetDrawGroupProperties, BlankFunction);

    // Scene Management
    ADD_RSDK_FUNCTION(FunctionTable_SetScene, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetEngineState, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CheckValidScene, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CheckSceneFolder, CheckSceneFolder);
    ADD_RSDK_FUNCTION(FunctionTable_LoadScene, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_FindObject, FindObject);

    // Cameras
    ADD_RSDK_FUNCTION(FunctionTable_ClearCameras, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_AddCamera, BlankFunction);

    // API (Rev01 only)
    ADD_RSDK_FUNCTION(FunctionTable_GetAPIFunction, BlankFunction);

    // Window/Video Settings
    ADD_RSDK_FUNCTION(FunctionTable_GetVideoSetting, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetVideoSetting, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_UpdateWindow, BlankFunction)

    // Math
    ADD_RSDK_FUNCTION(FunctionTable_Sin1024, Sin1024);
    ADD_RSDK_FUNCTION(FunctionTable_Cos1024, Cos1024);
    ADD_RSDK_FUNCTION(FunctionTable_ATan1024, Tan1024);
    ADD_RSDK_FUNCTION(FunctionTable_ASin1024, ASin1024);
    ADD_RSDK_FUNCTION(FunctionTable_ACos1024, ACos1024);
    ADD_RSDK_FUNCTION(FunctionTable_Sin512, Sin512);
    ADD_RSDK_FUNCTION(FunctionTable_Cos512, Cos512);
    ADD_RSDK_FUNCTION(FunctionTable_ATan512, Tan512);
    ADD_RSDK_FUNCTION(FunctionTable_ASin512, ASin512);
    ADD_RSDK_FUNCTION(FunctionTable_ACos512, ACos512);
    ADD_RSDK_FUNCTION(FunctionTable_Sin256, Sin256);
    ADD_RSDK_FUNCTION(FunctionTable_Cos256, Cos256);
    ADD_RSDK_FUNCTION(FunctionTable_ATan256, Tan256);
    ADD_RSDK_FUNCTION(FunctionTable_ASin256, ASin256);
    ADD_RSDK_FUNCTION(FunctionTable_ACos256, ACos256);
    ADD_RSDK_FUNCTION(FunctionTable_Rand, Rand);
    ADD_RSDK_FUNCTION(FunctionTable_RandSeeded, RandSeeded);
    ADD_RSDK_FUNCTION(FunctionTable_SetRandSeed, SetRandSeed);
    ADD_RSDK_FUNCTION(FunctionTable_ATan2, ArcTanLookup);

    // Matrices
    ADD_RSDK_FUNCTION(FunctionTable_SetIdentityMatrix, SetIdentityMatrix);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixMultiply, MatrixMultiply);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixTranslateXYZ, MatrixTranslateXYZ);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixScaleXYZ, MatrixScaleXYZ);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixRotateX, MatrixRotateX);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixRotateY, MatrixRotateY);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixRotateZ, MatrixRotateZ);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixRotateXYZ, MatrixRotateXYZ);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixInverse, MatrixInverse);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixCopy, MatrixCopy);

    // Strings
    ADD_RSDK_FUNCTION(FunctionTable_InitString, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CopyString, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetString, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_AppendString, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_AppendText, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_LoadStringList, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SplitStringList, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetCString, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CompareStrings, BlankFunction);

    // Screens & Displays
    ADD_RSDK_FUNCTION(FunctionTable_GetDisplayInfo, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetWindowSize, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetScreenSize, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetClipBounds, BlankFunction);

    // Spritesheets
    ADD_RSDK_FUNCTION(FunctionTable_LoadSpriteSheet, LoadSpriteSheet);

    // Palettes & Colors
    ADD_RSDK_FUNCTION(FunctionTable_SetTintLookupTable, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetPaletteMask, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetPaletteEntry, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetPaletteEntry, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetActivePalette, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CopyPalette, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_RotatePalette, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetLimitedFade, BlankFunction);

    // Drawing
    ADD_RSDK_FUNCTION(FunctionTable_DrawRect, DrawRect);
    ADD_RSDK_FUNCTION(FunctionTable_DrawLine, DrawLine);
    ADD_RSDK_FUNCTION(FunctionTable_DrawCircle, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_DrawCircleOutline, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_DrawFace, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_DrawBlendedFace, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_DrawSprite, DrawSprite);
    ADD_RSDK_FUNCTION(FunctionTable_DrawDeformedSprite, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_DrawString, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_DrawTile, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CopyTile, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_DrawAniTile, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_FillScreen, BlankFunction);

    // Meshes & 3D Scenes
    ADD_RSDK_FUNCTION(FunctionTable_LoadMesh, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_Create3DScene, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_Prepare3DScene, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetDiffuseColor, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetDiffuseIntensity, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetSpecularIntensity, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_AddModelToScene, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetModelAnimation, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_AddMeshFrameToScene, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_Draw3DScene, BlankFunction);

    // Sprite Animations & Frames
    ADD_RSDK_FUNCTION(FunctionTable_LoadSpriteAnimation, LoadSpriteAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_CreateSpriteAnimation, CreateSpriteAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_SetSpriteAnimation, SetSpriteAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_EditSpriteAnimation, EditSpriteAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_SetSpriteString, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_FindSpriteAnimation, FindSpriteAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_GetFrame, GetFrame);
    ADD_RSDK_FUNCTION(FunctionTable_GetHitbox, GetHitbox);
    ADD_RSDK_FUNCTION(FunctionTable_GetFrameID, GetFrameID);
    ADD_RSDK_FUNCTION(FunctionTable_GetStringWidth, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_ProcessAnimation, ProcessAnimation);

    // Tile Layers
    ADD_RSDK_FUNCTION(FunctionTable_GetTileLayer, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetTileLayerID, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetLayerSize, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetTile, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetTile, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CopyTileLayer, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_ProcessParallax, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetScanlines, BlankFunction);

    // Object & Tile Collisions
    ADD_RSDK_FUNCTION(FunctionTable_CheckObjectCollisionTouch, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CheckObjectCollisionCircle, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CheckObjectCollisionBox, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_CheckObjectCollisionPlatform, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_ObjectTileCollision, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_ObjectTileGrip, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_ProcessObjectMovement, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetTileAngle, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetTileAngle, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetTileFlags, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetTileFlags, BlankFunction);

    // Audio
    ADD_RSDK_FUNCTION(FunctionTable_GetSfx, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_PlaySfx, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_StopSfx, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_PlayMusic, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SetChannelAttributes, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_StopChannel, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_PauseChannel, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_ResumeChannel, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_SfxPlaying, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_ChannelActive, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_GetChannelPos, BlankFunction);

    // Videos & "HD Images"
    ADD_RSDK_FUNCTION(FunctionTable_LoadVideo, BlankFunction);
    ADD_RSDK_FUNCTION(FunctionTable_LoadImage, BlankFunction);

    // Input
    ADD_RSDK_FUNCTION(FunctionTable_GetUnknownInputValue, BlankFunction);

    // User File Management
    ADD_RSDK_FUNCTION(FunctionTable_LoadUserFile, BlankFunction); // load user file from exe dir
    ADD_RSDK_FUNCTION(FunctionTable_SaveUserFile, BlankFunction); // save use file to exe dir

    // Printing (Rev02)

    // Editor
    ADD_RSDK_FUNCTION(FunctionTable_SetActiveVariable, SetActiveVariable);
    ADD_RSDK_FUNCTION(FunctionTable_AddEnumVariable, AddEnumVariable);

    // Printing (Rev01)
    ADD_RSDK_FUNCTION(FunctionTable_PrintMessage, BlankFunction);
}


}

} // namespace FunctionTable
