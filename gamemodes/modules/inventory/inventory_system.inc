#if defined _INVENTORY_SYSTEM_INCLUDED
    #endinput
#endif
#define _INVENTORY_SYSTEM_INCLUDED

#include <a_samp>

#if !defined MAX_INVENTORY_SLOTS
#define MAX_INVENTORY_SLOTS 24
#endif
#define INVALID_ITEM_ID -1

#define INVENTORY_ITEM_NONE 0
#define INVENTORY_ITEM_PHONE 1
#define INVENTORY_ITEM_TOOL 2
#define INVENTORY_ITEM_DRUG 3
#define INVENTORY_ITEM_WEAPON 4
#define INVENTORY_ITEM_FOOD 5
#define INVENTORY_ITEM_KEY 6
#define INVENTORY_ITEM_MATERIAL 7
#define INVENTORY_ITEM_MEDKIT 8

#define INVENTORY_MAX_STACK_DEFAULT 10
#define INVENTORY_STACKABLE 1
#define INVENTORY_UNSTACKABLE 0

enum E_ITEM_DATA
{
    item_id,
    item_name[32],
    bool:item_is_stackable,
    item_max_stack
}

new gInventoryItem[MAX_PLAYERS][MAX_INVENTORY_SLOTS];
new gInventoryItemQty[MAX_PLAYERS][MAX_INVENTORY_SLOTS];
new gInventoryItemMeta[MAX_PLAYERS][MAX_INVENTORY_SLOTS][32];
new gInventoryWeight[MAX_PLAYERS];
new gInventoryMoney[MAX_PLAYERS];
new gInventoryInitialized[MAX_PLAYERS];

forward Inventory_Load(playerid);
forward Inventory_Save(playerid);

stock Inventory_GetItemData(itemid, item_name_out[], item_name_len, &bool:is_stackable, &max_stack)
{
    if (itemid <= INVENTORY_ITEM_NONE)
    {
        format(item_name_out, item_name_len, "Leer");
        is_stackable = false;
        max_stack = 1;
        return false;
    }

    switch (itemid)
    {
        case INVENTORY_ITEM_PHONE:
        {
            format(item_name_out, item_name_len, "Telefon");
            is_stackable = false;
            max_stack = 1;
            return true;
        }
        case INVENTORY_ITEM_TOOL:
        {
            format(item_name_out, item_name_len, "Werkzeug");
            is_stackable = false;
            max_stack = 1;
            return true;
        }
        case INVENTORY_ITEM_DRUG:
        {
            format(item_name_out, item_name_len, "Droge");
            is_stackable = true;
            max_stack = 25;
            return true;
        }
        case INVENTORY_ITEM_WEAPON:
        {
            format(item_name_out, item_name_len, "Waffe");
            is_stackable = false;
            max_stack = 1;
            return true;
        }
        case INVENTORY_ITEM_FOOD:
        {
            format(item_name_out, item_name_len, "Essen");
            is_stackable = true;
            max_stack = 10;
            return true;
        }
        case INVENTORY_ITEM_KEY:
        {
            format(item_name_out, item_name_len, "Schluessel");
            is_stackable = false;
            max_stack = 1;
            return true;
        }
        case INVENTORY_ITEM_MATERIAL:
        {
            format(item_name_out, item_name_len, "Material");
            is_stackable = true;
            max_stack = 50;
            return true;
        }
        case INVENTORY_ITEM_MEDKIT:
        {
            format(item_name_out, item_name_len, "Medkit");
            is_stackable = true;
            max_stack = 6;
            return true;
        }
        default:
        {
            format(item_name_out, item_name_len, "Unbekannt");
            is_stackable = false;
            max_stack = 1;
            return false;
        }
    }
}

stock InventorySystem_Initialize()
{
    DB_EnsureInventorySchema();
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gInventoryWeight[i] = 0;
        gInventoryMoney[i] = 0;
        gInventoryInitialized[i] = 0;
        for (new s = 0; s < MAX_INVENTORY_SLOTS; s++)
        {
            gInventoryItem[i][s] = INVENTORY_ITEM_NONE;
            gInventoryItemQty[i][s] = 0;
            format(gInventoryItemMeta[i][s], 32, "NONE");
        }
    }
    print("[INV] Relationale Inventar-Architektur initialisiert");
}

stock InventorySystem_SetInv(playerid, slot, itemid, quantity, const metadata[])
{
    if (!Core_ValidatePlayerSlot(playerid))
    {
        return false;
    }
    if (!Core_ValidateInventorySlot(slot))
    {
        return false;
    }
    if (quantity < 0)
    {
        return false;
    }
    if (itemid < INVENTORY_ITEM_NONE || itemid > INVENTORY_ITEM_MEDKIT)
    {
        return false;
    }

    new bool:isStackable = false;
    new maxStack = INVENTORY_MAX_STACK_DEFAULT;
    new itemName[32];
    if (!Inventory_GetItemData(itemid, itemName, sizeof(itemName), isStackable, maxStack))
    {
        if (itemid != INVENTORY_ITEM_NONE)
        {
            return false;
        }
    }

    if (quantity == 0)
    {
        gInventoryItem[playerid][slot] = INVENTORY_ITEM_NONE;
        gInventoryItemQty[playerid][slot] = 0;
        format(gInventoryItemMeta[playerid][slot], 32, "NONE");
        return true;
    }

    if (!Core_ValidateItemStack(itemid, quantity, maxStack, isStackable))
    {
        return false;
    }

    gInventoryItem[playerid][slot] = itemid;
    gInventoryItemQty[playerid][slot] = quantity;
    format(gInventoryItemMeta[playerid][slot], 32, "%s", metadata);
    return true;
}

stock InventorySystem_UseItem(playerid, slot)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (slot < 0 || slot >= MAX_INVENTORY_SLOTS)
    {
        return false;
    }

    if (gInventoryItem[playerid][slot] == INVENTORY_ITEM_NONE)
    {
        return false;
    }

    if (gInventoryItemQty[playerid][slot] <= 0)
    {
        gInventoryItem[playerid][slot] = INVENTORY_ITEM_NONE;
        format(gInventoryItemMeta[playerid][slot], 32, "NONE");
        return false;
    }

    gInventoryItemQty[playerid][slot]--;
    if (gInventoryItemQty[playerid][slot] <= 0)
    {
        gInventoryItem[playerid][slot] = INVENTORY_ITEM_NONE;
        gInventoryItemQty[playerid][slot] = 0;
        format(gInventoryItemMeta[playerid][slot], 32, "NONE");
    }
    return true;
}

#define INV_USE_FAILED 0
#define INV_USE_CONSUMED 1
#define INV_USE_NO_EFFECT 2
#define INV_USE_OPEN_PHONE 3

// Wendet den Effekt eines Slots an und verbraucht nur verbrauchbare Items (persistent über Inventory_Remove).
stock InventorySystem_UseSlot(playerid, slot)
{
    if (!Core_ValidatePlayerSlot(playerid) || slot < 0 || slot >= MAX_INVENTORY_SLOTS)
    {
        return INV_USE_FAILED;
    }

    new itemid = gInventoryItem[playerid][slot];
    if (itemid == INVENTORY_ITEM_NONE || gInventoryItemQty[playerid][slot] <= 0)
    {
        return INV_USE_FAILED;
    }

    switch (itemid)
    {
        case INVENTORY_ITEM_PHONE:
        {
            return INV_USE_OPEN_PHONE;
        }
        case INVENTORY_ITEM_FOOD:
        {
            new Float:health;
            GetPlayerHealth(playerid, health);
            SetPlayerHealth(playerid, min(100.0, health + 25.0));
            Inventory_Remove(playerid, itemid, 1);
            return INV_USE_CONSUMED;
        }
        case INVENTORY_ITEM_MEDKIT:
        {
            SetPlayerHealth(playerid, 100.0);
            Inventory_Remove(playerid, itemid, 1);
            return INV_USE_CONSUMED;
        }
        case INVENTORY_ITEM_TOOL:
        {
            new vehicleid = GetPlayerVehicleID(playerid);
            if (vehicleid == 0)
            {
                return INV_USE_FAILED;
            }
            RepairVehicle(vehicleid);
            return INV_USE_NO_EFFECT;
        }
    }
    return INV_USE_NO_EFFECT;
}

stock InventorySystem_OnPlayerConnect(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    gInventoryInitialized[playerid] = 1;
    Inventory_Load(playerid);
}

public Inventory_Load(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 1;
    }

    gInventoryInitialized[playerid] = 1;
    DB_LoadPlayerInventory(playerid);
    return 1;
}

public Inventory_Save(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 1;
    }

    for (new i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        if (gInventoryItem[playerid][i] != INVENTORY_ITEM_NONE && gInventoryItemQty[playerid][i] > 0)
        {
            DB_SavePlayerInventoryItem(playerid, gInventoryItem[playerid][i], i, gInventoryItemQty[playerid][i], gInventoryItemMeta[playerid][i]);
        }
    }
    return 1;
}

stock Inventory_Add(playerid, itemid, quantity, metadata[] = "NONE")
{
    if (!Core_ValidatePlayerSlot(playerid))
    {
        return false;
    }
    if (itemid <= INVENTORY_ITEM_NONE || quantity <= 0)
    {
        return false;
    }

    new bool:isStackable = false;
    new maxStack = INVENTORY_MAX_STACK_DEFAULT;
    new itemName[32];
    if (!Inventory_GetItemData(itemid, itemName, sizeof(itemName), isStackable, maxStack))
    {
        return false;
    }
    if (!Core_ValidateItemStack(itemid, quantity, maxStack, isStackable))
    {
        return false;
    }

    for (new i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        if (gInventoryItem[playerid][i] == itemid && isStackable)
        {
            new newQty = gInventoryItemQty[playerid][i] + quantity;
            if (newQty <= maxStack)
            {
                gInventoryItemQty[playerid][i] = newQty;
                format(gInventoryItemMeta[playerid][i], 32, "%s", metadata);
                DB_SavePlayerInventoryItem(playerid, itemid, i, newQty, metadata);
                gInventoryWeight[playerid] += 1;
                return true;
            }
        }
    }

    for (new i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        if (gInventoryItem[playerid][i] == INVENTORY_ITEM_NONE)
        {
            gInventoryItem[playerid][i] = itemid;
            gInventoryItemQty[playerid][i] = quantity;
            format(gInventoryItemMeta[playerid][i], 32, "%s", metadata);
            gInventoryWeight[playerid] += 1;
            DB_SavePlayerInventoryItem(playerid, itemid, i, quantity, metadata);
            return true;
        }
    }

    return false;
}

stock Inventory_Remove(playerid, itemid, quantity)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (itemid <= INVENTORY_ITEM_NONE || quantity <= 0)
    {
        return false;
    }

    for (new i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        if (gInventoryItem[playerid][i] == itemid)
        {
            if (gInventoryItemQty[playerid][i] <= quantity)
            {
                gInventoryItem[playerid][i] = INVENTORY_ITEM_NONE;
                gInventoryItemQty[playerid][i] = 0;
                format(gInventoryItemMeta[playerid][i], 32, "NONE");
                gInventoryWeight[playerid] = max(0, gInventoryWeight[playerid] - 1);
                DB_RemovePlayerInventoryItem(playerid, itemid, quantity);
                return true;
            }
            else
            {
                gInventoryItemQty[playerid][i] -= quantity;
                DB_RemovePlayerInventoryItem(playerid, itemid, quantity);
                return true;
            }
        }
    }
    return false;
}

stock Inventory_HasItem(playerid, itemid, quantity)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (itemid <= INVENTORY_ITEM_NONE)
    {
        return false;
    }

    for (new i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        if (gInventoryItem[playerid][i] == itemid)
        {
            if (gInventoryItemQty[playerid][i] >= quantity)
            {
                return true;
            }
        }
    }
    return false;
}

stock InventorySystem_TransferItem(playerid, targetplayerid, itemid, quantity, metadata[] = "NONE")
{
    if (playerid < 0 || playerid >= MAX_PLAYERS || targetplayerid < 0 || targetplayerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Inventory_HasItem(playerid, itemid, quantity))
    {
        return false;
    }
    if (!Inventory_Add(targetplayerid, itemid, quantity, metadata))
    {
        return false;
    }
    return Inventory_Remove(playerid, itemid, quantity);
}

stock InventorySystem_GetWeight(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gInventoryWeight[playerid];
}

stock InventorySystem_GetFreeSlots(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }

    new count = 0;
    for (new i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        if (gInventoryItem[playerid][i] == INVENTORY_ITEM_NONE)
        {
            count++;
        }
    }
    return count;
}

stock InventorySystem_ShowInventory(playerid)
{
    new msg[256];
    format(msg, sizeof(msg), "[INV] Gewicht: %d | Freie Slots: %d | Status: %d", gInventoryWeight[playerid], InventorySystem_GetFreeSlots(playerid), gInventoryInitialized[playerid]);
    SendClientMessage(playerid, 0x00CCFFAA, msg);
}

stock InventorySystem_ResetPlayer(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    for (new i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        gInventoryItem[playerid][i] = INVENTORY_ITEM_NONE;
        gInventoryItemQty[playerid][i] = 0;
        format(gInventoryItemMeta[playerid][i], 32, "NONE");
    }
    gInventoryWeight[playerid] = 0;
}
