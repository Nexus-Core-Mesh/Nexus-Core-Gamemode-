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

    gPlayerData[playerid][E_PLAYER_BANK] = Core_GetPlayerBank(playerid);
    gHouseOwner[houseid] = playerid;
    gHouseLocked[houseid] = HOUSE_LOCKED;
    StateSystem_ApplyTreasuryDelta(gHouseState[houseid], price / 12);
    SendClientMessage(playerid, 0x00FF00AA, "[HOUSE] Haus erfolgreich gekauft. Staatseinnahme: %d$", price / 12);
    return true;
}

stock HouseSystem_SetOwner(houseid, ownerid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    gHouseOwner[houseid] = ownerid;
    gHouseLocked[houseid] = HOUSE_LOCKED;
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

#endif
