#if !defined AIRPORT_SYSTEM_INC
#define AIRPORT_SYSTEM_INC

#include <a_samp>

#define MAX_AIRPORTS 12

new gAirportState[MAX_AIRPORTS];
new gAirportName[MAX_AIRPORTS][32];
new gAirportRevenue[MAX_AIRPORTS];
new gAirportTransfer[MAX_AIRPORTS];
new gAirportCount[STATE_COUNT];

stock AirportSystem_Initialize()
{
    for (new i = 0; i < MAX_AIRPORTS; i++)
    {
        gAirportState[i] = -1;
        gAirportRevenue[i] = 0;
        gAirportTransfer[i] = 0;
        format(gAirportName[i], 32, "");
    }

    for (new s = 0; s < STATE_COUNT; s++)
    {
        gAirportCount[s] = 0;
    }

    AirportSystem_RegisterAirport(STATE_LOS_SANTOS, "Los Santos Airport", 19000, 6);
    AirportSystem_RegisterAirport(STATE_LOS_SANTOS, "City Air Hub", 12000, 4);
    AirportSystem_RegisterAirport(STATE_SAN_FIERRO, "San Fierro Harbor Terminal", 17000, 5);
    AirportSystem_RegisterAirport(STATE_SAN_FIERRO, "Mountain Airfield", 11000, 3);
    AirportSystem_RegisterAirport(STATE_LAS_VENTURAS, "Las Venturas Airport", 24000, 8);
    AirportSystem_RegisterAirport(STATE_LAS_VENTURAS, "Strip Air Terminal", 16000, 6);

    print("[AIRPORT] Airport network initialized");
}

stock AirportSystem_RegisterAirport(stateid, const name[], revenue, transfers)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    if (gAirportCount[stateid] >= MAX_AIRPORTS)
    {
        return false;
    }

    new idx = gAirportCount[stateid];
    new airportId = 0;
    for (new i = 0; i < MAX_AIRPORTS; i++)
    {
        if (gAirportState[i] == -1)
        {
            airportId = i;
            break;
        }
    }

    if (airportId == 0 && gAirportState[0] != -1)
    {
        airportId = MAX_AIRPORTS - 1;
    }

    gAirportState[airportId] = stateid;
    gAirportRevenue[airportId] = revenue;
    gAirportTransfer[airportId] = transfers;
    format(gAirportName[airportId], 32, "%s", name);
    gAirportCount[stateid]++;
    return true;
}

stock int:AirportSystem_GetRevenue(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }

    new total = 0;
    for (new i = 0; i < MAX_AIRPORTS; i++)
    {
        if (gAirportState[i] == stateid)
        {
            total += gAirportRevenue[i];
        }
    }
    return total;
}

stock AirportSystem_ShowOverview(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[AIRPORT] Ungültiger Staat.");
        return;
    }

    new name[32];
    StateSystem_GetStateName(stateid, name, sizeof(name));
    new message[256];
    format(message, sizeof(message), "[AIRPORT] %s | Airports: %d | Revenue: $%d", name, gAirportCount[stateid], AirportSystem_GetRevenue(stateid));
    SendClientMessage(playerid, 0x00D9FFFF, message);

    for (new i = 0; i < MAX_AIRPORTS; i++)
    {
        if (gAirportState[i] == stateid)
        {
            new line[128];
            format(line, sizeof(line), "- %s | Transfers: %d | Revenue: $%d", gAirportName[i], gAirportTransfer[i], gAirportRevenue[i]);
            SendClientMessage(playerid, 0xFFFFFFFF, line);
        }
    }
}

#endif
