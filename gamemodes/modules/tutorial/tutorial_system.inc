#if !defined TUTORIAL_SYSTEM_INC
#define TUTORIAL_SYSTEM_INC

#include <a_samp>

#define TUTORIAL_STAGE_NONE 0
#define TUTORIAL_STAGE_INTRO 1
#define TUTORIAL_STAGE_HOME 2
#define TUTORIAL_STAGE_GOV 3
#define TUTORIAL_STAGE_FINISH 4

new gTutorialStage[MAX_PLAYERS];
new Text:gTutorialTD[MAX_PLAYERS][4];

stock TutorialSystem_Initialize()
{
    print("[TUTORIAL] Tutorialsystem aktiviert");
}

stock TutorialSystem_Start(playerid)
{
    gTutorialStage[playerid] = TUTORIAL_STAGE_INTRO;
    TutorialSystem_ShowStage(playerid, gTutorialStage[playerid]);
    SendClientMessage(playerid, 0xFFCC00AA, "[TUTORIAL] Willkommen im GTA:SA Reallife. Folge dem kurzen Einstieg.");
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

    if (stage == TUTORIAL_STAGE_INTRO)
    {
        new const tutorialTitle[] = "Tutorial";
        new const tutorialWelcome[] = "Willkommen in Los Santos";
        new const tutorialIntro[] = "Nutze /help, /city und /gov, um loszulegen.";

        gTutorialTD[playerid][0] = UiDialog_TextDrawCreate(320.0, 120.0, tutorialTitle);
        TextDrawAlignment(gTutorialTD[playerid][0], 2);
        TextDrawLetterSize(gTutorialTD[playerid][0], 0.6, 1.8);
        TextDrawColor(gTutorialTD[playerid][0], 0x00D9FFFF);

        gTutorialTD[playerid][1] = UiDialog_TextDrawCreate(320.0, 160.0, tutorialWelcome);
        TextDrawAlignment(gTutorialTD[playerid][1], 2);
        TextDrawLetterSize(gTutorialTD[playerid][1], 0.35, 1.2);
        TextDrawColor(gTutorialTD[playerid][1], 0xFFFFFFFF);

        gTutorialTD[playerid][2] = UiDialog_TextDrawCreate(320.0, 185.0, tutorialIntro);
        TextDrawAlignment(gTutorialTD[playerid][2], 2);
        TextDrawLetterSize(gTutorialTD[playerid][2], 0.28, 1.0);
        TextDrawColor(gTutorialTD[playerid][2], 0xFFCC66FF);
    }
    else if (stage == TUTORIAL_STAGE_HOME)
    {
        new const tutorialHomeTitle[] = "Heimatstadt";
        new const tutorialHomeDesc[] = "Deine Heimatstadt bestimmt dein Spawn und deine Regierung.";

        gTutorialTD[playerid][0] = UiDialog_TextDrawCreate(320.0, 120.0, tutorialHomeTitle);
        TextDrawAlignment(gTutorialTD[playerid][0], 2);
        TextDrawLetterSize(gTutorialTD[playerid][0], 0.55, 1.6);
        TextDrawColor(gTutorialTD[playerid][0], 0x00FF00AA);

        gTutorialTD[playerid][1] = UiDialog_TextDrawCreate(320.0, 160.0, tutorialHomeDesc);
        TextDrawAlignment(gTutorialTD[playerid][1], 2);
        TextDrawLetterSize(gTutorialTD[playerid][1], 0.28, 1.0);
        TextDrawColor(gTutorialTD[playerid][1], 0xFFFFFFFF);
    }
    else if (stage == TUTORIAL_STAGE_GOV)
    {
        new const tutorialGovTitle[] = "Regierung";
        new const tutorialGovDesc[] = "Jeder Staat hat eine eigene Kasse, Verwaltung und Befehle.";

        gTutorialTD[playerid][0] = UiDialog_TextDrawCreate(320.0, 120.0, tutorialGovTitle);
        TextDrawAlignment(gTutorialTD[playerid][0], 2);
        TextDrawLetterSize(gTutorialTD[playerid][0], 0.6, 1.7);
        TextDrawColor(gTutorialTD[playerid][0], 0x8CC6FFFF);

        gTutorialTD[playerid][1] = UiDialog_TextDrawCreate(320.0, 160.0, tutorialGovDesc);
        TextDrawAlignment(gTutorialTD[playerid][1], 2);
        TextDrawLetterSize(gTutorialTD[playerid][1], 0.28, 1.0);
        TextDrawColor(gTutorialTD[playerid][1], 0xFFFFFFFF);
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
        if (gTutorialTD[playerid][i] != Text:0)
        {
            TextDrawShowForPlayer(playerid, gTutorialTD[playerid][i]);
        }
    }
}

stock TutorialSystem_HideStage(playerid)
{
    for (new i = 0; i < 4; i++)
    {
        if (gTutorialTD[playerid][i] != Text:0)
        {
            TextDrawHideForPlayer(playerid, gTutorialTD[playerid][i]);
            TextDrawDestroy(gTutorialTD[playerid][i]);
            gTutorialTD[playerid][i] = Text:0;
        }
    }
}

stock TutorialSystem_Next(playerid)
{
    if (gTutorialStage[playerid] < TUTORIAL_STAGE_FINISH)
    {
        gTutorialStage[playerid]++;
    }
    else
    {
        gTutorialStage[playerid] = TUTORIAL_STAGE_FINISH;
    }
    TutorialSystem_ShowStage(playerid, gTutorialStage[playerid]);

    if (gTutorialStage[playerid] == TUTORIAL_STAGE_FINISH)
    {
        new stateid = Core_GetPlayerHomeState(playerid);
        if (!Core_IsValidState(stateid))
        {
            stateid = Core_GetValidStateOrDefault(Core_GetPlayerState(playerid), STATE_LOS_SANTOS);
            Core_SetPlayerHomeState(playerid, stateid);
        }

        CameraSystem_Reset(playerid);
        CameraSystem_PlayStateScene(playerid, stateid);
        UiHub_ShowStatePick(playerid);
    }
}

stock int:TutorialSystem_GetCurrentStage(playerid)
{
    return gTutorialStage[playerid];
}

#endif
