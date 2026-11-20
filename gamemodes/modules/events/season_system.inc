#if !defined SEASON_SYSTEM_INC
#define SEASON_SYSTEM_INC

#include <a_samp>

#define SEASON_TYPE_NONE 0
#define SEASON_TYPE_SPRING 1
#define SEASON_TYPE_SUMMER 2
#define SEASON_TYPE_AUTUMN 3
#define SEASON_TYPE_WINTER 4

#define SEASON_REWARD_NONE 0
#define SEASON_REWARD_BRONZE 1
#define SEASON_REWARD_SILVER 2
#define SEASON_REWARD_GOLD 3
#define SEASON_REWARD_PLATINUM 4

new gSeasonType = SEASON_TYPE_SPRING;
new gSeasonStartTick = 0;
new gSeasonRewardTier = SEASON_REWARD_BRONZE;
new gSeasonStateModifier[STATE_COUNT];
new gSeasonRotationHours = 168;

stock SeasonSystem_Initialize()
{
    gSeasonType = SEASON_TYPE_SPRING;
    gSeasonStartTick = GetTickCount();
    gSeasonRewardTier = SEASON_REWARD_BRONZE;

    for (new i = 0; i < STATE_COUNT; i++)
    {
        gSeasonStateModifier[i] = 100;
    }

    gSeasonStateModifier[STATE_LOS_SANTOS] = 102;
    gSeasonStateModifier[STATE_SAN_FIERRO] = 104;
    gSeasonStateModifier[STATE_LAS_VENTURAS] = 108;

    print("[SEASON] Season-System initialisiert");
    Core_MarkModuleReady("season");
    return true;
}

stock SeasonSystem_GetStateModifier(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 100;
    }
    return gSeasonStateModifier[stateid];
}

stock SeasonSystem_SetStateModifier(stateid, modifier)
{
    if (!Core_IsValidState(stateid) || modifier < 50 || modifier > 200)
    {
        return false;
    }

    gSeasonStateModifier[stateid] = modifier;
    return true;
}

stock SeasonSystem_ApplyStateModifier(stateid, baseValue)
{
    if (!Core_IsValidState(stateid))
    {
        return baseValue;
    }
    return baseValue * gSeasonStateModifier[stateid] / 100;
}

stock SeasonSystem_GetSeasonSummary(stateid, output[], len)
{
    new seasonName[24];
    SeasonSystem_GetSeasonName(SeasonSystem_GetCurrentSeason(), seasonName, sizeof(seasonName));
    format(output, len, "%s | Job %d%% | Eco x%.2f | StateMod %d%% | Reward %d",
        seasonName,
        SeasonSystem_GetJobModifier(stateid),
        SeasonSystem_GetEconomyMultiplier(stateid),
        SeasonSystem_GetStateModifier(stateid),
        SeasonSystem_GetRewardTier());
}

stock SeasonSystem_GetCurrentSeason()
{
    if (gSeasonType < SEASON_TYPE_SPRING || gSeasonType > SEASON_TYPE_WINTER)
    {
        return SEASON_TYPE_SPRING;
    }
    return gSeasonType;
}

stock SeasonSystem_GetSeasonName(seasonid, output[], len)
{
    if (seasonid == SEASON_TYPE_NONE)
    {
        format(output, len, "Keine");
        return;
    }
    else if (seasonid == SEASON_TYPE_SPRING)
    {
        format(output, len, "Fruehling");
    }
    else if (seasonid == SEASON_TYPE_SUMMER)
    {
        format(output, len, "Sommer");
    }
    else if (seasonid == SEASON_TYPE_AUTUMN)
    {
        format(output, len, "Herbst");
    }
    else if (seasonid == SEASON_TYPE_WINTER)
    {
        format(output, len, "Winter");
    }
    else
    {
        format(output, len, "Fruehling");
    }
}

stock Float:SeasonSystem_GetEconomyMultiplier(stateid)
{
    new Float:multiplier = 1.0;
    new seasonid = SeasonSystem_GetCurrentSeason();

    switch (seasonid)
    {
        case SEASON_TYPE_SPRING:
        {
            multiplier = 1.0;
        }
        case SEASON_TYPE_SUMMER:
        {
            multiplier = 1.12;
        }
        case SEASON_TYPE_AUTUMN:
        {
            multiplier = 1.08;
        }
        case SEASON_TYPE_WINTER:
        {
            multiplier = 1.15;
        }
        default:
        {
            multiplier = 1.0;
        }
    }

    if (Core_IsValidState(stateid))
    {
        if (stateid == STATE_SAN_FIERRO)
        {
            multiplier *= 1.02;
        }
        else if (stateid == STATE_LAS_VENTURAS)
        {
            multiplier *= 1.04;
        }
    }

    if (Core_IsValidState(stateid))
    {
        multiplier *= float(gSeasonStateModifier[stateid]) / 100.0;
    }

    return multiplier;
}

stock SeasonSystem_GetBaseReward(stateid)
{
    new baseReward = 2500;
    if (!Core_IsValidState(stateid))
    {
        return baseReward;
    }

    if (stateid == STATE_SAN_FIERRO)
    {
        baseReward += 500;
    }
    else if (stateid == STATE_LAS_VENTURAS)
    {
        baseReward += 1000;
    }

    return baseReward * gSeasonStateModifier[stateid] / 100;
}

stock SeasonSystem_GetJobModifier(stateid)
{
    new modifier = 100;
    new seasonid = SeasonSystem_GetCurrentSeason();

    switch (seasonid)
    {
        case SEASON_TYPE_SPRING:
        {
            modifier = 100;
        }
        case SEASON_TYPE_SUMMER:
        {
            modifier = 112;
        }
        case SEASON_TYPE_AUTUMN:
        {
            modifier = 108;
        }
        case SEASON_TYPE_WINTER:
        {
            modifier = 115;
        }
    }

    if (Core_IsValidState(stateid))
    {
        if (stateid == STATE_LAS_VENTURAS)
        {
            modifier += 4;
        }
        modifier = modifier * gSeasonStateModifier[stateid] / 100;
    }

    return modifier;
}

stock SeasonSystem_ApplySeasonBonus(playerid, basePay)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return basePay;
    }

    new stateid = StateSystem_GetPlayerState(playerid);
    if (!Core_IsValidState(stateid))
    {
        return basePay;
    }

    new bonus = basePay * SeasonSystem_GetJobModifier(stateid) / 100;
    new adjusted = bonus * SeasonSystem_GetEconomyMultiplier(stateid) / 1.0;
    return adjusted;
}

stock SeasonSystem_AdvanceSeason()
{
    gSeasonType++;
    if (gSeasonType > SEASON_TYPE_WINTER)
    {
        gSeasonType = SEASON_TYPE_SPRING;
    }

    gSeasonStartTick = GetTickCount();
    if (gSeasonRewardTier >= SEASON_REWARD_PLATINUM)
    {
        gSeasonRewardTier = SEASON_REWARD_BRONZE;
    }
    else
    {
        gSeasonRewardTier++;
    }

    for (new i = 0; i < STATE_COUNT; i++)
    {
        gSeasonStateModifier[i] = 100 + ((gSeasonType + i) * 2);
        if (gSeasonStateModifier[i] > 200)
        {
            gSeasonStateModifier[i] = 200;
        }
    }

    return gSeasonType;
}

stock SeasonSystem_GetRewardTier()
{
    return gSeasonRewardTier;
}

stock SeasonSystem_ApplyStateEconomyPulse(stateid, baseDelta, const reason[], const actor[])
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new Float:multiplier = SeasonSystem_GetEconomyMultiplier(stateid);
    new adjustedDelta = floatround(float(baseDelta) * multiplier);
    if (adjustedDelta == 0)
    {
        return true;
    }

    if (!StateLedgerSystem_Transfer(stateid, adjustedDelta, reason, actor))
    {
        return false;
    }

    return true;
}

stock SeasonSystem_ApplyJobSeasonBonus(playerid, basePay)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return basePay;
    }

    new stateid = StateSystem_GetPlayerState(playerid);
    if (!Core_IsValidState(stateid))
    {
        return basePay;
    }

    new bonus = basePay * SeasonSystem_GetJobModifier(stateid) / 100;
    return bonus + SeasonSystem_ApplySeasonBonus(playerid, basePay);
}

stock SeasonSystem_NotifyPlayer(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    new seasonName[24];
    new stateid = StateSystem_GetPlayerState(playerid);
    SeasonSystem_GetSeasonName(SeasonSystem_GetCurrentSeason(), seasonName, sizeof(seasonName));
    new msg[192];
    format(msg, sizeof(msg), "[SEASON] Aktuelle Saison: %s | Staat %d | Wirtschaft x%.2f | Job-Modifier %d%% | Reward %d | StateMod %d%%",
        seasonName,
        stateid,
        SeasonSystem_GetEconomyMultiplier(stateid),
        SeasonSystem_GetJobModifier(stateid),
        SeasonSystem_GetRewardTier(),
        SeasonSystem_GetStateModifier(stateid));
    SendClientMessage(playerid, 0x7FD5FFFF, msg);
}

stock SeasonSystem_RewardPlayer(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new stateid = StateSystem_GetPlayerState(playerid);
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new reward = SeasonSystem_GetBaseReward(stateid);
    if (!Core_ChangePlayerBank(playerid, reward))
    {
        return false;
    }

    new msg[128];
    format(msg, sizeof(msg), "[SEASON] Saison-Belohnung erhalten: +$%d (%d)", reward, SeasonSystem_GetRewardTier());
    SendClientMessage(playerid, 0x00FF99AA, msg);

    StateLedgerSystem_Transfer(stateid, reward / 10, "season_player_reward", "season_system");
    return true;
}

#endif
