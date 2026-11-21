#if !defined ADMIN_SYSTEM_INC
#define ADMIN_SYSTEM_INC

#include <a_samp>

#define ADMIN_RANK_SUPPORT 1
#define ADMIN_RANK_MODERATOR 2
#define ADMIN_RANK_STATE_ADMIN 3
#define ADMIN_RANK_STATE_LEADER 4
#define ADMIN_RANK_OWNER 5
#define ADMIN_DEALERSHIP_TYPE_STANDARD 1

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

stock AdminSystem_IsHiddenProjectLeader(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }
    return Core_IsProjectLeader(playerid) || Core_HasAdminLevel(playerid, ADMIN_RANK_OWNER) || StateAdminSystem_IsGlobalOwner(playerid);
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

stock bool:AdminSystem_CanManageWorld(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (IsPlayerAdmin(playerid))
    {
        return true;
    }
    return AdminSystem_IsProjectLeader(playerid) || AdminSystem_IsOwner(playerid) || AdminSystem_IsStateLeader(playerid);
}

stock AdminSystem_ResetCameraState(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    CameraSystem_Reset(playerid);
    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, 1);
}

stock bool:AdminSystem_ActivateProjectLeader(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    if (!AdminSystem_IsHiddenProjectLeader(playerid) && !IsPlayerAdmin(playerid) && !AdminSystem_CanManageWorld(playerid))
    {
        return false;
    }

    Core_SetPlayerProjectLeader(playerid, 1);
    Core_SetPlayerAdminLevel(playerid, ADMIN_RANK_OWNER);
    AdminSystem_ResetCameraState(playerid);
    return true;
}

stock AdminSystem_ExtractCommandArgument(const cmdtext[], startIndex, arg[], len)
{
    new idx = startIndex;
    while (cmdtext[idx] != '\0' && cmdtext[idx] != ' ' && cmdtext[idx] != '\t' && cmdtext[idx] != '\r' && cmdtext[idx] != '\n')
    {
        idx++;
    }
    if (cmdtext[startIndex] == '\0')
    {
        arg[0] = '\0';
        return 0;
    }

    new length = idx - startIndex;
    if (length < len)
    {
        strmid(arg, cmdtext, startIndex, idx, len);
        return length;
    }

    strmid(arg, cmdtext, startIndex, idx, len);
    return len;
}

stock bool:AdminSystem_ApplyStateTeleport(playerid, targetid, stateid, bool:restoreCamera)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidPlayerId(targetid) || !Core_IsValidState(stateid))
    {
        return false;
    }

    if (targetid != playerid && !AdminSystem_CanManageWorld(playerid))
    {
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(stateid, Core_GetPlayerState(targetid));
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    Core_SetPlayerState(targetid, safeState);
    Core_SetPlayerHomeState(targetid, safeState);
    DB_ValidateStateTransfer(targetid, safeState, safeState);
    DB_SavePlayerState(targetid, safeState);
    StateSpawnSystem_Apply(targetid, safeState);

    if (restoreCamera)
    {
        CameraSystem_ExitCameraMode(targetid);
        SetCameraBehindPlayer(targetid);
        TogglePlayerControllable(targetid, 1);
    }

    return true;
}

stock AdminSystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
    {
        return false;
    }

    if (!Core_IsValidPlayerId(playerid))
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
        if (AdminSystem_ActivateProjectLeader(playerid))
        {
            AdminSystem_ResetCameraState(playerid);
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

    if (strcmp(cmd, "/tpstate", true) == 0 || strcmp(cmd, "/setstateplayer", true) == 0)
    {
        if (!AdminSystem_CanManageWorld(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Keine Berechtigung für State-Teleports.");
            return true;
        }

        new targetArg[16], stateArg[16];
        new pos = 0;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        while (cmdtext[pos] == ' ') pos++;
        if (cmdtext[pos] == '\0')
        {
            SendClientMessage(playerid, 0xFFCC00AA, "[ADMIN] Nutzung: /tpstate [targetid] [stateid]");
            return true;
        }

        new targetStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(targetArg, cmdtext, targetStart, pos, sizeof(targetArg));
        new targetid = strval(targetArg);
        while (cmdtext[pos] == ' ') pos++;
        if (cmdtext[pos] == '\0')
        {
            SendClientMessage(playerid, 0xFFCC00AA, "[ADMIN] Nutzung: /tpstate [targetid] [stateid]");
            return true;
        }
        new stateStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(stateArg, cmdtext, stateStart, pos, sizeof(stateArg));
        new stateid = strval(stateArg);

        if (!Core_IsValidPlayerId(targetid) || !Core_IsValidState(stateid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Zielspieler oder State ungültig.");
            return true;
        }

        if (!AdminSystem_ApplyStateTeleport(playerid, targetid, stateid, true))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] State-Transfer für den Zielspieler konnte nicht angewendet werden.");
            return true;
        }

        SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] State des Spielers gesetzt und gespawnt.");
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
        if (!AdminSystem_CanManageWorld(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Keine Berechtigung für State-Setzeingriffe.");
            return true;
        }

        new stateArg[16];
        new stateid = -1;
        new argPos = 0;
        while (cmdtext[argPos] != '\0' && cmdtext[argPos] != ' ' && cmdtext[argPos] != '\t' && cmdtext[argPos] != '\r' && cmdtext[argPos] != '\n')
        {
            argPos++;
        }
        while (cmdtext[argPos] == ' ' || cmdtext[argPos] == '\t' || cmdtext[argPos] == '\r' || cmdtext[argPos] == '\n')
        {
            argPos++;
        }
        if (cmdtext[argPos] == '\0')
        {
            SendClientMessage(playerid, 0xFFCC00AA, "[ADMIN] Nutzung: /setstate [0=LS|1=SF|2=LV]");
            return true;
        }
        strmid(stateArg, cmdtext, argPos, strlen(cmdtext), sizeof(stateArg));
        stateid = strval(stateArg);

        if (!Core_IsValidState(stateid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Ungültiger State.");
            return true;
        }

        Core_SetPlayerState(playerid, stateid);
        Core_SetPlayerHomeState(playerid, stateid);
        DB_ValidateStateTransfer(playerid, stateid, stateid);
        StateSpawnSystem_Apply(playerid, stateid);
        CameraSystem_ExitCameraMode(playerid);
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, 1);
        SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] State gesetzt und gespawnt.");
        return true;
    }

    if (strcmp(cmd, "/setstatecoord", true) == 0)
    {
        if (!AdminSystem_CanManageWorld(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Keine Berechtigung für State-Koordinaten.");
            return true;
        }

        new stateArg[16], xArg[16], yArg[16], zArg[16], angleArg[16];
        new pos = 0;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        while (cmdtext[pos] == ' ') pos++;
        new stateStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(stateArg, cmdtext, stateStart, pos, sizeof(stateArg));
        new stateid = strval(stateArg);
        while (cmdtext[pos] == ' ') pos++;
        new xStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(xArg, cmdtext, xStart, pos, sizeof(xArg));
        new Float:x = floatstr(xArg);
        while (cmdtext[pos] == ' ') pos++;
        new yStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(yArg, cmdtext, yStart, pos, sizeof(yArg));
        new Float:y = floatstr(yArg);
        while (cmdtext[pos] == ' ') pos++;
        new zStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(zArg, cmdtext, zStart, pos, sizeof(zArg));
        new Float:z = floatstr(zArg);
        while (cmdtext[pos] == ' ') pos++;
        new angleStart = pos;
        if (cmdtext[pos] != '\0')
        {
            while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
            strmid(angleArg, cmdtext, angleStart, pos, sizeof(angleArg));
        }
        else
        {
            angleArg = "0";
        }
        new Float:angle = floatstr(angleArg);

        if (!Core_IsValidState(stateid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Ungültiger State bei State-Koordinaten.");
            return true;
        }

        StateSpawnSystem_SetStateSpawn(stateid, x, y, z, angle);
        CameraSystem_ExitCameraMode(playerid);
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, 1);
        SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] State-Spawnkoordinaten übernommen.");
        return true;
    }

    if (strcmp(cmd, "/spawnvehicle", true) == 0)
    {
        if (!AdminSystem_CanManageWorld(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Keine Berechtigung für Fahrzeug-Spawns.");
            return true;
        }

        new modelArg[16];
        new pos = 0;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        while (cmdtext[pos] == ' ') pos++;
        if (cmdtext[pos] == '\0')
        {
            SendClientMessage(playerid, 0xFFCC00AA, "[ADMIN] Nutzung: /spawnvehicle [modelid]");
            return true;
        }
        strmid(modelArg, cmdtext, pos, strlen(cmdtext), sizeof(modelArg));
        new modelid = strval(modelArg);
        if (modelid <= 0)
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
        CameraSystem_ExitCameraMode(playerid);
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, 1);
        SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] Fahrzeug gespawnt.");
        return true;
    }

    if (strcmp(cmd, "/setcityhall", true) == 0)
    {
        if (!AdminSystem_CanManageWorld(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Keine Berechtigung für Stadthallen.");
            return true;
        }

        new stateArg[16], nameArg[32];
        new pos = 0;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        while (cmdtext[pos] == ' ') pos++;
        if (cmdtext[pos] == '\0')
        {
            SendClientMessage(playerid, 0xFFCC00AA, "[ADMIN] Nutzung: /setcityhall [stateid] [name]");
            return true;
        }
        new stateStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(stateArg, cmdtext, stateStart, pos, sizeof(stateArg));
        new stateid = strval(stateArg);
        while (cmdtext[pos] == ' ') pos++;
        if (cmdtext[pos] == '\0')
        {
            format(nameArg, sizeof(nameArg), "State City Hall");
        }
        else
        {
            strmid(nameArg, cmdtext, pos, strlen(cmdtext), sizeof(nameArg));
        }
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        if (!CityHallSystem_RegisterCityHall(stateid, nameArg, x, y, z))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Stadthalle konnte nicht gespeichert werden.");
            return true;
        }
        CameraSystem_ExitCameraMode(playerid);
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, 1);
        SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] Stadthalle gesetzt.");
        return true;
    }

    if (strcmp(cmd, "/setdealership", true) == 0)
    {
        if (!AdminSystem_CanManageWorld(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Keine Berechtigung für Autohaus-Editoren.");
            return true;
        }

        new stateArg[16], priceArg[16], typeArg[16];
        new pos = 0;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        while (cmdtext[pos] == ' ') pos++;
        if (cmdtext[pos] == '\0')
        {
            SendClientMessage(playerid, 0xFFCC00AA, "[ADMIN] Nutzung: /setdealership [stateid] [modelid] [price]");
            return true;
        }
        new stateStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(stateArg, cmdtext, stateStart, pos, sizeof(stateArg));
        new stateid = strval(stateArg);
        while (cmdtext[pos] == ' ') pos++;
        new modelStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(priceArg, cmdtext, modelStart, pos, sizeof(priceArg));
        new modelid = strval(priceArg);
        while (cmdtext[pos] == ' ') pos++;
        strmid(typeArg, cmdtext, pos, strlen(cmdtext), sizeof(typeArg));
        new price = strval(typeArg);
        if (!Core_IsValidState(stateid) || modelid <= 0 || price <= 0)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Autohausparameter ungültig.");
            return true;
        }
        DealershipSystem_CreateListingAtPlayer(playerid, stateid, "Admin Dealership", modelid, price, ADMIN_DEALERSHIP_TYPE_STANDARD);
        CameraSystem_ExitCameraMode(playerid);
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, 1);
        SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] Autohaus-Stammdaten übernommen.");
        return true;
    }

    if (strcmp(cmd, "/sethouse", true) == 0)
    {
        if (!AdminSystem_CanManageWorld(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Keine Berechtigung für Haus-Editoren.");
            return true;
        }
        new stateArg[16], priceArg[16], interiorArg[16];
        new pos = 0;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        while (cmdtext[pos] == ' ') pos++;
        if (cmdtext[pos] == '\0')
        {
            SendClientMessage(playerid, 0xFFCC00AA, "[ADMIN] Nutzung: /sethouse [stateid] [preis] [interior]");
            return true;
        }
        new stateStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(stateArg, cmdtext, stateStart, pos, sizeof(stateArg));
        new stateid = strval(stateArg);
        while (cmdtext[pos] == ' ') pos++;
        new priceStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(priceArg, cmdtext, priceStart, pos, sizeof(priceArg));
        new price = strval(priceArg);
        while (cmdtext[pos] == ' ') pos++;
        strmid(interiorArg, cmdtext, pos, strlen(cmdtext), sizeof(interiorArg));
        new interior = strval(interiorArg);
        if (!Core_IsValidState(stateid) || price <= 0)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Hausparameter ungültig.");
            return true;
        }
        HouseSystem_CreateHouseAtPlayer(playerid, stateid, price, interior);
        CameraSystem_ExitCameraMode(playerid);
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, 1);
        SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] Haus gesetzt.");
        return true;
    }

    if (strcmp(cmd, "/setbusiness", true) == 0)
    {
        if (!AdminSystem_CanManageWorld(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Keine Berechtigung für Business-Editoren.");
            return true;
        }
        new stateArg[16], valueArg[16], typeArg[16];
        new pos = 0;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        while (cmdtext[pos] == ' ') pos++;
        if (cmdtext[pos] == '\0')
        {
            SendClientMessage(playerid, 0xFFCC00AA, "[ADMIN] Nutzung: /setbusiness [stateid] [value] [type]");
            return true;
        }
        new stateStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(stateArg, cmdtext, stateStart, pos, sizeof(stateArg));
        new stateid = strval(stateArg);
        while (cmdtext[pos] == ' ') pos++;
        new valueStart = pos;
        while (cmdtext[pos] != '\0' && cmdtext[pos] != ' ') pos++;
        strmid(valueArg, cmdtext, valueStart, pos, sizeof(valueArg));
        new value = strval(valueArg);
        while (cmdtext[pos] == ' ') pos++;
        strmid(typeArg, cmdtext, pos, strlen(cmdtext), sizeof(typeArg));
        new type = strval(typeArg);
        if (!Core_IsValidState(stateid) || value <= 0)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[ADMIN] Businessparameter ungültig.");
            return true;
        }
        BusinessSystem_CreateBusinessAtPlayer(playerid, stateid, type, value, "Admin Business");
        CameraSystem_ExitCameraMode(playerid);
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, 1);
        SendClientMessage(playerid, 0x00FF00AA, "[ADMIN] Business gesetzt.");
        return true;
    }

    return false;
}

#endif
