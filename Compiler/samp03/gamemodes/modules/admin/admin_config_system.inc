#if !defined ADMIN_CONFIG_SYSTEM_INC
#define ADMIN_CONFIG_SYSTEM_INC

#include <a_samp>

new gAdminConfigStateEnabled[3];
new gAdminConfigStateBudget[3];
new gAdminConfigStateTax[3];
new gAdminConfigGangWarEnabled = 1;
new Text:gAdminConfigTD[MAX_PLAYERS][8];

stock AdminConfigSystem_Initialize()
{
    gAdminConfigStateEnabled[0] = 1;
    gAdminConfigStateEnabled[1] = 1;
    gAdminConfigStateEnabled[2] = 1;

    gAdminConfigStateBudget[0] = 500000;
    gAdminConfigStateBudget[1] = 450000;
    gAdminConfigStateBudget[2] = 400000;

    gAdminConfigStateTax[0] = 8;
    gAdminConfigStateTax[1] = 7;
    gAdminConfigStateTax[2] = 9;

    print("[ADMIN_CFG] Ingame-Konfigurationssystem initialisiert");
}

stock AdminConfigSystem_IsEnabled(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    return gAdminConfigStateEnabled[stateid] == 1;
}

stock AdminConfigSystem_SetBudget(stateid, budget)
{
    if (!Core_IsValidState(stateid))
    {
        return;
    }
    gAdminConfigStateBudget[stateid] = budget;
    StateSystem_SetTreasuryBalance(stateid, budget);
}

stock AdminConfigSystem_SetStateTax(stateid, tax)
{
    if (!Core_IsValidState(stateid))
    {
        return;
    }
    gAdminConfigStateTax[stateid] = tax;
    gStateTaxRate[stateid] = tax;
}

stock AdminConfigSystem_ToggleState(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return;
    }
    gAdminConfigStateEnabled[stateid] = 1 - gAdminConfigStateEnabled[stateid];
}

stock AdminConfigSystem_ToggleGangWar()
{
    gAdminConfigGangWarEnabled = 1 - gAdminConfigGangWarEnabled;
}

stock AdminConfigSystem_ShowMenu(playerid)
{
    AdminConfigSystem_HideMenu(playerid);

    gAdminConfigTD[playerid][0] = TextDrawCreate(320.0, 28.0, "ADMIN CONFIG");
    TextDrawAlignment(gAdminConfigTD[playerid][0], 2);
    TextDrawLetterSize(gAdminConfigTD[playerid][0], 0.55, 1.6);
    TextDrawColor(gAdminConfigTD[playerid][0], 0xFFD700FF);

    gAdminConfigTD[playerid][1] = TextDrawCreate(320.0, 52.0, "LS / SF / LV States");
    TextDrawAlignment(gAdminConfigTD[playerid][1], 2);
    TextDrawLetterSize(gAdminConfigTD[playerid][1], 0.24, 1.0);
    TextDrawColor(gAdminConfigTD[playerid][1], 0xFFFFFFFF);

    new stateText[128];
    format(stateText, sizeof(stateText), "LS:%d/%d | SF:%d/%d | LV:%d/%d", gAdminConfigStateEnabled[0], gAdminConfigStateBudget[0], gAdminConfigStateEnabled[1], gAdminConfigStateBudget[1], gAdminConfigStateEnabled[2], gAdminConfigStateBudget[2]);
    gAdminConfigTD[playerid][2] = TextDrawCreate(320.0, 74.0, stateText);
    TextDrawAlignment(gAdminConfigTD[playerid][2], 2);
    TextDrawLetterSize(gAdminConfigTD[playerid][2], 0.22, 1.0);
    TextDrawColor(gAdminConfigTD[playerid][2], 0x77FF77FF);

    new taxText[128];
    format(taxText, sizeof(taxText), "Steuern: LS %d%% | SF %d%% | LV %d%%", gAdminConfigStateTax[0], gAdminConfigStateTax[1], gAdminConfigStateTax[2]);
    gAdminConfigTD[playerid][3] = TextDrawCreate(320.0, 94.0, taxText);
    TextDrawAlignment(gAdminConfigTD[playerid][3], 2);
    TextDrawLetterSize(gAdminConfigTD[playerid][3], 0.22, 1.0);
    TextDrawColor(gAdminConfigTD[playerid][3], 0x80D4FFFF);

    new gangWarText[64];
    format(gangWarText, sizeof(gangWarText), "Gangwar aktiv: %s", (gAdminConfigGangWarEnabled == 1) ? ("Ja") : ("Nein"));
    gAdminConfigTD[playerid][4] = TextDrawCreate(320.0, 116.0, gangWarText);
    TextDrawAlignment(gAdminConfigTD[playerid][4], 2);
    TextDrawLetterSize(gAdminConfigTD[playerid][4], 0.22, 1.0);
    TextDrawColor(gAdminConfigTD[playerid][4], 0xFF9999FF);

    gAdminConfigTD[playerid][5] = TextDrawCreate(320.0, 138.0, "Befehle: /togglestate /togglewar /setbudget /settax");
    TextDrawAlignment(gAdminConfigTD[playerid][5], 2);
    TextDrawLetterSize(gAdminConfigTD[playerid][5], 0.20, 0.9);
    TextDrawColor(gAdminConfigTD[playerid][5], 0xFFFFFFFF);

    gAdminConfigTD[playerid][6] = TextDrawCreate(320.0, 160.0, "Beispiel: /setbudget 0 600000");
    TextDrawAlignment(gAdminConfigTD[playerid][6], 2);
    TextDrawLetterSize(gAdminConfigTD[playerid][6], 0.20, 0.9);
    TextDrawColor(gAdminConfigTD[playerid][6], 0xFFFFFFFF);

    gAdminConfigTD[playerid][7] = TextDrawCreate(320.0, 182.0, "Konfiguration live via Adminsystem");
    TextDrawAlignment(gAdminConfigTD[playerid][7], 2);
    TextDrawLetterSize(gAdminConfigTD[playerid][7], 0.18, 0.8);
    TextDrawColor(gAdminConfigTD[playerid][7], 0xBBBBBBFF);

    for (new i = 0; i < 8; i++)
    {
        TextDrawShowForPlayer(playerid, gAdminConfigTD[playerid][i]);
    }
}

stock AdminConfigSystem_HideMenu(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gAdminConfigTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gAdminConfigTD[playerid][i]);
            TextDrawDestroy(gAdminConfigTD[playerid][i]);
            gAdminConfigTD[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }
}

#endif
