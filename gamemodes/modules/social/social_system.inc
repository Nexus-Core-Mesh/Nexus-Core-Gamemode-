#if !defined SOCIAL_SYSTEM_INC
#define SOCIAL_SYSTEM_INC

#include <a_samp>

stock SocialSystem_SendMe(playerid, const text[])
{
    new msg[128];
    format(msg, sizeof(msg), "* %s %s", GetPlayerName(playerid), text);
    SendClientMessageToAll(0xC0C0C0AA, msg);
}

stock SocialSystem_SendDo(playerid, const text[])
{
    new msg[128];
    format(msg, sizeof(msg), "* %s", text);
    SendClientMessageToAll(0xB0E0E6AA, msg);
}

stock SocialSystem_SendReport(playerid, const text[])
{
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));

    new msg[256];
    format(msg, sizeof(msg), "[REPORT] %s: %s", name, text);
    SendClientMessageToAll(0xFFB000AA, msg);
}

stock SocialSystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
        return false;

    new cmd[32], idx;
    idx = sscanf(cmdtext, "s[32]", cmd);
    if (idx == 0)
        return false;

    if (strcmp(cmd, "/me", true) == 0)
    {
        new action[64];
        if (sscanf(cmdtext, "s[32]s[64]", cmd, action) != 0)
        {
            SendClientMessage(playerid, 0xFFCC66AA, "[SOCIAL] Syntax: /me <Aktivität>");
            return true;
        }
        SocialSystem_SendMe(playerid, action);
        return true;
    }

    if (strcmp(cmd, "/do", true) == 0)
    {
        new action[64];
        if (sscanf(cmdtext, "s[32]s[64]", cmd, action) != 0)
        {
            SendClientMessage(playerid, 0xFFCC66AA, "[SOCIAL] Syntax: /do <Beschreibung>");
            return true;
        }
        SocialSystem_SendDo(playerid, action);
        return true;
    }

    if (strcmp(cmd, "/report", true) == 0)
    {
        new text[64];
        if (sscanf(cmdtext, "s[32]s[64]", cmd, text) != 0)
        {
            SendClientMessage(playerid, 0xFFCC66AA, "[SOCIAL] Syntax: /report <Grund>");
            return true;
        }
        SocialSystem_SendReport(playerid, text);
        return true;
    }

    if (strcmp(cmd, "/help", true) == 0)
    {
        SendClientMessage(playerid, 0x33CCFFAA, "[HELP] /menu /hud /jobs /garage /vehicle /money /state /wanted /crime /dispatch /acceptcall /me /do /report");
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowMain(playerid);
        }
        return true;
    }

    return false;
}

#endif
