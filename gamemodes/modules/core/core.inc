#if !defined CORE_INC
#define CORE_INC

#include <a_samp>

#if !defined MAX_INVENTORY_SLOTS
#define MAX_INVENTORY_SLOTS 24
#endif
#if !defined MAX_HOUSE_STORAGE_SLOTS
#define MAX_HOUSE_STORAGE_SLOTS 12
#endif
#if !defined MAX_HOUSES
#define MAX_HOUSES 50
#endif
#if !defined MAX_UBB_VEHICLES
#define MAX_UBB_VEHICLES 100
#endif

#if !defined INVENTORY_ITEM_NONE
#define INVENTORY_ITEM_NONE 0
#endif

#if !defined DIALOG_STYLE_TABLIST
#define DIALOG_STYLE_TABLIST 4
#endif
#if !defined DIALOG_STYLE_TABLIST_HEADERS
#define DIALOG_STYLE_TABLIST_HEADERS 5
#endif

stock Float:GetDistanceBetweenPoints3D(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2)
{
    return floatsqroot(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)) + ((z2 - z1) * (z2 - z1)));
}

#define CORE_VERSION "1.0.0"
#define MAX_STATES 3
#define STATE_COUNT 3
#define STATE_LOS_SANTOS 0
#define STATE_SAN_FIERRO 1
#define STATE_LAS_VENTURAS 2
#define STATE_NAME_LOS_SANTOS "Los Santos"
#define STATE_NAME_SAN_FIERRO "San Fierro"
#define STATE_NAME_LAS_VENTURAS "Las Venturas"

// Architektonische Contract-Regeln:
// 1. core.inc MUSS zuerst in gamemode.pwn inkludiert werden.
// 2. Alle Player-Arrays mit einem zentralen Strukturmodell verwalten.
// 3. Module dürfen keine eigenen globalen Player-Daten als „zweite Quelle der Wahrheit" definieren.
// 4. Beim Hinzufügen neuer Module immer zuerst das zentrale Profil erweitern und danach die einzelnen Module kompatibel halten.

// Pawn-Tag-Problem: enum-Member sind standardmäßig tagged; als Array-Indizes
// müssen sie nur als untagged Integer-Konstanten auftauchen, damit der Core
// konsistent mit den nachgelagerten int-Return-Funktionen arbeitet.
const E_PLAYER_STATE = 0;
const E_PLAYER_HOME_STATE = 1;
const E_PLAYER_ADMIN_LEVEL = 2;
const E_PLAYER_ADMIN_STATE = 3;
const E_PLAYER_LEVEL = 4;
const E_PLAYER_XP = 5;
const E_PLAYER_REP = 6;
const E_PLAYER_BANK = 7;
const E_PLAYER_WANTED = 8;
const E_PLAYER_JAIL_ACTIVE = 9;
const E_PLAYER_JAIL_TIME = 10;
const E_PLAYER_LAW_ROLE = 11;
const E_PLAYER_EMERGENCY_ROLE = 12;
const E_PLAYER_EMERGENCY_STATE = 13;
const E_PLAYER_PROJECT_LEADER = 14;
const E_PLAYER_PREMIUM_TIER = 15;
const E_PLAYER_PREMIUM_EXPIRES = 16;
const E_PLAYER_PREMIUM_PERMANENT = 17;
const E_PLAYER_PROFILE_COUNT = 18;

#define ADMIN_RANK_SUPPORT 1
#define ADMIN_RANK_MODERATOR 2
#define ADMIN_RANK_STATE_ADMIN 3
#define ADMIN_RANK_STATE_LEADER 4
#define ADMIN_RANK_OWNER 5

new gPlayerData[MAX_PLAYERS][E_PLAYER_PROFILE_COUNT];
new gPlayerBank[MAX_PLAYERS];
new gPlayerCash[MAX_PLAYERS];

stock Core_IsValidPlayerId(playerid)
{
    return (playerid >= 0 && playerid < MAX_PLAYERS);
}

stock Core_ResetPlayerData(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    for (new i = 0; i < E_PLAYER_PROFILE_COUNT; i++)
    {
        gPlayerData[playerid][i] = 0;
    }

    gPlayerData[playerid][E_PLAYER_STATE] = STATE_LOS_SANTOS;
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = STATE_LOS_SANTOS;
    gPlayerData[playerid][E_PLAYER_LEVEL] = 1;
    gPlayerData[playerid][E_PLAYER_BANK] = 2500;
    gPlayerBank[playerid] = 2500;
    gPlayerCash[playerid] = 2500;
    gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL] = 0;
    gPlayerData[playerid][E_PLAYER_ADMIN_STATE] = -1;
    gPlayerData[playerid][E_PLAYER_PROJECT_LEADER] = 0;
    gPlayerData[playerid][E_PLAYER_WANTED] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_ACTIVE] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_TIME] = 0;
    gPlayerData[playerid][E_PLAYER_PREMIUM_TIER] = 0;
    gPlayerData[playerid][E_PLAYER_PREMIUM_EXPIRES] = 0;
    gPlayerData[playerid][E_PLAYER_PREMIUM_PERMANENT] = 0;
}

stock Core_IsValidState(stateid)
{
    return (stateid >= STATE_LOS_SANTOS && stateid < STATE_COUNT);
}

stock int:Core_ResolveStateId(int:stateid)
{
    if (Core_IsValidState(stateid))
    {
        return stateid;
    }
    return int:STATE_LOS_SANTOS;
}

stock Core_IsValidAdminState(stateid)
{
    return (stateid == -1 || Core_IsValidState(stateid));
}

stock int:Core_GetPlayerState(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return int:STATE_LOS_SANTOS;
    }
    return int:gPlayerData[playerid][E_PLAYER_STATE];
}

stock Core_NormalizePlayerState(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    if (!Core_IsValidState(gPlayerData[playerid][E_PLAYER_STATE]))
    {
        gPlayerData[playerid][E_PLAYER_STATE] = STATE_LOS_SANTOS;
    }

    if (!Core_IsValidState(gPlayerData[playerid][E_PLAYER_HOME_STATE]))
    {
        gPlayerData[playerid][E_PLAYER_HOME_STATE] = STATE_LOS_SANTOS;
    }
}

stock int:Core_GetPlayerBank(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return int:0;
    }
    if (gPlayerData[playerid][E_PLAYER_BANK] != gPlayerBank[playerid])
    {
        gPlayerData[playerid][E_PLAYER_BANK] = gPlayerBank[playerid];
    }
    return int:gPlayerData[playerid][E_PLAYER_BANK];
}

stock Core_SetPlayerBank(playerid, amount)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    gPlayerData[playerid][E_PLAYER_BANK] = amount;
    gPlayerBank[playerid] = amount;
}

stock Core_ChangePlayerBank(playerid, delta)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new nextBank = Core_GetPlayerBank(playerid) + int:delta;
    if (nextBank < 0)
    {
        return false;
    }

    gPlayerData[playerid][E_PLAYER_BANK] = nextBank;
    gPlayerBank[playerid] = nextBank;
    return true;
}

stock int:Core_GetPlayerAdminLevel(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return int:0;
    }
    return int:gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL];
}

stock int:Core_GetPlayerProjectLeader(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return int:0;
    }
    return int:gPlayerData[playerid][E_PLAYER_PROJECT_LEADER];
}

stock Core_SetPlayerProjectLeader(playerid, enabled)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    gPlayerData[playerid][E_PLAYER_PROJECT_LEADER] = (enabled ? 1 : 0);
}

stock bool:Core_IsProjectLeader(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }
    return gPlayerData[playerid][E_PLAYER_PROJECT_LEADER] == 1 || Core_GetPlayerAdminLevel(playerid) >= ADMIN_RANK_OWNER;
}

stock int:Core_GetPlayerAdminState(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return int:-1;
    }
    return int:gPlayerData[playerid][E_PLAYER_ADMIN_STATE];
}

#define PREMIUM_TIER_NONE 0
#define PREMIUM_TIER_30_DAYS 1
#define PREMIUM_TIER_90_DAYS 2
#define PREMIUM_TIER_300_DAYS 3
#define PREMIUM_TIER_PERMANENT 4

stock Core_GetPlayerPremiumTier(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return PREMIUM_TIER_NONE;
    }
    return gPlayerData[playerid][E_PLAYER_PREMIUM_TIER];
}

stock Core_SetPlayerPremiumTier(playerid, tier)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    if (tier < PREMIUM_TIER_NONE || tier > PREMIUM_TIER_PERMANENT)
    {
        tier = PREMIUM_TIER_NONE;
    }
    gPlayerData[playerid][E_PLAYER_PREMIUM_TIER] = tier;
}

stock Core_GetPlayerPremiumExpiry(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return 0;
    }
    return gPlayerData[playerid][E_PLAYER_PREMIUM_EXPIRES];
}

stock Core_SetPlayerPremiumExpiry(playerid, expiresAt)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    gPlayerData[playerid][E_PLAYER_PREMIUM_EXPIRES] = expiresAt;
}

stock bool:Core_HasPermanentPremium(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }
    return gPlayerData[playerid][E_PLAYER_PREMIUM_PERMANENT] == 1;
}

stock Core_SetPermanentPremium(playerid, bool:permanent)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    gPlayerData[playerid][E_PLAYER_PREMIUM_PERMANENT] = permanent ? 1 : 0;
    if (permanent)
    {
        gPlayerData[playerid][E_PLAYER_PREMIUM_TIER] = PREMIUM_TIER_PERMANENT;
        gPlayerData[playerid][E_PLAYER_PREMIUM_EXPIRES] = 0;
    }
}

stock bool:Core_IsPremiumActive(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }
    if (Core_HasPermanentPremium(playerid))
    {
        return true;
    }

    new tier = Core_GetPlayerPremiumTier(playerid);
    new expiry = Core_GetPlayerPremiumExpiry(playerid);
    if (tier == PREMIUM_TIER_NONE)
    {
        return false;
    }
    if (expiry <= 0)
    {
        return false;
    }
    return gettime() < expiry;
}

stock Core_ActivatePremium(playerid, tier, expiresAt)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }
    if (tier < PREMIUM_TIER_NONE || tier > PREMIUM_TIER_PERMANENT)
    {
        return false;
    }

    if (tier == PREMIUM_TIER_PERMANENT)
    {
        Core_SetPermanentPremium(playerid, true);
        return true;
    }

    Core_SetPlayerPremiumTier(playerid, tier);
    Core_SetPlayerPremiumExpiry(playerid, expiresAt);
    Core_SetPermanentPremium(playerid, false);
    return true;
}

stock Core_ExtendPremium(playerid, tier, days)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }
    if (days <= 0)
    {
        return false;
    }

    if (tier == PREMIUM_TIER_PERMANENT)
    {
        Core_SetPermanentPremium(playerid, true);
        return true;
    }

    new now = gettime();
    new exp = Core_GetPlayerPremiumExpiry(playerid);
    if (exp <= now)
    {
        exp = now;
    }
    new secs = days * 86400;
    Core_SetPlayerPremiumTier(playerid, tier);
    Core_SetPlayerPremiumExpiry(playerid, exp + secs);
    Core_SetPermanentPremium(playerid, false);
    return true;
}

stock Core_SetPlayerState(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    if (!Core_IsValidState(stateid))
    {
        printf("[CORE] invalid state rejected: player=%d state=%d", playerid, stateid);
        return;
    }
    new oldState = gPlayerData[playerid][E_PLAYER_STATE];
    if (oldState != stateid)
    {
        printf("[CORE] state transition: player=%d old=%d new=%d", playerid, oldState, stateid);
    }
    gPlayerData[playerid][E_PLAYER_STATE] = stateid;
}

stock Core_SetPlayerHomeState(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    if (!Core_IsValidState(stateid))
    {
        printf("[CORE] invalid home-state rejected: player=%d state=%d", playerid, stateid);
        return;
    }
    new oldState = gPlayerData[playerid][E_PLAYER_HOME_STATE];
    if (oldState != stateid)
    {
        printf("[CORE] home-state transition: player=%d old=%d new=%d", playerid, oldState, stateid);
    }
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = stateid;
}

stock Core_SetPlayerAdminLevel(playerid, level)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL] = level;
}

stock Core_SetPlayerAdminState(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    if (!Core_IsValidAdminState(stateid))
    {
        return;
    }
    gPlayerData[playerid][E_PLAYER_ADMIN_STATE] = stateid;
}

stock Core_GetStateName(stateid, output[], len)
{
    if (!Core_IsValidState(stateid))
    {
        format(output, len, "Unknown");
        return;
    }

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
        format(output, len, "Unknown");
    }
}

stock Core_GetStateShortName(stateid, output[], len)
{
    if (!Core_IsValidState(stateid))
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

stock Core_HasAdminLevel(playerid, requiredLevel)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }
    return gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL] >= requiredLevel;
}

stock Core_IsPlayerInState(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }
    return Core_IsValidState(stateid) && gPlayerData[playerid][E_PLAYER_STATE] == stateid;
}

stock Core_IsPlayerHomeState(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }
    return Core_IsValidState(stateid) && gPlayerData[playerid][E_PLAYER_HOME_STATE] == stateid;
}

stock int:Core_GetValidStateOrDefault(stateid, fallbackState)
{
    if (Core_IsValidState(stateid))
    {
        return stateid;
    }
    if (Core_IsValidState(fallbackState))
    {
        return fallbackState;
    }
    return STATE_LOS_SANTOS;
}

stock int:Core_GetPlayerHomeState(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return int:STATE_LOS_SANTOS;
    }
    return int:gPlayerData[playerid][E_PLAYER_HOME_STATE];
}

stock int:Core_GetPlayerCash(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return int:0;
    }
    return int:gPlayerCash[playerid];
}

stock Core_SetPlayerCash(playerid, amount)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    gPlayerCash[playerid] = amount;
}

stock Core_ChangePlayerCash(playerid, delta)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new nextCash = gPlayerCash[playerid] + delta;
    if (nextCash < 0)
    {
        return false;
    }

    gPlayerCash[playerid] = nextCash;
    return true;
}

stock InitCore()
{
    print("[CORE] Initialisiert");
    print("[CORE] Version: " CORE_VERSION);
    print("[CORE] Architektur-Contract: zentrale PlayerData-Struktur aktiv");
}

stock Core_OnPlayerConnect(playerid)
{
    Core_ResetPlayerData(playerid);
    SendClientMessage(playerid, 0x33CCFFAA, "Willkommen auf dem GTA:SA Reallife Server.");
    SendClientMessage(playerid, 0xFFFFFFFF, "Mit /help kannst du die wichtigsten Befehle sehen.");
}

stock Core_OnPlayerDisconnect(playerid, reason)
{
    (void:reason);
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    new stateid = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_STATE], STATE_LOS_SANTOS);
    new homeState = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_HOME_STATE], stateid);
    if (!Core_IsValidState(stateid))
    {
        stateid = STATE_LOS_SANTOS;
    }
    if (!Core_IsValidState(homeState))
    {
        homeState = stateid;
    }

    gPlayerData[playerid][E_PLAYER_STATE] = stateid;
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = homeState;
    gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL] = 0;
    gPlayerData[playerid][E_PLAYER_ADMIN_STATE] = -1;
    gPlayerData[playerid][E_PLAYER_WANTED] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_ACTIVE] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_TIME] = 0;
    gPlayerBank[playerid] = Core_GetPlayerBank(playerid);
    gPlayerCash[playerid] = Core_GetPlayerCash(playerid);
}

#endif
