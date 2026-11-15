#if !defined EMS_HOSPITAL_INC
#define EMS_HOSPITAL_INC

#include <a_samp>

#define HOSPITAL_STATE_NONE 0
#define HOSPITAL_STATE_ACTIVE 1

new gHospitalState[3];
new gHospitalCost[3];
new gHospitalRespawnX[3];
new gHospitalRespawnY[3];
new gHospitalRespawnZ[3];

stock EmsHospital_Initialize()
{
    gHospitalState[STATE_LOS_SANTOS] = HOSPITAL_STATE_ACTIVE;
    gHospitalState[STATE_SAN_FIERRO] = HOSPITAL_STATE_ACTIVE;
    gHospitalState[STATE_LAS_VENTURAS] = HOSPITAL_STATE_ACTIVE;

    gHospitalCost[STATE_LOS_SANTOS] = 3500;
    gHospitalCost[STATE_SAN_FIERRO] = 2900;
    gHospitalCost[STATE_LAS_VENTURAS] = 3100;

    gHospitalRespawnX[STATE_LOS_SANTOS] = 1187; gHospitalRespawnY[STATE_LOS_SANTOS] = -1323; gHospitalRespawnZ[STATE_LOS_SANTOS] = 14;
    gHospitalRespawnX[STATE_SAN_FIERRO] = -2672; gHospitalRespawnY[STATE_SAN_FIERRO] = 410; gHospitalRespawnZ[STATE_SAN_FIERRO] = 7;
    gHospitalRespawnX[STATE_LAS_VENTURAS] = 1421; gHospitalRespawnY[STATE_LAS_VENTURAS] = 1853; gHospitalRespawnZ[STATE_LAS_VENTURAS] = 13;

    print("[EMS] Staatliche Krankenhäuser und Versicherung initialisiert");
}

stock EmsHospital_TreatPlayer(playerid, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (!StateSystem_IsValidState(stateid))
    {
        return false;
    }

    new cost = gHospitalCost[stateid];
    if (GetPlayerMoney(playerid) < cost)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[EMS] Du hast nicht genug Geld für die Behandlung.");
        return false;
    }

    GivePlayerMoney(playerid, -cost);
    StateSystem_ApplyTreasuryDelta(stateid, cost);
    SetPlayerPos(playerid, gHospitalRespawnX[stateid], gHospitalRespawnY[stateid], gHospitalRespawnZ[stateid]);
    SendClientMessage(playerid, 0x00FF00AA, "[EMS] Behandlung abgeschlossen. Du bist im Staatskrankenhaus wieder aufgewacht.");
    return true;
}

stock EmsHospital_RespawnPlayer(playerid)
{
    new stateid = StateSystem_GetPlayerState(playerid);
    if (!StateSystem_IsValidState(stateid))
    {
        stateid = STATE_LOS_SANTOS;
    }
    SetPlayerPos(playerid, gHospitalRespawnX[stateid], gHospitalRespawnY[stateid], gHospitalRespawnZ[stateid]);
    SendClientMessage(playerid, 0x00D9FFFF, "[EMS] Respawn im zuständigen Hospital.");
}

#endif
