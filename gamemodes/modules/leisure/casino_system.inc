#if !defined CASINO_SYSTEM_INC
#define CASINO_SYSTEM_INC

#include <a_samp>

#define MAX_TABLES 8
new gCasinoTableState[MAX_TABLES];
new gCasinoHouseProfit; 

stock CasinoSystem_Initialize()
{
    for (new i = 0; i < MAX_TABLES; i++)
    {
        gCasinoTableState[i] = 0;
    }
    gCasinoHouseProfit = 0;
    print("[CASINO] Casino-System initialisiert");
}

stock CasinoSystem_OpenTable(tableid)
{
    if (tableid < 0 || tableid >= MAX_TABLES)
    {
        return;
    }
    gCasinoTableState[tableid] = 1;
}

stock CasinoSystem_CloseTable(tableid)
{
    if (tableid < 0 || tableid >= MAX_TABLES)
    {
        return;
    }
    gCasinoTableState[tableid] = 0;
}

stock CasinoSystem_RecordProfit(amount)
{
    gCasinoHouseProfit += amount;
}

#endif
