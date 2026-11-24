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
#define TUTORIAL_STAGE_FINISH 10
#endif

#define LOGIN_STATE_NONE 0
#define LOGIN_STATE_REGISTER 1
#define LOGIN_STATE_LOGIN 2
#define LOGIN_STATE_ACTIVE 3

new gPlayerLoginState[MAX_PLAYERS];
new gTutorialStage[MAX_PLAYERS];
new gLoginAccountExists[MAX_PLAYERS];
new Text:gLoginTD[MAX_PLAYERS][12];
new gPlayerSkinChoice[MAX_PLAYERS];
new bool:gPlayerSkinSelected[MAX_PLAYERS];
new gPlayerGender[MAX_PLAYERS];
new bool:gPlayerGenderSelected[MAX_PLAYERS];
new bool:gPlayerPasswordReady[MAX_PLAYERS];
new gPlayerWeather[MAX_PLAYERS];
new bool:gLoginSpawned[MAX_PLAYERS];
new bool:gLoginSpawnPending[MAX_PLAYERS];
new bool:gPlayerHomeStateSelected[MAX_PLAYERS];
new gPlayerPendingPassword[MAX_PLAYERS][64];

forward TutorialSystem_Start(playerid);
forward TutorialSystem_MarkFinished(playerid);
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
                LoginSystem_SelectHomeState(playerid, STATE_LOS_SANTOS);
                return 1;
            }
            if (i == 9)
            {
                LoginSystem_SelectHomeState(playerid, STATE_SAN_FIERRO);
                return 1;
            }
            if (i == 10)
            {
                LoginSystem_SelectHomeState(playerid, STATE_LAS_VENTURAS);
                return 1;
            }
            if (i == 11)
            {
                LoginSystem_HideRegisterScreen(playerid);
                UiHub_ShowStatePick(playerid);
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

    return 0;
}

stock LoginSystem_AutoResolveFlow(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    gPlayerLoginState[playerid] = LOGIN_STATE_NONE;
    gPlayerPasswordReady[playerid] = false;
    gPlayerSkinSelected[playerid] = false;
    gPlayerGenderSelected[playerid] = false;
    gPlayerHomeStateSelected[playerid] = false;
    gLoginSpawned[playerid] = false;
    gLoginSpawnPending[playerid] = false;
    gTutorialStage[playerid] = TUTORIAL_STAGE_NONE;
    gPlayerPendingPassword[playerid][0] = EOS;

    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    TutorialSystem_HideStage(playerid);
    CancelSelectTextDraw(playerid);

    // Echter, namensbasierter SQLite-Lookup entscheidet ueber Login vs. Register.
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    DB_SetAccountName(playerid, name);

    new bool:registered = (DB_AccountExists(name) != 0);
    gLoginAccountExists[playerid] = registered;

    CameraSystem_Reset(playerid);
    TogglePlayerControllable(playerid, false);

    if (registered)
    {
        gPlayerLoginState[playerid] = LOGIN_STATE_LOGIN;
        SendClientMessage(playerid, 0x00D9FFFF, "[LOGIN] Willkommen zurueck! Bitte gib dein Passwort ein.");
    }
    else
    {
        gPlayerLoginState[playerid] = LOGIN_STATE_REGISTER;
        SendClientMessage(playerid, 0x00D9FFFF, "[REGISTER] Neuer Account erkannt. Bitte setze ein Passwort.");
    }

    UiHub_ShowPasswordInput(playerid);
}

// Zentrale Passwort-Verarbeitung (aus dem Passwort-Dialog aufgerufen).
stock LoginSystem_SubmitPassword(playerid, const password[])
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    new name[MAX_PLAYER_NAME];
    DB_ResolveAccountName(playerid, name, sizeof(name));

    if (gPlayerLoginState[playerid] == LOGIN_STATE_REGISTER)
    {
        if (strlen(password) < 3)
        {
            SendClientMessage(playerid, 0xFF6666AA, "[REGISTER] Passwort zu kurz (mindestens 3 Zeichen).");
            UiHub_ShowPasswordInput(playerid);
            return;
        }
        format(gPlayerPendingPassword[playerid], 64, "%s", password);
        gPlayerPasswordReady[playerid] = true;
        SendClientMessage(playerid, 0x00FF00AA, "[REGISTER] Passwort gesetzt. Weiter mit der Charaktererstellung.");
        LoginSystem_CompleteReg(playerid);
        return;
    }

    // Bestehender Account -> Passwort gegen SQLite pruefen.
    if (!DB_VerifyPassword(name, password))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[LOGIN] Falsches Passwort. Bitte erneut versuchen.");
        UiHub_ShowPasswordInput(playerid);
        return;
    }

    gPlayerPasswordReady[playerid] = true;
    DB_LoadAccount(playerid, name);
    LoginSystem_CompleteLogin(playerid);
}

stock LoginSystem_CompleteRegistrationFlow(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    gPlayerLoginState[playerid] = LOGIN_STATE_REGISTER;
    TutorialSystem_Reset(playerid);
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CameraSystem_Reset(playerid);
    TogglePlayerControllable(playerid, false);
    SendClientMessage(playerid, 0x00FF00AA, "[REGISTER] Passwort gesetzt. Tutorial startet jetzt sauber in der korrekten Reihenfolge.");
    TutorialSystem_Start(playerid);
}

stock LoginSystem_AdvanceToStateSelection(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    CameraSystem_Reset(playerid);
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);

    if (gPlayerLoginState[playerid] != LOGIN_STATE_REGISTER)
    {
        return;
    }

    if (!LoginSystem_HasRequiredGender(playerid))
    {
        LoginSystem_RequireGenderSelection(playerid);
        return;
    }

    if (TutorialSystem_GetCurrentStage(playerid) < TUTORIAL_STAGE_FINISH)
    {
        TutorialSystem_Start(playerid);
        return;
    }

    if (!Core_IsValidState(Core_GetPlayerHomeState(playerid)))
    {
        UiHub_ShowStatePick(playerid);
        return;
    }

    LoginSystem_ActivateSession(playerid, Core_GetPlayerHomeState(playerid));
    StateSpawnSystem_Apply(playerid, Core_GetPlayerHomeState(playerid));
    RoleplayUI_ShowDashboard(playerid);
}

stock LoginSystem_OnPlayerConnect(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    gPlayerLoginState[playerid] = LOGIN_STATE_NONE;
    gLoginAccountExists[playerid] = false;
    gPlayerPasswordReady[playerid] = false;
    gPlayerSkinSelected[playerid] = false;
    gPlayerGenderSelected[playerid] = false;
    gLoginSpawned[playerid] = false;
    gLoginSpawnPending[playerid] = false;

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
    TutorialSystem_HideStage(playerid);
    CancelSelectTextDraw(playerid);
    gPlayerLoginState[playerid] = LOGIN_STATE_NONE;
    LoginSystem_AutoResolveFlow(playerid);
}

stock LoginSystem_OnPlayerDisconnect(playerid, reason)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    // Echten Kontostand erfassen und den gesamten Account persistieren, solange
    // der Spieler noch authentifiziert ist (sichert Bargeld/Bank/Level/State/Skin).
    if (DB_IsAuthenticated(playerid))
    {
        Core_SetPlayerCash(playerid, GetPlayerMoney(playerid));
        DB_SaveAccount(playerid);
    }

    LoginSystem_HideRegisterScreen(playerid);
    LoginSystem_HideLoginScreen(playerid);
    TutorialSystem_HideStage(playerid);
    CancelSelectTextDraw(playerid);
    gPlayerLoginState[playerid] = LOGIN_STATE_NONE;
    gLoginAccountExists[playerid] = false;
    gPlayerPasswordReady[playerid] = false;
    gPlayerSkinSelected[playerid] = false;
    gPlayerGenderSelected[playerid] = false;
    gLoginSpawned[playerid] = false;
    gLoginSpawnPending[playerid] = false;
    gTutorialStage[playerid] = TUTORIAL_STAGE_NONE;

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
    CameraSystem_Reset(playerid);
    TogglePlayerControllable(playerid, true);
    RoleplayUI_Hide(playerid);
}

stock LoginSystem_ShowLoginScreen(playerid)
{
    // Dialog-First: keine TextDraw-Overlays mehr. Alte TDs werden sauber entfernt
    // und der Passwort-Dialog (DIALOG_STYLE_PASSWORD) uebernimmt die Eingabe.
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    UiHub_ShowPasswordInput(playerid);
}

stock LoginSystem_ShowRegisterScreen(playerid)
{
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    UiHub_ShowPasswordInput(playerid);
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

stock LoginSystem_SetPlayerGender(playerid, gender)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    if (!Core_IsValidGender(gender))
    {
        gender = GENDER_NONE;
    }

    gPlayerGender[playerid] = gender;
    gPlayerGenderSelected[playerid] = (gender == GENDER_MALE || gender == GENDER_FEMALE);
    Core_SetPlayerGender(playerid, gender);
    gPlayerData[playerid][E_PLAYER_GENDER] = gender;
}

stock LoginSystem_RequireGenderSelection(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);
    TutorialSystem_HideStage(playerid);

    new const title[] = "Geschlecht";
    new const list[] = "Männlich\nWeiblich";
    UiDialog_Show(playerid, DIALOG_UI_GENDER_PICK, DIALOG_STYLE_LIST, title, list, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
}

stock bool:LoginSystem_HasRequiredGender(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new gender = Core_GetPlayerGender(playerid);
    return (gender == GENDER_MALE || gender == GENDER_FEMALE);
}

stock LoginSystem_ShowSkinPicker(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);
    TutorialSystem_HideStage(playerid);

    if (!LoginSystem_HasRequiredGender(playerid))
    {
        LoginSystem_RequireGenderSelection(playerid);
        return;
    }

    gPlayerSkinSelected[playerid] = false;
    gPlayerSkinChoice[playerid] = 0;

    new title[64];
    new skinList[512];
    new gender = Core_GetPlayerGender(playerid);
    if (gender == GENDER_MALE)
    {
        format(title, sizeof(title), "Männliche Skin-Auswahl");
        format(skinList, sizeof(skinList), "Skin 2\nSkin 4\nSkin 7\nSkin 23\nSkin 26\nSkin 30\nSkin 45\nSkin 73\nSkin 78\nSkin 96\nSkin 102\nSkin 104\nSkin 109\nSkin 124\nSkin 130");
    }
    else if (gender == GENDER_FEMALE)
    {
        format(title, sizeof(title), "Weibliche Skin-Auswahl");
        format(skinList, sizeof(skinList), "Skin 9\nSkin 10\nSkin 11\nSkin 12\nSkin 14\nSkin 15\nSkin 16\nSkin 17\nSkin 18\nSkin 20\nSkin 21\nSkin 22\nSkin 25\nSkin 27\nSkin 29");
    }
    else
    {
        format(title, sizeof(title), "Skin-Auswahl");
        format(skinList, sizeof(skinList), "Skin 2\nSkin 9\nSkin 14\nSkin 15\nSkin 23\nSkin 26\nSkin 29\nSkin 30\nSkin 45\nSkin 73\nSkin 78\nSkin 96\nSkin 104\nSkin 130\nSkin 212");
    }

    UiDialog_Show(playerid, DIALOG_UI_SKIN_PICK, DIALOG_STYLE_LIST, title, skinList, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
}

stock LoginSystem_ApplySkinChoice(playerid, skinid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    if (skinid < 0 || skinid > 311)
    {
        skinid = (gPlayerGender[playerid] == GENDER_FEMALE ? 9 : 2);
    }

    gPlayerSkinChoice[playerid] = skinid;
    gPlayerSkinSelected[playerid] = true;
    SetPlayerSkin(playerid, skinid);
    Core_SetPlayerSkin(playerid, skinid);
    gPlayerData[playerid][E_PLAYER_SKIN] = skinid;
    SendClientMessage(playerid, 0x00FF99AA, "[LOGIN] Skin ausgewählt. Der finale Spawn setzt den Charakter sauber durch.");
}

stock LoginSystem_PrepareSpawnInfo(playerid, stateid)
{
    new safeState = Core_GetValidStateOrDefault(stateid, Core_GetPlayerHomeState(playerid));
    if (!Core_IsValidState(safeState))
    {
        safeState = Core_GetValidStateOrDefault(Core_GetPlayerState(playerid), STATE_LOS_SANTOS);
    }
    if (!Core_IsValidState(safeState))
    {
        safeState = STATE_LOS_SANTOS;
    }

    new Float:x = 0.0;
    new Float:y = 0.0;
    new Float:z = 0.0;
    new Float:angle = 0.0;
    if (gStateSpawnCount[safeState] > 0)
    {
        StateSpawnSystem_GetStateSpawn(safeState, gStateSpawnCount[safeState] - 1, x, y, z, angle);
    }
    else
    {
        x = 1500.0; y = -1710.0; z = 13.0; angle = 0.0;
    }

    new skinid = gPlayerSkinChoice[playerid];
    if (skinid < 0 || skinid > 311)
    {
        skinid = Core_GetPlayerSkin(playerid);
    }
    if (skinid < 0 || skinid > 311)
    {
        skinid = gPlayerData[playerid][E_PLAYER_SKIN];
    }
    if (skinid < 0 || skinid > 311)
    {
        skinid = (gPlayerGender[playerid] == GENDER_FEMALE ? 9 : 2);
    }
    if (skinid < 0 || skinid > 311)
    {
        skinid = (gPlayerGender[playerid] == GENDER_FEMALE ? 9 : 2);
    }

    gPlayerSkinChoice[playerid] = skinid;
    gPlayerSkinSelected[playerid] = true;
    SetPlayerSkin(playerid, skinid);
    Core_SetPlayerSkin(playerid, skinid);
    gPlayerData[playerid][E_PLAYER_SKIN] = skinid;
    SetSpawnInfo(playerid, 255, skinid, x, y, z + 0.5, angle, 0, 0, 0, 0, 0, 0);
}

stock LoginSystem_BeginSession(playerid)
{
    gLoginSpawned[playerid] = false;
    gLoginSpawnPending[playerid] = false;
    TogglePlayerSpectating(playerid, true);
}

stock LoginSystem_ApplyPlayerWeather(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    if (gPlayerData[playerid][E_PLAYER_LEVEL] < 5)
    {
        if (gPlayerWeather[playerid] != 8)
        {
            gPlayerWeather[playerid] = 8;
            SetPlayerWeather(playerid, 8);
        }
        return;
    }

    if (gPlayerWeather[playerid] != 0)
    {
        gPlayerWeather[playerid] = 0;
        SetPlayerWeather(playerid, 0);
    }
}

stock LoginSystem_OnPlayerSpawn(playerid)
{
    gLoginSpawned[playerid] = true;
    LoginSystem_ApplyPlayerWeather(playerid);
    if (!gLoginSpawnPending[playerid])
    {
        return 0;
    }

    gLoginSpawnPending[playerid] = false;
    if (TutorialSystem_GetCurrentStage(playerid) < TUTORIAL_STAGE_FINISH)
    {
        TutorialSystem_Start(playerid);
        return 1;
    }
    return 0;
}

stock LoginSystem_SelectHomeState(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return;
    }

    Core_SetPlayerHomeState(playerid, stateid);
    Core_SetPlayerState(playerid, stateid);
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = stateid;
    gPlayerData[playerid][E_PLAYER_STATE] = stateid;
    gPlayerHomeStateSelected[playerid] = true;

    new message[128];
    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    format(message, sizeof(message), "[LOGIN] Heimatstadt vorgemerkt: %s", stateName);
    SendClientMessage(playerid, 0x00FF00AA, message);
}

stock LoginSystem_FinishAuth(playerid, const message[])
{
    new bool:isRegisterFlow = (gPlayerLoginState[playerid] == LOGIN_STATE_REGISTER);
    new homeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), Core_GetPlayerState(playerid));
    if (!Core_IsValidState(homeState))
    {
        homeState = STATE_LOS_SANTOS;
    }

    if (!gPlayerPasswordReady[playerid])
    {
        SendClientMessage(playerid, 0xFF6666AA, "[AUTH] Passwort muss gesetzt sein, bevor der Flow weiterlaufen darf.");
        UiHub_ShowPasswordInput(playerid);
        return;
    }

    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);

    if (isRegisterFlow)
    {
        if (!LoginSystem_HasRequiredGender(playerid))
        {
            LoginSystem_RequireGenderSelection(playerid);
            return;
        }

        if (TutorialSystem_GetCurrentStage(playerid) < TUTORIAL_STAGE_FINISH)
        {
            TutorialSystem_Start(playerid);
            return;
        }

        if (!Core_IsValidState(Core_GetPlayerHomeState(playerid)))
        {
            UiHub_ShowStatePick(playerid);
            return;
        }

        if (!gPlayerSkinSelected[playerid])
        {
            LoginSystem_ShowSkinPicker(playerid);
            return;
        }

        if (!gLoginSpawned[playerid])
        {
            gLoginSpawnPending[playerid] = true;
            TogglePlayerSpectating(playerid, false);
            return;
        }

        LoginSystem_ActivateSession(playerid, homeState);
        SendClientMessage(playerid, 0x00FF00AA, message);
        StateSpawnSystem_Apply(playerid, homeState);
        return;
    }

    if (!Core_IsValidState(Core_GetPlayerHomeState(playerid)))
    {
        UiHub_ShowStatePick(playerid);
        return;
    }

    if (!gPlayerSkinSelected[playerid] && Core_GetPlayerSkin(playerid) <= 0)
    {
        LoginSystem_ShowSkinPicker(playerid);
        return;
    }

    if (!gLoginSpawned[playerid])
    {
        gLoginSpawnPending[playerid] = true;
        TogglePlayerSpectating(playerid, false);
        return;
    }

    LoginSystem_ActivateSession(playerid, homeState);
    SendClientMessage(playerid, 0x00FF00AA, message);
    StateSpawnSystem_Apply(playerid, homeState);
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
        safeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), STATE_LOS_SANTOS);
    }
    if (!Core_IsValidState(safeState))
    {
        safeState = STATE_LOS_SANTOS;
    }

    new safeHomeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), safeState);
    if (!Core_IsValidState(safeHomeState))
    {
        safeHomeState = safeState;
    }

    if (!Core_IsValidState(safeState) || !Core_IsValidState(safeHomeState))
    {
        return;
    }

    Core_SetPlayerState(playerid, safeState);
    Core_SetPlayerHomeState(playerid, safeHomeState);
    gPlayerData[playerid][E_PLAYER_STATE] = safeState;
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = safeHomeState;
    gPlayerLoginState[playerid] = LOGIN_STATE_ACTIVE;
    gLoginAccountExists[playerid] = true;
    gPlayerPasswordReady[playerid] = true;
    gLoginSpawnPending[playerid] = false;
    gLoginSpawned[playerid] = false;
    TogglePlayerControllable(playerid, true);
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);
    TutorialSystem_HideStage(playerid);
    CameraSystem_ExitCameraMode(playerid);
    CameraSystem_Reset(playerid);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);

    DB_ValidateStateTransfer(playerid, safeState, safeHomeState);
    // Ab jetzt ist der Spieler authentifiziert -> persistente Schreibpfade frei.
    DB_MarkAuthenticated(playerid, true);
    DB_SavePlayerState(playerid, safeState);

    // Gespeichertes Bargeld autoritativ auf das Spielgeld anwenden (SQLite -> HUD).
    // Ueberschreibt den pauschalen Connect-Betrag, damit zurueckkehrende Spieler
    // ihren echten Kontostand sehen und neue Spieler mit dem Startwert starten.
    ResetPlayerMoney(playerid);
    new startCash = Core_GetPlayerCash(playerid);
    if (startCash != 0)
    {
        GivePlayerMoney(playerid, startCash);
    }

    // Inventar erst nach erfolgreichem Login laden (haelt OnPlayerConnect schlank).
    InventorySystem_OnPlayerAuthenticated(playerid);
    // Persistenten Admin-Rang in den In-Memory-Spiegel uebernehmen.
    AdminSystem_OnPlayerAuthenticated(playerid);
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

    if (!gPlayerPasswordReady[playerid])
    {
        UiHub_ShowPasswordInput(playerid);
        return;
    }

    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);

    // Bestehender Account: Account-Daten wurden bereits aus SQLite geladen.
    // Direkt in die gespeicherte Heimatstadt spawnen - kein Tutorial, kein Skin.
    new homeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), Core_GetPlayerState(playerid));
    if (!Core_IsValidState(homeState))
    {
        homeState = STATE_LOS_SANTOS;
    }

    gPlayerLoginState[playerid] = LOGIN_STATE_LOGIN;
    gPlayerHomeStateSelected[playerid] = true;
    gPlayerGenderSelected[playerid] = LoginSystem_HasRequiredGender(playerid);
    gPlayerSkinSelected[playerid] = true;

    LoginSystem_ActivateSession(playerid, homeState);
    LoginSystem_PrepareSpawnInfo(playerid, homeState);
    TogglePlayerSpectating(playerid, false);
    SpawnPlayer(playerid);
    StateSpawnSystem_Apply(playerid, homeState);
    RoleplayUI_ShowDashboard(playerid);
    SendClientMessage(playerid, 0x00FF00AA, "[LOGIN] Login erfolgreich. Willkommen zurueck in deiner Heimatstadt.");
}

stock LoginSystem_FinalizeRegisterSpawn(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    if (!gPlayerPasswordReady[playerid])
    {
        UiHub_ShowPasswordInput(playerid);
        return;
    }

    new homeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), Core_GetPlayerState(playerid));
    if (!Core_IsValidState(homeState))
    {
        homeState = STATE_LOS_SANTOS;
    }

    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);
    TutorialSystem_HideStage(playerid);

    // Account jetzt persistent in SQLite anlegen (namensbasiert, inkl. Passwort).
    new name[MAX_PLAYER_NAME];
    DB_ResolveAccountName(playerid, name, sizeof(name));
    DB_CreateAccount(playerid, name, gPlayerPendingPassword[playerid]);
    gPlayerPendingPassword[playerid][0] = EOS;

    gPlayerLoginState[playerid] = LOGIN_STATE_REGISTER;
    LoginSystem_ActivateSession(playerid, homeState);
    LoginSystem_PrepareSpawnInfo(playerid, homeState);
    TogglePlayerSpectating(playerid, false);
    SpawnPlayer(playerid);
    StateSpawnSystem_Apply(playerid, homeState);
    RoleplayUI_ShowDashboard(playerid);
    SendClientMessage(playerid, 0x00FF00AA, "[REGISTER] Registrierung abgeschlossen. Willkommen in San Andreas!");
}

stock LoginSystem_CompleteReg(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    gPlayerLoginState[playerid] = LOGIN_STATE_REGISTER;
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);

    // Deterministische Reihenfolge: Passwort -> Geschlecht -> Skin -> Heimatstadt
    // -> Tutorial -> finaler Spawn. Jeder Dialog ruft am Ende wieder CompleteReg
    // auf und springt so zum jeweils naechsten offenen Schritt.
    if (!gPlayerPasswordReady[playerid])
    {
        UiHub_ShowPasswordInput(playerid);
        return;
    }

    if (!LoginSystem_HasRequiredGender(playerid))
    {
        LoginSystem_RequireGenderSelection(playerid);
        return;
    }

    if (!gPlayerSkinSelected[playerid])
    {
        LoginSystem_ShowSkinPicker(playerid);
        return;
    }

    if (!gPlayerHomeStateSelected[playerid])
    {
        UiHub_ShowStatePick(playerid);
        return;
    }

    if (TutorialSystem_GetCurrentStage(playerid) < TUTORIAL_STAGE_FINISH)
    {
        TutorialSystem_Start(playerid);
        return;
    }

    LoginSystem_FinalizeRegisterSpawn(playerid);
}

#endif
