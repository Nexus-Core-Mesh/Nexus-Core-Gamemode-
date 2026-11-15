#if !defined EMERGENCY_SYSTEM_INC
#define EMERGENCY_SYSTEM_INC

#include <a_samp>

#define EMERGENCY_ROLE_NONE 0
#define EMERGENCY_ROLE_POLICE 1
#define EMERGENCY_ROLE_EMS 2
#define MAX_EMERGENCY_STATIONS 6

new gEmergencyRole[MAX_PLAYERS];
new gEmergencyState[MAX_PLAYERS];
new gEmergencyStationCount = 0;
new gEmergencyStationState[MAX_EMERGENCY_STATIONS];
new Float:gEmergencyStationX[MAX_EMERGENCY_STATIONS];
new Float:gEmergencyStationY[MAX_EMERGENCY_STATIONS];
new Float:gEmergencyStationZ[MAX_EMERGENCY_STATIONS];
new gEmergencyStationName[MAX_EMERGENCY_STATIONS][32];

stock EmergencySystem_Initialize()
{
    gEmergencyStationCount = 0;

    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gEmergencyRole[i] = EMERGENCY_ROLE_NONE;
        gEmergencyState[i] = STATE_LOS_SANTOS;
    }

    EmergencySystem_RegisterStation(STATE_LOS_SANTOS, "LSPD HQ", 1535.0, -1675.0, 13.0);
    EmergencySystem_RegisterStation(STATE_LOS_SANTOS, "LS EMS", 1184.0, -1323.0, 14.0);
    EmergencySystem_RegisterStation(STATE_SAN_FIERRO, "SF Police", -1930.0, 289.0, 35.0);
    EmergencySystem_RegisterStation(STATE_SAN_FIERRO, "SF EMS", -2025.0, 142.0, 28.0);
    EmergencySystem_RegisterStation(STATE_LAS_VENTURAS, "LV Police", 1668.0, 1447.0, 10.0);
    EmergencySystem_RegisterStation(STATE_LAS_VENTURAS, "LV EMS", 1418.0, 1308.0, 12.0);

    print("[EMERGENCY] Polizei- und EMS-System initialisiert");
}

stock EmergencySystem_RegisterStation(stateid, const name[], Float:x, Float:y, Float:z)
{
    if (gEmergencyStationCount >= MAX_EMERGENCY_STATIONS)
        return false;
    if (!Core_IsValidState(stateid))
        return false;

    gEmergencyStationState[gEmergencyStationCount] = stateid;
    format(gEmergencyStationName[gEmergencyStationCount], 32, "%s", name);
    gEmergencyStationX[gEmergencyStationCount] = x;
    gEmergencyStationY[gEmergencyStationCount] = y;
    gEmergencyStationZ[gEmergencyStationCount] = z;
    gEmergencyStationCount++;
    return true;
}

stock EmergencySystem_OnPlayerConnect(playerid)
{
    gEmergencyRole[playerid] = EMERGENCY_ROLE_NONE;
    gEmergencyState[playerid] = STATE_LOS_SANTOS;
}

stock EmergencySystem_SetRole(playerid, role, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
        return;
    if (role < EMERGENCY_ROLE_NONE || role > EMERGENCY_ROLE_EMS)
        return;

    gEmergencyRole[playerid] = role;
    if (Core_IsValidState(stateid))
    {
        gEmergencyState[playerid] = stateid;
    }
}

stock int:EmergencySystem_GetRole(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
        return EMERGENCY_ROLE_NONE;
    return gEmergencyRole[playerid];
}

stock EmergencySystem_HealPlayer(playerid, targetid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS || targetid < 0 || targetid >= MAX_PLAYERS)
        return;

    SetPlayerHealth(targetid, 100.0);
    SetPlayerArmour(targetid, 50.0);
    SendClientMessage(targetid, 0x00FF00AA, "[EMS] Du wurdest behandelt und stabilisiert.");
    SendClientMessage(playerid, 0x00FF00AA, "[EMS] Behandlung wurde erfolgreich ausgeführt.");
}

stock EmergencySystem_ShowStations(playerid)
{
    new msg[256];
    format(msg, sizeof(msg), "[EMERGENCY] Verfügbare Einsatzstationen: %d", gEmergencyStationCount);
    SendClientMessage(playerid, 0x66CCFFAA, msg);

    for (new i = 0; i < gEmergencyStationCount; i++)
    {
        new stationMsg[128];
        format(stationMsg, sizeof(stationMsg), "%s | Staat %d | Pos: %.0f %.0f %.0f", gEmergencyStationName[i], gEmergencyStationState[i], gEmergencyStationX[i], gEmergencyStationY[i], gEmergencyStationZ[i]);
        SendClientMessage(playerid, 0xFFFFFFFF, stationMsg);
    }
}

stock EmergencySystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
        return false;

    new cmd[32], idx;
    idx = sscanf(cmdtext, "s[32]", cmd);
    if (idx == 0)
        return false;

    if (strcmp(cmd, "/ems", true) == 0)
    {
        if (gEmergencyRole[playerid] != EMERGENCY_ROLE_EMS)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[EMS] Du bist kein EMS-Mitarbeiter.");
            return true;
        }

        new target;
        if (sscanf(cmdtext, "s[32]d", cmd, target) != 0)
        {
            SendClientMessage(playerid, 0xFFCC66AA, "[EMS] Syntax: /ems <PlayerID>");
            return true;
        }

        EmergencySystem_HealPlayer(playerid, target);
        return true;
    }

    if (strcmp(cmd, "/police", true) == 0)
    {
        if (gEmergencyRole[playerid] != EMERGENCY_ROLE_POLICE)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[POLICE] Du bist kein Polizeibeamter.");
            return true;
        }

        new msg[128];
        format(msg, sizeof(msg), "[POLICE] Staat: %d | Bereitschaft: %d | Wanted-Status aktiv", gEmergencyState[playerid], StateSystem_GetPlayerState(playerid));
        SendClientMessage(playerid, 0x00A6FFAA, msg);
        return true;
    }

    if (strcmp(cmd, "/stations", true) == 0)
    {
        EmergencySystem_ShowStations(playerid);
        return true;
    }

    return false;
}

#endif
