#if !defined PRISON_SYSTEM_INC
#define PRISON_SYSTEM_INC

#include <a_samp>

#define MAX_PRISON_STATES 3
#define PRISON_CELL_COUNT 3
#define PRISON_STATUS_FREE 0
#define PRISON_STATUS_INCARCERATED 1

new gPrisonState[MAX_PLAYERS];
new gPrisonTimeLeft[MAX_PLAYERS];
new gPrisonOriginState[MAX_PLAYERS];
new gPrisonCell[MAX_PLAYERS];
new gPrisonBail[MAX_PLAYERS];
new gPrisonTransferFee[MAX_PLAYERS];
new gPrisonFinePaid[MAX_PLAYERS];
new gPrisonBank[3];

new gPrisonSpawnX[3][PRISON_CELL_COUNT];
new gPrisonSpawnY[3][PRISON_CELL_COUNT];
new gPrisonSpawnZ[3][PRISON_CELL_COUNT];

stock PrisonSystem_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gPrisonState[i] = PRISON_STATUS_FREE;
        gPrisonTimeLeft[i] = 0;
        gPrisonOriginState[i] = -1;
        gPrisonCell[i] = 0;
        gPrisonBail[i] = 0;
        gPrisonTransferFee[i] = 0;
        gPrisonFinePaid[i] = 0;
    }

    gPrisonBank[STATE_LOS_SANTOS] = 150000;
    gPrisonBank[STATE_SAN_FIERRO] = 120000;
    gPrisonBank[STATE_LAS_VENTURAS] = 135000;

    gPrisonSpawnX[STATE_LOS_SANTOS][0] = 1640; gPrisonSpawnY[STATE_LOS_SANTOS][0] = 1750; gPrisonSpawnZ[STATE_LOS_SANTOS][0] = 10;
    gPrisonSpawnX[STATE_SAN_FIERRO][0] = -1980; gPrisonSpawnY[STATE_SAN_FIERRO][0] = 152; gPrisonSpawnZ[STATE_SAN_FIERRO][0] = 27;
    gPrisonSpawnX[STATE_LAS_VENTURAS][0] = 1760; gPrisonSpawnY[STATE_LAS_VENTURAS][0] = 920; gPrisonSpawnZ[STATE_LAS_VENTURAS][0] = 10;

    print("[PRISON] Justiz- und Haftsystem initialisiert");
}

stock PrisonSystem_OnPlayerConnect(playerid)
{
    gPrisonState[playerid] = PRISON_STATUS_FREE;
    gPrisonTimeLeft[playerid] = 0;
    gPrisonOriginState[playerid] = StateSystem_GetPlayerState(playerid);
    gPlayerData[playerid][E_PLAYER_JAIL_ACTIVE] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_TIME] = 0;
}

stock PrisonSystem_SendToPrison(playerid, stateid, prisonSeconds)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }

    gPrisonState[playerid] = PRISON_STATUS_INCARCERATED;
    gPrisonTimeLeft[playerid] = prisonSeconds;
    gPrisonOriginState[playerid] = stateid;
    gPrisonCell[playerid] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_ACTIVE] = 1;
    gPlayerData[playerid][E_PLAYER_JAIL_TIME] = prisonSeconds;
    gPlayerData[playerid][E_PLAYER_STATE] = stateid;

    SetPlayerPos(playerid, gPrisonSpawnX[stateid][gPrisonCell[playerid]], gPrisonSpawnY[stateid][gPrisonCell[playerid]], gPrisonSpawnZ[stateid][gPrisonCell[playerid]]);
    TogglePlayerControllable(playerid, false);
    gPrisonBank[stateid] += 2500;
    StateSystem_ApplyTreasuryDelta(stateid, 2500);

    new msg[128];
    format(msg, sizeof(msg), "[PRISON] Du wurdest in %s inhaftiert. Zeit: %d s", (stateid == STATE_LOS_SANTOS) ? ("Los Santos") : ((stateid == STATE_SAN_FIERRO) ? ("San Fierro") : ("Las Venturas")), prisonSeconds);
    SendClientMessage(playerid, 0xFF6666AA, msg);
    return true;
}

stock PrisonSystem_ReleasePlayer(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    gPrisonState[playerid] = PRISON_STATUS_FREE;
    gPrisonTimeLeft[playerid] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_ACTIVE] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_TIME] = 0;
    TogglePlayerControllable(playerid, true);

    new stateid = StateSystem_GetPlayerState(playerid);
    if (stateid < 0 || stateid >= 3)
    {
        stateid = STATE_LOS_SANTOS;
    }

    StateSystem_SpawnPlayerForState(playerid, stateid);
    SendClientMessage(playerid, 0x00FF00AA, "[PRISON] Haft beendet. Du wurdest freigelassen.");
    return true;
}

stock PrisonSystem_PayBail(playerid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (amount <= 0)
    {
        return false;
    }

    new money = GetPlayerMoney(playerid);
    if (money < amount)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[PRISON] Du hast nicht genug Geld für die Kaution.");
        return false;
    }

    new stateid = StateSystem_GetPlayerState(playerid);
    if (!StateSystem_IsValidState(stateid))
    {
        stateid = STATE_LOS_SANTOS;
    }

    GivePlayerMoney(playerid, -amount);
    StateSystem_ApplyTreasuryDelta(stateid, amount);
    gPrisonBank[stateid] += amount;
    PrisonSystem_ReleasePlayer(playerid);
    SendClientMessage(playerid, 0x00FF00FF, "[PRISON] Du wurdest auf Kaution freigelassen.");
    return true;
}

stock int:PrisonSystem_GetTimeLeft(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gPrisonTimeLeft[playerid];
}

stock PrisonSystem_Tick(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (gPrisonState[playerid] != PRISON_STATUS_INCARCERATED)
    {
        return false;
    }

    if (gPrisonTimeLeft[playerid] > 0)
    {
        gPrisonTimeLeft[playerid]--;
        gPlayerData[playerid][E_PLAYER_JAIL_TIME] = gPrisonTimeLeft[playerid];
        if (gPrisonTimeLeft[playerid] <= 0)
        {
            PrisonSystem_ReleasePlayer(playerid);
            return true;
        }
    }
    return true;
}

stock PrisonSystem_ShowStatus(playerid)
{
    new line[256];
    format(line, sizeof(line), "[PRISON] Status: %d | Zeit: %d | Origin: %d | Staat: %d", gPrisonState[playerid], gPrisonTimeLeft[playerid], gPrisonOriginState[playerid], StateSystem_GetPlayerState(playerid));
    SendClientMessage(playerid, 0xFFCC66AA, line);
}

#endif
