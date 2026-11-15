#if !defined COURT_SYSTEM_INC
#define COURT_SYSTEM_INC

#include <a_samp>

#define COURT_STATE_NONE 0
#define COURT_STATE_OPEN 1
#define COURT_STATE_HEARING 2

new gCourtState[3];
new gCourtFine[3];
new gCourtBail[3];
new gCourtSessionOpen[MAX_PLAYERS];

stock CourtSystem_Initialize()
{
    for (new i = 0; i < 3; i++)
    {
        gCourtState[i] = COURT_STATE_NONE;
        gCourtFine[i] = 5000;
        gCourtBail[i] = 15000;
    }

    print("[COURT] Gerichtssystem initialisiert");
}

stock CourtSystem_OpenSession(stateid)
{
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }

    gCourtState[stateid] = COURT_STATE_OPEN;
    gCourtFine[stateid] += 1000;
    return true;
}

stock CourtSystem_IssueFine(playerid, stateid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }

    if (amount > 0)
    {
        new cash = GetPlayerMoney(playerid);
        if (cash >= amount)
        {
            GivePlayerMoney(playerid, -amount);
        }
    }

    gCourtFine[stateid] += amount;
    gPrisonBank[stateid] += amount;
    StateSystem_ApplyTreasuryDelta(stateid, amount);
    SendClientMessage(playerid, 0x00FF00AA, "[COURT] Geldstrafe gezahlt. Die Einnahmen fließen in die Staatskasse.");
    return true;
}

stock CourtSystem_SetBail(playerid, stateid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }

    gCourtBail[stateid] = amount;
    gCourtSessionOpen[playerid] = 1;
    return true;
}

stock CourtSystem_ShowInfo(playerid)
{
    new msg[256];
    format(msg, sizeof(msg), "[COURT] Staat: %d | Strafe: %d | Kaution: %d | Status: %d", StateSystem_GetPlayerState(playerid), gCourtFine[StateSystem_GetPlayerState(playerid)], gCourtBail[StateSystem_GetPlayerState(playerid)], gCourtState[StateSystem_GetPlayerState(playerid)]);
    SendClientMessage(playerid, 0x99CCFFAA, msg);
}

#endif
