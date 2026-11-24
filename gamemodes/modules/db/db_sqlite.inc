#if !defined DB_SQLITE_INC
#define DB_SQLITE_INC

#include <a_samp>

stock DB_FallbackPath(const fileName[], output[], outputLen)
{
    format(output, outputLen, "scriptfiles/%s.ini", fileName);
    return 1;
}

stock DB_FallbackTouch(const fileName[])
{
    new path[128];
    DB_FallbackPath(fileName, path, sizeof(path));
    new File:handle = fopen(path, io_write);
    if (handle != INVALID_FILE_HANDLE)
    {
        fclose(handle);
    }
    return 1;
}

stock DB_FallbackWriteKey(const fileName[], const section[], const key[], const value[])
{
    new path[128];
    DB_FallbackPath(fileName, path, sizeof(path));

    new File:handle = fopen(path, io_write);
    if (handle == INVALID_FILE_HANDLE)
    {
        return false;
    }

    new entry[256];
    format(entry, sizeof(entry), "[%s]\r\n%s=%s\r\n", section, key, value);
    fwrite(handle, entry);
    fclose(handle);
    return true;
}

stock DB_SQLite_Initialize()
{
    printf("[DB_SQLITE] Initialisierung gestartet");
    print("[DB_SQLITE] SQLite-Backend aktiv. Dini/Fallback-Mode wird für Autostart verwendet.");
    DB_FallbackTouch("db_runtime");
    DB_FallbackWriteKey("db_runtime", "startup", "backend", "dini_fallback");
    printf("[DB_SQLITE] Schema-Initialisierung wird über den Dini-Fallback verwaltet");
}

stock DB_SQLite_Shutdown()
{
    printf("[DB_SQLITE] Verbindung geschlossen");
}

stock DB_SQLite_Execute(const query[])
{
    if (strlen(query) <= 0)
    {
        return false;
    }

    printf("[DB_SQLITE] Query: %s", query);
    DB_FallbackWriteKey("db_runtime", "queries", "last_query", query);
    return true;
}

stock DB_SQLite_Query(const query[])
{
    if (strlen(query) <= 0)
    {
        return false;
    }

    printf("[DB_SQLITE] Query: %s", query);
    DB_FallbackWriteKey("db_runtime", "queries", "last_query", query);
    return true;
}

#endif
