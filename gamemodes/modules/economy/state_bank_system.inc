#if !defined STATE_BANK_SYSTEM_INC
#define STATE_BANK_SYSTEM_INC

#include <a_samp>

stock StateBank_Initialize()
{
    print("[STATE_BANK] Staatskassen initialisiert");
}

stock StateBank_Shutdown()
{
    print("[STATE_BANK] Staatskassen heruntergefahren");
}

stock int:StateBank_GetBalance(stateid)
{
    if (!Core_IsValidState(stateid))
    {
        return 0;
    }
    return StateSystem_GetBankCash(stateid);
}

stock StateBank_Deposit(stateid, amount)
{
    if (!Core_IsValidState(stateid) || amount <= 0)
    {
        return false;
    }
    return StateSystem_ApplyTreasuryDelta(stateid, amount);
}

stock StateBank_Withdraw(stateid, amount)
{
    if (!Core_IsValidState(stateid) || amount <= 0)
    {
        return false;
    }
    if (StateSystem_GetBankCash(stateid) < amount)
    {
        return false;
    }
    return StateSystem_ApplyTreasuryDelta(stateid, -amount);
}

#endif
