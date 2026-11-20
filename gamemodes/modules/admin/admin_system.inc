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

stock AdminSystem_IsProjectLeader(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return Core_IsProjectLeader(playerid) || StateAdminSystem_IsGlobalOwner(playerid) || AdminSystem_IsOwner(playerid);
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
    if (cmdtext[0] != '/')
    {
        return false;
    }

    new cmd[32];
    new cmdPos = 0;
    while (cmdtext[cmdPos] != '\0' && cmdtext[cmdPos] != ' ' && cmdtext[cmdPos] != '\t' && cmdtext[cmdPos] != '\r' && cmdtext[cmdPos] != '\n')
    {
        if (cmdPos + 1 >= sizeof(cmd))
        {
            break;
        }
        cmd[cmdPos] = cmdtext[cmdPos];
        cmdPos++;
    }
    cmd[cmdPos] = '\0';

    if (strcmp(cmd, "/admin", true) == 0 || strcmp(cmd, "/mod", true) == 0)
    {
        if (AdminSystem_IsModerator(playerid) || AdminSystem_IsProjectLeader(playerid))
        {
            UiDialog_Show(playerid, DIALOG_UI_ADMIN_MENU, DIALOG_STYLE_LIST, "Admin-Menü", "Konfiguration\nGangwar\nStaatsstatus\nBerechtigungen\nState-Spawns\nFahrzeug-Spawnen\nHaus verwalten\nBusiness verwalten", "Öffnen", "Schließen");
            return true;
        }
        SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Kein Adminzugang.");
        return true;
    }

    if (strcmp(cmd, "/projectlead", true) == 0 || strcmp(cmd, "/projectleader", true) == 0)
    {
        if (AdminSystem_IsProjectLeader(playerid) || AdminSystem_IsOwner(playerid))
        {
            Core_SetPlayerProjectLeader(playerid, 1);
            SendClientMessage(playerid, 0x00FF00AA, "[PROJECT] Projektleiter-Zugang aktiviert.");
            return true;
        }
        SendClientMessage(playerid, 0xFF0000AA, "[PROJECT] Zugriff verweigert: Projektleiter-/Admin-Recht fehlt.");
        return true;
    }

    if (strcmp(cmd, "/state", true) == 0)
    {
        SendClientMessage(playerid, 0xFFFF00AA, "[STATE] Verfügbar: Los Santos, San Fierro, Las Venturas");
        return true;
    }

    if (strcmp(cmd, "/config", true) == 0)
    {
        if (AdminSystem_IsModerator(playerid) || AdminSystem_IsProjectLeader(playerid))
        {
            AdminConfigSystem_ShowMenu(playerid);
            return true;
        }
        SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Kein Zugangsrecht zur Konfiguration.");
        return true;
    }

    if (strcmp(cmd, "/togglewar", true) == 0)
    {
        if (AdminSystem_IsModerator(playerid) || AdminSystem_IsProjectLeader(playerid))
        {
            GangWarSystem_Toggle();
            SendClientMessage(playerid, 0x00FF00AA, "[GANGWAR] Status gewechselt.");
            return true;
        }
        return true;
    }

    if (strcmp(cmd, "/setstate", true) == 0)
    {
        if (!AdminSystem_IsProjectLeader(playerid) && !AdminSystem_IsOwner(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Keine Berechtigung für State-Setzeingriffe.");
            return true;
        }

        new params[32];
        new stateid;
        if (sscanf(cmdtext, "s[32]d", params, stateid) != 0)
        {
            SendClientMessage(playerid, 0xFFCC00AA, "[ADMIN] Nutzung: /setstate [0=LS|1=SF|2=LV]");
            return true;
        }

        if (!Core_IsValidState(stateid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Ungültiger State.");
            return true;
        }

        Core_SetPlayerState(playerid, stateid);
        Core_SetPlayerHomeState(playerid, stateid);
        StateSpawnSystem_Apply(playerid, stateid);
        SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] State gesetzt und gespawnt.");
        return true;
    }

    if (strcmp(cmd, "/spawnvehicle", true) == 0)
    {
        if (!AdminSystem_IsProjectLeader(playerid) && !AdminSystem_IsOwner(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Keine Berechtigung für Fahrzeug-Spawns.");
            return true;
        }

        new params[32];
        new modelid;
        if (sscanf(cmdtext, "s[32]d", params, modelid) != 0 || modelid <= 0)
        {
            SendClientMessage(playerid, 0xFFCC00AA, "[ADMIN] Nutzung: /spawnvehicle [modelid]");
            return true;
        }

        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        new vehicleid = VehicleSystem_SpawnAdminVehicle(modelid, x + 2.0, y + 2.0, z + 1.5, playerid);
        if (vehicleid == INVALID_VEHICLE_ID)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Fahrzeug konnte nicht gespawnt werden oder das Slot-Limit wurde erreicht.");
            return true;
        }

        PutPlayerInVehicle(playerid, vehicleid, 0);
        SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] Fahrzeug gespawnt.");
        return true;
    }

    return false;
}

#endif
