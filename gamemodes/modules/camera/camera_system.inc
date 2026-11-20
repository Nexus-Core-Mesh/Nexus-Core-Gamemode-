#if !defined CAMERA_SYSTEM_INC
#define CAMERA_SYSTEM_INC

#include <a_samp>

#define CAMERA_MODE_NONE 0
#define CAMERA_MODE_TUTORIAL 1
#define CAMERA_MODE_FREE 2
#define CAMERA_MODE_DRONE 3
#define CAMERA_MODE_FOCUS 4

new gCameraMode[MAX_PLAYERS];
new bool:gCameraActive[MAX_PLAYERS];
new Float:gCameraPosX[MAX_PLAYERS];
new Float:gCameraPosY[MAX_PLAYERS];
new Float:gCameraPosZ[MAX_PLAYERS];
new Float:gCameraLookX[MAX_PLAYERS];
new Float:gCameraLookY[MAX_PLAYERS];
new Float:gCameraLookZ[MAX_PLAYERS];

stock CameraSystem_Initialize()
{
    print("[CAMERA] Kamerasystem initialisiert");
}

stock CameraSystem_Reset(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    gCameraMode[playerid] = CAMERA_MODE_NONE;
    gCameraActive[playerid] = false;
    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, true);
}

stock bool:CameraSystem_IsActive(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return gCameraActive[playerid];
}

stock CameraSystem_SetMode(playerid, mode, Float:x, Float:y, Float:z, Float:lookX, Float:lookY, Float:lookZ)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    gCameraMode[playerid] = mode;
    gCameraActive[playerid] = true;
    gCameraPosX[playerid] = x;
    gCameraPosY[playerid] = y;
    gCameraPosZ[playerid] = z;
    gCameraLookX[playerid] = lookX;
    gCameraLookY[playerid] = lookY;
    gCameraLookZ[playerid] = lookZ;

    SetPlayerCameraPos(playerid, x, y, z);
    SetPlayerCameraLookAt(playerid, lookX, lookY, lookZ);
    TogglePlayerControllable(playerid, false);
}

stock CameraSystem_StartFreeCamera(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    CameraSystem_Reset(playerid);
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    CameraSystem_SetMode(playerid, CAMERA_MODE_FREE, x + 8.0, y + 8.0, z + 8.0, x + 10.0, y + 10.0, z + 2.0);
    SendClientMessage(playerid, 0x00D9FFFF, "[CAMERA] Freie Kameraperspektive aktiv.");
}

stock CameraSystem_StartDroneCamera(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    CameraSystem_Reset(playerid);
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    CameraSystem_SetMode(playerid, CAMERA_MODE_DRONE, x + 25.0, y + 25.0, z + 30.0, x, y, z + 2.0);
    SendClientMessage(playerid, 0x00D9FFFF, "[CAMERA] Drohnenansicht aktiviert.");
}

stock CameraSystem_StartFocusCamera(playerid, Float:x, Float:y, Float:z, Float:lookX, Float:lookY, Float:lookZ)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    CameraSystem_SetMode(playerid, CAMERA_MODE_FOCUS, x, y, z, lookX, lookY, lookZ);
    SendClientMessage(playerid, 0x00D9FFFF, "[CAMERA] Fokusmodus aktiviert.");
}

stock CameraSystem_StartStatePreview(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidState(stateid))
    {
        return;
    }

    new Float:x = float(gStateSpawnX[stateid]);
    new Float:y = float(gStateSpawnY[stateid]);
    new Float:z = float(gStateSpawnZ[stateid]);
    CameraSystem_SetMode(playerid, CAMERA_MODE_FOCUS, x + 18.0, y + 18.0, z + 18.0, x, y, z + 2.0);
    SendClientMessage(playerid, 0x00D9FFFF, "[CAMERA] Spawn-/Staatsvorschau aktiv.");
}

stock CameraSystem_ToggleFreeCamera(playerid)
{
    if (gCameraMode[playerid] == CAMERA_MODE_FREE && gCameraActive[playerid])
    {
        CameraSystem_Reset(playerid);
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, true);
        SendClientMessage(playerid, 0x00D9FFFF, "[CAMERA] Freie Kamera beendet.");
        return;
    }

    CameraSystem_StartFreeCamera(playerid);
}

stock CameraSystem_ExitCameraMode(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    CameraSystem_Reset(playerid);
    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, true);
}
stock CameraSystem_PlayIntroScene(playerid)
{
    gCameraMode[playerid] = CAMERA_MODE_TUTORIAL;
    SetPlayerPos(playerid, 1480.0, -1750.0, 13.5);
    SetPlayerFacingAngle(playerid, 180.0);
    SetPlayerCameraPos(playerid, 1490.0, -1745.0, 18.5);
    SetPlayerCameraLookAt(playerid, 1480.0, -1750.0, 13.5);
    TogglePlayerControllable(playerid, false);
    SendClientMessage(playerid, 0xFFCC00AA, "[CAMERA] Tutorial-Szene: Willkommen in Los Santos.");
}

stock CameraSystem_PlayStateScene(playerid, stateid)
{
    if (stateid == 0)
    {
        SetPlayerCameraPos(playerid, 1508.0, -1706.0, 20.0);
        SetPlayerCameraLookAt(playerid, 1520.0, -1730.0, 14.0);
        SendClientMessage(playerid, 0x00FF66AA, "[CAMERA] Heimatstadt: Los Santos");
    }
    else if (stateid == 1)
    {
        SetPlayerCameraPos(playerid, -1960.0, 128.0, 40.0);
        SetPlayerCameraLookAt(playerid, -1930.0, 118.0, 20.0);
        SendClientMessage(playerid, 0x00FF66AA, "[CAMERA] Heimatstadt: San Fierro");
    }
    else if (stateid == 2)
    {
        SetPlayerCameraPos(playerid, 1690.0, 1224.0, 40.0);
        SetPlayerCameraLookAt(playerid, 1710.0, 1215.0, 20.0);
        SendClientMessage(playerid, 0x00FF66AA, "[CAMERA] Heimatstadt: Las Venturas");
    }
}

#endif
