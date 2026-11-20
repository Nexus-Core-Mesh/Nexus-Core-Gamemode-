#if !defined STATE_SPAWN_SYSTEM_INC
#define STATE_SPAWN_SYSTEM_INC

#include <a_samp>

#define MAX_STATE_SPAWN_POINTS 3

new gStateSpawnXByState[STATE_COUNT][MAX_STATE_SPAWN_POINTS];
new gStateSpawnYByState[STATE_COUNT][MAX_STATE_SPAWN_POINTS];
new gStateSpawnZByState[STATE_COUNT][MAX_STATE_SPAWN_POINTS];
new gStateSpawnHeadingByState[STATE_COUNT][MAX_STATE_SPAWN_POINTS];
new gStateSpawnCount[STATE_COUNT];

stock StateSpawnSystem_Initialize()
{
    for (new stateid = 0; stateid < STATE_COUNT; stateid++)
    {
        gStateSpawnCount[stateid] = 0;
        for (new slot = 0; slot < MAX_STATE_SPAWN_POINTS; slot++)
        {
            gStateSpawnXByState[stateid][slot] = 0.0;
            gStateSpawnYByState[stateid][slot] = 0.0;
            gStateSpawnZByState[stateid][slot] = 0.0;
            gStateSpawnHeadingByState[stateid][slot] = 0.0;
        }
    }

    StateSpawnSystem_RegisterSpawn(STATE_LOS_SANTOS, 1500.0, -1710.0, 13.0, 0.0);
    StateSpawnSystem_RegisterSpawn(STATE_LOS_SANTOS, 1464.0, -1788.0, 13.0, 90.0);
    StateSpawnSystem_RegisterSpawn(STATE_LOS_SANTOS, 1568.0, -1680.0, 13.0, 180.0);

    StateSpawnSystem_RegisterSpawn(STATE_SAN_FIERRO, -1930.0, 120.0, 27.0, 90.0);
    StateSpawnSystem_RegisterSpawn(STATE_SAN_FIERRO, -1938.0, 160.0, 27.0, 180.0);
    StateSpawnSystem_RegisterSpawn(STATE_SAN_FIERRO, -1885.0, 80.0, 27.0, 270.0);

    StateSpawnSystem_RegisterSpawn(STATE_LAS_VENTURAS, 1700.0, 1210.0, 16.0, 0.0);
    StateSpawnSystem_RegisterSpawn(STATE_LAS_VENTURAS, 1765.0, 1288.0, 16.0, 180.0);
    StateSpawnSystem_RegisterSpawn(STATE_LAS_VENTURAS, 1630.0, 1170.0, 16.0, 90.0);

    print("[STATE_SPAWN] Staatliche Spawns für LS/SF/LV initialisiert");
}

stock StateSpawnSystem_RegisterSpawn(stateid, Float:x, Float:y, Float:z, Float:heading)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (gStateSpawnCount[stateid] >= MAX_STATE_SPAWN_POINTS)
    {
        return false;
    }

    new slot = gStateSpawnCount[stateid];
    gStateSpawnXByState[stateid][slot] = x;
    gStateSpawnYByState[stateid][slot] = y;
    gStateSpawnZByState[stateid][slot] = z;
    gStateSpawnHeadingByState[stateid][slot] = heading;
    gStateSpawnCount[stateid]++;
    return true;
}

stock int:StateSpawnSystem_GetSpawnCount(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }
    return gStateSpawnCount[stateid];
}

stock StateSpawnSystem_Apply(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidState(stateid))
    {
        return false;
    }

    if (gStateSpawnCount[stateid] <= 0)
    {
        StateSystem_SpawnPlayerForState(playerid, stateid);
        Core_SetPlayerState(playerid, stateid);
        Core_SetPlayerHomeState(playerid, stateid);
        DB_SetPlayerState(playerid, stateid);
        DB_SavePlayerState(playerid, stateid);
        return true;
    }

    new slot = gStateSpawnCount[stateid] - 1;
    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));

    SetPlayerPos(playerid,
        gStateSpawnXByState[stateid][slot],
        gStateSpawnYByState[stateid][slot],
        gStateSpawnZByState[stateid][slot]);
    SetPlayerFacingAngle(playerid, gStateSpawnHeadingByState[stateid][slot]);
    Core_SetPlayerState(playerid, stateid);
    Core_SetPlayerHomeState(playerid, stateid);
    DB_SetPlayerState(playerid, stateid);
    DB_SavePlayerState(playerid, stateid);
    CameraSystem_Reset(playerid);

    new msg[128];
    format(msg, sizeof(msg), "[STATE] Spawn gesetzt: %s", stateName);
    SendClientMessage(playerid, 0x00FF00AA, msg);
    return true;
}

stock StateSpawnSystem_SetHome(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidState(stateid))
    {
        return false;
    }

    Core_SetPlayerHomeState(playerid, stateid);
    Core_SetPlayerState(playerid, stateid);
    return StateSpawnSystem_Apply(playerid, stateid);
}

#endif
