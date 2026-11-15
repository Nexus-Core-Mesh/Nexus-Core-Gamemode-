#if !defined SHOP_SYSTEM_INC
#define SHOP_SYSTEM_INC

#include <a_samp>

#define MAX_SHOPS 12
#define SHOP_TYPE_GUN 1
#define SHOP_TYPE_FOOD 2
#define SHOP_TYPE_GARAGE 3
#define SHOP_TYPE_DRUG 4
#define SHOP_TYPE_SERVICE 5

new gShopState[MAX_SHOPS];
new gShopType[MAX_SHOPS];
new gShopStock[MAX_SHOPS];
new gShopPrice[MAX_SHOPS];
new gShopOpen[MAX_SHOPS];
new gShopName[MAX_SHOPS][24];
new Text:gShopTD[MAX_PLAYERS][8];

stock ShopSystem_Initialize()
{
    for (new i = 0; i < MAX_SHOPS; i++)
    {
        gShopState[i] = -1;
        gShopType[i] = 0;
        gShopStock[i] = 0;
        gShopPrice[i] = 0;
        gShopOpen[i] = 0;
        format(gShopName[i], 24, "Unbekannt");
    }

    ShopSystem_RegisterShop(0, STATE_LOS_SANTOS, SHOP_TYPE_GUN, "Cobra Arms", 15, 1500);
    ShopSystem_RegisterShop(1, STATE_LOS_SANTOS, SHOP_TYPE_FOOD, "Burger Spot", 30, 250);
    ShopSystem_RegisterShop(2, STATE_SAN_FIERRO, SHOP_TYPE_GARAGE, "SF Garage", 8, 8000);
    ShopSystem_RegisterShop(3, STATE_LAS_VENTURAS, SHOP_TYPE_SERVICE, "LV Service", 12, 1200);

    print("[SHOP] Laden- und Shop-System initialisiert");
}

stock ShopSystem_RegisterShop(shopid, stateid, type, const name[], stockCount, price)
{
    if (shopid < 0 || shopid >= MAX_SHOPS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    gShopState[shopid] = stateid;
    gShopType[shopid] = type;
    gShopStock[shopid] = stockCount;
    gShopPrice[shopid] = price;
    gShopOpen[shopid] = 1;
    format(gShopName[shopid], 24, "%s", name);
    return true;
}

stock ShopSystem_BuyItem(playerid, shopid, quantity)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (shopid < 0 || shopid >= MAX_SHOPS)
    {
        return false;
    }
    if (gShopOpen[shopid] != 1)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[SHOP] Dieses Geschäft ist derzeit geschlossen.");
        return false;
    }

    new totalCost = gShopPrice[shopid] * quantity;
    if (gShopStock[shopid] < quantity)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[SHOP] Nicht genug Lagerbestand.");
        return false;
    }

    gShopStock[shopid] -= quantity;
    SendClientMessage(playerid, 0x00FF00AA, "[SHOP] Einkauf abgeschlossen.");
    printf("[SHOP] Spieler %d kaufte %d Einheiten aus Laden %d, Kosten: %d", playerid, quantity, shopid, totalCost);
    return true;
}

stock ShopSystem_ShowShopInfo(playerid, shopid)
{
    if (shopid < 0 || shopid >= MAX_SHOPS)
    {
        return;
    }

    new msg[192];
    format(msg, sizeof(msg), "[SHOP] %s | Staat: %d | Typ: %d | Lager: %d | Preis: %d | Status: %s", gShopName[shopid], gShopState[shopid], gShopType[shopid], gShopStock[shopid], gShopPrice[shopid], (gShopOpen[shopid] == 1) ? ("offen") : ("geschlossen"));
    SendClientMessage(playerid, 0x00D9FFFF, msg);
}

stock ShopSystem_ShowStateShops(playerid, stateid)
{
    ShopSystem_HideStateShops(playerid);

    gShopTD[playerid][0] = TextDrawCreate(320.0, 30.0, "SHOP OVERVIEW");
    TextDrawAlignment(gShopTD[playerid][0], 2);
    TextDrawLetterSize(gShopTD[playerid][0], 0.52, 1.5);
    TextDrawColor(gShopTD[playerid][0], 0xFFCC66AA);

    new count = 0;
    new lineY = 60;
    for (new i = 0; i < MAX_SHOPS && count < 6; i++)
    {
        if (gShopState[i] == stateid)
        {
            new shopText[64];
            format(shopText, sizeof(shopText), "%s [%d]", gShopName[i], gShopStock[i]);
            gShopTD[playerid][1 + count] = TextDrawCreate(320.0, float(lineY), shopText);
            TextDrawAlignment(gShopTD[playerid][1 + count], 2);
            TextDrawLetterSize(gShopTD[playerid][1 + count], 0.22, 0.9);
            TextDrawColor(gShopTD[playerid][1 + count], 0xFFFFFFFF);
            lineY += 18;
            count++;
        }
    }

    for (new i = 0; i < 7; i++)
    {
        if (gShopTD[playerid][i] != Text:0)
        {
            TextDrawShowForPlayer(playerid, gShopTD[playerid][i]);
        }
    }
}

stock ShopSystem_HideStateShops(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gShopTD[playerid][i] != Text:0)
        {
            TextDrawHideForPlayer(playerid, gShopTD[playerid][i]);
            TextDrawDestroy(gShopTD[playerid][i]);
            gShopTD[playerid][i] = Text:0;
        }
    }
}

#endif
