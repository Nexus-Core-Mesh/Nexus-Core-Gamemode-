#if !defined UBB_VEHICLE_SYSTEM_INC
#define UBB_VEHICLE_SYSTEM_INC

#include <a_samp>

#if !defined MAX_UBB_VEHICLES
#define MAX_UBB_VEHICLES 100
#endif
#define UBB_VEHICLE_STATUS_FREE 0
#define UBB_VEHICLE_STATUS_LOCKED 1
#define UBB_VEHICLE_STATUS_IN_USE 2

new gUbbVehicleModel[MAX_UBB_VEHICLES];
new gUbbVehicleOwner[MAX_UBB_VEHICLES];
new gUbbVehicleState[MAX_UBB_VEHICLES];
new gUbbVehicleFuel[MAX_UBB_VEHICLES];
new gUbbVehiclePrice[MAX_UBB_VEHICLES];
new gUbbVehicleCount = 0;

stock UBBVehicleSystem_Initialize()
{
     DB_EnsureUBBVehicleSchema();
    for (new i = 0; i < MAX_UBB_VEHICLES; i++)
    {
        gUbbVehicleModel[i] = 0;
        gUbbVehicleOwner[i] = -1;
        gUbbVehicleState[i] = UBB_VEHICLE_STATUS_FREE;
        gUbbVehicleFuel[i] = 100;
        gUbbVehiclePrice[i] = 0;
    }

    print("[UBB_VEHICLE] Fahrzeug- und Besitzsystem für UBB/Portal-Integration initialisiert");
}

stock UBBVehicleSystem_RegisterVehicle(vehicleid, modelid, ownerid, stateid, price)
{
    if (!Core_ValidateVehicleIndex(vehicleid))
    {
        return false;
    }
    if (modelid <= 0)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    if (gUbbVehicleModel[vehicleid] != 0)
    {
        return false;
    }
    if (price < 0)
    {
        return false;
    }

    gUbbVehicleModel[vehicleid] = modelid;
    gUbbVehicleOwner[vehicleid] = ownerid;
    gUbbVehicleState[vehicleid] = UBB_VEHICLE_STATUS_LOCKED;
    gUbbVehicleFuel[vehicleid] = 100;
    gUbbVehiclePrice[vehicleid] = price;
    DB_SaveUBBVehicle(vehicleid, modelid, ownerid, stateid, 100, price, UBB_VEHICLE_STATUS_LOCKED);
    gUbbVehicleCount++;
    return true;
}

stock UBBVehicleSystem_SetVehicleState(vehicleid, newState)
{
    if (!Core_ValidateVehicleIndex(vehicleid))
    {
        return false;
    }
    if (gUbbVehicleModel[vehicleid] == 0)
    {
        return false;
    }
    if (newState != UBB_VEHICLE_STATUS_FREE && newState != UBB_VEHICLE_STATUS_LOCKED && newState != UBB_VEHICLE_STATUS_IN_USE)
    {
        return false;
    }

    gUbbVehicleState[vehicleid] = newState;
    return true;
}

stock UBBVehicleSystem_LockVehicle(vehicleid)
{
    if (!Core_ValidateVehicleIndex(vehicleid))
    {
        return false;
    }
    if (gUbbVehicleModel[vehicleid] == 0)
    {
        return false;
    }

    gUbbVehicleState[vehicleid] = UBB_VEHICLE_STATUS_LOCKED;
    if (gUbbVehicleOwner[vehicleid] >= 0 && gUbbVehicleOwner[vehicleid] < MAX_PLAYERS)
    {
        DB_SaveUBBVehicle(vehicleid, gUbbVehicleModel[vehicleid], gUbbVehicleOwner[vehicleid], StateSystem_GetPlayerState(gUbbVehicleOwner[vehicleid]), gUbbVehicleFuel[vehicleid], gUbbVehiclePrice[vehicleid], UBB_VEHICLE_STATUS_LOCKED);
    }
    return true;
}

stock UBBVehicleSystem_UnlockVehicle(vehicleid)
{
    if (!Core_ValidateVehicleIndex(vehicleid))
    {
        return false;
    }
    if (gUbbVehicleModel[vehicleid] == 0)
    {
        return false;
    }

    gUbbVehicleState[vehicleid] = UBB_VEHICLE_STATUS_FREE;
    if (gUbbVehicleOwner[vehicleid] >= 0 && gUbbVehicleOwner[vehicleid] < MAX_PLAYERS)
    {
        DB_SaveUBBVehicle(vehicleid, gUbbVehicleModel[vehicleid], gUbbVehicleOwner[vehicleid], StateSystem_GetPlayerState(gUbbVehicleOwner[vehicleid]), gUbbVehicleFuel[vehicleid], gUbbVehiclePrice[vehicleid], UBB_VEHICLE_STATUS_FREE);
    }
    return true;
}

stock int:UBBVehicleSystem_GetOwner(vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_UBB_VEHICLES)
    {
        return -1;
    }
    return gUbbVehicleOwner[vehicleid];
}

stock UBBVehicleSystem_SetOwner(vehicleid, ownerid)
{
    if (vehicleid < 0 || vehicleid >= MAX_UBB_VEHICLES)
    {
        return false;
    }
    gUbbVehicleOwner[vehicleid] = ownerid;
    return true;
}

stock UBBVehicleSystem_Refuel(vehicleid, amount)
{
    if (vehicleid < 0 || vehicleid >= MAX_UBB_VEHICLES)
    {
        return false;
    }
    if (amount <= 0)
    {
        return false;
    }

    gUbbVehicleFuel[vehicleid] += amount;
    if (gUbbVehicleFuel[vehicleid] > 100)
    {
        gUbbVehicleFuel[vehicleid] = 100;
    }
    return true;
}

stock int:UBBVehicleSystem_GetFuel(vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_UBB_VEHICLES)
    {
        return 0;
    }
    return gUbbVehicleFuel[vehicleid];
}

stock UBBVehicleSystem_OnPlayerConnect(playerid)
{
    if (!Core_ValidatePlayerSlot(playerid))
    {
        return false;
    }

    for (new i = 0; i < MAX_UBB_VEHICLES; i++)
    {
        if (gUbbVehicleOwner[i] == playerid)
        {
            DB_SaveUBBVehicle(i, gUbbVehicleModel[i], playerid, StateSystem_GetPlayerState(playerid), gUbbVehicleFuel[i], gUbbVehiclePrice[i], gUbbVehicleState[i]);
        }
    }

    return true;
}

#endif
