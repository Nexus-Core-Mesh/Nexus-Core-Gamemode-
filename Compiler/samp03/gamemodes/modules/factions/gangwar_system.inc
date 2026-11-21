#if !defined GANGWAR_SYSTEM_INC
#define GANGWAR_SYSTEM_INC

#include <a_samp>

new gGangWarEnabled = 1;
new gGangWarActive[3];
new gGangWarFactionA[3];
new gGangWarFactionB[3];
new gGangWarScoreA[3];
new gGangWarScoreB[3];
new Text:gGangWarTD[MAX_PLAYERS][8];

stock GangWarSystem_Initialize()
{
    for (new i = 0; i < 3; i++)
    {
        gGangWarActive[i] = 0;
        gGangWarFactionA[i] = -1;
        gGangWarFactionB[i] = -1;
        gGangWarScoreA[i] = 0;
        gGangWarScoreB[i] = 0;
    }

    print("[GANGWAR] Gangwar-System initialisiert");
    Core_MarkModuleReady("factions");
}

stock GangWarSystem_IsActive(stateid)
{
    return Core_IsValidState(stateid) && gGangWarActive[stateid] == 1;
}

stock int:GangWarSystem_GetFactionA(stateid)
{
    return Core_IsValidState(stateid) ? gGangWarFactionA[stateid] : -1;
}

stock int:GangWarSystem_GetFactionB(stateid)
{
    return Core_IsValidState(stateid) ? gGangWarFactionB[stateid] : -1;
}

stock int:GangWarSystem_GetScoreA(stateid)
{
    return Core_IsValidState(stateid) ? gGangWarScoreA[stateid] : 0;
}

stock int:GangWarSystem_GetScoreB(stateid)
{
    return Core_IsValidState(stateid) ? gGangWarScoreB[stateid] : 0;
}

stock GangWarSystem_Toggle()
{
    gGangWarEnabled = 1 - gGangWarEnabled;
}

stock GangWarSystem_StartWar(stateid, factionA, factionB)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (gGangWarEnabled != 1)
    {
        return false;
    }
    if (factionA < 0 || factionB < 0)
    {
        return false;
    }
    if (factionA == factionB)
    {
        return false;
    }

    gGangWarActive[stateid] = 1;
    gGangWarFactionA[stateid] = factionA;
    gGangWarFactionB[stateid] = factionB;
    gGangWarScoreA[stateid] = 0;
    gGangWarScoreB[stateid] = 0;
    return true;
}

stock GangWarSystem_EndWar(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return;
    }

    gGangWarActive[stateid] = 0;
    gGangWarFactionA[stateid] = -1;
    gGangWarFactionB[stateid] = -1;
    gGangWarScoreA[stateid] = 0;
    gGangWarScoreB[stateid] = 0;
}

stock GangWarSystem_ResolveWar(stateid, winnerFactionId)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (gGangWarActive[stateid] != 1)
    {
        return false;
    }
    if (winnerFactionId != gGangWarFactionA[stateid] && winnerFactionId != gGangWarFactionB[stateid])
    {
        return false;
    }

    if (winnerFactionId >= 0 && winnerFactionId < MAX_FACTIONS)
    {
        FactionSystem_TransferToBalance(winnerFactionId, 15000);
        new winState = FactionSystem_GetState(winnerFactionId);
        if (Core_IsValidState(winState))
        {
            StateSystem_ApplyTreasuryDelta(winState, 5000);
        }
    }

    StateSystem_ApplyTreasuryDelta(stateid, 2500);
    GangWarSystem_EndWar(stateid);
    return true;
}

stock GangWarSystem_AddScore(stateid, factionid, points)
{
    if (!Core_IsValidState(stateid))
    {
        return;
    }
    if (gGangWarActive[stateid] != 1)
    {
        return;
    }
    if (factionid == gGangWarFactionA[stateid])
    {
        gGangWarScoreA[stateid] += points;
    }
    else if (factionid == gGangWarFactionB[stateid])
    {
        gGangWarScoreB[stateid] += points;
    }

    if (gGangWarScoreA[stateid] >= 10 || gGangWarScoreB[stateid] >= 10)
    {
        if (gGangWarScoreA[stateid] > gGangWarScoreB[stateid])
        {
            GangWarSystem_ResolveWar(stateid, gGangWarFactionA[stateid]);
        }
        else if (gGangWarScoreB[stateid] > gGangWarScoreA[stateid])
        {
            GangWarSystem_ResolveWar(stateid, gGangWarFactionB[stateid]);
        }
    }
}

stock GangWarSystem_ShowStatus(playerid)
{
    GangWarSystem_HideStatus(playerid);

    new stateid = Core_GetPlayerState(playerid);
    if (!Core_IsValidState(stateid))
    {
        stateid = STATE_LOS_SANTOS;
    }

    gGangWarTD[playerid][0] = TextDrawCreate(320.0, 35.0, "GANGWAR STATUS");
    TextDrawAlignment(gGangWarTD[playerid][0], 2);
    TextDrawLetterSize(gGangWarTD[playerid][0], 0.55, 1.5);
    TextDrawColor(gGangWarTD[playerid][0], 0xFF0000FF);

    new warText[128];
    format(warText, sizeof(warText), "Aktiv: %s | Phase: %d", (gGangWarEnabled == 1) ? ("Ja") : ("Nein"), gGangWarActive[stateid]);
    gGangWarTD[playerid][1] = TextDrawCreate(320.0, 60.0, warText);
    TextDrawAlignment(gGangWarTD[playerid][1], 2);
    TextDrawLetterSize(gGangWarTD[playerid][1], 0.22, 1.0);
    TextDrawColor(gGangWarTD[playerid][1], 0xFFFFFFFF);

    new factionText[128];
    format(factionText, sizeof(factionText), "A: %d | B: %d | Punkte: %d:%d", gGangWarFactionA[stateid], gGangWarFactionB[stateid], gGangWarScoreA[stateid], gGangWarScoreB[stateid]);
    gGangWarTD[playerid][2] = TextDrawCreate(320.0, 84.0, factionText);
    TextDrawAlignment(gGangWarTD[playerid][2], 2);
    TextDrawLetterSize(gGangWarTD[playerid][2], 0.22, 1.0);
    TextDrawColor(gGangWarTD[playerid][2], 0xFFFF00FF);

    for (new i = 0; i < 3; i++)
    {
        TextDrawShowForPlayer(playerid, gGangWarTD[playerid][i]);
    }
}

stock GangWarSystem_HideStatus(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gGangWarTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gGangWarTD[playerid][i]);
            TextDrawDestroy(gGangWarTD[playerid][i]);
            gGangWarTD[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }
}

#endif
