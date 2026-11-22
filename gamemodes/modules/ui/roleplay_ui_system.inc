#if !defined ROLEPLAY_UI_SYSTEM_INC
#define ROLEPLAY_UI_SYSTEM_INC

#include <a_samp>

#define RP_UI_MAX_LINES 10

new Text:gRoleplayUIDraw[MAX_PLAYERS][RP_UI_MAX_LINES];
new gRoleplayUIVisible[MAX_PLAYERS];

stock RoleplayUI_ExtractCommandToken(const cmdtext[], output[], outLen)
{
    new idx = 0;
    while (cmdtext[idx] != '\0' && cmdtext[idx] != ' ' && cmdtext[idx] != '\t' && cmdtext[idx] != '\r' && cmdtext[idx] != '\n')
    {
        if (idx + 1 >= outLen)
        {
            break;
        }
        output[idx] = cmdtext[idx];
        idx++;
    }
    output[idx] = '\0';
    return idx;
}

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

    new playerHeader[96];
    format(playerHeader, sizeof(playerHeader), "NEXUS CORE | %s", playerName);

    gRoleplayUIDraw[playerid][0] = TextDrawCreate(12.0, 18.0, "_");
    TextDrawUseBox(gRoleplayUIDraw[playerid][0], 1);
    TextDrawBoxColor(gRoleplayUIDraw[playerid][0], 0x111111BB);
    TextDrawTextSize(gRoleplayUIDraw[playerid][0], 200.0, 0.0);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][0], 0.0, 8.0);

    gRoleplayUIDraw[playerid][1] = TextDrawCreate(18.0, 20.0, playerHeader);
    TextDrawAlignment(gRoleplayUIDraw[playerid][1], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][1], 0.22, 0.9);
    TextDrawColor(gRoleplayUIDraw[playerid][1], 0x33CCFFFF);

    new stateText[96];
    format(stateText, sizeof(stateText), "Staat: %s | Cash: $%d | Bank: $%d", stateName, GetPlayerMoney(playerid), Core_GetPlayerBank(playerid));
    gRoleplayUIDraw[playerid][2] = TextDrawCreate(18.0, 36.0, stateText);
    TextDrawAlignment(gRoleplayUIDraw[playerid][2], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][2], 0.18, 0.75);
    TextDrawColor(gRoleplayUIDraw[playerid][2], 0xFFFFFFFF);

    new progressText[96];
    new currentJob = JobsCore_GetPlayerJob(playerid);
    new jobLabel[24];
    JobsCore_GetJobTypeName(currentJob, jobLabel, sizeof(jobLabel));
    format(progressText, sizeof(progressText), "Job: %s | Wanted: %d | Level: %d", jobLabel, gPlayerData[playerid][E_PLAYER_WANTED], gPlayerData[playerid][E_PLAYER_LEVEL]);
    gRoleplayUIDraw[playerid][3] = TextDrawCreate(18.0, 49.0, progressText);
    TextDrawAlignment(gRoleplayUIDraw[playerid][3], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][3], 0.18, 0.75);
    TextDrawColor(gRoleplayUIDraw[playerid][3], 0x99FF99AA);

    new miscText[96];
    format(miscText, sizeof(miscText), "XP: %d | Rep: %d | Jail: %d | Admin: %d", gPlayerData[playerid][E_PLAYER_XP], gPlayerData[playerid][E_PLAYER_REP], gPlayerData[playerid][E_PLAYER_JAIL_TIME], gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL]);
    gRoleplayUIDraw[playerid][4] = TextDrawCreate(18.0, 62.0, miscText);
    TextDrawAlignment(gRoleplayUIDraw[playerid][4], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][4], 0.18, 0.75);
    TextDrawColor(gRoleplayUIDraw[playerid][4], 0xFFCC66AA);

    gRoleplayUIDraw[playerid][5] = TextDrawCreate(220.0, 18.0, "_");
    TextDrawUseBox(gRoleplayUIDraw[playerid][5], 1);
    TextDrawBoxColor(gRoleplayUIDraw[playerid][5], 0x1B1B1B99);
    TextDrawTextSize(gRoleplayUIDraw[playerid][5], 440.0, 0.0);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][5], 0.0, 5.3);

    gRoleplayUIDraw[playerid][6] = TextDrawCreate(226.0, 20.0, "LIVE EVENT / RUECKSPIEGEL");
    TextDrawAlignment(gRoleplayUIDraw[playerid][6], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][6], 0.18, 0.75);
    TextDrawColor(gRoleplayUIDraw[playerid][6], 0x8CC6FFFF);

    gRoleplayUIDraw[playerid][7] = TextDrawCreate(226.0, 36.0, "Status: Stadt-/State-Updates und Systemmeldungen");
    TextDrawAlignment(gRoleplayUIDraw[playerid][7], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][7], 0.16, 0.7);
    TextDrawColor(gRoleplayUIDraw[playerid][7], 0xFFFFFFFF);

    gRoleplayUIDraw[playerid][8] = TextDrawCreate(470.0, 18.0, "_");
    TextDrawUseBox(gRoleplayUIDraw[playerid][8], 1);
    TextDrawBoxColor(gRoleplayUIDraw[playerid][8], 0x1B1B1B99);
    TextDrawTextSize(gRoleplayUIDraw[playerid][8], 610.0, 0.0);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][8], 0.0, 5.0);

    gRoleplayUIDraw[playerid][9] = TextDrawCreate(476.0, 20.0, "KONTEXT | MENU / TEXT-WARS");
    TextDrawAlignment(gRoleplayUIDraw[playerid][9], 1);
    TextDrawLetterSize(gRoleplayUIDraw[playerid][9], 0.18, 0.75);
    TextDrawColor(gRoleplayUIDraw[playerid][9], 0xFFCC66AA);

    for (new i = 0; i < RP_UI_MAX_LINES; i++)
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

    new cmd[64];
    RoleplayUI_ExtractCommandToken(cmdtext, cmd, sizeof(cmd));

    if (cmd[0] == '\0')
    {
        return false;
    }

    if (strcmp(cmd, "/hud", true) == 0 || strcmp(cmd, "/dashboard", true) == 0)
    {
        if (!UiHub_Ready(playerid))
        {
            return true;
        }
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
