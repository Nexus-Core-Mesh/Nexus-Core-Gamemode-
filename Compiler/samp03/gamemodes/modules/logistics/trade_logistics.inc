#if !defined TRADE_LOGISTICS_INC
#define TRADE_LOGISTICS_INC

#include <a_samp>

#define MAX_LOGISTICS_JOBS 12
#define LOGISTICS_ROUTE_CARGO 1
#define LOGISTICS_ROUTE_SMUGGLE 2
#define LOGISTICS_ROUTE_INTERSTATE 3

new gLogisticsState[MAX_LOGISTICS_JOBS];
new gLogisticsType[MAX_LOGISTICS_JOBS];
new gLogisticsFee[MAX_LOGISTICS_JOBS];
new gLogisticsName[MAX_LOGISTICS_JOBS][32];
new gLogisticsCount = 0;

stock TradeLogistics_Initialize()
{
    TradeLogistics_RegisterRoute(0, STATE_LOS_SANTOS, LOGISTICS_ROUTE_CARGO, "LS -> LV Transport", 1500);
    TradeLogistics_RegisterRoute(1, STATE_SAN_FIERRO, LOGISTICS_ROUTE_INTERSTATE, "SF -> LS Handoff", 1800);
    TradeLogistics_RegisterRoute(2, STATE_LAS_VENTURAS, LOGISTICS_ROUTE_SMUGGLE, "LV -> LS Schmuggel", 3000);

    print("[LOGISTICS] Inter-State-Logistik und Schmuggelrouten initialisiert");
}

stock TradeLogistics_RegisterRoute(routeid, stateid, type, const name[], fee)
{
    if (routeid < 0 || routeid >= MAX_LOGISTICS_JOBS)
    {
        return false;
    }
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }

    gLogisticsState[routeid] = stateid;
    gLogisticsType[routeid] = type;
    gLogisticsFee[routeid] = fee;
    format(gLogisticsName[routeid], 32, "%s", name);
    gLogisticsCount++;
    return true;
}

stock int:TradeLogistics_GetFee(routeid)
{
    if (routeid < 0 || routeid >= MAX_LOGISTICS_JOBS)
    {
        return 0;
    }
    return gLogisticsFee[routeid];
}

stock TradeLogistics_ProcessDelivery(playerid, routeid)
{
    if (routeid < 0 || routeid >= MAX_LOGISTICS_JOBS)
    {
        return;
    }

    new fee = gLogisticsFee[routeid];
    new payout = fee + (gLogisticsState[routeid] * 200);
    GivePlayerMoney(playerid, payout);
    StateSystem_ApplyTreasuryDelta(gLogisticsState[routeid], fee / 2);

    new msg[128];
    format(msg, sizeof(msg), "[LOGISTICS] Lieferung abgeschlossen. Gewinn: $%d | Route: %s", payout, gLogisticsName[routeid]);
    SendClientMessage(playerid, 0x00FF00AA, msg);
}

stock TradeLogistics_IsSmugglingRoute(routeid)
{
    return routeid >= 0 && routeid < MAX_LOGISTICS_JOBS && gLogisticsType[routeid] == LOGISTICS_ROUTE_SMUGGLE;
}

#endif
