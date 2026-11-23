#if !defined MISSION_BOARD_INC
#define MISSION_BOARD_INC

#include <a_samp>

#define MAX_MISSION_BOARD_ITEMS 20

new gMissionBoardState[MAX_MISSION_BOARD_ITEMS];
new gMissionBoardActive[MAX_MISSION_BOARD_ITEMS];
new gMissionBoardReward[MAX_MISSION_BOARD_ITEMS];
new gMissionBoardName[MAX_MISSION_BOARD_ITEMS][48];
new gMissionBoardCount = 0;

stock MissionBoard_Initialize()
{
    gMissionBoardCount = 0;
    for (new i = 0; i < MAX_MISSION_BOARD_ITEMS; i++)
    {
        gMissionBoardState[i] = -1;
        gMissionBoardActive[i] = 0;
        gMissionBoardReward[i] = 0;
        format(gMissionBoardName[i], 48, "");
    }

    MissionBoard_RegisterMission(STATE_LOS_SANTOS, "City Sweep", 1500);
    MissionBoard_RegisterMission(STATE_LOS_SANTOS, "Harbor Security", 2200);
    MissionBoard_RegisterMission(STATE_SAN_FIERRO, "Bridge Patrol", 1800);
    MissionBoard_RegisterMission(STATE_LAS_VENTURAS, "Casino Escort", 2600);

    print("[MISSION_BOARD] Mission Board und Aufgabenliste initialisiert");
}

stock MissionBoard_RegisterMission(stateid, const name[], reward)
{
    if (!Core_IsValidState(stateid) || gMissionBoardCount >= MAX_MISSION_BOARD_ITEMS)
    {
        return false;
    }

    new idx = gMissionBoardCount;
    gMissionBoardState[idx] = stateid;
    gMissionBoardActive[idx] = 1;
    gMissionBoardReward[idx] = reward;
    format(gMissionBoardName[idx], 48, "%s", name);
    gMissionBoardCount++;
    return true;
}

stock MissionBoard_Show(playerid, stateid)
{
    if (!Core_IsValidState(stateid))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[MISSION_BOARD] Ungültiger Staat.");
        return;
    }

    new stateName[32];
    StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    new msg[256];
    format(msg, sizeof(msg), "[MISSION_BOARD] %s | Aufgaben: %d", stateName, gMissionBoardCount);
    SendClientMessage(playerid, 0x00D9FFFF, msg);

    for (new i = 0; i < gMissionBoardCount; i++)
    {
        if (gMissionBoardState[i] == stateid && gMissionBoardActive[i] == 1)
        {
            new line[160];
            format(line, sizeof(line), "- %s | Reward: $%d", gMissionBoardName[i], gMissionBoardReward[i]);
            SendClientMessage(playerid, 0xFFFFFFFF, line);
        }
    }
}

stock MissionBoard_Accept(playerid, missionId)
{
    if (!Core_IsValidPlayerId(playerid) || missionId < 0 || missionId >= gMissionBoardCount)
    {
        return false;
    }

    if (gMissionBoardActive[missionId] == 0)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[MISSION_BOARD] Aufgabe bereits abgeschlossen.");
        return false;
    }

    new stateid = Core_GetValidStateOrDefault(gMissionBoardState[missionId], Core_GetPlayerState(playerid));
    if (!Core_IsValidState(stateid))
    {
        stateid = STATE_LOS_SANTOS;
    }

    new reward = gMissionBoardReward[missionId];
    new tax = reward * StateSystem_GetTaxRate(stateid) / 100;
    new net = reward - tax;

    if (!StateLedgerSystem_Transfer(stateid, max(0, tax), "mission_board_tax", "mission_board"))
    {
        tax = 0;
    }

    if (!Core_ChangePlayerBank(playerid, net))
    {
        SendClientMessage(playerid, 0xFF0000AA, "[MISSION_BOARD] Bankfehler beim Aufgabenabschluss.");
        return false;
    }

    JobsCore_AddPlayerJobXP(playerid, max(25, reward / 250));
    ProgressionSystem_AddXP(playerid, max(15, reward / 300));
    ProgressionSystem_AddRep(playerid, max(1, reward / 10000));

    gMissionBoardActive[missionId] = 0;
    SendClientMessage(playerid, 0x00FF00AA, "[MISSION_BOARD] Aufgabe angenommen und abgearbeitet. +$%d | +%d XP | +%d Rep", net, max(15, reward / 300), max(1, reward / 10000));
    return true;
}

#endif
