#if !defined STATE_TAX_SYSTEM_INC
#define STATE_TAX_SYSTEM_INC

#include <a_samp>

// State treasury tax/revenue data lives in state_system.inc as the single source of truth.
new gStateTaxSourceCount[STATE_COUNT];
new gStateTaxSourceName[STATE_COUNT][8][32];
new gStateTaxSourceValue[STATE_COUNT][8];

stock StateTaxSystem_Initialize()
{
    for (new i = 0; i < STATE_COUNT; i++)
    {
        gStateTaxSourceCount[i] = 0;
        for (new j = 0; j < 8; j++)
        {
            format(gStateTaxSourceName[i][j], 32, "");
            gStateTaxSourceValue[i][j] = 0;
        }
    }

    StateTaxSystem_RegisterSource(STATE_LOS_SANTOS, "Vehicle Tax", 1200);
    StateTaxSystem_RegisterSource(STATE_LOS_SANTOS, "Business Tax", 2200);
    StateTaxSystem_RegisterSource(STATE_SAN_FIERRO, "Port Fees", 1800);
    StateTaxSystem_RegisterSource(STATE_SAN_FIERRO, "Property Tax", 1500);
    StateTaxSystem_RegisterSource(STATE_LAS_VENTURAS, "Casino Tax", 2600);
    StateTaxSystem_RegisterSource(STATE_LAS_VENTURAS, "Tourism Levy", 2100);

    print("[STATE_TAX] Staatliche Steuer- und Revenue-Logik initialisiert");
}

stock StateTaxSystem_RegisterSource(stateid, const sourceName[], amount)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    if (gStateTaxSourceCount[stateid] >= 8)
    {
        return false;
    }

    format(gStateTaxSourceName[stateid][gStateTaxSourceCount[stateid]], 32, "%s", sourceName);
    gStateTaxSourceValue[stateid][gStateTaxSourceCount[stateid]] = amount;
    gStateTaxSourceCount[stateid]++;
    return true;
}

stock StateTaxSystem_CollectRevenue(stateid, amount)
{
    if (!Core_IsValidState(stateid) || amount <= 0)
    {
        return false;
    }

    return StateSystem_ApplyTreasuryDelta(stateid, amount);
}

stock StateTaxSystem_ApplyStateTax(stateid, taxRate)
{
    if (!Core_IsValidState(stateid) || taxRate < 0)
    {
        return false;
    }

    new revenue = StateSystem_GetBankCash(stateid) * taxRate / 100;
    return StateTaxSystem_CollectRevenue(stateid, revenue);
}

stock int:StateTaxSystem_GetRevenue(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }
    return StateSystem_GetTaxRevenue(stateid);
}

stock int:StateTaxSystem_GetInfrastructureRevenue(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }
    return StateSystem_GetInfraRev(stateid);
}

stock StateTaxSystem_ShowSummary(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[STATE_TAX] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));

    new msg[256];
    format(msg, sizeof(msg), "[STATE_TAX] %s | Treasury: $%d | TaxRevenue: $%d | Infra: $%d", stateName, StateSystem_GetBankCash(stateid), StateSystem_GetTaxRevenue(stateid), StateSystem_GetInfraRev(stateid));
    SendClientMessage(playerid, 0x00D9FFFF, msg);

    if (gStateTaxSourceCount[stateid] > 0)
    {
        new sources[256];
        format(sources, sizeof(sources), "[STATE_TAX] Quellen: ");
        for (new i = 0; i < gStateTaxSourceCount[stateid]; i++)
        {
            if (i > 0)
            {
                strcat(sources, " | ");
            }
            new entry[48];
            format(entry, sizeof(entry), "%s:$%d", gStateTaxSourceName[stateid][i], gStateTaxSourceValue[stateid][i]);
            strcat(sources, entry);
        }
        SendClientMessage(playerid, 0xFFFFFFFF, sources);
    }
}

#endif
