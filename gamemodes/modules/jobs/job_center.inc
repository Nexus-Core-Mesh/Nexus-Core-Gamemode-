#if !defined JOB_CENTER_INC
#define JOB_CENTER_INC

#include <a_samp>

#define JOB_TASK_NONE 0
#define JOB_TASK_PICKUP 1
#define JOB_TASK_DELIVER 2
#define JOB_TASK_COOLDOWN_MS 15000
#define JOB_CENTER_MAX_LIST 16

new gJobTaskStage[MAX_PLAYERS];
new gJobTaskStartTick[MAX_PLAYERS];
new gJobTaskLastDone[MAX_PLAYERS];
new gJobCenterPick[MAX_PLAYERS];
new gJobCenterListIds[MAX_PLAYERS][JOB_CENTER_MAX_LIST];
new gJobCenterListCount[MAX_PLAYERS];

// Pickup (0) und Abgabe (1) je Staat.
new Float:gJobTaskPoint[STATE_COUNT][2][3] =
{
    {{2790.0, -2417.0, 13.6}, {1740.0, -1850.0, 13.5}},
    {{-1730.0, 120.0, 3.5}, {-2030.0, 160.0, 28.8}},
    {{1680.0, 1000.0, 10.8}, {2030.0, 1010.0, 10.8}}
};

stock JobCenter_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        JobCenter_Reset(i);
        gJobTaskLastDone[i] = 0;
    }

    print("[JOB_CENTER] Job-Zentrum (Annahme, Aufträge, Fortschritt) initialisiert");
    Core_MarkModuleReady("jobs");
}

stock JobCenter_Reset(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    gJobTaskStage[playerid] = JOB_TASK_NONE;
    gJobTaskStartTick[playerid] = 0;
    gJobCenterPick[playerid] = -1;
    gJobCenterListCount[playerid] = 0;
}

stock JobCenter_OnPlayerConnect(playerid)
{
    JobCenter_Reset(playerid);
    gJobTaskLastDone[playerid] = 0;
}

stock JobCenter_OnPlayerDisconnect(playerid)
{
    TransportWorkers_UnregisterWorker(playerid);
    JobCenter_Reset(playerid);
}

stock JobCenter_CancelTask(playerid)
{
    if (gJobTaskStage[playerid] != JOB_TASK_NONE)
    {
        DisablePlayerCheckpoint(playerid);
    }
    gJobTaskStage[playerid] = JOB_TASK_NONE;
}

stock JobCenter_ShowMain(playerid)
{
    if (!Core_AssertLifecycleStage(CORE_LIFECYCLE_RUNTIME_READY))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[JOBS] Das Job-System startet noch.");
        return;
    }

    new jobName[32];
    DynamicJobSystem_GetJobName(JobsCore_GetPlayerJobId(playerid), jobName, sizeof(jobName));

    new header[64];
    format(header, sizeof(header), "Job-Zentrum | %s", jobName);

    new list[256];
    format(list, sizeof(list), "Jobs im Staat ansehen & annehmen\nAuftrag starten\nAuftrag abbrechen\nFortschritt & Lohn\nJob kündigen");
    ShowPlayerDialog(playerid, DIALOG_JOB_MAIN, DIALOG_STYLE_LIST, header, list, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_CLOSE);
}

stock JobCenter_ShowJobList(playerid)
{
    new stateid = StateSystem_GetPlayerState(playerid);
    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));

    new list[1024];
    format(list, sizeof(list), "Job\tTyp\tGrundlohn\n");
    gJobCenterListCount[playerid] = 0;

    for (new n = 0; n < JOB_CENTER_MAX_LIST; n++)
    {
        new jobid = DynamicJobSystem_GetStateJobId(stateid, n);
        if (jobid == -1)
        {
            break;
        }

        new jobName[32];
        new typeName[32];
        DynamicJobSystem_GetJobName(jobid, jobName, sizeof(jobName));
        JobsCore_GetJobTypeName(DynamicJobSystem_GetJobType(jobid), typeName, sizeof(typeName));

        new line[96];
        format(line, sizeof(line), "%s%s\t%s\t$%d\n", jobName, DynamicJobSystem_IsJobActive(jobid) ? ("") : (" (inaktiv)"), typeName, DynamicJobSystem_GetBasePay(jobid) * StateJobs_GetPayMultiplier(stateid) / 100);
        strcat(list, line);

        gJobCenterListIds[playerid][gJobCenterListCount[playerid]] = jobid;
        gJobCenterListCount[playerid]++;
    }

    new title[64];
    format(title, sizeof(title), "Jobs in %s", stateName);
    ShowPlayerDialog(playerid, DIALOG_JOB_LIST, DIALOG_STYLE_TABLIST_HEADERS, title, list, "Annehmen", UI_DIALOG_BUTTON_BACK);
}

stock JobCenter_ShowStatus(playerid)
{
    new jobName[32];
    new typeName[32];
    DynamicJobSystem_GetJobName(JobsCore_GetPlayerJobId(playerid), jobName, sizeof(jobName));
    JobsCore_GetJobTypeName(JobsCore_GetPlayerJob(playerid), typeName, sizeof(typeName));

    new info[512];
    format(info, sizeof(info), "Job: %s (%s)\nStaat: %d\nJob-XP: %d\nVerdienter Lohn: $%d\nLevel: %d | XP: %d | Rep: %d\nBank: $%d\nAuftrag: %s",
        jobName, typeName, JobsCore_GetPlayerJobState(playerid), JobsCore_GetPlayerJobXP(playerid), JobsCore_GetPlayerJobPay(playerid),
        ProgressionSystem_GetLevel(playerid), ProgressionSystem_GetXP(playerid), ProgressionSystem_GetRep(playerid),
        Core_GetPlayerBank(playerid),
        (gJobTaskStage[playerid] == JOB_TASK_NONE) ? ("keiner") : ((gJobTaskStage[playerid] == JOB_TASK_PICKUP) ? ("Ware abholen") : ("Ware abliefern")));
    ShowPlayerDialog(playerid, DIALOG_JOB_STATUS, DIALOG_STYLE_MSGBOX, "Job-Fortschritt", info, UI_DIALOG_BUTTON_BACK, UI_DIALOG_BUTTON_CLOSE);
}

stock JobCenter_AcceptJob(playerid, jobid)
{
    new stateid = StateSystem_GetPlayerState(playerid);
    if (!DynamicJobSystem_AssignPlayerJob(playerid, jobid, stateid))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[JOBS] Dieser Job ist nicht verfügbar.");
        return false;
    }

    JobCenter_CancelTask(playerid);
    TransportWorkers_UnregisterWorker(playerid);

    new type = DynamicJobSystem_GetJobType(jobid);
    if (type == JOB_TYPE_TRUCKER)
    {
        TransportWorkers_RegisterWorker(playerid, stateid, TRANSPORT_ROLE_DRIVER);
    }
    else if (type == JOB_TYPE_LOGISTICS)
    {
        TransportWorkers_RegisterWorker(playerid, stateid, TRANSPORT_ROLE_LOGISTICS);
    }

    new jobName[32];
    DynamicJobSystem_GetJobName(jobid, jobName, sizeof(jobName));
    new msg[96];
    format(msg, sizeof(msg), "[JOBS] Du arbeitest jetzt als %s.", jobName);
    SendClientMessage(playerid, 0x00FF99AA, msg);
    return true;
}

stock JobCenter_StartTask(playerid)
{
    if (JobsCore_GetPlayerJob(playerid) == JOB_TYPE_NONE)
    {
        SendClientMessage(playerid, 0xFF6666AA, "[JOBS] Du hast keinen Job. Wähle zuerst einen im Job-Zentrum.");
        return false;
    }

    new stateid = JobsCore_GetPlayerJobState(playerid);
    if (!Core_IsValidState(stateid) || stateid != StateSystem_GetPlayerState(playerid))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[JOBS] Dein Job ist an einen anderen Staat gebunden.");
        return false;
    }
    if (gJobTaskStage[playerid] != JOB_TASK_NONE)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[JOBS] Du hast bereits einen laufenden Auftrag.");
        return false;
    }

    gJobTaskStage[playerid] = JOB_TASK_PICKUP;
    gJobTaskStartTick[playerid] = GetTickCount();
    SetPlayerCheckpoint(playerid, gJobTaskPoint[stateid][0][0], gJobTaskPoint[stateid][0][1], gJobTaskPoint[stateid][0][2], 5.0);
    SendClientMessage(playerid, 0x00D9FFFF, "[JOBS] Auftrag gestartet: Hole die Ware am markierten Punkt ab.");
    return true;
}

// Rückgabe true, wenn der Checkpoint zum Job-Auftrag gehörte.
stock JobCenter_OnEnterCheckpoint(playerid)
{
    if (gJobTaskStage[playerid] == JOB_TASK_NONE)
    {
        return false;
    }

    new stateid = JobsCore_GetPlayerJobState(playerid);
    if (!Core_IsValidState(stateid))
    {
        JobCenter_CancelTask(playerid);
        return true;
    }

    DisablePlayerCheckpoint(playerid);
    if (gJobTaskStage[playerid] == JOB_TASK_PICKUP)
    {
        gJobTaskStage[playerid] = JOB_TASK_DELIVER;
        SetPlayerCheckpoint(playerid, gJobTaskPoint[stateid][1][0], gJobTaskPoint[stateid][1][1], gJobTaskPoint[stateid][1][2], 5.0);
        SendClientMessage(playerid, 0x00D9FFFF, "[JOBS] Ware aufgenommen. Liefere sie am nächsten Punkt ab.");
        return true;
    }

    gJobTaskStage[playerid] = JOB_TASK_NONE;
    JobCenter_FinishTask(playerid, stateid);
    return true;
}

stock JobCenter_FinishTask(playerid, stateid)
{
    new now = GetTickCount();
    if (gJobTaskLastDone[playerid] != 0 && now - gJobTaskLastDone[playerid] < JOB_TASK_COOLDOWN_MS)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[JOBS] Zu schnell abgeschlossen - keine Belohnung.");
        return;
    }

    new jobType = JobsCore_GetPlayerJob(playerid);
    new basePay = DynamicJobSystem_GetBasePay(JobsCore_GetPlayerJobId(playerid));
    new xp = 10;
    new rep = 1;
    new itemid = 0;
    new qty = 0;

    switch (jobType)
    {
        case JOB_TYPE_TRUCKER, JOB_TYPE_LOGISTICS:
        {
            // Transportnetzwerk: Routengebühr des Staates erhöht den Lohn, Fracht landet im Inventar.
            basePay += TradeLogistics_GetFee(stateid) / 10;
            itemid = INVENTORY_ITEM_MATERIAL;
            qty = 2;
            xp = 15;
        }
        case JOB_TYPE_MEDIC:
        {
            itemid = INVENTORY_ITEM_MEDKIT;
            qty = 1;
            rep = 2;
        }
        case JOB_TYPE_POLICE, JOB_TYPE_STATE:
        {
            rep = 2;
        }
        case JOB_TYPE_CIVIL:
        {
            itemid = INVENTORY_ITEM_FOOD;
            qty = 1;
        }
    }

    if (JobsCore_CompleteTask(playerid, basePay, xp, rep, itemid, qty) >= 0)
    {
        gJobTaskLastDone[playerid] = now;
    }
}

// Rückgabe true, wenn der Dialog vom Job-Zentrum behandelt wurde.
stock JobCenter_OnDialogResponse(playerid, dialogid, response, listitem, const inputtext[])
{
    #pragma unused inputtext
    if (dialogid < DIALOG_UI_RANGE_JOBS_FIRST || dialogid > DIALOG_UI_RANGE_JOBS_LAST)
    {
        return false;
    }

    switch (dialogid)
    {
        case DIALOG_JOB_MAIN:
        {
            if (!response)
            {
                return true;
            }
            switch (listitem)
            {
                case 0: JobCenter_ShowJobList(playerid);
                case 1:
                {
                    JobCenter_StartTask(playerid);
                }
                case 2:
                {
                    JobCenter_CancelTask(playerid);
                    SendClientMessage(playerid, 0xFFCC66AA, "[JOBS] Auftrag abgebrochen.");
                }
                case 3: JobCenter_ShowStatus(playerid);
                case 4:
                {
                    ShowPlayerDialog(playerid, DIALOG_JOB_QUIT, DIALOG_STYLE_MSGBOX, "Job kündigen", "Willst du deinen aktuellen Job wirklich kündigen?", "Kündigen", "Abbrechen");
                }
            }
        }
        case DIALOG_JOB_LIST:
        {
            if (!response)
            {
                JobCenter_ShowMain(playerid);
                return true;
            }
            if (listitem < 0 || listitem >= gJobCenterListCount[playerid])
            {
                return true;
            }
            gJobCenterPick[playerid] = gJobCenterListIds[playerid][listitem];

            new jobName[32];
            DynamicJobSystem_GetJobName(gJobCenterPick[playerid], jobName, sizeof(jobName));
            new text[128];
            format(text, sizeof(text), "Möchtest du den Job \"%s\" annehmen?", jobName);
            ShowPlayerDialog(playerid, DIALOG_JOB_CONFIRM, DIALOG_STYLE_MSGBOX, "Job annehmen", text, "Annehmen", UI_DIALOG_BUTTON_BACK);
        }
        case DIALOG_JOB_CONFIRM:
        {
            if (!response)
            {
                JobCenter_ShowJobList(playerid);
                return true;
            }
            JobCenter_AcceptJob(playerid, gJobCenterPick[playerid]);
        }
        case DIALOG_JOB_STATUS:
        {
            if (response)
            {
                JobCenter_ShowMain(playerid);
            }
        }
        case DIALOG_JOB_QUIT:
        {
            if (response)
            {
                JobCenter_CancelTask(playerid);
                TransportWorkers_UnregisterWorker(playerid);
                JobsCore_QuitJob(playerid);
                SendClientMessage(playerid, 0xFFCC66AA, "[JOBS] Du hast deinen Job gekündigt.");
            }
        }
    }
    return true;
}

#endif
