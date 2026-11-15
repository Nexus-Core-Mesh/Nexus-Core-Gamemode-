#if !defined STATE_SERVICES_INC
#define STATE_SERVICES_INC

#include <a_samp>

#define STATE_SERVICE_TYPE_CITY_HALL 0
#define STATE_SERVICE_TYPE_AMMU 1
#define STATE_SERVICE_TYPE_HOSPITAL 2
#define STATE_SERVICE_TYPE_LANDMARK 3
#define STATE_SERVICE_TYPE_GOVERNMENT 4
#define MAX_STATE_SERVICES 24

new gStateServiceState[MAX_STATE_SERVICES];
new gStateServiceType[MAX_STATE_SERVICES];
new gStateServiceOpen[MAX_STATE_SERVICES];
new gStateServicePriority[MAX_STATE_SERVICES];
new gStateServiceName[MAX_STATE_SERVICES][32];

stock StateServices_Initialize()
{
    for (new i = 0; i < MAX_STATE_SERVICES; i++)
    {
        gStateServiceState[i] = -1;
        gStateServiceType[i] = STATE_SERVICE_TYPE_CITY_HALL;
        gStateServiceOpen[i] = 1;
        gStateServicePriority[i] = 0;
        format(gStateServiceName[i], 32, "Unbekannter Service");
    }

    StateServices_RegisterService(STATE_LOS_SANTOS, "Los Santos Rathaus", STATE_SERVICE_TYPE_CITY_HALL, 1);
    StateServices_RegisterService(STATE_LOS_SANTOS, "Ammu Nations", STATE_SERVICE_TYPE_AMMU, 2);
    StateServices_RegisterService(STATE_LOS_SANTOS, "Hospital Los Santos", STATE_SERVICE_TYPE_HOSPITAL, 3);
    StateServices_RegisterService(STATE_LOS_SANTOS, "Los Santos Government", STATE_SERVICE_TYPE_GOVERNMENT, 4);

    StateServices_RegisterService(STATE_SAN_FIERRO, "San Fierro Rathaus", STATE_SERVICE_TYPE_CITY_HALL, 1);
    StateServices_RegisterService(STATE_SAN_FIERRO, "Ammu Nations SF", STATE_SERVICE_TYPE_AMMU, 2);
    StateServices_RegisterService(STATE_SAN_FIERRO, "St. George Hospital", STATE_SERVICE_TYPE_HOSPITAL, 3);
    StateServices_RegisterService(STATE_SAN_FIERRO, "San Fierro Government", STATE_SERVICE_TYPE_GOVERNMENT, 4);

    StateServices_RegisterService(STATE_LAS_VENTURAS, "Las Venturas Rathaus", STATE_SERVICE_TYPE_CITY_HALL, 1);
    StateServices_RegisterService(STATE_LAS_VENTURAS, "Ammu Nations LV", STATE_SERVICE_TYPE_AMMU, 2);
    StateServices_RegisterService(STATE_LAS_VENTURAS, "Las Venturas Hospital", STATE_SERVICE_TYPE_HOSPITAL, 3);
    StateServices_RegisterService(STATE_LAS_VENTURAS, "Las Venturas Government", STATE_SERVICE_TYPE_GOVERNMENT, 4);

    print("[STATE_SERVICES] Staatliche Service-Infrastruktur initialisiert");
}

stock StateServices_GetTypeName(serviceType, output[], len)
{
    if (len <= 0)
    {
        return;
    }
    output[0] = '\0';

    if (serviceType == STATE_SERVICE_TYPE_CITY_HALL)
    {
        format(output, len, "City Hall");
        return;
    }
    if (serviceType == STATE_SERVICE_TYPE_AMMU)
    {
        format(output, len, "Ammu");
        return;
    }
    if (serviceType == STATE_SERVICE_TYPE_HOSPITAL)
    {
        format(output, len, "Hospital");
        return;
    }
    if (serviceType == STATE_SERVICE_TYPE_LANDMARK)
    {
        format(output, len, "Landmark");
        return;
    }
    if (serviceType == STATE_SERVICE_TYPE_GOVERNMENT)
    {
        format(output, len, "Government");
        return;
    }
    format(output, len, "Unknown");
}

stock StateServices_RegisterService(stateid, const name[], serviceType, priority)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    for (new i = 0; i < MAX_STATE_SERVICES; i++)
    {
        if (gStateServiceState[i] == stateid && gStateServiceType[i] == serviceType)
        {
            gStateServicePriority[i] = priority;
            format(gStateServiceName[i], 32, "%s", name);
            gStateServiceOpen[i] = 1;
            return true;
        }
    }

    for (new i = 0; i < MAX_STATE_SERVICES; i++)
    {
        if (gStateServiceState[i] == -1)
        {
            gStateServiceState[i] = stateid;
            gStateServiceType[i] = serviceType;
            gStateServiceOpen[i] = 1;
            gStateServicePriority[i] = priority;
            format(gStateServiceName[i], 32, "%s", name);
            return true;
        }
    }

    return false;
}

stock int:StateServices_GetStateServiceCount(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }

    new count = 0;
    for (new i = 0; i < MAX_STATE_SERVICES; i++)
    {
        if (gStateServiceState[i] == stateid)
        {
            count++;
        }
    }
    return count;
}

stock StateServices_ShowOverview(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[STATE_SERVICES] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));

    new services[256];
    format(services, sizeof(services), "[STATE_SERVICES] %s: ", stateName);

    new first = 1;
    for (new i = 0; i < MAX_STATE_SERVICES; i++)
    {
        if (gStateServiceState[i] != stateid)
        {
            continue;
        }

        if (!first)
        {
            strcat(services, " | ");
        }
        strcat(services, gStateServiceName[i]);
        first = 0;
    }

    if (first)
    {
        strcat(services, "Keine Services registriert");
    }

    SendClientMessage(playerid, 0x00D9FFFF, services);
}

stock StateServices_ShowPlayerServices(playerid)
{
    new stateid = StateSystem_GetPlayerState(playerid);
    StateServices_ShowOverview(playerid, stateid);
}

stock StateServices_IsServiceOpen(stateid, serviceType)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    for (new i = 0; i < MAX_STATE_SERVICES; i++)
    {
        if (gStateServiceState[i] == stateid && gStateServiceType[i] == serviceType)
        {
            return gStateServiceOpen[i] == 1;
        }
    }
    return false;
}

stock StateServices_SetServiceStatus(stateid, serviceType, bool:open)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    for (new i = 0; i < MAX_STATE_SERVICES; i++)
    {
        if (gStateServiceState[i] == stateid && gStateServiceType[i] == serviceType)
        {
            gStateServiceOpen[i] = open ? 1 : 0;
            return true;
        }
    }

    return false;
}

#endif
