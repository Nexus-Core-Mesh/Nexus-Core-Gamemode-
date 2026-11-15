#if !defined CONTRABAND_SYSTEM_INC
#define CONTRABAND_SYSTEM_INC

#include <a_samp>

#define MAX_CONTRABAND_ITEMS 12

new gContrabandState[MAX_CONTRABAND_ITEMS];
new gContrabandName[MAX_CONTRABAND_ITEMS][32];
new gContrabandPrice[MAX_CONTRABAND_ITEMS];
new gContrabandRisk[MAX_CONTRABAND_ITEMS];
new gContrabandCount = 0;

stock ContrabandSystem_Initialize()
{
    gContrabandCount = 0;
    for (new i = 0; i < MAX_CONTRABAND_ITEMS; i++)
    {
        gContrabandState[i] = -1;
        gContrabandPrice[i] = 0;
        gContrabandRisk[i] = 0;
        format(gContrabandName[i], 32, "");
    }

    ContrabandSystem_RegisterItem(STATE_LOS_SANTOS, "Stolen Goods", 1800, 65);
    ContrabandSystem_RegisterItem(STATE_SAN_FIERRO, "Smugglers Supply", 2200, 70);
    ContrabandSystem_RegisterItem(STATE_LAS_VENTURAS, "Luxury Smuggling", 2800, 78);

    print("[CONTRABAND] Black-market logistics initialized");
}

stock ContrabandSystem_RegisterItem(stateid, const name[], price, risk)
{
    if (!Core_IsValidState(stateid) || gContrabandCount >= MAX_CONTRABAND_ITEMS)
    {
        return false;
    }

    gContrabandState[gContrabandCount] = stateid;
    format(gContrabandName[gContrabandCount], 32, "%s", name);
    gContrabandPrice[gContrabandCount] = price;
    gContrabandRisk[gContrabandCount] = risk;
    gContrabandCount++;
    return true;
}

stock ContrabandSystem_Show(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[CONTRABAND] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new message[256];
    format(message, sizeof(message), "[CONTRABAND] %s | Items: %d", stateName, gContrabandCount);
    SendClientMessage(playerid, 0x00D9FFFF, message);

    for (new i = 0; i < gContrabandCount; i++)
    {
        if (gContrabandState[i] == stateid)
        {
            new line[180];
            format(line, sizeof(line), "- %s | Price: $%d | Risk: %d%%", gContrabandName[i], gContrabandPrice[i], gContrabandRisk[i]);
            SendClientMessage(playerid, 0xFFFFFFFF, line);
        }
    }
}

stock ContrabandSystem_Trade(playerid, stateid, itemIndex, quantity)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidState(stateid) || itemIndex < 0 || itemIndex >= gContrabandCount)
    {
        return false;
    }

    if (gContrabandState[itemIndex] != stateid)
    {
        return false;
    }

    new totalCost = gContrabandPrice[itemIndex] * quantity;
    if (!Core_ChangePlayerBank(playerid, -totalCost))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[CONTRABAND] Du hast nicht genug Geld.");
        return false;
    }

    new stateIncome = totalCost / 2;
    StateSystem_ApplyTreasuryDelta(stateid, stateIncome);
    SendClientMessage(playerid, 0x00FF00AA, "[CONTRABAND] Handel abgeschlossen.");
    return true;
}

#endif
