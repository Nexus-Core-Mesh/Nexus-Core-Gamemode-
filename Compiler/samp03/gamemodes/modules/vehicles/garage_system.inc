#if !defined GARAGE_SYSTEM_INC
#define GARAGE_SYSTEM_INC

#include <a_samp>

#define MAX_GARAGES 12
#define MAX_GARAGE_VEHICLES 8

new gGarageCount = 0;
new gGarageState[MAX_GARAGES];
new gGarageVehicleCount[MAX_GARAGES];
new Float:gGarageX[MAX_GARAGES];
new Float:gGarageY[MAX_GARAGES];
new Float:gGarageZ[MAX_GARAGES];
new gGarageName[MAX_GARAGES][32];
new gGarageVehicleModel[MAX_GARAGES][MAX_GARAGE_VEHICLES];
new gGarageVehiclePrice[MAX_GARAGES][MAX_GARAGE_VEHICLES];

stock GarageSystem_Initialize()
{
    gGarageCount = 0;
    for (new i = 0; i < MAX_GARAGES; i++)
    {
        gGarageState[i] = -1;
        gGarageVehicleCount[i] = 0;
        gGarageX[i] = 0.0;
        gGarageY[i] = 0.0;
        gGarageZ[i] = 0.0;
        gGarageName[i][0] = 0;
        for (new j = 0; j < MAX_GARAGE_VEHICLES; j++)
        {
            gGarageVehicleModel[i][j] = 0;
            gGarageVehiclePrice[i][j] = 0;
        }
    }

    GarageSystem_RegisterGarage(STATE_LOS_SANTOS, "LS City Garage", 1520.0, -1675.0, 13.0);
    GarageSystem_RegisterGarage(STATE_SAN_FIERRO, "SF Depot", -1875.0, 155.0, 26.0);
    GarageSystem_RegisterGarage(STATE_LAS_VENTURAS, "LV Marina Garage", 1699.0, 1190.0, 10.0);

    print("[GARAGE] Garagen- und Fahrzeuglager initialisiert");
}

stock GarageSystem_RegisterGarage(stateid, const name[], Float:x, Float:y, Float:z)
{
    if (gGarageCount >= MAX_GARAGES)
        return false;
    if (!Core_IsValidState(stateid))
        return false;

    gGarageState[gGarageCount] = stateid;
    format(gGarageName[gGarageCount], 32, "%s", name);
    gGarageX[gGarageCount] = x;
    gGarageY[gGarageCount] = y;
    gGarageZ[gGarageCount] = z;
    gGarageVehicleCount[gGarageCount] = 0;
    gGarageCount++;
    return true;
}

stock GarageSystem_AddVehicleToGarage(garageid, modelid, price)
{
    if (garageid < 0 || garageid >= gGarageCount)
        return false;
    if (gGarageVehicleCount[garageid] >= MAX_GARAGE_VEHICLES)
        return false;

    new idx = gGarageVehicleCount[garageid];
    gGarageVehicleModel[garageid][idx] = modelid;
    gGarageVehiclePrice[garageid][idx] = price;
    gGarageVehicleCount[garageid]++;
    return true;
}

stock GarageSystem_ShowGarageList(playerid)
{
    new msg[256];
    format(msg, sizeof(msg), "[GARAGE] Verfügbare Garagen: %d", gGarageCount);
    SendClientMessage(playerid, 0x33CCFFAA, msg);

    for (new i = 0; i < gGarageCount; i++)
    {
        new garageMsg[128];
        format(garageMsg, sizeof(garageMsg), "%s | Staat %d | Fahrzeuge: %d | Pos: %.0f %.0f %.0f", gGarageName[i], gGarageState[i], gGarageVehicleCount[i], gGarageX[i], gGarageY[i], gGarageZ[i]);
        SendClientMessage(playerid, 0xFFFFFFFF, garageMsg);
    }
}

stock GarageSystem_StoreVehicle(playerid, vehicleid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
        return;

    new stateid = StateSystem_GetPlayerState(playerid);
    SendClientMessage(playerid, 0x00FF00AA, "[GARAGE] Fahrzeug im Staatsslot gespeichert.");
    printf("[GARAGE] Spieler %d hat Fahrzeug %d in Staat %d eingelagert.", playerid, vehicleid, stateid);
}

stock GarageSystem_SpawnVehicle(playerid, garageid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
        return;
    if (garageid < 0 || garageid >= gGarageCount)
        return;

    SetPlayerPos(playerid, gGarageX[garageid], gGarageY[garageid], gGarageZ[garageid]);
    SendClientMessage(playerid, 0x66CCFFAA, "[GARAGE] Du wurdest zur Garage teleportiert.");
}

stock GarageSystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
        return false;

    new cmd[32], idx;
    idx = sscanf(cmdtext, "s[32]", cmd);
    if (idx == 0)
        return false;

    if (strcmp(cmd, "/garage", true) == 0)
    {
        GarageSystem_ShowGarageList(playerid);
        return true;
    }

    if (strcmp(cmd, "/spawngarage", true) == 0)
    {
        new garageid;
        if (sscanf(cmdtext, "s[32]d", cmd, garageid) != 0)
        {
            garageid = StateSystem_GetPlayerState(playerid);
        }
        if (garageid < 0 || garageid >= gGarageCount)
        {
            garageid = 0;
        }
        GarageSystem_SpawnVehicle(playerid, garageid);
        return true;
    }

    return false;
}

#endif
