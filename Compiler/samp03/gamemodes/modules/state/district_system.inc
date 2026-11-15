#if !defined DISTRICT_SYSTEM_INC
#define DISTRICT_SYSTEM_INC

#include <a_samp>

#define MAX_DISTRICTS 18

new gDistrictState[MAX_DISTRICTS];
new gDistrictType[MAX_DISTRICTS][16];
new gDistrictName[MAX_DISTRICTS][32];
new Float:gDistrictX[MAX_DISTRICTS];
new Float:gDistrictY[MAX_DISTRICTS];
new Float:gDistrictZ[MAX_DISTRICTS];
new gDistrictCount = 0;

stock DistrictSystem_Initialize()
{
    gDistrictCount = 0;

    for (new i = 0; i < MAX_DISTRICTS; i++)
    {
        gDistrictState[i] = -1;
        format(gDistrictType[i], 16, "");
        format(gDistrictName[i], 32, "");
        gDistrictX[i] = 0.0;
        gDistrictY[i] = 0.0;
        gDistrictZ[i] = 0.0;
    }

    DistrictSystem_RegisterDistrict(STATE_LOS_SANTOS, "Downtown", "commercial", 1500.0, -1670.0, 13.0);
    DistrictSystem_RegisterDistrict(STATE_LOS_SANTOS, "Vinewood", "residential", 1340.0, 1035.0, 11.0);
    DistrictSystem_RegisterDistrict(STATE_LOS_SANTOS, "Harbor", "industrial", 1515.0, -2000.0, 13.0);

    DistrictSystem_RegisterDistrict(STATE_SAN_FIERRO, "City Center", "commercial", -1935.0, 160.0, 27.0);
    DistrictSystem_RegisterDistrict(STATE_SAN_FIERRO, "Airport", "transport", -1500.0, -150.0, 14.0);
    DistrictSystem_RegisterDistrict(STATE_SAN_FIERRO, "Bayview", "residential", -2130.0, 80.0, 35.0);

    DistrictSystem_RegisterDistrict(STATE_LAS_VENTURAS, "The Strip", "tourism", 2040.0, 1110.0, 10.0);
    DistrictSystem_RegisterDistrict(STATE_LAS_VENTURAS, "Marina", "harbor", 1705.0, 1230.0, 11.0);
    DistrictSystem_RegisterDistrict(STATE_LAS_VENTURAS, "Old Town", "residential", 1760.0, 1425.0, 10.0);

    print("[DISTRICT] Staatliche Bezirke und Stadtviertel initialisiert");
}

stock DistrictSystem_RegisterDistrict(stateid, const name[], const type[], Float:x, Float:y, Float:z)
{
    if (!Core_IsValidState(stateid) || gDistrictCount >= MAX_DISTRICTS)
    {
        return false;
    }

    new idx = gDistrictCount;
    gDistrictState[idx] = stateid;
    format(gDistrictName[idx], 32, "%s", name);
    format(gDistrictType[idx], 16, "%s", type);
    gDistrictX[idx] = x;
    gDistrictY[idx] = y;
    gDistrictZ[idx] = z;
    gDistrictCount++;
    return true;
}

stock int:DistrictSystem_GetCountForState(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }

    new count = 0;
    for (new i = 0; i < gDistrictCount; i++)
    {
        if (gDistrictState[i] == stateid)
        {
            count++;
        }
    }
    return count;
}

stock DistrictSystem_ShowDistricts(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[DISTRICT] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new msg[256];
    format(msg, sizeof(msg), "[DISTRICT] %s | Bezirke: %d", stateName, DistrictSystem_GetCountForState(stateid));
    SendClientMessage(playerid, 0x00D9FFFF, msg);

    for (new i = 0; i < gDistrictCount; i++)
    {
        if (gDistrictState[i] != stateid)
        {
            continue;
        }

        new entry[128];
        format(entry, sizeof(entry), "- %s (%s) | %.0f %.0f %.0f", gDistrictName[i], gDistrictType[i], gDistrictX[i], gDistrictY[i], gDistrictZ[i]);
        SendClientMessage(playerid, 0xFFFFFFFF, entry);
    }
}

stock DistrictSystem_IsNearDistrict(playerid, stateid, Float:radius)
{
    if (!Core_IsValidState(stateid) || playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    for (new i = 0; i < gDistrictCount; i++)
    {
        if (gDistrictState[i] != stateid)
        {
            continue;
        }

        if (GetDistanceBetweenPoints3D(x, y, z, gDistrictX[i], gDistrictY[i], gDistrictZ[i]) <= radius)
        {
            return true;
        }
    }

    return false;
}

#endif
