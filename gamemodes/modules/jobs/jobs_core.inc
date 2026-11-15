#if !defined JOBS_CORE_INC
#define JOBS_CORE_INC

#include <a_samp>

#if !defined JOB_TYPE_NONE
#define JOB_TYPE_NONE 0
#endif
#if !defined JOB_TYPE_CIVIL
#define JOB_TYPE_CIVIL 1
#endif
#if !defined JOB_TYPE_POLICE
#define JOB_TYPE_POLICE 2
#endif
#if !defined JOB_TYPE_MEDIC
#define JOB_TYPE_MEDIC 3
#endif
#if !defined JOB_TYPE_TRUCKER
#define JOB_TYPE_TRUCKER 4
#endif
#if !defined JOB_TYPE_BUSINESS
#define JOB_TYPE_BUSINESS 5
#endif
#if !defined JOB_TYPE_STATE
#define JOB_TYPE_STATE 6
#endif
#if !defined JOB_TYPE_GANG
#define JOB_TYPE_GANG 7
#endif
#if !defined JOB_TYPE_LOGISTICS
#define JOB_TYPE_LOGISTICS 8
#endif

#define JOB_ID_NONE -1
#define JOB_REWARD_XP_MIN 1

forward Inventory_Add(playerid, itemid, quantity, metadata[] = "NONE");

new gPlayerJob[MAX_PLAYERS];
new gPlayerJobId[MAX_PLAYERS];
new gPlayerJobState[MAX_PLAYERS];
new gPlayerJobXP[MAX_PLAYERS];
new gPlayerJobPay[MAX_PLAYERS];
new gPlayerJobTasksDone[MAX_PLAYERS];

stock JobsCore_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        JobsCore_OnPlayerConnect(i);
    }
    print("[JOBS] Job-System initialisiert");
    print("[JOBS] Staatliche, zivile, logistische und Fraktionsjobs aktiv");
    Core_MarkModuleReady("jobs");
}

stock JobsCore_Shutdown()
{
    print("[JOBS] Job-System beendet");
}

stock JobsCore_OnPlayerConnect(playerid)
{
    gPlayerJob[playerid] = JOB_TYPE_NONE;
    gPlayerJobId[playerid] = JOB_ID_NONE;
    gPlayerJobState[playerid] = -1;
    gPlayerJobXP[playerid] = 0;
    gPlayerJobPay[playerid] = 0;
    gPlayerJobTasksDone[playerid] = 0;
}

stock JobsCore_OnPlayerDisconnect(playerid)
{
    JobsCore_OnPlayerConnect(playerid);
}

stock JobsCore_GetPlayerJobId(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return JOB_ID_NONE;
    }
    return gPlayerJobId[playerid];
}

stock JobsCore_SetPlayerJobId(playerid, jobid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    gPlayerJobId[playerid] = jobid;
    return true;
}

stock JobsCore_QuitJob(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS || gPlayerJob[playerid] == JOB_TYPE_NONE)
    {
        return false;
    }
    gPlayerJob[playerid] = JOB_TYPE_NONE;
    gPlayerJobId[playerid] = JOB_ID_NONE;
    gPlayerJobState[playerid] = -1;
    return true;
}

// Staatliche Berufe werden aus der Staatskasse bezahlt, alle anderen zahlen Lohnsteuer in die Staatskasse.
stock JobsCore_IsPublicJob(jobType)
{
    return jobType == JOB_TYPE_POLICE || jobType == JOB_TYPE_MEDIC || jobType == JOB_TYPE_STATE;
}

stock JobsCore_GetJobTypeName(jobType, output[], len)
{
    switch (jobType)
    {
        case JOB_TYPE_NONE: format(output, len, "Kein Job");
        case JOB_TYPE_CIVIL: format(output, len, "Zivilist");
        case JOB_TYPE_POLICE: format(output, len, "Polizei");
        case JOB_TYPE_MEDIC: format(output, len, "Sanitäter");
        case JOB_TYPE_TRUCKER: format(output, len, "Truckfahrer");
        case JOB_TYPE_BUSINESS: format(output, len, "Business");
        case JOB_TYPE_STATE: format(output, len, "Staatsjob");
        case JOB_TYPE_GANG: format(output, len, "Gang/Fraktion");
        case JOB_TYPE_LOGISTICS: format(output, len, "Logistik");
        default: format(output, len, "Unbekannt");
    }
}

// Zentrale Belohnungspipeline: Lohn (Staatskasse/Steuer) -> Bank, XP, Reputation, Güter -> Inventar/Haussafe.
// Gibt den ausgezahlten Nettolohn zurück oder -1, wenn der Abschluss abgelehnt wurde.
stock JobsCore_CompleteTask(playerid, basePay, xp, rep, itemid = 0, itemQty = 0)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS || gPlayerJob[playerid] == JOB_TYPE_NONE)
    {
        return -1;
    }

    new stateid = gPlayerJobState[playerid];
    if (!Core_IsValidState(stateid))
    {
        return -1;
    }

    new gross = basePay * StateJobs_GetPayMultiplier(stateid) / 100;
    new tax = gross * StateSystem_GetTaxRate(stateid) / 100;
    new net = gross - tax;

    if (JobsCore_IsPublicJob(gPlayerJob[playerid]))
    {
        if (!StateSystem_ApplyTreasuryDelta(stateid, -net))
        {
            SendClientMessage(playerid, 0xFF6666AA, "[JOBS] Die Staatskasse kann den Lohn aktuell nicht zahlen.");
            return -1;
        }
    }
    else if (tax > 0)
    {
        StateSystem_ApplyTreasuryDelta(stateid, tax);
    }

    if (net > 0 && !JobsCore_PayPlayerJob(playerid, net))
    {
        if (JobsCore_IsPublicJob(gPlayerJob[playerid]))
        {
            StateSystem_ApplyTreasuryDelta(stateid, net);
        }
        return -1;
    }

    JobsCore_AddPlayerJobXP(playerid, xp);
    ProgressionSystem_AddXP(playerid, max(xp, JOB_REWARD_XP_MIN));
    if (rep > 0)
    {
        ProgressionSystem_AddRep(playerid, rep);
    }
    gPlayerJobTasksDone[playerid]++;

    if (itemid > 0 && itemQty > 0)
    {
        JobsCore_GrantItem(playerid, itemid, itemQty);
    }

    new msg[128];
    format(msg, sizeof(msg), "[JOBS] Auftrag erledigt: +$%d (Steuer $%d) | +%d XP | +%d Rep", net, tax, xp, rep);
    SendClientMessage(playerid, 0x00FF99AA, msg);
    return net;
}

// Güter landen zuerst im Spielerinventar, bei vollem Inventar im Haussafe des Spielers.
stock JobsCore_GrantItem(playerid, itemid, quantity)
{
    if (Inventory_Add(playerid, itemid, quantity, "NONE"))
    {
        SendClientMessage(playerid, 0x66CCFFAA, "[JOBS] Gut wurde deinem Inventar hinzugefügt.");
        return true;
    }

    for (new h = 0; h < MAX_HOUSES; h++)
    {
        if (gHouseStorageOwner[h] == playerid && HouseStorageSystem_StoreItem(playerid, h, itemid, quantity))
        {
            SendClientMessage(playerid, 0x66CCFFAA, "[JOBS] Inventar voll - Gut wurde in deinem Haussafe eingelagert.");
            return true;
        }
    }

    SendClientMessage(playerid, 0xFF6666AA, "[JOBS] Kein Platz für das Gut in Inventar oder Haussafe.");
    return false;
}

stock JobsCore_SetPlayerJob(playerid, jobid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    gPlayerJob[playerid] = jobid;
    return true;
}

stock JobsCore_SetPlayerJobState(playerid, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    gPlayerJobState[playerid] = stateid;
    return true;
}

stock JobsCore_IsPlayerInStateJob(playerid, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    return Core_IsValidState(stateid) && gPlayerJobState[playerid] == stateid;
}

stock JobsCore_CanPlayerTakeJob(playerid, stateid, requiredJobType)
{
    if (!JobsCore_IsPlayerInStateJob(playerid, stateid))
    {
        return false;
    }
    return gPlayerJob[playerid] == requiredJobType || requiredJobType == JOB_TYPE_NONE;
}

stock JobsCore_AddPlayerJobXP(playerid, xp)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    gPlayerJobXP[playerid] += xp;
    return true;
}

stock JobsCore_GetPlayerJobPay(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gPlayerJobPay[playerid];
}

stock JobsCore_PayPlayerJob(playerid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (amount <= 0)
    {
        return false;
    }
    if (Core_ChangePlayerBank(playerid, amount))
    {
        gPlayerJobPay[playerid] += amount;
        return true;
    }
    return false;
}

stock JobsCore_GetPlayerJob(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return JOB_TYPE_NONE;
    }
    return gPlayerJob[playerid];
}

stock JobsCore_GetPlayerJobState(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return -1;
    }
    return gPlayerJobState[playerid];
}

stock JobsCore_GetPlayerJobXP(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gPlayerJobXP[playerid];
}

stock JobsCore_ShowPlayerJobInfo(playerid)
{
    new jobName[32];
    JobsCore_GetJobTypeName(gPlayerJob[playerid], jobName, sizeof(jobName));

    new msg[128];
    format(msg, sizeof(msg), "[JOBS] Job: %s | Staat: %d | XP: %d | Aufträge: %d | Bank: %d", jobName, gPlayerJobState[playerid], gPlayerJobXP[playerid], gPlayerJobTasksDone[playerid], Core_GetPlayerBank(playerid));
    SendClientMessage(playerid, 0xFFCC66AA, msg);
}

#endif
