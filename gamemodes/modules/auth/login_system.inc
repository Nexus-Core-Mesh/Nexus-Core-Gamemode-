#if !defined LOGIN_SYSTEM_INC
#define LOGIN_SYSTEM_INC

#include <a_samp>

#define LOGIN_STATE_NONE 0
#define LOGIN_STATE_REGISTER 1
#define LOGIN_STATE_LOGIN 2
#define LOGIN_STATE_ACTIVE 3

new gLoginState[MAX_PLAYERS];
new gPlayerHomeState[MAX_PLAYERS];
new Text:gLoginTD[MAX_PLAYERS][8];

stock LoginSystem_Initialize()
{
    print("[LOGIN] Textdraw-Login Modul initialisiert");
}

stock LoginSystem_OnPlayerConnect(playerid)
{
    gLoginState[playerid] = LOGIN_STATE_REGISTER;
    gPlayerHomeState[playerid] = STATE_LOS_SANTOS;
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = STATE_LOS_SANTOS;
    gPlayerData[playerid][E_PLAYER_STATE] = STATE_LOS_SANTOS;
    Core_NormalizePlayerState(playerid);
    LoginSystem_ShowRegisterScreen(playerid);
    TogglePlayerControllable(playerid, false);
}

stock LoginSystem_OnPlayerDisconnect(playerid, reason)
{
    LoginSystem_HideRegisterScreen(playerid);
    gLoginState[playerid] = LOGIN_STATE_NONE;
    gPlayerHomeState[playerid] = 0;
}

stock LoginSystem_ShowRegisterScreen(playerid)
{
    LoginSystem_HideRegisterScreen(playerid);

    new const loginTitle[] = "GTA:SA Reallife";
    new const loginRegister[] = "REGISTRIERUNG";
    new const loginHome[] = "Bitte w~hle deine Heimatstadt:";
    new const loginCities[] = "LS - Los Santos   SF - San Fierro   LV - Las Venturas";
    new const loginCmds[] = "Befehle:";
    new const loginCommandHelp[] = "/register <Name> <Passwort> /city <LS|SF|LV>";
    new const loginTutorial[] = "Nach der Registrierung startet das Tutorial.";
    new const loginLink[] = "https://example.com/reallife";

    gLoginTD[playerid][0] = UiDialog_TextDrawCreate(320.0, 40.0, loginTitle);
    TextDrawAlignment(gLoginTD[playerid][0], 2);
    TextDrawLetterSize(gLoginTD[playerid][0], 0.8, 2.2);
    TextDrawUseBox(gLoginTD[playerid][0], 0);
    TextDrawColor(gLoginTD[playerid][0], 0x00D9FFFF);

    gLoginTD[playerid][1] = UiDialog_TextDrawCreate(320.0, 75.0, loginRegister);
    TextDrawAlignment(gLoginTD[playerid][1], 2);
    TextDrawLetterSize(gLoginTD[playerid][1], 0.45, 1.4);
    TextDrawColor(gLoginTD[playerid][1], 0xFFFFFFFF);

    gLoginTD[playerid][2] = UiDialog_TextDrawCreate(320.0, 110.0, loginHome);
    TextDrawAlignment(gLoginTD[playerid][2], 2);
    TextDrawLetterSize(gLoginTD[playerid][2], 0.28, 1.0);
    TextDrawColor(gLoginTD[playerid][2], 0xFFFFFFFF);

    gLoginTD[playerid][3] = UiDialog_TextDrawCreate(320.0, 140.0, loginCities);
    TextDrawAlignment(gLoginTD[playerid][3], 2);
    TextDrawLetterSize(gLoginTD[playerid][3], 0.28, 1.0);
    TextDrawColor(gLoginTD[playerid][3], 0x55FF55FF);

    gLoginTD[playerid][4] = UiDialog_TextDrawCreate(320.0, 170.0, loginCmds);
    TextDrawAlignment(gLoginTD[playerid][4], 2);
    TextDrawLetterSize(gLoginTD[playerid][4], 0.28, 1.0);
    TextDrawColor(gLoginTD[playerid][4], 0xFFFFFFFF);

    gLoginTD[playerid][5] = UiDialog_TextDrawCreate(320.0, 190.0, loginCommandHelp);
    TextDrawAlignment(gLoginTD[playerid][5], 2);
    TextDrawLetterSize(gLoginTD[playerid][5], 0.25, 1.0);
    TextDrawColor(gLoginTD[playerid][5], 0xFFCC66FF);

    gLoginTD[playerid][6] = UiDialog_TextDrawCreate(320.0, 212.0, loginTutorial);
    TextDrawAlignment(gLoginTD[playerid][6], 2);
    TextDrawLetterSize(gLoginTD[playerid][6], 0.24, 0.9);
    TextDrawColor(gLoginTD[playerid][6], 0xBBBBBBFF);

    gLoginTD[playerid][7] = UiDialog_TextDrawCreate(320.0, 245.0, loginLink);
    TextDrawAlignment(gLoginTD[playerid][7], 2);
    TextDrawLetterSize(gLoginTD[playerid][7], 0.2, 0.8);
    TextDrawColor(gLoginTD[playerid][7], 0x8CC6FFFF);

    for (new i = 0; i < 8; i++)
    {
        TextDrawShowForPlayer(playerid, gLoginTD[playerid][i]);
    }
}

stock LoginSystem_HideRegisterScreen(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gLoginTD[playerid][i] != Text:0)
        {
            TextDrawHideForPlayer(playerid, gLoginTD[playerid][i]);
            TextDrawDestroy(gLoginTD[playerid][i]);
            gLoginTD[playerid][i] = Text:0;
        }
    }
}

stock LoginSystem_SetPlayerHomeState(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return;
    }
    gPlayerHomeState[playerid] = stateid;
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = stateid;
    gPlayerData[playerid][E_PLAYER_STATE] = stateid;
    gLoginState[playerid] = LOGIN_STATE_ACTIVE;
    TogglePlayerControllable(playerid, true);

    new message[128];
    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    format(message, sizeof(message), "[LOGIN] Deine Heimatstadt wurde gesetzt: %s", stateName);
    SendClientMessage(playerid, 0x00FF00AA, message);
    StateSystem_SpawnPlayerForState(playerid, stateid);
}

stock LoginSystem_IsActive(playerid)
{
    return gLoginState[playerid] == LOGIN_STATE_ACTIVE;
}

stock int:LoginSystem_GetHomeState(playerid)
{
    return gPlayerHomeState[playerid];
}

stock LoginSystem_CompleteReg(playerid)
{
    LoginSystem_HideRegisterScreen(playerid);
    gLoginState[playerid] = LOGIN_STATE_ACTIVE;
    TogglePlayerControllable(playerid, true);
    SendClientMessage(playerid, 0x00FF00AA, "[LOGIN] Registrierung erfolgreich. Das Tutorial startet nun.");
}

#endif
