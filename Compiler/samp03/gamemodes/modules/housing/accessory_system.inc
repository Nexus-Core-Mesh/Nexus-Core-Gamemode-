#if !defined ACCESSORY_SYSTEM_INC
#define ACCESSORY_SYSTEM_INC

#include <a_samp>

#define MAX_PLAYER_ACCESSORIES 8
#define ACCESSORY_NONE 0
#define ACCESSORY_HAT 1
#define ACCESSORY_GLASSES 2
#define ACCESSORY_WATCH 3
#define ACCESSORY_CHAIN 4
#define ACCESSORY_BACKPACK 5

new gAccessoryItem[MAX_PLAYERS][MAX_PLAYER_ACCESSORIES];
new gAccessoryColor[MAX_PLAYERS][MAX_PLAYER_ACCESSORIES];
new gAccessoryLevel[MAX_PLAYERS];

stock AccessorySystem_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gAccessoryLevel[i] = 0;
        for (new a = 0; a < MAX_PLAYER_ACCESSORIES; a++)
        {
            gAccessoryItem[i][a] = ACCESSORY_NONE;
            gAccessoryColor[i][a] = 0;
        }
    }
    DB_EnsureAccessorySchema();
    print("[ACCESSORY] Accessoire-/Fashion-System initialisiert");
}

stock AccessorySystem_Equip(playerid, accessoryType, color)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (accessoryType < ACCESSORY_NONE || accessoryType > ACCESSORY_BACKPACK)
    {
        return false;
    }

    for (new i = 0; i < MAX_PLAYER_ACCESSORIES; i++)
    {
        if (gAccessoryItem[playerid][i] == ACCESSORY_NONE)
        {
            gAccessoryItem[playerid][i] = accessoryType;
            gAccessoryColor[playerid][i] = color;
            gAccessoryLevel[playerid]++;
            DB_SaveAccessory(playerid, accessoryType, color, 1);
            return true;
        }
    }

    return false;
}

stock AccessorySystem_UnEquip(playerid, accessoryType)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    for (new i = 0; i < MAX_PLAYER_ACCESSORIES; i++)
    {
        if (gAccessoryItem[playerid][i] == accessoryType)
        {
            gAccessoryItem[playerid][i] = ACCESSORY_NONE;
            gAccessoryColor[playerid][i] = 0;
            if (gAccessoryLevel[playerid] > 0)
            {
                gAccessoryLevel[playerid]--;
            }
            DB_SaveAccessory(playerid, accessoryType, 0, 0);
            return true;
        }
    }

    return false;
}

stock int:AccessorySystem_GetCount(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gAccessoryLevel[playerid];
}

stock AccessorySystem_ShowInventory(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    new msg[128];
    format(msg, sizeof(msg), "[ACCESSORY] Anzahl: %d | slots: %d", gAccessoryLevel[playerid], MAX_PLAYER_ACCESSORIES);
    SendClientMessage(playerid, 0xFF99AAFF, msg);
}

#endif
