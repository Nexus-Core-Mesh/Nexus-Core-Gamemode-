#if !defined CAMERA_SYSTEM_INC
#define CAMERA_SYSTEM_INC

#include <a_samp>

#define CAMERA_MODE_NONE 0
#define CAMERA_MODE_TUTORIAL 1

new gCameraMode[MAX_PLAYERS];

stock CameraSystem_Initialize()
{
    print("[CAMERA] Kamerasystem initialisiert");
}

stock CameraSystem_Reset(playerid)
{
    gCameraMode[playerid] = CAMERA_MODE_NONE;
    SetPlayerCameraPos(playerid, 0.0, 0.0, 0.0);
    SetPlayerCameraLookAt(playerid, 0.0, 0.0, 0.0);
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
