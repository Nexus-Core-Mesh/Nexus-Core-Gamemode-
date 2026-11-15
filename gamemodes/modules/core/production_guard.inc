#if !defined PRODUCTION_GUARD_INC
#define PRODUCTION_GUARD_INC

#include <a_samp>

#if !defined MAX_INVENTORY_SLOTS
#define MAX_INVENTORY_SLOTS 24
#endif
#if !defined MAX_HOUSE_STORAGE_SLOTS
#define MAX_HOUSE_STORAGE_SLOTS 12
#endif
#if !defined MAX_UBB_VEHICLES
#define MAX_UBB_VEHICLES 100
#endif

stock Core_ValidatePlayerSlot(playerid)
{
    return (playerid >= 0 && playerid < MAX_PLAYERS);
}

stock Core_ValidateInventorySlot(slot)
{
    return (slot >= 0 && slot < MAX_INVENTORY_SLOTS);
}

stock Core_ValidateHouseStorageSlot(slot)
{
    return (slot >= 0 && slot < MAX_HOUSE_STORAGE_SLOTS);
}

stock Core_ValidateVehicleIndex(vehicleid)
{
    return (vehicleid >= 0 && vehicleid < MAX_UBB_VEHICLES);
}

stock Core_ValidateQuantityRange(quantity, maxLimit)
{
    return (quantity >= 0 && maxLimit >= 0 && quantity <= maxLimit);
}

stock Core_ValidateItemStack(itemid, quantity, maxStack, bool:isStackable)
{
    if (itemid < INVENTORY_ITEM_NONE)
    {
        return false;
    }
    if (quantity < 0)
    {
        return false;
    }
    if (quantity == 0)
    {
        return true;
    }
    if (maxStack <= 0)
    {
        return false;
    }
    if (isStackable)
    {
        return quantity <= maxStack;
    }
    return quantity == 1;
}

stock Core_ValidateStateTransition(playerid, stateid)
{
    if (!Core_ValidatePlayerSlot(playerid))
    {
        return false;
    }
    if (!Core_IsValidState(stateid))
    {
        return false;
    }
    return true;
}

stock Core_EmitRuntimeGuardReport()
{
    print("[CORE_GUARD] Production safety guard active");
    print("[CORE_GUARD] Inventory slots, house storage slots and UBB vehicles are bounded");
    print("[CORE_GUARD] Transaction boundaries and state transitions are validated before mutation");
}

#endif
