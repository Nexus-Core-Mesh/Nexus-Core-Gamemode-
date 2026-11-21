#if !defined HOUSE_SYSTEM_INC
#define HOUSE_SYSTEM_INC

#include <a_samp>

#define MAX_HOUSES 50
#define HOUSE_LOCKED 0
#define HOUSE_UNLOCKED 1

new gHouseState[MAX_HOUSES];
new gHouseOwner[MAX_HOUSES];
new gHousePrice[MAX_HOUSES];
new gHouseInterior[MAX_HOUSES];
new gHouseLocked[MAX_HOUSES];

stock HouseSystem_Initialize()
{
    for (new i = 0; i < MAX_HOUSES; i++)
    {
        gHouseState[i] = -1;
        gHouseOwner[i] = -1;
        gHousePrice[i] = 0;
        gHouseInterior[i] = 0;
        gHouseLocked[i] = HOUSE_LOCKED;
    }

    HouseSystem_CreateHouse(0, 0, 250000, 1, -1);
    HouseSystem_CreateHouse(1, 1, 230000, 2, -1);
    HouseSystem_CreateHouse(2, 2, 260000, 3, -1);

    print("[HOUSE] Dynamisches Haussystem initialisiert");
}

stock HouseSystem_CreateHouse(houseid, stateid, price, interior, ownerid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    gHouseState[houseid] = stateid;
    gHouseOwner[houseid] = ownerid;
    gHousePrice[houseid] = price;
    gHouseInterior[houseid] = interior;
    gHouseLocked[houseid] = (ownerid == -1) ? HOUSE_UNLOCKED : HOUSE_LOCKED;
    return true;
}

stock HouseSystem_BuyHouse(playerid, houseid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (gHouseOwner[houseid] != -1)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[HOUSE] Dieses Haus ist bereits vergeben.");
        return false;
    }

    new price = gHousePrice[houseid];
    if (!Core_ChangePlayerBank(playerid, -price))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[HOUSE] Du hast nicht genug Geld auf der Bank für dieses Haus.");
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(gHouseState[houseid], STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        Core_ChangePlayerBank(playerid, price);
        SendClientMessage(playerid, 0xFF0000AA, "[HOUSE] Staatlicher Kontext für das Haus ist ungültig.");
        return false;
    }

    gPlayerData[playerid][E_PLAYER_BANK] = Core_GetPlayerBank(playerid);
    gHouseOwner[houseid] = playerid;
    gHouseLocked[houseid] = HOUSE_LOCKED;
    if (!StateLedgerSystem_Transfer(safeState, max(0, price / 12), "house_purchase", "house_system"))
    {
        Core_ChangePlayerBank(playerid, price);
        gHouseOwner[houseid] = -1;
        gHouseLocked[houseid] = HOUSE_UNLOCKED;
        SendClientMessage(playerid, 0xFF0000AA, "[HOUSE] Staatlicher Transfer für das Haus fehlgeschlagen.");
        return false;
    }
    SendClientMessage(playerid, 0x00FF00AA, "[HOUSE] Haus erfolgreich gekauft. Staatseinnahme: %d$", price / 12);
    return true;
}

stock HouseSystem_SetOwner(houseid, ownerid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (ownerid < -1 || ownerid >= MAX_PLAYERS)
    {
        return false;
    }

    gHouseOwner[houseid] = ownerid;
    gHouseLocked[houseid] = (ownerid == -1) ? HOUSE_UNLOCKED : HOUSE_LOCKED;
    return true;
}

stock HouseSystem_SetState(houseid, stateid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(stateid, STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    gHouseState[houseid] = safeState;
    return true;
}

stock HouseSystem_SetPrice(houseid, price)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (price < 0)
    {
        return false;
    }

    gHousePrice[houseid] = price;
    return true;
}

stock int:HouseSystem_GetOwner(houseid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return -1;
    }
    return gHouseOwner[houseid];
}

stock int:HouseSystem_GetState(houseid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return -1;
    }
    return gHouseState[houseid];
}

stock int:HouseSystem_GetPrice(houseid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return 0;
    }
    return gHousePrice[houseid];
}

stock HouseSystem_ShowInfo(playerid, houseid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return;
    }

    new msg[256];
    format(msg, sizeof(msg), "[HOUSE] Haus %d | Staat: %d | Preis: %d | Besitzer: %d | Interior: %d", houseid, gHouseState[houseid], gHousePrice[houseid], gHouseOwner[houseid], gHouseInterior[houseid]);
    SendClientMessage(playerid, 0xFFFF00AA, msg);
}

stock HouseSystem_CreateHouseAtPlayer(playerid, stateid, price, interior)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new houseid = 0;
    while (houseid < MAX_HOUSES && gHouseOwner[houseid] != -1)
    {
        houseid++;
    }
    if (houseid >= MAX_HOUSES)
    {
        return false;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    gHouseState[houseid] = stateid;
    gHouseOwner[houseid] = -1;
    gHousePrice[houseid] = price > 0 ? price : 250000;
    gHouseInterior[houseid] = interior > 0 ? interior : 1;
    gHouseLocked[houseid] = HOUSE_UNLOCKED;
    return true;
}

#endif
