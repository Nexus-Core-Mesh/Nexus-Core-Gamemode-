#if !defined DB_MYSQL_INC
#define DB_MYSQL_INC

#include <a_samp>

stock DB_MySQL_Initialize()
{
    printf("[DB_MYSQL] Initialisierung vorbereitet");
    print("[DB_MYSQL] MySQL 4.1-Adapter bereit, aber aktuell nicht aktiv.");
}

stock DB_MySQL_Shutdown()
{
    printf("[DB_MYSQL] Verbindung geschlossen");
}

stock DB_MySQL_Execute(const query[])
{
    printf("[DB_MYSQL] Query: %s", query);
    return true;
}

stock DB_MySQL_Query(const query[])
{
    printf("[DB_MYSQL] Query: %s", query);
    return true;
}

#endif
