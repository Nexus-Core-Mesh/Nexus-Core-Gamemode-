#if !defined TAXI_SYSTEM_INC
#define TAXI_SYSTEM_INC

#include <a_samp>

#define MAX_TAXI_STANDS 10

new gTaxiStandState[MAX_TAXI_STANDS];
new gTaxiStandName[MAX_TAXI_STANDS][32];
new gTaxiStandCount = 0;
new gPlayerTaxiState[MAX_PLAYERS];
new gPlayerTaxiOnDuty[MAX_PLAYERS];

stock TaxiSystem_Initialize()
{
    gTaxiStandCount = 0;

    for (new i = 0; i < MAX_TAXI_STANDS; i++)
    {
        gTaxiStandState[i] = -1;
        format(gTaxiStandName[i], 32, "");
    }

    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gPlayerTaxiState[i] = -1;
        gPlayerTaxiOnDuty[i] = 0;
    }

    TaxiSystem_RegisterStand(STATE_LOS_SANTOS, "Downtown Taxi");
    TaxiSystem_RegisterStand(STATE_SAN_FIERRO, "Harbor Taxi");
    TaxiSystem_RegisterStand(STATE_LAS_VENTURAS, "Strip Taxi");

    print("[TAXI] Taxi-und Transportnetzwerk initialisiert");
}

stock TaxiSystem_RegisterStand(stateid, const name[])
{
    if (!Core_IsValidState(stateid) || gTaxiStandCount >= MAX_TAXI_STANDS)
    {
        return false;
    }

    gTaxiStandState[gTaxiStandCount] = stateid;
    format(gTaxiStandName[gTaxiStandCount], 32, "%s", name);
    gTaxiStandCount++;
    return true;
}

stock TaxiSystem_StartShift(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidState(stateid))
    {
        return false;
    }

    gPlayerTaxiState[playerid] = stateid;
    gPlayerTaxiOnDuty[playerid] = 1;
    SendClientMessage(playerid, 0x00D9FFFF, "[TAXI] Dienst gestartet. Fahrten sind jetzt aktiv.");
    return true;
}

stock TaxiSystem_EndShift(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    gPlayerTaxiState[playerid] = -1;
    gPlayerTaxiOnDuty[playerid] = 0;
    SendClientMessage(playerid, 0xFFCC66AA, "[TAXI] Dienst beendet.");
    return true;
}

stock TaxiSystem_ShowNetwork(playerid)
{
    new stateName[32];
    new stateid = StateSystem_GetPlayerState(playerid);
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));

    new message[256];
    format(message, sizeof(message), "[TAXI] %s | Stands: %d", stateName, gTaxiStandCount);
    SendClientMessage(playerid, 0x00D9FFFF, message);

    for (new i = 0; i < gTaxiStandCount; i++)
    {
        if (gTaxiStandState[i] == stateid)
        {
            new line[128];
            format(line, sizeof(line), "- %s", gTaxiStandName[i]);
            SendClientMessage(playerid, 0xFFFFFFFF, line);
        }
    }
}

#endif
