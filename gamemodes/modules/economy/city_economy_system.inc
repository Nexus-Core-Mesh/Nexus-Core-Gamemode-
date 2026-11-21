#if !defined CITY_ECONOMY_SYSTEM_INC
#define CITY_ECONOMY_SYSTEM_INC

#include <a_samp>

#define MAX_CITY_ECONOMY_STATES 3
#define ECONOMY_TYPE_TAX 1
#define ECONOMY_TYPE_SHOP 2
#define ECONOMY_TYPE_LABOR 3
#define ECONOMY_TYPE_DRUG 4

new gCityEconomyValue[MAX_CITY_ECONOMY_STATES];
new gCityTaxIncome[MAX_CITY_ECONOMY_STATES];
new gCityTradeVolume[MAX_CITY_ECONOMY_STATES];
new gCityResourceIndex[MAX_CITY_ECONOMY_STATES];
new Text:gCityEconomyTD[MAX_PLAYERS][8];

stock CityEconomy_Initialize()
{
    gCityEconomyValue[STATE_LOS_SANTOS] = 1000000;
    gCityEconomyValue[STATE_SAN_FIERRO] = 900000;
    gCityEconomyValue[STATE_LAS_VENTURAS] = 1100000;

    gCityTaxIncome[STATE_LOS_SANTOS] = 120000;
    gCityTaxIncome[STATE_SAN_FIERRO] = 110000;
    gCityTaxIncome[STATE_LAS_VENTURAS] = 135000;

    gCityTradeVolume[STATE_LOS_SANTOS] = 250000;
    gCityTradeVolume[STATE_SAN_FIERRO] = 220000;
    gCityTradeVolume[STATE_LAS_VENTURAS] = 280000;

    gCityResourceIndex[STATE_LOS_SANTOS] = 1;
    gCityResourceIndex[STATE_SAN_FIERRO] = 2;
    gCityResourceIndex[STATE_LAS_VENTURAS] = 3;

    print("[CITY_ECONOMY] Dynamische Stadtwirtschaft initialisiert");
}

stock CityEconomy_Tick()
{
    for (new i = 0; i < MAX_CITY_ECONOMY_STATES; i++)
    {
        gCityEconomyValue[i] += 2500 + (i * 800);
        gCityTradeVolume[i] += 750 + (i * 200);
    }
}

stock CityEconomy_ConsumeTax(stateid, amount)
{
    if (stateid < 0 || stateid >= MAX_CITY_ECONOMY_STATES)
    {
        return false;
    }
    if (amount < 0)
    {
        return false;
    }

    gCityTaxIncome[stateid] += amount;
    gCityEconomyValue[stateid] += amount;
    StateSystem_ApplyTreasuryDelta(stateid, amount);
    return true;
}

stock int:CityEconomy_GetValue(stateid)
{
    if (stateid < 0 || stateid >= MAX_CITY_ECONOMY_STATES)
    {
        return 0;
    }
    return gCityEconomyValue[stateid];
}

stock int:CityEconomy_GetTaxIncome(stateid)
{
    if (stateid < 0 || stateid >= MAX_CITY_ECONOMY_STATES)
    {
        return 0;
    }
    return gCityTaxIncome[stateid];
}

stock CityEconomy_ShowOverview(playerid)
{
    CityEconomy_HideOverview(playerid);

    gCityEconomyTD[playerid][0] = TextDrawCreate(320.0, 28.0, "CITY ECONOMY");
    TextDrawAlignment(gCityEconomyTD[playerid][0], 2);
    TextDrawLetterSize(gCityEconomyTD[playerid][0], 0.55, 1.5);
    TextDrawColor(gCityEconomyTD[playerid][0], 0x00CCFFAA);

    new lsText[64];
    format(lsText, sizeof(lsText), "LS: $%d | Tax: $%d", gCityEconomyValue[STATE_LOS_SANTOS], gCityTaxIncome[STATE_LOS_SANTOS]);
    gCityEconomyTD[playerid][1] = TextDrawCreate(320.0, 58.0, lsText);
    TextDrawAlignment(gCityEconomyTD[playerid][1], 2);
    TextDrawLetterSize(gCityEconomyTD[playerid][1], 0.22, 0.9);
    TextDrawColor(gCityEconomyTD[playerid][1], 0xFFFFFFFF);

    new sfText[64];
    format(sfText, sizeof(sfText), "SF: $%d | Tax: $%d", gCityEconomyValue[STATE_SAN_FIERRO], gCityTaxIncome[STATE_SAN_FIERRO]);
    gCityEconomyTD[playerid][2] = TextDrawCreate(320.0, 82.0, sfText);
    TextDrawAlignment(gCityEconomyTD[playerid][2], 2);
    TextDrawLetterSize(gCityEconomyTD[playerid][2], 0.22, 0.9);
    TextDrawColor(gCityEconomyTD[playerid][2], 0xFFFFFFFF);

    new lvText[64];
    format(lvText, sizeof(lvText), "LV: $%d | Tax: $%d", gCityEconomyValue[STATE_LAS_VENTURAS], gCityTaxIncome[STATE_LAS_VENTURAS]);
    gCityEconomyTD[playerid][3] = TextDrawCreate(320.0, 106.0, lvText);
    TextDrawAlignment(gCityEconomyTD[playerid][3], 2);
    TextDrawLetterSize(gCityEconomyTD[playerid][3], 0.22, 0.9);
    TextDrawColor(gCityEconomyTD[playerid][3], 0xFFFFFFFF);

    for (new i = 0; i < 4; i++)
    {
        if (gCityEconomyTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawShowForPlayer(playerid, gCityEconomyTD[playerid][i]);
        }
    }
}

stock CityEconomy_HideOverview(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gCityEconomyTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gCityEconomyTD[playerid][i]);
            TextDrawDestroy(gCityEconomyTD[playerid][i]);
            gCityEconomyTD[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }
}

#endif
