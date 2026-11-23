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
new gHouseExtraCount[MAX_HOUSES];
new gHouseExtraId[MAX_HOUSES][4];
new gHouseExtraState[MAX_HOUSES][4];
new gHouseExtraOwner[MAX_HOUSES][4];
new gHouseExtraStatus[MAX_HOUSES][4];

#define HOUSE_EXTRA_STATUS_INACTIVE 0
#define HOUSE_EXTRA_STATUS_ACTIVE 1
#define HOUSE_EXTRA_STATUS_OVERRIDDEN 2
#define HOUSE_EXTRA_STATUS_DELETED 3

stock HouseSystem_Initialize()
{
    for (new i = 0; i < MAX_HOUSES; i++)
    {
        gHouseState[i] = -1;
        gHouseOwner[i] = -1;
        gHousePrice[i] = 0;
        gHouseInterior[i] = 0;
        gHouseLocked[i] = HOUSE_LOCKED;
        gHouseExtraCount[i] = 0;
        for (new e = 0; e < 4; e++)
        {
            gHouseExtraId[i][e] = -1;
            gHouseExtraState[i][e] = -1;
            gHouseExtraOwner[i][e] = -1;
            gHouseExtraStatus[i][e] = HOUSE_EXTRA_STATUS_INACTIVE;
        }
    }

    HouseSystem_CreateHouse(0, 0, 250000, 1, -1);
    HouseSystem_CreateHouse(1, 1, 230000, 2, -1);
    HouseSystem_CreateHouse(2, 2, 260000, 3, -1);

    HouseSystem_RegisterExtra(0, 1, STATE_LOS_SANTOS, -1, HOUSE_EXTRA_STATUS_ACTIVE);
    HouseSystem_RegisterExtra(0, 2, STATE_LOS_SANTOS, -1, HOUSE_EXTRA_STATUS_INACTIVE);
    HouseSystem_RegisterExtra(1, 1, STATE_SAN_FIERRO, -1, HOUSE_EXTRA_STATUS_ACTIVE);
    HouseSystem_RegisterExtra(2, 1, STATE_LAS_VENTURAS, -1, HOUSE_EXTRA_STATUS_ACTIVE);

    print("[HOUSE] Dynamisches Haussystem initialisiert");
}

stock HouseSystem_RegisterExtra(houseid, extraid, stateid, ownerid, status)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (extraid < 0 || extraid >= 4)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        stateid = STATE_LOS_SANTOS;
    }
    if (status < HOUSE_EXTRA_STATUS_INACTIVE || status > HOUSE_EXTRA_STATUS_DELETED)
    {
        status = HOUSE_EXTRA_STATUS_INACTIVE;
    }

    gHouseExtraId[houseid][extraid] = extraid;
    gHouseExtraState[houseid][extraid] = stateid;
    gHouseExtraOwner[houseid][extraid] = ownerid;
    gHouseExtraStatus[houseid][extraid] = status;
    if (gHouseExtraCount[houseid] <= extraid)
    {
        gHouseExtraCount[houseid] = extraid + 1;
    }
    return true;
}

stock HouseSystem_SetExtraStatus(houseid, extraid, status)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (extraid < 0 || extraid >= 4)
    {
        return false;
    }
    if (status < HOUSE_EXTRA_STATUS_INACTIVE || status > HOUSE_EXTRA_STATUS_DELETED)
    {
        return false;
    }

    gHouseExtraStatus[houseid][extraid] = status;
    if (status == HOUSE_EXTRA_STATUS_DELETED)
    {
        gHouseExtraOwner[houseid][extraid] = -1;
    }
    return true;
}

stock int:HouseSystem_GetExtraStatus(houseid, extraid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return HOUSE_EXTRA_STATUS_INACTIVE;
    }
    if (extraid < 0 || extraid >= 4)
    {
        return HOUSE_EXTRA_STATUS_INACTIVE;
    }
    return gHouseExtraStatus[houseid][extraid];
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

    if (ownerid == -1)
    {
        for (new e = 0; e < 4; e++)
        {
            if (gHouseExtraStatus[houseid][e] == HOUSE_EXTRA_STATUS_ACTIVE)
            {
                gHouseExtraStatus[houseid][e] = HOUSE_EXTRA_STATUS_INACTIVE;
            }
        }
    }
    return true;
}

stock HouseSystem_SellHouse(playerid, houseid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (gHouseOwner[houseid] != playerid)
    {
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(gHouseState[houseid], STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    for (new i = 0; i < MAX_HOUSES; i++)
    {
        if (gHouseStorageOwner[i] == playerid && gHouseStorageState[i] == safeState)
        {
            gHouseStorageOwner[i] = -1;
            gHouseStorageState[i] = safeState;
        }
    }

    for (new e = 0; e < 4; e++)
    {
        if (gHouseExtraOwner[houseid][e] == playerid || gHouseExtraStatus[houseid][e] == HOUSE_EXTRA_STATUS_ACTIVE)
        {
            gHouseExtraStatus[houseid][e] = HOUSE_EXTRA_STATUS_INACTIVE;
            gHouseExtraOwner[houseid][e] = -1;
        }
    }

    gHouseOwner[houseid] = -1;
    gHouseLocked[houseid] = HOUSE_UNLOCKED;
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

stock int:HouseSystem_FindNearbyOwnedHouse(playerid, Float:radius = 25.0)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return -1;
    }

    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);

    for (new i = 0; i < MAX_HOUSES; i++)
    {
        if (gHouseOwner[i] == playerid)
        {
            new Float:houseX = float(i * 6 + 2);
            new Float:houseY = float(i * 4 + 3);
            if (GetPlayerDistanceFromPoint(playerid, houseX, houseY, pz) <= radius)
            {
                return i;
            }
        }
    }

    return -1;
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
