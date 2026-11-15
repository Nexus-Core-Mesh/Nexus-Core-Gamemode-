#if !defined TUNING_SYSTEM_INC
#define TUNING_SYSTEM_INC

#include <a_samp>

#define MAX_VEHICLE_MODS 16
#define VEHICLE_MOD_NONE 0
#define VEHICLE_MOD_SPOILER 1
#define VEHICLE_MOD_WHEELS 2
#define VEHICLE_MOD_PAINT 3
#define VEHICLE_MOD_HYDRAULICS 4

new gVehicleMods[MAX_VEHICLES][MAX_VEHICLE_MODS];
new gVehiclePaint[MAX_VEHICLES];

stock TuningSystem_Initialize()
{
    for (new i = 0; i < MAX_VEHICLES; i++)
    {
        gVehiclePaint[i] = 0;
        for (new m = 0; m < MAX_VEHICLE_MODS; m++)
        {
            gVehicleMods[i][m] = VEHICLE_MOD_NONE;
        }
    }
    print("[TUNING] Fahrzeug-Tuning und Modifikationen initialisiert");
}

stock TuningSystem_SaveVehicleMod(vehicleid, componentid, modid, value)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return false;
    }
    if (componentid < 0 || componentid >= MAX_VEHICLE_MODS)
    {
        return false;
    }

    gVehicleMods[vehicleid][componentid] = modid;
    gVehiclePaint[vehicleid] = value;
    DB_SaveVehicleMod(vehicleid, componentid, modid, value);
    return true;
}

stock TuningSystem_ApplyVehicleMods(vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return false;
    }

    for (new i = 0; i < MAX_VEHICLE_MODS; i++)
    {
        if (gVehicleMods[vehicleid][i] != VEHICLE_MOD_NONE)
        {
            AddVehicleComponent(vehicleid, gVehicleMods[vehicleid][i]);
        }
    }

    if (gVehiclePaint[vehicleid] != 0)
    {
        ChangeVehicleColor(vehicleid, gVehiclePaint[vehicleid], gVehiclePaint[vehicleid]);
    }
    return true;
}

stock TuningSystem_AddMod(playerid, vehicleid, componentid, modid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return false;
    }

    gVehicleMods[vehicleid][componentid] = modid;
    AddVehicleComponent(vehicleid, modid);
    DB_SaveVehicleMod(vehicleid, componentid, modid, 1);
    SendClientMessage(playerid, 0x00FF00AA, "[TUNING] Modifikation auf das Fahrzeug angewendet.");
    return true;
}

stock TuningSystem_ShowInfo(playerid, vehicleid)
{
    if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
    {
        return;
    }

    new msg[256];
    format(msg, sizeof(msg), "[TUNING] Fahrzeug %d | Paint: %d | Mods: %d", vehicleid, gVehiclePaint[vehicleid], gVehicleMods[vehicleid][0]);
    SendClientMessage(playerid, 0xFFCC66AA, msg);
}

#endif
