#if !defined DISTRICT_ECONOMY_INC
#define DISTRICT_ECONOMY_INC

#include <a_samp>

#define MAX_DISTRICT_ECONOMY 16

new gDistrictEconomyState[MAX_DISTRICT_ECONOMY];
new gDistrictEconomyName[MAX_DISTRICT_ECONOMY][32];
new gDistrictEconomyValue[MAX_DISTRICT_ECONOMY];
new gDistrictEconomyCount = 0;

stock DistrictEconomy_Initialize()
{
    gDistrictEconomyCount = 0;
    for (new i = 0; i < MAX_DISTRICT_ECONOMY; i++)
    {
        gDistrictEconomyState[i] = -1;
        gDistrictEconomyValue[i] = 0;
        format(gDistrictEconomyName[i], 32, "");
    }

    DistrictEconomy_RegDistrict(STATE_LOS_SANTOS, "Downtown", 18000);
    DistrictEconomy_RegDistrict(STATE_SAN_FIERRO, "Harbor", 15000);
    DistrictEconomy_RegDistrict(STATE_LAS_VENTURAS, "The Strip", 22000);

    print("[DISTRICT_ECONOMY] District economy initialized");
}

stock DistrictEconomy_RegDistrict(stateid, const name[], value)
{
    if (!Core_IsValidState(stateid) || gDistrictEconomyCount >= MAX_DISTRICT_ECONOMY)
    {
        return false;
    }

    gDistrictEconomyState[gDistrictEconomyCount] = stateid;
    format(gDistrictEconomyName[gDistrictEconomyCount], 32, "%s", name);
    gDistrictEconomyValue[gDistrictEconomyCount] = value;
    gDistrictEconomyCount++;
    return true;
}

stock DistrictEconomy_CollectIncome(stateid, value)
{
    if (!Core_IsValidState(stateid) || value <= 0)
    {
        return false;
    }

    return StateSystem_ApplyTreasuryDelta(stateid, value);
}

stock DistrictEconomy_Show(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[DISTRICT_ECONOMY] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new message[256];
    format(message, sizeof(message), "[DISTRICT_ECONOMY] %s | Districts: %d", stateName, gDistrictEconomyCount);
    SendClientMessage(playerid, 0x00D9FFFF, message);

    for (new i = 0; i < gDistrictEconomyCount; i++)
    {
        if (gDistrictEconomyState[i] == stateid)
        {
            new line[160];
            format(line, sizeof(line), "- %s | Value: $%d", gDistrictEconomyName[i], gDistrictEconomyValue[i]);
            SendClientMessage(playerid, 0xFFFFFFFF, line);
        }
    }
}

#endif
