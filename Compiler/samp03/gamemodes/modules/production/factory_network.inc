#if !defined FACTORY_NETWORK_INC
#define FACTORY_NETWORK_INC

#include <a_samp>

#define MAX_FACTORY_ITEMS 18
#define FACTORY_TYPE_LOGISTICS 1
#define FACTORY_TYPE_CHEMICAL 2
#define FACTORY_TYPE_REFINERY 3

new gFactoryState[MAX_FACTORY_ITEMS];
new gFactoryType[MAX_FACTORY_ITEMS];
new gFactoryProduct[MAX_FACTORY_ITEMS][32];
new gFactoryOutput[MAX_FACTORY_ITEMS];
new gFactoryCount = 0;

stock FactoryNetwork_Initialize()
{
    gFactoryCount = 0;
    for (new i = 0; i < MAX_FACTORY_ITEMS; i++)
    {
        gFactoryState[i] = -1;
        gFactoryType[i] = 0;
        format(gFactoryProduct[i], 32, "");
        gFactoryOutput[i] = 0;
    }

    FactoryNetwork_RegisterFactory(STATE_LOS_SANTOS, FACTORY_TYPE_LOGISTICS, "Transport Goods", 1000);
    FactoryNetwork_RegisterFactory(STATE_SAN_FIERRO, FACTORY_TYPE_CHEMICAL, "Industrial Mix", 1200);
    FactoryNetwork_RegisterFactory(STATE_LAS_VENTURAS, FACTORY_TYPE_REFINERY, "Luxury Fuel", 1700);

    print("[FACTORY_NETWORK] Produktionsnetzwerk initialisiert");
}

stock FactoryNetwork_RegisterFactory(stateid, type, const product[], output)
{
    if (!Core_IsValidState(stateid) || gFactoryCount >= MAX_FACTORY_ITEMS)
    {
        return false;
    }

    new idx = gFactoryCount;
    gFactoryState[idx] = stateid;
    gFactoryType[idx] = type;
    format(gFactoryProduct[idx], 32, "%s", product);
    gFactoryOutput[idx] = output;
    gFactoryCount++;
    return true;
}

stock FactoryNetwork_ShowFactories(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[FACTORY_NETWORK] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new msg[256];
    format(msg, sizeof(msg), "[FACTORY_NETWORK] %s | Fabriken: %d", stateName, gFactoryCount);
    SendClientMessage(playerid, 0x00D9FFFF, msg);

    for (new i = 0; i < gFactoryCount; i++)
    {
        if (gFactoryState[i] == stateid)
        {
            new typeName[24];
            if (gFactoryType[i] == FACTORY_TYPE_LOGISTICS)
                format(typeName, sizeof(typeName), "Logistics");
            else if (gFactoryType[i] == FACTORY_TYPE_CHEMICAL)
                format(typeName, sizeof(typeName), "Chemical");
            else
                format(typeName, sizeof(typeName), "Refinery");

            new line[220];
            format(line, sizeof(line), "- %s | Type: %s | Output: $%d", gFactoryProduct[i], typeName, gFactoryOutput[i]);
            SendClientMessage(playerid, 0xFFFFFFFF, line);
        }
    }
}

#endif
