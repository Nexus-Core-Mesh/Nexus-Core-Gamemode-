#if !defined ADMIN_SYSTEM_INC
#define ADMIN_SYSTEM_INC

#include <a_samp>

#define ADMIN_RANK_SUPPORT 1
#define ADMIN_RANK_MODERATOR 2
#define ADMIN_RANK_STATE_ADMIN 3
#define ADMIN_RANK_STATE_LEADER 4
#define ADMIN_RANK_OWNER 5

new gAdminLevel[MAX_PLAYERS];
new gAdminState[MAX_PLAYERS];

stock AdminSystem_Initialize()
{
    print("[ADMIN] System initialisiert");
    print("[ADMIN] 3x/1Admin System aktiviert");
}

stock AdminSystem_Shutdown()
{
    print("[ADMIN] System heruntergefahren");
}

stock AdminSystem_OnPlayerConnect(playerid)
{
    gAdminLevel[playerid] = 0;
    gAdminState[playerid] = -1;
    Core_SetPlayerAdminLevel(playerid, 0);
    Core_SetPlayerAdminState(playerid, -1);
    Core_NormalizePlayerState(playerid);
    gPlayerData[playerid][E_PLAYER_STATE] = StateSystem_GetPlayerState(playerid);
}

stock AdminSystem_OnPlayerDisconnect(playerid, reason)
{
    gAdminLevel[playerid] = 0;
    gAdminState[playerid] = -1;
    Core_SetPlayerAdminLevel(playerid, 0);
    Core_SetPlayerAdminState(playerid, -1);
}

stock AdminSystem_IsOwner(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return (gAdminLevel[playerid] >= ADMIN_RANK_OWNER) || Core_HasAdminLevel(playerid, ADMIN_RANK_OWNER) || StateAdminSystem_IsGlobalOwner(playerid);
}

stock AdminSystem_IsStateLeader(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return (gAdminLevel[playerid] >= ADMIN_RANK_STATE_LEADER) || Core_HasAdminLevel(playerid, ADMIN_RANK_STATE_LEADER) || StateAdminSystem_IsStateLeader(playerid);
}

stock AdminSystem_IsStateAdmin(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return (gAdminLevel[playerid] >= ADMIN_RANK_STATE_ADMIN) || Core_HasAdminLevel(playerid, ADMIN_RANK_STATE_ADMIN) || StateAdminSystem_IsStateAdmin(playerid);
}

stock AdminSystem_IsModerator(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return (gAdminLevel[playerid] >= ADMIN_RANK_MODERATOR) || Core_HasAdminLevel(playerid, ADMIN_RANK_MODERATOR) || StateAdminSystem_IsModerator(playerid);
}

stock AdminSystem_SetRank(playerid, level)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (level < 0 || level > ADMIN_RANK_OWNER)
    {
        return false;
    }

    gAdminLevel[playerid] = level;
    Core_SetPlayerAdminLevel(playerid, level);
    return true;
}

stock AdminSystem_SetStateRank(playerid, stateid, level)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (level < 0 || level > ADMIN_RANK_STATE_LEADER)
    {
        return false;
    }

    gAdminState[playerid] = stateid;
    Core_SetPlayerAdminState(playerid, stateid);
    gAdminLevel[playerid] = level;
    Core_SetPlayerAdminLevel(playerid, level);
    return true;
}

stock AdminSystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] == '/')
    {
        new cmd[32], idx;
        idx = sscanf(cmdtext, "s[32]", cmd);
        if (idx == 0)
        {
            return false;
        }

        if (strcmp(cmd, "admin", true) == 0)
        {
            if (AdminSystem_IsModerator(playerid))
            {
                SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] Du bist im Adminsystem aktiv.");
                return true;
            }
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Kein Adminzugang.");
            return true;
        }

        if (strcmp(cmd, "state", true) == 0)
        {
            SendClientMessage(playerid, 0xFFFF00AA, "[STATE] Verfügbar: Los Santos, San Fierro, Las Venturas");
            return true;
        }

        if (strcmp(cmd, "config", true) == 0)
        {
            if (AdminSystem_IsModerator(playerid))
            {
                AdminConfigSystem_ShowMenu(playerid);
                return true;
            }
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Kein Zugangsrecht zur Konfiguration.");
            return true;
        }

        if (strcmp(cmd, "togglewar", true) == 0)
        {
            if (AdminSystem_IsModerator(playerid))
            {
                GangWarSystem_Toggle();
                SendClientMessage(playerid, 0x00FF00AA, "[GANGWAR] Status gewechselt.");
                return true;
            }
            return true;
        }
    }
    return false;
}

#endif
