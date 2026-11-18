#if !defined STATE_JOBS_INC
#define STATE_JOBS_INC

#include <a_samp>

#define JOB_STATE_NONE 0
#define JOB_STATE_CIVIL 1
#define JOB_STATE_POLICE 2
#define JOB_STATE_MEDIC 3
#define JOB_STATE_TRUCKER 4
#define JOB_STATE_BUSINESS 5
#define JOB_STATE_GOV 6
#define JOB_STATE_LOGISTICS 7

new gStateJobPayPercent[3] = {100, 100, 100};
new gStateJobCount[3];
new gStateJobType[3][10][24];

stock StateJobs_Initialize()
{
    gStateJobCount[STATE_LOS_SANTOS] = 5;
    gStateJobCount[STATE_SAN_FIERRO] = 5;
    gStateJobCount[STATE_LAS_VENTURAS] = 5;

    format(gStateJobType[STATE_LOS_SANTOS][0], 24, "Zivilist");
    format(gStateJobType[STATE_LOS_SANTOS][1], 24, "Polizei");
    format(gStateJobType[STATE_LOS_SANTOS][2], 24, "Sanitäter");
    format(gStateJobType[STATE_LOS_SANTOS][3], 24, "Truckfahrer");
    format(gStateJobType[STATE_LOS_SANTOS][4], 24, "Logistik");

    format(gStateJobType[STATE_SAN_FIERRO][0], 24, "Staatsdienst");
    format(gStateJobType[STATE_SAN_FIERRO][1], 24, "Wirtschaft");
    format(gStateJobType[STATE_SAN_FIERRO][2], 24, "Mechaniker");
    format(gStateJobType[STATE_SAN_FIERRO][3], 24, "Casino");
    format(gStateJobType[STATE_SAN_FIERRO][4], 24, "Service");

    format(gStateJobType[STATE_LAS_VENTURAS][0], 24, "Casino");
    format(gStateJobType[STATE_LAS_VENTURAS][1], 24, "Tourismus");
    format(gStateJobType[STATE_LAS_VENTURAS][2], 24, "Service");
    format(gStateJobType[STATE_LAS_VENTURAS][3], 24, "Logistik");
    format(gStateJobType[STATE_LAS_VENTURAS][4], 24, "Transport");

    gStateJobPayPercent[STATE_LOS_SANTOS] = 100;
    gStateJobPayPercent[STATE_SAN_FIERRO] = 110;
    gStateJobPayPercent[STATE_LAS_VENTURAS] = 120;

    print("[STATE_JOBS] Staatsspezifische Jobs initialisiert");
    Core_MarkModuleReady("jobs");
}

// Lohnfaktor in Prozent; wird von JobsCore_CompleteTask auf den Grundlohn angewendet.
stock int:StateJobs_GetPayMultiplier(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 100;
    }
    return int:gStateJobPayPercent[stateid];
}

stock StateJobs_SetPayMultiplier(stateid, percent)
{
    if (!Core_IsValidState(stateid) || percent < 50 || percent > 300)
    {
        return false;
    }
    gStateJobPayPercent[stateid] = percent;
    return true;
}

stock StateJobs_ShowForState(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return;
    }

    new msg[256];
    format(msg, sizeof(msg), "[JOBS] Staat %d: %s | %s | %s | %s | %s", stateid, gStateJobType[stateid][0], gStateJobType[stateid][1], gStateJobType[stateid][2], gStateJobType[stateid][3], gStateJobType[stateid][4]);
    SendClientMessage(playerid, 0x00FF99AA, msg);
    SendClientMessage(playerid, 0xFFCC66AA, "[JOBS] Jobzuordnung erfolgt im Job-Zentrum (/jobs).");
}

#endif
