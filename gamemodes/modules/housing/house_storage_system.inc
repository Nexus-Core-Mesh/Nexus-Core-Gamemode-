#if !defined HOUSE_STORAGE_SYSTEM_INC
#define HOUSE_STORAGE_SYSTEM_INC

#include <a_samp>

#if !defined MAX_HOUSE_STORAGE_SLOTS
#define MAX_HOUSE_STORAGE_SLOTS 12
#endif
#if !defined MAX_HOUSES
#define MAX_HOUSES 50
#endif
#if !defined INVENTORY_ITEM_NONE
#define INVENTORY_ITEM_NONE 0
#endif
#define HOUSE_STORAGE_NONE 0
#define HOUSE_STORAGE_SAFE 1
#define HOUSE_STORAGE_LOCKER 2

new gHouseStorageOwner[MAX_HOUSES];
new gHouseStorageState[MAX_HOUSES];
new gHouseStorageSafe[MAX_HOUSES];
new gHouseStorageMoney[MAX_HOUSES];
new gHouseStorageItem[MAX_HOUSES][MAX_HOUSE_STORAGE_SLOTS];
new gHouseStorageQty[MAX_HOUSES][MAX_HOUSE_STORAGE_SLOTS];
new gHouseStorageMeta[MAX_HOUSES][MAX_HOUSE_STORAGE_SLOTS][32];

stock HouseStorageSystem_OnPlayerConnect(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    for (new i = 0; i < MAX_HOUSES; i++)
    {
        if (gHouseStorageOwner[i] == playerid || gHouseStorageOwner[i] == -1)
        {
            DB_LoadHouseStorage(i);
        }
    }

    return true;
}

stock HouseStorageSystem_Initialize()
{
    for (new i = 0; i < MAX_HOUSES; i++)
    {
        gHouseStorageOwner[i] = -1;
        gHouseStorageState[i] = STATE_LOS_SANTOS;
        gHouseStorageSafe[i] = HOUSE_STORAGE_NONE;
        gHouseStorageMoney[i] = 0;
        for (new s = 0; s < MAX_HOUSE_STORAGE_SLOTS; s++)
        {
            gHouseStorageItem[i][s] = INVENTORY_ITEM_NONE;
            gHouseStorageQty[i][s] = 0;
            format(gHouseStorageMeta[i][s], 32, "NONE");
        }
    }

    HouseStorageSystem_RegisterHouseStorage(0, STATE_LOS_SANTOS, -1, HOUSE_STORAGE_SAFE, 5000);
    HouseStorageSystem_RegisterHouseStorage(1, STATE_SAN_FIERRO, -1, HOUSE_STORAGE_LOCKER, 3000);
    HouseStorageSystem_RegisterHouseStorage(2, STATE_LAS_VENTURAS, -1, HOUSE_STORAGE_SAFE, 8000);

    print("[HOUSE_STORAGE] Haus-Safes und Lagerplätze initialisiert");
}

stock HouseStorageSystem_RegisterHouseStorage(houseid, stateid, ownerid, type, money)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    gHouseStorageOwner[houseid] = ownerid;
    gHouseStorageState[houseid] = stateid;
    gHouseStorageSafe[houseid] = type;
    gHouseStorageMoney[houseid] = money;
    return true;
}

stock HouseStorageSystem_AssignOwner(playerid, houseid, ownerid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    gHouseStorageOwner[houseid] = ownerid;
    gHouseStorageSafe[houseid] = HOUSE_STORAGE_SAFE;
    return true;
}

stock HouseStorageSystem_StoreItem(playerid, houseid, itemid, quantity, metadata[] = "NONE")
{
    if (!Core_ValidatePlayerSlot(playerid))
    {
        return false;
    }
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (itemid <= INVENTORY_ITEM_NONE || quantity <= 0)
    {
        return false;
    }
    if (quantity > 99999)
    {
        return false;
    }

    for (new i = 0; i < MAX_HOUSE_STORAGE_SLOTS; i++)
    {
        if (gHouseStorageItem[houseid][i] == itemid)
        {
            new newQty = gHouseStorageQty[houseid][i] + quantity;
            if (newQty > 99999)
            {
                return false;
            }
            gHouseStorageQty[houseid][i] = newQty;
            format(gHouseStorageMeta[houseid][i], 32, "%s", metadata);
            DB_SaveHouseStorageItem(houseid, itemid, i, gHouseStorageQty[houseid][i], metadata);
            return true;
        }
    }

    for (new i = 0; i < MAX_HOUSE_STORAGE_SLOTS; i++)
    {
        if (gHouseStorageItem[houseid][i] == HOUSE_STORAGE_NONE)
        {
            gHouseStorageItem[houseid][i] = itemid;
            gHouseStorageQty[houseid][i] = quantity;
            format(gHouseStorageMeta[houseid][i], 32, "%s", metadata);
            DB_SaveHouseStorageItem(houseid, itemid, i, quantity, metadata);
            return true;
        }
    }

    SendClientMessage(playerid, 0xFF0000AA, "[HOUSE] Das Hauslager ist voll.");
    return false;
}

stock HouseStorageSystem_SetItemAtSlot(houseid, slot, itemid, quantity, const metadata[])
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (!Core_ValidateHouseStorageSlot(slot))
    {
        return false;
    }
    if (quantity < 0 || quantity > 99999)
    {
        return false;
    }
    if (itemid < INVENTORY_ITEM_NONE || itemid > INVENTORY_ITEM_MEDKIT)
    {
        return false;
    }

    if (quantity == 0)
    {
        gHouseStorageItem[houseid][slot] = HOUSE_STORAGE_NONE;
        gHouseStorageQty[houseid][slot] = 0;
        format(gHouseStorageMeta[houseid][slot], 32, "NONE");
        DB_SaveHouseStorageItem(houseid, HOUSE_STORAGE_NONE, slot, 0, "NONE");
        return true;
    }

    gHouseStorageItem[houseid][slot] = itemid;
    gHouseStorageQty[houseid][slot] = quantity;
    format(gHouseStorageMeta[houseid][slot], 32, "%s", metadata);
    DB_SaveHouseStorageItem(houseid, itemid, slot, quantity, metadata);
    return true;
}

stock HouseStorageSystem_TakeItem(playerid, houseid, itemid, quantity)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (itemid <= INVENTORY_ITEM_NONE || quantity <= 0)
    {
        return false;
    }

    for (new i = 0; i < MAX_HOUSE_STORAGE_SLOTS; i++)
    {
        if (gHouseStorageItem[houseid][i] == itemid)
        {
            if (gHouseStorageQty[houseid][i] < quantity)
            {
                return false;
            }

            gHouseStorageQty[houseid][i] -= quantity;
            if (gHouseStorageQty[houseid][i] <= 0)
            {
                gHouseStorageItem[houseid][i] = HOUSE_STORAGE_NONE;
                gHouseStorageQty[houseid][i] = 0;
                format(gHouseStorageMeta[houseid][i], 32, "NONE");
            }
            DB_SaveHouseStorageItem(houseid, gHouseStorageItem[houseid][i], i, gHouseStorageQty[houseid][i], gHouseStorageMeta[houseid][i]);
            return true;
        }
    }

    SendClientMessage(playerid, 0xFF0000AA, "[HOUSE] Dieses Item ist im Turmsafe nicht vorhanden.");
    return false;
}

stock HouseStorageSystem_DepositMoney(playerid, houseid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (amount <= 0)
    {
        return false;
    }

    gHouseStorageMoney[houseid] += amount;
    return true;
}

stock HouseStorageSystem_WithdrawMoney(playerid, houseid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (amount <= 0 || gHouseStorageMoney[houseid] < amount)
    {
        return false;
    }

    gHouseStorageMoney[houseid] -= amount;
    return true;
}

stock int:HouseStorageSystem_GetSafeBalance(houseid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return 0;
    }
    return gHouseStorageMoney[houseid];
}

#endif
