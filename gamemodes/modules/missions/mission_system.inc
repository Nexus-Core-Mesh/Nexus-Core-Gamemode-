#if !defined MISSION_SYSTEM_INC
#define MISSION_SYSTEM_INC

#include <a_samp>

#define MAX_MISSIONS 12
#define MISSION_TYPE_DELIVERY 1
#define MISSION_TYPE_ESCORT 2
#define MISSION_TYPE_HIJACK 3
#define MISSION_TYPE_RESCUE 4
#define MISSION_TYPE_GANG 5

new gMissionState[MAX_MISSIONS];
new gMissionType[MAX_MISSIONS];
new gMissionReward[MAX_MISSIONS];
new gMissionActive[MAX_MISSIONS];
new gMissionName[MAX_MISSIONS][24];
new gPlayerMission[MAX_PLAYERS];
new Text:gMissionTD[MAX_PLAYERS][8];

stock MissionSystem_Initialize()
{
    for (new i = 0; i < MAX_MISSIONS; i++)
    {
        gMissionState[i] = -1;
        gMissionType[i] = 0;
        gMissionReward[i] = 0;
        gMissionActive[i] = 0;
        format(gMissionName[i], 24, "Unbekannt");
    }

    MissionSystem_RegisterMission(0, STATE_LOS_SANTOS, MISSION_TYPE_DELIVERY, 2500, "Downtown Drop");
    MissionSystem_RegisterMission(1, STATE_LOS_SANTOS, MISSION_TYPE_ESCORT, 3800, "Highway Escort");
    MissionSystem_RegisterMission(2, STATE_SAN_FIERRO, MISSION_TYPE_RESCUE, 4200, "Harbor Rescue");
    MissionSystem_RegisterMission(3, STATE_LAS_VENTURAS, MISSION_TYPE_GANG, 5500, "Casino Run");

    print("[MISSION] Missions- und Delivery-System initialisiert");
}

stock MissionSystem_RegisterMission(missionid, stateid, type, reward, const name[])
{
    if (missionid < 0 || missionid >= MAX_MISSIONS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    gMissionState[missionid] = stateid;
    gMissionType[missionid] = type;
    gMissionReward[missionid] = reward;
    gMissionActive[missionid] = 1;
    format(gMissionName[missionid], 24, "%s", name);
    return true;
}

stock MissionSystem_AcceptMission(playerid, missionid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (missionid < 0 || missionid >= MAX_MISSIONS)
    {
        return false;
    }
    if (gMissionActive[missionid] != 1)
    {
        return false;
    }

    gPlayerMission[playerid] = missionid;
    SendClientMessage(playerid, 0x00FF00AA, "[MISSION] Mission angenommen.");
    return true;
}

stock MissionSystem_CompleteMission(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    if (gPlayerMission[playerid] < 0 || gPlayerMission[playerid] >= MAX_MISSIONS)
    {
        return;
    }

    new reward = gMissionReward[gPlayerMission[playerid]];
    printf("[MISSION] Spieler %d hat Mission %d abgeschlossen und %d$ erhalten.", playerid, gPlayerMission[playerid], reward);
    gPlayerMission[playerid] = -1;
    SendClientMessage(playerid, 0x00FF00AA, "[MISSION] Mission abgeschlossen.");
}

stock MissionSystem_ShowMissionList(playerid)
{
    MissionSystem_HideMissionList(playerid);

    gMissionTD[playerid][0] = TextDrawCreate(320.0, 30.0, "MISSION BOARD");
    TextDrawAlignment(gMissionTD[playerid][0], 2);
    TextDrawLetterSize(gMissionTD[playerid][0], 0.55, 1.5);
    TextDrawColor(gMissionTD[playerid][0], 0x33CCFFAA);

    new lineY = 60;
    for (new i = 0; i < MAX_MISSIONS && i < 6; i++)
    {
        if (gMissionActive[i] == 1)
        {
            new missionText[64];
            format(missionText, sizeof(missionText), "%s [%d$]", gMissionName[i], gMissionReward[i]);
            gMissionTD[playerid][1 + i] = TextDrawCreate(320.0, float(lineY), missionText);
            TextDrawAlignment(gMissionTD[playerid][1 + i], 2);
            TextDrawLetterSize(gMissionTD[playerid][1 + i], 0.22, 0.9);
            TextDrawColor(gMissionTD[playerid][1 + i], 0xFFFFFFFF);
            lineY += 18;
        }
    }

    for (new i = 0; i < 8; i++)
    {
        if (gMissionTD[playerid][i] != Text:0)
        {
            TextDrawShowForPlayer(playerid, gMissionTD[playerid][i]);
        }
    }
}

stock MissionSystem_HideMissionList(playerid)
{
    for (new i = 0; i < 8; i++)
    {
        if (gMissionTD[playerid][i] != Text:0)
        {
            TextDrawHideForPlayer(playerid, gMissionTD[playerid][i]);
            TextDrawDestroy(gMissionTD[playerid][i]);
            gMissionTD[playerid][i] = Text:0;
        }
    }
}

#endif
