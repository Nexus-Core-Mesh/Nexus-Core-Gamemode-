#if !defined RUNTIME_REGISTRY_INC
#define RUNTIME_REGISTRY_INC

#include <a_samp>

#define MAX_RUNTIME_MODULES 64
#define MAX_RUNTIME_HOOKS 32
#define MAX_RUNTIME_LABEL_LEN 48

new gRuntimeModuleCount = 0;
new gRuntimeModules[MAX_RUNTIME_MODULES][MAX_RUNTIME_LABEL_LEN];
new gRuntimeConnectHookCount = 0;
new gRuntimeConnectHooks[MAX_RUNTIME_HOOKS][MAX_RUNTIME_LABEL_LEN];
new gRuntimeDisconnectHookCount = 0;
new gRuntimeDisconnectHooks[MAX_RUNTIME_HOOKS][MAX_RUNTIME_LABEL_LEN];

forward Core_OnPlayerConnect(playerid);
forward Core_OnPlayerDisconnect(playerid, reason);
forward LoginSystem_OnPlayerConnect(playerid);
forward LoginSystem_OnPlayerDisconnect(playerid, reason);
forward StateSystem_OnPlayerConnect(playerid);
forward StateSystem_OnPlayerDisconnect(playerid, reason);
forward InventorySystem_OnPlayerConnect(playerid);
forward HouseStorageSystem_Connect(playerid);
forward UBBVehicleSystem_Connect(playerid);
forward AdminSystem_OnPlayerConnect(playerid);
forward AdminSystem_OnPlayerDisconnect(playerid, reason);
forward JobsCore_OnPlayerConnect(playerid);
forward ProgressionSystem_Connect(playerid);
forward JobCenter_OnPlayerConnect(playerid);
forward TextWars_OnPlayerConnect(playerid);
forward StateControl_OnPlayerConnect(playerid);
forward UiHub_OnPlayerConnect(playerid);
forward PremiumSystem_OnPlayerConnect(playerid);
forward PremiumSystem_OnPlayerDisconnect(playerid, reason);
forward JobsCore_OnPlayerDisconnect(playerid);
forward JobCenter_OnPlayerDisconnect(playerid);
forward TextWars_OnPlayerDisconnect(playerid);
forward LawSystem_OnPlayerDisconnect(playerid);
forward CrimeSystem_OnPlayerDisconnect(playerid);
forward EmergencySystem_OnPlayerDisconnect(playerid);

stock Core_RuntimeModuleExists(const moduleName[])
{
    for (new i = 0; i < gRuntimeModuleCount; i++)
    {
        if (strcmp(gRuntimeModules[i], moduleName, true) == 0)
        {
            return true;
        }
    }
    return false;
}

stock Core_RegisterRuntimeModule(const moduleName[])
{
    if (Core_RuntimeModuleExists(moduleName))
    {
        return true;
    }

    if (gRuntimeModuleCount >= MAX_RUNTIME_MODULES)
    {
        return false;
    }

    format(gRuntimeModules[gRuntimeModuleCount], MAX_RUNTIME_LABEL_LEN, "%s", moduleName);
    gRuntimeModuleCount++;
    return true;
}

stock Core_ConnectHookExists(const hookName[])
{
    for (new i = 0; i < gRuntimeConnectHookCount; i++)
    {
        if (strcmp(gRuntimeConnectHooks[i], hookName, true) == 0)
        {
            return true;
        }
    }
    return false;
}

stock Core_RegisterConnectHook(const hookName[])
{
    if (Core_ConnectHookExists(hookName))
    {
        return true;
    }

    if (gRuntimeConnectHookCount >= MAX_RUNTIME_HOOKS)
    {
        return false;
    }

    format(gRuntimeConnectHooks[gRuntimeConnectHookCount], MAX_RUNTIME_LABEL_LEN, "%s", hookName);
    gRuntimeConnectHookCount++;
    return true;
}

stock Core_DisconnectHookExists(const hookName[])
{
    for (new i = 0; i < gRuntimeDisconnectHookCount; i++)
    {
        if (strcmp(gRuntimeDisconnectHooks[i], hookName, true) == 0)
        {
            return true;
        }
    }
    return false;
}

stock Core_RegisterDisconnectHook(const hookName[])
{
    if (Core_DisconnectHookExists(hookName))
    {
        return true;
    }

    if (gRuntimeDisconnectHookCount >= MAX_RUNTIME_HOOKS)
    {
        return false;
    }

    format(gRuntimeDisconnectHooks[gRuntimeDisconnectHookCount], MAX_RUNTIME_LABEL_LEN, "%s", hookName);
    gRuntimeDisconnectHookCount++;
    return true;
}

stock Core_PrintRuntimeRegistry()
{
    printf("[CORE] Runtime-Registrierung: %d Module geladen", gRuntimeModuleCount);
    for (new i = 0; i < gRuntimeModuleCount; i++)
    {
        printf("[CORE] Module[%d]: %s", i, gRuntimeModules[i]);
    }

    printf("[CORE] Connect-Hooks: %d", gRuntimeConnectHookCount);
    for (new i = 0; i < gRuntimeConnectHookCount; i++)
    {
        printf("[CORE] ConnectHook[%d]: %s", i, gRuntimeConnectHooks[i]);
    }

    printf("[CORE] Disconnect-Hooks: %d", gRuntimeDisconnectHookCount);
    for (new i = 0; i < gRuntimeDisconnectHookCount; i++)
    {
        printf("[CORE] DisconnectHook[%d]: %s", i, gRuntimeDisconnectHooks[i]);
    }
}

stock Core_RunPlayerConnectHooks(playerid)
{
    if (!Core_ValidatePlayerSlot(playerid))
    {
        return;
    }

    Core_OnPlayerConnect(playerid);
    AdminSystem_OnPlayerConnect(playerid);
    LoginSystem_OnPlayerConnect(playerid);
    StateSystem_OnPlayerConnect(playerid);
    InventorySystem_OnPlayerConnect(playerid);
    HouseStorageSystem_Connect(playerid);
    UBBVehicleSystem_Connect(playerid);
    ProgressionSystem_Connect(playerid);
    JobsCore_OnPlayerConnect(playerid);
    JobCenter_OnPlayerConnect(playerid);
    TextWars_OnPlayerConnect(playerid);
    StateControl_OnPlayerConnect(playerid);
    UiHub_OnPlayerConnect(playerid);
    PremiumSystem_OnPlayerConnect(playerid);
}

stock Core_RunPlayerDisconnectHooks(playerid, reason)
{
    TextWars_OnPlayerDisconnect(playerid);
    JobCenter_OnPlayerDisconnect(playerid);
    JobsCore_OnPlayerDisconnect(playerid);
    LoginSystem_OnPlayerDisconnect(playerid, reason);
    AdminSystem_OnPlayerDisconnect(playerid, reason);
    StateSystem_OnPlayerDisconnect(playerid, reason);
    PremiumSystem_OnPlayerDisconnect(playerid, reason);
    LawSystem_OnPlayerDisconnect(playerid);
    CrimeSystem_OnPlayerDisconnect(playerid);
    EmergencySystem_OnPlayerDisconnect(playerid);
    HouseSystem_OnPlayerDisconnect(playerid);
    Core_OnPlayerDisconnect(playerid, reason);
}

stock Core_RegisterDefaultRuntimeModules()
{
    Core_RegisterRuntimeModule("core");
    Core_RegisterRuntimeModule("db");
    Core_RegisterRuntimeModule("state");
    Core_RegisterRuntimeModule("admin");
    Core_RegisterRuntimeModule("auth");
    Core_RegisterRuntimeModule("economy");
    Core_RegisterRuntimeModule("jobs");
    Core_RegisterRuntimeModule("housing");
    Core_RegisterRuntimeModule("vehicles");
    Core_RegisterRuntimeModule("factions");
    Core_RegisterRuntimeModule("government");
    Core_RegisterRuntimeModule("world");
    Core_RegisterRuntimeModule("social");
    Core_RegisterRuntimeModule("web");
    Core_RegisterRuntimeModule("ui");

    Core_RegisterConnectHook("Core_OnPlayerConnect");
    Core_RegisterConnectHook("AdminSystem_OnPlayerConnect");
    Core_RegisterConnectHook("LoginSystem_OnPlayerConnect");
    Core_RegisterConnectHook("StateSystem_OnPlayerConnect");
    Core_RegisterConnectHook("InventorySystem_OnPlayerConnect");
    Core_RegisterConnectHook("HouseStorageSystem_Connect");
    Core_RegisterConnectHook("UBBVehicleSystem_Connect");
    Core_RegisterConnectHook("ProgressionSystem_Connect");
    Core_RegisterConnectHook("JobsCore_OnPlayerConnect");
    Core_RegisterConnectHook("JobCenter_OnPlayerConnect");
    Core_RegisterConnectHook("TextWars_OnPlayerConnect");
    Core_RegisterConnectHook("StateControl_OnPlayerConnect");
    Core_RegisterConnectHook("UiHub_OnPlayerConnect");
    Core_RegisterConnectHook("PremiumSystem_OnPlayerConnect");

    Core_RegisterDisconnectHook("TextWars_OnPlayerDisconnect");
    Core_RegisterDisconnectHook("JobCenter_OnPlayerDisconnect");
    Core_RegisterDisconnectHook("JobsCore_OnPlayerDisconnect");
    Core_RegisterDisconnectHook("LoginSystem_OnPlayerDisconnect");
    Core_RegisterDisconnectHook("AdminSystem_OnPlayerDisconnect");
    Core_RegisterDisconnectHook("StateSystem_OnPlayerDisconnect");
    Core_RegisterDisconnectHook("PremiumSystem_OnPlayerDisconnect");
    Core_RegisterDisconnectHook("LawSystem_OnPlayerDisconnect");
    Core_RegisterDisconnectHook("CrimeSystem_OnPlayerDisconnect");
    Core_RegisterDisconnectHook("EmergencySystem_OnPlayerDisconnect");
    Core_RegisterDisconnectHook("Core_OnPlayerDisconnect");

    return true;
}

#endif
