#if !defined FUEL_STATION_SYSTEM_INC
#define FUEL_STATION_SYSTEM_INC

#include <a_samp>

#define MAX_FUEL_STATIONS 12
#define FUEL_STATION_TYPE_GAS 0
#define FUEL_STATION_TYPE_DIESEL 1

new gFuelStationState[MAX_FUEL_STATIONS];
new Float:gFuelStationX[MAX_FUEL_STATIONS];
new Float:gFuelStationY[MAX_FUEL_STATIONS];
new Float:gFuelStationZ[MAX_FUEL_STATIONS];
new gFuelStationPrice[MAX_FUEL_STATIONS];
new gFuelStationCount = 0;

stock FuelStationSystem_Initialize()
{
    DB_EnsureFuelStationSchema();
    for (new i = 0; i < MAX_FUEL_STATIONS; i++)
    {
        gFuelStationState[i] = -1;
        gFuelStationPrice[i] = 10;
        gFuelStationX[i] = 0.0;
        gFuelStationY[i] = 0.0;
        gFuelStationZ[i] = 0.0;
    }

    FuelStationSystem_RegStation(STATE_LOS_SANTOS, 1500.0, -1700.0, 13.0, 12);
    FuelStationSystem_RegStation(STATE_SAN_FIERRO, -1900.0, 120.0, 27.0, 11);
    FuelStationSystem_RegStation(STATE_LAS_VENTURAS, 1705.0, 1215.0, 10.0, 13);

    print("[FUEL] Tankstellen- und Benzinpreise initialisiert");
}

stock FuelStationSystem_RegStation(stateid, Float:x, Float:y, Float:z, price)
{
    if (gFuelStationCount >= MAX_FUEL_STATIONS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    gFuelStationState[gFuelStationCount] = stateid;
    gFuelStationX[gFuelStationCount] = x;
    gFuelStationY[gFuelStationCount] = y;
    gFuelStationZ[gFuelStationCount] = z;
    gFuelStationPrice[gFuelStationCount] = price;
    DB_Execute("INSERT OR REPLACE INTO fuel_stations (state_id, x, y, z, price, updated_at) VALUES (-1, 0.0, 0.0, 0.0, 0, datetime('now'))");
    gFuelStationCount++;
    return true;
}

stock int:FuelStationSystem_GetPrice(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }

    for (new i = 0; i < gFuelStationCount; i++)
    {
        if (gFuelStationState[i] == stateid)
        {
            return gFuelStationPrice[i];
        }
    }

    return 10;
}

stock FuelStationSystem_RefuelPlayer(playerid, stateid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new cost = FuelStationSystem_GetPrice(stateid) * amount;
    if (GetPlayerMoney(playerid) < cost)
    {
        return false;
    }

    GivePlayerMoney(playerid, -cost);
    StateSystem_ApplyTreasuryDelta(stateid, cost);
    return true;
}

#endif
