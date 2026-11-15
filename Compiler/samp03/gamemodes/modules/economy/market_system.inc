#if !defined MARKET_SYSTEM_INC
#define MARKET_SYSTEM_INC

#include <a_samp>

#define MAX_MARKET_ITEMS 12

new gMarketItemCount[STATE_COUNT];
new gMarketItemName[STATE_COUNT][MAX_MARKET_ITEMS][32];
new gMarketItemPrice[STATE_COUNT][MAX_MARKET_ITEMS];
new gMarketItemStock[STATE_COUNT][MAX_MARKET_ITEMS];

stock MarketSystem_Initialize()
{
    for (new stateid = 0; stateid < STATE_COUNT; stateid++)
    {
        gMarketItemCount[stateid] = 0;
        for (new i = 0; i < MAX_MARKET_ITEMS; i++)
        {
            format(gMarketItemName[stateid][i], 32, "");
            gMarketItemPrice[stateid][i] = 0;
            gMarketItemStock[stateid][i] = 0;
        }
    }

    MarketSystem_RegisterItem(STATE_LOS_SANTOS, "Groceries", 250, 50);
    MarketSystem_RegisterItem(STATE_LOS_SANTOS, "Fuel Canister", 300, 30);
    MarketSystem_RegisterItem(STATE_LOS_SANTOS, "Repair Kit", 800, 12);

    MarketSystem_RegisterItem(STATE_SAN_FIERRO, "Electronics", 450, 20);
    MarketSystem_RegisterItem(STATE_SAN_FIERRO, "Construction Pack", 600, 18);
    MarketSystem_RegisterItem(STATE_SAN_FIERRO, "Medical Supply", 420, 25);

    MarketSystem_RegisterItem(STATE_LAS_VENTURAS, "Luxury Goods", 700, 10);
    MarketSystem_RegisterItem(STATE_LAS_VENTURAS, "Tourism Kit", 550, 22);
    MarketSystem_RegisterItem(STATE_LAS_VENTURAS, "Casino Chips", 990, 15);

    print("[MARKET] State Market System initialisiert");
}

stock MarketSystem_RegisterItem(stateid, const name[], price, itemStock)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    if (gMarketItemCount[stateid] >= MAX_MARKET_ITEMS)
    {
        return false;
    }

    new idx = gMarketItemCount[stateid];
    format(gMarketItemName[stateid][idx], 32, "%s", name);
    gMarketItemPrice[stateid][idx] = price;
    gMarketItemStock[stateid][idx] = itemStock;
    gMarketItemCount[stateid]++;
    return true;
}

stock int:MarketSystem_GetItemPrice(stateid, index)
{
    if (!Core_IsValidState(stateid) || index < 0 || index >= gMarketItemCount[stateid])
    {
        return 0;
    }
    return gMarketItemPrice[stateid][index];
}

stock int:MarketSystem_GetItemStock(stateid, index)
{
    if (!Core_IsValidState(stateid) || index < 0 || index >= gMarketItemCount[stateid])
    {
        return 0;
    }
    return gMarketItemStock[stateid][index];
}

stock MarketSystem_BuyItem(playerid, stateid, itemIndex)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidState(stateid))
    {
        return false;
    }

    if (itemIndex < 0 || itemIndex >= gMarketItemCount[stateid])
    {
        return false;
    }

    new itemPrice = gMarketItemPrice[stateid][itemIndex];
    if (Core_GetPlayerBank(playerid) < itemPrice)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[MARKET] Nicht genug Geld auf der Bank.");
        return false;
    }

    if (gMarketItemStock[stateid][itemIndex] <= 0)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[MARKET] Dieser Artikel ist momentan ausverkauft.");
        return false;
    }

    if (!Core_ChangePlayerBank(playerid, -itemPrice))
    {
        return false;
    }

    gMarketItemStock[stateid][itemIndex]--;
    StateSystem_ApplyTreasuryDelta(stateid, itemPrice / 4);
    SendClientMessage(playerid, 0x00FF00AA, "[MARKET] Einkauf erfolgreich.");
    return true;
}

stock MarketSystem_ShowMarket(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[MARKET] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new message[256];
    format(message, sizeof(message), "[MARKET] %s Markt | Bank: $%d", stateName, Core_GetPlayerBank(playerid));
    SendClientMessage(playerid, 0x00D9FFFF, message);

    for (new i = 0; i < gMarketItemCount[stateid]; i++)
    {
        new itemText[128];
        format(itemText, sizeof(itemText), "[%d] %s | Preis: $%d | Lager: %d", i, gMarketItemName[stateid][i], gMarketItemPrice[stateid][i], gMarketItemStock[stateid][i]);
        SendClientMessage(playerid, 0xFFFFFFFF, itemText);
    }
}

#endif
