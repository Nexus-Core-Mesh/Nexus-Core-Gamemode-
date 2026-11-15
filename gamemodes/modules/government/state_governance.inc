#if !defined STATE_GOVERNANCE_INC
#define STATE_GOVERNANCE_INC

#include <a_samp>

#define MAX_GOVERNANCE_OFFICES 8

new gStateGovernmentLevel[STATE_COUNT];
new gStateOfficeCount[STATE_COUNT];
new gStateOfficeName[STATE_COUNT][MAX_GOVERNANCE_OFFICES][32];

#if defined _STATE_BUDGET_SYSTEM_INC
    #undef gStateBudgetAlert
#endif

stock StateGovernance_Initialize()
{
    for (new i = 0; i < STATE_COUNT; i++)
    {
        gStateGovernmentLevel[i] = 1;
        gStateBudgetAlert[i] = 0;
        gStateOfficeCount[i] = 0;
        StateSystem_SetPolicy(i, "Stabilisierung");
        for (new j = 0; j < MAX_GOVERNANCE_OFFICES; j++)
        {
            format(gStateOfficeName[i][j], 32, "");
        }
    }

    StateGovernance_SetPolicy(STATE_LOS_SANTOS, "Urban Growth");
    StateGovernance_SetPolicy(STATE_SAN_FIERRO, "Industrial Growth");
    StateGovernance_SetPolicy(STATE_LAS_VENTURAS, "Tourism Growth");
    StateGovernance_SetLevel(STATE_LOS_SANTOS, 3);
    StateGovernance_SetLevel(STATE_SAN_FIERRO, 2);
    StateGovernance_SetLevel(STATE_LAS_VENTURAS, 3);

    StateGovernance_RegisterOffice(STATE_LOS_SANTOS, "City Hall");
    StateGovernance_RegisterOffice(STATE_SAN_FIERRO, "Harbor Office");
    StateGovernance_RegisterOffice(STATE_LAS_VENTURAS, "Tourism Office");

    print("[STATE_GOVERNANCE] Staatliche Verwaltung jedes Bundesstaates initialisiert");
}

stock StateGovernance_SetPolicy(stateid, const policy[])
{
    StateSystem_SetPolicy(stateid, policy);
}

stock StateGovernance_SetLevel(stateid, level)
{
    if (!Core_IsValidState(stateid))
    {
        return;
    }
    gStateGovernmentLevel[stateid] = level;
}

stock int:StateGovernance_GetLevel(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }
    return gStateGovernmentLevel[stateid];
}

stock StateGovernance_RegisterOffice(stateid, const office[])
{
    if (!Core_IsValidState(stateid) || gStateOfficeCount[stateid] >= MAX_GOVERNANCE_OFFICES)
    {
        return false;
    }

    format(gStateOfficeName[stateid][gStateOfficeCount[stateid]], 32, "%s", office);
    gStateOfficeCount[stateid]++;
    return true;
}

stock StateGovernance_ApproveBudget(stateid, amount)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (amount <= 0)
    {
        return false;
    }
    return StateSystem_ApplyTreasuryDelta(stateid, amount);
}

stock StateGovernance_ShowOverview(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[STATE_GOVERNANCE] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    new policy[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    StateSystem_GetPolicy(stateid, policy, sizeof(policy));
    new message[256];
    format(message, sizeof(message), "[GOVERNANCE] %s | Level: %d | Budget: %d | Policy: %s", stateName, gStateGovernmentLevel[stateid], StateSystem_GetBankCash(stateid), policy);
    SendClientMessage(playerid, 0x00D9FFFF, message);

    for (new i = 0; i < gStateOfficeCount[stateid]; i++)
    {
        new line[128];
        format(line, sizeof(line), "- Office: %s", gStateOfficeName[stateid][i]);
        SendClientMessage(playerid, 0xFFFFFFFF, line);
    }
}

#endif
