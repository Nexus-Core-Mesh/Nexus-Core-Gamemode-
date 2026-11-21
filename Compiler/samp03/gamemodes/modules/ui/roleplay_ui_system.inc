#if !defined ROLEPLAY_UI_SYSTEM_INC
#define ROLEPLAY_UI_SYSTEM_INC

#include <a_samp>

#define RP_UI_MAX_LINES 10

new Text:gRoleplayUIDraw[MAX_PLAYERS][RP_UI_MAX_LINES];
new gRoleplayUIVisible[MAX_PLAYERS];

stock RoleplayUI_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gRoleplayUIVisible[i] = 0;
    }

    print("[RP_UI] Ingame-Textdraw Dashboard initialisiert");
}

stock RoleplayUI_Hide(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    for (new i = 0; i < RP_UI_MAX_LINES; i++)
    {
        if (gRoleplayUIDraw[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gRoleplayUIDraw[playerid][i]);
            TextDrawDestroy(gRoleplayUIDraw[playerid][i]);
            gRoleplayUIDraw[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }

    gRoleplayUIVisible[playerid] = 0;
}

stock RoleplayUI_ShowDashboard(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    RoleplayUI_Hide(playerid);

    new playerName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, playerName, sizeof(playerName));

    new stateName[32];
    StateSystem_GetStateName(StateSystem_GetPlayerState(playerid), stateName, sizeof(stateName));

    new header[64];
    format(header, sizeof(header), "NEXUS CORE | %s", playerName);
    gRoleplayUIDraw[playerid][0] = TextDrawCreate(15.0, 20.0, header);
    TextDrawAlignment(gRoleplayUIDraw[playerid][0], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][0], 0.32, 1.1);
    TextDrawColor(gRoleplayUIDraw[playerid][0], 0x33CCFFFF);

    new stateText[64];
    format(stateText, sizeof(stateText), "Staat: %s | Kasse: %d", stateName, StateSystem_GetBankCash(StateSystem_GetPlayerState(playerid)));
    gRoleplayUIDraw[playerid][1] = TextDrawCreate(15.0, 40.0, stateText);
    TextDrawAlignment(gRoleplayUIDraw[playerid][1], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][1], 0.22, 0.9);
    TextDrawColor(gRoleplayUIDraw[playerid][1], 0xFFFFFFFF);

    new progressText[64];
    format(progressText, sizeof(progressText), "Level: %d | XP: %d | Rep: %d", gPlayerData[playerid][E_PLAYER_LEVEL], gPlayerData[playerid][E_PLAYER_XP], gPlayerData[playerid][E_PLAYER_REP]);
    gRoleplayUIDraw[playerid][2] = TextDrawCreate(15.0, 58.0, progressText);
    TextDrawAlignment(gRoleplayUIDraw[playerid][2], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][2], 0.22, 0.9);
    TextDrawColor(gRoleplayUIDraw[playerid][2], 0x99FF99AA);

    new wantedText[48];
    format(wantedText, sizeof(wantedText), "Wanted: %d | Jail: %d", gPlayerData[playerid][E_PLAYER_WANTED], gPlayerData[playerid][E_PLAYER_JAIL_TIME]);
    gRoleplayUIDraw[playerid][3] = TextDrawCreate(15.0, 76.0, wantedText);
    TextDrawAlignment(gRoleplayUIDraw[playerid][3], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][3], 0.22, 0.9);
    TextDrawColor(gRoleplayUIDraw[playerid][3], 0xFF6666AA);

    new adminText[48];
    format(adminText, sizeof(adminText), "Admin: %d | StaatAdmin: %d", gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL], gPlayerData[playerid][E_PLAYER_ADMIN_STATE]);
    gRoleplayUIDraw[playerid][4] = TextDrawCreate(15.0, 94.0, adminText);
    TextDrawAlignment(gRoleplayUIDraw[playerid][4], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][4], 0.22, 0.9);
    TextDrawColor(gRoleplayUIDraw[playerid][4], 0x33CCFFAA);

    new moneyText[48];
    format(moneyText, sizeof(moneyText), "Bank: $%d | Cash: $%d", gPlayerData[playerid][E_PLAYER_BANK], GetPlayerMoney(playerid));
    gRoleplayUIDraw[playerid][5] = TextDrawCreate(15.0, 112.0, moneyText);
    TextDrawAlignment(gRoleplayUIDraw[playerid][5], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][5], 0.22, 0.9);
    TextDrawColor(gRoleplayUIDraw[playerid][5], 0xFFCC66AA);

    gRoleplayUIDraw[playerid][6] = TextDrawCreate(15.0, 130.0, "Y: Menue  N: Text-Wars  /menu /jobs /statemenu");
    TextDrawAlignment(gRoleplayUIDraw[playerid][6], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][6], 0.18, 0.75);
    TextDrawColor(gRoleplayUIDraw[playerid][6], 0xFFFFFFFF);

    gRoleplayUIDraw[playerid][7] = TextDrawCreate(15.0, 145.0, "/mapmenu /hud /wanted /stats /missions");
    TextDrawAlignment(gRoleplayUIDraw[playerid][7], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][7], 0.18, 0.75);
    TextDrawColor(gRoleplayUIDraw[playerid][7], 0xFFFFFFFF);

    gRoleplayUIDraw[playerid][8] = TextDrawCreate(15.0, 160.0, "Modular RP | 3 States | 1 Admin Layer");
    TextDrawAlignment(gRoleplayUIDraw[playerid][8], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][8], 0.18, 0.75);
    TextDrawColor(gRoleplayUIDraw[playerid][8], 0x8CC6FFFF);

    for (new i = 0; i < 9; i++)
    {
        if (gRoleplayUIDraw[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawShowForPlayer(playerid, gRoleplayUIDraw[playerid][i]);
        }
    }

    gRoleplayUIVisible[playerid] = 1;
}

stock RoleplayUI_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
    {
        return false;
    }

    new cmd[32];
    new idx = sscanf(cmdtext, "s[32]", cmd);
    if (idx == 0)
    {
        return false;
    }

    if (strcmp(cmd, "/hud", true) == 0 || strcmp(cmd, "/dashboard", true) == 0)
    {
        RoleplayUI_ShowDashboard(playerid);
        return true;
    }

    if (strcmp(cmd, "/hudhide", true) == 0)
    {
        RoleplayUI_Hide(playerid);
        return true;
    }

    return false;
}

#endif
