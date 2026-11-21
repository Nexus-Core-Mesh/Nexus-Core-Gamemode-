#if !defined VEHICLE_SYSTEM_INC
#define VEHICLE_SYSTEM_INC

#include <a_samp>

#define MAX_VEHICLES 100
#define VEHICLE_STATE_NONE -1
#define VEHICLE_TYPE_CAR 0
#define VEHICLE_TYPE_MOTORCYCLE 1
#define VEHICLE_TYPE_AIR 2
#define VEHICLE_TYPE_BOAT 3

new gVehicleModel[MAX_VEHICLES];
new gVehicleState[MAX_VEHICLES];
new gVehicleOwner[MAX_VEHICLES];
new gVehiclePrice[MAX_VEHICLES];
new gVehicleFuel[MAX_VEHICLES];
new gVehicleType[MAX_VEHICLES];

stock VehicleSystem_Initialize()
{
    for (new i = 0; i < MAX_VEHICLES; i++)
    {
        gVehicleModel[i] = 0;
        gVehicleState[i] = VEHICLE_STATE_NONE;
        gVehicleOwner[i] = -1;
        gVehiclePrice[i] = 0;
        gVehicleFuel[i] = 100;
        gVehicleType[i] = VEHICLE_TYPE_CAR;
    }

    VehicleSystem_CreateVehicle(0, 400, 0, -1, 25000, VEHICLE_TYPE_CAR);
    VehicleSystem_CreateVehicle(1, 541, 1, -1, 26000, VEHICLE_TYPE_CAR);
    VehicleSystem_CreateVehicle(2, 463, 2, -1, 32000, VEHICLE_TYPE_BOAT);
    VehicleSystem_CreateVehicle(3, 522, 0, -1, 40000, VEHICLE_TYPE_AIR);

    print("[VEHICLE] Fahrzeugsystem mit Staatslogik initialisiert");
}

stock VehicleSystem_CreateVehicle(vehicleid, modelid, stateid, ownerid, price, type)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return false;
    }
    new safeState = Core_GetValidStateOrDefault(stateid, STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    gVehicleModel[vehicleid] = modelid;
    gVehicleState[vehicleid] = safeState;
    gVehicleOwner[vehicleid] = ownerid;
    gVehiclePrice[vehicleid] = price;
    gVehicleFuel[vehicleid] = 100;
    gVehicleType[vehicleid] = type;
    return true;
}

stock VehicleSystem_SetState(vehicleid, stateid)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return false;
    }
    new safeState = Core_GetValidStateOrDefault(stateid, STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        return false;
    }

    gVehicleState[vehicleid] = safeState;
    return true;
}

stock VehicleSystem_GetFreeSlot()
{
    for (new i = 0; i < MAX_VEHICLES; i++)
    {
        if (gVehicleModel[i] == 0)
        {
            return i;
        }
    }
    return -1;
}

stock VehicleSystem_SpawnAdminVehicle(modelid, Float:x, Float:y, Float:z, ownerid)
{
    if (modelid <= 0)
    {
        return INVALID_VEHICLE_ID;
    }
    if (!Core_IsValidPlayerId(ownerid))
    {
        return INVALID_VEHICLE_ID;
    }

    new slot = VehicleSystem_GetFreeSlot();
    if (slot < 0)
    {
        return INVALID_VEHICLE_ID;
    }

    new vehicleid = CreateVehicle(modelid, x, y, z, 0.0, -1, -1, 600000);
    if (vehicleid == INVALID_VEHICLE_ID)
    {
        return INVALID_VEHICLE_ID;
    }

    new safeState = Core_GetValidStateOrDefault(Core_GetPlayerState(ownerid), STATE_LOS_SANTOS);
    if (!Core_IsValidState(safeState))
    {
        safeState = STATE_LOS_SANTOS;
    }

    gVehicleModel[slot] = modelid;
    gVehicleState[slot] = safeState;
    gVehicleOwner[slot] = ownerid;
    gVehiclePrice[slot] = 0;
    gVehicleFuel[slot] = 100;
    gVehicleType[slot] = VEHICLE_TYPE_CAR;
    return vehicleid;
}

stock VehicleSystem_SetOwner(vehicleid, ownerid)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return false;
    }

    gVehicleOwner[vehicleid] = ownerid;
    return true;
}

stock int:VehicleSystem_GetOwner(vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return -1;
    }
    return gVehicleOwner[vehicleid];
}

stock int:VehicleSystem_GetState(vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return VEHICLE_STATE_NONE;
    }
    return gVehicleState[vehicleid];
}

stock int:VehicleSystem_GetPrice(vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return 0;
    }
    return gVehiclePrice[vehicleid];
}

stock VehicleSystem_Refuel(vehicleid, amount)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return false;
    }
    if (amount < 0)
    {
        return false;
    }

    gVehicleFuel[vehicleid] += amount;
    if (gVehicleFuel[vehicleid] > 100)
    {
        gVehicleFuel[vehicleid] = 100;
    }
    return true;
}

stock VehicleSystem_ShowInfo(playerid, vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return;
    }

    new msg[256];
    format(msg, sizeof(msg), "[VEHICLE] Fahrzeug %d | Model: %d | Staat: %d | Besitzer: %d | Preis: %d | Tank: %d%%", vehicleid, gVehicleModel[vehicleid], gVehicleState[vehicleid], gVehicleOwner[vehicleid], gVehiclePrice[vehicleid], gVehicleFuel[vehicleid]);
    SendClientMessage(playerid, 0x66CCFFAA, msg);
}

#endif
