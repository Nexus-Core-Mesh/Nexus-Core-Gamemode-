#if !defined CRIME_SYSTEM_INC
#define CRIME_SYSTEM_INC

#include <a_samp>

#define CRIME_TYPE_NONE 0
#define CRIME_TYPE_ROBBERY 1
#define CRIME_TYPE_DRUG_RUN 2
#define CRIME_TYPE_ARSON 3
#define CRIME_TYPE_WEAPON_SMUGGLING 4

new gCrimeHeat[MAX_PLAYERS];
new gCrimeProgress[MAX_PLAYERS];
new gPlayerCrimeState[MAX_PLAYERS];
new gLastCrimeType[MAX_PLAYERS];
new Text:gCrimeTD[MAX_PLAYERS][8];

stock CrimeSystem_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gCrimeHeat[i] = 0;
        gCrimeProgress[i] = 0;
        gPlayerCrimeState[i] = 0;
        gLastCrimeType[i] = CRIME_TYPE_NONE;
    }

    print("[CRIME] Verbrechens- und Crime-System initialisiert");
}

stock CrimeSystem_OnPlayerConnect(playerid)
{
    gCrimeHeat[playerid] = 0;
    gCrimeProgress[playerid] = 0;
    gPlayerCrimeState[playerid] = 0;
    gLastCrimeType[playerid] = CRIME_TYPE_NONE;
}

stock CrimeSystem_StartRobbery(playerid, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    new safeState = Core_GetValidStateOrDefault(stateid, Core_GetPlayerState(playerid));
    if (!Core_IsValidState(safeState))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[CRIME] Ungültiger Staatskontext für den Raubüberfall.");
        return;
    }

    gPlayerCrimeState[playerid] = 1;
    gCrimeProgress[playerid] = 25;
    gLastCrimeType[playerid] = CRIME_TYPE_ROBBERY;

    gCrimeHeat[playerid] += 5;
    if (gCrimeHeat[playerid] > 100)
    {
        gCrimeHeat[playerid] = 100;
    }

    SendClientMessage(playerid, 0xFF6666AA, "[CRIME] Raubüberfall gestartet. Die Polizei ist aufmerksam.");
}

stock CrimeSystem_AddHeat(playerid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    if (amount <= 0)
    {
        return;
    }

    gCrimeHeat[playerid] += amount;
    if (gCrimeHeat[playerid] > 100)
    {
        gCrimeHeat[playerid] = 100;
    }

    if (gCrimeHeat[playerid] >= 35)
    {
        LawSystem_AddWanted(playerid, 1);
    }
}

stock CrimeSystem_CommitCrime(playerid, type)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    gLastCrimeType[playerid] = type;
    gPlayerCrimeState[playerid] = 1;
    gCrimeProgress[playerid] = 100;

    switch (type)
    {
        case CRIME_TYPE_ROBBERY: CrimeSystem_AddHeat(playerid, 22);
        case CRIME_TYPE_DRUG_RUN: CrimeSystem_AddHeat(playerid, 18);
        case CRIME_TYPE_ARSON: CrimeSystem_AddHeat(playerid, 26);
        case CRIME_TYPE_WEAPON_SMUGGLING: CrimeSystem_AddHeat(playerid, 30);
        default: gCrimeProgress[playerid] = 0;
    }

    return true;
}

stock CrimeSystem_ShowCrimeInfo(playerid)
{
    CrimeSystem_HideCrimeInfo(playerid);

    gCrimeTD[playerid][0] = TextDrawCreate(320.0, 30.0, "CRIME TRACKER");
    TextDrawAlignment(gCrimeTD[playerid][0], 2);
    TextDrawLetterSize(gCrimeTD[playerid][0], 0.52, 1.4);
    TextDrawColor(gCrimeTD[playerid][0], 0xFF4040AA);

    new heatText[64];
    format(heatText, sizeof(heatText), "Heat: %d | Progress: %d%%", gCrimeHeat[playerid], gCrimeProgress[playerid]);
    gCrimeTD[playerid][1] = TextDrawCreate(320.0, 62.0, heatText);
    TextDrawAlignment(gCrimeTD[playerid][1], 2);
    TextDrawLetterSize(gCrimeTD[playerid][1], 0.24, 1.0);
    TextDrawColor(gCrimeTD[playerid][1], 0xFFFFFFFF);

    new typeText[64];
    format(typeText, sizeof(typeText), "Aktivitaet: %d | Staat: %d", gLastCrimeType[playerid], StateSystem_GetPlayerState(playerid));
    gCrimeTD[playerid][2] = TextDrawCreate(320.0, 86.0, typeText);
    TextDrawAlignment(gCrimeTD[playerid][2], 2);
    TextDrawLetterSize(gCrimeTD[playerid][2], 0.22, 0.9);
    TextDrawColor(gCrimeTD[playerid][2], 0xFFCC66FF);

    for (new i = 0; i < 3; i++)
    {
        if (gCrimeTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawShowForPlayer(playerid, gCrimeTD[playerid][i]);
        }
    }
}

stock CrimeSystem_HideCrimeInfo(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gCrimeTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gCrimeTD[playerid][i]);
            TextDrawDestroy(gCrimeTD[playerid][i]);
            gCrimeTD[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }
}

#endif
