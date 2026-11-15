#if !defined PROGRESSION_SYSTEM_INC
#define PROGRESSION_SYSTEM_INC

#include <a_samp>

#define PROGRESSION_BASE_XP 1000

new gPlayerLevel[MAX_PLAYERS];
new gPlayerXP[MAX_PLAYERS];
new gPlayerRep[MAX_PLAYERS];

stock ProgressionSystem_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gPlayerLevel[i] = 1;
        gPlayerXP[i] = 0;
        gPlayerRep[i] = 0;
    }

    print("[PROGRESSION] Levelsystem und Spielerfortschritt initialisiert");
}

stock ProgressionSystem_OnPlayerConnect(playerid)
{
    gPlayerLevel[playerid] = 1;
    gPlayerXP[playerid] = 0;
    gPlayerRep[playerid] = 0;
    gPlayerData[playerid][E_PLAYER_LEVEL] = 1;
    gPlayerData[playerid][E_PLAYER_XP] = 0;
    gPlayerData[playerid][E_PLAYER_REP] = 0;
    gPlayerData[playerid][E_PLAYER_BANK] = 2500;
    GivePlayerMoney(playerid, 2500);
}

stock ProgressionSystem_SyncBank(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    Core_SetPlayerBank(playerid, gPlayerData[playerid][E_PLAYER_BANK]);
}

stock ProgressionSystem_AddXP(playerid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    if (amount <= 0)
    {
        return;
    }

    gPlayerXP[playerid] += amount;
    gPlayerData[playerid][E_PLAYER_XP] = gPlayerXP[playerid];
    while (gPlayerXP[playerid] >= PROGRESSION_BASE_XP)
    {
        gPlayerXP[playerid] -= PROGRESSION_BASE_XP;
        gPlayerLevel[playerid]++;
        gPlayerData[playerid][E_PLAYER_LEVEL] = gPlayerLevel[playerid];
    }
}

stock ProgressionSystem_AddRep(playerid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    if (amount <= 0)
    {
        return;
    }

    gPlayerRep[playerid] += amount;
    gPlayerData[playerid][E_PLAYER_REP] = gPlayerRep[playerid];
}

stock int:ProgressionSystem_GetLevel(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 1;
    }
    return gPlayerLevel[playerid];
}

stock int:ProgressionSystem_GetXP(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gPlayerXP[playerid];
}

stock int:ProgressionSystem_GetRep(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gPlayerRep[playerid];
}

stock ProgressionSystem_ShowStats(playerid)
{
    new msg[256];
    format(msg, sizeof(msg), "[PROGRESSION] Level: %d | XP: %d | Reputation: %d | Bank: %d", gPlayerLevel[playerid], gPlayerXP[playerid], gPlayerRep[playerid], Core_GetPlayerBank(playerid));
    SendClientMessage(playerid, 0x99CC00AA, msg);
}

#endif
