#if !defined LAW_SYSTEM_INC
#define LAW_SYSTEM_INC

#include <a_samp>

#define LAW_ROLE_NONE 0
#define LAW_ROLE_POLICE 1
#define LAW_ROLE_EMS 2
#define LAW_ROLE_GOV 3
#define MAX_WANTED_LEVEL 6

new gWantedLevel[MAX_PLAYERS];
new gArrestTime[MAX_PLAYERS];
new gPlayerLawRole[MAX_PLAYERS];
new gPolicePresence[3];
new gStateWantedCount[3];
new Text:gLawTD[MAX_PLAYERS][8];

stock LawSystem_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gWantedLevel[i] = 0;
        gArrestTime[i] = 0;
        gPlayerLawRole[i] = LAW_ROLE_NONE;
    }

    for (new i = 0; i < 3; i++)
    {
        gPolicePresence[i] = 2;
        gStateWantedCount[i] = 0;
    }

    print("[LAW] Wanted-, Arrest- und Polizeisystem initialisiert");
}

stock LawSystem_OnPlayerConnect(playerid)
{
    gWantedLevel[playerid] = 0;
    gArrestTime[playerid] = 0;
    gPlayerLawRole[playerid] = LAW_ROLE_NONE;
    gPlayerData[playerid][E_PLAYER_WANTED] = 0;
    gPlayerData[playerid][E_PLAYER_LAW_ROLE] = LAW_ROLE_NONE;
}

stock LawSystem_OnPlayerDisconnect(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    LawSystem_HideWanted(playerid);
    new stateid = Core_GetValidStateOrDefault(Core_GetPlayerState(playerid), Core_GetPlayerHomeState(playerid));
    if (Core_IsValidState(stateid))
    {
        DispatchSystem_ClearStateCalls(stateid);
    }

    gWantedLevel[playerid] = 0;
    gArrestTime[playerid] = 0;
    gPlayerLawRole[playerid] = LAW_ROLE_NONE;
    gPlayerData[playerid][E_PLAYER_WANTED] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_ACTIVE] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_TIME] = 0;
    gPlayerData[playerid][E_PLAYER_LAW_ROLE] = LAW_ROLE_NONE;
}

stock LawSystem_SetWantedLevel(playerid, level)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    if (level < 0)
    {
        level = 0;
    }
    if (level > MAX_WANTED_LEVEL)
    {
        level = MAX_WANTED_LEVEL;
    }

    new lastLevel = gWantedLevel[playerid];
    if (lastLevel == level && gPlayerData[playerid][E_PLAYER_WANTED] == level)
    {
        return;
    }

    gWantedLevel[playerid] = level;
    gPlayerData[playerid][E_PLAYER_WANTED] = level;

    new stateid = Core_GetValidStateOrDefault(StateSystem_GetPlayerState(playerid), Core_GetPlayerHomeState(playerid));
    if (!Core_IsValidState(stateid))
    {
        stateid = STATE_LOS_SANTOS;
    }

    if (level > 0)
    {
        if (lastLevel == 0)
        {
            gStateWantedCount[stateid] = gStateWantedCount[stateid] + 1;
            if (gStateWantedCount[stateid] < 0)
            {
                gStateWantedCount[stateid] = 0;
            }

            DispatchSystem_CreateCall(stateid, "Wanted-Verbrecher im Einsatzgebiet", DISPATCH_PRIORITY_HIGH);
        }
    }
    else if (lastLevel > 0)
    {
        if (gStateWantedCount[stateid] > 0)
        {
            gStateWantedCount[stateid]--;
        }
        if (gStateWantedCount[stateid] < 0)
        {
            gStateWantedCount[stateid] = 0;
        }
        DispatchSystem_ClearStateCalls(stateid);
        LawSystem_ClearWanted(playerid);
    }
}

stock LawSystem_AddWanted(playerid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    if (amount <= 0)
    {
        return;
    }

    new newLevel = gWantedLevel[playerid] + amount;
    if (newLevel > MAX_WANTED_LEVEL)
    {
        newLevel = MAX_WANTED_LEVEL;
    }

    gWantedLevel[playerid] = newLevel;
    gPlayerData[playerid][E_PLAYER_WANTED] = newLevel;
    if (newLevel > 0)
    {
        new stateid = Core_GetValidStateOrDefault(StateSystem_GetPlayerState(playerid), Core_GetPlayerHomeState(playerid));
        if (Core_IsValidState(stateid))
        {
            DispatchSystem_CreateCall(stateid, "Wanted-Verbrecher im Einsatzgebiet", DISPATCH_PRIORITY_HIGH);
        }
    }
}

stock LawSystem_ClearWanted(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    gWantedLevel[playerid] = 0;
    gArrestTime[playerid] = 0;
    gPlayerData[playerid][E_PLAYER_WANTED] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_ACTIVE] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_TIME] = 0;
}

stock LawSystem_IsWanted(playerid)
{
    return (playerid >= 0 && playerid < MAX_PLAYERS && gWantedLevel[playerid] > 0);
}

stock LawSystem_SetLawRole(playerid, role)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    gPlayerLawRole[playerid] = role;
    gPlayerData[playerid][E_PLAYER_LAW_ROLE] = role;
}

stock int:LawSystem_GetLawRole(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return LAW_ROLE_NONE;
    }
    return gPlayerLawRole[playerid];
}

stock LawSystem_ArrestPlayer(playerid, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    gArrestTime[playerid] = 120;
    gWantedLevel[playerid] = 0;
    if (stateid >= 0 && stateid <= 2)
    {
        gPolicePresence[stateid]++;
    }

    SendClientMessage(playerid, 0xFFCC66AA, "[LAW] Du wurdest festgenommen und inhaftiert.");
}

stock LawSystem_ShowWanted(playerid)
{
    LawSystem_HideWanted(playerid);

    gLawTD[playerid][0] = TextDrawCreate(320.0, 30.0, "WANTED STATUS");
    TextDrawAlignment(gLawTD[playerid][0], 2);
    TextDrawLetterSize(gLawTD[playerid][0], 0.52, 1.5);
    TextDrawColor(gLawTD[playerid][0], 0xFF0000FF);

    new wantedText[64];
    format(wantedText, sizeof(wantedText), "Level: %d | Haft: %d s", gWantedLevel[playerid], gArrestTime[playerid]);
    gLawTD[playerid][1] = TextDrawCreate(320.0, 62.0, wantedText);
    TextDrawAlignment(gLawTD[playerid][1], 2);
    TextDrawLetterSize(gLawTD[playerid][1], 0.24, 1.0);
    TextDrawColor(gLawTD[playerid][1], 0xFFFFFFFF);

    new roleText[64];
    format(roleText, sizeof(roleText), "Rolle: %d | Staat: %d", gPlayerLawRole[playerid], StateSystem_GetPlayerState(playerid));
    gLawTD[playerid][2] = TextDrawCreate(320.0, 86.0, roleText);
    TextDrawAlignment(gLawTD[playerid][2], 2);
    TextDrawLetterSize(gLawTD[playerid][2], 0.22, 0.9);
    TextDrawColor(gLawTD[playerid][2], 0x99CCFFFF);

    for (new i = 0; i < 3; i++)
    {
        if (gLawTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawShowForPlayer(playerid, gLawTD[playerid][i]);
        }
    }
}

stock LawSystem_HideWanted(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gLawTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gLawTD[playerid][i]);
            TextDrawDestroy(gLawTD[playerid][i]);
            gLawTD[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }
}

#endif
