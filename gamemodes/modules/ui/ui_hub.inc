#if !defined UI_HUB_INC
#define UI_HUB_INC

#include <a_samp>

// UI-Hub: dialoggesteuerte Oberflächen für Inventar, Handy, Business, Ammu-Nation, Mission-Board, Garage und Tuning.
// Jede Funktion ist ohne Chat-Befehl erreichbar: Taste Y oder /menu öffnen das Hauptmenü.
#define UI_AMMU_RADIUS 40.0
#define UI_MAX_PICK 24
#define UI_TUNE_COUNT 6

new gUiInvSlotMap[MAX_PLAYERS][MAX_INVENTORY_SLOTS];
new gUiInvSlotCount[MAX_PLAYERS];
new gUiInvSlot[MAX_PLAYERS];
new gUiInvItem[MAX_PLAYERS];
new gUiInvGiveTarget[MAX_PLAYERS];
new gUiPick[MAX_PLAYERS][UI_MAX_PICK];
new gUiPickCount[MAX_PLAYERS];
new gUiSelected[MAX_PLAYERS];

new const gUiTuneName[UI_TUNE_COUNT][24] = {"Nitro 2x", "Nitro 5x", "Nitro 10x", "Hydraulik", "Stereo", "Wheels Shadow"};
new const gUiTuneComponent[UI_TUNE_COUNT] = {1009, 1008, 1010, 1087, 1086, 1073};
new const gUiTuneSlot[UI_TUNE_COUNT] = {5, 5, 5, VEHICLE_MOD_HYDRAULICS, 6, VEHICLE_MOD_WHEELS};
new const gUiTunePrice[UI_TUNE_COUNT] = {2000, 3500, 5000, 3500, 800, 1800};

stock UiHub_Initialize()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        UiHub_OnPlayerConnect(i);
    }

    print("[UI_HUB] Dialog-First Menüsystem initialisiert");
    Core_MarkModuleReady("ui");
}

stock UiHub_OnPlayerConnect(playerid)
{
    gUiInvSlotCount[playerid] = 0;
    gUiInvSlot[playerid] = -1;
    gUiInvItem[playerid] = 0;
    gUiInvGiveTarget[playerid] = INVALID_PLAYER_ID;
    gUiPickCount[playerid] = 0;
    gUiSelected[playerid] = -1;
}

stock UiHub_Ready(playerid)
{
    if (!Core_AssertLifecycleStage(CORE_LIFECYCLE_RUNTIME_READY))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[UI] Der Server startet noch, bitte kurz warten.");
        return false;
    }
    if (!LoginSystem_IsActive(playerid))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[UI] Bitte zuerst einloggen.");
        return false;
    }
    return true;
}

stock UiHub_ShowMain(playerid)
{
    if (!UiHub_Ready(playerid))
    {
        return;
    }

    ShowPlayerDialog(playerid, DIALOG_UI_MAIN, DIALOG_STYLE_LIST, "Nexus Core | Hauptmenü",
        "Inventar\nHandy\nBusiness\nAmmu-Nation\nMission-Board\nGarage\nFahrzeug-Tuning\nJob-Zentrum\nText-Wars\nStaats-Leitstand\nSpieler-Dashboard",
        UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_CLOSE);
}

stock UiHub_ShowText(playerid, title[], text[])
{
    ShowPlayerDialog(playerid, DIALOG_UI_NONE, DIALOG_STYLE_MSGBOX, title, text, UI_DIALOG_BUTTON_CLOSE, "");
}

// ---------------------------------------------------------------- Inventar

stock UiHub_GetItemName(itemid, output[], len)
{
    new bool:stackable = false;
    new maxStack = 0;
    Inventory_GetItemData(itemid, output, len, stackable, maxStack);
}

stock UiHub_ShowInventory(playerid)
{
    if (!UiHub_Ready(playerid))
    {
        return;
    }

    new list[1536];
    format(list, sizeof(list), "Slot\tItem\tMenge\n");
    gUiInvSlotCount[playerid] = 0;

    for (new s = 0; s < MAX_INVENTORY_SLOTS; s++)
    {
        if (gInventoryItem[playerid][s] == INVENTORY_ITEM_NONE || gInventoryItemQty[playerid][s] <= 0)
        {
            continue;
        }

        new name[32];
        UiHub_GetItemName(gInventoryItem[playerid][s], name, sizeof(name));
        new line[64];
        format(line, sizeof(line), "%d\t%s\t%dx\n", s + 1, name, gInventoryItemQty[playerid][s]);
        strcat(list, line);
        gUiInvSlotMap[playerid][gUiInvSlotCount[playerid]++] = s;
    }

    if (gUiInvSlotCount[playerid] == 0)
    {
        UiHub_ShowText(playerid, "Inventar", "Dein Inventar ist leer.");
        return;
    }

    new title[64];
    format(title, sizeof(title), "Inventar | Freie Slots: %d", InventorySystem_GetFreeSlots(playerid));
    ShowPlayerDialog(playerid, DIALOG_UI_INV_LIST, DIALOG_STYLE_TABLIST_HEADERS, title, list, "Aktion", UI_DIALOG_BUTTON_CLOSE);
}

stock UiHub_SlotStillValid(playerid)
{
    new slot = gUiInvSlot[playerid];
    return slot >= 0 && slot < MAX_INVENTORY_SLOTS && gInventoryItem[playerid][slot] == gUiInvItem[playerid] && gInventoryItemQty[playerid][slot] > 0;
}

stock int:UiHub_FindHouseSafe(playerid)
{
    for (new h = 0; h < MAX_HOUSES; h++)
    {
        if (gHouseStorageOwner[h] == playerid && Core_IsValidState(gHouseStorageState[h]))
        {
            return h;
        }
    }
    return -1;
}

stock UiHub_ShowInventoryAction(playerid)
{
    new name[32];
    UiHub_GetItemName(gUiInvItem[playerid], name, sizeof(name));
    new title[64];
    format(title, sizeof(title), "%s (%dx)", name, gInventoryItemQty[playerid][gUiInvSlot[playerid]]);
    ShowPlayerDialog(playerid, DIALOG_UI_INV_ACTION, DIALOG_STYLE_LIST, title,
        "Benutzen\n1 wegwerfen\nAlle wegwerfen\nAn Spieler übertragen\nIn Haussafe legen", UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
}

stock UiHub_InventoryUse(playerid)
{
    new result = InventorySystem_UseSlot(playerid, gUiInvSlot[playerid]);
    switch (result)
    {
        case INV_USE_OPEN_PHONE: UiHub_ShowPhone(playerid);
        case INV_USE_CONSUMED: SendClientMessage(playerid, 0x00FF99AA, "[INV] Item benutzt.");
        case INV_USE_NO_EFFECT: SendClientMessage(playerid, 0xFFCC66AA, "[INV] Dieses Item hat aktuell keinen Effekt.");
        default: SendClientMessage(playerid, 0xFF6666AA, "[INV] Item kann gerade nicht benutzt werden.");
    }
}

stock UiHub_InventoryDrop(playerid, bool:all)
{
    new qty = all ? gInventoryItemQty[playerid][gUiInvSlot[playerid]] : 1;
    if (Inventory_Remove(playerid, gUiInvItem[playerid], qty))
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[INV] Item weggeworfen.");
    }
}

stock UiHub_InventoryToSafe(playerid)
{
    new house = UiHub_FindHouseSafe(playerid);
    if (house == -1)
    {
        SendClientMessage(playerid, 0xFF6666AA, "[INV] Du besitzt keinen Haussafe.");
        return;
    }

    new qty = gInventoryItemQty[playerid][gUiInvSlot[playerid]];
    new meta[32];
    format(meta, sizeof(meta), "%s", gInventoryItemMeta[playerid][gUiInvSlot[playerid]]);
    if (HouseStorageSystem_StoreItem(playerid, house, gUiInvItem[playerid], qty, meta))
    {
        Inventory_Remove(playerid, gUiInvItem[playerid], qty);
        SendClientMessage(playerid, 0x00FF99AA, "[INV] Item im Haussafe eingelagert.");
    }
    else
    {
        SendClientMessage(playerid, 0xFF6666AA, "[INV] Haussafe voll oder nicht erreichbar.");
    }
}

stock UiHub_InventoryGive(playerid, const inputtext[], bool:askQuantity)
{
    new target = INVALID_PLAYER_ID;
    if (askQuantity)
    {
        if (sscanf(inputtext, "u", target) != 0 || target == INVALID_PLAYER_ID || target == playerid || !IsPlayerConnected(target))
        {
            SendClientMessage(playerid, 0xFF6666AA, "[INV] Spieler nicht gefunden.");
            return;
        }

        new Float:x, Float:y, Float:z;
        GetPlayerPos(target, x, y, z);
        if (GetPlayerDistanceFromPoint(playerid, x, y, z) > 6.0)
        {
            SendClientMessage(playerid, 0xFF6666AA, "[INV] Der Spieler ist zu weit entfernt.");
            return;
        }
        gUiInvGiveTarget[playerid] = target;

        if (gInventoryItemQty[playerid][gUiInvSlot[playerid]] > 1)
        {
            ShowPlayerDialog(playerid, DIALOG_UI_INV_GIVE_QTY, DIALOG_STYLE_INPUT, "Menge übertragen", "Wie viele Stück übertragen?", "Senden", UI_DIALOG_BUTTON_BACK);
            return;
        }
        UiHub_InventoryTransfer(playerid, 1);
        return;
    }

    UiHub_InventoryTransfer(playerid, strval(inputtext));
}

stock UiHub_InventoryTransfer(playerid, quantity)
{
    new target = gUiInvGiveTarget[playerid];
    if (!IsPlayerConnected(target) || quantity <= 0 || quantity > gInventoryItemQty[playerid][gUiInvSlot[playerid]])
    {
        SendClientMessage(playerid, 0xFF6666AA, "[INV] Ungültige Menge oder Ziel.");
        return;
    }

    new meta[32];
    format(meta, sizeof(meta), "%s", gInventoryItemMeta[playerid][gUiInvSlot[playerid]]);
    if (InventorySystem_TransferItem(playerid, target, gUiInvItem[playerid], quantity, meta))
    {
        SendClientMessage(playerid, 0x00FF99AA, "[INV] Item übertragen.");
        SendClientMessage(target, 0x00FF99AA, "[INV] Du hast ein Item erhalten.");
    }
    else
    {
        SendClientMessage(playerid, 0xFF6666AA, "[INV] Übertragung fehlgeschlagen (Inventar des Ziels voll?).");
    }
}

// ---------------------------------------------------------------- Handy

stock UiHub_ShowPhone(playerid)
{
    new title[48];
    format(title, sizeof(title), "Handy | Nummer %d", gPhoneNumber[playerid]);
    ShowPlayerDialog(playerid, DIALOG_UI_PHONE_MAIN, DIALOG_STYLE_LIST, title,
        "Kontakte ansehen\nKontakt hinzufügen\nSMS verfassen\nNachrichten lesen", UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_CLOSE);
}

stock UiHub_ShowPhoneContacts(playerid)
{
    new list[1024];
    for (new i = 0; i < gPhoneContactCount[playerid]; i++)
    {
        strcat(list, gPhoneContacts[playerid][i]);
        strcat(list, "\n");
    }
    if (gPhoneContactCount[playerid] == 0)
    {
        format(list, sizeof(list), "Keine Kontakte gespeichert.");
    }
    ShowPlayerDialog(playerid, DIALOG_UI_PHONE_MAIN + 3, DIALOG_STYLE_MSGBOX, "Kontakte", list, UI_DIALOG_BUTTON_BACK, UI_DIALOG_BUTTON_CLOSE);
}

stock UiHub_ShowPhoneMessages(playerid)
{
    new list[2048];
    for (new i = 0; i < gPhoneMessageCount[playerid]; i++)
    {
        strcat(list, gPhoneMessages[playerid][i]);
        strcat(list, "\n");
    }
    if (gPhoneMessageCount[playerid] == 0)
    {
        format(list, sizeof(list), "Keine Nachrichten.");
    }
    ShowPlayerDialog(playerid, DIALOG_UI_PHONE_MAIN + 3, DIALOG_STYLE_MSGBOX, "Nachrichten", list, UI_DIALOG_BUTTON_BACK, UI_DIALOG_BUTTON_CLOSE);
}

// ---------------------------------------------------------------- Business

stock UiHub_ShowBusinessList(playerid)
{
    new list[1536];
    format(list, sizeof(list), "Business\tStaat\tWert\tStatus\n");
    gUiPickCount[playerid] = 0;

    for (new b = 0; b < MAX_BUSINESSES && gUiPickCount[playerid] < UI_MAX_PICK; b++)
    {
        if (gBizActive[b] != 1 || !Core_IsValidState(gBizState[b]))
        {
            continue;
        }

        new stateShort[8];
        Core_GetStateShortName(gBizState[b], stateShort, sizeof(stateShort));
        new line[96];
        format(line, sizeof(line), "%s\t%s\t$%d\t%s\n", gBizName[b], stateShort, gBizValue[b], (gBizOwner[b] == -1) ? ("frei") : ((gBizOwner[b] == playerid) ? ("dein Besitz") : ("vergeben")));
        strcat(list, line);
        gUiPick[playerid][gUiPickCount[playerid]++] = b;
    }

    ShowPlayerDialog(playerid, DIALOG_UI_BIZ_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Business-Verwaltung", list, "Öffnen", UI_DIALOG_BUTTON_CLOSE);
}

stock UiHub_ShowBusinessAction(playerid, bizid)
{
    new title[48];
    format(title, sizeof(title), "%s", gBizName[bizid]);
    if (gBizOwner[bizid] == -1)
    {
        ShowPlayerDialog(playerid, DIALOG_UI_BIZ_ACTION, DIALOG_STYLE_LIST, title, "Informationen\nKaufen", UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
    }
    else if (gBizOwner[bizid] == playerid)
    {
        ShowPlayerDialog(playerid, DIALOG_UI_BIZ_ACTION, DIALOG_STYLE_LIST, title, "Informationen\nEinkommen auszahlen", UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
    }
    else
    {
        ShowPlayerDialog(playerid, DIALOG_UI_BIZ_ACTION, DIALOG_STYLE_LIST, title, "Informationen", UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
    }
}

// ---------------------------------------------------------------- Ammu-Nation

stock UiHub_ShowAmmu(playerid)
{
    new stateid = StateSystem_GetPlayerState(playerid);
    if (!AmmuSystem_IsNearShop(playerid, stateid, UI_AMMU_RADIUS))
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[AMMU] Du musst bei einer Ammu-Nation deines Staates sein.");
        return;
    }

    new list[512];
    format(list, sizeof(list), "Waffe\tPreis\n");
    gUiPickCount[playerid] = 0;

    for (new i = 0; i < gWeaponShopCount && gUiPickCount[playerid] < UI_MAX_PICK; i++)
    {
        if (gWeaponShopState[i] != stateid)
        {
            continue;
        }
        new line[64];
        format(line, sizeof(line), "%s\t$%d\n", gWeaponShopName[i], gWeaponShopPrice[i]);
        strcat(list, line);
        gUiPick[playerid][gUiPickCount[playerid]++] = i;
    }

    if (gUiPickCount[playerid] == 0)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[AMMU] Keine Waffen im aktuellen Staat verfügbar.");
        return;
    }
    ShowPlayerDialog(playerid, DIALOG_UI_AMMU_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Ammu-Nation", list, "Kaufen", UI_DIALOG_BUTTON_CLOSE);
}

// ---------------------------------------------------------------- Mission-Board

stock UiHub_ShowMissions(playerid)
{
    new stateid = StateSystem_GetPlayerState(playerid);
    new list[768];
    format(list, sizeof(list), "Aufgabe\tBelohnung\n");
    gUiPickCount[playerid] = 0;

    for (new m = 0; m < gMissionBoardCount && gUiPickCount[playerid] < UI_MAX_PICK; m++)
    {
        if (gMissionBoardState[m] != stateid || gMissionBoardActive[m] != 1)
        {
            continue;
        }
        new line[80];
        format(line, sizeof(line), "%s\t$%d\n", gMissionBoardName[m], gMissionBoardReward[m]);
        strcat(list, line);
        gUiPick[playerid][gUiPickCount[playerid]++] = m;
    }

    if (gUiPickCount[playerid] == 0)
    {
        UiHub_ShowText(playerid, "Mission-Board", "Aktuell sind keine Aufgaben verfügbar.");
        return;
    }
    ShowPlayerDialog(playerid, DIALOG_UI_MISSION_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Mission-Board", list, "Annehmen", UI_DIALOG_BUTTON_CLOSE);
}

// ---------------------------------------------------------------- Garage & Tuning

stock UiHub_ShowGarages(playerid)
{
    new list[512];
    format(list, sizeof(list), "Garage\tStaat\tFahrzeuge\n");
    gUiPickCount[playerid] = 0;

    for (new g = 0; g < gGarageCount && gUiPickCount[playerid] < UI_MAX_PICK; g++)
    {
        new stateShort[8];
        Core_GetStateShortName(gGarageState[g], stateShort, sizeof(stateShort));
        new line[80];
        format(line, sizeof(line), "%s\t%s\t%d\n", gGarageName[g], stateShort, gGarageVehicleCount[g]);
        strcat(list, line);
        gUiPick[playerid][gUiPickCount[playerid]++] = g;
    }
    ShowPlayerDialog(playerid, DIALOG_UI_GARAGE_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Garagen", list, "Öffnen", UI_DIALOG_BUTTON_CLOSE);
}

stock UiHub_ShowTuning(playerid)
{
    new vehicleid = GetPlayerVehicleID(playerid);
    if (vehicleid == 0 || GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[TUNING] Du musst Fahrer eines Fahrzeugs sein.");
        return;
    }

    new list[512];
    format(list, sizeof(list), "Modifikation\tPreis\n");
    for (new t = 0; t < UI_TUNE_COUNT; t++)
    {
        new line[64];
        format(line, sizeof(line), "%s\t$%d\n", gUiTuneName[t], gUiTunePrice[t]);
        strcat(list, line);
    }
    ShowPlayerDialog(playerid, DIALOG_UI_TUNING_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Fahrzeug-Tuning", list, "Einbauen", UI_DIALOG_BUTTON_CLOSE);
}

stock UiHub_BuyTuning(playerid, option)
{
    new vehicleid = GetPlayerVehicleID(playerid);
    if (vehicleid == 0 || GetPlayerState(playerid) != PLAYER_STATE_DRIVER || option < 0 || option >= UI_TUNE_COUNT)
    {
        SendClientMessage(playerid, 0xFFCC66AA, "[TUNING] Du musst Fahrer eines Fahrzeugs sein.");
        return;
    }
    if (!Core_ChangePlayerBank(playerid, -gUiTunePrice[option]))
    {
        SendClientMessage(playerid, 0xFF6666AA, "[TUNING] Nicht genug Geld auf der Bank.");
        return;
    }
    if (!TuningSystem_AddMod(playerid, vehicleid, gUiTuneSlot[option], gUiTuneComponent[option]))
    {
        Core_ChangePlayerBank(playerid, gUiTunePrice[option]);
        return;
    }
    StateSystem_ApplyTreasuryDelta(StateSystem_GetPlayerState(playerid), gUiTunePrice[option] / 5);
}

// ---------------------------------------------------------------- Routing

stock UiHub_OnDialogResponse(playerid, dialogid, response, listitem, const inputtext[])
{
    if (dialogid < DIALOG_UI_RANGE_HUB_FIRST || dialogid > DIALOG_UI_RANGE_HUB_LAST)
    {
        return false;
    }
    if (!UiHub_Ready(playerid))
    {
        return true;
    }

    switch (dialogid)
    {
        case DIALOG_UI_MAIN:
        {
            if (!response)
            {
                return true;
            }
            switch (listitem)
            {
                case 0: UiHub_ShowInventory(playerid);
                case 1: UiHub_ShowPhone(playerid);
                case 2: UiHub_ShowBusinessList(playerid);
                case 3: UiHub_ShowAmmu(playerid);
                case 4: UiHub_ShowMissions(playerid);
                case 5: UiHub_ShowGarages(playerid);
                case 6: UiHub_ShowTuning(playerid);
                case 7: JobCenter_ShowMain(playerid);
                case 8: TextWars_ShowMain(playerid);
                case 9: StateControl_ShowMain(playerid);
                case 10: RoleplayUI_ShowDashboard(playerid);
            }
        }
        case DIALOG_UI_INV_LIST:
        {
            if (!response || listitem < 0 || listitem >= gUiInvSlotCount[playerid])
            {
                return true;
            }
            gUiInvSlot[playerid] = gUiInvSlotMap[playerid][listitem];
            gUiInvItem[playerid] = gInventoryItem[playerid][gUiInvSlot[playerid]];
            UiHub_ShowInventoryAction(playerid);
        }
        case DIALOG_UI_INV_ACTION:
        {
            if (!response)
            {
                UiHub_ShowInventory(playerid);
                return true;
            }
            if (!UiHub_SlotStillValid(playerid))
            {
                SendClientMessage(playerid, 0xFF6666AA, "[INV] Der Slot hat sich geändert.");
                UiHub_ShowInventory(playerid);
                return true;
            }
            switch (listitem)
            {
                case 0: UiHub_InventoryUse(playerid);
                case 1:
                {
                    UiHub_InventoryDrop(playerid, false);
                    UiHub_ShowInventory(playerid);
                }
                case 2:
                {
                    UiHub_InventoryDrop(playerid, true);
                    UiHub_ShowInventory(playerid);
                }
                case 3: ShowPlayerDialog(playerid, DIALOG_UI_INV_GIVE_TARGET, DIALOG_STYLE_INPUT, "Übertragen", "Spieler-ID oder Name (in deiner Nähe):", "Weiter", UI_DIALOG_BUTTON_BACK);
                case 4:
                {
                    UiHub_InventoryToSafe(playerid);
                    UiHub_ShowInventory(playerid);
                }
            }
        }
        case DIALOG_UI_INV_GIVE_TARGET:
        {
            if (!response)
            {
                UiHub_ShowInventoryAction(playerid);
                return true;
            }
            if (UiHub_SlotStillValid(playerid))
            {
                UiHub_InventoryGive(playerid, inputtext, true);
            }
        }
        case DIALOG_UI_INV_GIVE_QTY:
        {
            if (response && UiHub_SlotStillValid(playerid))
            {
                UiHub_InventoryGive(playerid, inputtext, false);
            }
        }
        case DIALOG_UI_PHONE_MAIN:
        {
            if (!response)
            {
                return true;
            }
            switch (listitem)
            {
                case 0: UiHub_ShowPhoneContacts(playerid);
                case 1: ShowPlayerDialog(playerid, DIALOG_UI_PHONE_ADD, DIALOG_STYLE_INPUT, "Kontakt hinzufügen", "Name des Kontakts:", "Speichern", UI_DIALOG_BUTTON_BACK);
                case 2: ShowPlayerDialog(playerid, DIALOG_UI_PHONE_SMS, DIALOG_STYLE_INPUT, "SMS verfassen", "Nachricht (max. 63 Zeichen):", "Senden", UI_DIALOG_BUTTON_BACK);
                case 3: UiHub_ShowPhoneMessages(playerid);
            }
        }
        case DIALOG_UI_PHONE_MAIN + 3:
        {
            if (response)
            {
                UiHub_ShowPhone(playerid);
            }
        }
        case DIALOG_UI_PHONE_ADD:
        {
            if (!response)
            {
                UiHub_ShowPhone(playerid);
                return true;
            }
            if (strlen(inputtext) == 0 || strlen(inputtext) > 31 || !UiDialog_IsSafeText(inputtext))
            {
                SendClientMessage(playerid, 0xFF6666AA, "[PHONE] Ungültiger Kontaktname.");
            }
            else
            {
                PhoneSystem_AddContact(playerid, inputtext);
            }
            UiHub_ShowPhone(playerid);
        }
        case DIALOG_UI_PHONE_SMS:
        {
            if (!response)
            {
                UiHub_ShowPhone(playerid);
                return true;
            }
            if (strlen(inputtext) == 0 || strlen(inputtext) > 63 || !UiDialog_IsSafeText(inputtext))
            {
                SendClientMessage(playerid, 0xFF6666AA, "[PHONE] Ungültige Nachricht.");
            }
            else
            {
                PhoneSystem_SendSMS(playerid, inputtext);
            }
            UiHub_ShowPhone(playerid);
        }
        case DIALOG_UI_BIZ_LIST:
        {
            if (!response || listitem < 0 || listitem >= gUiPickCount[playerid])
            {
                return true;
            }
            gUiSelected[playerid] = gUiPick[playerid][listitem];
            UiHub_ShowBusinessAction(playerid, gUiSelected[playerid]);
        }
        case DIALOG_UI_BIZ_ACTION:
        {
            if (!response)
            {
                UiHub_ShowBusinessList(playerid);
                return true;
            }
            new bizid = gUiSelected[playerid];
            if (listitem == 0)
            {
                BusinessSystem_ShowInfo(playerid, bizid);
            }
            else if (gBizOwner[bizid] == -1)
            {
                BusinessSystem_BuyBusiness(playerid, bizid);
            }
            else
            {
                new paid = BusinessSystem_PayoutToOwner(playerid, bizid);
                if (paid > 0)
                {
                    new msg[64];
                    format(msg, sizeof(msg), "[BIZ] $%d Einkommen ausgezahlt.", paid);
                    SendClientMessage(playerid, 0x00FF99AA, msg);
                }
                else
                {
                    SendClientMessage(playerid, 0xFFCC66AA, "[BIZ] Kein Einkommen zum Auszahlen.");
                }
            }
        }
        case DIALOG_UI_AMMU_LIST:
        {
            if (!response || listitem < 0 || listitem >= gUiPickCount[playerid])
            {
                return true;
            }
            gUiSelected[playerid] = gUiPick[playerid][listitem];
            new text[128];
            format(text, sizeof(text), "%s für $%d kaufen?", gWeaponShopName[gUiSelected[playerid]], gWeaponShopPrice[gUiSelected[playerid]]);
            ShowPlayerDialog(playerid, DIALOG_UI_AMMU_CONFIRM, DIALOG_STYLE_MSGBOX, "Kauf bestätigen", text, "Kaufen", UI_DIALOG_BUTTON_BACK);
        }
        case DIALOG_UI_AMMU_CONFIRM:
        {
            if (!response)
            {
                UiHub_ShowAmmu(playerid);
                return true;
            }
            new stateid = StateSystem_GetPlayerState(playerid);
            if (AmmuSystem_IsNearShop(playerid, stateid, UI_AMMU_RADIUS) && gUiSelected[playerid] >= 0 && gWeaponShopState[gUiSelected[playerid]] == stateid)
            {
                WeaponShopSystem_BuyWeapon(playerid, gWeaponShopWeapon[gUiSelected[playerid]], stateid);
            }
        }
        case DIALOG_UI_MISSION_LIST:
        {
            if (!response || listitem < 0 || listitem >= gUiPickCount[playerid])
            {
                return true;
            }
            gUiSelected[playerid] = gUiPick[playerid][listitem];
            new text[128];
            format(text, sizeof(text), "Aufgabe \"%s\" annehmen? Belohnung: $%d", gMissionBoardName[gUiSelected[playerid]], gMissionBoardReward[gUiSelected[playerid]]);
            ShowPlayerDialog(playerid, DIALOG_UI_MISSION_CONFIRM, DIALOG_STYLE_MSGBOX, "Mission annehmen", text, "Annehmen", UI_DIALOG_BUTTON_BACK);
        }
        case DIALOG_UI_MISSION_CONFIRM:
        {
            if (!response)
            {
                UiHub_ShowMissions(playerid);
                return true;
            }
            if (gUiSelected[playerid] >= 0 && gMissionBoardState[gUiSelected[playerid]] == StateSystem_GetPlayerState(playerid) && MissionBoard_Accept(playerid, gUiSelected[playerid]))
            {
                ProgressionSystem_AddXP(playerid, 20);
                ProgressionSystem_AddRep(playerid, 1);
            }
        }
        case DIALOG_UI_GARAGE_LIST:
        {
            if (!response || listitem < 0 || listitem >= gUiPickCount[playerid])
            {
                return true;
            }
            gUiSelected[playerid] = gUiPick[playerid][listitem];
            ShowPlayerDialog(playerid, DIALOG_UI_GARAGE_ACTION, DIALOG_STYLE_LIST, gGarageName[gUiSelected[playerid]],
                "Zur Garage teleportieren\nAktuelles Fahrzeug einlagern\nTuning öffnen", UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
        }
        case DIALOG_UI_GARAGE_ACTION:
        {
            if (!response)
            {
                UiHub_ShowGarages(playerid);
                return true;
            }
            switch (listitem)
            {
                case 0: GarageSystem_SpawnVehicle(playerid, gUiSelected[playerid]);
                case 1:
                {
                    new vehicleid = GetPlayerVehicleID(playerid);
                    if (vehicleid == 0)
                    {
                        SendClientMessage(playerid, 0xFFCC66AA, "[GARAGE] Du sitzt in keinem Fahrzeug.");
                    }
                    else
                    {
                        GarageSystem_StoreVehicle(playerid, vehicleid);
                    }
                }
                case 2: UiHub_ShowTuning(playerid);
            }
        }
        case DIALOG_UI_TUNING_LIST:
        {
            if (!response || listitem < 0 || listitem >= UI_TUNE_COUNT)
            {
                return true;
            }
            gUiSelected[playerid] = listitem;
            new text[96];
            format(text, sizeof(text), "%s für $%d einbauen?", gUiTuneName[listitem], gUiTunePrice[listitem]);
            ShowPlayerDialog(playerid, DIALOG_UI_TUNING_CONFIRM, DIALOG_STYLE_MSGBOX, "Tuning bestätigen", text, "Einbauen", UI_DIALOG_BUTTON_BACK);
        }
        case DIALOG_UI_TUNING_CONFIRM:
        {
            if (!response)
            {
                UiHub_ShowTuning(playerid);
                return true;
            }
            UiHub_BuyTuning(playerid, gUiSelected[playerid]);
        }
    }
    return true;
}

stock UiHub_HandleCommand(playerid, const cmdtext[])
{
    if (cmdtext[0] != '/')
    {
        return false;
    }

    if (strcmp(cmdtext, "/menu", true) == 0 || strcmp(cmdtext, "/m", true) == 0)
    {
        UiHub_ShowMain(playerid);
        return true;
    }
    if (strcmp(cmdtext, "/inv", true) == 0 || strcmp(cmdtext, "/inventory", true) == 0)
    {
        UiHub_ShowInventory(playerid);
        return true;
    }
    if (strcmp(cmdtext, "/handy", true) == 0 || strcmp(cmdtext, "/phone", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowPhone(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/biz", true) == 0 || strcmp(cmdtext, "/business", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowBusinessList(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/ammu", true) == 0 || strcmp(cmdtext, "/weapons", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowAmmu(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/missions", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowMissions(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/garage", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowGarages(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/tuning", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowTuning(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/jobs", true) == 0 || strcmp(cmdtext, "/jobcenter", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            JobCenter_ShowMain(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/statemenu", true) == 0 || strcmp(cmdtext, "/leitstand", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            StateControl_ShowMain(playerid);
        }
        return true;
    }
    return false;
}

#endif
