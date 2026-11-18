#if !defined TERRITORY_SYSTEM_INC
#define TERRITORY_SYSTEM_INC

#include <a_samp>

#define MAX_TERRITORIES 30
#define TERRITORY_TYPE_RESIDENTIAL 0
#define TERRITORY_TYPE_COMMERCIAL 1
#define TERRITORY_TYPE_INDUSTRIAL 2
#define TERRITORY_TYPE_GOVERNMENT 3
#define TERRITORY_TYPE_RECREATIONAL 4

new gTerritoryState[MAX_TERRITORIES];
new gTerritoryType[MAX_TERRITORIES];
new gTerritoryControl[MAX_TERRITORIES];
new gTerritoryValue[MAX_TERRITORIES];
new gTerritoryCount = 0;
new gTerritoryName[MAX_TERRITORIES][40];

stock TerritorySystem_Initialize()
{
    gTerritoryCount = 0;
    for (new i = 0; i < MAX_TERRITORIES; i++)
    {
        gTerritoryState[i] = -1;
        gTerritoryType[i] = TERRITORY_TYPE_RESIDENTIAL;
        gTerritoryControl[i] = -1;
        gTerritoryValue[i] = 0;
        format(gTerritoryName[i], 40, "");
    }

    TerritorySystem_RegTerr(STATE_LOS_SANTOS, "Downtown Core", TERRITORY_TYPE_COMMERCIAL, 450000, STATE_LOS_SANTOS);
    TerritorySystem_RegTerr(STATE_LOS_SANTOS, "Vinewood Heights", TERRITORY_TYPE_RESIDENTIAL, 325000, STATE_LOS_SANTOS);
    TerritorySystem_RegTerr(STATE_LOS_SANTOS, "Harbor Industrial", TERRITORY_TYPE_INDUSTRIAL, 510000, STATE_LOS_SANTOS);

    TerritorySystem_RegTerr(STATE_SAN_FIERRO, "Bayview District", TERRITORY_TYPE_RESIDENTIAL, 310000, STATE_SAN_FIERRO);
    TerritorySystem_RegTerr(STATE_SAN_FIERRO, "Airport Logistics", TERRITORY_TYPE_INDUSTRIAL, 470000, STATE_SAN_FIERRO);
    TerritorySystem_RegTerr(STATE_SAN_FIERRO, "City Center", TERRITORY_TYPE_COMMERCIAL, 420000, STATE_SAN_FIERRO);

    TerritorySystem_RegTerr(STATE_LAS_VENTURAS, "The Strip", TERRITORY_TYPE_COMMERCIAL, 620000, STATE_LAS_VENTURAS);
    TerritorySystem_RegTerr(STATE_LAS_VENTURAS, "Marina Plaza", TERRITORY_TYPE_RECREATIONAL, 360000, STATE_LAS_VENTURAS);
    TerritorySystem_RegTerr(STATE_LAS_VENTURAS, "Old Town", TERRITORY_TYPE_RESIDENTIAL, 340000, STATE_LAS_VENTURAS);

    print("[TERRITORY] Territorien, Districts und Einflusszonen initialisiert");
    Core_MarkModuleReady("world");
}

stock TerritorySystem_RegTerr(stateid, const name[], type, value, controller)
{
    if (!Core_IsValidState(stateid) || gTerritoryCount >= MAX_TERRITORIES)
    {
        return false;
    }

    new idx = gTerritoryCount;
    gTerritoryState[idx] = stateid;
    gTerritoryType[idx] = type;
    gTerritoryControl[idx] = controller;
    gTerritoryValue[idx] = value;
    format(gTerritoryName[idx], 40, "%s", name);
    gTerritoryCount++;
    return true;
}

stock int:TerritorySystem_GetTotalCount()
{
    return gTerritoryCount;
}

// Liefert die n-te (0-basiert) Territoriums-ID, deren Gebiet im Staat liegt, oder -1.
stock int:TerritorySystem_GetNthForState(stateid, nth)
{
    new count = 0;
    for (new i = 0; i < gTerritoryCount; i++)
    {
        if (gTerritoryState[i] == stateid)
        {
            if (count == nth)
            {
                return i;
            }
            count++;
        }
    }
    return -1;
}

stock TerritorySystem_GetName(territoryId, output[], len)
{
    if (territoryId < 0 || territoryId >= gTerritoryCount)
    {
        format(output, len, "Unbekannt");
        return;
    }
    format(output, len, "%s", gTerritoryName[territoryId]);
}

stock int:TerritorySystem_GetControl(territoryId)
{
    return (territoryId >= 0 && territoryId < gTerritoryCount) ? gTerritoryControl[territoryId] : -1;
}

stock int:TerritorySystem_GetValue(territoryId)
{
    return (territoryId >= 0 && territoryId < gTerritoryCount) ? gTerritoryValue[territoryId] : 0;
}

stock int:TerritorySystem_GetType(territoryId)
{
    return (territoryId >= 0 && territoryId < gTerritoryCount) ? gTerritoryType[territoryId] : 0;
}

stock int:TerritorySystem_GetControlCount(controlState)
{
    new count = 0;
    for (new i = 0; i < gTerritoryCount; i++)
    {
        if (gTerritoryControl[i] == controlState)
        {
            count++;
        }
    }
    return count;
}

stock int:TerritorySystem_GetCount(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }

    new count = 0;
    for (new i = 0; i < gTerritoryCount; i++)
    {
        if (gTerritoryState[i] == stateid)
        {
            count++;
        }
    }
    return count;
}

stock TerritorySystem_ShowTerritories(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[TERRITORY] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new message[256];
    format(message, sizeof(message), "[TERRITORY] %s | Bezirke: %d", stateName, TerritorySystem_GetCount(stateid));
    SendClientMessage(playerid, 0x00D9FFFF, message);

    for (new i = 0; i < gTerritoryCount; i++)
    {
        if (gTerritoryState[i] != stateid)
        {
            continue;
        }

        new line[160];
        format(line, sizeof(line), "- %s | Typ: %d | Wert: $%d | Kontrolle: %d", gTerritoryName[i], gTerritoryType[i], gTerritoryValue[i], gTerritoryControl[i]);
        SendClientMessage(playerid, 0xFFFFFFFF, line);
    }
}

stock TerritorySystem_ClaimTerritory(playerid, territoryId, newState)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidState(newState))
    {
        return false;
    }

    if (territoryId < 0 || territoryId >= gTerritoryCount)
    {
        return false;
    }

    if (gTerritoryControl[territoryId] == newState)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[TERRITORY] Dieses Territorium gehört bereits diesem Staat.");
        return false;
    }

    gTerritoryState[territoryId] = newState;
    gTerritoryControl[territoryId] = newState;
    gTerritoryValue[territoryId] += 25000;

    new stateName[32];
    StateSystem_GetStateName(newState, stateName, sizeof(stateName));
    new msg[160];
    format(msg, sizeof(msg), "[TERRITORY] %s übernommen. Einflussbereich für %s erweitert.", gTerritoryName[territoryId], stateName);
    SendClientMessage(playerid, 0x00FF00AA, msg);
    return true;
}

#endif
