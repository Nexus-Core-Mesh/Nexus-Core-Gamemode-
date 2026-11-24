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

enum E_PLAYER_PROFILE
{
    E_PLAYER_STATE,
    E_PLAYER_HOME_STATE,
    E_PLAYER_ADMIN_LEVEL,
    E_PLAYER_ADMIN_STATE,
    E_PLAYER_LEVEL,
    E_PLAYER_XP,
    E_PLAYER_REP,
    E_PLAYER_BANK,
    E_PLAYER_WANTED,
    E_PLAYER_JAIL_ACTIVE,
    E_PLAYER_JAIL_TIME,
    E_PLAYER_LAW_ROLE,
    E_PLAYER_EMERGENCY_ROLE,
    E_PLAYER_EMERGENCY_STATE,
    E_PLAYER_PROFILE_COUNT
};

new gPlayerData[MAX_PLAYERS][E_PLAYER_PROFILE_COUNT];
new gPlayerBank[MAX_PLAYERS];
new gPlayerCash[MAX_PLAYERS];

#define CORE_SQLITE_ENABLED 1
#define CORE_LEGACY_ENABLED 1
#define CORE_MYSQL_ENABLED 0

new gSQLiteModuleEnabled = CORE_SQLITE_ENABLED;
new gLegacyModuleEnabled = CORE_LEGACY_ENABLED;
new gMySQLModuleEnabled = CORE_MYSQL_ENABLED;

stock Core_SetSQLiteModuleEnabled(enabled)
{
    gSQLiteModuleEnabled = enabled;
    return gSQLiteModuleEnabled;
}

stock Core_SetLegacyModuleEnabled(enabled)
{
    gLegacyModuleEnabled = enabled;
    return gLegacyModuleEnabled;
}

stock Core_SetMySQLModuleEnabled(enabled)
{
    gMySQLModuleEnabled = enabled;
    return gMySQLModuleEnabled;
}

stock Core_IsSQLiteModuleEnabled()
{
    return gSQLiteModuleEnabled == 1;
}

stock Core_IsLegacyModuleEnabled()
{
    return gLegacyModuleEnabled == 1;
}

stock Core_IsMySQLModuleEnabled()
{
    return gMySQLModuleEnabled == 1;
}

stock Core_HasActivePersistenceLayer()
{
    return Core_IsSQLiteModuleEnabled() || Core_IsLegacyModuleEnabled();
}

stock Core_GetPersistenceModeSummary(output[], len)
{
    if (Core_IsSQLiteModuleEnabled() && Core_IsLegacyModuleEnabled())
    {
        format(output, len, "sqlite");
        return;
    }

    if (Core_IsSQLiteModuleEnabled())
    {
        format(output, len, "sqlite-primary");
        return;
    }

    if (Core_IsLegacyModuleEnabled())
    {
        format(output, len, "sqlite");
        return;
    }

    format(output, len, "offline");
}

stock Core_CanProcessPersistenceHotpath(playerid, requireValidState = false)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    if (!Core_IsSQLiteModuleEnabled() && !Core_IsLegacyModuleEnabled())
    {
        return false;
    }

    if (requireValidState && !Core_IsValidState(gPlayerData[playerid][E_PLAYER_STATE]))
    {
        return false;
    }

    return true;
}

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
    gPlayerData[playerid][E_PLAYER_WANTED] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_ACTIVE] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_TIME] = 0;
}

stock Core_IsValidState(stateid)
{
    return (stateid >= STATE_LOS_SANTOS && stateid < STATE_COUNT);
}

stock int:Core_ResolveStateId(stateid)
{
    if (Core_IsValidState(stateid))
    {
        return stateid;
    }
    return STATE_LOS_SANTOS;
}

stock Core_IsValidAdminState(stateid)
{
    return (stateid == -1 || Core_IsValidState(stateid));
}

stock int:Core_GetPlayerState(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return STATE_LOS_SANTOS;
    }
    return gPlayerData[playerid][E_PLAYER_STATE];
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
        return 0;
    }
    if (gPlayerData[playerid][E_PLAYER_BANK] != gPlayerBank[playerid])
    {
        gPlayerData[playerid][E_PLAYER_BANK] = gPlayerBank[playerid];
    }
    return gPlayerData[playerid][E_PLAYER_BANK];
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

    new nextBank = Core_GetPlayerBank(playerid) + delta;
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
        return 0;
    }
    return gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL];
}

stock int:Core_GetPlayerAdminState(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return -1;
    }
    return gPlayerData[playerid][E_PLAYER_ADMIN_STATE];
}

stock Core_SetPlayerState(playerid, stateid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }
    if (!Core_IsValidState(stateid))
    {
        return;
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
        return;
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
        return STATE_LOS_SANTOS;
    }
    return gPlayerData[playerid][E_PLAYER_HOME_STATE];
}

stock int:Core_GetPlayerCash(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return 0;
    }
    return gPlayerCash[playerid];
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
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    new preservedState = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_STATE], STATE_LOS_SANTOS);
    new preservedHome = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_HOME_STATE], preservedState);

    gPlayerData[playerid][E_PLAYER_STATE] = preservedState;
    gPlayerData[playerid][E_PLAYER_HOME_STATE] = preservedHome;
    gPlayerData[playerid][E_PLAYER_ADMIN_LEVEL] = 0;
    gPlayerData[playerid][E_PLAYER_ADMIN_STATE] = -1;
    gPlayerData[playerid][E_PLAYER_WANTED] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_ACTIVE] = 0;
    gPlayerData[playerid][E_PLAYER_JAIL_TIME] = 0;
    gPlayerBank[playerid] = Core_GetPlayerBank(playerid);
    gPlayerCash[playerid] = Core_GetPlayerCash(playerid);
}

#endif
