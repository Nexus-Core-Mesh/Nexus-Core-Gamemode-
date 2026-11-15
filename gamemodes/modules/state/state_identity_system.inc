#if !defined STATE_IDENTITY_SYSTEM_INC
#define STATE_IDENTITY_SYSTEM_INC

#include <a_samp>

new gStateIdentityName[STATE_COUNT][32];
new gStateIdentityShort[STATE_COUNT][8];
new gStateIdentityCapital[STATE_COUNT][32];
new gStateIdentityFocus[STATE_COUNT][64];
new gStateIdentityTaxRate[STATE_COUNT];
new gStateIdentityTreasury[STATE_COUNT];

stock StateIdentitySystem_Initialize()
{
    format(gStateIdentityName[STATE_LOS_SANTOS], 32, "Los Santos");
    format(gStateIdentityShort[STATE_LOS_SANTOS], 8, "LS");
    format(gStateIdentityCapital[STATE_LOS_SANTOS], 32, "Downtown Hall");
    format(gStateIdentityFocus[STATE_LOS_SANTOS], 64, "Business, nightlife, trade");
    gStateIdentityTaxRate[STATE_LOS_SANTOS] = 8;
    gStateIdentityTreasury[STATE_LOS_SANTOS] = 500000;

    format(gStateIdentityName[STATE_SAN_FIERRO], 32, "San Fierro");
    format(gStateIdentityShort[STATE_SAN_FIERRO], 8, "SF");
    format(gStateIdentityCapital[STATE_SAN_FIERRO], 32, "Harbor Authority");
    format(gStateIdentityFocus[STATE_SAN_FIERRO], 64, "Industry, logistics, docks");
    gStateIdentityTaxRate[STATE_SAN_FIERRO] = 7;
    gStateIdentityTreasury[STATE_SAN_FIERRO] = 450000;

    format(gStateIdentityName[STATE_LAS_VENTURAS], 32, "Las Venturas");
    format(gStateIdentityShort[STATE_LAS_VENTURAS], 8, "LV");
    format(gStateIdentityCapital[STATE_LAS_VENTURAS], 32, "Casino District");
    format(gStateIdentityFocus[STATE_LAS_VENTURAS], 64, "Tourism, casinos, luxury economy");
    gStateIdentityTaxRate[STATE_LAS_VENTURAS] = 9;
    gStateIdentityTreasury[STATE_LAS_VENTURAS] = 400000;

    print("[STATE_IDENTITY] Staatliche Identitäten und Wirtschaftsprofile initialisiert");
}

stock StateIdentitySystem_RegisterStateIdentity(stateid, const name[], const shortName[], const capital[], const focus[], taxRate, treasury)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    format(gStateIdentityName[stateid], 32, "%s", name);
    format(gStateIdentityShort[stateid], 8, "%s", shortName);
    format(gStateIdentityCapital[stateid], 32, "%s", capital);
    format(gStateIdentityFocus[stateid], 64, "%s", focus);
    gStateIdentityTaxRate[stateid] = taxRate;
    gStateIdentityTreasury[stateid] = treasury;

    return true;
}

stock StateIdentitySystem_GetStateSummary(stateid, output[], len)
{
    if (!Core_IsValidState(stateid))
    {
        format(output, len, "Unbekannter Staat");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    format(output, len, "%s | %s | Hauptstadt: %s | Fokus: %s | Steuer: %d%% | Kasse: $%d", stateName, gStateIdentityShort[stateid], gStateIdentityCapital[stateid], gStateIdentityFocus[stateid], gStateIdentityTaxRate[stateid], gStateIdentityTreasury[stateid]);
}

stock StateIdentitySystem_ApplyRevenue(stateid, amount)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (amount <= 0)
    {
        return false;
    }

    new nextBalance = gStateIdentityTreasury[stateid] + amount;
    if (nextBalance < 0)
    {
        return false;
    }

    gStateIdentityTreasury[stateid] = nextBalance;
    return StateSystem_ApplyTreasuryDelta(stateid, amount);
}

stock StateIdentitySystem_ShowSummary(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    new summary[256];
    StateIdentitySystem_GetStateSummary(stateid, summary, sizeof(summary));
    SendClientMessage(playerid, 0x00D9FFFF, summary);
}

#endif
