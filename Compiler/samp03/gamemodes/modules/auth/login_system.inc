#if !defined LOGIN_SYSTEM_INC
#define LOGIN_SYSTEM_INC

#include <a_samp>

#define LOGIN_STATE_NONE 0
#define LOGIN_STATE_REGISTER 1
#define LOGIN_STATE_LOGIN 2
#define LOGIN_STATE_ACTIVE 3

new gLoginState[MAX_PLAYERS];
new Text:gLoginTD[MAX_PLAYERS][8];

stock LoginSystem_Initialize()
{
    print("[LOGIN] Textdraw-Login Modul initialisiert");
}

stock LoginSystem_OnPlayerConnect(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    gLoginState[playerid] = LOGIN_STATE_REGISTER;

    new preservedHome = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_HOME_STATE], STATE_LOS_SANTOS);
    new preservedState = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_STATE], preservedHome);

    Core_SetPlayerHomeState(playerid, preservedHome);
    Core_SetPlayerState(playerid, preservedState);
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = preservedHome;
    gPlayerData[playerid][E_PLAYER_STATE] = preservedState;
    Core_NormalizePlayerState(playerid);
    LoginSystem_ShowRegisterScreen(playerid);
    TogglePlayerControllable(playerid, false);
}

stock LoginSystem_OnPlayerDisconnect(playerid, reason)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    LoginSystem_HideRegisterScreen(playerid);
    gLoginState[playerid] = LOGIN_STATE_NONE;

    new preservedHome = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_HOME_STATE], Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_STATE], STATE_LOS_SANTOS));
    Core_SetPlayerHomeState(playerid, preservedHome);
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = preservedHome;
    if (!Core_IsValidState(gPlayerData[playerid][E_PLAYER_STATE]))
    {
        gPlayerData[playerid][E_PLAYER_STATE] = preservedHome;
        Core_SetPlayerState(playerid, preservedHome);
    }
}

stock LoginSystem_ShowRegisterScreen(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    LoginSystem_HideRegisterScreen(playerid);

    gLoginTD[playerid][0] = TextDrawCreate(320.0, 40.0, "GTA:SA Reallife");
    TextDrawAlignment(gLoginTD[playerid][0], 2);
    TextDrawLetterSize(gLoginTD[playerid][0], 0.8, 2.2);
    TextDrawUseBox(gLoginTD[playerid][0], 0);
    TextDrawColor(gLoginTD[playerid][0], 0x00D9FFFF);

    gLoginTD[playerid][1] = TextDrawCreate(320.0, 75.0, "REGISTRIERUNG");
    TextDrawAlignment(gLoginTD[playerid][1], 2);
    TextDrawLetterSize(gLoginTD[playerid][1], 0.45, 1.4);
    TextDrawColor(gLoginTD[playerid][1], 0xFFFFFFFF);

    gLoginTD[playerid][2] = TextDrawCreate(320.0, 110.0, "Bitte w~hle deine Heimatstadt:");
    TextDrawAlignment(gLoginTD[playerid][2], 2);
    TextDrawLetterSize(gLoginTD[playerid][2], 0.28, 1.0);
    TextDrawColor(gLoginTD[playerid][2], 0xFFFFFFFF);

    gLoginTD[playerid][3] = TextDrawCreate(320.0, 140.0, "LS - Los Santos   SF - San Fierro   LV - Las Venturas");
    TextDrawAlignment(gLoginTD[playerid][3], 2);
    TextDrawLetterSize(gLoginTD[playerid][3], 0.28, 1.0);
    TextDrawColor(gLoginTD[playerid][3], 0x55FF55FF);

    gLoginTD[playerid][4] = TextDrawCreate(320.0, 170.0, "Befehle:");
    TextDrawAlignment(gLoginTD[playerid][4], 2);
    TextDrawLetterSize(gLoginTD[playerid][4], 0.28, 1.0);
    TextDrawColor(gLoginTD[playerid][4], 0xFFFFFFFF);

    gLoginTD[playerid][5] = TextDrawCreate(320.0, 190.0, "/register <Name> <Passwort> /city <LS|SF|LV>");
    TextDrawAlignment(gLoginTD[playerid][5], 2);
    TextDrawLetterSize(gLoginTD[playerid][5], 0.25, 1.0);
    TextDrawColor(gLoginTD[playerid][5], 0xFFCC66FF);

    gLoginTD[playerid][6] = TextDrawCreate(320.0, 212.0, "Nach der Registrierung startet das Tutorial.");
    TextDrawAlignment(gLoginTD[playerid][6], 2);
    TextDrawLetterSize(gLoginTD[playerid][6], 0.24, 0.9);
    TextDrawColor(gLoginTD[playerid][6], 0xBBBBBBFF);

    gLoginTD[playerid][7] = TextDrawCreate(320.0, 245.0, "https://example.com/reallife");
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
        if (gLoginTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gLoginTD[playerid][i]);
            TextDrawDestroy(gLoginTD[playerid][i]);
            gLoginTD[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }
}

stock LoginSystem_SetPlayerHomeState(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return;
    }

    Core_SetPlayerHomeState(playerid, stateid);
    Core_SetPlayerState(playerid, stateid);
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
    return Core_GetPlayerHomeState(playerid);
}

stock LoginSystem_CompleteRegistration(playerid)
{
    LoginSystem_HideRegisterScreen(playerid);
    gLoginState[playerid] = LOGIN_STATE_ACTIVE;
    TogglePlayerControllable(playerid, true);
    SendClientMessage(playerid, 0x00FF00AA, "[LOGIN] Registrierung erfolgreich. Das Tutorial startet nun.");
}

#endif
