#if !defined BUSINESS_SYSTEM_INC
#define BUSINESS_SYSTEM_INC

#include <a_samp>

#define MAX_BUSINESSES 30
#define BUSINESS_TYPE_NONE 0
#define BUSINESS_TYPE_BAR 1
#define BUSINESS_TYPE_SHOP 2
#define BUSINESS_TYPE_GARAGE 3
#define BUSINESS_TYPE_CLUB 4
#define BUSINESS_TYPE_CASINO 5
#define BUSINESS_TYPE_LOGISTICS 6

new gBizState[MAX_BUSINESSES];
new gBizOwner[MAX_BUSINESSES];
new gBizType[MAX_BUSINESSES];
new gBizValue[MAX_BUSINESSES];
new gBizIncome[MAX_BUSINESSES];
new gBizName[MAX_BUSINESSES][32];
new gBizTaxRate[MAX_BUSINESSES];
new gBizActive[MAX_BUSINESSES];

stock BusinessSystem_Initialize()
{
    for (new i = 0; i < MAX_BUSINESSES; i++)
    {
        gBizState[i] = -1;
        gBizOwner[i] = -1;
        gBizType[i] = BUSINESS_TYPE_NONE;
        gBizValue[i] = 0;
        gBizIncome[i] = 0;
        gBizTaxRate[i] = 10;
        gBizActive[i] = 1;
        format(gBizName[i], 32, "Unbenannt");
    }

    BusinessSystem_CreateBusiness(0, 0, BUSINESS_TYPE_SHOP, 150000, -1, "Los Santos Market");
    BusinessSystem_CreateBusiness(1, 1, BUSINESS_TYPE_BAR, 180000, -1, "San Fierro Lounge");
    BusinessSystem_CreateBusiness(2, 2, BUSINESS_TYPE_CLUB, 220000, -1, "LV Casino Lounge");
    BusinessSystem_CreateBusiness(3, 0, BUSINESS_TYPE_LOGISTICS, 260000, -1, "LS Cargo Hub");
    BusinessSystem_CreateBusiness(4, 2, BUSINESS_TYPE_CASINO, 420000, -1, "Venturas Royale");

    gBizTaxRate[0] = 8;
    gBizTaxRate[1] = 7;
    gBizTaxRate[2] = 9;
    gBizTaxRate[3] = 10;
    gBizTaxRate[4] = 12;

    print("[BIZ] Bizen und Gewerbe-System initialisiert");
}

stock BusinessSystem_CreateBusiness(bizid, stateid, type, value, ownerid, const name[])
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return false;
    }
    new safeState = Core_GetValidStateOrDefault(stateid, STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    gBizState[bizid] = safeState;
    gBizOwner[bizid] = ownerid;
    gBizType[bizid] = (type > 0) ? type : BUSINESS_TYPE_SHOP;
    gBizValue[bizid] = (value > 0) ? value : 150000;
    gBizIncome[bizid] = gBizValue[bizid] / 25;
    gBizTaxRate[bizid] = 10;
    gBizActive[bizid] = 1;
    format(gBizName[bizid], 32, "%s", name);
    return true;
}

stock BusinessSystem_BuyBusiness(playerid, bizid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return false;
    }
    if (!Core_IsValidState(gBizState[bizid]))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[BIZ] Dieses Geschäft ist keinem gültigen Staat zugeordnet.");
        return false;
    }
    if (gBizOwner[bizid] != -1)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[BIZ] Dieses Geschäft ist bereits vergeben.");
        return false;
    }
    if (gBizActive[bizid] != 1)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[BIZ] Dieses Geschäft ist momentan nicht aktiv.");
        return false;
    }

    if (gPlayerData[playerid][E_PLAYER_BANK] < gBizValue[bizid])
    {
        SendClientMessage(playerid, 0xFF0000AA, "[BIZ] Du hast nicht genug Geld auf der Bank für das Business.");
        return false;
    }

    if (!Core_ChangePlayerBank(playerid, -gBizValue[bizid]))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[BIZ] Kontostand reicht für den Kauf nicht aus.");
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(gBizState[bizid], STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        Core_ChangePlayerBank(playerid, gBizValue[bizid]);
        SendClientMessage(playerid, 0xFF0000AA, "[BIZ] Staatlicher Kontext für das Geschäft ist ungültig.");
        return false;
    }

    if (!StateLedgerSystem_Transfer(safeState, max(0, gBizValue[bizid] / 10), "business_purchase", "business_system"))
    {
        Core_ChangePlayerBank(playerid, gBizValue[bizid]);
        SendClientMessage(playerid, 0xFF0000AA, "[BIZ] Staatlicher Transfer für das Business fehlgeschlagen.");
        return false;
    }

    gBizOwner[bizid] = playerid;
    SendClientMessage(playerid, 0x00FF00AA, "[BIZ] Geschäft erfolgreich erworben.");
    return true;
}

stock BusinessSystem_SetTaxRate(bizid, rate)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return false;
    }
    if (rate < 0 || rate > 25)
    {
        return false;
    }

    gBizTaxRate[bizid] = rate;
    return true;
}

stock int:BusinessSystem_GetTaxRate(bizid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return 0;
    }
    return gBizTaxRate[bizid];
}

stock bool:BusinessSystem_IsBusinessActive(bizid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return false;
    }
    return gBizActive[bizid] == 1;
}

stock BusinessSystem_ApplyEconomyPulse(bizid, amount, const reason[])
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return false;
    }
    if (!Core_IsValidState(gBizState[bizid]))
    {
        return false;
    }
    if (amount == 0)
    {
        return true;
    }

    new safeState = Core_GetValidStateOrDefault(gBizState[bizid], STATE_LOS_SANTOS);
    new total = gBizIncome[bizid] + amount;
    if (total < 0)
    {
        return false;
    }

    gBizIncome[bizid] = total;
    if (!StateLedgerSystem_Transfer(safeState, max(0, amount / 2), reason, "business_system"))
    {
        gBizIncome[bizid] = max(0, total - amount);
        return false;
    }
    return true;
}

stock BusinessSystem_CollectIncome(bizid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return false;
    }
    if (gBizOwner[bizid] == -1)
    {
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(gBizState[bizid], STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    new income = gBizIncome[bizid];
    if (income < 0)
    {
        income = 0;
    }

    gBizIncome[bizid] = income;
    return StateLedgerSystem_Transfer(safeState, max(0, income / 4), "business_income", "business_system");
}

stock BusinessSystem_GetIncome(bizid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return 0;
    }
    return gBizIncome[bizid];
}

stock BusinessSystem_GetBusinessState(bizid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return STATE_LOS_SANTOS;
    }
    return Core_GetValidStateOrDefault(gBizState[bizid], STATE_LOS_SANTOS);
}

stock BusinessSystem_SetOwner(bizid, ownerid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return false;
    }
    if (ownerid < -1 || ownerid >= MAX_PLAYERS)
    {
        return false;
    }

    gBizOwner[bizid] = ownerid;
    return true;
}

stock BusinessSystem_SetState(bizid, stateid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(stateid, STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    gBizState[bizid] = safeState;
    return true;
}

// Zahlt das aufgelaufene Einkommen abzüglich Staatsanteil auf das Bankkonto des Besitzers aus.
stock int:BusinessSystem_PayoutToOwner(playerid, bizid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES || gBizOwner[bizid] != playerid)
    {
        return -1;
    }

    new int:income = max(0, gBizIncome[bizid]);
    new int:net = income - income / 4;
    if (income == 0 || !BusinessSystem_CollectIncome(bizid))
    {
        return 0;
    }
    if (!Core_ChangePlayerBank(playerid, net))
    {
        return -1;
    }
    gBizIncome[bizid] = 0;
    return net;
}

stock BusinessSystem_TransferIncome(bizid, amount)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return false;
    }
    if (gBizOwner[bizid] == -1)
    {
        return false;
    }
    if (amount < 0)
    {
        return false;
    }

    gBizIncome[bizid] += amount;
    return true;
}

stock int:BusinessSystem_GetOwner(bizid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return -1;
    }
    return gBizOwner[bizid];
}

stock int:BusinessSystem_GetState(bizid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return -1;
    }
    return gBizState[bizid];
}

stock BusinessSystem_ShowInfo(playerid, bizid)
{
    if (bizid < 0 || bizid >= MAX_BUSINESSES)
    {
        return;
    }

    new msg[256];
    format(msg, sizeof(msg), "[BIZ] %s | Staat: %d | Typ: %d | Wert: %d | Gewinn: %d | Besitzer: %d", gBizName[bizid], gBizState[bizid], gBizType[bizid], gBizValue[bizid], gBizIncome[bizid], gBizOwner[bizid]);
    SendClientMessage(playerid, 0x00D9FFFF, msg);
}

stock BusinessSystem_CreateBusinessAtPlayer(playerid, stateid, type, value, const name[])
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new bizid = 0;
    while (bizid < MAX_BUSINESSES && gBizOwner[bizid] != -1)
    {
        bizid++;
    }
    if (bizid >= MAX_BUSINESSES)
    {
        return false;
    }

    gBizState[bizid] = stateid;
    gBizOwner[bizid] = -1;
    gBizType[bizid] = type > 0 ? type : BUSINESS_TYPE_SHOP;
    gBizValue[bizid] = value > 0 ? value : 150000;
    gBizIncome[bizid] = gBizValue[bizid] / 25;
    gBizTaxRate[bizid] = 10;
    gBizActive[bizid] = 1;
    format(gBizName[bizid], 32, "%s", name);
    return true;
}

#endif
