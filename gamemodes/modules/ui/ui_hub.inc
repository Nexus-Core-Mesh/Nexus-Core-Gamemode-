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

    if (LoginSystem_IsActive(playerid))
    {
        return true;
    }

    if (gPlayerLoginState[playerid] == LOGIN_STATE_LOGIN || gPlayerLoginState[playerid] == LOGIN_STATE_REGISTER)
    {
        return false;
    }

    if (gTutorialStage[playerid] > TUTORIAL_STAGE_NONE && gTutorialStage[playerid] < TUTORIAL_STAGE_FINISH)
    {
        return false;
    }

    SendClientMessage(playerid, 0xFF6666AA, "[UI] Bitte zuerst einloggen.");
    return false;
}

stock UiHub_ShowMain(playerid)
{
    if (!UiHub_Ready(playerid))
    {
        return;
    }

    CameraSystem_ExitCameraMode(playerid);
    RoleplayUI_Hide(playerid);

    new const title[] = "Nexus Core | Hauptmenü";
    new const menu[] = "Stadt & Spawn\nInventar\nHandy\nBusiness\nAmmu-Nation\nMission-Board\nGarage\nFahrzeug-Menü\nFahrzeug-Status\nFahrzeug-Tuning\nJob-Menü\nJob-Zentrum\nText-Wars\nStaats-Leitstand\nSpieler-Dashboard\nSchnellzugriff\nKamera: Frei\nKamera: Drohne\nKamera: Reset";
    UiDialog_Show(playerid, DIALOG_UI_MAIN, DIALOG_STYLE_LIST, title, menu, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_CLOSE);
}

stock UiHub_ShowQuickActionMenu(playerid)
{
    if (!UiHub_Ready(playerid))
    {
        return;
    }

    CameraSystem_ExitCameraMode(playerid);
    RoleplayUI_Hide(playerid);

    new const title[] = "Schnellzugriff";
    new const menu[] = "Stadt & Spawn\nStaatsinfo\nDienstleistungen\nFinanzen\nStatus & Wanted\nWirtschaft\nShops\nJobs\nFahrzeug-Menü\nStats\nFreie Kamera\nDrohnen-Kamera\nKamera aus";
    UiDialog_Show(playerid, DIALOG_UI_STATE_MENU, DIALOG_STYLE_LIST, title, menu, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
}

stock UiHub_HandleNearbyContext(playerid)
{
    if (!UiHub_Ready(playerid))
    {
        return false;
    }

    new stateid = StateSystem_GetPlayerState(playerid);
    if (!Core_IsValidState(stateid))
    {
        stateid = Core_GetValidStateOrDefault(Core_GetPlayerHomeState(playerid), STATE_LOS_SANTOS);
    }

    new Float:x = 0.0, Float:y = 0.0, Float:z = 0.0;
    GetPlayerPos(playerid, x, y, z);

    new Float:jobCenterX[3] = {1535.0, -1915.0, 1668.0};
    new Float:jobCenterY[3] = {-1675.0, 270.0, 1447.0};
    new Float:jobCenterZ[3] = {13.0, 35.0, 10.0};
    new inJobZone = 0;

    for (new i = 0; i < 3; i++)
    {
        if (GetPlayerDistanceFromPoint(playerid, jobCenterX[i], jobCenterY[i], jobCenterZ[i]) <= 30.0)
        {
            inJobZone = 1;
            break;
        }
    }

    new bool:hasMission = (gPlayerMission[playerid] >= 0 && gPlayerMission[playerid] < MAX_MISSIONS);
    new bool:nearDispatch = false;
    for (new i = 0; i < gEmergencyStationCount; i++)
    {
        if (gEmergencyStationState[i] == stateid && GetPlayerDistanceFromPoint(playerid, gEmergencyStationX[i], gEmergencyStationY[i], gEmergencyStationZ[i]) <= 30.0)
        {
            nearDispatch = true;
            break;
        }
    }

    new houseid = HouseSystem_FindNearbyOwnedHouse(playerid, 25.0);
    if (houseid != -1)
    {
        HouseSystem_ShowInfo(playerid, houseid);
        UiHub_ShowInventory(playerid);
        return true;
    }

    new bizid = BusinessSystem_FindNearbyOwnedBusiness(playerid, 25.0);
    if (bizid != -1)
    {
        BusinessSystem_ShowInfo(playerid, bizid);
        UiHub_ShowBusinessList(playerid);
        return true;
    }

    if (InventorySystem_HasNearbyHouseStorage(playerid, 25.0))
    {
        UiHub_ShowInventory(playerid);
        SendClientMessage(playerid, 0x00FF99AA, "[J] Hauslager in Reichweite erkannt: Inventar geöffnet.");
        return true;
    }

    if (inJobZone)
    {
        JobCenter_ShowMain(playerid);
        return true;
    }

    if (nearDispatch)
    {
        DispatchSystem_ShowQueue(playerid, stateid);
        return true;
    }

    if (hasMission)
    {
        MissionSystem_ShowMissionList(playerid);
        return true;
    }

    if (GetPlayerVehicleID(playerid) > 0)
    {
        UiHub_ShowVehicleMenu(playerid);
        return true;
    }

    UiHub_ShowQuickActionMenu(playerid);
    return true;
}

stock UiHub_ShowAuthMenu(playerid)
{
    if (!LoginSystem_IsActive(playerid))
    {
        if (gPlayerLoginState[playerid] == LOGIN_STATE_LOGIN)
        {
            LoginSystem_ShowLoginScreen(playerid);
            return;
        }
        if (gPlayerLoginState[playerid] == LOGIN_STATE_REGISTER)
        {
            LoginSystem_ShowRegisterScreen(playerid);
            return;
        }
        LoginSystem_AutoResolveFlow(playerid);
        return;
    }

    new const title[] = "Account";
    new const menu[] = "Login\nRegistrierung\nPasswort nur\nStadt wählen\nTutorial erneut";
    UiDialog_Show(playerid, DIALOG_UI_AUTH_MAIN, DIALOG_STYLE_LIST, title, menu, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
}

stock UiHub_ShowPasswordInput(playerid)
{
    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);

    if (gPlayerLoginState[playerid] == LOGIN_STATE_REGISTER)
    {
        UiDialog_Show(playerid, DIALOG_UI_AUTH_PASSWORD, DIALOG_STYLE_PASSWORD, "Registrierung", "Neuer Account. Bitte waehle ein Passwort (mind. 3 Zeichen).", "Weiter", "");
        return;
    }

    UiDialog_Show(playerid, DIALOG_UI_AUTH_PASSWORD, DIALOG_STYLE_PASSWORD, "Login", "Willkommen zurueck. Bitte gib dein Passwort ein.", "Einloggen", "");
}

stock UiHub_ShowStatePick(playerid)
{
    if (gPlayerLoginState[playerid] == LOGIN_STATE_LOGIN)
    {
        return;
    }

    if (gTutorialStage[playerid] < TUTORIAL_STAGE_FINISH && gPlayerLoginState[playerid] != LOGIN_STATE_REGISTER)
    {
        return;
    }

    LoginSystem_HideLoginScreen(playerid);
    LoginSystem_HideRegisterScreen(playerid);
    CancelSelectTextDraw(playerid);

    CameraSystem_ExitCameraMode(playerid);
    RoleplayUI_Hide(playerid);

    new stateList[128];
    format(stateList, sizeof(stateList), "Los Santos\nSan Fierro\nLas Venturas");
    UiDialog_Show(playerid, DIALOG_UI_STATE_PICK, DIALOG_STYLE_LIST, "Stadt & Spawn", stateList, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
}

stock UiHub_ShowVehicleMenu(playerid)
{
    if (!UiHub_Ready(playerid))
    {
        return;
    }

    new info[256];
    format(info, sizeof(info), "Status\nGaragen\nTuning\nNahe Fahrzeuge\nSpritpreise\nFahrzeug-Weggeben");
    UiDialog_Show(playerid, DIALOG_UI_VEHICLE_MENU, DIALOG_STYLE_LIST, "Fahrzeuge", info, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
}

stock UiHub_ShowVehicleStatus(playerid)
{
    if (!UiHub_Ready(playerid))
    {
        return;
    }

    new vehicleid = GetPlayerVehicleID(playerid);
    if (vehicleid == 0)
    {
        UiHub_ShowText(playerid, "Fahrzeug-Status", "Du sitzt derzeit in keinem Fahrzeug.\nWähle im Fahrzeug-Menü eine Aktion oder nutze /garage.");
        return;
    }

    new stateName[24];
    new ownerName[MAX_PLAYER_NAME];
    new info[256];

    new stateid = VehicleSystem_GetState(vehicleid);
    if (!Core_IsValidState(stateid))
    {
        format(stateName, sizeof(stateName), "Unbekannt");
    }
    else
    {
        StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
    }

    new ownerid = VehicleSystem_GetOwner(vehicleid);
    if (ownerid < 0 || ownerid >= MAX_PLAYERS || !IsPlayerConnected(ownerid))
    {
        format(ownerName, sizeof(ownerName), "Stadt");
    }
    else
    {
        GetPlayerName(ownerid, ownerName, sizeof(ownerName));
    }

    format(info, sizeof(info), "Fahrzeug-ID: %d\nModell: %d\nStaat: %s\nBesitzer: %s\nPreis: $%d\nTank: %d%%\nStatus: %s",
        vehicleid,
        GetVehicleModel(vehicleid),
        stateName,
        ownerName,
        VehicleSystem_GetPrice(vehicleid),
        gVehicleFuel[vehicleid],
        (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) ? ("Fahrzeug aktiv") : ("Nicht im Fahrerbereich"));

    UiDialog_Show(playerid, DIALOG_UI_VEHICLE_STATUS, DIALOG_STYLE_MSGBOX, "Fahrzeug-Status", info, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_CLOSE);
}

stock UiHub_ShowJobMenu(playerid)
{
    if (!UiHub_Ready(playerid))
    {
        return;
    }

    new jobName[32];
    new jobTypeName[32];
    new currentJob = JobsCore_GetPlayerJob(playerid);
    new currentJobId = JobsCore_GetPlayerJobId(playerid);
    DynamicJobSystem_GetJobName(currentJobId, jobName, sizeof(jobName));
    JobsCore_GetJobTypeName(currentJob, jobTypeName, sizeof(jobTypeName));

    new info[256];
    format(info, sizeof(info), "Aktueller Job: %s\nTyp: %s\nJobs ansehen\nAuftrag starten\nAuftrag abbrechen\nJob kündigen", jobName, jobTypeName);
    UiDialog_Show(playerid, DIALOG_UI_JOB_MENU, DIALOG_STYLE_LIST, "Job-Menü", info, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
}

stock UiHub_ShowText(playerid, const title[], const text[])
{
    new caption[64];
    new info[256];
    format(caption, sizeof(caption), "%s", title);
    format(info, sizeof(info), "%s", text);
    new closeButton[1];
    closeButton[0] = 0;
    UiDialog_Show(playerid, DIALOG_UI_NONE, DIALOG_STYLE_MSGBOX, caption, info, UI_DIALOG_BUTTON_CLOSE, closeButton);
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
    UiDialog_Show(playerid, DIALOG_UI_INV_LIST, DIALOG_STYLE_TABLIST_HEADERS, title, list, "Aktion", UI_DIALOG_BUTTON_CLOSE);
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
    UiDialog_Show(playerid, DIALOG_UI_INV_ACTION, DIALOG_STYLE_LIST, title,
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
            UiDialog_Show(playerid, DIALOG_UI_INV_GIVE_QTY, DIALOG_STYLE_INPUT, "Menge übertragen", "Wie viele Stück übertragen?", "Senden", UI_DIALOG_BUTTON_BACK);
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
    new const phoneMenu[] = "Kontakte ansehen\nKontakt hinzufügen\nSMS verfassen\nNachrichten lesen";
    UiDialog_Show(playerid, DIALOG_UI_PHONE_MAIN, DIALOG_STYLE_LIST, title, phoneMenu, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_CLOSE);
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
    UiDialog_Show(playerid, DIALOG_UI_PHONE_MAIN + 3, DIALOG_STYLE_MSGBOX, "Kontakte", list, UI_DIALOG_BUTTON_BACK, UI_DIALOG_BUTTON_CLOSE);
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
    UiDialog_Show(playerid, DIALOG_UI_PHONE_MAIN + 3, DIALOG_STYLE_MSGBOX, "Nachrichten", list, UI_DIALOG_BUTTON_BACK, UI_DIALOG_BUTTON_CLOSE);
}

// ---------------------------------------------------------------- Business

stock UiHub_ShowBusinessList(playerid)
{
    if (!Core_IsValidPlayerId(playerid))
    {
        return 0;
    }

    new list[1536];
    format(list, sizeof(list), "Business\tStaat\tWert\tStatus\n");
    gUiPickCount[playerid] = 0;

    for (new b = 0; b < MAX_BUSINESSES && gUiPickCount[playerid] < UI_MAX_PICK; b++)
    {
        if (b < 0 || b >= MAX_BUSINESSES || gBizActive[b] != 1 || !Core_IsValidState(gBizState[b]))
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

    if (gUiPickCount[playerid] == 0)
    {
        UiHub_ShowText(playerid, "Business-Verwaltung", "Aktuell sind keine aktiven Businesses verfügbar.");
        return 0;
    }

    new const bizCaption[] = "Business-Verwaltung";
    new const bizButton[] = "Öffnen";
    UiDialog_Show(playerid, DIALOG_UI_BIZ_LIST, DIALOG_STYLE_TABLIST_HEADERS, bizCaption, list, bizButton, UI_DIALOG_BUTTON_CLOSE);
    return 1;
}

stock UiHub_ShowBusinessAction(playerid, bizid)
{
    if (!Core_IsValidPlayerId(playerid) || bizid < 0 || bizid >= MAX_BUSINESSES || gBizActive[bizid] != 1 || !Core_IsValidState(gBizState[bizid]))
    {
        UiHub_ShowBusinessList(playerid);
        return 0;
    }

    new title[48];
    format(title, sizeof(title), "%s", gBizName[bizid]);
    if (gBizOwner[bizid] == -1)
    {
        new const bizInfoBuy[] = "Informationen\nKaufen";
        UiDialog_Show(playerid, DIALOG_UI_BIZ_ACTION, DIALOG_STYLE_LIST, title, bizInfoBuy, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
    }
    else if (gBizOwner[bizid] == playerid)
    {
        new const bizInfoPayout[] = "Informationen\nEinkommen auszahlen";
        UiDialog_Show(playerid, DIALOG_UI_BIZ_ACTION, DIALOG_STYLE_LIST, title, bizInfoPayout, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
    }
    else
    {
        new const bizInfo[] = "Informationen";
        UiDialog_Show(playerid, DIALOG_UI_BIZ_ACTION, DIALOG_STYLE_LIST, title, bizInfo, UI_DIALOG_BUTTON_OK, UI_DIALOG_BUTTON_BACK);
    }
    return 1;
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
    new const ammuCaption[] = "Ammu-Nation";
    new const ammuButton[] = "Kaufen";
    UiDialog_Show(playerid, DIALOG_UI_AMMU_LIST, DIALOG_STYLE_TABLIST_HEADERS, ammuCaption, list, ammuButton, UI_DIALOG_BUTTON_CLOSE);
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
    new const missionCaption[] = "Mission-Board";
    new const missionButton[] = "Annehmen";
    UiDialog_Show(playerid, DIALOG_UI_MISSION_LIST, DIALOG_STYLE_TABLIST_HEADERS, missionCaption, list, missionButton, UI_DIALOG_BUTTON_CLOSE);
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
    new const garageCaption[] = "Garagen";
    new const garageButton[] = "Öffnen";
    UiDialog_Show(playerid, DIALOG_UI_GARAGE_LIST, DIALOG_STYLE_TABLIST_HEADERS, garageCaption, list, garageButton, UI_DIALOG_BUTTON_CLOSE);
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
    new const tuningCaption[] = "Fahrzeug-Tuning";
    new const tuningButton[] = "Einbauen";
    UiDialog_Show(playerid, DIALOG_UI_TUNING_LIST, DIALOG_STYLE_TABLIST_HEADERS, tuningCaption, list, tuningButton, UI_DIALOG_BUTTON_CLOSE);
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
    if (gPlayerLoginState[playerid] != LOGIN_STATE_ACTIVE && gPlayerLoginState[playerid] != LOGIN_STATE_NONE)
    {
        // Auth flows must always process their own dialogs regardless of general UI readiness.
    }
    else if (!UiHub_Ready(playerid))
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
                case 0:
                {
                    UiHub_ShowStatePick(playerid);
                }
                case 1: UiHub_ShowInventory(playerid);
                case 2: UiHub_ShowPhone(playerid);
                case 3: UiHub_ShowBusinessList(playerid);
                case 4: UiHub_ShowAmmu(playerid);
                case 5: UiHub_ShowMissions(playerid);
                case 6: UiHub_ShowGarages(playerid);
                case 7: UiHub_ShowVehicleMenu(playerid);
                case 8: UiHub_ShowVehicleStatus(playerid);
                case 9: UiHub_ShowTuning(playerid);
                case 10: UiHub_ShowJobMenu(playerid);
                case 11: JobCenter_ShowMain(playerid);
                case 12: TextWars_ShowMain(playerid);
                case 13: StateControl_ShowMain(playerid);
                case 14: RoleplayUI_ShowDashboard(playerid);
                case 15: UiHub_ShowQuickActionMenu(playerid);
                case 16: CameraSystem_StartFreeCamera(playerid);
                case 17: CameraSystem_StartDroneCamera(playerid);
                case 18:
                {
                    CameraSystem_ExitCameraMode(playerid);
                    CameraSystem_Reset(playerid);
                    SetCameraBehindPlayer(playerid);
                    TogglePlayerControllable(playerid, true);
                }
            }
        }
        case DIALOG_UI_STATE_PICK:
        {
            if (!response)
            {
                if (!LoginSystem_IsActive(playerid))
                {
                    UiHub_ShowStatePick(playerid);
                    return true;
                }
                UiHub_ShowMain(playerid);
                return true;
            }

            if (listitem < 0 || listitem >= STATE_COUNT)
            {
                return true;
            }

            if (!LoginSystem_IsActive(playerid))
            {
                LoginSystem_SelectHomeState(playerid, listitem);
                LoginSystem_CompleteReg(playerid);
                return true;
            }

            if (!Core_IsValidState(listitem) || !StateSystem_IsValidState(listitem))
            {
                return true;
            }

            Core_SetPlayerState(playerid, listitem);
            Core_SetPlayerHomeState(playerid, listitem);
            DB_SetPlayerState(playerid, listitem);
            DB_SavePlayerState(playerid, listitem);
            LoginSystem_ShowSkinPicker(playerid);
            CameraSystem_Reset(playerid);
            SendClientMessage(playerid, 0x00FF99AA, "[UI] Stadt gewählt. Jetzt wird der Skin gesetzt, bevor der finale Spawn erfolgt.");
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
                case 3: UiDialog_Show(playerid, DIALOG_UI_INV_GIVE_TARGET, DIALOG_STYLE_INPUT, "Übertragen", "Spieler-ID oder Name (in deiner Nähe):", "Weiter", UI_DIALOG_BUTTON_BACK);
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
                case 1: UiDialog_Show(playerid, DIALOG_UI_PHONE_ADD, DIALOG_STYLE_INPUT, "Kontakt hinzufügen", "Name des Kontakts:", "Speichern", UI_DIALOG_BUTTON_BACK);
                case 2: UiDialog_Show(playerid, DIALOG_UI_PHONE_SMS, DIALOG_STYLE_INPUT, "SMS verfassen", "Nachricht (max. 63 Zeichen):", "Senden", UI_DIALOG_BUTTON_BACK);
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
            UiDialog_Show(playerid, DIALOG_UI_AMMU_CONFIRM, DIALOG_STYLE_MSGBOX, "Kauf bestätigen", text, "Kaufen", UI_DIALOG_BUTTON_BACK);
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
            UiDialog_Show(playerid, DIALOG_UI_MISSION_CONFIRM, DIALOG_STYLE_MSGBOX, "Mission annehmen", text, "Annehmen", UI_DIALOG_BUTTON_BACK);
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
            UiDialog_Show(playerid, DIALOG_UI_GARAGE_ACTION, DIALOG_STYLE_LIST, gGarageName[gUiSelected[playerid]],
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
            UiDialog_Show(playerid, DIALOG_UI_TUNING_CONFIRM, DIALOG_STYLE_MSGBOX, "Tuning bestätigen", text, "Einbauen", UI_DIALOG_BUTTON_BACK);
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
        case DIALOG_UI_VEHICLE_STATUS:
        {
            if (response)
            {
                UiHub_ShowMain(playerid);
            }
        }
        case DIALOG_UI_VEHICLE_MENU:
        {
            if (!response)
            {
                UiHub_ShowMain(playerid);
                return true;
            }
            switch (listitem)
            {
                case 0: UiHub_ShowVehicleStatus(playerid);
                case 1: UiHub_ShowGarages(playerid);
                case 2: UiHub_ShowTuning(playerid);
                case 3: UiHub_ShowVehicleStatus(playerid);
                case 4:
                {
                    SendClientMessage(playerid, 0x00FF99AA, "[VEH] Fahrzeugaktionen über den Garagen- und Tuning-Flow laufen. Nutze das Fahrzeug-Menü für Status, Garage und Tuning.");
                    UiHub_ShowVehicleMenu(playerid);
                }
                case 5:
                {
                    SendClientMessage(playerid, 0x00FF99AA, "[VEH] Spritpreise und Fahrzeugstatus sind im Status-Dialog verfügbar.");
                    UiHub_ShowVehicleStatus(playerid);
                }
            }
        }
        case DIALOG_UI_JOB_MENU:
        {
            if (!response)
            {
                UiHub_ShowMain(playerid);
                return true;
            }
            switch (listitem)
            {
                case 0: JobCenter_ShowMain(playerid);
                case 1: JobCenter_StartTask(playerid);
                case 2: JobCenter_CancelTask(playerid);
                case 3: JobCenter_ShowStatus(playerid);
                case 4:
                {
                    UiDialog_Show(playerid, DIALOG_JOB_QUIT, DIALOG_STYLE_MSGBOX, "Job kündigen", "Willst du deinen aktuellen Job wirklich kündigen?", "Kündigen", "Abbrechen");
                }
            }
        }
        case DIALOG_UI_STATE_MENU:
        {
            if (!response)
            {
                UiHub_ShowMain(playerid);
                return true;
            }
            switch (listitem)
            {
                case 0: UiHub_ShowStatePick(playerid);
                case 1: GovernmentSystem_ShowStateInfo(playerid, StateSystem_GetPlayerState(playerid));
                case 2: StateServices_Show(playerid);
                case 3: StateBudgetSystem_ShowOverview(playerid, StateSystem_GetPlayerState(playerid));
                case 4: LawSystem_ShowWanted(playerid);
                case 5: CityEconomy_ShowOverview(playerid);
                case 6: ShopSystem_ShowStateShops(playerid, StateSystem_GetPlayerState(playerid));
                case 7: UiHub_ShowJobMenu(playerid);
                case 8: UiHub_ShowVehicleMenu(playerid);
                case 9: ProgressionSystem_ShowStats(playerid);
                case 10: CameraSystem_StartFreeCamera(playerid);
                case 11: CameraSystem_StartDroneCamera(playerid);
                case 12:
                {
                    CameraSystem_ExitCameraMode(playerid);
                    CameraSystem_Reset(playerid);
                    SetCameraBehindPlayer(playerid);
                    TogglePlayerControllable(playerid, true);
                }
            }
        }
        case DIALOG_UI_AUTH_MAIN:
        {
            if (!response)
            {
                UiHub_ShowAuthMenu(playerid);
                return true;
            }
            switch (listitem)
            {
                case 0:
                {
                    gPlayerLoginState[playerid] = LOGIN_STATE_LOGIN;
                    UiHub_ShowPasswordInput(playerid);
                }
                case 1:
                {
                    gPlayerLoginState[playerid] = LOGIN_STATE_REGISTER;
                    UiHub_ShowPasswordInput(playerid);
                }
                case 2:
                {
                    gPlayerLoginState[playerid] = LOGIN_STATE_LOGIN;
                    UiHub_ShowPasswordInput(playerid);
                }
                case 3:
                {
                    UiHub_ShowStatePick(playerid);
                }
                case 4:
                {
                    CameraSystem_Reset(playerid);
                    TutorialSystem_Start(playerid);
                }
            }
        }
        case DIALOG_UI_TUTORIAL:
        {
            if (gTutorialStage[playerid] > TUTORIAL_STAGE_NONE && gTutorialStage[playerid] < TUTORIAL_STAGE_FINISH)
            {
                TutorialSystem_Next(playerid);
            }
        }
        case DIALOG_UI_AUTH_PASSWORD:
        {
            if (!response)
            {
                // Abbruch im Auth-Flow -> Passwort bleibt Pflicht, erneut anzeigen.
                UiHub_ShowPasswordInput(playerid);
                return true;
            }
            if (strlen(inputtext) <= 0)
            {
                SendClientMessage(playerid, 0xFF6666AA, "[AUTH] Passwort darf nicht leer sein.");
                UiHub_ShowPasswordInput(playerid);
                return true;
            }
            if (!UiDialog_IsSafeText(inputtext))
            {
                SendClientMessage(playerid, 0xFF6666AA, "[AUTH] Passwort enthält ungültige Zeichen.");
                UiHub_ShowPasswordInput(playerid);
                return true;
            }

            // Zentrale Verarbeitung: Register speichert Passwort, Login prueft gegen SQLite.
            LoginSystem_SubmitPassword(playerid, inputtext);
        }
        case DIALOG_UI_GENDER_PICK:
        {
            if (!response)
            {
                LoginSystem_RequireGenderSelection(playerid);
                return true;
            }

            if (listitem == 0)
            {
                LoginSystem_SetPlayerGender(playerid, GENDER_MALE);
            }
            else if (listitem == 1)
            {
                LoginSystem_SetPlayerGender(playerid, GENDER_FEMALE);
            }
            else
            {
                return true;
            }

            if (gPlayerLoginState[playerid] == LOGIN_STATE_REGISTER)
            {
                LoginSystem_CompleteReg(playerid);
                return true;
            }

            LoginSystem_ShowSkinPicker(playerid);
        }
        case DIALOG_UI_SKIN_PICK:
        {
            if (!response)
            {
                if (gPlayerLoginState[playerid] == LOGIN_STATE_REGISTER)
                {
                    LoginSystem_ShowSkinPicker(playerid);
                }
                return true;
            }

            if (listitem < 0)
            {
                return true;
            }

            new gender = Core_GetPlayerGender(playerid);
            new skinIndex = listitem;
            new selectedSkin = 0;
            if (gender == GENDER_MALE)
            {
                new const malePool[15] = {2, 4, 7, 23, 26, 30, 45, 73, 78, 96, 102, 104, 109, 124, 130};
                if (skinIndex >= 0 && skinIndex < sizeof(malePool))
                {
                    selectedSkin = malePool[skinIndex];
                }
            }
            else if (gender == GENDER_FEMALE)
            {
                new const femalePool[15] = {9, 10, 11, 12, 14, 15, 16, 17, 18, 20, 21, 22, 25, 27, 29};
                if (skinIndex >= 0 && skinIndex < sizeof(femalePool))
                {
                    selectedSkin = femalePool[skinIndex];
                }
            }
            else
            {
                new const fallbackPool[15] = {2, 9, 14, 15, 23, 26, 29, 30, 45, 73, 78, 96, 104, 130, 212};
                if (skinIndex >= 0 && skinIndex < sizeof(fallbackPool))
                {
                    selectedSkin = fallbackPool[skinIndex];
                }
            }

            if (selectedSkin < 0 || selectedSkin > 311)
            {
                selectedSkin = (gender == GENDER_FEMALE ? 9 : 2);
            }

            LoginSystem_ApplySkinChoice(playerid, selectedSkin);
            if (gPlayerLoginState[playerid] == LOGIN_STATE_REGISTER)
            {
                LoginSystem_CompleteReg(playerid);
            }
            else
            {
                LoginSystem_CompleteLogin(playerid);
            }
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
        if (!UiHub_Ready(playerid))
        {
            return true;
        }
        UiHub_ShowMain(playerid);
        return true;
    }
    if (strcmp(cmdtext, "/help", true) == 0)
    {
        SendClientMessage(playerid, 0x33CCFFAA, "[HELP] /menu /hud /jobs /garage /vehicle /money /state /wanted /crime /dispatch /acceptcall /me /do /report");
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowMain(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/hud", true) == 0 || strcmp(cmdtext, "/dashboard", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            RoleplayUI_ShowDashboard(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/hudhide", true) == 0)
    {
        RoleplayUI_Hide(playerid);
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
            UiHub_ShowJobMenu(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/vehicle", true) == 0 || strcmp(cmdtext, "/veh", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowVehicleMenu(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/statemenu", true) == 0 || strcmp(cmdtext, "/leitstand", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowQuickActionMenu(playerid);
        }
        return true;
    }
    if (strcmp(cmdtext, "/quick", true) == 0 || strcmp(cmdtext, "/hub", true) == 0)
    {
        if (UiHub_Ready(playerid))
        {
            UiHub_ShowQuickActionMenu(playerid);
        }
        return true;
    }
    return false;
}

#endif
