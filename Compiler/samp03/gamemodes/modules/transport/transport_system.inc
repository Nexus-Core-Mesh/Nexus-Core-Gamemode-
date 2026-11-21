#if !defined TRANSPORT_SYSTEM_INC
#define TRANSPORT_SYSTEM_INC

#include <a_samp>

#define TRANSPORT_TYPE_NONE 0
#define TRANSPORT_TYPE_TAXI 1
#define TRANSPORT_TYPE_BUS 2
#define TRANSPORT_TYPE_TRUCK 3
#define MAX_TRANSPORT_ROUTES 12

new gTransportRouteName[3][MAX_TRANSPORT_ROUTES][32];
new gTransportRouteCount[3];
new gPlayerTransportType[MAX_PLAYERS];
new gPlayerTransportRoute[MAX_PLAYERS];
new Text:gTransportTD[MAX_PLAYERS][8];

stock TransportSystem_Initialize()
{
    for (new stateIndex = 0; stateIndex < 3; stateIndex++)
    {
        gTransportRouteCount[stateIndex] = 0;
        for (new i = 0; i < MAX_TRANSPORT_ROUTES; i++)
        {
            format(gTransportRouteName[stateIndex][i], 32, "");
        }
    }

    TransportSystem_RegisterRoute(STATE_LOS_SANTOS, "Downtown Loop");
    TransportSystem_RegisterRoute(STATE_LOS_SANTOS, "Beach Run");
    TransportSystem_RegisterRoute(STATE_SAN_FIERRO, "City Center");
    TransportSystem_RegisterRoute(STATE_SAN_FIERRO, "Harbor Route");
    TransportSystem_RegisterRoute(STATE_LAS_VENTURAS, "Strip Tour");
    TransportSystem_RegisterRoute(STATE_LAS_VENTURAS, "Airport Link");

    print("[TRANSPORT] Taxi-, Bus- und Truck-System initialisiert");
}

stock TransportSystem_OnPlayerConnect(playerid)
{
    gPlayerTransportType[playerid] = TRANSPORT_TYPE_NONE;
    gPlayerTransportRoute[playerid] = -1;
}

stock TransportSystem_RegisterRoute(stateid, const route[])
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (gTransportRouteCount[stateid] >= MAX_TRANSPORT_ROUTES)
    {
        return false;
    }

    format(gTransportRouteName[stateid][gTransportRouteCount[stateid]], 32, "%s", route);
    gTransportRouteCount[stateid]++;
    return true;
}

stock TransportSystem_StartRoute(playerid, type, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (type < TRANSPORT_TYPE_TAXI || type > TRANSPORT_TYPE_TRUCK)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    gPlayerTransportType[playerid] = type;
    gPlayerTransportRoute[playerid] = stateid;
    SendClientMessage(playerid, 0x66CCFFAA, "[TRANSPORT] Route gestartet.");
    return true;
}

stock TransportSystem_ShowRoutes(playerid)
{
    TransportSystem_HideRoutes(playerid);

    gTransportTD[playerid][0] = TextDrawCreate(320.0, 30.0, "TRANSPORT ROUTES");
    TextDrawAlignment(gTransportTD[playerid][0], 2);
    TextDrawLetterSize(gTransportTD[playerid][0], 0.52, 1.5);
    TextDrawColor(gTransportTD[playerid][0], 0x00CCFFFF);

    new stateText[64];
    format(stateText, sizeof(stateText), "Staat: %d | Typ: %d", StateSystem_GetPlayerState(playerid), gPlayerTransportType[playerid]);
    gTransportTD[playerid][1] = TextDrawCreate(320.0, 60.0, stateText);
    TextDrawAlignment(gTransportTD[playerid][1], 2);
    TextDrawLetterSize(gTransportTD[playerid][1], 0.22, 1.0);
    TextDrawColor(gTransportTD[playerid][1], 0xFFFFFFFF);

    new routeText[128];
    new stateid = StateSystem_GetPlayerState(playerid);
    if (stateid >= 0 && stateid < 3)
    {
        new list[128];
        for (new i = 0; i < gTransportRouteCount[stateid]; i++)
        {
            if (strlen(list) > 0)
            {
                strcat(list, " | ");
            }
            strcat(list, gTransportRouteName[stateid][i]);
        }
        format(routeText, sizeof(routeText), "Routen: %s", list);
    }
    else
    {
        format(routeText, sizeof(routeText), "Routen: Keine");
    }

    gTransportTD[playerid][2] = TextDrawCreate(320.0, 84.0, routeText);
    TextDrawAlignment(gTransportTD[playerid][2], 2);
    TextDrawLetterSize(gTransportTD[playerid][2], 0.20, 0.8);
    TextDrawColor(gTransportTD[playerid][2], 0xCCFFCCFF);

    for (new i = 0; i < 3; i++)
    {
        if (gTransportTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawShowForPlayer(playerid, gTransportTD[playerid][i]);
        }
    }
}

stock TransportSystem_HideRoutes(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gTransportTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gTransportTD[playerid][i]);
            TextDrawDestroy(gTransportTD[playerid][i]);
            gTransportTD[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }
}

#endif
