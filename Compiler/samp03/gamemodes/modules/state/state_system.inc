#if !defined STATE_SYSTEM_INC
#define STATE_SYSTEM_INC

#include <a_samp>

#if !defined STATE_LOS_SANTOS
#define STATE_LOS_SANTOS 0
#endif
#if !defined STATE_SAN_FIERRO
#define STATE_SAN_FIERRO 1
#endif
#if !defined STATE_LAS_VENTURAS
#define STATE_LAS_VENTURAS 2
#endif
#if !defined STATE_COUNT
#define STATE_COUNT 3
#endif

#if !defined STATE_NAME_LOS_SANTOS
#define STATE_NAME_LOS_SANTOS "Los Santos"
#endif
#if !defined STATE_NAME_SAN_FIERRO
#define STATE_NAME_SAN_FIERRO "San Fierro"
#endif
#if !defined STATE_NAME_LAS_VENTURAS
#define STATE_NAME_LAS_VENTURAS "Las Venturas"
#endif

new gStateBankCash[STATE_COUNT];
new gStateBank[STATE_COUNT];
new gGovernmentBudget[STATE_COUNT];
new gStateTaxRate[STATE_COUNT];
new gStateTaxRevenue[STATE_COUNT];
new gStateInfrastructureRevenue[STATE_COUNT];
new gStatePolicy[STATE_COUNT][24];
new gStateLeaderName[STATE_COUNT][24];
new gStateSpawnX[STATE_COUNT];
new gStateSpawnY[STATE_COUNT];
new gStateSpawnZ[STATE_COUNT];
new gStateEnabled[STATE_COUNT];

stock StateSystem_IsEnabled(stateid)
{
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }
    return gStateEnabled[stateid] == 1;
}

stock StateSystem_SetEnabled(stateid, bool:enabled)
{
    if (!StateSystem_IsValidState(stateid))
    {
        return;
    }
    gStateEnabled[stateid] = enabled ? 1 : 0;
}

stock int:StateSystem_NormalizeStateId(stateid)
{
    if (stateid >= 0 && stateid < STATE_COUNT)
    {
        return stateid;
    }
    return STATE_LOS_SANTOS;
}

stock StateSystem_IsValidState(stateid)
{
    return (stateid >= 0 && stateid < STATE_COUNT);
}

stock StateSystem_GetStateShortName(stateid, output[], len)
{
    if (!StateSystem_IsValidState(stateid))
    {
        format(output, len, "UNK");
        return;
    }

    if (stateid == STATE_LOS_SANTOS)
    {
        format(output, len, "LS");
    }
    else if (stateid == STATE_SAN_FIERRO)
    {
        format(output, len, "SF");
    }
    else if (stateid == STATE_LAS_VENTURAS)
    {
        format(output, len, "LV");
    }
    else
    {
        format(output, len, "UNK");
    }
}

stock StateSystem_Initialize()
{
    gStateBankCash[STATE_LOS_SANTOS] = 500000;
    gStateBankCash[STATE_SAN_FIERRO] = 450000;
    gStateBankCash[STATE_LAS_VENTURAS] = 400000;

    gStateTaxRate[STATE_LOS_SANTOS] = 8;
    gStateTaxRate[STATE_SAN_FIERRO] = 7;
    gStateTaxRate[STATE_LAS_VENTURAS] = 9;

    gStateTaxRevenue[STATE_LOS_SANTOS] = 0;
    gStateTaxRevenue[STATE_SAN_FIERRO] = 0;
    gStateTaxRevenue[STATE_LAS_VENTURAS] = 0;

    gStateInfrastructureRevenue[STATE_LOS_SANTOS] = 0;
    gStateInfrastructureRevenue[STATE_SAN_FIERRO] = 0;
    gStateInfrastructureRevenue[STATE_LAS_VENTURAS] = 0;

    format(gStatePolicy[STATE_LOS_SANTOS], 24, "Stabilitaet");
    format(gStatePolicy[STATE_SAN_FIERRO], 24, "Industrie");
    format(gStatePolicy[STATE_LAS_VENTURAS], 24, "Tourismus");

    format(gStateLeaderName[STATE_LOS_SANTOS], 24, "Mayor LS");
    format(gStateLeaderName[STATE_SAN_FIERRO], 24, "Mayor SF");
    format(gStateLeaderName[STATE_LAS_VENTURAS], 24, "Mayor LV");

    gStateEnabled[STATE_LOS_SANTOS] = 1;
    gStateEnabled[STATE_SAN_FIERRO] = 1;
    gStateEnabled[STATE_LAS_VENTURAS] = 1;

    gStateSpawnX[STATE_LOS_SANTOS] = 1500;
    gStateSpawnY[STATE_LOS_SANTOS] = -1710;
    gStateSpawnZ[STATE_LOS_SANTOS] = 13;

    gStateSpawnX[STATE_SAN_FIERRO] = -1930;
    gStateSpawnY[STATE_SAN_FIERRO] = 120;
    gStateSpawnZ[STATE_SAN_FIERRO] = 27;

    gStateSpawnX[STATE_LAS_VENTURAS] = 1700;
    gStateSpawnY[STATE_LAS_VENTURAS] = 1210;
    gStateSpawnZ[STATE_LAS_VENTURAS] = 16;

    DB_RegisterState(STATE_LOS_SANTOS, STATE_NAME_LOS_SANTOS, "LS", gStateBankCash[STATE_LOS_SANTOS], gStateTaxRate[STATE_LOS_SANTOS]);
    DB_RegisterState(STATE_SAN_FIERRO, STATE_NAME_SAN_FIERRO, "SF", gStateBankCash[STATE_SAN_FIERRO], gStateTaxRate[STATE_SAN_FIERRO]);
    DB_RegisterState(STATE_LAS_VENTURAS, STATE_NAME_LAS_VENTURAS, "LV", gStateBankCash[STATE_LAS_VENTURAS], gStateTaxRate[STATE_LAS_VENTURAS]);
    DB_SetStateTreasury(STATE_LOS_SANTOS, gStateBankCash[STATE_LOS_SANTOS]);
    DB_SetStateTreasury(STATE_SAN_FIERRO, gStateBankCash[STATE_SAN_FIERRO]);
    DB_SetStateTreasury(STATE_LAS_VENTURAS, gStateBankCash[STATE_LAS_VENTURAS]);

    print("[STATE] Drei Staaten aktiviert");
    printf("[STATE] Kassen: LS=%d, SF=%d, LV=%d", gStateBankCash[STATE_LOS_SANTOS], gStateBankCash[STATE_SAN_FIERRO], gStateBankCash[STATE_LAS_VENTURAS]);
}

stock StateSystem_OnPlayerConnect(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    DB_EnsurePlayerRecord(playerid, STATE_LOS_SANTOS);
    DB_LoadPlayerState(playerid);
    new stateid = DB_GetPlayerState(playerid);
    if (!Core_IsValidState(stateid))
    {
        stateid = STATE_LOS_SANTOS;
    }

    Core_SetPlayerState(playerid, stateid);
    Core_SetPlayerHomeState(playerid, stateid);
    Core_SetPlayerCash(playerid, DB_GetPlayerCash(playerid));
    DB_SavePlayerState(playerid, stateid);
}

stock StateSystem_OnPlayerDisconnect(playerid, reason)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    new stateid = StateSystem_GetPlayerState(playerid);
    if (!Core_IsValidState(stateid))
    {
        stateid = STATE_LOS_SANTOS;
    }

    Core_SetPlayerState(playerid, stateid);
    Core_SetPlayerHomeState(playerid, stateid);
    Core_SetPlayerCash(playerid, gPlayerCash[playerid]);
    DB_SavePlayerState(playerid, stateid);
}

stock StateSystem_OnPlayerSpawn(playerid)
{
    new stateName[32];
    StateSystem_GetStateName(Core_GetPlayerState(playerid), stateName, sizeof(stateName));

    SendClientMessage(playerid, 0x00FF00AA, "[STATE] Aktueller Staat:");
    SendClientMessage(playerid, 0x00FF00AA, stateName);
}

stock StateSystem_SetPlayerState(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    Core_SetPlayerState(playerid, stateid);
    DB_SetPlayerState(playerid, stateid);
    return true;
}

stock int:StateSystem_GetPlayerState(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return STATE_LOS_SANTOS;
    }
    return Core_ResolveStateId(Core_GetPlayerState(playerid));
}

stock StateSystem_GetStateName(stateid, output[], len)
{
    if (stateid == STATE_LOS_SANTOS)
    {
        format(output, len, STATE_NAME_LOS_SANTOS);
    }
    else if (stateid == STATE_SAN_FIERRO)
    {
        format(output, len, STATE_NAME_SAN_FIERRO);
    }
    else if (stateid == STATE_LAS_VENTURAS)
    {
        format(output, len, STATE_NAME_LAS_VENTURAS);
    }
    else
    {
        format(output, len, "Unbekannt");
    }
}

stock StateSystem_SpawnPlayerForState(playerid, stateid)
{
    new normalizedState = StateSystem_NormalizeStateId(stateid);

    if (!Core_IsValidState(normalizedState))
    {
        return;
    }

    SetPlayerPos(playerid, gStateSpawnX[normalizedState], gStateSpawnY[normalizedState], gStateSpawnZ[normalizedState]);
    SetPlayerFacingAngle(playerid, 0.0);
    Core_SetPlayerState(playerid, normalizedState);
    Core_SetPlayerHomeState(playerid, normalizedState);
}

stock int:StateSystem_GetBankCash(stateid)
{
    if (stateid < 0 || stateid >= STATE_COUNT)
    {
        return 0;
    }
    return gStateBankCash[stateid];
}

stock StateSystem_SetTreasuryBalance(stateid, amount)
{
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }

    if (amount < 0)
    {
        amount = 0;
    }

    gStateBankCash[stateid] = amount;
    gStateBank[stateid] = amount;
    gGovernmentBudget[stateid] = amount;

    DB_SetStateTreasury(stateid, amount);
    return true;
}

stock StateSystem_TransferToBank(stateid, amount)
{
    if (stateid < 0 || stateid >= STATE_COUNT)
    {
        return false;
    }

    return StateSystem_ApplyTreasuryDelta(stateid, amount);
}

stock int:StateSystem_AbsInt(value)
{
    return value < 0 ? -value : value;
}

stock StateSystem_ApplyTreasuryDelta(stateid, delta)
{
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }

    new nextBalance = gStateBankCash[stateid] + delta;
    if (nextBalance < 0)
    {
        return false;
    }

    if (delta > 0)
    {
        gStateTaxRevenue[stateid] += delta;
    }
    else if (delta < 0)
    {
        gStateInfrastructureRevenue[stateid] += (StateSystem_AbsInt(delta) / 2);
    }

    return StateSystem_SetTreasuryBalance(stateid, nextBalance);
}

stock int:StateSystem_GetTaxRate(stateid)
{
    if (stateid < 0 || stateid >= STATE_COUNT)
    {
        return 0;
    }
    return gStateTaxRate[stateid];
}

stock StateSystem_SetTaxRate(stateid, rate)
{
    if (!StateSystem_IsValidState(stateid) || rate < 0 || rate > 30)
    {
        return false;
    }

    gStateTaxRate[stateid] = rate;
    new stateName[32];
    new shortName[8];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    StateSystem_GetStateShortName(stateid, shortName, sizeof(shortName));
    return DB_RegisterState(stateid, stateName, shortName, gStateBankCash[stateid], rate);
}

stock int:StateSystem_GetTaxRevenue(stateid)
{
    if (!StateSystem_IsValidState(stateid))
    {
        return 0;
    }
    return gStateTaxRevenue[stateid];
}

stock int:StateSystem_GetInfraRev(stateid)
{
    if (!StateSystem_IsValidState(stateid))
    {
        return 0;
    }
    return gStateInfrastructureRevenue[stateid];
}

stock StateSystem_SetLeaderName(stateid, const name[])
{
    if (!StateSystem_IsValidState(stateid))
    {
        return;
    }
    format(gStateLeaderName[stateid], 24, "%s", name);
}

stock StateSystem_GetLeaderName(stateid, output[], len)
{
    if (!StateSystem_IsValidState(stateid))
    {
        format(output, len, "Unbekannt");
        return;
    }
    format(output, len, "%s", gStateLeaderName[stateid]);
}

stock StateSystem_SetPolicy(stateid, const policy[])
{
    if (!StateSystem_IsValidState(stateid))
    {
        return;
    }
    format(gStatePolicy[stateid], 24, "%s", policy);
}

stock StateSystem_GetPolicy(stateid, output[], len)
{
    if (!StateSystem_IsValidState(stateid))
    {
        format(output, len, "Unbekannt");
        return;
    }
    format(output, len, "%s", gStatePolicy[stateid]);
}

stock StateSystem_ShowOverview(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid) || !StateSystem_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[STATE] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    new leaderName[24];
    new policy[24];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    StateSystem_GetLeaderName(stateid, leaderName, sizeof(leaderName));
    StateSystem_GetPolicy(stateid, policy, sizeof(policy));

    new msg[256];
    format(msg, sizeof(msg), "[STATE] %s | Budget: $%d | Steuer: %d%% | Leader: %s | Policy: %s | Einnahmen: $%d", stateName, StateSystem_GetBankCash(stateid), StateSystem_GetTaxRate(stateid), leaderName, policy, StateSystem_GetTaxRevenue(stateid));
    SendClientMessage(playerid, 0x00D9FFFF, msg);
}

#endif
