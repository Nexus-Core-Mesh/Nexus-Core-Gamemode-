#if !defined PHONE_SYSTEM_INC
#define PHONE_SYSTEM_INC

#include <a_samp>

#define MAX_PHONE_CONTACTS 20
#define MAX_SMS_COUNT 25
#define PHONE_STATE_OFF 0
#define PHONE_STATE_ON 1

new gPhoneState[MAX_PLAYERS];
new gPhoneNumber[MAX_PLAYERS];
new gPhoneContacts[MAX_PLAYERS][MAX_PHONE_CONTACTS][32];
new gPhoneContactCount[MAX_PLAYERS];
new gPhoneMessages[MAX_PLAYERS][MAX_SMS_COUNT][64];
new gPhoneMessageCount[MAX_PLAYERS];

stock PhoneSystem_Initialize()
{
    DB_EnsureContactSchema();
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gPhoneState[i] = PHONE_STATE_OFF;
        gPhoneNumber[i] = 100000 + i;
        gPhoneContactCount[i] = 0;
        gPhoneMessageCount[i] = 0;
    }

    print("[PHONE] Handy-/SIM-System initialisiert");
}

stock PhoneSystem_OnPlayerConnect(playerid)
{
    gPhoneState[playerid] = PHONE_STATE_ON;
    gPhoneNumber[playerid] = 100000 + playerid;
    gPhoneContactCount[playerid] = 0;
    gPhoneMessageCount[playerid] = 0;
    DB_AddPlayerContact(playerid, "Support", "100000");
    SendClientMessage(playerid, 0x00D9FFFF, "[PHONE] SIM aktiv. Deine Nummer: 100000");
}

stock PhoneSystem_AddContact(playerid, const contactName[])
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    if (gPhoneContactCount[playerid] >= MAX_PHONE_CONTACTS)
    {
        return;
    }

    format(gPhoneContacts[playerid][gPhoneContactCount[playerid]], 32, "%s", contactName);
    DB_AddPlayerContact(playerid, contactName, "100000");
    gPhoneContactCount[playerid]++;
}

stock PhoneSystem_SendSMS(playerid, const message[])
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
        return;
    }
    if (gPhoneMessageCount[playerid] >= MAX_SMS_COUNT)
    {
        return;
    }

    format(gPhoneMessages[playerid][gPhoneMessageCount[playerid]], 64, "%s", message);
    gPhoneMessageCount[playerid]++;
    SendClientMessage(playerid, 0x66FF66AA, "[PHONE] SMS gespeichert.");
}

stock PhoneSystem_ShowContacts(playerid)
{
    new msg[256];
    format(msg, sizeof(msg), "[PHONE] Kontakte: %d | Nummer: %d", gPhoneContactCount[playerid], gPhoneNumber[playerid]);
    SendClientMessage(playerid, 0x00D9FFFF, msg);
}

#endif
