#if !defined JAIL_SYSTEM_INC
#define JAIL_SYSTEM_INC

#include <a_samp>

#define MAX_JAIL_LOCATIONS 8
#define JAIL_STATE_NONE 0
#define JAIL_STATE_ACTIVE 1
#define JAIL_STATE_RELEASED 2

new gJailActive[MAX_PLAYERS];
new gJailState[MAX_PLAYERS];
new gJailTime[MAX_PLAYERS];
new gJailCell[MAX_PLAYERS];
new gJailLocationCount = 0;
new Float:gJailLocationX[MAX_JAIL_LOCATIONS];
new Float:gJailLocationY[MAX_JAIL_LOCATIONS];
new Float:gJailLocationZ[MAX_JAIL_LOCATIONS];
new gJailLocationState[MAX_JAIL_LOCATIONS];
new gJailLocationName[MAX_JAIL_LOCATIONS][32];
new Text:gJailTD[MAX_PLAYERS][8];

stock JailSystem_Initialize()
{
    gJailLocationCount = 0;

    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gJailActive[i] = 0;
        gJailState[i] = JAIL_STATE_RELEASED;
        gJailTime[i] = 0;
        gJailCell[i] = -1;
    }

    JailSystem_RegisterLocation(STATE_LOS_SANTOS, "LS Jail", 1662.0, -1710.0, 13.0);
    JailSystem_RegisterLocation(STATE_SAN_FIERRO, "SF Jail", -1870.0, 260.0, 27.0);
    JailSystem_RegisterLocation(STATE_LAS_VENTURAS, "LV Jail", 1780.0, 1180.0, 16.0);

    print("[JAIL] Gefängnis- und Haftsystem initialisiert");
}

stock JailSystem_RegisterLocation(stateid, const name[], Float:x, Float:y, Float:z)
{
    if (gJailLocationCount >= MAX_JAIL_LOCATIONS)
        return false;
    if (!Core_IsValidState(stateid))
        return false;

    gJailLocationState[gJailLocationCount] = stateid;
    format(gJailLocationName[gJailLocationCount], 32, "%s", name);
    gJailLocationX[gJailLocationCount] = x;
    gJailLocationY[gJailLocationCount] = y;
    gJailLocationZ[gJailLocationCount] = z;
    gJailLocationCount++;
    return true;
}

stock JailSystem_OnPlayerConnect(playerid)
{
    gJailActive[playerid] = 0;
    gJailState[playerid] = JAIL_STATE_RELEASED;
    gJailTime[playerid] = 0;
    gJailCell[playerid] = -1;
}

stock JailSystem_SendToJail(playerid, stateid, timeMinutes)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
        return;
    if (!Core_IsValidState(stateid))
        return;

    gJailActive[playerid] = 1;
    gJailState[playerid] = JAIL_STATE_ACTIVE;
    gJailTime[playerid] = timeMinutes;

    new jailCell = stateid;
    if (jailCell >= gJailLocationCount)
        jailCell = 0;

    gJailCell[playerid] = jailCell;
    SetPlayerPos(playerid, gJailLocationX[jailCell], gJailLocationY[jailCell], gJailLocationZ[jailCell]);
    SetPlayerHealth(playerid, 100.0);
    TogglePlayerControllable(playerid, false);

    new msg[128];
    format(msg, sizeof(msg), "[JAIL] Du wurdest in %s eingesperrt für %d Minuten.", gJailLocationName[jailCell], timeMinutes);
    SendClientMessage(playerid, 0xFF0000AA, msg);
}

stock JailSystem_ReleasePlayer(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
        return;

    gJailActive[playerid] = 0;
    gJailState[playerid] = JAIL_STATE_RELEASED;
    gJailTime[playerid] = 0;
    gJailCell[playerid] = -1;
    TogglePlayerControllable(playerid, true);
    SendClientMessage(playerid, 0x00FF00AA, "[JAIL] Du bist aus dem Gefängnis entlassen worden.");
}

stock JailSystem_IsInJail(playerid)
{
    return (playerid >= 0 && playerid < MAX_PLAYERS && gJailActive[playerid] == 1);
}

stock JailSystem_Tick()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (gJailActive[i] == 1)
        {
            gJailTime[i]--;
            if (gJailTime[i] <= 0)
            {
                JailSystem_ReleasePlayer(i);
            }
        }
    }
}

stock JailSystem_ShowHUD(playerid)
{
    JailSystem_HideHUD(playerid);

    gJailTD[playerid][0] = TextDrawCreate(320.0, 30.0, "JAIL HUD");
    TextDrawAlignment(gJailTD[playerid][0], 2);
    TextDrawLetterSize(gJailTD[playerid][0], 0.5, 1.5);
    TextDrawColor(gJailTD[playerid][0], 0xFF0000AA);

    new bootText[64];
    format(bootText, sizeof(bootText), "Zeit: %d min | Zelle: %d", gJailTime[playerid], gJailCell[playerid]);
    gJailTD[playerid][1] = TextDrawCreate(320.0, 62.0, bootText);
    TextDrawAlignment(gJailTD[playerid][1], 2);
    TextDrawLetterSize(gJailTD[playerid][1], 0.22, 1.0);
    TextDrawColor(gJailTD[playerid][1], 0xFFFFFFFF);

    for (new i = 0; i < 2; i++)
    {
        if (gJailTD[playerid][i] != Text:0)
            TextDrawShowForPlayer(playerid, gJailTD[playerid][i]);
    }
}

stock JailSystem_HideHUD(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gJailTD[playerid][i] != Text:0)
        {
            TextDrawHideForPlayer(playerid, gJailTD[playerid][i]);
            TextDrawDestroy(gJailTD[playerid][i]);
            gJailTD[playerid][i] = Text:0;
        }
    }
}

stock JailSystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
        return false;

    new cmd[32];
    new idx = sscanf(cmdtext, "s[32]", cmd);
    if (idx == 0)
        return false;

    if (strcmp(cmd, "/jail", true) == 0)
    {
        if (!AdminSystem_IsModerator(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[JAIL] Kein Zugriff.");
            return true;
        }

        new target, minutes, stateid;
        if (sscanf(cmdtext, "s[32]ddd", cmd, target, minutes, stateid) != 0)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[JAIL] Syntax: /jail <PlayerID> <Minuten> <StateID>");
            return true;
        }

        JailSystem_SendToJail(target, stateid, minutes);
        return true;
    }

    if (strcmp(cmd, "/release", true) == 0)
    {
        if (!AdminSystem_IsModerator(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[JAIL] Kein Zugriff.");
            return true;
        }

        new target;
        if (sscanf(cmdtext, "s[32]d", cmd, target) != 0)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[JAIL] Syntax: /release <PlayerID>");
            return true;
        }

        JailSystem_ReleasePlayer(target);
        return true;
    }

    return false;
}

#endif
