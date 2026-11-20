#if !defined PREMIUM_SYSTEM_INC
#define PREMIUM_SYSTEM_INC

#include <a_samp>
#include <sscanf2>

#if !defined CORE_INC
#include "../core/core.inc"
#endif

#if !defined PREMIUM_TIER_NONE
#define PREMIUM_TIER_NONE 0
#endif
#if !defined PREMIUM_TIER_30_DAYS
#define PREMIUM_TIER_30_DAYS 1
#endif
#if !defined PREMIUM_TIER_90_DAYS
#define PREMIUM_TIER_90_DAYS 2
#endif
#if !defined PREMIUM_TIER_300_DAYS
#define PREMIUM_TIER_300_DAYS 3
#endif
#if !defined PREMIUM_TIER_PERMANENT
#define PREMIUM_TIER_PERMANENT 4
#endif

#define PREMIUM_FILE_PREFIX "premium"
#define PREMIUM_SAVE_KEY "profile"

new gPremiumSystemEnabled = 1;

stock PremiumSystem_IsEnabled()
{
    return gPremiumSystemEnabled == 1;
}

stock PremiumSystem_SetEnabled(bool:enabled)
{
    gPremiumSystemEnabled = enabled ? 1 : 0;
}

stock bool:PremiumSystem_IsTierValid(tier)
{
    return tier >= PREMIUM_TIER_NONE && tier <= PREMIUM_TIER_PERMANENT;
}

stock PremiumSystem_GetTierLabel(tier, output[], len)
{
    if (!PremiumSystem_IsTierValid(tier))
    {
        format(output, len, "NONE");
        return;
    }

    if (tier == PREMIUM_TIER_30_DAYS)
    {
        format(output, len, "30 Tage");
    }
    else if (tier == PREMIUM_TIER_90_DAYS)
    {
        format(output, len, "90 Tage");
    }
    else if (tier == PREMIUM_TIER_300_DAYS)
    {
        format(output, len, "300 Tage");
    }
    else if (tier == PREMIUM_TIER_PERMANENT)
    {
        format(output, len, "Permanent");
    }
    else
    {
        format(output, len, "NONE");
    }
}

stock PremiumSystem_GetTierDays(tier)
{
    if (!PremiumSystem_IsTierValid(tier))
    {
        return 0;
    }

    if (tier == PREMIUM_TIER_30_DAYS)
    {
        return 30;
    }
    if (tier == PREMIUM_TIER_90_DAYS)
    {
        return 90;
    }
    if (tier == PREMIUM_TIER_300_DAYS)
    {
        return 300;
    }
    if (tier == PREMIUM_TIER_PERMANENT)
    {
        return -1;
    }
    return 0;
}

stock PremiumSystem_WriteProfileSafe(playerid, const profileData[])
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new fileName[128];
    format(fileName, sizeof(fileName), "premium/player_%d", playerid);

    new tempPath[128];
    format(tempPath, sizeof(tempPath), "scriptfiles/%s.tmp", fileName);

    new File:tempHandle = fopen(tempPath, io_write);
    if (tempHandle == INVALID_FILE_HANDLE)
    {
        return false;
    }
    fwrite(tempHandle, profileData);
    fclose(tempHandle);

    new validated[128];
    new File:validateHandle = fopen(tempPath, io_read);
    if (validateHandle == INVALID_FILE_HANDLE)
    {
        fremove(tempPath);
        return false;
    }
    fread(validateHandle, validated, sizeof(validated));
    fclose(validateHandle);

    if (strcmp(validated, profileData, true) != 0)
    {
        fremove(tempPath);
        return false;
    }

    if (!dini_Exists(fileName))
    {
        new File:bootstrapHandle = fopen("scriptfiles/premium/.keep", io_write);
        if (bootstrapHandle != INVALID_FILE_HANDLE)
        {
            fclose(bootstrapHandle);
        }
    }

    if (!dini_Set(fileName, PREMIUM_SAVE_KEY, profileData))
    {
        fremove(tempPath);
        return false;
    }

    if (fexist(tempPath))
    {
        fremove(tempPath);
    }
    return true;
}

stock bool:PremiumSystem_IsPlayerPremiumExpired(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return true;
    }

    if (Core_HasPermanentPremium(playerid))
    {
        return false;
    }

    new tier = Core_GetPlayerPremiumTier(playerid);
    if (!PremiumSystem_IsTierValid(tier) || tier == PREMIUM_TIER_NONE)
    {
        return true;
    }

    new expiry = Core_GetPlayerPremiumExpiry(playerid);
    if (expiry <= 0)
    {
        return true;
    }

    return gettime() >= expiry;
}

stock PremiumSystem_RefreshPlayerPremium(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    if (Core_HasPermanentPremium(playerid))
    {
        Core_SetPlayerPremiumTier(playerid, PREMIUM_TIER_PERMANENT);
        Core_SetPlayerPremiumExpiry(playerid, 0);
        return true;
    }

    new tier = Core_GetPlayerPremiumTier(playerid);
    if (!PremiumSystem_IsTierValid(tier) || tier == PREMIUM_TIER_NONE)
    {
        Core_SetPlayerPremiumTier(playerid, PREMIUM_TIER_NONE);
        Core_SetPlayerPremiumExpiry(playerid, 0);
        return false;
    }

    if (PremiumSystem_IsPlayerPremiumExpired(playerid))
    {
        Core_SetPlayerPremiumTier(playerid, PREMIUM_TIER_NONE);
        Core_SetPlayerPremiumExpiry(playerid, 0);
        Core_SetPermanentPremium(playerid, false);
        return false;
    }

    return true;
}

stock bool:PremiumSystem_ActivatePremium(playerid, tier, expiresAt)
{
    if (!Core_IsValidPlayerId(playerid) || !PremiumSystem_IsTierValid(tier))
    {
        return false;
    }

    if (tier == PREMIUM_TIER_PERMANENT)
    {
        Core_ActivatePremium(playerid, tier, 0);
        return true;
    }

    if (expiresAt <= 0)
    {
        expiresAt = gettime() + (PremiumSystem_GetTierDays(tier) * 86400);
    }

    return Core_ActivatePremium(playerid, tier, expiresAt);
}

stock bool:PremiumSystem_ExtendPremium(playerid, tier, days)
{
    if (!Core_IsValidPlayerId(playerid) || !PremiumSystem_IsTierValid(tier) || days <= 0)
    {
        return false;
    }

    if (tier == PREMIUM_TIER_PERMANENT)
    {
        Core_SetPermanentPremium(playerid, true);
        Core_SetPlayerPremiumTier(playerid, PREMIUM_TIER_PERMANENT);
        Core_SetPlayerPremiumExpiry(playerid, 0);
        return true;
    }

    return Core_ExtendPremium(playerid, tier, days);
}

stock PremiumSystem_LoadPlayerPremium(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new fileName[64];
    format(fileName, sizeof(fileName), "%s/player_%d", PREMIUM_FILE_PREFIX, playerid);
    if (!dini_Exists(fileName))
    {
        Core_SetPlayerPremiumTier(playerid, PREMIUM_TIER_NONE);
        Core_SetPlayerPremiumExpiry(playerid, 0);
        Core_SetPermanentPremium(playerid, false);
        return false;
    }

    new profileData[128];
    if (!dini_Get(fileName, PREMIUM_SAVE_KEY, profileData, sizeof(profileData)))
    {
        Core_SetPlayerPremiumTier(playerid, PREMIUM_TIER_NONE);
        Core_SetPlayerPremiumExpiry(playerid, 0);
        Core_SetPermanentPremium(playerid, false);
        return false;
    }

    new tier = PREMIUM_TIER_NONE;
    new expiry = 0;
    new permanent = 0;
    if (sscanf(profileData, "p<|>iii", tier, expiry, permanent) != 0)
    {
        Core_SetPlayerPremiumTier(playerid, PREMIUM_TIER_NONE);
        Core_SetPlayerPremiumExpiry(playerid, 0);
        Core_SetPermanentPremium(playerid, false);
        return false;
    }

    if (!PremiumSystem_IsTierValid(tier))
    {
        tier = PREMIUM_TIER_NONE;
        expiry = 0;
        permanent = 0;
    }

    Core_SetPlayerPremiumTier(playerid, tier);
    Core_SetPlayerPremiumExpiry(playerid, expiry);
    Core_SetPermanentPremium(playerid, permanent == 1);
    return PremiumSystem_RefreshPlayerPremium(playerid);
}

stock PremiumSystem_SavePlayerPremium(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    new fileName[64];
    format(fileName, sizeof(fileName), "%s/player_%d", PREMIUM_FILE_PREFIX, playerid);

    new tier = Core_GetPlayerPremiumTier(playerid);
    new expiry = Core_GetPlayerPremiumExpiry(playerid);
    new permanent = Core_HasPermanentPremium(playerid) ? 1 : 0;
    if (permanent == 1)
    {
        expiry = 0;
    }

    new profileData[128];
    format(profileData, sizeof(profileData), "%d|%d|%d", tier, expiry, permanent);
    return PremiumSystem_WriteProfileSafe(playerid, profileData);
}

stock bool:PremiumSystem_IsPremiumActive(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return false;
    }

    if (Core_HasPermanentPremium(playerid))
    {
        return true;
    }

    return Core_IsPremiumActive(playerid);
}

stock PremiumSystem_Initialize()
{
    print("[PREMIUM] Premium-System initialisiert");
    Core_MarkModuleReady("premium");
    return 1;
}

stock PremiumSystem_OnPlayerConnect(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    PremiumSystem_LoadPlayerPremium(playerid);
    PremiumSystem_RefreshPlayerPremium(playerid);
    if (PremiumSystem_IsPremiumActive(playerid))
    {
        new label[32];
        PremiumSystem_GetTierLabel(Core_GetPlayerPremiumTier(playerid), label, sizeof(label));
        printf("[PREMIUM] Spieler %d hat Premium-Stufe: %s", playerid, label);
    }
}

stock PremiumSystem_OnPlayerDisconnect(playerid, reason)
{
    (void:reason);
    if (!Core_IsValidPlayerId(playerid))
    {
        return;
    }

    PremiumSystem_RefreshPlayerPremium(playerid);
    PremiumSystem_SavePlayerPremium(playerid);
}

#endif
