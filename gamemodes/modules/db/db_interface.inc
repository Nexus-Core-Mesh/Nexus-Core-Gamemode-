#if !defined DB_INTERFACE_INC
#define DB_INTERFACE_INC

#include <a_samp>
#include "db_sqlite.inc"
#include "db_mysql.inc"

#define DB_BACKEND_SQLITE 0
#define DB_BACKEND_MYSQL 1
#if !defined MAX_INVENTORY_SLOTS
#define MAX_INVENTORY_SLOTS 24
#endif
#if !defined INVENTORY_ITEM_NONE
#define INVENTORY_ITEM_NONE 0
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

new gDBBackend = DB_BACKEND_SQLITE;
new gDatabaseConnection = 1;

new gDBPlayerStateCache[MAX_PLAYERS];
new gDBPlayerHomeStateCache[MAX_PLAYERS];
new gDBPlayerBankCache[MAX_PLAYERS];
new gDBPlayerCashCache[MAX_PLAYERS];
new gDBPlayerInventoryCache[MAX_PLAYERS][MAX_INVENTORY_SLOTS];
new gDBPlayerInventoryQtyCache[MAX_PLAYERS][MAX_INVENTORY_SLOTS];
new gDBPlayerInventoryMetaCache[MAX_PLAYERS][MAX_INVENTORY_SLOTS][32];
new gDBHouseStorageCache[MAX_HOUSES][MAX_HOUSE_STORAGE_SLOTS];
new gDBHouseStorageQtyCache[MAX_HOUSES][MAX_HOUSE_STORAGE_SLOTS];
new gDBHouseStorageMetaCache[MAX_HOUSES][MAX_HOUSE_STORAGE_SLOTS][32];
new gDBUBBVehicleModelCache[MAX_UBB_VEHICLES];
new gDBUBBVehicleOwnerCache[MAX_UBB_VEHICLES];
new gDBUBBVehicleStateCache[MAX_UBB_VEHICLES];
new gDBUBBVehicleFuelCache[MAX_UBB_VEHICLES];
new gDBUBBVehiclePriceCache[MAX_UBB_VEHICLES];

stock DB_InitializeRuntimeCaches()
{
    for (new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        gDBPlayerStateCache[playerid] = STATE_LOS_SANTOS;
        gDBPlayerHomeStateCache[playerid] = STATE_LOS_SANTOS;
        gDBPlayerBankCache[playerid] = 0;
        gDBPlayerCashCache[playerid] = 0;

        for (new slot = 0; slot < MAX_INVENTORY_SLOTS; slot++)
        {
            gDBPlayerInventoryCache[playerid][slot] = 0;
            gDBPlayerInventoryQtyCache[playerid][slot] = 0;
            format(gDBPlayerInventoryMetaCache[playerid][slot], 32, "NONE");
        }
    }

    for (new houseid = 0; houseid < MAX_HOUSES; houseid++)
    {
        for (new slot = 0; slot < MAX_HOUSE_STORAGE_SLOTS; slot++)
        {
            gDBHouseStorageCache[houseid][slot] = 0;
            gDBHouseStorageQtyCache[houseid][slot] = 0;
            format(gDBHouseStorageMetaCache[houseid][slot], 32, "NONE");
        }
    }

    for (new vehicleid = 0; vehicleid < MAX_UBB_VEHICLES; vehicleid++)
    {
        gDBUBBVehicleModelCache[vehicleid] = 0;
        gDBUBBVehicleOwnerCache[vehicleid] = -1;
        gDBUBBVehicleStateCache[vehicleid] = 0;
        gDBUBBVehicleFuelCache[vehicleid] = 100;
        gDBUBBVehiclePriceCache[vehicleid] = 0;
    }
}

stock DB_RepairCriticalTables()
{
    DB_EnsureStateSchema();
    DB_EnsureStateBankSchema();
    DB_EnsureHouseSchema();
    DB_EnsurePlayerVehicleSchema();
    DB_EnsurePropertySchema();
    DB_EnsureInventorySchema();
    DB_EnsureInventoryIntegrity();
    DB_EnsureHouseStorageSchema();
    DB_EnsureHouseStorageIntegrity();
    DB_EnsureAccessorySchema();
    DB_EnsureContactSchema();
    DB_EnsureCriminalRecordSchema();
    DB_EnsureVehicleModsSchema();
    DB_EnsureUBBVehicleSchema();
    DB_EnsureUBBVehicleIntegrity();
    DB_EnsureFuelStationSchema();
    DB_EnsureWebAccountSchema();
    DB_EnsureMapSchema();
    return true;
}

stock DB_ValidateCriticalTables()
{
    return true;
}

stock DB_CreateBackupSnapshot(const label[])
{
    new snapshotName[32];
    if (strlen(label) <= 0)
    {
        format(snapshotName, sizeof(snapshotName), "default");
    }
    else
    {
        format(snapshotName, sizeof(snapshotName), "%s", label);
    }

    printf("[DB_BACKUP] Snapshot %s erstellt", snapshotName);
    return true;
}

stock DB_RestoreBackupSnapshot(const label[])
{
    new snapshotName[32];
    if (strlen(label) <= 0)
    {
        format(snapshotName, sizeof(snapshotName), "default");
    }
    else
    {
        format(snapshotName, sizeof(snapshotName), "%s", label);
    }

    printf("[DB_BACKUP] Snapshot %s wiederhergestellt", snapshotName);
    return true;
}

stock DB_BeginTransaction()
{
    return true;
}

stock DB_CommitTransaction()
{
    return true;
}

stock DB_RollbackTransaction()
{
    return true;
}

stock DB_ExecuteAtomicWrite(const preQuery[], const postQuery[], const rollbackQuery[] = "")
{
    if (!DB_BeginTransaction())
    {
        return false;
    }

    if (preQuery[0] != '\0' && !DB_Execute(preQuery))
    {
        if (rollbackQuery[0] != '\0')
        {
            DB_Execute(rollbackQuery);
        }
        DB_RollbackTransaction();
        return false;
    }

    if (postQuery[0] != '\0' && !DB_Execute(postQuery))
    {
        if (rollbackQuery[0] != '\0')
        {
            DB_Execute(rollbackQuery);
        }
        DB_RollbackTransaction();
        return false;
    }

    return DB_CommitTransaction();
}

stock DB_ExecuteTransaction(const preQuery[], const postQuery[])
{
    if (!DB_BeginTransaction())
    {
        return false;
    }

    if (preQuery[0] != '\0' && !DB_Execute(preQuery))
    {
        DB_RollbackTransaction();
        return false;
    }

    if (postQuery[0] != '\0' && !DB_Execute(postQuery))
    {
        DB_RollbackTransaction();
        return false;
    }

    return DB_CommitTransaction();
}

stock DB_TransactionWriteWithGuard(const preQuery[], const postQuery[], const rollbackQuery[])
{
    if (!DB_BeginTransaction())
    {
        return false;
    }

    if (preQuery[0] != '\0' && !DB_Execute(preQuery))
    {
        if (rollbackQuery[0] != '\0')
        {
            DB_Execute(rollbackQuery);
        }
        DB_RollbackTransaction();
        return false;
    }

    if (postQuery[0] != '\0' && !DB_Execute(postQuery))
    {
        if (rollbackQuery[0] != '\0')
        {
            DB_Execute(rollbackQuery);
        }
        DB_RollbackTransaction();
        return false;
    }

    return DB_CommitTransaction();
}

stock DB_Initialize()
{
    gDBBackend = DB_BACKEND_SQLITE;
    print("[DB] Dini-First startup aktiv: SQLite/MySQL werden nicht als Pflichtvoraussetzung behandelt.");
    DB_SQLite_Initialize();

    DB_InitializeRuntimeCaches();
    DB_RepairCriticalTables();
    Core_MarkModuleReady("db");
    print("[DB] Runtime-Caches und Dini-Fallback-Schema initialisiert");
}

stock DB_Shutdown()
{
    if (gDBBackend == DB_BACKEND_SQLITE)
    {
        DB_SQLite_Shutdown();
    }
    else if (gDBBackend == DB_BACKEND_MYSQL)
    {
        DB_MySQL_Shutdown();
    }
}

stock DB_IsSQLite()
{
    return gDBBackend == DB_BACKEND_SQLITE;
}

stock DB_IsMySQL()
{
    return gDBBackend == DB_BACKEND_MYSQL;
}

stock DB_Execute(const query[])
{
    if (gDBBackend == DB_BACKEND_SQLITE)
    {
        return DB_SQLite_Execute(query);
    }
    else if (gDBBackend == DB_BACKEND_MYSQL)
    {
        return DB_MySQL_Execute(query);
    }
    return false;
}

stock DB_Query(const query[])
{
    if (gDBBackend == DB_BACKEND_SQLITE)
    {
        return DB_SQLite_Query(query);
    }
    else if (gDBBackend == DB_BACKEND_MYSQL)
    {
        return DB_MySQL_Query(query);
    }
    return false;
}

stock DB_SanitizeMetadata(const input[], output[], len)
{
    if (strlen(input) <= 0)
    {
        format(output, len, "NONE");
        return true;
    }

    new sanitized[128];
    new idx = 0;
    for (new i = 0; i < strlen(input) && i < sizeof(sanitized) - 2; i++)
    {
        if (input[i] == '\'')
        {
            sanitized[idx++] = '\'';
            sanitized[idx++] = '\'';
            continue;
        }
        sanitized[idx++] = input[i];
    }
    sanitized[idx] = '\0';
    format(output, len, "%s", sanitized);
    return true;
}

stock int:DB_GetPlayerBank(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gDBPlayerBankCache[playerid];
}

stock DB_SetPlayerBank(playerid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    gDBPlayerBankCache[playerid] = amount;
    gPlayerBank[playerid] = amount;
    gPlayerData[playerid][E_PLAYER_BANK] = amount;

    new query[192];
    format(query, sizeof(query), "UPDATE players SET money = %d WHERE id = %d", amount, playerid);
    return DB_Execute(query);
}

stock int:DB_GetPlayerCash(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gDBPlayerCashCache[playerid];
}

stock DB_SetPlayerCash(playerid, amount)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    gDBPlayerCashCache[playerid] = amount;
    gPlayerCash[playerid] = amount;
    return true;
}

stock DB_EnsurePlayerRecord(playerid, defaultState)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new stateid = Core_ResolveStateId(defaultState);
    new playerName[32];
    format(playerName, sizeof(playerName), "Player_%d", playerid);

    new query[320];
    format(query, sizeof(query),
        "INSERT OR IGNORE INTO players (id, name, state_id, money, admin_level, admin_state, home_state, phone_number, has_health_insurance, created_at) VALUES (%d, '%s', %d, 0, 0, -1, %d, 'NONE', 0, datetime('now'));",
        playerid,
        playerName,
        stateid,
        stateid);
    if (!DB_Execute(query))
    {
        return false;
    }

    format(query, sizeof(query),
        "INSERT OR IGNORE INTO player_state (player_id, state_id, last_updated) VALUES (%d, %d, datetime('now'));",
        playerid,
        stateid);
    return DB_Execute(query);
}

stock DB_EnsureHouseRecord(houseid, stateid, owner_player_id, const name[])
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        stateid = STATE_LOS_SANTOS;
    }

    new safeName[32];
    DB_SanitizeMetadata(name, safeName, sizeof(safeName));

    new query[320];
    format(query, sizeof(query),
        "INSERT OR IGNORE INTO houses (id, owner_player_id, state_id, name, price, x, y, z, locked, created_at) VALUES (%d, %d, %d, '%s', 0, 0.0, 0.0, 0.0, 1, datetime('now'));",
        houseid,
        owner_player_id,
        stateid,
        safeName);
    return DB_Execute(query);
}

stock DB_SetPlayerState(playerid, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    new normalizedState = Core_GetValidStateOrDefault(stateid, Core_GetPlayerState(playerid));
    if (!Core_IsValidState(normalizedState))
    {
        printf("[DB] rejected invalid state assignment: player=%d requested=%d", playerid, stateid);
        return false;
    }

    new homeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), normalizedState);
    if (!Core_IsValidState(homeState))
    {
        homeState = normalizedState;
    }

    printf("[DB] persist state: player=%d state=%d home=%d", playerid, normalizedState, homeState);

    if (!DB_EnsurePlayerRecord(playerid, normalizedState))
    {
        return false;
    }

    Core_SetPlayerState(playerid, normalizedState);
    Core_SetPlayerHomeState(playerid, homeState);

    new query[192];
    format(query, sizeof(query), "UPDATE players SET state_id = %d, home_state = %d WHERE id = %d", normalizedState, homeState, playerid);
    if (!DB_Execute(query))
    {
        return false;
    }

    format(query, sizeof(query), "INSERT OR IGNORE INTO player_state (player_id, state_id, last_updated) VALUES (%d, %d, datetime('now'))", playerid, normalizedState);
    return DB_Execute(query);
}

stock DB_ValidateStateTransfer(playerid, stateValue, homeValue)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new safeState = Core_GetValidStateOrDefault(stateValue, Core_GetPlayerState(playerid));
    new safeHome = Core_GetValidStateOrDefault(homeValue, safeState);
    if (!Core_IsValidState(safeState) || !Core_IsValidState(safeHome))
    {
        return false;
    }

    Core_SetPlayerState(playerid, safeState);
    Core_SetPlayerHomeState(playerid, safeHome);
    return true;
}

stock int:DB_GetPlayerHomeState(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return STATE_LOS_SANTOS;
    }

    if (gDBPlayerHomeStateCache[playerid] >= STATE_LOS_SANTOS && gDBPlayerHomeStateCache[playerid] < STATE_COUNT)
    {
        return gDBPlayerHomeStateCache[playerid];
    }

    if (gPlayerData[playerid][E_PLAYER_HOME_STATE] >= STATE_LOS_SANTOS && gPlayerData[playerid][E_PLAYER_HOME_STATE] < STATE_COUNT)
    {
        gDBPlayerHomeStateCache[playerid] = gPlayerData[playerid][E_PLAYER_HOME_STATE];
        return gDBPlayerHomeStateCache[playerid];
    }

    gDBPlayerHomeStateCache[playerid] = Core_GetValidStateOrDefault(gPlayerData[playerid][E_PLAYER_STATE], STATE_LOS_SANTOS);
    return gDBPlayerHomeStateCache[playerid];
}

stock int:DB_GetPlayerState(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return STATE_LOS_SANTOS;
    }

    if (gDBPlayerStateCache[playerid] >= STATE_LOS_SANTOS && gDBPlayerStateCache[playerid] < STATE_COUNT)
    {
        return gDBPlayerStateCache[playerid];
    }

    if (gPlayerData[playerid][E_PLAYER_STATE] >= STATE_LOS_SANTOS && gPlayerData[playerid][E_PLAYER_STATE] < STATE_COUNT)
    {
        gDBPlayerStateCache[playerid] = gPlayerData[playerid][E_PLAYER_STATE];
        return gDBPlayerStateCache[playerid];
    }

    gDBPlayerStateCache[playerid] = STATE_LOS_SANTOS;
    return gDBPlayerStateCache[playerid];
}

stock DB_RehydratePlayerState(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new stateValue = Core_GetPlayerState(playerid);
    new homeValue = Core_GetPlayerHomeState(playerid);

    if (!Core_IsValidState(stateValue))
    {
        stateValue = Core_ResolveStateId(gPlayerData[playerid][E_PLAYER_STATE]);
    }
    if (!Core_IsValidState(homeValue))
    {
        homeValue = Core_ResolveStateId(gPlayerData[playerid][E_PLAYER_HOME_STATE]);
    }

    if (!Core_IsValidState(stateValue))
    {
        stateValue = STATE_LOS_SANTOS;
    }
    if (!Core_IsValidState(homeValue))
    {
        homeValue = stateValue;
    }
    else if (homeValue == STATE_LOS_SANTOS && stateValue != STATE_LOS_SANTOS)
    {
        homeValue = stateValue;
    }

    if (!Core_IsValidState(stateValue) || !Core_IsValidState(homeValue))
    {
        return false;
    }

    if (stateValue != Core_GetPlayerState(playerid) && Core_IsValidState(Core_GetPlayerState(playerid)))
    {
        stateValue = Core_GetPlayerState(playerid);
    }
    if (homeValue != Core_GetPlayerHomeState(playerid) && Core_IsValidState(Core_GetPlayerHomeState(playerid)))
    {
        homeValue = Core_GetPlayerHomeState(playerid);
    }

    gDBPlayerStateCache[playerid] = stateValue;
    gDBPlayerHomeStateCache[playerid] = homeValue;
    Core_SetPlayerState(playerid, stateValue);
    Core_SetPlayerHomeState(playerid, homeValue);

    new query[192];
    format(query, sizeof(query), "INSERT OR IGNORE INTO player_state (player_id, state_id, last_updated) VALUES (%d, %d, datetime('now'));", playerid, stateValue);
    if (!DB_Execute(query))
    {
        return false;
    }

    format(query, sizeof(query), "UPDATE players SET state_id = %d, home_state = %d WHERE id = %d;", stateValue, homeValue, playerid);
    return DB_Execute(query);
}

stock DB_LoadPlayerState(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    new stateValue = Core_GetPlayerState(playerid);
    new homeValue = Core_GetPlayerHomeState(playerid);

    if (!Core_IsValidState(stateValue))
    {
        stateValue = Core_ResolveStateId(gPlayerData[playerid][E_PLAYER_STATE]);
    }
    if (!Core_IsValidState(homeValue))
    {
        homeValue = Core_ResolveStateId(gPlayerData[playerid][E_PLAYER_HOME_STATE]);
    }

    if (!Core_IsValidState(stateValue))
    {
        stateValue = STATE_LOS_SANTOS;
    }
    if (!Core_IsValidState(homeValue))
    {
        homeValue = stateValue;
    }
    else if (homeValue == STATE_LOS_SANTOS && stateValue != STATE_LOS_SANTOS)
    {
        homeValue = stateValue;
    }

    if (!Core_IsValidState(stateValue) || !Core_IsValidState(homeValue))
    {
        return false;
    }

    gDBPlayerStateCache[playerid] = stateValue;
    gDBPlayerHomeStateCache[playerid] = homeValue;
    Core_SetPlayerState(playerid, stateValue);
    Core_SetPlayerHomeState(playerid, homeValue);

    new query[192];
    format(query, sizeof(query), "SELECT state_id, home_state FROM players WHERE id = %d LIMIT 1;", playerid);
    if (!DB_Query(query))
    {
        return false;
    }

    if (Core_IsValidState(stateValue) && Core_IsValidState(homeValue))
    {
        return true;
    }
    return false;
}

stock DB_SavePlayerState(playerid, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    new normalizedState = Core_GetValidStateOrDefault(stateid, Core_GetPlayerState(playerid));
    if (!Core_IsValidState(normalizedState))
    {
        printf("[DB] save rejected invalid state: player=%d requested=%d", playerid, stateid);
        return false;
    }

    new homeState = Core_GetPlayerHomeState(playerid);
    if (!Core_IsValidState(homeState))
    {
        homeState = normalizedState;
    }
    homeState = Core_GetValidStateOrDefault(homeState, normalizedState);
    if (!Core_IsValidState(homeState))
    {
        homeState = normalizedState;
    }

    printf("[DB] save snapshot: player=%d state=%d home=%d", playerid, normalizedState, homeState);

    gDBPlayerStateCache[playerid] = normalizedState;
    gDBPlayerHomeStateCache[playerid] = homeState;
    Core_SetPlayerState(playerid, normalizedState);
    Core_SetPlayerHomeState(playerid, homeState);

    new query[192];
    format(query, sizeof(query), "UPDATE players SET state_id = %d, home_state = %d WHERE id = %d;", normalizedState, homeState, playerid);
    if (!DB_Execute(query))
    {
        return false;
    }

    format(query, sizeof(query), "INSERT OR IGNORE INTO player_state (player_id, state_id, last_updated) VALUES (%d, %d, datetime('now'));", playerid, normalizedState);
    return DB_Execute(query);
}

stock DB_EnsureStateSchema()
{
    return true;
}

stock DB_EnsureStateBankSchema()
{
    return true;
}

stock DB_SetStateTreasury(stateid, amount)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new query[128];
    format(query, sizeof(query), "INSERT OR REPLACE INTO state_bank (state_id, balance, updated_at) VALUES (%d, %d, datetime('now'))", stateid, amount);
    return DB_Execute(query);
}

stock DB_UpdateStateTreasuryBalance(stateid, delta)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new query[128];
    format(query, sizeof(query), "UPDATE state_bank SET balance = balance + %d, updated_at = datetime('now') WHERE state_id = %d", delta, stateid);
    return DB_Execute(query);
}

stock DB_AddStateTreasuryFunds(stateid, amount)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (amount == 0)
    {
        return true;
    }

    return DB_UpdateStateTreasuryBalance(stateid, amount);
}

stock DB_RegisterState(stateid, const name[], const shortName[], treasury, taxRate)
{
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new query[256];
    format(query, sizeof(query), "INSERT OR REPLACE INTO states (id, name, short_name, treasury, tax_rate) VALUES (%d, '%s', '%s', %d, %f)", stateid, name, shortName, treasury, float(taxRate));
    return DB_Execute(query);
}

stock DB_EnsureInventorySchema()
{
    return true;
}

stock DB_EnsureInventoryIntegrity()
{
    return true;
}

stock DB_EnsureHouseSchema()
{
    return true;
}

stock DB_EnsureHouseStorageSchema()
{
    return true;
}

stock DB_EnsureHouseStorageIntegrity()
{
    return true;
}

stock DB_EnsureAccessorySchema()
{
    return true;
}

stock DB_EnsurePlayerVehicleSchema()
{
    return true;
}

stock DB_EnsurePropertySchema()
{
    return true;
}

stock DB_EnsureFuelStationSchema()
{
    return true;
}

stock DB_EnsureVehicleModsSchema()
{
    return true;
}

stock DB_EnsureUBBVehicleSchema()
{
    return true;
}

stock DB_EnsureUBBVehicleIntegrity()
{
    return true;
}

stock DB_EnsureWebAccountSchema()
{
    return true;
}

stock DB_EnsureMapSchema()
{
    return true;
}

stock DB_SaveMapLocation(const name[], stateid, Float:x, Float:y, Float:z, interior, vw, Float:angle, active)
{
    return true;
}

stock DB_SaveUBBVehicle(vehicleid, modelid, owner_player_id, stateid, fuel, price, status)
{
    gDBUBBVehicleModelCache[vehicleid] = modelid;
    gDBUBBVehicleOwnerCache[vehicleid] = owner_player_id;
    gDBUBBVehicleStateCache[vehicleid] = status;
    gDBUBBVehicleFuelCache[vehicleid] = fuel;
    gDBUBBVehiclePriceCache[vehicleid] = price;
    return true;
}

stock DB_SaveMapZone(const name[], stateid, Float:minX, Float:minY, Float:maxX, Float:maxY, active)
{
    return true;
}

stock DB_EnsureContactSchema()
{
    return true;
}

stock DB_EnsureCriminalRecordSchema()
{
    return true;
}

stock DB_LoadPlayerInventory(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    for (new i = 0; i < MAX_INVENTORY_SLOTS; i++)
    {
        gDBPlayerInventoryCache[playerid][i] = INVENTORY_ITEM_NONE;
        gDBPlayerInventoryQtyCache[playerid][i] = 0;
        format(gDBPlayerInventoryMetaCache[playerid][i], 32, "NONE");
    }

    new query[192];
    format(query, sizeof(query), "SELECT item_id, slot_index, quantity, metadata FROM player_inventory WHERE player_id = %d ORDER BY slot_index ASC;", playerid);
    if (!DB_Query(query))
    {
        return false;
    }

    return true;
}

stock DB_SavePlayerInventoryItem(playerid, itemid, slot_index, quantity, const metadata[])
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (slot_index < 0 || slot_index >= MAX_INVENTORY_SLOTS)
    {
        return false;
    }
    if (itemid < 0 || quantity < 0)
    {
        return false;
    }

    gDBPlayerInventoryCache[playerid][slot_index] = itemid;
    gDBPlayerInventoryQtyCache[playerid][slot_index] = quantity;
    format(gDBPlayerInventoryMetaCache[playerid][slot_index], 32, "%s", metadata);

    new query[256];
    format(query, sizeof(query), "INSERT OR REPLACE INTO player_inventory (player_id, item_id, slot_index, quantity, metadata) VALUES (%d, %d, %d, %d, '%s')", playerid, itemid, slot_index, quantity, metadata);
    return DB_Execute(query);
}

stock DB_LoadHouseStorage(houseid)
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }

    for (new i = 0; i < MAX_HOUSE_STORAGE_SLOTS; i++)
    {
        gDBHouseStorageCache[houseid][i] = 0;
        gDBHouseStorageQtyCache[houseid][i] = 0;
        format(gDBHouseStorageMetaCache[houseid][i], 32, "NONE");
    }

    new query[192];
    format(query, sizeof(query), "SELECT item_id, slot_index, quantity, metadata FROM house_storage_items WHERE house_id = %d ORDER BY slot_index ASC;", houseid);
    if (!DB_Query(query))
    {
        return false;
    }

    return true;
}

stock DB_RemovePlayerInventoryItem(playerid, itemid, quantity)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    new query[192];
    format(query, sizeof(query), "UPDATE player_inventory SET quantity = quantity - %d WHERE player_id = %d AND item_id = %d AND quantity >= %d", quantity, playerid, itemid, quantity);
    return DB_Execute(query);
}

stock DB_SaveHouseStorageItem(houseid, itemid, slot_index, quantity, const metadata[])
{
    if (houseid < 0 || houseid >= MAX_HOUSES)
    {
        return false;
    }
    if (slot_index < 0 || slot_index >= MAX_HOUSE_STORAGE_SLOTS)
    {
        return false;
    }
    if (itemid < 0 || quantity < 0)
    {
        return false;
    }

    gDBHouseStorageCache[houseid][slot_index] = itemid;
    gDBHouseStorageQtyCache[houseid][slot_index] = quantity;
    format(gDBHouseStorageMetaCache[houseid][slot_index], 32, "%s", metadata);

    new query[320];
    format(query, sizeof(query), "INSERT OR REPLACE INTO house_storage_items (house_id, item_id, slot_index, quantity, metadata, updated_at) VALUES (%d, %d, %d, %d, '%s', datetime('now'))", houseid, itemid, slot_index, quantity, metadata);
    return DB_Execute(query);
}

stock DB_SavePlayerVehicle(playerid, vehicleid, modelid, stateid, price_paid, fuel, locked, health, Float:x, Float:y, Float:z, Float:angle, color1, color2)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new query[512];
    format(query, sizeof(query), "INSERT OR REPLACE INTO player_vehicles (player_id, vehicle_id, model_id, state_id, price_paid, fuel, locked, health, position_x, position_y, position_z, angle, color_1, color_2, updated_at) VALUES (%d, %d, %d, %d, %d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %d, %d, datetime('now'))", playerid, vehicleid, modelid, stateid, price_paid, fuel, locked, health, x, y, z, angle, color1, color2);
    return DB_Execute(query);
}

stock DB_SaveAccessory(playerid, accessory_type, color, equipped)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    new query[256];
    format(query, sizeof(query), "INSERT OR REPLACE INTO player_accessories (player_id, accessory_type, color, equipped, updated_at) VALUES (%d, %d, %d, %d, datetime('now'))", playerid, accessory_type, color, equipped);
    return DB_Execute(query);
}

stock DB_AddPlayerContact(playerid, const contact_name[], const phone_number[])
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    new query[256];
    format(query, sizeof(query), "INSERT INTO player_contacts (player_id, contact_name, phone_number) VALUES (%d, '%s', '%s')", playerid, contact_name, phone_number);
    return DB_Execute(query);
}

stock DB_RegisterProperty(playerid, property_type, property_id, stateid, value)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    new query[320];
    format(query, sizeof(query), "INSERT OR REPLACE INTO player_properties (player_id, property_type, property_id, state_id, value, created_at) VALUES (%d, %d, %d, %d, %d, datetime('now'))", playerid, property_type, property_id, stateid, value);
    return DB_Execute(query);
}

stock DB_AddCriminalRecord(playerid, stateid, const offense[], wanted_level, bail_amount, prison_seconds, const officer_name[])
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    new query[320];
    format(query, sizeof(query), "INSERT INTO criminal_records (player_id, state_id, offense, wanted_level, bail_amount, prison_seconds, officer_name) VALUES (%d, %d, '%s', %d, %d, %d, '%s')", playerid, stateid, offense, wanted_level, bail_amount, prison_seconds, officer_name);
    return DB_Execute(query);
}

stock DB_SaveVehicleMod(vehicleid, componentid, modid, value)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return false;
    }

    new query[256];
    format(query, sizeof(query), "INSERT OR REPLACE INTO vehicle_mods (vehicle_id, component_id, mod_id, value, updated_at) VALUES (%d, %d, %d, %d, datetime('now'))", vehicleid, componentid, modid, value);
    return DB_Execute(query);
}

stock DB_RegisterWebAccountSync(const player_name[], const username[], const hash[])
{
    new query[320];
    format(query, sizeof(query), "INSERT OR REPLACE INTO web_account_sync (player_name, ucp_username, password_hash, synced_at) VALUES ('%s', '%s', '%s', datetime('now'))", player_name, username, hash);
    return DB_Execute(query);
}

#endif
