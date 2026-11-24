#if !defined DB_SQLITE_INC
#define DB_SQLITE_INC

#include <a_samp>
#include <a_sampdb>

// Reines SQLite-Backend. Alle Laufzeit-Lese-/Schreibpfade laufen ueber diese
// einzige Datenbankdatei. Es gibt keinen Dini-/INI-Fallback mehr.
#define DB_SQLITE_FILE "data/db/gtasa_reallife.db"

new DB:gSQLiteDB = DB:0;

stock DB_SQLite_IsReady()
{
    return (gSQLiteDB != DB:0);
}

// Einfacher, deterministischer Passwort-Hash (nur Ziffern -> SQL-sicher).
stock DB_HashPassword(const input[], output[], len)
{
    new hashA = 5381;
    new hashB = 52711;
    for (new i = 0; input[i] != 0; i++)
    {
        hashA = (hashA * 33) ^ input[i];
        hashB = (hashB * 31) + input[i];
    }
    if (hashA < 0) hashA = -hashA;
    if (hashB < 0) hashB = -hashB;
    format(output, len, "%d%d", hashA, hashB);
    return 1;
}

stock DB_SanitizeName(const input[], output[], len)
{
    new idx = 0;
    for (new i = 0; input[i] != 0 && idx < len - 1; i++)
    {
        new c = input[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '_' || c == '[' || c == ']' ||
            c == '(' || c == ')' || c == '$' || c == '.' || c == '=' || c == '@')
        {
            output[idx++] = c;
        }
    }
    output[idx] = EOS;
    if (idx == 0)
    {
        format(output, len, "Player");
    }
    return 1;
}

stock DB_SQLite_EnsureSchema()
{
    if (!DB_SQLite_IsReady())
    {
        return;
    }

    new DBResult:result;

    result = db_query(gSQLiteDB, "CREATE TABLE IF NOT EXISTS players (id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT UNIQUE COLLATE NOCASE,password TEXT NOT NULL DEFAULT '',gender INTEGER NOT NULL DEFAULT 0,skin INTEGER NOT NULL DEFAULT 0,state_id INTEGER NOT NULL DEFAULT 0,home_state INTEGER NOT NULL DEFAULT 0,money INTEGER NOT NULL DEFAULT 2500,bank INTEGER NOT NULL DEFAULT 2500,level INTEGER NOT NULL DEFAULT 0,created_at TEXT);");
    if (result) db_free_result(result);

    result = db_query(gSQLiteDB, "CREATE TABLE IF NOT EXISTS player_inventory (id INTEGER PRIMARY KEY AUTOINCREMENT,player_id INTEGER NOT NULL,item_id INTEGER NOT NULL,slot_index INTEGER NOT NULL,quantity INTEGER NOT NULL DEFAULT 0,metadata TEXT DEFAULT 'NONE',UNIQUE(player_id, slot_index));");
    if (result) db_free_result(result);

    result = db_query(gSQLiteDB, "CREATE TABLE IF NOT EXISTS houses (id INTEGER PRIMARY KEY,owner_player_id INTEGER NOT NULL DEFAULT -1,state_id INTEGER NOT NULL DEFAULT 0,name TEXT DEFAULT '',price INTEGER NOT NULL DEFAULT 0,x REAL DEFAULT 0.0,y REAL DEFAULT 0.0,z REAL DEFAULT 0.0,locked INTEGER NOT NULL DEFAULT 1,created_at TEXT);");
    if (result) db_free_result(result);

    result = db_query(gSQLiteDB, "CREATE TABLE IF NOT EXISTS vehicles (id INTEGER PRIMARY KEY AUTOINCREMENT,player_id INTEGER NOT NULL,model_id INTEGER NOT NULL DEFAULT 0,state_id INTEGER NOT NULL DEFAULT 0,price_paid INTEGER NOT NULL DEFAULT 0,fuel INTEGER NOT NULL DEFAULT 100,locked INTEGER NOT NULL DEFAULT 1,updated_at TEXT);");
    if (result) db_free_result(result);

    result = db_query(gSQLiteDB, "CREATE TABLE IF NOT EXISTS states (id INTEGER PRIMARY KEY,name TEXT DEFAULT '',short_name TEXT DEFAULT '',treasury INTEGER NOT NULL DEFAULT 0,tax_rate REAL NOT NULL DEFAULT 0.0);");
    if (result) db_free_result(result);

    print("[DB_SQLITE] Schema geprueft/erstellt (players, player_inventory, houses, vehicles, states).");
}

stock DB_SQLite_Initialize()
{
    printf("[DB_SQLITE] Initialisierung gestartet");
    gSQLiteDB = db_open(DB_SQLITE_FILE);
    if (!DB_SQLite_IsReady())
    {
        print("[DB_SQLITE] FEHLER: Datenbank konnte nicht geoeffnet werden!");
        return;
    }
    printf("[DB_SQLITE] Datenbank geoeffnet: %s", DB_SQLITE_FILE);
    DB_SQLite_EnsureSchema();
}

stock DB_SQLite_Shutdown()
{
    if (DB_SQLite_IsReady())
    {
        db_close(gSQLiteDB);
        gSQLiteDB = DB:0;
    }
    printf("[DB_SQLITE] Verbindung geschlossen");
}

stock DB_SQLite_Execute(const query[])
{
    if (strlen(query) <= 0 || !DB_SQLite_IsReady())
    {
        return false;
    }

    new mutableQuery[512];
    strcat(mutableQuery, query, sizeof(mutableQuery));

    new DBResult:result = db_query(gSQLiteDB, mutableQuery);
    if (result)
    {
        db_free_result(result);
    }
    return true;
}

stock DB_SQLite_Query(const query[])
{
    return DB_SQLite_Execute(query);
}

// -------- Account-Ebene (namensbasiert, einzige Persistenzquelle) --------

stock DB_SQLite_AccountExists(const name[])
{
    if (!DB_SQLite_IsReady())
    {
        return 0;
    }

    new safeName[MAX_PLAYER_NAME];
    DB_SanitizeName(name, safeName, sizeof(safeName));

    new query[160];
    format(query, sizeof(query), "SELECT id FROM players WHERE name = '%s' COLLATE NOCASE LIMIT 1;", safeName);
    new DBResult:result = db_query(gSQLiteDB, query);
    new exists = 0;
    if (result)
    {
        exists = (db_num_rows(result) > 0) ? 1 : 0;
        db_free_result(result);
    }
    return exists;
}

stock DB_SQLite_VerifyPassword(const name[], const inputPassword[])
{
    if (!DB_SQLite_IsReady())
    {
        return 0;
    }

    new safeName[MAX_PLAYER_NAME];
    DB_SanitizeName(name, safeName, sizeof(safeName));

    new hash[48];
    DB_HashPassword(inputPassword, hash, sizeof(hash));

    new query[192];
    format(query, sizeof(query), "SELECT password FROM players WHERE name = '%s' COLLATE NOCASE LIMIT 1;", safeName);
    new DBResult:result = db_query(gSQLiteDB, query);
    new match = 0;
    if (result)
    {
        if (db_num_rows(result) > 0)
        {
            new stored[48];
            db_get_field_assoc(result, "password", stored, sizeof(stored));
            if (strcmp(stored, hash, false) == 0)
            {
                match = 1;
            }
        }
        db_free_result(result);
    }
    return match;
}

stock DB_SQLite_LoadAccountInto(playerid, const name[])
{
    if (!DB_SQLite_IsReady() || !Core_IsValidPlayerId(playerid))
    {
        return 0;
    }

    new safeName[MAX_PLAYER_NAME];
    DB_SanitizeName(name, safeName, sizeof(safeName));

    new query[192];
    format(query, sizeof(query),
        "SELECT gender, skin, state_id, home_state, money, bank, level FROM players WHERE name = '%s' COLLATE NOCASE LIMIT 1;",
        safeName);

    new DBResult:result = db_query(gSQLiteDB, query);
    new found = 0;
    if (result)
    {
        if (db_num_rows(result) > 0)
        {
            new field[24];

            db_get_field_assoc(result, "gender", field, sizeof(field));
            new gender = strval(field);
            if (gender != GENDER_MALE && gender != GENDER_FEMALE) gender = GENDER_NONE;

            db_get_field_assoc(result, "skin", field, sizeof(field));
            new skin = strval(field);
            if (skin < 0 || skin > 311) skin = 0;

            db_get_field_assoc(result, "state_id", field, sizeof(field));
            new stateid = strval(field);
            stateid = Core_GetValidStateOrDefault(stateid, STATE_LOS_SANTOS);

            db_get_field_assoc(result, "home_state", field, sizeof(field));
            new homeState = strval(field);
            homeState = Core_GetValidStateOrDefault(homeState, stateid);

            db_get_field_assoc(result, "money", field, sizeof(field));
            new money = strval(field);

            db_get_field_assoc(result, "bank", field, sizeof(field));
            new bank = strval(field);

            db_get_field_assoc(result, "level", field, sizeof(field));
            new level = strval(field);

            Core_SetPlayerGender(playerid, gender);
            gPlayerData[playerid][E_PLAYER_GENDER] = gender;
            Core_SetPlayerSkin(playerid, skin);
            gPlayerData[playerid][E_PLAYER_SKIN] = skin;
            Core_SetPlayerState(playerid, stateid);
            Core_SetPlayerHomeState(playerid, homeState);
            gPlayerData[playerid][E_PLAYER_STATE] = stateid;
            gPlayerData[playerid][E_PLAYER_HOME_STATE] = homeState;
            gPlayerData[playerid][E_PLAYER_LEVEL] = level;
            Core_SetPlayerCash(playerid, money);
            Core_SetPlayerBank(playerid, bank);
            gPlayerData[playerid][E_PLAYER_BANK] = bank;

            found = 1;
        }
        db_free_result(result);
    }
    return found;
}

stock DB_SQLite_CreateAccount(playerid, const name[], const inputPassword[])
{
    if (!DB_SQLite_IsReady() || !Core_IsValidPlayerId(playerid))
    {
        return 0;
    }

    new safeName[MAX_PLAYER_NAME];
    DB_SanitizeName(name, safeName, sizeof(safeName));

    new hash[48];
    DB_HashPassword(inputPassword, hash, sizeof(hash));

    new gender = Core_GetPlayerGender(playerid);
    if (gender != GENDER_MALE && gender != GENDER_FEMALE) gender = GENDER_NONE;
    new skin = Core_GetPlayerSkin(playerid);
    if (skin < 0 || skin > 311) skin = 0;
    new stateid = Core_GetValidStateOrDefault(Core_GetPlayerState(playerid), STATE_LOS_SANTOS);
    new homeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), stateid);
    new money = Core_GetPlayerCash(playerid);
    new bank = Core_GetPlayerBank(playerid);

    new query[384];
    format(query, sizeof(query),
        "INSERT OR REPLACE INTO players (id, name, password, gender, skin, state_id, home_state, money, bank, level, created_at) VALUES ((SELECT id FROM players WHERE name = '%s' COLLATE NOCASE), '%s', '%s', %d, %d, %d, %d, %d, %d, %d, datetime('now'));",
        safeName, safeName, hash, gender, skin, stateid, homeState, money, bank, gPlayerData[playerid][E_PLAYER_LEVEL]);

    return DB_SQLite_Execute(query) ? 1 : 0;
}

stock DB_SQLite_SaveAccount(playerid, const name[])
{
    if (!DB_SQLite_IsReady() || !Core_IsValidPlayerId(playerid))
    {
        return 0;
    }

    new safeName[MAX_PLAYER_NAME];
    DB_SanitizeName(name, safeName, sizeof(safeName));

    new gender = Core_GetPlayerGender(playerid);
    if (gender != GENDER_MALE && gender != GENDER_FEMALE) gender = GENDER_NONE;
    new skin = Core_GetPlayerSkin(playerid);
    if (skin < 0 || skin > 311) skin = 0;
    new stateid = Core_GetValidStateOrDefault(Core_GetPlayerState(playerid), STATE_LOS_SANTOS);
    new homeState = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), stateid);
    new money = Core_GetPlayerCash(playerid);
    new bank = Core_GetPlayerBank(playerid);

    new query[320];
    format(query, sizeof(query),
        "UPDATE players SET gender = %d, skin = %d, state_id = %d, home_state = %d, money = %d, bank = %d, level = %d WHERE name = '%s' COLLATE NOCASE;",
        gender, skin, stateid, homeState, money, bank, gPlayerData[playerid][E_PLAYER_LEVEL], safeName);

    return DB_SQLite_Execute(query) ? 1 : 0;
}

#endif
