#if !defined STATE_BUDGET_SYSTEM_INC
#define STATE_BUDGET_SYSTEM_INC

#include <a_samp>

new gStateBudgetAlert[STATE_COUNT];
new gStateBudgetLastTransfer[STATE_COUNT];

stock StateBudgetSystem_Initialize()
{
    for (new i = 0; i < STATE_COUNT; i++)
    {
        gStateBudgetAlert[i] = 0;
        gStateBudgetLastTransfer[i] = 0;
    }
    print("[STATE_BUDGET] Staatliche Treasury- und Budget-Übersicht initialisiert");
}

stock StateBudgetSystem_Transfer(stateid, amount, const reason[], const actor[])
{
    if (!Core_IsValidState(stateid) || amount == 0)
    {
        return false;
    }

    if (!StateSystem_ApplyTreasuryDelta(stateid, amount))
    {
        return false;
    }

    gStateBudgetLastTransfer[stateid] = amount;
    if (StateSystem_GetBankCash(stateid) < 50000)
    {
        gStateBudgetAlert[stateid] = 1;
    }
    else
    {
        gStateBudgetAlert[stateid] = 0;
    }

    return StateLedgerSystem_LogTransaction(stateid, (amount > 0) ? 1 : 4, amount, actor, reason);
}

stock int:StateBudgetSystem_GetAlertState(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }
    return gStateBudgetAlert[stateid];
}

stock int:StateBudgetSystem_GetLastTransfer(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }
    return gStateBudgetLastTransfer[stateid];
}

stock StateBudgetSystem_ShowOverview(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid) || !Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[STATE_BUDGET] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    new policy[32];
    new leader[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    StateSystem_GetPolicy(stateid, policy, sizeof(policy));
    StateSystem_GetLeaderName(stateid, leader, sizeof(leader));

    new msg[256];
    format(msg, sizeof(msg), "[STATE_BUDGET] %s | Budget: $%d | Steuer: %d%% | Leader: %s | Policy: %s | Alert: %d", stateName, StateSystem_GetBankCash(stateid), StateSystem_GetTaxRate(stateid), leader, policy, gStateBudgetAlert[stateid]);
    SendClientMessage(playerid, 0x00D9FFFF, msg);
}

#endif
