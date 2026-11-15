#if !defined CITY_HALL_SYSTEM_INC
#define CITY_HALL_SYSTEM_INC

#include <a_samp>

#define MAX_CITY_HALLS 3

new gCityHallState[MAX_CITY_HALLS];
new Float:gCityHallPosX[MAX_CITY_HALLS];
new Float:gCityHallPosY[MAX_CITY_HALLS];
new Float:gCityHallPosZ[MAX_CITY_HALLS];
new gCityHallOpen[MAX_CITY_HALLS];
new gCityHallName[MAX_CITY_HALLS][32];

stock CityHallSystem_Initialize()
{
    for (new i = 0; i < MAX_CITY_HALLS; i++)
    {
        gCityHallState[i] = -1;
        gCityHallPosX[i] = 0.0;
        gCityHallPosY[i] = 0.0;
        gCityHallPosZ[i] = 0.0;
        gCityHallOpen[i] = 1;
        format(gCityHallName[i], 32, "Unbekanntes Rathaus");
    }

    CityHallSystem_RegisterCityHall(STATE_LOS_SANTOS, "Los Santos Rathaus", 1503.0, -1700.0, 13.0);
    CityHallSystem_RegisterCityHall(STATE_SAN_FIERRO, "San Fierro Rathaus", -1914.0, 287.0, 35.0);
    CityHallSystem_RegisterCityHall(STATE_LAS_VENTURAS, "Las Venturas Rathaus", 1603.0, 1810.0, 10.0);

    print("[CITY_HALL] Stadthallen und Rathäuser initialisiert");
}

stock CityHallSystem_RegisterCityHall(stateid, const name[], Float:x, Float:y, Float:z)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new hallid = stateid;
    if (hallid < 0 || hallid >= MAX_CITY_HALLS)
    {
        return false;
    }

    gCityHallState[hallid] = stateid;
    gCityHallPosX[hallid] = x;
    gCityHallPosY[hallid] = y;
    gCityHallPosZ[hallid] = z;
    gCityHallOpen[hallid] = 1;
    format(gCityHallName[hallid], 32, "%s", name);
    return true;
}

stock CityHallSystem_ShowInfo(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[CITY_HALL] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    Core_GetStateName(stateid, stateName, sizeof(stateName));

    new msg[256];
    format(msg, sizeof(msg), "[CITY_HALL] %s | %s | offen: %s", stateName, gCityHallName[stateid], (gCityHallOpen[stateid] == 1) ? ("Ja") : ("Nein"));
    SendClientMessage(playerid, 0x00D9FFFF, msg);
}

stock CityHallSystem_IsNearCityHall(playerid, stateid, Float:radius)
{
    if (!Core_IsValidState(stateid) || playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    return GetDistanceBetweenPoints3D(x, y, z, gCityHallPosX[stateid], gCityHallPosY[stateid], gCityHallPosZ[stateid]) <= radius;
}

stock CityHallSystem_NotifyNearbyPlayer(playerid)
{
    new stateid = Core_GetPlayerState(playerid);
    if (!Core_IsValidState(stateid))
    {
        return;
    }

    if (CityHallSystem_IsNearCityHall(playerid, stateid, 30.0))
    {
        SendClientMessage(playerid, 0x33CCFFAA, "[CITY_HALL] Du bist am Rathaus deines Staates. Wahlen, Dienste und Staatsfinanzen verwalten sich hier.");
    }
}

#endif
