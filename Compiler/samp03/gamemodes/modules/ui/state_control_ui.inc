#if !defined STATE_CONTROL_UI_INC
#define STATE_CONTROL_UI_INC

#include <a_samp>

// Leitstand für das 3-Staaten-System (Los Santos, San Fierro, Las Venturas).
#define STATE_CONTROL_MAX_TAX 30
#define STATE_CONTROL_POLICY_LEN 23

new gStateControlTarget[MAX_PLAYERS];
new gStateControlTransferDir[MAX_PLAYERS];

stock StateControl_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gStateControlTarget[i] = -1;
        gStateControlTransferDir[i] = 0;
    }

    print("[STATE_CONTROL] Klickbarer State-Leitstand initialisiert");
    Core_MarkModuleReady("government");
}

stock StateControl_OnPlayerConnect(playerid)
{
    gStateControlTarget[playerid] = -1;
    gStateControlTransferDir[playerid] = 0;
}

// State-Admins steuern ihren Staat, State-Leader und höher alle Staaten.
stock StateControl_CanManage(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (Core_HasAdminLevel(playerid, ADMIN_RANK_STATE_LEADER))
    {
        return true;
    }
    return Core_HasAdminLevel(playerid, ADMIN_RANK_STATE_ADMIN) && Core_GetPlayerAdminState(playerid) == stateid;
}

stock StateControl_ShowMain(playerid)
{
    if (!Core_AssertLifecycleStage(CORE_LIFECYCLE_RUNTIME_READY))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Der Leitstand startet noch.");
        return;
    }

    new stateid = gStateControlTarget[playerid];
    if (!Core_IsValidState(stateid))
    {
        stateid = StateSystem_GetPlayerState(playerid);
        gStateControlTarget[playerid] = stateid;
    }
    if (!StateControl_CanManage(playerid, stateid))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Keine Berechtigung für diesen Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new title[64];
    format(title, sizeof(title), "Staats-Leitstand | %s", stateName);

    ShowPlayerDialog(playerid, DIALOG_STATE_MAIN, DIALOG_STYLE_LIST, title,
        "Staat wechseln\nÜbersicht & Kasse\nSteuersatz ändern\nTreasury-Transfer\nPolitik setzen\nJob-Lohnfaktor\nBudget genehmigen\nText-Wars öffnen\nGangwar-System an/aus\nRegierungsinfo (Chat)",
        UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_CLOSE);
}

stock StateControl_ShowPick(playerid)
{
    ShowPlayerDialog(playerid, DIALOG_STATE_PICK, DIALOG_STYLE_LIST, "Staat wählen", "Los Santos\nSan Fierro\nLas Venturas", UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
}

stock StateControl_ShowOverview(playerid, stateid)
{
    new stateName[32];
    new leaderName[24];
    new policy[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    StateSystem_GetLeaderName(stateid, leaderName, sizeof(leaderName));
    StateSystem_GetPolicy(stateid, policy, sizeof(policy));

    new info[512];
    format(info, sizeof(info), "Staat: %s\nLeiter: %s\nStaatskasse: $%d\nSteuersatz: %d%%\nSteuereinnahmen: $%d\nInfrastruktur: $%d\nPolitik: %s\nRegierungslevel: %d\nJob-Lohnfaktor: %d%%\nTransportarbeiter: %d",
        stateName, leaderName, StateSystem_GetBankCash(stateid), StateSystem_GetTaxRate(stateid), StateSystem_GetTaxRevenue(stateid), StateSystem_GetInfraRev(stateid),
        policy, StateGovernance_GetLevel(stateid), StateJobs_GetPayMultiplier(stateid), TransportWorkers_GetWorkerCount(stateid));
    ShowPlayerDialog(playerid, DIALOG_STATE_ACTION, DIALOG_STYLE_MSGBOX, "Übersicht & Kasse", info, UI_DIALOG_BUTTON_BACK, UI_DIALOG_BUTTON_CLOSE);
}

stock StateControl_ShowInput(playerid, dialogid, title[], prompt[])
{
    ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE_INPUT, title, prompt, "Anwenden", UI_DIALOG_BUTTON_BACK);
}

stock StateControl_DoTransfer(playerid, stateid, direction, amount)
{
    if (amount <= 0 || amount > 10000000)
    {
        SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Ungültiger Betrag.");
        return false;
    }

    new actor[MAX_PLAYER_NAME];
    GetPlayerName(playerid, actor, sizeof(actor));

    if (direction == 0)
    {
        // Einzahlung aus der eigenen Bank: keine Geldschöpfung.
        if (!Core_ChangePlayerBank(playerid, -amount))
        {
            SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Dein Bankguthaben reicht nicht.");
            return false;
        }
        if (!StateBudgetSystem_Transfer(stateid, amount, "ui_deposit", actor))
        {
            Core_ChangePlayerBank(playerid, amount);
            SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Transfer abgelehnt.");
            return false;
        }
    }
    else
    {
        if (!Core_HasAdminLevel(playerid, ADMIN_RANK_STATE_LEADER))
        {
            SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Auszahlungen sind State-Leadern vorbehalten.");
            return false;
        }
        if (!StateBudgetSystem_Transfer(stateid, -amount, "ui_withdraw", actor))
        {
            SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Die Staatskasse reicht nicht.");
            return false;
        }
        Core_ChangePlayerBank(playerid, amount);
    }

    new msg[96];
    format(msg, sizeof(msg), "[STATE_CONTROL] Transfer ausgeführt. Staatskasse: $%d", StateSystem_GetBankCash(stateid));
    SendClientMessage(playerid, 0x00FF99AA, msg);
    return true;
}

stock StateControl_ApproveBudget(playerid, stateid, amount)
{
    new actor[MAX_PLAYER_NAME];
    GetPlayerName(playerid, actor, sizeof(actor));
    return StateGovernance_ApproveBudget(stateid, amount) && StateLedgerSystem_LogTx(stateid, STATE_LEDGER_TYPE_GOV, amount, actor, "ui_budget_approval");
}

stock StateControl_OnDialogResponse(playerid, dialogid, response, listitem, const inputtext[])
{
    if (dialogid < DIALOG_UI_RANGE_STATE_FIRST || dialogid > DIALOG_UI_RANGE_STATE_LAST)
    {
        return false;
    }

    new stateid = gStateControlTarget[playerid];
    if (!StateControl_CanManage(playerid, stateid))
    {
        return true;
    }

    switch (dialogid)
    {
        case DIALOG_STATE_MAIN:
        {
            if (!response)
            {
                return true;
            }
            switch (listitem)
            {
                case 0: StateControl_ShowPick(playerid);
                case 1: StateControl_ShowOverview(playerid, stateid);
                case 2: StateControl_ShowInput(playerid, DIALOG_STATE_TAX_INPUT, "Steuersatz", "Neuer Steuersatz in Prozent (0-30):");
                case 3:
                {
                    ShowPlayerDialog(playerid, DIALOG_STATE_TRANSFER_DIR, DIALOG_STYLE_LIST, "Treasury-Transfer", "Einzahlen (aus eigener Bank)\nAuszahlen (auf eigene Bank, nur Leader)", UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
                }
                case 4: StateControl_ShowInput(playerid, DIALOG_STATE_POLICY_INPUT, "Staatspolitik", "Neue Politik (max. 23 Zeichen, ohne Sonderzeichen):");
                case 5: StateControl_ShowInput(playerid, DIALOG_STATE_WAGE_INPUT, "Job-Lohnfaktor", "Lohnfaktor in Prozent (50-300):");
                case 6: StateControl_ShowInput(playerid, DIALOG_STATE_BUDGET_INPUT, "Budget genehmigen", "Betrag, der aus dem Landeshaushalt der Staatskasse gutgeschrieben wird:");
                case 7: TextWars_ShowMain(playerid);
                case 8:
                {
                    GangWarSystem_Toggle();
                    SendClientMessage(playerid, 0xFFCC66AA, "[STATE_CONTROL] Gangwar-System umgeschaltet.");
                    StateControl_ShowMain(playerid);
                }
                case 9: StateGovernance_ShowOverview(playerid, stateid);
            }
        }
        case DIALOG_STATE_PICK:
        {
            if (response && listitem >= 0 && listitem < STATE_COUNT)
            {
                if (StateControl_CanManage(playerid, listitem))
                {
                    gStateControlTarget[playerid] = listitem;
                }
                else
                {
                    SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Keine Berechtigung für diesen Staat.");
                }
            }
            StateControl_ShowMain(playerid);
        }
        case DIALOG_STATE_ACTION:
        {
            if (response)
            {
                StateControl_ShowMain(playerid);
            }
        }
        case DIALOG_STATE_TAX_INPUT:
        {
            if (!response)
            {
                StateControl_ShowMain(playerid);
                return true;
            }
            if (!StateSystem_SetTaxRate(stateid, strval(inputtext)))
            {
                SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Steuersatz muss zwischen 0 und 30 liegen.");
            }
            else
            {
                SendClientMessage(playerid, 0x00FF99AA, "[STATE_CONTROL] Steuersatz aktualisiert.");
            }
            StateControl_ShowMain(playerid);
        }
        case DIALOG_STATE_TRANSFER_DIR:
        {
            if (!response)
            {
                StateControl_ShowMain(playerid);
                return true;
            }
            gStateControlTransferDir[playerid] = listitem;
            StateControl_ShowInput(playerid, DIALOG_STATE_TRANSFER_INPUT, "Treasury-Transfer", "Betrag in Dollar:");
        }
        case DIALOG_STATE_TRANSFER_INPUT:
        {
            if (response)
            {
                StateControl_DoTransfer(playerid, stateid, gStateControlTransferDir[playerid], strval(inputtext));
            }
            StateControl_ShowMain(playerid);
        }
        case DIALOG_STATE_POLICY_INPUT:
        {
            if (response)
            {
                if (strlen(inputtext) == 0 || strlen(inputtext) > STATE_CONTROL_POLICY_LEN || !UiDialog_IsSafeText(inputtext))
                {
                    SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Ungültige Politik-Bezeichnung.");
                }
                else
                {
                    StateGovernance_SetPolicy(stateid, inputtext);
                    SendClientMessage(playerid, 0x00FF99AA, "[STATE_CONTROL] Politik gesetzt.");
                }
            }
            StateControl_ShowMain(playerid);
        }
        case DIALOG_STATE_WAGE_INPUT:
        {
            if (response)
            {
                if (!StateJobs_SetPayMultiplier(stateid, strval(inputtext)))
                {
                    SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Lohnfaktor muss zwischen 50 und 300 liegen.");
                }
                else
                {
                    SendClientMessage(playerid, 0x00FF99AA, "[STATE_CONTROL] Job-Lohnfaktor aktualisiert.");
                }
            }
            StateControl_ShowMain(playerid);
        }
        case DIALOG_STATE_BUDGET_INPUT:
        {
            if (response)
            {
                new amount = strval(inputtext);
                if (!Core_HasAdminLevel(playerid, ADMIN_RANK_STATE_LEADER))
                {
                    SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Budgetfreigaben sind State-Leadern vorbehalten.");
                }
                else if (amount <= 0 || amount > 1000000 || !StateControl_ApproveBudget(playerid, stateid, amount))
                {
                    SendClientMessage(playerid, 0xFF6666AA, "[STATE_CONTROL] Betrag ungültig (1 - 1000000).");
                }
                else
                {
                    SendClientMessage(playerid, 0x00FF99AA, "[STATE_CONTROL] Budget genehmigt.");
                }
            }
            StateControl_ShowMain(playerid);
        }
    }
    return true;
}

#endif
