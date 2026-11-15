#if !defined ROLEPLAY_COMMAND_SYSTEM_INC
#define ROLEPLAY_COMMAND_SYSTEM_INC

#include <a_samp>

stock RoleplayCommandSystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
    {
        return false;
    }

    new cmd[32], idx;
    idx = sscanf(cmdtext, "s[32]", cmd);
    if (idx == 0)
    {
        return false;
    }

    if (strcmp(cmd, "/me", true) == 0)
    {
        SendClientMessage(playerid, 0x99CCFFFF, "[ME] Du führst eine RP-Aktion aus.");
        return true;
    }

    if (strcmp(cmd, "/do", true) == 0)
    {
        SendClientMessage(playerid, 0xCCFF99FF, "[DO] Du beschreibst die Umgebung im Spiel.");
        return true;
    }

    if (strcmp(cmd, "/pm", true) == 0)
    {
        SendClientMessage(playerid, 0xD9B3FFFF, "[PM] Privatnachricht-Logik bereit.");
        return true;
    }

    if (strcmp(cmd, "/report", true) == 0)
    {
        SendClientMessage(playerid, 0xFF6666FF, "[REPORT] Report verarbeitet. Bitte Details an Support/Moderator senden.");
        return true;
    }

    if (strcmp(cmd, "/gang", true) == 0)
    {
        SendClientMessage(playerid, 0x00FF99AA, "[GANG] Fraktions- und Gangsystem aktiv. Nutze staatliche Fraktions- und Gangwar-Interfaces.");
        return true;
    }

    if (strcmp(cmd, "/factions", true) == 0 || strcmp(cmd, "/factioninfo", true) == 0)
    {
        new stateid = Core_GetPlayerState(playerid);
        new found = 0;
        for (new i = 0; i < MAX_FACTIONS; i++)
        {
            if (gFactionIsActive[i] == 1 && gFactionState[i] == stateid)
            {
                FactionSystem_ShowInfo(playerid, i);
                found = 1;
                break;
            }
        }
        if (!found)
        {
            SendClientMessage(playerid, 0xFF6666FF, "[FACTION] In deinem Staat sind aktuell keine aktiven Fraktionen oder Gangs gelistet.");
        }
        return true;
    }

    if (strcmp(cmd, "/war", true) == 0)
    {
        GangWarSystem_ShowStatus(playerid);
        return true;
    }

    if (strcmp(cmd, "/fight", true) == 0)
    {
        GangFightSystem_ShowStatus(playerid);
        return true;
    }

    if (strcmp(cmd, "/job", true) == 0)
    {
        JobsCore_ShowPlayerJobInfo(playerid);
        return true;
    }

    return false;
}

#endif
