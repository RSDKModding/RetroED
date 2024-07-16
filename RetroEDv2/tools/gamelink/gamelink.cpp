#include "includes.hpp"

#include "tools/sceneeditorv5.hpp"

void BlankFunction() {}

#define ADD_RSDK_FUNCTION(id, func) RSDKFunctionTable[id] = (void *)func;
#define ADD_API_FUNCTION(id, func)  APIFunctionTable[id] = (void *)func;
#define ADD_MOD_FUNCTION(id, func)  modFunctionTable[id] = (void *)func;

byte *gameGlobalVariablesPtr                = NULL;
void (*gameGlobalVarsInitCB)(void *globals) = NULL;

#include "gamefunctionTable_rev01.cpp"
#include "gamefunctionTable_rev02.cpp"
#include "gamefunctionTable_rev0U.cpp"

namespace FunctionTable
{

enum ModFunctionTableIDs {
    ModTable_RegisterGlobals,
    ModTable_RegisterObject,
    ModTable_RegisterObjectSTD,
    ModTable_RegisterObjectHook,
    ModTable_FindObject,
    ModTable_GetGlobals,
    ModTable_Super,
    ModTable_LoadModInfo,
    ModTable_GetModPath,
    ModTable_GetModCount,
    ModTable_GetModIDByIndex,
    ModTable_ForeachModID,
    ModTable_AddModCallback,
    ModTable_AddModCallbackSTD,
    ModTable_AddPublicFunction,
    ModTable_GetPublicFunction,
    ModTable_GetSettingsBool,
    ModTable_GetSettingsInt,
    ModTable_GetSettingsFloat,
    ModTable_GetSettingsString,
    ModTable_SetSettingsBool,
    ModTable_SetSettingsInt,
    ModTable_SetSettingsFloat,
    ModTable_SetSettingsString,
    ModTable_SaveSettings,
    ModTable_GetConfigBool,
    ModTable_GetConfigInt,
    ModTable_GetConfigFloat,
    ModTable_GetConfigString,
    ModTable_ForeachConfig,
    ModTable_ForeachConfigCategory,
    ModTable_RegisterAchievement,
    ModTable_GetAchievementInfo,
    ModTable_GetAchievementIndexByID,
    ModTable_GetAchievementCount,
    ModTable_LoadShader,
    ModTable_StateMachineRun,
    ModTable_RegisterStateHook,
    ModTable_HandleRunState_HighPriority,
    ModTable_HandleRunState_LowPriority,

    // Mod API REV02

    // Mod Settings (Part 2)
    ModTable_ForeachSetting,
    ModTable_ForeachSettingCategory,

    // Files
    ModTable_ExcludeFile,
    ModTable_ExcludeAllFiles,
    ModTable_ReloadFile,
    ModTable_ReloadAllFiles,

    // Graphics
    ModTable_GetSpriteAnimation,
    ModTable_GetSpriteSurface,
    ModTable_GetPaletteBank,
    ModTable_GetActivePaletteBuffer,
    ModTable_GetRGB32To16Buffer,
    ModTable_GetBlendLookupTable,
    ModTable_GetSubtractLookupTable,
    ModTable_GetTintLookupTable,
    ModTable_GetMaskColor,
    ModTable_GetScanEdgeBuffer,
    ModTable_GetCamera,
    ModTable_GetShader,
    ModTable_GetModel,
    ModTable_GetScene3D,
    ModTable_DrawDynamicAniTile,

    // Audio
    ModTable_GetSfx,
    ModTable_GetChannel,

    // Objects/Entities
    ModTable_GetGroupEntities,

    // Collision
    ModTable_SetPathGripSensors,
    ModTable_FloorCollision,
    ModTable_LWallCollision,
    ModTable_RoofCollision,
    ModTable_RWallCollision,
    ModTable_FindFloorPosition,
    ModTable_FindLWallPosition,
    ModTable_FindRoofPosition,
    ModTable_FindRWallPosition,
    ModTable_CopyCollisionMask,
    ModTable_GetCollisionInfo,

    ModTable_Count,
};

void *modFunctionTable[ModTable_Count];

void InitModFunctionTable()
{
    // ============================
    // Mod Function Table
    // ============================

    // Registration & Core
    ADD_MOD_FUNCTION(ModTable_RegisterGlobals, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_RegisterObject, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_RegisterObjectSTD, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_RegisterObjectHook, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_FindObject, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetGlobals, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_Super, BlankFunction);

    // Mod Info
    ADD_MOD_FUNCTION(ModTable_LoadModInfo, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetModPath, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetModCount, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetModIDByIndex, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_ForeachModID, BlankFunction);

    // Mod Callbacks & Public Functions
    ADD_MOD_FUNCTION(ModTable_AddModCallback, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_AddModCallbackSTD, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_AddPublicFunction, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetPublicFunction, BlankFunction);

    // Mod Settings
    ADD_MOD_FUNCTION(ModTable_GetSettingsBool, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetSettingsInt, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetSettingsFloat, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetSettingsString, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_SetSettingsBool, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_SetSettingsInt, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_SetSettingsFloat, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_SetSettingsString, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_SaveSettings, BlankFunction);

    // Config
    ADD_MOD_FUNCTION(ModTable_GetConfigBool, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetConfigInt, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetConfigFloat, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetConfigString, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_ForeachConfig, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_ForeachConfigCategory, BlankFunction);

    // Achievements
    ADD_MOD_FUNCTION(ModTable_RegisterAchievement, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetAchievementInfo, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetAchievementIndexByID, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetAchievementCount, BlankFunction);

    // Shaders
    ADD_MOD_FUNCTION(ModTable_LoadShader, BlankFunction);

    // StateMachine
    ADD_MOD_FUNCTION(ModTable_StateMachineRun, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_RegisterStateHook, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_HandleRunState_HighPriority, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_HandleRunState_LowPriority, BlankFunction);

    // MOD API REV02

    // Mod Settings (Part 2)
    ADD_MOD_FUNCTION(ModTable_ForeachSetting, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_ForeachSettingCategory, BlankFunction);

    // Files
    ADD_MOD_FUNCTION(ModTable_ExcludeFile, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_ExcludeAllFiles, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_ReloadFile, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_ReloadAllFiles, BlankFunction);

    // Graphics
    ADD_MOD_FUNCTION(ModTable_GetSpriteAnimation, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetSpriteSurface, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetPaletteBank, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetActivePaletteBuffer, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetRGB32To16Buffer, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetBlendLookupTable, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetSubtractLookupTable, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetTintLookupTable, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetMaskColor, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetScanEdgeBuffer, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetCamera, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetShader, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetModel, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetScene3D, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_DrawDynamicAniTile, BlankFunction);

    // Audio
    ADD_MOD_FUNCTION(ModTable_GetSfx, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetChannel, BlankFunction);

    // Objects/Entities
    ADD_MOD_FUNCTION(ModTable_GetGroupEntities, BlankFunction);

    // Collision
    ADD_MOD_FUNCTION(ModTable_SetPathGripSensors, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_FloorCollision, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_LWallCollision, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_RoofCollision, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_RWallCollision, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_FindFloorPosition, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_FindLWallPosition, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_FindRoofPosition, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_FindRWallPosition, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_CopyCollisionMask, BlankFunction);
    ADD_MOD_FUNCTION(ModTable_GetCollisionInfo, BlankFunction);
}

} // namespace FunctionTable

void GameLink::Setup()
{
    using namespace FunctionTable;
    CalculateTrigAngles();

    Rev01::InitFunctionTables();
    Rev02::InitFunctionTables();
    Rev0U::InitFunctionTables();

    InitModFunctionTable();
}

void GameLink::LinkGameObjects(QString gameName)
{
    unload();

    if (!logicLib)
        logicLib = new QLibrary;

    logicLib->setFileName(gameName);
    logicLib->load();
    if (!logicLib->isLoaded()){
        PrintLog("Failed to link: " + logicLib->errorString());
        if (logicLib->errorString().contains("0x000000c1"))
            error = 1;
    }
    // 0x000000c1 = wrong architecture
    void (*linkGameLogic)(void *) = NULL;

    if (logicLib->isLoaded()) {
        linkGameLogic = (void (*)(void *))logicLib->resolve("LinkGameLogicDLL");

        int *RSDKRevision = (int *)logicLib->resolve("RSDKRevision");
        if (RSDKRevision){
            if (*RSDKRevision == -1){
                linkGameLogic = NULL;
                error = 2;
                PrintLog("Failed to link: Not a valid Game library");
            } else {
                revision = *RSDKRevision;
            }
        }
    }

    if (linkGameLogic) {
        // engine adds this obj no matter what game dll is
        FunctionTable::RegisterObjectv5U(&blankObject, "Blank Object", sizeof(GameEntityvU),
                                         sizeof(GameObject), NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                         NULL, NULL, NULL);

        switch (revision) {
            case 1: {
                void (*linkGameLogicV1)(FunctionTable::Rev01::GameInfo info) =
                    (void (*)(FunctionTable::Rev01::GameInfo))linkGameLogic;

                FunctionTable::Rev01::GameInfo info;
                info.rsdkFunctionTable = &FunctionTable::Rev01::RSDKFunctionTable;
                info.engineInfo        = &v5Editor->viewer->gameInfoV1;
                info.sceneInfo         = &v5Editor->viewer->sceneInfoV1;
                info.controller        = v5Editor->viewer->controllerV1;
                info.stickL            = v5Editor->viewer->stickLV1;
                info.touchMouse        = &v5Editor->viewer->touchMouseV1;
                info.screenInfo        = v5Editor->viewer->screens;
                info.modFunctionTable  = &FunctionTable::modFunctionTable;

                linkGameLogicV1(info);
                break;
            }

            case 2: {
                FunctionTable::Rev02::GameInfo info;
                info.rsdkFunctionTable = &FunctionTable::Rev02::RSDKFunctionTable;
                info.apiFunctionTable  = &FunctionTable::Rev02::APIFunctionTable;
                info.currentSKU        = &v5Editor->viewer->skuInfo;
                info.engineInfo        = &v5Editor->viewer->gameInfo;
                info.sceneInfo         = &v5Editor->viewer->sceneInfo;
                info.controller        = v5Editor->viewer->controller;
                info.stickL            = v5Editor->viewer->stickL;
                info.stickR            = v5Editor->viewer->stickR;
                info.triggerL          = v5Editor->viewer->triggerL;
                info.triggerR          = v5Editor->viewer->triggerR;
                info.touchMouse        = &v5Editor->viewer->touchMouse;
                info.unknown           = &v5Editor->viewer->unknownInfo;
                info.screenInfo        = v5Editor->viewer->screens;
                info.modFunctionTable  = &FunctionTable::modFunctionTable;

                linkGameLogic(&info);
                break;
            }

            default:
            case 3: {
                FunctionTable::Rev0U::GameInfo info;
                info.rsdkFunctionTable = &FunctionTable::Rev0U::RSDKFunctionTable;
                info.apiFunctionTable  = &FunctionTable::Rev0U::APIFunctionTable;
                info.currentSKU        = &v5Editor->viewer->skuInfo;
                info.engineInfo        = &v5Editor->viewer->gameInfo;
                info.sceneInfo         = &v5Editor->viewer->sceneInfo;
                info.controller        = v5Editor->viewer->controller;
                info.stickL            = v5Editor->viewer->stickL;
                info.stickR            = v5Editor->viewer->stickR;
                info.triggerL          = v5Editor->viewer->triggerL;
                info.triggerR          = v5Editor->viewer->triggerR;
                info.touchMouse        = &v5Editor->viewer->touchMouse;
                info.unknown           = &v5Editor->viewer->unknownInfo;
                info.screenInfo        = v5Editor->viewer->screens;
                info.hedgehogLink      = NULL;
                info.modFunctionTable  = &FunctionTable::modFunctionTable;

                linkGameLogic(&info);
                break;
            }
        }

        globalVariablesPtr = gameGlobalVariablesPtr;
        globalVarsInitCB   = gameGlobalVarsInitCB;

        if (globalVarsInitCB)
            globalVarsInitCB(globalVariablesPtr);

        PrintLog("sucessfully linked game logic!");
        PrintLog(QString("linked %1 objects!").arg(gameObjectList.count()));
    }
    else {
        PrintLog("failed to link game logic...");
    }

    for (int i = 0; i < gameObjectList.count(); ++i) {
        if (gameObjectList[i].sVars)
            *gameObjectList[i].sVars = NULL;
    }
}

void GameLink::unload()
{
    if (logicLib) {
        if (logicLib->isLoaded()) {
            logicLib->unload();
        }
        logicLib = nullptr;
    }

    gameObjectList.clear();
}
