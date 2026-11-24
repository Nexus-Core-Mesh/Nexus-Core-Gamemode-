#if !defined TUTORIAL_SYSTEM_INC
#define TUTORIAL_SYSTEM_INC

#include <a_samp>

#define TUTORIAL_STAGE_NONE 0
#define TUTORIAL_STAGE_INTRO 1
#define TUTORIAL_STAGE_HOME 2
#define TUTORIAL_STAGE_GOV 3
#define TUTORIAL_STAGE_FINISH 10
#define TUTORIAL_SCENE_COUNT 9
#define TUTORIAL_SCENE_TIME 11500

new Text:gTutorialTD[MAX_PLAYERS][4];
new gTutorialToken[MAX_PLAYERS];

forward TutorialSystem_AutoNext(playerid, token);

new const gTutorialSceneTitle[TUTORIAL_SCENE_COUNT][40] = {
    "Los Santos - Die Skyline",
    "Los Santos - Santa Maria Beach",
    "Los Santos - Grove Street",
    "San Fierro - Die Bucht",
    "San Fierro - Easter Basin",
    "San Fierro - Chinatown",
    "Las Venturas - Der Strip",
    "Las Venturas - Die Casinos",
    "Las Venturas - Die Wueste"
};

new const gTutorialSceneText[TUTORIAL_SCENE_COUNT][2][112] = {
    {"Willkommen in San Andreas. Dieser Staat besteht aus drei Staedten mit eigener Regierung.", "Los Santos ist die Metropole: viele Jobs, hohe Kriminalitaet, starke Wirtschaft."},
    {"Am Strand beginnt fuer viele der Aufstieg: Fischer, Fahrer, Lieferanten.", "Mit /jobs findest du deinen ersten Job und verdienst dein erstes Geld."},
    {"Grove Street ist Gang-Revier. Hier zaehlen Ruf und Zusammenhalt.", "Polizei und Gangs kontrollieren die Strassen - achte auf dein Wanted-Level (/wanted)."},
    {"San Fierro ist die Stadt der Technik und des Handels.", "Der Staat verwaltet eine eigene Kasse und setzt Steuern fest (/treasury)."},
    {"Hafen und Flughafen verbinden die Staaten miteinander.", "Transportjobs und Routen (/routes) bringen dir Waren und Geld zwischen den Staedten."},
    {"In Chinatown laufen Maerkte, Shops und Gewerbe (/market, /shops).", "Mit einem eigenen Geschaeft nimmst du an der Staatswirtschaft teil."},
    {"Las Venturas lebt vom Spiel und vom Nachtleben.", "Casinos, Events und Saisons bringen Abwechslung (/city, /gov)."},
    {"Hinter den Fassaden entscheidet die Regierung ueber Gesetze und Dienste.", "Waehle gleich deine Heimatstadt - dort startest du und gehoerst zum Staat."},
    {"Zwischen den Staedten liegt die Wueste - Platz fuer Fahrzeuge, Haeuser und Abenteuer.", "Mit /menu oeffnest du jederzeit das Hauptmenue. Viel Erfolg in GTA:SA Reallife!"}
};

// fromPos(xyz), toPos(xyz), fromLook(xyz), toLook(xyz)
new const Float:gTutorialSceneCam[TUTORIAL_SCENE_COUNT][12] = {
    {1420.0, -1525.0, 130.0,  1605.0, -1510.0, 118.0,  1495.0, -1625.0, 45.0,  1515.0, -1645.0, 45.0},
    {350.0, -1890.0, 30.0,    430.0, -1825.0, 20.0,   385.0, -1985.0, 10.0,   365.0, -1985.0, 10.0},
    {2470.0, -1725.0, 28.0,  2525.0, -1645.0, 20.0,  2490.0, -1675.0, 12.0,  2490.0, -1675.0, 12.0},
    {-1985.0, 310.0, 92.0,   -1895.0, 170.0, 70.0,  -1950.0, 160.0, 34.0,  -1925.0, 115.0, 30.0},
    {-1545.0, -110.0, 42.0,  -1490.0, -215.0, 32.0,  -1410.0, -255.0, 18.0,  -1400.0, -250.0, 18.0},
    {-2145.0, 720.0, 72.0,   -2085.0, 640.0, 60.0,  -2140.0, 680.0, 28.0,  -2140.0, 660.0, 28.0},
    {2055.0, 980.0, 50.0,    2000.0, 1105.0, 45.0,  2025.0, 1000.0, 18.0,  2035.0, 1000.0, 16.0},
    {2205.0, 1585.0, 42.0,   2140.0, 1715.0, 38.0,  2205.0, 1660.0, 14.0,  2205.0, 1680.0, 12.0},
    {2260.0, 2085.0, 45.0,   2300.0, 1985.0, 35.0,  2285.0, 2035.0, 12.0,  2290.0, 2035.0, 10.0}
};

stock TutorialSystem_Initialize()
{
    print("[TUTORIAL] Tutorialsystem aktiviert");
}

stock TutorialSystem_Reset(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    TutorialSystem_HideStage(playerid);
    gTutorialStage[playerid] = TUTORIAL_STAGE_NONE;
}

stock TutorialSystem_MarkFinished(playerid)
{
    TutorialSystem_HideStage(playerid);
    gTutorialStage[playerid] = TUTORIAL_STAGE_FINISH;
}

stock TutorialSystem_Start(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    if (gPlayerLoginState[playerid] == LOGIN_STATE_ACTIVE)
    {
        return;
    }

    if (gPlayerLoginState[playerid] != LOGIN_STATE_REGISTER)
    {
        return;
    }

    if (!gPlayerPasswordReady[playerid])
    {
        SendClientMessage(playerid, 0xFF6666AA, "[TUTORIAL] Passwort muss vor dem Tutorial gesetzt sein.");
        UiHub_ShowPasswordInput(playerid);
        return;
    }

    if (gTutorialStage[playerid] >= TUTORIAL_STAGE_FINISH)
    {
        TutorialSystem_MarkFinished(playerid);
        return;
    }

    if (gTutorialStage[playerid] > TUTORIAL_STAGE_NONE && gTutorialStage[playerid] < TUTORIAL_STAGE_FINISH)
    {
        return;
    }

    TutorialSystem_Reset(playerid);
    gTutorialStage[playerid] = TUTORIAL_STAGE_INTRO;
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);
    CameraSystem_Reset(playerid);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    TogglePlayerControllable(playerid, false);
    TutorialSystem_ShowStage(playerid, gTutorialStage[playerid]);
    SendClientMessage(playerid, 0xFFCC00AA, "[TUTORIAL] Willkommen im GTA:SA Reallife. Der Kameraflug zeigt dir alle drei Staedte. Danach waehlst du dein Geschlecht und deine Heimatstadt.");
}

stock TutorialSystem_ShowStage(playerid, stage)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    if (stage < TUTORIAL_STAGE_NONE || stage > TUTORIAL_STAGE_FINISH)
    {
        stage = TUTORIAL_STAGE_NONE;
    }

    TutorialSystem_HideStage(playerid);
    gTutorialToken[playerid]++;

    if (stage >= TUTORIAL_STAGE_INTRO && stage < TUTORIAL_STAGE_FINISH)
    {
        new scene = stage - 1;
        new counter[48];
        format(counter, sizeof(counter), "Szene %d/%d", stage, TUTORIAL_SCENE_COUNT);

        gTutorialTD[playerid][0] = UiDialog_TextDrawCreate(320.0, 300.0, gTutorialSceneTitle[scene]);
        TextDrawAlignment(gTutorialTD[playerid][0], 2);
        TextDrawLetterSize(gTutorialTD[playerid][0], 0.55, 1.7);
        TextDrawColor(gTutorialTD[playerid][0], 0x00D9FFFF);
        TextDrawSetOutline(gTutorialTD[playerid][0], 1);

        gTutorialTD[playerid][1] = UiDialog_TextDrawCreate(320.0, 335.0, gTutorialSceneText[scene][0]);
        TextDrawAlignment(gTutorialTD[playerid][1], 2);
        TextDrawLetterSize(gTutorialTD[playerid][1], 0.26, 1.0);
        TextDrawColor(gTutorialTD[playerid][1], 0xFFFFFFFF);
        TextDrawSetOutline(gTutorialTD[playerid][1], 1);

        gTutorialTD[playerid][2] = UiDialog_TextDrawCreate(320.0, 355.0, gTutorialSceneText[scene][1]);
        TextDrawAlignment(gTutorialTD[playerid][2], 2);
        TextDrawLetterSize(gTutorialTD[playerid][2], 0.26, 1.0);
        TextDrawColor(gTutorialTD[playerid][2], 0xFFCC66FF);
        TextDrawSetOutline(gTutorialTD[playerid][2], 1);

        gTutorialTD[playerid][3] = UiDialog_TextDrawCreate(320.0, 385.0, counter);
        TextDrawAlignment(gTutorialTD[playerid][3], 2);
        TextDrawLetterSize(gTutorialTD[playerid][3], 0.22, 0.9);
        TextDrawColor(gTutorialTD[playerid][3], 0xBBBBBBFF);
        TextDrawSetOutline(gTutorialTD[playerid][3], 1);

        InterpolateCameraPos(playerid,
            gTutorialSceneCam[scene][0], gTutorialSceneCam[scene][1], gTutorialSceneCam[scene][2],
            gTutorialSceneCam[scene][3], gTutorialSceneCam[scene][4], gTutorialSceneCam[scene][5],
            TUTORIAL_SCENE_TIME - 500, CAMERA_MOVE);
        InterpolateCameraLookAt(playerid,
            gTutorialSceneCam[scene][6], gTutorialSceneCam[scene][7], gTutorialSceneCam[scene][8],
            gTutorialSceneCam[scene][9], gTutorialSceneCam[scene][10], gTutorialSceneCam[scene][11],
            TUTORIAL_SCENE_TIME - 500, CAMERA_MOVE);
        SetTimerEx("TutorialSystem_AutoNext", TUTORIAL_SCENE_TIME, false, "ii", playerid, gTutorialToken[playerid]);
    }
    else if (stage == TUTORIAL_STAGE_FINISH)
    {
        new const tutorialDoneTitle[] = "Tutorial abgeschlossen";
        new const tutorialDoneDesc[] = "Du bist bereit f~r die Welt von GTA:SA Reallife.";

        gTutorialTD[playerid][0] = UiDialog_TextDrawCreate(320.0, 120.0, tutorialDoneTitle);
        TextDrawAlignment(gTutorialTD[playerid][0], 2);
        TextDrawLetterSize(gTutorialTD[playerid][0], 0.55, 1.6);
        TextDrawColor(gTutorialTD[playerid][0], 0x00FF00AA);

        gTutorialTD[playerid][1] = UiDialog_TextDrawCreate(320.0, 160.0, tutorialDoneDesc);
        TextDrawAlignment(gTutorialTD[playerid][1], 2);
        TextDrawLetterSize(gTutorialTD[playerid][1], 0.28, 1.0);
        TextDrawColor(gTutorialTD[playerid][1], 0xFFFFFFFF);
    }

    for (new i = 0; i < 4; i++)
    {
        if (gTutorialTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawShowForPlayer(playerid, gTutorialTD[playerid][i]);
        }
    }
}

stock TutorialSystem_HideStage(playerid)
{
    CancelSelectTextDraw(playerid);
    for (new i = 0; i < 4; i++)
    {
        if (gTutorialTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gTutorialTD[playerid][i]);
            TextDrawDestroy(gTutorialTD[playerid][i]);
            gTutorialTD[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }
}

public TutorialSystem_AutoNext(playerid, token)
{
    if (!IsPlayerConnected(playerid) || gTutorialToken[playerid] != token)
    {
        return 0;
    }
    if (gTutorialStage[playerid] <= TUTORIAL_STAGE_NONE || gTutorialStage[playerid] >= TUTORIAL_STAGE_FINISH)
    {
        return 0;
    }
    TutorialSystem_Next(playerid);
    return 1;
}

stock TutorialSystem_Next(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    if (gPlayerLoginState[playerid] == LOGIN_STATE_ACTIVE)
    {
        return;
    }

    if (gPlayerLoginState[playerid] != LOGIN_STATE_REGISTER)
    {
        return;
    }

    if (!gPlayerPasswordReady[playerid])
    {
        SendClientMessage(playerid, 0xFF6666AA, "[TUTORIAL] Passwort muss vor dem Tutorial beendet werden.");
        UiHub_ShowPasswordInput(playerid);
        return;
    }

    if (gTutorialStage[playerid] < TUTORIAL_STAGE_FINISH)
    {
        gTutorialStage[playerid]++;
    }
    else
    {
        gTutorialStage[playerid] = TUTORIAL_STAGE_FINISH;
    }

    if (gTutorialStage[playerid] == TUTORIAL_STAGE_FINISH)
    {
        TutorialSystem_HideStage(playerid);
        CameraSystem_Reset(playerid);
        TogglePlayerControllable(playerid, false);
        LoginSystem_RequireGenderSelection(playerid);
        SendClientMessage(playerid, 0x00FF00AA, "[TUTORIAL] Tutorial abgeschlossen. Wähle jetzt dein Geschlecht und danach deine Heimatstadt.");
        return;
    }

    TutorialSystem_ShowStage(playerid, gTutorialStage[playerid]);
}

stock int:TutorialSystem_GetCurrentStage(playerid)
{
    return gTutorialStage[playerid];
}

#endif
