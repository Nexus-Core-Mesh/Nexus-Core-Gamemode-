#if !defined STATE_ADMIN_SYSTEM_INC
#define STATE_ADMIN_SYSTEM_INC

#include <a_samp>

#define STATE_ADMIN_COUNT 3
#define ADMIN_ROLE_NONE 0
#define ADMIN_ROLE_SUPPORT 1
#define ADMIN_ROLE_MODERATOR 2
#define ADMIN_ROLE_STATE_ADMIN 3
#define ADMIN_ROLE_STATE_LEADER 4
#define ADMIN_ROLE_SERVER_OWNER 5

stock StateAdminSystem_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gAdminLevel[i] = ADMIN_ROLE_NONE;
        gAdminState[i] = -1;
        Core_SetPlayerAdminLevel(i, ADMIN_ROLE_NONE);
        Core_SetPlayerAdminState(i, -1);
    }

    print("[ADMIN_STATE] 3x/1Admin-System initialisiert");
}

stock StateAdminSystem_GetRoleName(level, output[], len)
{
    if (len <= 0)
    {
        return;
    }
    output[0] = '\0';

    if (level <= ADMIN_ROLE_NONE)
    {
        format(output, len, "Kein Rang");
        return;
    }
    if (level == ADMIN_ROLE_SUPPORT)
    {
        format(output, len, "Support");
        return;
    }
    if (level == ADMIN_ROLE_MODERATOR)
    {
        format(output, len, "Moderator");
        return;
    }
    if (level == ADMIN_ROLE_STATE_ADMIN)
    {
        format(output, len, "State Admin");
        return;
    }
    if (level == ADMIN_ROLE_STATE_LEADER)
    {
        format(output, len, "State Leader");
        return;
    }
    if (level == ADMIN_ROLE_SERVER_OWNER)
    {
        format(output, len, "Server Owner");
        return;
    }
    format(output, len, "Unknown");
}

stock StateAdminSystem_AssignGlobal(playerid, level)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    if (level < ADMIN_ROLE_NONE || level > ADMIN_ROLE_SERVER_OWNER)
    {
        level = ADMIN_ROLE_NONE;
    }

    gAdminLevel[playerid] = level;
    gAdminState[playerid] = -1;
    Core_SetPlayerAdminLevel(playerid, level);
    Core_SetPlayerAdminState(playerid, -1);
    gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL] = level;
    gPlayerData[playerid][E_PLAYER_ADMIN_STATE] = -1;
}

stock StateAdminSystem_AssignState(playerid, stateid, level)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    if (!Core_IsValidState(stateid))
    {
        return;
    }
    if (level < ADMIN_ROLE_NONE || level > ADMIN_ROLE_STATE_LEADER)
    {
        level = ADMIN_ROLE_NONE;
    }

    gAdminLevel[playerid] = level;
    gAdminState[playerid] = stateid;
    Core_SetPlayerAdminLevel(playerid, level);
    Core_SetPlayerAdminState(playerid, stateid);
    gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL] = level;
    gPlayerData[playerid][E_PLAYER_ADMIN_STATE] = stateid;
}

stock StateAdminSystem_HasAccess(playerid, requiredLevel, stateid = -1)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (requiredLevel <= ADMIN_ROLE_NONE)
    {
        return true;
    }

    if (gAdminLevel[playerid] >= requiredLevel || Core_HasAdminLevel(playerid, requiredLevel))
    {
        return true;
    }

    if (stateid != -1 && gAdminState[playerid] == stateid && gAdminLevel[playerid] >= requiredLevel)
    {
        return true;
    }

    return (gAdminLevel[playerid] >= requiredLevel);
}

stock StateAdminSystem_IsGlobalOwner(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return (gAdminLevel[playerid] >= ADMIN_ROLE_SERVER_OWNER) || Core_HasAdminLevel(playerid, ADMIN_ROLE_SERVER_OWNER);
}

stock StateAdminSystem_IsStateLeader(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return (gAdminLevel[playerid] >= ADMIN_ROLE_STATE_LEADER) || Core_HasAdminLevel(playerid, ADMIN_ROLE_STATE_LEADER);
}

stock StateAdminSystem_IsStateAdmin(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return (gAdminLevel[playerid] >= ADMIN_ROLE_STATE_ADMIN) || Core_HasAdminLevel(playerid, ADMIN_ROLE_STATE_ADMIN);
}

stock StateAdminSystem_IsModerator(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return (gAdminLevel[playerid] >= ADMIN_ROLE_MODERATOR) || Core_HasAdminLevel(playerid, ADMIN_ROLE_MODERATOR);
}

stock StateAdminSystem_ShowSummary(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    new roleName[32];
    new stateName[32];
    new level = gAdminLevel[playerid];
    StateAdminSystem_GetRoleName(level, roleName, sizeof(roleName));

    if (gAdminState[playerid] != -1)
    {
        StateSystem_GetStateName(gAdminState[playerid], stateName, sizeof(stateName));
        format(stateName, sizeof(stateName), " %s", stateName);
    }
    else
    {
        format(stateName, sizeof(stateName), " Global");
    }

    printf("[ADMIN_STATE] %d -> %s%s", playerid, roleName, stateName);
    SendClientMessage(playerid, 0x00D9FFFF, "[ADMIN_STATE] Rollenstatus aktualisiert.");
}

stock StateAdminSystem_Clear(playerid)
{
    gAdminLevel[playerid] = ADMIN_ROLE_NONE;
    gAdminState[playerid] = -1;
    Core_SetPlayerAdminLevel(playerid, ADMIN_ROLE_NONE);
    Core_SetPlayerAdminState(playerid, -1);
    gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL] = 0;
    gPlayerData[playerid][E_PLAYER_ADMIN_STATE] = -1;
}

#endif
