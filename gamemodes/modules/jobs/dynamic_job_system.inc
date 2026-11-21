#if !defined DYNAMIC_JOB_SYSTEM_INC
#define DYNAMIC_JOB_SYSTEM_INC

#include <a_samp>

#define MAX_DYNAMIC_JOBS 48
#define JOB_FLAG_ACTIVE 1
#define JOB_FLAG_DISABLED 0

// Job-Typen (JOB_TYPE_*) stammen ausschließlich aus jobs_core.inc.
new gDynamicJobCount = 0;
new gDynamicJobName[MAX_DYNAMIC_JOBS][32];
new gDynamicJobState[MAX_DYNAMIC_JOBS];
new gDynamicJobType[MAX_DYNAMIC_JOBS];
new gDynamicJobActive[MAX_DYNAMIC_JOBS];
new Text:gDynamicJobTD[MAX_PLAYERS][8];
new gDynamicJobBasePay[MAX_DYNAMIC_JOBS];

stock DynamicJobSystem_Initialize()
{
    gDynamicJobCount = 0;

    DynamicJobSystem_RegisterJob("Zivilist", STATE_LOS_SANTOS, JOB_TYPE_CIVIL);
    DynamicJobSystem_RegisterJob("Truckfahrer", STATE_LOS_SANTOS, JOB_TYPE_TRUCKER);
    DynamicJobSystem_RegisterJob("Polizei", STATE_LOS_SANTOS, JOB_TYPE_POLICE);
    DynamicJobSystem_RegisterJob("Sanitäter", STATE_LOS_SANTOS, JOB_TYPE_MEDIC);
    DynamicJobSystem_RegisterJob("Logistik", STATE_LOS_SANTOS, JOB_TYPE_LOGISTICS);

    DynamicJobSystem_RegisterJob("Staatsdienst", STATE_SAN_FIERRO, JOB_TYPE_STATE);
    DynamicJobSystem_RegisterJob("Mechaniker", STATE_SAN_FIERRO, JOB_TYPE_BUSINESS);
    DynamicJobSystem_RegisterJob("Wirtschaft", STATE_SAN_FIERRO, JOB_TYPE_BUSINESS);
    DynamicJobSystem_RegisterJob("Casino", STATE_SAN_FIERRO, JOB_TYPE_BUSINESS);
    DynamicJobSystem_RegisterJob("Service", STATE_SAN_FIERRO, JOB_TYPE_STATE);

    DynamicJobSystem_RegisterJob("Casino", STATE_LAS_VENTURAS, JOB_TYPE_BUSINESS);
    DynamicJobSystem_RegisterJob("Tourismus", STATE_LAS_VENTURAS, JOB_TYPE_CIVIL);
    DynamicJobSystem_RegisterJob("Service", STATE_LAS_VENTURAS, JOB_TYPE_STATE);
    DynamicJobSystem_RegisterJob("Logistik", STATE_LAS_VENTURAS, JOB_TYPE_LOGISTICS);
    DynamicJobSystem_RegisterJob("Transport", STATE_LAS_VENTURAS, JOB_TYPE_TRUCKER);

    print("[JOB_DYNAMIC] Dynamisches Job-System initialisiert");
    Core_MarkModuleReady("jobs");
}

stock DynamicJobSystem_AssignJob(playerid, jobid, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (jobid < 0 || jobid >= gDynamicJobCount)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        stateid = gDynamicJobState[jobid];
    }

    if (gDynamicJobState[jobid] != stateid)
    {
        return false;
    }
    if (gDynamicJobActive[jobid] != JOB_FLAG_ACTIVE)
    {
        return false;
    }

    JobsCore_SetPlayerJob(playerid, gDynamicJobType[jobid]);
    JobsCore_SetPlayerJobId(playerid, jobid);
    JobsCore_SetPlayerJobState(playerid, stateid);
    return true;
}

stock DynamicJobSystem_GetJobName(jobid, output[], len)
{
    if (jobid < 0 || jobid >= gDynamicJobCount)
    {
        format(output, len, "Kein Job");
        return;
    }
    format(output, len, "%s", gDynamicJobName[jobid]);
}

stock DynamicJobSystem_IsJobActive(jobid)
{
    return jobid >= 0 && jobid < gDynamicJobCount && gDynamicJobActive[jobid] == JOB_FLAG_ACTIVE;
}

// Liefert die n-te (0-basiert) Job-ID eines Staates oder -1.
stock int:DynamicJobSystem_GetStateJobId(stateid, nth)
{
    new count = 0;
    for (new i = 0; i < gDynamicJobCount; i++)
    {
        if (gDynamicJobState[i] == stateid)
        {
            if (count == nth)
            {
                return i;
            }
            count++;
        }
    }
    return -1;
}

stock int:DynamicJobSystem_GetBasePay(jobid)
{
    if (jobid < 0 || jobid >= gDynamicJobCount)
    {
        return 0;
    }
    return gDynamicJobBasePay[jobid];
}

stock int:DynamicJobSystem_GetJobType(jobid)
{
    if (jobid < 0 || jobid >= gDynamicJobCount)
    {
        return JOB_TYPE_CIVIL;
    }
    return gDynamicJobType[jobid];
}

stock int:DynamicJobSystem_GetJobState(jobid)
{
    if (jobid < 0 || jobid >= gDynamicJobCount)
    {
        return STATE_LOS_SANTOS;
    }
    return gDynamicJobState[jobid];
}

stock int:DynamicJobSystem_BasePayForType(type)
{
    switch (type)
    {
        case JOB_TYPE_POLICE, JOB_TYPE_MEDIC: return 900;
        case JOB_TYPE_STATE: return 800;
        case JOB_TYPE_TRUCKER, JOB_TYPE_LOGISTICS: return 700;
        case JOB_TYPE_BUSINESS: return 650;
    }
    return 500;
}

stock DynamicJobSystem_RegisterJob(const name[], stateid, type)
{
    if (gDynamicJobCount >= MAX_DYNAMIC_JOBS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    format(gDynamicJobName[gDynamicJobCount], 32, "%s", name);
    gDynamicJobState[gDynamicJobCount] = stateid;
    gDynamicJobType[gDynamicJobCount] = type;
    gDynamicJobActive[gDynamicJobCount] = JOB_FLAG_ACTIVE;
    gDynamicJobBasePay[gDynamicJobCount] = DynamicJobSystem_BasePayForType(type);
    gDynamicJobCount++;
    return true;
}

stock DynamicJobSystem_SetJobActive(jobid, active)
{
    if (jobid < 0 || jobid >= gDynamicJobCount)
    {
        return false;
    }
    gDynamicJobActive[jobid] = active;
    return true;
}

stock DynamicJobSystem_ShowStateJobs(playerid, stateid)
{
    DynamicJobSystem_HideStateJobs(playerid);

    gDynamicJobTD[playerid][0] = TextDrawCreate(320.0, 30.0, "JOB LIST");
    TextDrawAlignment(gDynamicJobTD[playerid][0], 2);
    TextDrawLetterSize(gDynamicJobTD[playerid][0], 0.55, 1.5);
    TextDrawColor(gDynamicJobTD[playerid][0], 0x33CCFFAA);

    new title[64];
    format(title, sizeof(title), "Staat: %d", stateid);
    gDynamicJobTD[playerid][1] = TextDrawCreate(320.0, 55.0, title);
    TextDrawAlignment(gDynamicJobTD[playerid][1], 2);
    TextDrawLetterSize(gDynamicJobTD[playerid][1], 0.22, 1.0);
    TextDrawColor(gDynamicJobTD[playerid][1], 0xFFFFFFFF);

    new count = 0;
    new lineY = 82;
    for (new i = 0; i < gDynamicJobCount && count < 6; i++)
    {
        if (gDynamicJobState[i] == stateid)
        {
            new jobLine[64];
            format(jobLine, sizeof(jobLine), "%s [%s]", gDynamicJobName[i], (gDynamicJobActive[i] == JOB_FLAG_ACTIVE) ? ("aktiv") : ("inaktiv"));
            gDynamicJobTD[playerid][2 + count] = TextDrawCreate(320.0, float(lineY), jobLine);
            TextDrawAlignment(gDynamicJobTD[playerid][2 + count], 2);
            TextDrawLetterSize(gDynamicJobTD[playerid][2 + count], 0.20, 0.8);
            TextDrawColor(gDynamicJobTD[playerid][2 + count], 0x99FF99AA);
            lineY += 18;
            count++;
        }
    }

    for (new i = 0; i < 8; i++)
    {
        if (gDynamicJobTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawShowForPlayer(playerid, gDynamicJobTD[playerid][i]);
        }
    }
}

stock DynamicJobSystem_HideStateJobs(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gDynamicJobTD[playerid][i] != Text:INVALID_TEXT_DRAW)
        {
            TextDrawHideForPlayer(playerid, gDynamicJobTD[playerid][i]);
            TextDrawDestroy(gDynamicJobTD[playerid][i]);
            gDynamicJobTD[playerid][i] = Text:INVALID_TEXT_DRAW;
        }
    }
}

stock int:DynamicJobSystem_GetJobCount()
{
    return gDynamicJobCount;
}

#endif
