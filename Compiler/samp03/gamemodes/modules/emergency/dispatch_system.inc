#if !defined DISPATCH_SYSTEM_INC
#define DISPATCH_SYSTEM_INC

#include <a_samp>

#define MAX_DISPATCH_CALLS 16
#define DISPATCH_PRIORITY_LOW 1
#define DISPATCH_PRIORITY_MEDIUM 2
#define DISPATCH_PRIORITY_HIGH 3

new gDispatchCallState[MAX_DISPATCH_CALLS];
new gDispatchCallActive[MAX_DISPATCH_CALLS];
new gDispatchCallPriority[MAX_DISPATCH_CALLS];
new gDispatchCallType[MAX_DISPATCH_CALLS][32];
new gDispatchCallCount = 0;

stock DispatchSystem_Initialize()
{
    gDispatchCallCount = 0;
    for (new i = 0; i < MAX_DISPATCH_CALLS; i++)
    {
        gDispatchCallState[i] = -1;
        gDispatchCallActive[i] = 0;
        gDispatchCallPriority[i] = DISPATCH_PRIORITY_LOW;
        format(gDispatchCallType[i], 32, "");
    }

    DispatchSystem_CreateCall(STATE_LOS_SANTOS, "Störung in der Innenstadt", DISPATCH_PRIORITY_MEDIUM);
    DispatchSystem_CreateCall(STATE_SAN_FIERRO, "Fahrzeugunfall am Hafen", DISPATCH_PRIORITY_HIGH);
    DispatchSystem_CreateCall(STATE_LAS_VENTURAS, "Casino-Sicherung alarmiert", DISPATCH_PRIORITY_HIGH);

    print("[DISPATCH] Emergency Dispatch System initialisiert");
}

stock DispatchSystem_CreateCall(stateid, const callType[], priority)
{
    if (!Core_IsValidState(stateid) || gDispatchCallCount >= MAX_DISPATCH_CALLS)
    {
        return false;
    }

    new idx = gDispatchCallCount;
    gDispatchCallState[idx] = stateid;
    gDispatchCallActive[idx] = 1;
    gDispatchCallPriority[idx] = priority;
    format(gDispatchCallType[idx], 32, "%s", callType);
    gDispatchCallCount++;
    return true;
}

stock DispatchSystem_ShowQueue(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[DISPATCH] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new msg[256];
    format(msg, sizeof(msg), "[DISPATCH] %s Dispatch Queue | %d offene Anrufe", stateName, gDispatchCallCount);
    SendClientMessage(playerid, 0x00D9FFFF, msg);

    for (new i = 0; i < gDispatchCallCount; i++)
    {
        if (gDispatchCallState[i] != stateid || gDispatchCallActive[i] == 0)
        {
            continue;
        }

        new entry[128];
        format(entry, sizeof(entry), "[%d] %s | Prioritaet: %d", i, gDispatchCallType[i], gDispatchCallPriority[i]);
        SendClientMessage(playerid, 0xFFFFFFFF, entry);
    }
}

stock DispatchSystem_AcceptCall(playerid, callIndex)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS || callIndex < 0 || callIndex >= gDispatchCallCount)
    {
        return false;
    }

    if (gDispatchCallState[callIndex] != StateSystem_GetPlayerState(playerid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[DISPATCH] Dieser Einsatz liegt nicht in deinem Staat.");
        return false;
    }

    if (gDispatchCallActive[callIndex] == 0)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[DISPATCH] Einsatz bereits angenommen.");
        return false;
    }

    gDispatchCallActive[callIndex] = 0;
    SendClientMessage(playerid, 0x00FF00AA, "[DISPATCH] Einsatz erfolgreich angenommen und bearbeitet.");
    return true;
}

stock DispatchSystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
    {
        return false;
    }

    new cmd[32], idx;
    idx = sscanf(cmdtext, "s[32]", cmd);
    if (idx == 0)
    {
        return false;
    }

    if (strcmp(cmd, "/dispatch", true) == 0)
    {
        DispatchSystem_ShowQueue(playerid, StateSystem_GetPlayerState(playerid));
        return true;
    }

    if (strcmp(cmd, "/acceptcall", true) == 0)
    {
        new callid;
        if (sscanf(cmdtext, "s[32]d", cmd, callid) != 0)
        {
            SendClientMessage(playerid, 0xFFCC66AA, "[DISPATCH] Syntax: /acceptcall <ID>");
            return true;
        }

        DispatchSystem_AcceptCall(playerid, callid);
        return true;
    }

    return false;
}

#endif
