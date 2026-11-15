#if !defined AMMU_SYSTEM_INC
#define AMMU_SYSTEM_INC

#include <a_samp>

#define MAX_AMMU_SHOPS 12

new gAmmuState[MAX_AMMU_SHOPS];
new Float:gAmmuPosX[MAX_AMMU_SHOPS];
new Float:gAmmuPosY[MAX_AMMU_SHOPS];
new Float:gAmmuPosZ[MAX_AMMU_SHOPS];
new gAmmuOpen[MAX_AMMU_SHOPS];
new gAmmuName[MAX_AMMU_SHOPS][32];

stock AmmuSystem_Initialize()
{
    for (new i = 0; i < MAX_AMMU_SHOPS; i++)
    {
        gAmmuState[i] = -1;
        gAmmuPosX[i] = 0.0;
        gAmmuPosY[i] = 0.0;
        gAmmuPosZ[i] = 0.0;
        gAmmuOpen[i] = 1;
        format(gAmmuName[i], 32, "Ammu Nation");
    }

    AmmuSystem_RegisterShop(STATE_LOS_SANTOS, "Ammu Nation Los Santos", 1365.0, -1278.0, 13.0);
    AmmuSystem_RegisterShop(STATE_SAN_FIERRO, "Ammu Nation San Fierro", -2078.0, 251.0, 35.0);
    AmmuSystem_RegisterShop(STATE_LAS_VENTURAS, "Ammu Nation Las Venturas", 2117.0, 933.0, 10.0);

    print("[AMMU] Ammu Nations Systeme initialisiert");
}

stock AmmuSystem_RegisterShop(stateid, const name[], Float:x, Float:y, Float:z)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    for (new i = 0; i < MAX_AMMU_SHOPS; i++)
    {
        if (gAmmuState[i] == -1)
        {
            gAmmuState[i] = stateid;
            gAmmuPosX[i] = x;
            gAmmuPosY[i] = y;
            gAmmuPosZ[i] = z;
            gAmmuOpen[i] = 1;
            format(gAmmuName[i], 32, "%s", name);
            return true;
        }
    }

    return false;
}

stock AmmuSystem_IsNearShop(playerid, stateid, Float:radius)
{
    if (!Core_IsValidState(stateid) || playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    for (new i = 0; i < MAX_AMMU_SHOPS; i++)
    {
        if (gAmmuState[i] == stateid)
        {
            new Float:x, Float:y, Float:z;
            GetPlayerPos(playerid, x, y, z);
            if (GetDistanceBetweenPoints3D(x, y, z, gAmmuPosX[i], gAmmuPosY[i], gAmmuPosZ[i]) <= radius)
            {
                return true;
            }
        }
    }
    return false;
}

stock AmmuSystem_ShowNearbyInfo(playerid)
{
    new stateid = StateSystem_GetPlayerState(playerid);
    if (!Core_IsValidState(stateid))
    {
        return;
    }

    if (AmmuSystem_IsNearShop(playerid, stateid, 25.0))
    {
        SendClientMessage(playerid, 0xFFCC00AA, "[AMMU] Ammu Nation: Waffen, Munition und Ausrüstung für deinen Staat.");
    }
}

#endif
