#if !defined AD_SYSTEM_INC
#define AD_SYSTEM_INC

#include <a_samp>

#define MAX_ADS 20
new gAdState[MAX_ADS];
new gAdBudget[MAX_ADS];
new gAdOwner[MAX_ADS];
new gAdText[MAX_ADS][64];
new gAdCount = 0;

stock AdSystem_Initialize()
{
    print("[AD] Werbenetzwerk initialisiert");
}

stock AdSystem_CreateAd(playerid, const text[], stateid, budget)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (gAdCount >= MAX_ADS)
    {
        return false;
    }
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }
    if (GetPlayerMoney(playerid) < budget)
    {
        return false;
    }

    GivePlayerMoney(playerid, -budget);
    gAdState[gAdCount] = stateid;
    gAdBudget[gAdCount] = budget;
    gAdOwner[gAdCount] = playerid;
    format(gAdText[gAdCount], 64, "%s", text);
    StateSystem_ApplyTreasuryDelta(stateid, budget);
    gAdCount++;
    return true;
}

stock AdSystem_ShowAds(playerid)
{
    new msg[256];
    format(msg, sizeof(msg), "[AD] Aktive Anzeigen: %d | Staat: %d", gAdCount, StateSystem_GetPlayerState(playerid));
    SendClientMessage(playerid, 0xFFCC66AA, msg);
}

#endif
