#if !defined GOVERNMENT_SYSTEM_INC
#define GOVERNMENT_SYSTEM_INC

#include <a_samp>

#define GOVERNMENT_NONE 0
#define GOVERNMENT_CITY 1
#define GOVERNMENT_STATE 2

new gGovernmentState[MAX_PLAYERS];

stock GovernmentSystem_Initialize()
{
    StateSystem_SetLeaderName(STATE_LOS_SANTOS, "Mayor LS");
    StateSystem_SetLeaderName(STATE_SAN_FIERRO, "Mayor SF");
    StateSystem_SetLeaderName(STATE_LAS_VENTURAS, "Mayor LV");
    print("[GOV] Drei Staatsregierungen initialisiert");
}

stock GovernmentSystem_SetStateLeader(stateid, const name[])
{
    StateSystem_SetLeaderName(stateid, name);
}

stock GovernmentSystem_SetPlayerRole(playerid, level)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    if (level < GOVERNMENT_NONE)
    {
        level = GOVERNMENT_NONE;
    }
    if (level > GOVERNMENT_STATE)
    {
        level = GOVERNMENT_STATE;
    }

    gGovernmentState[playerid] = level;
}

stock int:GovernmentSystem_GetPlayerRole(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return GOVERNMENT_NONE;
    }
    return gGovernmentState[playerid];
}

stock int:GovernmentSystem_GetBudget(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }
    return StateSystem_GetBankCash(stateid);
}

stock GovernmentSystem_TransferBudget(stateid, amount)
{
    if (!Core_IsValidState(stateid) || amount == 0)
    {
        return false;
    }

    if (!StateLedgerSystem_ValidateTransfer(stateid, amount))
    {
        return false;
    }

    return StateLedgerSystem_Transfer(stateid, amount, "government_budget_transfer", "government_system");
}

stock GovernmentSystem_ShowStateInfo(playerid, stateid)
{
    new message[128];
    new leaderName[24];
    StateSystem_GetLeaderName(stateid, leaderName, sizeof(leaderName));
    format(message, sizeof(message), "[GOV] Staat: %d | Budget: %d | Leiter: %s", stateid, StateSystem_GetBankCash(stateid), leaderName);
    SendClientMessage(playerid, 0x00D9FFFF, message);
}

#endif
