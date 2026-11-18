#if !defined TEXT_WARS_INC
#define TEXT_WARS_INC

#include <a_samp>

// Text-Wars: klickbares Live-Widget (PlayerTextDraws) und Dialog-Menü für Gangwar, Gangfight und Gebiete.
#define TW_DRAW_BOX 0
#define TW_DRAW_TITLE 1
#define TW_DRAW_WAR 2
#define TW_DRAW_FIGHT 3
#define TW_DRAW_LAND 4
#define TW_DRAW_FACTION 5
#define TW_BTN_STATUS 6
#define TW_BTN_LAND 7
#define TW_BTN_ATTACK 8
#define TW_BTN_TROOPS 9
#define TW_BTN_CLOSE 10
#define TW_DRAW_COUNT 11

#define TW_CLAIM_COST 25000
#define TW_MAX_PICK 12
#define TW_MODE_WAR 1
#define TW_MODE_FIGHT 2
#define TW_COLOR_SELECT 0x33CCFFFF

new PlayerText:gTWDraw[MAX_PLAYERS][TW_DRAW_COUNT];
new gTWVisible[MAX_PLAYERS];
new gTWSelecting[MAX_PLAYERS];
new gTWPickMode[MAX_PLAYERS];
new gTWPickFaction[MAX_PLAYERS][TW_MAX_PICK];
new gTWPickCount[MAX_PLAYERS];
new gTWTerritoryPick[MAX_PLAYERS];
new gTWTerritoryList[MAX_PLAYERS][MAX_TERRITORIES];
new gTWTerritoryCount[MAX_PLAYERS];
new gTWTimer = 0;

forward TextWars_Tick();

stock TextWars_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        TextWars_Reset(i);
    }

    gTWTimer = SetTimer("TextWars_Tick", 1000, true);
    print("[TEXT_WARS] Klickbare Kriegs-Widgets initialisiert");
    Core_MarkModuleReady("factions");
}

stock TextWars_Shutdown()
{
    if (gTWTimer != 0)
    {
        KillTimer(gTWTimer);
        gTWTimer = 0;
    }
}

stock TextWars_Reset(playerid)
{
    for (new d = 0; d < TW_DRAW_COUNT; d++)
    {
        gTWDraw[playerid][d] = PlayerText:INVALID_TEXT_DRAW;
    }
    gTWVisible[playerid] = 0;
    gTWSelecting[playerid] = 0;
    gTWPickMode[playerid] = 0;
    gTWPickCount[playerid] = 0;
    gTWTerritoryPick[playerid] = -1;
    gTWTerritoryCount[playerid] = 0;
}

stock TextWars_OnPlayerConnect(playerid)
{
    TextWars_Reset(playerid);
}

stock TextWars_OnPlayerDisconnect(playerid)
{
    TextWars_Reset(playerid);
}

stock int:TextWars_GetPlayerFaction(playerid)
{
    for (new f = 0; f < MAX_FACTIONS; f++)
    {
        if (gFactionIsActive[f] == 1 && FactionSystem_IsPlayerMember(playerid, f))
        {
            return f;
        }
    }
    return -1;
}

stock TextWars_IsFactionLeader(playerid, factionid)
{
    return factionid >= 0 && (gFactionLeader[factionid] == playerid || FactionSystem_GetRank(playerid) >= 3);
}

stock TextWars_GetFactionName(factionid, output[], len)
{
    if (factionid < 0 || factionid >= MAX_FACTIONS || gFactionIsActive[factionid] != 1)
    {
        format(output, len, "-");
        return;
    }
    format(output, len, "%s", gFactionName[factionid]);
}

stock TextWars_NotifyFaction(factionid, color, const message[])
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return;
    }
    for (new p = 0; p < MAX_PLAYERS; p++)
    {
        if (IsPlayerConnected(p) && FactionSystem_IsPlayerMember(p, factionid))
        {
            SendClientMessage(p, color, message);
        }
    }
}

stock PlayerText:TextWars_CreateLabel(playerid, Float:x, Float:y, text[], Float:lw, Float:lh, color)
{
    new PlayerText:t = CreatePlayerTextDraw(playerid, x, y, text);
    PlayerTextDrawLetterSize(playerid, t, lw, lh);
    PlayerTextDrawColor(playerid, t, color);
    PlayerTextDrawFont(playerid, t, 1);
    PlayerTextDrawSetOutline(playerid, t, 1);
    PlayerTextDrawSetProportional(playerid, t, 1);
    return t;
}

stock PlayerText:TextWars_CreateButton(playerid, Float:x, Float:y, text[])
{
    new PlayerText:t = TextWars_CreateLabel(playerid, x, y, text, 0.22, 1.0, 0xFFFFFFFF);
    PlayerTextDrawAlignment(playerid, t, 2);
    PlayerTextDrawUseBox(playerid, t, 1);
    PlayerTextDrawBoxColor(playerid, t, 0x224466BB);
    PlayerTextDrawTextSize(playerid, t, 14.0, 70.0);
    PlayerTextDrawSetSelectable(playerid, t, 1);
    return t;
}

stock TextWars_Hide(playerid)
{
    if (gTWSelecting[playerid] == 1)
    {
        CancelSelectTextDraw(playerid);
    }
    for (new d = 0; d < TW_DRAW_COUNT; d++)
    {
        if (gTWDraw[playerid][d] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, gTWDraw[playerid][d]);
            gTWDraw[playerid][d] = PlayerText:INVALID_TEXT_DRAW;
        }
    }
    gTWVisible[playerid] = 0;
    gTWSelecting[playerid] = 0;
}

stock TextWars_Show(playerid)
{
    if (!Core_AssertLifecycleStage(CORE_LIFECYCLE_RUNTIME_READY))
    {
        return;
    }

    TextWars_Hide(playerid);

    new PlayerText:box = CreatePlayerTextDraw(playerid, 470.0, 150.0, "_");
    PlayerTextDrawUseBox(playerid, box, 1);
    PlayerTextDrawBoxColor(playerid, box, 0x000000AA);
    PlayerTextDrawTextSize(playerid, box, 632.0, 0.0);
    PlayerTextDrawLetterSize(playerid, box, 0.0, 17.6);
    gTWDraw[playerid][TW_DRAW_BOX] = box;

    gTWDraw[playerid][TW_DRAW_TITLE] = TextWars_CreateLabel(playerid, 476.0, 153.0, "TEXT-WARS", 0.3, 1.2, 0xFF3333FF);
    gTWDraw[playerid][TW_DRAW_WAR] = TextWars_CreateLabel(playerid, 476.0, 172.0, "_", 0.2, 0.9, 0xFFFFFFFF);
    gTWDraw[playerid][TW_DRAW_FIGHT] = TextWars_CreateLabel(playerid, 476.0, 186.0, "_", 0.2, 0.9, 0xFFFF00FF);
    gTWDraw[playerid][TW_DRAW_LAND] = TextWars_CreateLabel(playerid, 476.0, 200.0, "_", 0.2, 0.9, 0x99FF99FF);
    gTWDraw[playerid][TW_DRAW_FACTION] = TextWars_CreateLabel(playerid, 476.0, 214.0, "_", 0.2, 0.9, 0x8CC6FFFF);

    gTWDraw[playerid][TW_BTN_STATUS] = TextWars_CreateButton(playerid, 513.0, 232.0, "STATUS");
    gTWDraw[playerid][TW_BTN_LAND] = TextWars_CreateButton(playerid, 592.0, 232.0, "GEBIETE");
    gTWDraw[playerid][TW_BTN_ATTACK] = TextWars_CreateButton(playerid, 513.0, 252.0, "ANGRIFF");
    gTWDraw[playerid][TW_BTN_TROOPS] = TextWars_CreateButton(playerid, 592.0, 252.0, "TRUPPEN");
    gTWDraw[playerid][TW_BTN_CLOSE] = TextWars_CreateButton(playerid, 552.0, 272.0, "SCHLIESSEN");

    for (new d = 0; d < TW_DRAW_COUNT; d++)
    {
        PlayerTextDrawShow(playerid, gTWDraw[playerid][d]);
    }

    gTWVisible[playerid] = 1;
    TextWars_Refresh(playerid);
    TextWars_Select(playerid);
}

// Aktiviert den Mauszeiger auf dem Widget, ESC gibt ihn wieder frei (Widget bleibt als Live-Anzeige).
stock TextWars_Select(playerid)
{
    if (gTWVisible[playerid] == 1)
    {
        SelectTextDraw(playerid, TW_COLOR_SELECT);
        gTWSelecting[playerid] = 1;
    }
}

stock TextWars_Toggle(playerid)
{
    if (gTWVisible[playerid] == 0)
    {
        TextWars_Show(playerid);
    }
    else if (gTWSelecting[playerid] == 0)
    {
        TextWars_Select(playerid);
    }
    else
    {
        TextWars_Hide(playerid);
    }
}

stock TextWars_Refresh(playerid)
{
    if (gTWVisible[playerid] == 0)
    {
        return;
    }

    new stateid = Core_GetPlayerState(playerid);
    new stateShort[8];
    Core_GetStateShortName(stateid, stateShort, sizeof(stateShort));

    new nameA[32], nameB[32];
    new line[96];

    if (GangWarSystem_IsActive(stateid))
    {
        TextWars_GetFactionName(GangWarSystem_GetFactionA(stateid), nameA, sizeof(nameA));
        TextWars_GetFactionName(GangWarSystem_GetFactionB(stateid), nameB, sizeof(nameB));
        format(line, sizeof(line), "Krieg %s: %s %d:%d %s", stateShort, nameA, GangWarSystem_GetScoreA(stateid), GangWarSystem_GetScoreB(stateid), nameB);
    }
    else
    {
        format(line, sizeof(line), "Krieg %s: kein aktiver Gangwar", stateShort);
    }
    PlayerTextDrawSetString(playerid, gTWDraw[playerid][TW_DRAW_WAR], line);

    new fightid = GangFightSystem_GetFight(stateid);
    if (fightid != -1)
    {
        format(line, sizeof(line), "Fight: %d:%d | %ds", GangFightSystem_GetScoreA(fightid), GangFightSystem_GetScoreB(fightid), GangFightSystem_GetTimeLeft(fightid));
    }
    else
    {
        format(line, sizeof(line), "Fight: keiner aktiv");
    }
    PlayerTextDrawSetString(playerid, gTWDraw[playerid][TW_DRAW_FIGHT], line);

    format(line, sizeof(line), "Gebiete LS:%d SF:%d LV:%d", TerritorySystem_GetControlCount(STATE_LOS_SANTOS), TerritorySystem_GetControlCount(STATE_SAN_FIERRO), TerritorySystem_GetControlCount(STATE_LAS_VENTURAS));
    PlayerTextDrawSetString(playerid, gTWDraw[playerid][TW_DRAW_LAND], line);

    new factionid = TextWars_GetPlayerFaction(playerid);
    if (factionid == -1)
    {
        format(line, sizeof(line), "Keine Fraktion");
    }
    else
    {
        format(line, sizeof(line), "%s | Kasse $%d", gFactionName[factionid], FactionSystem_GetBalance(factionid));
    }
    PlayerTextDrawSetString(playerid, gTWDraw[playerid][TW_DRAW_FACTION], line);
}

public TextWars_Tick()
{
    GangFightSystem_Tick();
    for (new p = 0; p < MAX_PLAYERS; p++)
    {
        if (IsPlayerConnected(p) && gTWVisible[p] == 1)
        {
            TextWars_Refresh(p);
        }
    }
    return 1;
}

stock TextWars_ShowMain(playerid)
{
    ShowPlayerDialog(playerid, DIALOG_WAR_MAIN, DIALOG_STYLE_LIST, "Text-Wars",
        "Kriegs- & Fight-Status\nGebiete\nGangwar erklären\nGangfight starten\nTruppenbewegungen\nLive-Widget umschalten",
        UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_CLOSE);
}

stock TextWars_ShowStatus(playerid)
{
    new stateid = Core_GetPlayerState(playerid);
    new stateName[32];
    new nameA[32], nameB[32];
    Core_GetStateName(stateid, stateName, sizeof(stateName));

    new info[512];
    if (GangWarSystem_IsActive(stateid))
    {
        TextWars_GetFactionName(GangWarSystem_GetFactionA(stateid), nameA, sizeof(nameA));
        TextWars_GetFactionName(GangWarSystem_GetFactionB(stateid), nameB, sizeof(nameB));
        format(info, sizeof(info), "Gangwar in %s\n%s  %d : %d  %s\n(Sieg ab 10 Punkten)\n", stateName, nameA, GangWarSystem_GetScoreA(stateid), GangWarSystem_GetScoreB(stateid), nameB);
    }
    else
    {
        format(info, sizeof(info), "Gangwar in %s: keiner aktiv\n", stateName);
    }

    new fightid = GangFightSystem_GetFight(stateid);
    new fightText[160];
    if (fightid != -1)
    {
        TextWars_GetFactionName(GangFightSystem_GetFactionA(fightid), nameA, sizeof(nameA));
        TextWars_GetFactionName(GangFightSystem_GetFactionB(fightid), nameB, sizeof(nameB));
        format(fightText, sizeof(fightText), "\nGangfight: %s %d : %d %s (%ds verbleibend)", nameA, GangFightSystem_GetScoreA(fightid), GangFightSystem_GetScoreB(fightid), nameB, GangFightSystem_GetTimeLeft(fightid));
    }
    else
    {
        format(fightText, sizeof(fightText), "\nGangfight: keiner aktiv");
    }
    strcat(info, fightText);
    ShowPlayerDialog(playerid, DIALOG_WAR_INFO, DIALOG_STYLE_MSGBOX, "Kriegsstatus", info, UI_DIALOG_BUTTON_BACK, UI_DIALOG_BUTTON_CLOSE);
}

stock TextWars_ShowTerritories(playerid)
{
    new stateid = Core_GetPlayerState(playerid);
    new list[1536];
    format(list, sizeof(list), "Gebiet\tTyp\tWert\tKontrolle\n");
    gTWTerritoryCount[playerid] = 0;

    for (new n = 0; n < MAX_TERRITORIES; n++)
    {
        new tid = TerritorySystem_GetNthForState(stateid, n);
        if (tid == -1)
        {
            break;
        }

        new name[40];
        new ctrl[16];
        TerritorySystem_GetName(tid, name, sizeof(name));
        Core_GetStateShortName(TerritorySystem_GetControl(tid), ctrl, sizeof(ctrl));

        new line[128];
        format(line, sizeof(line), "%s\t%d\t$%d\t%s\n", name, TerritorySystem_GetType(tid), TerritorySystem_GetValue(tid), ctrl);
        strcat(list, line);
        gTWTerritoryList[playerid][gTWTerritoryCount[playerid]++] = tid;
    }

    ShowPlayerDialog(playerid, DIALOG_WAR_TERRITORY_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Gebiete deines Staates", list, "Wählen", UI_DIALOG_BUTTON_BACK);
}

stock TextWars_ShowFactionPick(playerid, mode)
{
    new own = TextWars_GetPlayerFaction(playerid);
    if (own == -1 || !TextWars_IsFactionLeader(playerid, own))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[TEXT_WARS] Nur Fraktionsleiter können Angriffe koordinieren.");
        return;
    }

    new list[512];
    gTWPickCount[playerid] = 0;
    gTWPickMode[playerid] = mode;
    for (new f = 0; f < MAX_FACTIONS && gTWPickCount[playerid] < TW_MAX_PICK; f++)
    {
        if (f == own || gFactionIsActive[f] != 1)
        {
            continue;
        }
        if (gFactionType[f] != FACTION_TYPE_GANG && gFactionType[f] != FACTION_TYPE_LEGAL)
        {
            continue;
        }

        new line[64];
        format(line, sizeof(line), "%s (Staat %d)\n", gFactionName[f], gFactionState[f]);
        strcat(list, line);
        gTWPickFaction[playerid][gTWPickCount[playerid]++] = f;
    }

    if (gTWPickCount[playerid] == 0)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[TEXT_WARS] Keine angreifbaren Fraktionen vorhanden.");
        return;
    }

    ShowPlayerDialog(playerid, DIALOG_WAR_FACTION_PICK, DIALOG_STYLE_LIST, (mode == TW_MODE_WAR) ? ("Gangwar erklären gegen...") : ("Gangfight starten gegen..."), list, "Angreifen", UI_DIALOG_BUTTON_BACK);
}

stock TextWars_ShowTroops(playerid)
{
    new own = TextWars_GetPlayerFaction(playerid);
    if (own == -1)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[TEXT_WARS] Du gehörst keiner Fraktion an.");
        return;
    }

    new list[1024];
    format(list, sizeof(list), "Mitglied\tHP\tArmor\tPosition\n");
    for (new p = 0; p < MAX_PLAYERS; p++)
    {
        if (!IsPlayerConnected(p) || !FactionSystem_IsPlayerMember(p, own))
        {
            continue;
        }

        new name[MAX_PLAYER_NAME];
        new Float:hp, Float:armor, Float:x, Float:y, Float:z;
        GetPlayerName(p, name, sizeof(name));
        GetPlayerHealth(p, hp);
        GetPlayerArmour(p, armor);
        GetPlayerPos(p, x, y, z);

        new line[96];
        format(line, sizeof(line), "%s\t%.0f\t%.0f\t%.0f, %.0f\n", name, hp, armor, x, y);
        strcat(list, line);
    }
    ShowPlayerDialog(playerid, DIALOG_WAR_INFO, DIALOG_STYLE_TABLIST_HEADERS, "Truppenbewegungen", list, UI_DIALOG_BUTTON_BACK, UI_DIALOG_BUTTON_CLOSE);
}

stock TextWars_StartAttack(playerid, targetFaction, mode)
{
    new own = TextWars_GetPlayerFaction(playerid);
    if (own == -1 || !TextWars_IsFactionLeader(playerid, own) || targetFaction < 0)
    {
        return false;
    }

    new stateid = FactionSystem_GetState(own);
    new bool:started = false;
    if (mode == TW_MODE_WAR)
    {
        started = GangWarSystem_StartWar(stateid, own, targetFaction);
    }
    else
    {
        started = GangFightSystem_StartFight(stateid, own, targetFaction);
    }

    if (!started)
    {
        SendClientMessage(playerid, 0xFF6666AA, "[TEXT_WARS] Angriff nicht möglich (läuft bereits oder deaktiviert).");
        return false;
    }

    new msg[128];
    format(msg, sizeof(msg), "[TEXT_WARS] %s: %s gegen %s gestartet!", (mode == TW_MODE_WAR) ? ("GANGWAR") : ("GANGFIGHT"), gFactionName[own], gFactionName[targetFaction]);
    TextWars_NotifyFaction(own, 0xFF3333FF, msg);
    TextWars_NotifyFaction(targetFaction, 0xFF3333FF, msg);
    return true;
}

// Punkte für Kills zwischen Kriegsgegnern.
stock TextWars_OnPlayerDeath(playerid, killerid)
{
    if (!IsPlayerConnected(killerid) || killerid == playerid)
    {
        return;
    }

    new killerFaction = TextWars_GetPlayerFaction(killerid);
    new victimFaction = TextWars_GetPlayerFaction(playerid);
    if (killerFaction == -1 || victimFaction == -1 || killerFaction == victimFaction)
    {
        return;
    }

    for (new s = 0; s < STATE_COUNT; s++)
    {
        if (!GangWarSystem_IsActive(s))
        {
            continue;
        }
        new a = GangWarSystem_GetFactionA(s);
        new b = GangWarSystem_GetFactionB(s);
        if ((killerFaction == a && victimFaction == b) || (killerFaction == b && victimFaction == a))
        {
            GangWarSystem_AddScore(s, killerFaction, 1);
        }
    }

    new fightid = GangFightSystem_GetFight(FactionSystem_GetState(killerFaction));
    if (fightid != -1)
    {
        new fa = GangFightSystem_GetFactionA(fightid);
        new fb = GangFightSystem_GetFactionB(fightid);
        if ((killerFaction == fa && victimFaction == fb) || (killerFaction == fb && victimFaction == fa))
        {
            GangFightSystem_AddScore(fightid, killerFaction, 1);
        }
    }
}

stock TextWars_ClaimTerritory(playerid, territoryId)
{
    new own = TextWars_GetPlayerFaction(playerid);
    if (own == -1 || !TextWars_IsFactionLeader(playerid, own))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[TEXT_WARS] Nur Fraktionsleiter können Gebiete beanspruchen.");
        return false;
    }

    new newState = FactionSystem_GetState(own);
    if (!FactionSystem_DeductFromBalance(own, TW_CLAIM_COST))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[TEXT_WARS] Die Fraktionskasse reicht nicht ($25000 nötig).");
        return false;
    }
    if (!TerritorySystem_ClaimTerritory(playerid, territoryId, newState))
    {
        FactionSystem_TransferToBalance(own, TW_CLAIM_COST);
        return false;
    }
    StateSystem_ApplyTreasuryDelta(newState, TW_CLAIM_COST / 5);
    return true;
}

stock TextWars_HandleButton(playerid, d)
{
    switch (d)
    {
        case TW_BTN_STATUS: TextWars_ShowStatus(playerid);
        case TW_BTN_LAND: TextWars_ShowTerritories(playerid);
        case TW_BTN_ATTACK: TextWars_ShowMain(playerid);
        case TW_BTN_TROOPS: TextWars_ShowTroops(playerid);
        case TW_BTN_CLOSE: TextWars_Hide(playerid);
    }
}

stock TextWars_OnClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    for (new d = TW_BTN_STATUS; d < TW_DRAW_COUNT; d++)
    {
        if (gTWDraw[playerid][d] == playertextid && gTWVisible[playerid] == 1)
        {
            // Dialoge brauchen einen freien Mauszeiger.
            if (d != TW_BTN_CLOSE)
            {
                CancelSelectTextDraw(playerid);
                gTWSelecting[playerid] = 0;
            }
            TextWars_HandleButton(playerid, d);
            return true;
        }
    }
    return false;
}

stock TextWars_OnClickTextDraw(playerid, Text:clickedid)
{
    // ESC: Zeiger freigeben, Widget bleibt als Live-Anzeige bestehen.
    if (clickedid == Text:INVALID_TEXT_DRAW && gTWSelecting[playerid] == 1)
    {
        gTWSelecting[playerid] = 0;
    }
}

stock TextWars_OnDialogResponse(playerid, dialogid, response, listitem, const inputtext[])
{
    #pragma unused inputtext
    if (dialogid < DIALOG_UI_RANGE_WAR_FIRST || dialogid > DIALOG_UI_RANGE_WAR_LAST)
    {
        return false;
    }

    switch (dialogid)
    {
        case DIALOG_WAR_MAIN:
        {
            if (!response)
            {
                return true;
            }
            switch (listitem)
            {
                case 0: TextWars_ShowStatus(playerid);
                case 1: TextWars_ShowTerritories(playerid);
                case 2: TextWars_ShowFactionPick(playerid, TW_MODE_WAR);
                case 3: TextWars_ShowFactionPick(playerid, TW_MODE_FIGHT);
                case 4: TextWars_ShowTroops(playerid);
                case 5: TextWars_Toggle(playerid);
            }
        }
        case DIALOG_WAR_INFO:
        {
            if (response)
            {
                TextWars_ShowMain(playerid);
            }
        }
        case DIALOG_WAR_TERRITORY_LIST:
        {
            if (!response)
            {
                TextWars_ShowMain(playerid);
                return true;
            }
            if (listitem < 0 || listitem >= gTWTerritoryCount[playerid])
            {
                return true;
            }
            gTWTerritoryPick[playerid] = gTWTerritoryList[playerid][listitem];

            new name[40];
            TerritorySystem_GetName(gTWTerritoryPick[playerid], name, sizeof(name));
            new text[160];
            format(text, sizeof(text), "%s beanspruchen?\nKosten: $%d aus der Fraktionskasse.\nNur für Fraktionsleiter.", name, TW_CLAIM_COST);
            ShowPlayerDialog(playerid, DIALOG_WAR_TERRITORY_ACTION, DIALOG_STYLE_MSGBOX, "Gebiet beanspruchen", text, "Beanspruchen", UI_DIALOG_BUTTON_BACK);
        }
        case DIALOG_WAR_TERRITORY_ACTION:
        {
            if (!response)
            {
                TextWars_ShowTerritories(playerid);
                return true;
            }
            TextWars_ClaimTerritory(playerid, gTWTerritoryPick[playerid]);
        }
        case DIALOG_WAR_FACTION_PICK:
        {
            if (!response)
            {
                TextWars_ShowMain(playerid);
                return true;
            }
            if (listitem < 0 || listitem >= gTWPickCount[playerid])
            {
                return true;
            }
            TextWars_StartAttack(playerid, gTWPickFaction[playerid][listitem], gTWPickMode[playerid]);
        }
    }
    return true;
}

stock TextWars_HandleCommand(playerid, const cmdtext[])
{
    if (strcmp(cmdtext, "/textwars", true) == 0 || strcmp(cmdtext, "/warmenu", true) == 0)
    {
        TextWars_ShowMain(playerid);
        return true;
    }
    if (strcmp(cmdtext, "/warhud", true) == 0 || strcmp(cmdtext, "/gangwar", true) == 0 || strcmp(cmdtext, "/gangfight", true) == 0)
    {
        TextWars_Show(playerid);
        return true;
    }
    if (strcmp(cmdtext, "/territories", true) == 0)
    {
        TextWars_ShowTerritories(playerid);
        return true;
    }
    return false;
}

#endif
