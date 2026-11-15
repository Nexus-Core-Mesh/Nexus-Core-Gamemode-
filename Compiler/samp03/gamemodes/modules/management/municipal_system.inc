#if !defined MUNICIPAL_SYSTEM_INC
#define MUNICIPAL_SYSTEM_INC

#include <a_samp>

#define MUNICIPAL_TYPE_POWER 0
#define MUNICIPAL_TYPE_WATER 1
#define MUNICIPAL_TYPE_WASTE 2
#define MUNICIPAL_TYPE_TRANSPORT 3
#define MUNICIPAL_TYPE_TOURISM 4
#define MAX_MUNICIPAL_SERVICES 20

new gMunicipalState[MAX_MUNICIPAL_SERVICES];
new gMunicipalType[MAX_MUNICIPAL_SERVICES];
new gMunicipalRevenue[MAX_MUNICIPAL_SERVICES];
new gMunicipalName[MAX_MUNICIPAL_SERVICES][32];
new gMunicipalCount[STATE_COUNT];

stock MunicipalSystem_Initialize()
{
    for (new i = 0; i < STATE_COUNT; i++)
    {
        gMunicipalCount[i] = 0;
    }

    for (new i = 0; i < MAX_MUNICIPAL_SERVICES; i++)
    {
        gMunicipalState[i] = -1;
        gMunicipalType[i] = MUNICIPAL_TYPE_POWER;
        gMunicipalRevenue[i] = 0;
        format(gMunicipalName[i], 32, "");
    }

    MunicipalSystem_RegisterService(STATE_LOS_SANTOS, "Power Grid", MUNICIPAL_TYPE_POWER, 14000);
    MunicipalSystem_RegisterService(STATE_LOS_SANTOS, "Water Supply", MUNICIPAL_TYPE_WATER, 8500);
    MunicipalSystem_RegisterService(STATE_LOS_SANTOS, "Waste & Recycling", MUNICIPAL_TYPE_WASTE, 6200);

    MunicipalSystem_RegisterService(STATE_SAN_FIERRO, "Harbor Utilities", MUNICIPAL_TYPE_TRANSPORT, 18000);
    MunicipalSystem_RegisterService(STATE_SAN_FIERRO, "Industrial Water", MUNICIPAL_TYPE_WATER, 10500);
    MunicipalSystem_RegisterService(STATE_SAN_FIERRO, "Public Logistics", MUNICIPAL_TYPE_TRANSPORT, 9000);

    MunicipalSystem_RegisterService(STATE_LAS_VENTURAS, "Tourism Power", MUNICIPAL_TYPE_TOURISM, 16500);
    MunicipalSystem_RegisterService(STATE_LAS_VENTURAS, "Resort Water", MUNICIPAL_TYPE_WATER, 11500);
    MunicipalSystem_RegisterService(STATE_LAS_VENTURAS, "Strip Services", MUNICIPAL_TYPE_TOURISM, 9900);

    print("[MUNICIPAL] Municipal service network initialized");
}

stock MunicipalSystem_GetTypeName(typeid, output[], len)
{
    if (len <= 0)
    {
        return;
    }
    output[0] = '\0';

    if (typeid == MUNICIPAL_TYPE_POWER)
    {
        format(output, len, "Power");
        return;
    }
    if (typeid == MUNICIPAL_TYPE_WATER)
    {
        format(output, len, "Water");
        return;
    }
    if (typeid == MUNICIPAL_TYPE_WASTE)
    {
        format(output, len, "Waste");
        return;
    }
    if (typeid == MUNICIPAL_TYPE_TRANSPORT)
    {
        format(output, len, "Transport");
        return;
    }
    if (typeid == MUNICIPAL_TYPE_TOURISM)
    {
        format(output, len, "Tourism");
        return;
    }
    format(output, len, "Unknown");
}

stock MunicipalSystem_RegisterService(stateid, const name[], type, revenue)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    for (new i = 0; i < MAX_MUNICIPAL_SERVICES; i++)
    {
        if (gMunicipalState[i] == stateid && gMunicipalType[i] == type)
        {
            gMunicipalRevenue[i] = revenue;
            format(gMunicipalName[i], 32, "%s", name);
            return true;
        }
    }

    if (gMunicipalCount[stateid] >= MAX_MUNICIPAL_SERVICES)
    {
        return false;
    }

    new idx = -1;
    for (new i = 0; i < MAX_MUNICIPAL_SERVICES; i++)
    {
        if (gMunicipalState[i] == -1)
        {
            idx = i;
            break;
        }
    }

    if (idx == -1)
    {
        return false;
    }

    gMunicipalState[idx] = stateid;
    gMunicipalType[idx] = type;
    gMunicipalRevenue[idx] = revenue;
    format(gMunicipalName[idx], 32, "%s", name);
    gMunicipalCount[stateid]++;
    return true;
}

stock int:MunicipalSystem_GetServiceCount(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }

    new total = 0;
    for (new i = 0; i < MAX_MUNICIPAL_SERVICES; i++)
    {
        if (gMunicipalState[i] == stateid)
        {
            total++;
        }
    }
    return total;
}

stock int:MunicipalSystem_GetStateRevenue(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }

    new total = 0;
    for (new i = 0; i < MAX_MUNICIPAL_SERVICES; i++)
    {
        if (gMunicipalState[i] == stateid)
        {
            total += gMunicipalRevenue[i];
        }
    }
    return total;
}

stock MunicipalSystem_ShowOverview(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[MUNICIPAL] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new summary[256];
    format(summary, sizeof(summary), "[MUNICIPAL] %s | Services: %d | Revenue: $%d", stateName, MunicipalSystem_GetServiceCount(stateid), MunicipalSystem_GetStateRevenue(stateid));
    SendClientMessage(playerid, 0x00D9FFFF, summary);

    for (new i = 0; i < MAX_MUNICIPAL_SERVICES; i++)
    {
        if (gMunicipalState[i] == stateid)
        {
            new line[128];
            new typeName[24];
            MunicipalSystem_GetTypeName(gMunicipalType[i], typeName, sizeof(typeName));
            format(line, sizeof(line), "- %s | Type: %s | Revenue: $%d", gMunicipalName[i], typeName, gMunicipalRevenue[i]);
            SendClientMessage(playerid, 0xFFFFFFFF, line);
        }
    }
}

#endif
