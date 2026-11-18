#if !defined TRANSPORT_WORKERS_INC
#define TRANSPORT_WORKERS_INC

#include <a_samp>

#define MAX_TRANSPORT_WORKERS 24
#define TRANSPORT_ROLE_DRIVER 1
#define TRANSPORT_ROLE_COURIER 2
#define TRANSPORT_ROLE_LOGISTICS 3

new gTransportWorkerState[MAX_TRANSPORT_WORKERS];
new gTransportWorkerPlayer[MAX_TRANSPORT_WORKERS];
new gTransportWorkerRole[MAX_TRANSPORT_WORKERS];
new gTransportWorkerActive[MAX_TRANSPORT_WORKERS];
new gTransportWorkerCount = 0;

stock TransportWorkers_Initialize()
{
    gTransportWorkerCount = 0;
    for (new i = 0; i < MAX_TRANSPORT_WORKERS; i++)
    {
        gTransportWorkerState[i] = -1;
        gTransportWorkerPlayer[i] = -1;
        gTransportWorkerRole[i] = 0;
        gTransportWorkerActive[i] = 0;
    }

    print("[TRANSPORT_WORKERS] Transport-Arbeiter und Logistiknetzwerk initialisiert");
}

stock TransportWorkers_RegisterWorker(playerid, stateid, role)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidState(stateid) || role < TRANSPORT_ROLE_DRIVER || role > TRANSPORT_ROLE_LOGISTICS)
    {
        return false;
    }

    // Bestehenden Eintrag aktualisieren oder freien Slot wiederverwenden, damit Slots nicht auslaufen.
    new idx = -1;
    for (new i = 0; i < gTransportWorkerCount; i++)
    {
        if (gTransportWorkerPlayer[i] == playerid)
        {
            idx = i;
            break;
        }
        if (idx == -1 && gTransportWorkerActive[i] == 0)
        {
            idx = i;
        }
    }

    if (idx == -1)
    {
        if (gTransportWorkerCount >= MAX_TRANSPORT_WORKERS)
        {
            return false;
        }
        idx = gTransportWorkerCount++;
    }

    gTransportWorkerPlayer[idx] = playerid;
    gTransportWorkerState[idx] = stateid;
    gTransportWorkerRole[idx] = role;
    gTransportWorkerActive[idx] = 1;
    return true;
}

stock TransportWorkers_UnregWorker(playerid)
{
    for (new i = 0; i < gTransportWorkerCount; i++)
    {
        if (gTransportWorkerPlayer[i] == playerid)
        {
            gTransportWorkerActive[i] = 0;
            gTransportWorkerPlayer[i] = -1;
            return true;
        }
    }
    return false;
}

stock int:TransportWorkers_GetWorkerCount(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }

    new count = 0;
    for (new i = 0; i < gTransportWorkerCount; i++)
    {
        if (gTransportWorkerState[i] == stateid && gTransportWorkerActive[i] == 1)
        {
            count++;
        }
    }
    return count;
}

stock TransportWorkers_ShowWorkers(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[TRANSPORT_WORKERS] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new message[256];
    format(message, sizeof(message), "[TRANSPORT_WORKERS] %s | Fahrer: %d", stateName, TransportWorkers_GetWorkerCount(stateid));
    SendClientMessage(playerid, 0x00D9FFFF, message);

    for (new i = 0; i < gTransportWorkerCount; i++)
    {
        if (gTransportWorkerState[i] == stateid && gTransportWorkerActive[i] == 1)
        {
            new roleName[24];
            if (gTransportWorkerRole[i] == TRANSPORT_ROLE_DRIVER)
                format(roleName, sizeof(roleName), "Driver");
            else if (gTransportWorkerRole[i] == TRANSPORT_ROLE_COURIER)
                format(roleName, sizeof(roleName), "Courier");
            else
                format(roleName, sizeof(roleName), "Logistics");

            new line[128];
            format(line, sizeof(line), "- Player: %d | Rolle: %s", gTransportWorkerPlayer[i], roleName);
            SendClientMessage(playerid, 0xFFFFFFFF, line);
        }
    }
}

#endif
