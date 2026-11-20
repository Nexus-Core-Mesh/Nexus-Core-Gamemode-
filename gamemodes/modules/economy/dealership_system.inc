#if !defined DEALERSHIP_SYSTEM_INC
#define DEALERSHIP_SYSTEM_INC

#include <a_samp>

#define MAX_DEALERSHIPS 12
#define DEALERSHIP_TYPE_NONE 0
#define DEALERSHIP_TYPE_STANDARD 1
#define DEALERSHIP_TYPE_IMPORT 2
#define DEALERSHIP_TYPE_LUXURY 3

new gDealerState[MAX_DEALERSHIPS];
new gDealerModel[MAX_DEALERSHIPS];
new gDealerPrice[MAX_DEALERSHIPS];
new gDealerVehicleId[MAX_DEALERSHIPS];
new gDealerName[MAX_DEALERSHIPS][32];
new gDealerCount = 0;

stock DealershipSystem_Initialize()
{
    for (new i = 0; i < MAX_DEALERSHIPS; i++)
    {
        gDealerState[i] = -1;
        gDealerModel[i] = 0;
        gDealerPrice[i] = 0;
        gDealerVehicleId[i] = -1;
        format(gDealerName[i], 32, "");
    }

    DealershipSystem_CreateListing(0, STATE_LOS_SANTOS, "LS Standard Autos", 400, 25000, DEALERSHIP_TYPE_STANDARD);
    DealershipSystem_CreateListing(1, STATE_SAN_FIERRO, "SF Import Center", 541, 32000, DEALERSHIP_TYPE_IMPORT);
    DealershipSystem_CreateListing(2, STATE_LAS_VENTURAS, "LV Luxury Motors", 562, 75000, DEALERSHIP_TYPE_LUXURY);

    print("[DEALER] Staatliche Autohäuser initialisiert");
}

stock DealershipSystem_CreateListing(dealerid, stateid, const name[], modelid, price, type)
{
    if (dealerid < 0 || dealerid >= MAX_DEALERSHIPS)
    {
        return false;
    }
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }

    gDealerState[dealerid] = stateid;
    gDealerModel[dealerid] = modelid;
    gDealerPrice[dealerid] = price;
    gDealerVehicleId[dealerid] = -1;
    format(gDealerName[dealerid], 32, "%s", name);
    gDealerCount++;

    return true;
}

stock DealershipSystem_BuyVehicle(playerid, dealerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (dealerid < 0 || dealerid >= MAX_DEALERSHIPS)
    {
        return false;
    }

    new price = gDealerPrice[dealerid];
    new stateid = gDealerState[dealerid];
    if (GetPlayerMoney(playerid) < price)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[DEALER] Du hast nicht genug Geld für das Fahrzeug.");
        return false;
    }

    GivePlayerMoney(playerid, -price);
    StateSystem_ApplyTreasuryDelta(stateid, price);

    gDealerVehicleId[dealerid] = CreateVehicle(gDealerModel[dealerid], 0.0, 0.0, 5.0, 0.0, 1, 1, 600000);
    new msg[128];
    format(msg, sizeof(msg), "[DEALER] Fahrzeug gekauft. Staatseinnahme: $%d | Fahrzeug-ID: %d", price, gDealerVehicleId[dealerid]);
    SendClientMessage(playerid, 0x00FF00AA, msg);
    return true;
}

stock int:DealershipSystem_GetDealerPrice(dealerid)
{
    if (dealerid < 0 || dealerid >= MAX_DEALERSHIPS)
    {
        return 0;
    }
    return gDealerPrice[dealerid];
}

stock int:DealershipSystem_GetDealerState(dealerid)
{
    if (dealerid < 0 || dealerid >= MAX_DEALERSHIPS)
    {
        return -1;
    }
    return gDealerState[dealerid];
}

stock DealershipSystem_ShowCatalog(playerid)
{
    new msg[256];
    format(msg, sizeof(msg), "[DEALER] Listings: %d | Staat: %d", gDealerCount, StateSystem_GetPlayerState(playerid));
    SendClientMessage(playerid, 0xFFFF66AA, msg);
}

stock DealershipSystem_CreateListingAtPlayer(playerid, stateid, const name[], modelid, price, type)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new dealerid = gDealerCount;
    if (dealerid >= MAX_DEALERSHIPS)
    {
        return false;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    gDealerState[dealerid] = stateid;
    gDealerModel[dealerid] = modelid;
    gDealerPrice[dealerid] = price;
    gDealerVehicleId[dealerid] = -1;
    format(gDealerName[dealerid], 32, "%s", name);
    gDealerCount++;
    return true;
}

#endif
