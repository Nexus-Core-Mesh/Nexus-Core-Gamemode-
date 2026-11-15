#if !defined EVENT_SYSTEM_INC
#define EVENT_SYSTEM_INC

#include <a_samp>

#define MAX_EVENTS 8
#define EVENT_TYPE_NONE 0
#define EVENT_TYPE_PAINTBALL 1
#define EVENT_TYPE_DEATHMATCH 2
#define EVENT_TYPE_RACE 3
#define EVENT_TYPE_DEMOLITION 4

new gEventType[MAX_EVENTS];
new gEventActive[MAX_EVENTS];
new gEventWorld[MAX_EVENTS];
new gEventState[MAX_EVENTS];
new gEventPrizePool[MAX_EVENTS];

stock EventSystem_Initialize()
{
    for (new i = 0; i < MAX_EVENTS; i++)
    {
        gEventType[i] = EVENT_TYPE_NONE;
        gEventActive[i] = 0;
        gEventWorld[i] = i + 100;
        gEventState[i] = -1;
        gEventPrizePool[i] = 0;
    }
    print("[EVENT] Event-System initialisiert");
}

stock EventSystem_CreateEvent(eventid, type, stateid, prizepool)
{
    if (eventid < 0 || eventid >= MAX_EVENTS)
    {
        return false;
    }
    if (stateid < 0 || stateid >= 3)
    {
        return false;
    }

    gEventType[eventid] = type;
    gEventActive[eventid] = 1;
    gEventState[eventid] = stateid;
    gEventPrizePool[eventid] = prizepool;
    return true;
}

stock EventSystem_StartEvent(eventid)
{
    if (eventid < 0 || eventid >= MAX_EVENTS)
    {
        return;
    }
    gEventActive[eventid] = 1;
    print("[EVENT] Event gestartet");
}

stock EventSystem_StopEvent(eventid)
{
    if (eventid < 0 || eventid >= MAX_EVENTS)
    {
        return;
    }
    gEventActive[eventid] = 0;
    print("[EVENT] Event beendet");
}

stock int:EventSystem_GetPrizePool(eventid)
{
    if (eventid < 0 || eventid >= MAX_EVENTS)
    {
        return 0;
    }
    return gEventPrizePool[eventid];
}

#endif
