#if !defined GANG_FIGHT_SYSTEM_INC
#define GANG_FIGHT_SYSTEM_INC

#include <a_samp>

#define MAX_GANG_FIGHTS 6
#define GANG_FIGHT_NONE 0
#define GANG_FIGHT_ACTIVE 1

new gGangFightActive[MAX_GANG_FIGHTS];
new gGangFightState[MAX_GANG_FIGHTS];
new gGangFightA[MAX_GANG_FIGHTS];
new gGangFightB[MAX_GANG_FIGHTS];
new gGangFightTimer[MAX_GANG_FIGHTS];
new gGangFightScoreA[MAX_GANG_FIGHTS];
new gGangFightScoreB[MAX_GANG_FIGHTS];
new Text:gGangFightTD[MAX_PLAYERS][8];

stock GangFightSystem_Initialize()
{
    for (new i = 0; i < MAX_GANG_FIGHTS; i++)
    {
        gGangFightActive[i] = GANG_FIGHT_NONE;
        gGangFightState[i] = -1;
        gGangFightA[i] = -1;
        gGangFightB[i] = -1;
        gGangFightTimer[i] = 0;
        gGangFightScoreA[i] = 0;
        gGangFightScoreB[i] = 0;
    }

    print("[GANG_FIGHT] Gangfight-System initialisiert");
    Core_MarkModuleReady("factions");
}

// Erste aktive Auseinandersetzung eines Staates oder -1.
stock int:GangFightSystem_GetActiveFightForState(stateid)
{
    for (new i = 0; i < MAX_GANG_FIGHTS; i++)
    {
        if (gGangFightActive[i] == GANG_FIGHT_ACTIVE && gGangFightState[i] == stateid)
        {
            return i;
        }
    }
    return -1;
}

stock int:GangFightSystem_GetFactionA(fightid)
{
    return (fightid >= 0 && fightid < MAX_GANG_FIGHTS) ? gGangFightA[fightid] : -1;
}

stock int:GangFightSystem_GetFactionB(fightid)
{
    return (fightid >= 0 && fightid < MAX_GANG_FIGHTS) ? gGangFightB[fightid] : -1;
}

stock int:GangFightSystem_GetScoreA(fightid)
{
    return (fightid >= 0 && fightid < MAX_GANG_FIGHTS) ? gGangFightScoreA[fightid] : 0;
}

stock int:GangFightSystem_GetScoreB(fightid)
{
    return (fightid >= 0 && fightid < MAX_GANG_FIGHTS) ? gGangFightScoreB[fightid] : 0;
}

stock int:GangFightSystem_GetTimeLeft(fightid)
{
    return (fightid >= 0 && fightid < MAX_GANG_FIGHTS) ? gGangFightTimer[fightid] : 0;
}

stock GangFightSystem_StartFight(stateid, factionA, factionB)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    for (new i = 0; i < MAX_GANG_FIGHTS; i++)
    {
        if (gGangFightActive[i] == GANG_FIGHT_NONE)
        {
            gGangFightActive[i] = GANG_FIGHT_ACTIVE;
            gGangFightState[i] = stateid;
            gGangFightA[i] = factionA;
            gGangFightB[i] = factionB;
            gGangFightTimer[i] = 180;
            gGangFightScoreA[i] = 0;
            gGangFightScoreB[i] = 0;
            return true;
        }
    }

    return false;
}

stock GangFightSystem_ResolveFight(fightid, winnerFactionId)
{
    if (fightid < 0 || fightid >= MAX_GANG_FIGHTS)
    {
        return false;
    }
    if (gGangFightActive[fightid] != GANG_FIGHT_ACTIVE)
    {
        return false;
    }
    if (winnerFactionId != gGangFightA[fightid] && winnerFactionId != gGangFightB[fightid])
    {
        return false;
    }

    if (winnerFactionId >= 0 && winnerFactionId < MAX_FACTIONS)
    {
        FactionSystem_TransferToBalance(winnerFactionId, 8000);
        new winState = FactionSystem_GetState(winnerFactionId);
        if (Core_IsValidState(winState))
        {
            StateSystem_ApplyTreasuryDelta(winState, 2000);
        }
    }

    gGangFightActive[fightid] = GANG_FIGHT_NONE;
    gGangFightState[fightid] = -1;
    gGangFightA[fightid] = -1;
    gGangFightB[fightid] = -1;
    gGangFightTimer[fightid] = 0;
    gGangFightScoreA[fightid] = 0;
    gGangFightScoreB[fightid] = 0;
    return true;
}

stock GangFightSystem_AddScore(fightid, factionid, points)
{
    if (fightid < 0 || fightid >= MAX_GANG_FIGHTS)
    {
        return;
    }
    if (gGangFightActive[fightid] != GANG_FIGHT_ACTIVE)
    {
        return;
    }

    if (factionid == gGangFightA[fightid])
    {
        gGangFightScoreA[fightid] += points;
    }
    else if (factionid == gGangFightB[fightid])
    {
        gGangFightScoreB[fightid] += points;
    }

    if (gGangFightScoreA[fightid] >= 6 || gGangFightScoreB[fightid] >= 6)
    {
        if (gGangFightScoreA[fightid] > gGangFightScoreB[fightid])
        {
            GangFightSystem_ResolveFight(fightid, gGangFightA[fightid]);
        }
        else if (gGangFightScoreB[fightid] > gGangFightScoreA[fightid])
        {
            GangFightSystem_ResolveFight(fightid, gGangFightB[fightid]);
        }
    }
}

stock GangFightSystem_Tick()
{
    for (new i = 0; i < MAX_GANG_FIGHTS; i++)
    {
        if (gGangFightActive[i] == GANG_FIGHT_ACTIVE)
        {
            if (gGangFightTimer[i] > 0)
            {
                gGangFightTimer[i]--;
            }
            else
            {
                gGangFightActive[i] = GANG_FIGHT_NONE;
                gGangFightState[i] = -1;
                gGangFightA[i] = -1;
                gGangFightB[i] = -1;
            }
        }
    }
}

stock GangFightSystem_ShowStatus(playerid)
{
    GangFightSystem_HideStatus(playerid);

    new fightid = GangFightSystem_GetActiveFightForState(Core_GetPlayerState(playerid));
    if (fightid == -1)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[GANG_FIGHT] In deinem Staat läuft aktuell kein Gangfight.");
        return;
    }
    gGangFightTD[playerid][0] = TextDrawCreate(320.0, 30.0, "GANGFIGHT HUD");
    TextDrawAlignment(gGangFightTD[playerid][0], 2);
    TextDrawLetterSize(gGangFightTD[playerid][0], 0.50, 1.4);
    TextDrawColor(gGangFightTD[playerid][0], 0xFF3333AA);

    new fightText[128];
    format(fightText, sizeof(fightText), "A:%d vs B:%d | Time:%d", gGangFightA[fightid], gGangFightB[fightid], gGangFightTimer[fightid]);
    gGangFightTD[playerid][1] = TextDrawCreate(320.0, 58.0, fightText);
    TextDrawAlignment(gGangFightTD[playerid][1], 2);
    TextDrawLetterSize(gGangFightTD[playerid][1], 0.22, 1.0);
    TextDrawColor(gGangFightTD[playerid][1], 0xFFFFFFFF);

    new scoreText[128];
    format(scoreText, sizeof(scoreText), "Score: %d:%d", gGangFightScoreA[fightid], gGangFightScoreB[fightid]);
    gGangFightTD[playerid][2] = TextDrawCreate(320.0, 82.0, scoreText);
    TextDrawAlignment(gGangFightTD[playerid][2], 2);
    TextDrawLetterSize(gGangFightTD[playerid][2], 0.22, 1.0);
    TextDrawColor(gGangFightTD[playerid][2], 0xFFFF00FF);

    for (new i = 0; i < 3; i++)
    {
        TextDrawShowForPlayer(playerid, gGangFightTD[playerid][i]);
    }
}

stock GangFightSystem_HideStatus(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gGangFightTD[playerid][i] != Text:0)
        {
            TextDrawHideForPlayer(playerid, gGangFightTD[playerid][i]);
            TextDrawDestroy(gGangFightTD[playerid][i]);
            gGangFightTD[playerid][i] = Text:0;
        }
    }
}

#endif
