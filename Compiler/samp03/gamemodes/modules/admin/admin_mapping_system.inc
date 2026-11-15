#if !defined ADMIN_MAPPING_SYSTEM_INC
#define ADMIN_MAPPING_SYSTEM_INC

#include <a_samp>

#define MAX_MAP_LOCATIONS 220
#define MAX_MAP_ZONES 100

#define MAP_LOCATION_ACTIVE 1
#define MAP_LOCATION_INACTIVE 0

#define MAP_ZONE_ACTIVE 1
#define MAP_ZONE_DISABLED 0

enum E_MAP_LOCATION
{
    EML_NAME[48],
    EML_STATE,
    Float:EML_X,
    Float:EML_Y,
    Float:EML_Z,
    EML_INTERIOR,
    EML_VW,
    Float:EML_ANGLE,
    EML_ACTIVE
}

new gMapLocations[MAX_MAP_LOCATIONS][E_MAP_LOCATION];
new gMapLocationCount = 0;

enum E_MAP_ZONE
{
    EMZ_NAME[32],
    EMZ_STATE,
    Float:EMZ_MIN_X,
    Float:EMZ_MIN_Y,
    Float:EMZ_MAX_X,
    Float:EMZ_MAX_Y,
    EMZ_ACTIVE
}

new gMapZones[MAX_MAP_ZONES][E_MAP_ZONE];
new gMapZoneCount = 0;

new gAdminMapSelectedLocation[MAX_PLAYERS];
new gAdminMapMode[MAX_PLAYERS];
new Text:gAdminMapTD[MAX_PLAYERS][12];

stock AdminMappingSystem_Initialize()
{
    gMapLocationCount = 0;
    gMapZoneCount = 0;

    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gAdminMapSelectedLocation[i] = -1;
        gAdminMapMode[i] = 0;
    }

    AdminMappingSystem_RegisterLocation("LS_Spawn", STATE_LOS_SANTOS, 1500.0, -1710.0, 13.0, 0, 0, 0.0, MAP_LOCATION_ACTIVE);
    AdminMappingSystem_RegisterLocation("LS_PD", STATE_LOS_SANTOS, 1561.0, -1675.0, 16.0, 0, 0, 180.0, MAP_LOCATION_ACTIVE);
    AdminMappingSystem_RegisterLocation("LS_Hospital", STATE_LOS_SANTOS, 1187.0, -1323.0, 14.0, 0, 0, 90.0, MAP_LOCATION_ACTIVE);

    AdminMappingSystem_RegisterLocation("SF_Spawn", STATE_SAN_FIERRO, -1930.0, 120.0, 27.0, 0, 0, 90.0, MAP_LOCATION_ACTIVE);
    AdminMappingSystem_RegisterLocation("SF_PD", STATE_SAN_FIERRO, -1626.0, 690.0, 12.0, 0, 0, 90.0, MAP_LOCATION_ACTIVE);
    AdminMappingSystem_RegisterLocation("SF_Hospital", STATE_SAN_FIERRO, -2672.0, 410.0, 7.0, 0, 0, 90.0, MAP_LOCATION_ACTIVE);

    AdminMappingSystem_RegisterLocation("LV_Spawn", STATE_LAS_VENTURAS, 1700.0, 1210.0, 16.0, 0, 0, 180.0, MAP_LOCATION_ACTIVE);
    AdminMappingSystem_RegisterLocation("LV_PD", STATE_LAS_VENTURAS, 1590.0, 1000.0, 10.0, 0, 0, 270.0, MAP_LOCATION_ACTIVE);
    AdminMappingSystem_RegisterLocation("LV_Hospital", STATE_LAS_VENTURAS, 1421.0, 1853.0, 13.0, 0, 0, 270.0, MAP_LOCATION_ACTIVE);

    AdminMappingSystem_RegisterZone("LS_Gangwar", STATE_LOS_SANTOS, 1500.0, -1800.0, 1900.0, -1200.0, MAP_ZONE_ACTIVE);
    AdminMappingSystem_RegisterZone("SF_Gangwar", STATE_SAN_FIERRO, -2100.0, -200.0, -1400.0, 500.0, MAP_ZONE_ACTIVE);
    AdminMappingSystem_RegisterZone("LV_Gangwar", STATE_LAS_VENTURAS, 1400.0, 850.0, 2000.0, 1700.0, MAP_ZONE_ACTIVE);

    print("[ADMIN_MAP] Vollwertiges Mapping-/Teleport-System initialisiert");
}

stock AdminMappingSystem_RegisterLocation(const name[], stateid, Float:x, Float:y, Float:z, interior, vw, Float:angle, active)
{
    if (gMapLocationCount >= MAX_MAP_LOCATIONS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    format(gMapLocations[gMapLocationCount][EML_NAME], 48, "%s", name);
    gMapLocations[gMapLocationCount][EML_STATE] = stateid;
    gMapLocations[gMapLocationCount][EML_X] = x;
    gMapLocations[gMapLocationCount][EML_Y] = y;
    gMapLocations[gMapLocationCount][EML_Z] = z;
    gMapLocations[gMapLocationCount][EML_INTERIOR] = interior;
    gMapLocations[gMapLocationCount][EML_VW] = vw;
    gMapLocations[gMapLocationCount][EML_ANGLE] = angle;
    gMapLocations[gMapLocationCount][EML_ACTIVE] = active;

    DB_SaveMapLocation(name, stateid, x, y, z, interior, vw, angle, active);
    gMapLocationCount++;
    return true;
}

stock AdminMappingSystem_RegisterZone(const name[], stateid, Float:minX, Float:minY, Float:maxX, Float:maxY, active)
{
    if (gMapZoneCount >= MAX_MAP_ZONES)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    format(gMapZones[gMapZoneCount][EMZ_NAME], 32, "%s", name);
    gMapZones[gMapZoneCount][EMZ_STATE] = stateid;
    gMapZones[gMapZoneCount][EMZ_MIN_X] = minX;
    gMapZones[gMapZoneCount][EMZ_MIN_Y] = minY;
    gMapZones[gMapZoneCount][EMZ_MAX_X] = maxX;
    gMapZones[gMapZoneCount][EMZ_MAX_Y] = maxY;
    gMapZones[gMapZoneCount][EMZ_ACTIVE] = active;

    DB_SaveMapZone(name, stateid, minX, minY, maxX, maxY, active);
    gMapZoneCount++;
    return true;
}

stock AdminMappingSystem_CreateLocationForPlayer(playerid, const name[], stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    return AdminMappingSystem_RegisterLocation(name, stateid, x, y, z, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid), a, MAP_LOCATION_ACTIVE);
}

stock AdminMappingSystem_UpdateLocation(locationid, const name[], stateid)
{
    if (locationid < 0 || locationid >= gMapLocationCount)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    format(gMapLocations[locationid][EML_NAME], 48, "%s", name);
    gMapLocations[locationid][EML_STATE] = stateid;
    GetPlayerPos(0, gMapLocations[locationid][EML_X], gMapLocations[locationid][EML_Y], gMapLocations[locationid][EML_Z]);
    return true;
}

stock AdminMappingSystem_RemoveLocation(locationid)
{
    if (locationid < 0 || locationid >= gMapLocationCount)
    {
        return false;
    }

    for (new i = locationid; i < gMapLocationCount - 1; i++)
    {
        gMapLocations[i] = gMapLocations[i + 1];
    }
    gMapLocationCount--;
    return true;
}

stock AdminMappingSystem_TeleportPlayerToLocation(playerid, locationid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (locationid < 0 || locationid >= gMapLocationCount)
    {
        return false;
    }

    SetPlayerPos(playerid, gMapLocations[locationid][EML_X], gMapLocations[locationid][EML_Y], gMapLocations[locationid][EML_Z]);
    SetPlayerFacingAngle(playerid, gMapLocations[locationid][EML_ANGLE]);
    SetPlayerInterior(playerid, gMapLocations[locationid][EML_INTERIOR]);
    SetPlayerVirtualWorld(playerid, gMapLocations[locationid][EML_VW]);
    gAdminMapSelectedLocation[playerid] = locationid;
    return true;
}

stock AdminMappingSystem_ShowLocationList(playerid)
{
    SendClientMessage(playerid, 0x00D9FFFF, "[MAP] Verfügbare Orte:");
    for (new i = 0; i < gMapLocationCount && i < 20; i++)
    {
        new msg[128];
        format(msg, sizeof(msg), "ID %d | %s | State %d | X: %.2f Y: %.2f Z: %.2f", i, gMapLocations[i][EML_NAME], gMapLocations[i][EML_STATE], gMapLocations[i][EML_X], gMapLocations[i][EML_Y], gMapLocations[i][EML_Z]);
        SendClientMessage(playerid, 0xFFFFFFFF, msg);
    }
}

stock AdminMappingSystem_ShowZoneList(playerid)
{
    SendClientMessage(playerid, 0x00D9FFFF, "[MAP] Verfügbare Zonen:");
    for (new i = 0; i < gMapZoneCount && i < 20; i++)
    {
        new msg[128];
        format(msg, sizeof(msg), "ZONE %d | %s | Staat %d | Min: %.2f,%.2f | Max: %.2f,%.2f", i, gMapZones[i][EMZ_NAME], gMapZones[i][EMZ_STATE], gMapZones[i][EMZ_MIN_X], gMapZones[i][EMZ_MIN_Y], gMapZones[i][EMZ_MAX_X], gMapZones[i][EMZ_MAX_Y]);
        SendClientMessage(playerid, 0xFFFFFFFF, msg);
    }
}

stock AdminMappingSystem_ShowAdminMenu(playerid)
{
    AdminMappingSystem_HideAdminMenu(playerid);

    gAdminMapTD[playerid][0] = TextDrawCreate(320.0, 30.0, "MAPPING CONTROL");
    TextDrawAlignment(gAdminMapTD[playerid][0], 2);
    TextDrawLetterSize(gAdminMapTD[playerid][0], 0.52, 1.5);
    TextDrawColor(gAdminMapTD[playerid][0], 0x35A2FFAA);

    new header[128];
    format(header, sizeof(header), "Standorte: %d | Zonen: %d", gMapLocationCount, gMapZoneCount);
    gAdminMapTD[playerid][1] = TextDrawCreate(320.0, 58.0, header);
    TextDrawAlignment(gAdminMapTD[playerid][1], 2);
    TextDrawLetterSize(gAdminMapTD[playerid][1], 0.22, 1.0);
    TextDrawColor(gAdminMapTD[playerid][1], 0xFFFFFFFF);

    for (new i = 0; i < 6 && i < gMapLocationCount; i++)
    {
        new lineText[64];
        format(lineText, sizeof(lineText), "%d: %s", i, gMapLocations[i][EML_NAME]);
        gAdminMapTD[playerid][2 + i] = TextDrawCreate(320.0, float(82 + (i * 18)), lineText);
        TextDrawAlignment(gAdminMapTD[playerid][2 + i], 2);
        TextDrawLetterSize(gAdminMapTD[playerid][2 + i], 0.20, 0.8);
        TextDrawColor(gAdminMapTD[playerid][2 + i], 0xFFC266FF);
    }

    gAdminMapTD[playerid][8] = TextDrawCreate(320.0, 200.0, "/mapadd /maptp /mapdel /maplist");
    TextDrawAlignment(gAdminMapTD[playerid][8], 2);
    TextDrawLetterSize(gAdminMapTD[playerid][8], 0.20, 0.8);
    TextDrawColor(gAdminMapTD[playerid][8], 0x99FF99AA);

    for (new i = 0; i < 9; i++)
    {
        if (gAdminMapTD[playerid][i] != Text:0)
        {
            TextDrawShowForPlayer(playerid, gAdminMapTD[playerid][i]);
        }
    }
}

stock AdminMappingSystem_HideAdminMenu(playerid)
{
    for (new i = 0; i < 12; i++)
    {
        if (gAdminMapTD[playerid][i] != Text:0)
        {
            TextDrawHideForPlayer(playerid, gAdminMapTD[playerid][i]);
            TextDrawDestroy(gAdminMapTD[playerid][i]);
            gAdminMapTD[playerid][i] = Text:0;
        }
    }
}

stock AdminMappingSystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
    {
        return false;
    }

    if (!AdminSystem_IsModerator(playerid))
    {
        return false;
    }

    new cmd[32];
    new idx = sscanf(cmdtext, "s[32]", cmd);
    if (idx == 0)
    {
        return false;
    }

    if (strcmp(cmd, "/mapmenu", true) == 0)
    {
        AdminMappingSystem_ShowAdminMenu(playerid);
        return true;
    }

    if (strcmp(cmd, "/mapadd", true) == 0)
    {
        new params[64];
        new name[32];
        new stateid;
        if (sscanf(cmdtext, "s[32]s[32]d", params, name, stateid) != 0 || strlen(name) == 0)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[MAP] Syntax: /mapadd <Name> <StateID>");
            return true;
        }

        if (AdminMappingSystem_CreateLocationForPlayer(playerid, name, stateid))
        {
            SendClientMessage(playerid, 0x00FF00AA, "[MAP] Standort gespeichert.");
        }
        else
        {
            SendClientMessage(playerid, 0xFF0000AA, "[MAP] Fehler beim Speichern des Standorts.");
        }
        return true;
    }

    if (strcmp(cmd, "/maptp", true) == 0)
    {
        new params[64];
        new locid;
        if (sscanf(cmdtext, "s[32]d", params, locid) != 0)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[MAP] Syntax: /maptp <ID>");
            return true;
        }

        if (AdminMappingSystem_TeleportPlayerToLocation(playerid, locid))
        {
            new locationName[48];
            format(locationName, sizeof(locationName), "%s", gMapLocations[locid][EML_NAME]);
            printf("[MAP] Spieler %d teleportiert nach %s", playerid, locationName);
            SendClientMessage(playerid, 0x00FF00AA, "[MAP] Teleportiert.");
        }
        else
        {
            SendClientMessage(playerid, 0xFF0000AA, "[MAP] Ungültige Location ID.");
        }
        return true;
    }

    if (strcmp(cmd, "/maplist", true) == 0)
    {
        AdminMappingSystem_ShowLocationList(playerid);
        return true;
    }

    if (strcmp(cmd, "/mapzones", true) == 0)
    {
        AdminMappingSystem_ShowZoneList(playerid);
        return true;
    }

    if (strcmp(cmd, "/mapdel", true) == 0)
    {
        new params[64];
        new locid;
        if (sscanf(cmdtext, "s[32]d", params, locid) != 0)
        {
            SendClientMessage(playerid, 0xFF0000AA, "[MAP] Syntax: /mapdel <ID>");
            return true;
        }

        if (AdminMappingSystem_RemoveLocation(locid))
        {
            SendClientMessage(playerid, 0x00FF00AA, "[MAP] Location gelöscht.");
        }
        else
        {
            SendClientMessage(playerid, 0xFF0000AA, "[MAP] Location konnte nicht gelöscht werden.");
        }
        return true;
    }

    return false;
}

#endif
