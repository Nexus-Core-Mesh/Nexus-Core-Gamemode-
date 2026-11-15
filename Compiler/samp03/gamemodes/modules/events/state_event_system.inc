#if !defined STATE_EVENT_SYSTEM_INC
#define STATE_EVENT_SYSTEM_INC

#include <a_samp>

#define MAX_STATE_EVENTS 10
#define EVENT_TYPE_NONE 0
#define EVENT_TYPE_FESTIVAL 1
#define EVENT_TYPE_POLICE_SWEEP 2
#define EVENT_TYPE_GANG_TENSION 3
#define EVENT_TYPE_ECONOMY 4

new gStateEventCount = 0;
new gStateEventType[MAX_STATE_EVENTS];
new gStateEventState[MAX_STATE_EVENTS];
new gStateEventActive[MAX_STATE_EVENTS];
new gStateEventMinutes[MAX_STATE_EVENTS];
new gStateEventText[MAX_STATE_EVENTS][64];

stock StateEventSystem_Initialize()
{
    gStateEventCount = 0;

    for (new i = 0; i < MAX_STATE_EVENTS; i++)
    {
        gStateEventType[i] = EVENT_TYPE_NONE;
        gStateEventState[i] = STATE_LOS_SANTOS;
        gStateEventActive[i] = 0;
        gStateEventMinutes[i] = 0;
        gStateEventText[i][0] = 0;
    }

    StateEventSystem_RegisterEvent(STATE_LOS_SANTOS, EVENT_TYPE_FESTIVAL, 45, "Beach Festival – Los Santos");
    StateEventSystem_RegisterEvent(STATE_SAN_FIERRO, EVENT_TYPE_ECONOMY, 35, "Staatsbudget-Sitzung – San Fierro");
    StateEventSystem_RegisterEvent(STATE_LAS_VENTURAS, EVENT_TYPE_GANG_TENSION, 60, "Gang-Konflikt – Las Venturas");

    print("[STATE_EVENT] Staatliche Event- und Newslogik initialisiert");
}

stock StateEventSystem_RegisterEvent(stateid, type, minutes, const text[])
{
    if (gStateEventCount >= MAX_STATE_EVENTS)
        return false;
    if (!Core_IsValidState(stateid))
        return false;
    if (type < EVENT_TYPE_NONE || type > EVENT_TYPE_ECONOMY)
        return false;

    gStateEventState[gStateEventCount] = stateid;
    gStateEventType[gStateEventCount] = type;
    gStateEventActive[gStateEventCount] = 1;
    gStateEventMinutes[gStateEventCount] = minutes;
    format(gStateEventText[gStateEventCount], 64, "%s", text);
    gStateEventCount++;
    return true;
}

stock StateEventSystem_ToggleForState(stateid)
{
    if (!Core_IsValidState(stateid))
        return;

    for (new i = 0; i < gStateEventCount; i++)
    {
        if (gStateEventState[i] == stateid)
        {
            gStateEventActive[i] = 1 - gStateEventActive[i];
        }
    }
}

stock StateEventSystem_ShowEvents(playerid)
{
    new msg[256];
    format(msg, sizeof(msg), "[EVENTS] Laufende Staatsevents: %d", gStateEventCount);
    SendClientMessage(playerid, 0xFFD700AA, msg);

    for (new i = 0; i < gStateEventCount; i++)
    {
        if (gStateEventActive[i] == 1)
        {
            new eventMsg[128];
            format(eventMsg, sizeof(eventMsg), "%s | Staat %d | Typ %d | %d min", gStateEventText[i], gStateEventState[i], gStateEventType[i], gStateEventMinutes[i]);
            SendClientMessage(playerid, 0xFFFFFFFF, eventMsg);
        }
    }
}

stock StateEventSystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
        return false;

    new cmd[32], idx;
    idx = sscanf(cmdtext, "s[32]", cmd);
    if (idx == 0)
        return false;

    if (strcmp(cmd, "/events", true) == 0)
    {
        StateEventSystem_ShowEvents(playerid);
        return true;
    }

    if (strcmp(cmd, "/eventtoggle", true) == 0)
    {
        if (!AdminSystem_IsModerator(playerid))
        {
            SendClientMessage(playerid, 0xFF0000AA, "[EVENTS] Kein Adminzugriff.");
            return true;
        }

        new stateid;
        if (sscanf(cmdtext, "s[32]d", cmd, stateid) != 0)
        {
            SendClientMessage(playerid, 0xFFCC66AA, "[EVENTS] Syntax: /eventtoggle <StateID>");
            return true;
        }

        StateEventSystem_ToggleForState(stateid);
        SendClientMessage(playerid, 0x00FF00AA, "[EVENTS] Eventzustand gewechselt.");
        return true;
    }

    return false;
}

#endif
