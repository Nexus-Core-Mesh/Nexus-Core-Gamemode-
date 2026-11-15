#if !defined FACTION_SYSTEM_INC
#define FACTION_SYSTEM_INC

#include <a_samp>

#define MAX_FACTIONS 20
#define MAX_FACTION_MEMBERS 32
#define FACTION_TYPE_NONE 0
#define FACTION_TYPE_GANG 1
#define FACTION_TYPE_LEGAL 2
#define FACTION_TYPE_STATE 3
#define FACTION_TYPE_EMERGENCY 4

new gFactionState[MAX_FACTIONS];
new gFactionOwner[MAX_FACTIONS];
new gFactionType[MAX_FACTIONS];
new gFactionRank[MAX_PLAYERS];
new gFactionBalance[MAX_FACTIONS];
new gFactionMemberCount[MAX_FACTIONS];
new gFactionMemberList[MAX_FACTIONS][MAX_FACTION_MEMBERS];
new gFactionName[MAX_FACTIONS][32];
new gFactionLeader[MAX_FACTIONS];
new gFactionTag[MAX_FACTIONS][8];
new gFactionIsActive[MAX_FACTIONS];
new gFactionRankName[MAX_FACTIONS][32];

stock FactionSystem_Initialize()
{
    for (new i = 0; i < MAX_FACTIONS; i++)
    {
        gFactionState[i] = -1;
        gFactionOwner[i] = -1;
        gFactionLeader[i] = -1;
        gFactionType[i] = FACTION_TYPE_NONE;
        gFactionBalance[i] = 0;
        gFactionMemberCount[i] = 0;
        gFactionIsActive[i] = 0;
        for (new j = 0; j < MAX_FACTION_MEMBERS; j++)
        {
            gFactionMemberList[i][j] = -1;
        }
        format(gFactionName[i], 32, "Unbenannt");
        format(gFactionTag[i], 8, "N/A");
    }

    FactionSystem_CreateFaction(0, 0, FACTION_TYPE_GANG, -1, "Vatos Locos");
    FactionSystem_CreateFaction(1, 1, FACTION_TYPE_GANG, -1, "San Fierro Syndicate");
    FactionSystem_CreateFaction(2, 2, FACTION_TYPE_LEGAL, -1, "Las Venturas Union");
    FactionSystem_CreateFaction(3, 0, FACTION_TYPE_STATE, -1, "Los Santos PD");
    FactionSystem_CreateFaction(4, 1, FACTION_TYPE_STATE, -1, "San Fierro FD");
    FactionSystem_CreateFaction(5, 2, FACTION_TYPE_EMERGENCY, -1, "LV EMS");

    format(gFactionTag[0], 8, "VLC");
    format(gFactionTag[1], 8, "SFS");
    format(gFactionTag[2], 8, "LVU");
    format(gFactionTag[3], 8, "LSPD");
    format(gFactionTag[4], 8, "SFFD");
    format(gFactionTag[5], 8, "LVEMS");

    gFactionIsActive[0] = 1;
    gFactionIsActive[1] = 1;
    gFactionIsActive[2] = 1;
    gFactionIsActive[3] = 1;
    gFactionIsActive[4] = 1;
    gFactionIsActive[5] = 1;

    FactionSystem_TransferToBalance(0, 15000);
    FactionSystem_TransferToBalance(1, 12000);
    FactionSystem_TransferToBalance(2, 18000);

    print("[FACTION] Fraktions- und Gangsystem initialisiert");
}

stock FactionSystem_CreateFaction(factionid, stateid, type, ownerid, const name[])
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }

    gFactionState[factionid] = stateid;
    gFactionOwner[factionid] = ownerid;
    gFactionLeader[factionid] = ownerid;
    gFactionType[factionid] = type;
    gFactionIsActive[factionid] = 1;
    format(gFactionName[factionid], 32, "%s", name);
    format(gFactionTag[factionid], 8, "FRCT");
    format(gFactionRankName[factionid], 32, "Mitglied");
    gFactionBalance[factionid] = 25000;
    gFactionMemberCount[factionid] = 0;
    return true;
}

stock FactionSystem_JoinFaction(playerid, factionid, rank)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return false;
    }
    if (gFactionIsActive[factionid] != 1)
    {
        return false;
    }
    if (!Core_IsValidState(gFactionState[factionid]))
    {
        return false;
    }
    if (gFactionMemberCount[factionid] >= MAX_FACTION_MEMBERS)
    {
        return false;
    }

    for (new i = 0; i < MAX_FACTION_MEMBERS; i++)
    {
        if (gFactionMemberList[factionid][i] == playerid)
        {
            return false;
        }
    }

    gFactionMemberList[factionid][gFactionMemberCount[factionid]] = playerid;
    gFactionMemberCount[factionid]++;
    gFactionRank[playerid] = rank;

    if (rank >= 5)
    {
        gFactionLeader[factionid] = playerid;
    }

    if (Core_IsValidState(Core_GetPlayerState(playerid)) && Core_GetPlayerState(playerid) != gFactionState[factionid])
    {
        SendClientMessage(playerid, 0xFFCC00AA, "[FACTION] Hinweis: Du bist in einem anderen Staat aktiv.");
    }

    new factionName[32];
    format(factionName, sizeof(factionName), "%s", gFactionName[factionid]);
    printf("[FACTION] %d tritt %s bei (Rang: %d)", playerid, factionName, rank);
    SendClientMessage(playerid, 0x00FF00AA, "[FACTION] Fraktion/Clan beigetreten.");
    return true;
}

stock FactionSystem_SetRankName(factionid, const rankName[])
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return;
    }
    format(gFactionRankName[factionid], 32, "%s", rankName);
}

stock int:FactionSystem_GetMemberState(playerid)
{
    for (new i = 0; i < MAX_FACTIONS; i++)
    {
        if (FactionSystem_IsPlayerMember(playerid, i))
        {
            return gFactionState[i];
        }
    }
    return -1;
}

stock FactionSystem_IsPlayerMember(playerid, factionid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return false;
    }

    for (new i = 0; i < MAX_FACTION_MEMBERS; i++)
    {
        if (gFactionMemberList[factionid][i] == playerid)
        {
            return true;
        }
    }
    return false;
}

stock FactionSystem_SetLeader(factionid, playerid)
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return false;
    }
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }

    gFactionLeader[factionid] = playerid;
    gFactionOwner[factionid] = playerid;
    gFactionRank[playerid] = 5;
    return true;
}

stock FactionSystem_RemoveMember(playerid, factionid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return false;
    }

    for (new i = 0; i < MAX_FACTION_MEMBERS; i++)
    {
        if (gFactionMemberList[factionid][i] == playerid)
        {
            gFactionMemberList[factionid][i] = -1;
            gFactionMemberCount[factionid] = max(0, gFactionMemberCount[factionid] - 1);
            if (gFactionRank[playerid] > 0)
            {
                gFactionRank[playerid] = 0;
            }
            return true;
        }
    }
    return false;
}

stock int:FactionSystem_GetState(factionid)
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return -1;
    }
    return gFactionState[factionid];
}

stock int:FactionSystem_GetRank(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return 0;
    }
    return gFactionRank[playerid];
}

stock int:FactionSystem_GetBalance(factionid)
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return 0;
    }
    return gFactionBalance[factionid];
}

stock FactionSystem_TransferToBalance(factionid, amount)
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return false;
    }
    if (amount < 0)
    {
        return false;
    }

    gFactionBalance[factionid] += amount;
    if (gFactionState[factionid] >= 0)
    {
        new treasuryShare = amount / 10;
        if (treasuryShare > 0)
        {
            StateSystem_ApplyTreasuryDelta(gFactionState[factionid], treasuryShare);
        }
    }
    return true;
}

stock FactionSystem_DeductFromBalance(factionid, amount)
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return false;
    }
    if (amount < 0)
    {
        return false;
    }

    if (gFactionBalance[factionid] < amount)
    {
        return false;
    }

    gFactionBalance[factionid] -= amount;
    return true;
}

stock int:FactionSystem_GetMemberCount(factionid)
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return 0;
    }
    return gFactionMemberCount[factionid];
}

stock int:FactionSystem_GetLeader(factionid)
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return -1;
    }
    return gFactionLeader[factionid];
}

stock FactionSystem_ShowInfo(playerid, factionid)
{
    if (factionid < 0 || factionid >= MAX_FACTIONS)
    {
        return;
    }

    new msg[256];
    format(msg, sizeof(msg), "[FACTION] %s [%s] | Staat: %d | Typ: %d | Kasse: %d | Mitglieder: %d | Leiter: %d", gFactionName[factionid], gFactionTag[factionid], gFactionState[factionid], gFactionType[factionid], gFactionBalance[factionid], gFactionMemberCount[factionid], gFactionLeader[factionid]);
    SendClientMessage(playerid, 0xFF66CCAA, msg);
}

#endif
