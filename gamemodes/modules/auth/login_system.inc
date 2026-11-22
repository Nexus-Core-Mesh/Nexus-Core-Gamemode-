#if !defined LOGIN_SYSTEM_INC
#define LOGIN_SYSTEM_INC

#include <a_samp>

#if !defined TUTORIAL_STAGE_NONE
#define TUTORIAL_STAGE_NONE 0
#endif
#if !defined TUTORIAL_STAGE_INTRO
#define TUTORIAL_STAGE_INTRO 1
#endif
#if !defined TUTORIAL_STAGE_HOME
#define TUTORIAL_STAGE_HOME 2
#endif
#if !defined TUTORIAL_STAGE_GOV
#define TUTORIAL_STAGE_GOV 3
#endif
#if !defined TUTORIAL_STAGE_FINISH
#define TUTORIAL_STAGE_FINISH 4
#endif

#define LOGIN_STATE_NONE 0
#define LOGIN_STATE_REGISTER 1
#define LOGIN_STATE_LOGIN 2
#define LOGIN_STATE_ACTIVE 3

new gPlayerLoginState[MAX_PLAYERS];
new gLoginAccountExists[MAX_PLAYERS];
new Text:gLoginTD[MAX_PLAYERS][12];

forward TutorialSystem_Start(playerid);
forward int:TutorialSystem_GetCurrentStage(playerid);

stock LoginSystem_HandleClickTextDraw(playerid, Text:clickedid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return 0;
    }

    if (clickedid == Text:INVALID_TEXT_DRAW)
    {
        CancelSelectTextDraw(playerid);
        return 1;
    }

    for (new i = 0; i < 12; i++)
    {
        if (gLoginTD[playerid][i] != clickedid)
        {
            continue;
        }

        if (gPlayerLoginState[playerid] == LOGIN_STATE_LOGIN)
        {
            if (i == 5)
            {
                UiHub_ShowPasswordInput(playerid);
                return 1;
            }
            if (i == 6)
            {
                LoginSystem_CompleteLogin(playerid);
                return 1;
            }
            return 0;
        }

        if (gPlayerLoginState[playerid] == LOGIN_STATE_REGISTER)
        {
            if (i == 6)
            {
                UiHub_ShowPasswordInput(playerid);
                return 1;
            }
            if (i == 7)
            {
                LoginSystem_CompleteReg(playerid);
                return 1;
            }
            if (i == 8)
            {
                LoginSystem_SetPlayerHomeState(playerid, STATE_LOS_SANTOS);
                return 1;
            }
            if (i == 9)
            {
                LoginSystem_SetPlayerHomeState(playerid, STATE_SAN_FIERRO);
                return 1;
            }
            if (i == 10)
            {
                LoginSystem_SetPlayerHomeState(playerid, STATE_LAS_VENTURAS);
                return 1;
            }
            if (i == 11)
            {
                TutorialSystem_Start(playerid);
                return 1;
            }
            return 0;
        }

        return 0;
    }

    return 0;
}

stock LoginSystem_Initialize()
{
    print("[LOGIN] Textdraw-Login Modul initialisiert");
}

stock LoginSystem_IsRegistered(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return 0;
    }

    if (gLoginAccountExists[playerid])
    {
        return 1;
    }

    if (gPlayerData[playerid][E_PLAYER_LEVEL] > 1)
    {
        return 1;
    }
    if (gPlayerData[playerid][E_PLAYER_BANK] != 2500 || gPlayerBank[playerid] != 2500)
    {
        return 1;
    }
    if (gPlayerCash[playerid] != 2500)
    {
        return 1;
    }
    if (gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL] > 0 || gPlayerData[playerid][E_PLAYER_PROJECT_LEADER] == 1)
    {
        return 1;
    }
    if (gPlayerData[playerid][E_PLAYER_WANTED] > 0 || gPlayerData[playerid][E_PLAYER_JAIL_ACTIVE] == 1)
    {
        return 1;
    }

    return 0;
}

stock LoginSystem_AutoResolveFlow(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    new registered = LoginSystem_IsRegistered(playerid);
    gLoginAccountExists[playerid] = registered;

    if (registered)
    {
        gPlayerLoginState[playerid] = LOGIN_STATE_LOGIN;
        LoginSystem_HideLoginScreen(playerid);
        LoginSystem_HideRegisterScreen(playerid);
        LoginSystem_ShowLoginScreen(playerid);
        UiHub_ShowPasswordInput(playerid);
        SendClientMessage(playerid, 0x00D9FFFF, "[LOGIN] Bestehender Account erkannt. Bitte Passwort eingeben und mit Weiter fortfahren.");
    }
    else
    {
        new defaultState = Core_GetValidStateOrDefault(Core_GetPlayerState(playerid), STATE_LOS_SANTOS);
        new defaultHomeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), defaultState);

        if (!Core_IsValidState(defaultState))
        {
            defaultState = STATE_LOS_SANTOS;
        }
        if (!Core_IsValidState(defaultHomeState))
        {
            defaultHomeState = defaultState;
        }

        Core_SetPlayerHomeState(playerid, defaultHomeState);
        Core_SetPlayerState(playerid, defaultState);
        gPlayerData[playerid][E_PLAYER_HOME_STATE] = defaultHomeState;
        gPlayerData[playerid][E_PLAYER_STATE] = defaultState;

        gPlayerLoginState[playerid] = LOGIN_STATE_REGISTER;
        LoginSystem_HideLoginScreen(playerid);
        LoginSystem_HideRegisterScreen(playerid);
        LoginSystem_ShowRegisterScreen(playerid);
        TutorialSystem_Start(playerid);
        SendClientMessage(playerid, 0x00D9FFFF, "[LOGIN] Neuer Spieler erkannt. Tutorial startet und der Account-Start wird vorbereitet.");
    }

    TogglePlayerControllable(playerid, false);
}

stock LoginSystem_AdvanceToStateSelection(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    gPlayerLoginState[playerid] = LOGIN_STATE_ACTIVE;
    CameraSystem_Reset(playerid);
    if (!Core_IsValidState(Core_GetPlayerHomeState(playerid)))
    {
        TutorialSystem_Start(playerid);
    }
    UiHub_ShowStatePick(playerid);
}

stock LoginSystem_OnPlayerConnect(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    gPlayerLoginState[playerid] = LOGIN_STATE_NONE;
    gLoginAccountExists[playerid] = false;

    new defaultState = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_STATE], STATE_LOS_SANTOS);
    new defaultHomeState = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_HOME_STATE], defaultState);
    if (!Core_IsValidState(defaultState))
    {
        defaultState = STATE_LOS_SANTOS;
    }
    if (!Core_IsValidState(defaultHomeState))
    {
        defaultHomeState = defaultState;
    }

    Core_SetPlayerHomeState(playerid, defaultHomeState);
    Core_SetPlayerState(playerid, defaultState);
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = defaultHomeState;
    gPlayerData[playerid][E_PLAYER_STATE] = defaultState;
    Core_NormalizePlayerState(playerid);
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    LoginSystem_AutoResolveFlow(playerid);
}

stock LoginSystem_OnPlayerDisconnect(playerid, reason)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    LoginSystem_HideRegisterScreen(playerid);
    LoginSystem_HideLoginScreen(playerid);
    CancelSelectTextDraw(playerid);
    gPlayerLoginState[playerid] = LOGIN_STATE_NONE;
    gLoginAccountExists[playerid] = false;

    new preservedState = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_STATE], STATE_LOS_SANTOS);
    new preservedHomeState = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_HOME_STATE], preservedState);
    if (!Core_IsValidState(preservedState))
    {
        preservedState = STATE_LOS_SANTOS;
    }
    if (!Core_IsValidState(preservedHomeState))
    {
        preservedHomeState = preservedState;
    }

    Core_SetPlayerHomeState(playerid, preservedHomeState);
    Core_SetPlayerState(playerid, preservedState);
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = preservedHomeState;
    gPlayerData[playerid][E_PLAYER_STATE] = preservedState;
    Core_NormalizePlayerState(playerid);
    gPlayerLoginState[playerid] = LOGIN_STATE_NONE;
}

stock LoginSystem_ShowLoginScreen(playerid)
{
    LoginSystem_HideRegisterScreen(playerid);
    LoginSystem_HideLoginScreen(playerid);

    new const loginTitle[] = "GTA:SA Reallife";
    new const loginHeader[] = "LOGIN";
    new const loginText[] = "Willkommen zurueck. Du wirst in den Spielablauf weitergeleitet.";
    new const loginHint[] = "Stadt, Jobs, Fahrzeuge und Menue sind direkt im Hub erreichbar.";
    new const loginInfo[] = "Aktiviert: Auto-Login-Flow, Tutorial, Kamera und Hub-Intro";
    new const passwordButton[] = "[ PASSWORT EINGEBEN ]";
    new const loginButton[] = "[ EINLOGGEN ]";

    gLoginTD[playerid][0] = UiDialog_TextDrawCreate(320.0, 40.0, loginTitle);
    TextDrawAlignment(gLoginTD[playerid][0], 2);
    TextDrawLetterSize(gLoginTD[playerid][0], 0.75, 2.0);
    TextDrawUseBox(gLoginTD[playerid][0], 1);
    TextDrawBoxColor(gLoginTD[playerid][0], 0x111111AA);
    TextDrawTextSize(gLoginTD[playerid][0], 200.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][0], 0);
    TextDrawColor(gLoginTD[playerid][0], 0x00D9FFFF);

    gLoginTD[playerid][1] = UiDialog_TextDrawCreate(320.0, 80.0, loginHeader);
    TextDrawAlignment(gLoginTD[playerid][1], 2);
    TextDrawLetterSize(gLoginTD[playerid][1], 0.5, 1.6);
    TextDrawUseBox(gLoginTD[playerid][1], 1);
    TextDrawBoxColor(gLoginTD[playerid][1], 0x003366AA);
    TextDrawTextSize(gLoginTD[playerid][1], 180.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][1], 0);
    TextDrawColor(gLoginTD[playerid][1], 0x00FF99AA);

    gLoginTD[playerid][2] = UiDialog_TextDrawCreate(320.0, 120.0, loginText);
    TextDrawAlignment(gLoginTD[playerid][2], 2);
    TextDrawLetterSize(gLoginTD[playerid][2], 0.28, 1.0);
    TextDrawSetSelectable(gLoginTD[playerid][2], 0);
    TextDrawColor(gLoginTD[playerid][2], 0xFFFFFFFF);

    gLoginTD[playerid][3] = UiDialog_TextDrawCreate(320.0, 150.0, loginHint);
    TextDrawAlignment(gLoginTD[playerid][3], 2);
    TextDrawLetterSize(gLoginTD[playerid][3], 0.25, 0.9);
    TextDrawSetSelectable(gLoginTD[playerid][3], 0);
    TextDrawColor(gLoginTD[playerid][3], 0xFFCC66FF);

    gLoginTD[playerid][4] = UiDialog_TextDrawCreate(320.0, 178.0, loginInfo);
    TextDrawAlignment(gLoginTD[playerid][4], 2);
    TextDrawLetterSize(gLoginTD[playerid][4], 0.2, 0.8);
    TextDrawSetSelectable(gLoginTD[playerid][4], 0);
    TextDrawColor(gLoginTD[playerid][4], 0xBBBBBBFF);

    gLoginTD[playerid][5] = UiDialog_TextDrawCreate(320.0, 230.0, passwordButton);
    TextDrawAlignment(gLoginTD[playerid][5], 2);
    TextDrawLetterSize(gLoginTD[playerid][5], 0.22, 1.0);
    TextDrawUseBox(gLoginTD[playerid][5], 1);
    TextDrawBoxColor(gLoginTD[playerid][5], 0x1A4D7AAA);
    TextDrawTextSize(gLoginTD[playerid][5], 220.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][5], 1);
    TextDrawColor(gLoginTD[playerid][5], 0xFFF5A5FF);

    gLoginTD[playerid][6] = UiDialog_TextDrawCreate(320.0, 270.0, loginButton);
    TextDrawAlignment(gLoginTD[playerid][6], 2);
    TextDrawLetterSize(gLoginTD[playerid][6], 0.22, 1.0);
    TextDrawUseBox(gLoginTD[playerid][6], 1);
    TextDrawBoxColor(gLoginTD[playerid][6], 0x2A9D5DCC);
    TextDrawTextSize(gLoginTD[playerid][6], 220.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][6], 1);
    TextDrawColor(gLoginTD[playerid][6], 0xFFFFFFFF);

    SelectTextDraw(playerid, 0x38BDF8FF);

    for (new i = 0; i < 7; i++)
    {
        if (gLoginTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawShowForPlayer(playerid, gLoginTD[playerid][i]);
        }
    }
}

stock LoginSystem_ShowRegisterScreen(playerid)
{
    LoginSystem_HideRegisterScreen(playerid);
    LoginSystem_HideLoginScreen(playerid);

    new const loginTitle[] = "GTA:SA Reallife";
    new const loginRegister[] = "REGISTRIERUNG";
    new const loginHome[] = "Bitte waehle deine Heimatstadt im Menue:";
    new const loginCities[] = "LS - Los Santos   SF - San Fierro   LV - Las Venturas";
    new const loginFlow[] = "Das Passwort bleibt die einzige freie Texteingabe.";
    new const loginTutorial[] = "Nach der Stadtwahl startet das Tutorial automatisch.";
    new const passwordButton[] = "[ PASSWORT EINGEBEN ]";
    new const registerButton[] = "[ ACCOUNT ERSTELLEN ]";
    new const lsButton[] = "[ LS: LOS SANTOS ]";
    new const sfButton[] = "[ SF: SAN FIERRO ]";
    new const lvButton[] = "[ LV: LAS VENTURAS ]";

    gLoginTD[playerid][0] = UiDialog_TextDrawCreate(320.0, 40.0, loginTitle);
    TextDrawAlignment(gLoginTD[playerid][0], 2);
    TextDrawLetterSize(gLoginTD[playerid][0], 0.8, 2.2);
    TextDrawUseBox(gLoginTD[playerid][0], 1);
    TextDrawBoxColor(gLoginTD[playerid][0], 0x111111AA);
    TextDrawTextSize(gLoginTD[playerid][0], 210.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][0], 0);
    TextDrawColor(gLoginTD[playerid][0], 0x00D9FFFF);

    gLoginTD[playerid][1] = UiDialog_TextDrawCreate(320.0, 75.0, loginRegister);
    TextDrawAlignment(gLoginTD[playerid][1], 2);
    TextDrawLetterSize(gLoginTD[playerid][1], 0.45, 1.4);
    TextDrawUseBox(gLoginTD[playerid][1], 1);
    TextDrawBoxColor(gLoginTD[playerid][1], 0x223344AA);
    TextDrawTextSize(gLoginTD[playerid][1], 180.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][1], 0);
    TextDrawColor(gLoginTD[playerid][1], 0xFFFFFFFF);

    gLoginTD[playerid][2] = UiDialog_TextDrawCreate(320.0, 110.0, loginHome);
    TextDrawAlignment(gLoginTD[playerid][2], 2);
    TextDrawLetterSize(gLoginTD[playerid][2], 0.28, 1.0);
    TextDrawSetSelectable(gLoginTD[playerid][2], 0);
    TextDrawColor(gLoginTD[playerid][2], 0xFFFFFFFF);

    gLoginTD[playerid][3] = UiDialog_TextDrawCreate(320.0, 140.0, loginCities);
    TextDrawAlignment(gLoginTD[playerid][3], 2);
    TextDrawLetterSize(gLoginTD[playerid][3], 0.28, 1.0);
    TextDrawSetSelectable(gLoginTD[playerid][3], 0);
    TextDrawColor(gLoginTD[playerid][3], 0x55FF55FF);

    gLoginTD[playerid][4] = UiDialog_TextDrawCreate(320.0, 170.0, loginFlow);
    TextDrawAlignment(gLoginTD[playerid][4], 2);
    TextDrawLetterSize(gLoginTD[playerid][4], 0.25, 1.0);
    TextDrawSetSelectable(gLoginTD[playerid][4], 0);
    TextDrawColor(gLoginTD[playerid][4], 0xFFCC66FF);

    gLoginTD[playerid][5] = UiDialog_TextDrawCreate(320.0, 195.0, loginTutorial);
    TextDrawAlignment(gLoginTD[playerid][5], 2);
    TextDrawLetterSize(gLoginTD[playerid][5], 0.24, 0.9);
    TextDrawSetSelectable(gLoginTD[playerid][5], 0);
    TextDrawColor(gLoginTD[playerid][5], 0xBBBBBBFF);

    gLoginTD[playerid][6] = UiDialog_TextDrawCreate(320.0, 230.0, passwordButton);
    TextDrawAlignment(gLoginTD[playerid][6], 2);
    TextDrawLetterSize(gLoginTD[playerid][6], 0.22, 1.0);
    TextDrawUseBox(gLoginTD[playerid][6], 1);
    TextDrawBoxColor(gLoginTD[playerid][6], 0x1A4D7AAA);
    TextDrawTextSize(gLoginTD[playerid][6], 220.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][6], 1);
    TextDrawColor(gLoginTD[playerid][6], 0xFFF5A5FF);

    gLoginTD[playerid][7] = UiDialog_TextDrawCreate(320.0, 258.0, registerButton);
    TextDrawAlignment(gLoginTD[playerid][7], 2);
    TextDrawLetterSize(gLoginTD[playerid][7], 0.22, 1.0);
    TextDrawUseBox(gLoginTD[playerid][7], 1);
    TextDrawBoxColor(gLoginTD[playerid][7], 0x2A9D5DCC);
    TextDrawTextSize(gLoginTD[playerid][7], 220.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][7], 1);
    TextDrawColor(gLoginTD[playerid][7], 0xFFFFFFFF);

    gLoginTD[playerid][8] = UiDialog_TextDrawCreate(120.0, 295.0, lsButton);
    TextDrawAlignment(gLoginTD[playerid][8], 2);
    TextDrawLetterSize(gLoginTD[playerid][8], 0.18, 0.8);
    TextDrawUseBox(gLoginTD[playerid][8], 1);
    TextDrawBoxColor(gLoginTD[playerid][8], 0x1D7FC9AA);
    TextDrawTextSize(gLoginTD[playerid][8], 220.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][8], 1);
    TextDrawColor(gLoginTD[playerid][8], 0xFFFFFFFF);

    gLoginTD[playerid][9] = UiDialog_TextDrawCreate(320.0, 295.0, sfButton);
    TextDrawAlignment(gLoginTD[playerid][9], 2);
    TextDrawLetterSize(gLoginTD[playerid][9], 0.18, 0.8);
    TextDrawUseBox(gLoginTD[playerid][9], 1);
    TextDrawBoxColor(gLoginTD[playerid][9], 0x8C6D3FBB);
    TextDrawTextSize(gLoginTD[playerid][9], 220.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][9], 1);
    TextDrawColor(gLoginTD[playerid][9], 0xFFFFFFFF);

    gLoginTD[playerid][10] = UiDialog_TextDrawCreate(520.0, 295.0, lvButton);
    TextDrawAlignment(gLoginTD[playerid][10], 2);
    TextDrawLetterSize(gLoginTD[playerid][10], 0.18, 0.8);
    TextDrawUseBox(gLoginTD[playerid][10], 1);
    TextDrawBoxColor(gLoginTD[playerid][10], 0xB05A2AAA);
    TextDrawTextSize(gLoginTD[playerid][10], 220.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][10], 1);
    TextDrawColor(gLoginTD[playerid][10], 0xFFFFFFFF);

    gLoginTD[playerid][11] = UiDialog_TextDrawCreate(320.0, 330.0, "[ TUTORIAL / WEITER ]");
    TextDrawAlignment(gLoginTD[playerid][11], 2);
    TextDrawLetterSize(gLoginTD[playerid][11], 0.18, 0.8);
    TextDrawUseBox(gLoginTD[playerid][11], 1);
    TextDrawBoxColor(gLoginTD[playerid][11], 0x7A4FD7AA);
    TextDrawTextSize(gLoginTD[playerid][11], 240.0, 0.0);
    TextDrawSetSelectable(gLoginTD[playerid][11], 1);
    TextDrawColor(gLoginTD[playerid][11], 0xFFFFFFFF);

    SelectTextDraw(playerid, 0x38BDF8FF);

    for (new i = 0; i < 12; i++)
    {
        if (gLoginTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawShowForPlayer(playerid, gLoginTD[playerid][i]);
        }
    }
}

stock LoginSystem_HideLoginScreen(playerid)
{
    CancelSelectTextDraw(playerid);
    for (new i = 0; i < 12; i++)
    {
        if (gLoginTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gLoginTD[playerid][i]);
            TextDrawDestroy(gLoginTD[playerid][i]);
            gLoginTD[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }
}

stock LoginSystem_HideRegisterScreen(playerid)
{
    CancelSelectTextDraw(playerid);
    for (new i = 0; i < 12; i++)
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
    gPlayerLoginState[playerid] = LOGIN_STATE_ACTIVE;
    gLoginAccountExists[playerid] = true;
    TogglePlayerControllable(playerid, true);

    new message[128];
    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    format(message, sizeof(message), "[LOGIN] Deine Heimatstadt wurde gesetzt: %s", stateName);
    SendClientMessage(playerid, 0x00FF00AA, message);
    StateSystem_SpawnPlayerForState(playerid, stateid);
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
}

stock LoginSystem_IsActive(playerid)
{
    return gPlayerLoginState[playerid] == LOGIN_STATE_ACTIVE;
}

stock LoginSystem_ActivateSession(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    new safeState = Core_GetValidStateOrDefault(stateid, Core_GetPlayerState(playerid));
    if (!Core_IsValidState(safeState))
    {
        safeState = STATE_LOS_SANTOS;
    }

    new safeHomeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), safeState);
    if (!Core_IsValidState(safeHomeState))
    {
        safeHomeState = safeState;
    }

    Core_SetPlayerState(playerid, safeState);
    Core_SetPlayerHomeState(playerid, safeHomeState);
    gPlayerData[playerid][E_PLAYER_STATE] = safeState;
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = safeHomeState;
    gPlayerLoginState[playerid] = LOGIN_STATE_ACTIVE;
    gLoginAccountExists[playerid] = true;
    TogglePlayerControllable(playerid, true);

    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);

    DB_ValidateStateTransfer(playerid, safeState, safeHomeState);
    DB_SavePlayerState(playerid, safeState);
}

stock int:LoginSystem_GetHomeState(playerid)
{
    return Core_GetPlayerHomeState(playerid);
}

stock LoginSystem_CompleteLogin(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    new homeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), Core_GetPlayerState(playerid));
    if (!Core_IsValidState(homeState))
    {
        homeState = STATE_LOS_SANTOS;
    }

    LoginSystem_ActivateSession(playerid, homeState);
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    SendClientMessage(playerid, 0x00FF00AA, "[LOGIN] Login erfolgreich. Das Tutorial startet nun.");
    CameraSystem_PlayStateScene(playerid, homeState);
    if (!Core_IsValidState(Core_GetPlayerHomeState(playerid)) || TutorialSystem_GetCurrentStage(playerid) < TUTORIAL_STAGE_FINISH)
    {
        TutorialSystem_Start(playerid);
    }
}

stock LoginSystem_CompleteReg(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    new homeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), Core_GetPlayerState(playerid));
    if (!Core_IsValidState(homeState))
    {
        homeState = STATE_LOS_SANTOS;
    }

    LoginSystem_ActivateSession(playerid, homeState);
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    SendClientMessage(playerid, 0x00FF00AA, "[LOGIN] Registrierung erfolgreich. Das Tutorial startet nun.");
    CameraSystem_PlayStateScene(playerid, homeState);
    if (!Core_IsValidState(Core_GetPlayerHomeState(playerid)) || TutorialSystem_GetCurrentStage(playerid) < TUTORIAL_STAGE_FINISH)
    {
        TutorialSystem_Start(playerid);
    }
}

#endif
