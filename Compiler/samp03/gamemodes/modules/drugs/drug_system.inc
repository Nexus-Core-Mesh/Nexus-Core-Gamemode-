#if !defined DRUG_SYSTEM_INC
#define DRUG_SYSTEM_INC

#include <a_samp>

#define MAX_DRUG_TYPES 5
#define DRUG_TYPE_WEED 0
#define DRUG_TYPE_COCAINE 1
#define DRUG_TYPE_METH 2
#define DRUG_TYPE_ECSTASY 3
#define DRUG_TYPE_OPIUM 4

new gDrugStock[3][MAX_DRUG_TYPES];
new gDrugPrice[3][MAX_DRUG_TYPES];
new gPlayerDrugInventory[MAX_PLAYERS][MAX_DRUG_TYPES];
new gDrugMarketActive = 1;
new Text:gDrugTD[MAX_PLAYERS][8];

stock DrugSystem_Initialize()
{
    for (new stateIndex = 0; stateIndex < 3; stateIndex++)
    {
        for (new typeIndex = 0; typeIndex < MAX_DRUG_TYPES; typeIndex++)
        {
            gDrugStock[stateIndex][typeIndex] = 25 + (stateIndex * 5) + (typeIndex * 3);
            gDrugPrice[stateIndex][typeIndex] = 500 + (typeIndex * 250) + (stateIndex * 100);
        }
    }

    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        for (new typeIndex = 0; typeIndex < MAX_DRUG_TYPES; typeIndex++)
        {
            gPlayerDrugInventory[i][typeIndex] = 0;
        }
    }

    print("[DRUGS] Drogenhandel und illegaler Markt initialisiert");
}

stock DrugSystem_BuyDrug(playerid, stateid, type, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (type < 0 || type >= MAX_DRUG_TYPES)
    {
        return false;
    }
    if (amount <= 0)
    {
        return false;
    }
    if (gDrugMarketActive != 1)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[DRUGS] Der Drogenticker ist derzeit deaktiviert.");
        return false;
    }

    if (gDrugStock[stateid][type] < amount)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[DRUGS] Nicht genug Vorrat im Markt.");
        return false;
    }

    gDrugStock[stateid][type] -= amount;
    gPlayerDrugInventory[playerid][type] += amount;
    CrimeSystem_AddHeat(playerid, 6 + amount);
    SendClientMessage(playerid, 0x00FF00AA, "[DRUGS] Drogen erworben.");
    return true;
}

stock int:DrugSystem_GetInventory(playerid, type)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    if (type < 0 || type >= MAX_DRUG_TYPES)
    {
        return 0;
    }
    return gPlayerDrugInventory[playerid][type];
}

stock DrugSystem_ShowMarket(playerid, stateid)
{
    DrugSystem_HideMarket(playerid);

    gDrugTD[playerid][0] = TextDrawCreate(320.0, 28.0, "DRUG MARKET");
    TextDrawAlignment(gDrugTD[playerid][0], 2);
    TextDrawLetterSize(gDrugTD[playerid][0], 0.55, 1.5);
    TextDrawColor(gDrugTD[playerid][0], 0x66FF66AA);

    new lineY = 60;
    for (new i = 0; i < MAX_DRUG_TYPES && i < 5; i++)
    {
        new drugText[64];
        format(drugText, sizeof(drugText), "%d: %d x $%d", i, gDrugStock[stateid][i], gDrugPrice[stateid][i]);
        gDrugTD[playerid][1 + i] = TextDrawCreate(320.0, float(lineY), drugText);
        TextDrawAlignment(gDrugTD[playerid][1 + i], 2);
        TextDrawLetterSize(gDrugTD[playerid][1 + i], 0.22, 0.8);
        TextDrawColor(gDrugTD[playerid][1 + i], 0xFFFFFFFF);
        lineY += 18;
    }

    for (new i = 0; i < 6; i++)
    {
        if (gDrugTD[playerid][i] != Text:0)
        {
            TextDrawShowForPlayer(playerid, gDrugTD[playerid][i]);
        }
    }
}

stock DrugSystem_HideMarket(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gDrugTD[playerid][i] != Text:0)
        {
            TextDrawHideForPlayer(playerid, gDrugTD[playerid][i]);
            TextDrawDestroy(gDrugTD[playerid][i]);
            gDrugTD[playerid][i] = Text:0;
        }
    }
}

#endif
