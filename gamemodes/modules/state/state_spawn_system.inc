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
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(stateid, Core_GetPlayerState(playerid));
    if (!Core_IsValidState(safeState))
    {
        safeState = STATE_LOS_SANTOS;
    }

    new safeHomeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), safeState);
    if (!Core_IsValidState(safeHomeState))
    {
        safeHomeState = safeState;
    }

    Core_SetPlayerState(playerid, safeState);
    Core_SetPlayerHomeState(playerid, safeHomeState);
    DB_ValidateStateTransfer(playerid, safeState, safeHomeState);
    DB_SetPlayerState(playerid, safeState);
    DB_SavePlayerState(playerid, safeState);

    new skinid = Core_GetPlayerSkin(playerid);
    if (skinid < 0 || skinid > 311)
    {
        skinid = gPlayerData[playerid][E_PLAYER_SKIN];
    }
    if (skinid < 0 || skinid > 311)
    {
        skinid = (Core_GetPlayerGender(playerid) == GENDER_FEMALE ? 9 : 2);
    }
    if (skinid >= 0 && skinid <= 311)
    {
        SetPlayerSkin(playerid, skinid);
        Core_SetPlayerSkin(playerid, skinid);
        gPlayerData[playerid][E_PLAYER_SKIN] = skinid;
    }

    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    CancelSelectTextDraw(playerid);
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    TutorialSystem_HideStage(playerid);
    TogglePlayerSpectating(playerid, false);
    CameraSystem_ExitCameraMode(playerid);
    CameraSystem_Reset(playerid);
    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, 1);

    if (gStateSpawnCount[safeState] <= 0)
    {
        StateSystem_SpawnPlayerForState(playerid, safeState);
        RoleplayUI_ShowDashboard(playerid);
        return true;
    }

    new slot = gStateSpawnCount[safeState] - 1;
    new stateName[32];
    StateSystem_GetStateName(safeState, stateName, sizeof(stateName));

    SetPlayerPos(playerid,
        gStateSpawnXByState[safeState][slot],
        gStateSpawnYByState[safeState][slot],
        gStateSpawnZByState[safeState][slot]);
    SetPlayerFacingAngle(playerid, gStateSpawnHeadingByState[safeState][slot]);
    RoleplayUI_ShowDashboard(playerid);

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

stock bool:StateSpawnSystem_RegisterStateCoordinates(stateid, Float:x, Float:y, Float:z, Float:angle)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new slot = gStateSpawnCount[stateid];
    if (slot >= MAX_STATE_SPAWN_POINTS)
    {
        return false;
    }
    gStateSpawnXByState[stateid][slot] = x;
    gStateSpawnYByState[stateid][slot] = y;
    gStateSpawnZByState[stateid][slot] = z;
    gStateSpawnHeadingByState[stateid][slot] = angle;
    gStateSpawnCount[stateid]++;
    return true;
}

stock bool:StateSpawnSystem_SetStateSpawn(stateid, Float:x, Float:y, Float:z, Float:angle)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(stateid, STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    new slot = gStateSpawnCount[safeState];
    if (slot >= MAX_STATE_SPAWN_POINTS)
    {
        slot = MAX_STATE_SPAWN_POINTS - 1;
    }

    gStateSpawnXByState[safeState][slot] = x;
    gStateSpawnYByState[safeState][slot] = y;
    gStateSpawnZByState[safeState][slot] = z;
    gStateSpawnHeadingByState[safeState][slot] = angle;
    if (gStateSpawnCount[safeState] < MAX_STATE_SPAWN_POINTS)
    {
        gStateSpawnCount[safeState]++;
    }
    return true;
}

stock bool:StateSpawnSystem_GetStateSpawn(stateid, slot, &Float:x, &Float:y, &Float:z, &Float:angle)
{
    if (!Core_IsValidState(stateid) || slot < 0 || slot >= MAX_STATE_SPAWN_POINTS)
    {
        x = 0.0; y = 0.0; z = 0.0; angle = 0.0;
        return false;
    }

    x = gStateSpawnXByState[stateid][slot];
    y = gStateSpawnYByState[stateid][slot];
    z = gStateSpawnZByState[stateid][slot];
    angle = gStateSpawnHeadingByState[stateid][slot];
    return true;
}

stock StateSpawnSystem_TpToState(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidState(stateid))
    {
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(stateid, Core_GetPlayerState(playerid));
    if (!Core_IsValidState(safeState))
    {
        safeState = STATE_LOS_SANTOS;
    }

    Core_SetPlayerState(playerid, safeState);
    Core_SetPlayerHomeState(playerid, safeState);
    DB_ValidateStateTransfer(playerid, safeState, safeState);
    DB_SavePlayerState(playerid, safeState);
    StateSpawnSystem_Apply(playerid, safeState);
    return true;
}

stock StateSpawnSystem_TpToPos(playerid, Float:x, Float:y, Float:z, Float:angle)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    SetPlayerPos(playerid, x, y, z);
    SetPlayerFacingAngle(playerid, angle);
    CameraSystem_Reset(playerid);
    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, 1);
    return true;
}

#endif
