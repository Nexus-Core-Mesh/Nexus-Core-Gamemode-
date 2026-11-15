#if !defined WEAPON_SHOP_SYSTEM_INC
#define WEAPON_SHOP_SYSTEM_INC

#include <a_samp>

#define MAX_WEAPON_SHOP_ITEMS 12

new gWeaponShopCount = 0;
new gWeaponShopState[MAX_WEAPON_SHOP_ITEMS];
new gWeaponShopWeapon[MAX_WEAPON_SHOP_ITEMS];
new gWeaponShopPrice[MAX_WEAPON_SHOP_ITEMS];
new gWeaponShopName[MAX_WEAPON_SHOP_ITEMS][32];

stock WeaponShopSystem_Initialize()
{
    gWeaponShopCount = 0;

    WeaponShopSystem_RegisterItem(STATE_LOS_SANTOS, WEAPON_COLT45, 500, "Pistole");
    WeaponShopSystem_RegisterItem(STATE_LOS_SANTOS, WEAPON_SHOTGUN, 1500, "Shotgun");
    WeaponShopSystem_RegisterItem(STATE_SAN_FIERRO, WEAPON_MP5, 2200, "MP5");
    WeaponShopSystem_RegisterItem(STATE_SAN_FIERRO, WEAPON_SAWEDOFF, 1700, "Sawed-Off");
    WeaponShopSystem_RegisterItem(STATE_LAS_VENTURAS, WEAPON_DEAGLE, 2500, "Desert Eagle");
    WeaponShopSystem_RegisterItem(STATE_LAS_VENTURAS, WEAPON_RIFLE, 3500, "Rifle");

    print("[WEAPONSHOP] Waffenladen-System initialisiert");
}

stock WeaponShopSystem_RegisterItem(stateid, weaponid, price, const name[])
{
    if (gWeaponShopCount >= MAX_WEAPON_SHOP_ITEMS)
        return false;
    if (!Core_IsValidState(stateid))
        return false;

    gWeaponShopState[gWeaponShopCount] = stateid;
    gWeaponShopWeapon[gWeaponShopCount] = weaponid;
    gWeaponShopPrice[gWeaponShopCount] = price;
    format(gWeaponShopName[gWeaponShopCount], 32, "%s", name);
    gWeaponShopCount++;
    return true;
}

stock WeaponShopSystem_ShowCatalog(playerid, stateid)
{
    new count = 0;
    new msg[256];
    format(msg, sizeof(msg), "[WEAPONSHOP] Waffenmarkt für Staat %d", stateid);
    SendClientMessage(playerid, 0xFFCC00AA, msg);

    for (new i = 0; i < gWeaponShopCount; i++)
    {
        if (gWeaponShopState[i] == stateid)
        {
            new itemMsg[128];
            format(itemMsg, sizeof(itemMsg), "%s | %d$ | Waffe: %d", gWeaponShopName[i], gWeaponShopPrice[i], gWeaponShopWeapon[i]);
            SendClientMessage(playerid, 0xFFFFFFFF, itemMsg);
            count++;
        }
    }

    if (count == 0)
    {
        SendClientMessage(playerid, 0xFF0000AA, "[WEAPONSHOP] Keine Waffen im aktuellen Staat verfügbar.");
    }
}

stock WeaponShopSystem_BuyWeapon(playerid, weaponid, stateid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
        return false;

    for (new i = 0; i < gWeaponShopCount; i++)
    {
        if (gWeaponShopState[i] == stateid && gWeaponShopWeapon[i] == weaponid)
        {
            if (!Core_ChangePlayerBank(playerid, -gWeaponShopPrice[i]))
            {
                SendClientMessage(playerid, 0xFF0000AA, "[WEAPONSHOP] Nicht genug Geld auf der Bank.");
                return false;
            }

            StateSystem_ApplyTreasuryDelta(stateid, gWeaponShopPrice[i] / 5);
            GivePlayerWeapon(playerid, weaponid, 100);
            new buyMsg[96];
            format(buyMsg, sizeof(buyMsg), "[WEAPONSHOP] Kauf erfolgreich. Staatseinnahme: %d$", gWeaponShopPrice[i] / 5);
            SendClientMessage(playerid, 0x00FF00AA, buyMsg);
            return true;
        }
    }

    SendClientMessage(playerid, 0xFF0000AA, "[WEAPONSHOP] Waffe im aktuellen Staat nicht verfügbar.");
    return false;
}

stock WeaponShopSystem_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
        return false;

    new cmd[32], idx;
    idx = sscanf(cmdtext, "s[32]", cmd);
    if (idx == 0)
        return false;

    if (strcmp(cmd, "/weapons", true) == 0)
    {
        WeaponShopSystem_ShowCatalog(playerid, StateSystem_GetPlayerState(playerid));
        return true;
    }

    if (strcmp(cmd, "/buyweapon", true) == 0)
    {
        new weaponid, stateid;
        if (sscanf(cmdtext, "s[32]dd", cmd, weaponid, stateid) != 0)
        {
            stateid = StateSystem_GetPlayerState(playerid);
            if (sscanf(cmdtext, "s[32]d", cmd, weaponid) != 0)
            {
                SendClientMessage(playerid, 0xFFCC66AA, "[WEAPONSHOP] Syntax: /buyweapon <WeaponID> [StateID]");
                return true;
            }
        }
        WeaponShopSystem_BuyWeapon(playerid, weaponid, stateid);
        return true;
    }

    return false;
}

#endif
