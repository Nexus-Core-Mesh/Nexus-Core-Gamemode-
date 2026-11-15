#if !defined MODULE_CONTRACT_INC
#define MODULE_CONTRACT_INC

#include <a_samp>

#define CORE_LIFECYCLE_BOOT 0
#define CORE_LIFECYCLE_DB_READY 1
#define CORE_LIFECYCLE_STATE_READY 2
#define CORE_LIFECYCLE_AUTH_READY 3
#define CORE_LIFECYCLE_WORLD_READY 4
#define CORE_LIFECYCLE_RUNTIME_READY 5
#define MAX_MODULE_CONTRACTS 48

new gLifecycleStage = CORE_LIFECYCLE_BOOT;
new gLifecycleModuleCount = 0;
new gLifecycleModules[MAX_MODULE_CONTRACTS][32];
new gLifecycleModuleMinStage[MAX_MODULE_CONTRACTS];
new gLifecycleModuleReady[MAX_MODULE_CONTRACTS];

stock Core_SetLifecycleStage(stage)
{
    if (stage < CORE_LIFECYCLE_BOOT || stage > CORE_LIFECYCLE_RUNTIME_READY)
    {
        return;
    }
    gLifecycleStage = stage;
}

stock int:Core_GetLifecycleStage()
{
    return gLifecycleStage;
}

// minStage: frühester Lifecycle-Stage, in dem das Modul initialisiert werden darf.
stock Core_RegisterModuleContract(const moduleName[], minStage = CORE_LIFECYCLE_BOOT)
{
    if (minStage < CORE_LIFECYCLE_BOOT || minStage > CORE_LIFECYCLE_RUNTIME_READY)
    {
        return false;
    }

    for (new i = 0; i < gLifecycleModuleCount; i++)
    {
        if (strcmp(gLifecycleModules[i], moduleName, true) == 0)
        {
            gLifecycleModuleMinStage[i] = max(gLifecycleModuleMinStage[i], minStage);
            return true;
        }
    }

    if (gLifecycleModuleCount >= MAX_MODULE_CONTRACTS)
    {
        return false;
    }

    format(gLifecycleModules[gLifecycleModuleCount], 32, "%s", moduleName);
    gLifecycleModuleMinStage[gLifecycleModuleCount] = minStage;
    gLifecycleModuleReady[gLifecycleModuleCount] = 0;
    gLifecycleModuleCount++;
    return true;
}

stock int:Core_FindModuleContract(const moduleName[])
{
    for (new i = 0; i < gLifecycleModuleCount; i++)
    {
        if (strcmp(gLifecycleModules[i], moduleName, true) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Markiert ein Modul als initialisiert. Schlägt fehl, wenn es nicht registriert ist
// oder sein Mindest-Stage noch nicht erreicht wurde (verfrühter DB-/State-Zugriff).
stock Core_MarkModuleReady(const moduleName[])
{
    new idx = Core_FindModuleContract(moduleName);
    if (idx < 0)
    {
        printf("[CORE] Modul '%s' ohne Contract initialisiert!", moduleName);
        return false;
    }
    if (gLifecycleStage < gLifecycleModuleMinStage[idx])
    {
        printf("[CORE] Modul '%s' zu früh initialisiert (Stage %d < %d)", moduleName, gLifecycleStage, gLifecycleModuleMinStage[idx]);
        return false;
    }
    gLifecycleModuleReady[idx] = 1;
    return true;
}

stock Core_IsModuleReady(const moduleName[])
{
    new idx = Core_FindModuleContract(moduleName);
    return idx >= 0 && gLifecycleModuleReady[idx] == 1;
}

stock Core_AssertLifecycleStage(minStage)
{
    return gLifecycleStage >= minStage;
}

stock Core_PrintLifecycleReport()
{
    printf("[CORE] Lifecycle stage: %d", gLifecycleStage);
    for (new i = 0; i < gLifecycleModuleCount; i++)
    {
        printf("[CORE] Contract[%d]: %s (minStage %d, ready: %d)", i, gLifecycleModules[i], gLifecycleModuleMinStage[i], gLifecycleModuleReady[i]);
        if (gLifecycleModuleReady[i] != 1)
        {
            printf("[CORE] WARNUNG: Modul '%s' wurde nicht initialisiert!", gLifecycleModules[i]);
        }
    }
}

#endif
