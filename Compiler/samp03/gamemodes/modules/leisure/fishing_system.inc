#if !defined FISHING_SYSTEM_INC
#define FISHING_SYSTEM_INC

#include <a_samp>

#define MAX_FISH_TYPES 8
new gFishCaught[MAX_PLAYERS];
new gFishType[MAX_PLAYERS];

stock FishingSystem_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gFishCaught[i] = 0;
        gFishType[i] = 0;
    }
    print("[FISHING] Fishing-System initialisiert");
}

stock FishingSystem_CatchFish(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    gFishCaught[playerid]++;
    gFishType[playerid] = random(MAX_FISH_TYPES);
    SendClientMessage(playerid, 0x00B7EBFF, "[FISHING] Fisch gefangen und für den Markt vorbereitet.");
}

stock int:FishingSystem_GetCaught(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gFishCaught[playerid];
}

#endif
