#if !defined RELATIONSHIP_SYSTEM_INC
#define RELATIONSHIP_SYSTEM_INC

#include <a_samp>

#define MAX_RELATIONSHIP_LABEL 32

new gPlayerRelationshipName[MAX_PLAYERS][MAX_RELATIONSHIP_LABEL];
new gPlayerRelationshipScore[MAX_PLAYERS];
new gPlayerRelationshipTarget[MAX_PLAYERS][MAX_RELATIONSHIP_LABEL];

stock RelationshipSystem_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        format(gPlayerRelationshipName[i], MAX_RELATIONSHIP_LABEL, "");
        format(gPlayerRelationshipTarget[i], MAX_RELATIONSHIP_LABEL, "");
        gPlayerRelationshipScore[i] = 0;
    }

    print("[RELATIONSHIP] Social relationship system initialized");
}

stock RelationshipSystem_Connect(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    format(gPlayerRelationshipName[playerid], MAX_RELATIONSHIP_LABEL, "Neutral");
    format(gPlayerRelationshipTarget[playerid], MAX_RELATIONSHIP_LABEL, "None");
    gPlayerRelationshipScore[playerid] = 0;
}

stock RelationshipSystem_SetState(playerid, const label[], const target[], score)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    format(gPlayerRelationshipName[playerid], MAX_RELATIONSHIP_LABEL, "%s", label);
    format(gPlayerRelationshipTarget[playerid], MAX_RELATIONSHIP_LABEL, "%s", target);
    gPlayerRelationshipScore[playerid] = score;
}

stock RelationshipSystem_ShowStatus(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }

    new message[256];
    format(message, sizeof(message), "[RELATIONSHIP] Status: %s | Target: %s | Score: %d", gPlayerRelationshipName[playerid], gPlayerRelationshipTarget[playerid], gPlayerRelationshipScore[playerid]);
    SendClientMessage(playerid, 0xCC99FFFF, message);
}

#endif
