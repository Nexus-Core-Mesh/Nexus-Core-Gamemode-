#if !defined STATE_LEDGER_SYSTEM_INC
#define STATE_LEDGER_SYSTEM_INC

#include <a_samp>

#define MAX_STATE_LEDGER_ENTRIES 32
#define STATE_LEDGER_TYPE_TAX 0
#define STATE_LEDGER_TYPE_BIZ 1
#define STATE_LEDGER_TYPE_GOV 2
#define STATE_LEDGER_TYPE_JOB 3
#define STATE_LEDGER_TYPE_ADMIN 4

new gStateLedgerEntryState[MAX_STATE_LEDGER_ENTRIES];
new gStateLedgerEntryType[MAX_STATE_LEDGER_ENTRIES];
new gStateLedgerEntryAmount[MAX_STATE_LEDGER_ENTRIES];
new gStateLedgerEntryTotal = 0;
new gStateLedgerEntryActor[MAX_STATE_LEDGER_ENTRIES][32];
new gStateLedgerEntryContext[MAX_STATE_LEDGER_ENTRIES][48];

stock StateLedgerSystem_Initialize()
{
    for (new i = 0; i < MAX_STATE_LEDGER_ENTRIES; i++)
    {
        gStateLedgerEntryState[i] = -1;
        gStateLedgerEntryType[i] = STATE_LEDGER_TYPE_ADMIN;
        gStateLedgerEntryAmount[i] = 0;
        format(gStateLedgerEntryActor[i], 32, "SYSTEM");
        format(gStateLedgerEntryContext[i], 48, "boot");
    }

    gStateLedgerEntryTotal = 0;

    StateLedgerSystem_LogTx(STATE_LOS_SANTOS, STATE_LEDGER_TYPE_TAX, 1500, "system", "initial_state_tax");
    StateLedgerSystem_LogTx(STATE_SAN_FIERRO, STATE_LEDGER_TYPE_BIZ, 1200, "system", "initial_business_fee");
    StateLedgerSystem_LogTx(STATE_LAS_VENTURAS, STATE_LEDGER_TYPE_GOV, 1800, "system", "initial_budget_transfer");

    print("[STATE_LEDGER] Treasury ledger initialized");
}

stock StateLedgerSystem_LogTx(stateid, type, amount, const actor[], const context[])
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (gStateLedgerEntryTotal >= MAX_STATE_LEDGER_ENTRIES)
    {
        return false;
    }

    new idx = gStateLedgerEntryTotal;
    gStateLedgerEntryState[idx] = stateid;
    gStateLedgerEntryType[idx] = type;
    gStateLedgerEntryAmount[idx] = amount;
    format(gStateLedgerEntryActor[idx], 32, "%s", actor);
    format(gStateLedgerEntryContext[idx], 48, "%s", context);
    gStateLedgerEntryTotal++;
    return true;
}

stock StateLedgerSystem_ValidateTransfer(stateid, amount)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    if (amount == 0)
    {
        return true;
    }

    new safeState = Core_GetValidStateOrDefault(stateid, STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    new beforeBalance = StateSystem_GetBankCash(safeState);
    if (amount < 0)
    {
        new negativeAmount = -amount;
        if (negativeAmount > beforeBalance)
        {
            return false;
        }
    }

    new nextBalance = beforeBalance + amount;
    if (nextBalance < 0)
    {
        return false;
    }

    return true;
}

stock StateLedgerSystem_ApplyValidatedTransfer(stateid, amount, const reason[], const actor[], ledgerType)
{
    new safeState = Core_GetValidStateOrDefault(stateid, STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState) || !StateLedgerSystem_ValidateTransfer(safeState, amount))
    {
        return false;
    }
    if (amount == 0)
    {
        return true;
    }

    new beforeBalance = StateSystem_GetBankCash(safeState);
    new nextBalance = beforeBalance + amount;
    if (nextBalance < 0)
    {
        return false;
    }

    if (!StateSystem_ApplyTreasuryDelta(safeState, amount))
    {
        return false;
    }

    new finalLedgerType = ledgerType;
    if (finalLedgerType < STATE_LEDGER_TYPE_TAX || finalLedgerType > STATE_LEDGER_TYPE_ADMIN)
    {
        finalLedgerType = (amount > 0) ? STATE_LEDGER_TYPE_BIZ : STATE_LEDGER_TYPE_ADMIN;
    }

    if (!StateLedgerSystem_LogTx(safeState, finalLedgerType, amount, actor, reason))
    {
        StateSystem_SetTreasuryBalance(safeState, beforeBalance);
        return false;
    }

    return true;
}

stock StateLedgerSystem_Transfer(stateid, amount, const reason[], const actor[])
{
    new safeState = Core_GetValidStateOrDefault(stateid, STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    new ledgerType = STATE_LEDGER_TYPE_GOV;
    if (amount > 0)
    {
        ledgerType = STATE_LEDGER_TYPE_BIZ;
    }
    else if (amount < 0)
    {
        ledgerType = STATE_LEDGER_TYPE_ADMIN;
    }

    return StateLedgerSystem_ApplyValidatedTransfer(safeState, amount, reason, actor, ledgerType);
}

stock int:StateLedgerSystem_GetEntryCount(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }

    new count = 0;
    for (new i = 0; i < gStateLedgerEntryTotal; i++)
    {
        if (gStateLedgerEntryState[i] == stateid)
        {
            count++;
        }
    }
    return count;
}

stock StateLedgerSystem_ShowSummary(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[STATE_LEDGER] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));

    new summary[256];
    format(summary, sizeof(summary), "[STATE_LEDGER] %s | Treasury: %d | Entries: %d", stateName, StateSystem_GetBankCash(stateid), StateLedgerSystem_GetEntryCount(stateid));
    SendClientMessage(playerid, 0x00D9FFFF, summary);

    for (new i = 0; i < gStateLedgerEntryTotal; i++)
    {
        if (gStateLedgerEntryState[i] != stateid)
        {
            continue;
        }

        new line[128];
        new typeName[24];
        switch (gStateLedgerEntryType[i])
        {
            case STATE_LEDGER_TYPE_TAX: format(typeName, sizeof(typeName), "TAX");
            case STATE_LEDGER_TYPE_BIZ: format(typeName, sizeof(typeName), "BIZ");
            case STATE_LEDGER_TYPE_GOV: format(typeName, sizeof(typeName), "GOV");
            case STATE_LEDGER_TYPE_JOB: format(typeName, sizeof(typeName), "JOB");
            default: format(typeName, sizeof(typeName), "ADMIN");
        }

        format(line, sizeof(line), "- %s | %s | %d | %s", gStateLedgerEntryActor[i], typeName, gStateLedgerEntryAmount[i], gStateLedgerEntryContext[i]);
        SendClientMessage(playerid, 0xFFFFFFFF, line);
    }
}

#endif
