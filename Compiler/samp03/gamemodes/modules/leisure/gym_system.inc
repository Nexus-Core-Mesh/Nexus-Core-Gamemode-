#if !defined GYM_SYSTEM_INC
#define GYM_SYSTEM_INC

#include <a_samp>

#define MAX_GYMS 6
new gGymState[MAX_GYMS];
new gGymStrength[MAX_PLAYERS];
new gGymStamina[MAX_PLAYERS];

stock GymSystem_Initialize()
{
    for (new i = 0; i < MAX_GYMS; i++)
    {
        gGymState[i] = -1;
    }
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gGymStrength[i] = 0;
        gGymStamina[i] = 0;
    }
    print("[GYM] Fitness-System initialisiert");
}

stock GymSystem_Train(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    gGymStrength[playerid] += 1;
    gGymStamina[playerid] += 1;
    SendClientMessage(playerid, 0x00FF00AA, "[GYM] Training erfolgreich. Stärke und Ausdauer gestiegen.");
}

stock int:GymSystem_GetStrength(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gGymStrength[playerid];
}

stock int:GymSystem_GetStamina(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gGymStamina[playerid];
}

#endif
