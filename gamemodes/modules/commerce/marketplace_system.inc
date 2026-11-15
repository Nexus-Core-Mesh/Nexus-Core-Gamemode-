#if !defined MARKETPLACE_SYSTEM_INC
#define MARKETPLACE_SYSTEM_INC

#include <a_samp>

#define MAX_MARKETPLACE_ITEMS 30

new gMarketplaceState[MAX_MARKETPLACE_ITEMS];
new gMarketplaceItem[MAX_MARKETPLACE_ITEMS][40];
new gMarketplacePrice[MAX_MARKETPLACE_ITEMS];
new gMarketplaceSupply[MAX_MARKETPLACE_ITEMS];
new gMarketplaceCount = 0;

stock MarketplaceSystem_Initialize()
{
    gMarketplaceCount = 0;
    for (new i = 0; i < MAX_MARKETPLACE_ITEMS; i++)
    {
        gMarketplaceState[i] = -1;
        format(gMarketplaceItem[i], 40, "");
        gMarketplacePrice[i] = 0;
        gMarketplaceSupply[i] = 0;
    }

    MarketplaceSystem_RegisterItem(STATE_LOS_SANTOS, "Mauerbau", 1200, 15);
    MarketplaceSystem_RegisterItem(STATE_SAN_FIERRO, "Elektronik", 1800, 12);
    MarketplaceSystem_RegisterItem(STATE_LAS_VENTURAS, "Luxuswaren", 2600, 9);

    print("[MARKETPLACE] Handelsplattform und Konsumgüter initialisiert");
}

stock MarketplaceSystem_RegisterItem(stateid, const item[], price, supply)
{
    if (!Core_IsValidState(stateid) || gMarketplaceCount >= MAX_MARKETPLACE_ITEMS)
    {
        return false;
    }

    new idx = gMarketplaceCount;
    gMarketplaceState[idx] = stateid;
    format(gMarketplaceItem[idx], 40, "%s", item);
    gMarketplacePrice[idx] = price;
    gMarketplaceSupply[idx] = supply;
    gMarketplaceCount++;
    return true;
}

stock MarketplaceSystem_Show(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[MARKETPLACE] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new msg[256];
    format(msg, sizeof(msg), "[MARKETPLACE] %s | Angebot: %d", stateName, gMarketplaceCount);
    SendClientMessage(playerid, 0x00D9FFFF, msg);

    for (new i = 0; i < gMarketplaceCount; i++)
    {
        if (gMarketplaceState[i] == stateid)
        {
            new line[180];
            format(line, sizeof(line), "- %s | Preis: $%d | Vorrat: %d", gMarketplaceItem[i], gMarketplacePrice[i], gMarketplaceSupply[i]);
            SendClientMessage(playerid, 0xFFFFFFFF, line);
        }
    }
}

#endif
