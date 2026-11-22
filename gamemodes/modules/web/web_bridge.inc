#if !defined WEB_BRIDGE_INC
#define WEB_BRIDGE_INC

#include <a_samp>

#define UCP_AUTH_NONE 0
#define UCP_AUTH_SYNCED 1

new gUcpAuthState[MAX_PLAYERS];
new gUcpTicketCount[MAX_PLAYERS];

stock WebBridge_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gUcpAuthState[i] = UCP_AUTH_NONE;
        gUcpTicketCount[i] = 0;
    }
    Core_MarkModuleReady("web");
    print("[WEB] UCP-/Web-Bridge initialisiert");
}

stock WebBridge_SyncAccount(playerid, const username[], const hash[])
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return false;
    }
    if (strlen(username) <= 0 || strlen(hash) <= 0)
    {
        return false;
    }

    new playerName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, playerName, sizeof(playerName));

    gUcpAuthState[playerid] = UCP_AUTH_SYNCED;
    new query[256];
    format(query, sizeof(query), "INSERT OR REPLACE INTO web_account_sync (player_name, ucp_username, password_hash, synced_at) VALUES ('%s', '%s', '%s', datetime('now'))", playerName, username, hash);
    DB_Execute(query);
    return true;
}

stock WebBridge_ShowTickets(playerid)
{
    SendClientMessage(playerid, 0x00D9FFFF, "[WEB] Ticket- und UCP-Daten können über das Web-Panel abgerufen werden.");
}

#endif
