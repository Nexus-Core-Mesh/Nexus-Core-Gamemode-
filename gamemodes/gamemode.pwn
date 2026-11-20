#include "../includes/dini.inc"
#include <a_samp>
#include <sscanf2>

#pragma warning disable 200
#pragma warning disable 201
#pragma warning disable 202
#pragma warning disable 203
#pragma warning disable 204
#pragma warning disable 208
#pragma warning disable 209
#pragma warning disable 213
#pragma warning disable 215
#pragma warning disable 217
#pragma warning disable 225
#pragma warning disable 234
#pragma warning disable 239

#include "modules/core/core.inc"
#include "modules/core/production_guard.inc"
#include "modules/core/runtime_registry.inc"
#include "modules/core/module_contract.inc"
#include "modules/ui/ui_dialog_ids.inc"
#include "modules/db/db_interface.inc"
#include "modules/admin/admin_system.inc"
#include "modules/admin/state_admin_system.inc"
#include "modules/admin/admin_mapping_system.inc"
#include "modules/state/state_system.inc"
#include "modules/state/state_identity_system.inc"
#include "modules/state/state_services.inc"
#include "modules/state/state_spawn_system.inc"
#include "modules/management/municipal_system.inc"
#include "modules/management/airport_system.inc"
#include "modules/state/district_system.inc"
#include "modules/economy/state_bank_system.inc"
#include "modules/economy/state_tax_system.inc"
#include "modules/economy/state_budget_system.inc"
#include "modules/economy/market_system.inc"
#include "modules/emergency/dispatch_system.inc"
#include "modules/housing/house_storage_system.inc"
#include "modules/jobs/jobs_core.inc"
#include "modules/jobs/state_jobs.inc"
#include "modules/auth/login_system.inc"
#include "modules/camera/camera_system.inc"
#include "modules/tutorial/tutorial_system.inc"
#include "modules/government/government_system.inc"
#include "modules/housing/house_system.inc"
#include "modules/business/business_system.inc"
#include "modules/factions/faction_system.inc"
#include "modules/factions/gangwar_system.inc"
#include "modules/factions/gang_fight_system.inc"
#include "modules/vehicles/vehicle_system.inc"
#include "modules/progression/progression_system.inc"
#include "modules/admin/admin_config_system.inc"
#include "modules/jobs/dynamic_job_system.inc"
#include "modules/law/law_system.inc"
#include "modules/law/jail_system.inc"
#include "modules/crime/crime_system.inc"
#include "modules/transport/transport_system.inc"
#include "modules/economy/city_economy_system.inc"
#include "modules/shops/shop_system.inc"
#include "modules/shops/weapon_shop_system.inc"
#include "modules/vehicles/garage_system.inc"
#include "modules/emergency/emergency_system.inc"
#include "modules/events/state_event_system.inc"
#include "modules/social/social_system.inc"
#include "modules/drugs/drug_system.inc"
#include "modules/missions/mission_system.inc"
#include "modules/ui/roleplay_ui_system.inc"
#include "modules/justice/prison_system.inc"
#include "modules/justice/court_system.inc"
#include "modules/logistics/trade_logistics.inc"
#include "modules/communication/phone_system.inc"
#include "modules/marketing/ad_system.inc"
#include "modules/medical/ems_hospital.inc"
#include "modules/inventory/inventory_system.inc"
#include "modules/housing/accessory_system.inc"
#include "modules/economy/dealership_system.inc"
#include "modules/vehicles/tuning_system.inc"
#include "modules/vehicles/ubb_vehicle_system.inc"
#include "modules/vehicles/fuel_station_system.inc"
#include "modules/web/web_bridge.inc"
#include "modules/events/event_system.inc"
#include "modules/leisure/gym_system.inc"
#include "modules/leisure/fishing_system.inc"
#include "modules/leisure/casino_system.inc"
#include "modules/government/city_hall_system.inc"
#include "modules/economy/ammu_system.inc"
#include "modules/world/landmarks_system.inc"
#include "modules/world/territory_system.inc"
#include "modules/world/district_economy.inc"
#include "modules/workers/transport_workers.inc"
#include "modules/utility/mission_board.inc"
#include "modules/production/factory_network.inc"
#include "modules/commerce/marketplace_system.inc"
#include "modules/government/state_governance.inc"
#include "modules/economy/contraband_system.inc"
#include "modules/transport/taxi_system.inc"
#include "modules/social/roleplay_command_system.inc"
#include "modules/social/relationship_system.inc"
#include "modules/management/state_ledger_system.inc"
#include "modules/jobs/job_center.inc"
#include "modules/factions/text_wars.inc"
#include "modules/ui/state_control_ui.inc"
#include "modules/ui/ui_hub.inc"

#define SERVER_NAME "GTA:SA Reallife Modular"

public OnGameModeInit()
{
    print("==================================================");
    print("~w~ " SERVER_NAME " initialisiert");
    print("==================================================");

    Core_SetLifecycleStage(CORE_LIFECYCLE_BOOT);
    Core_RegisterModuleContract("core");
    Core_RegisterModuleContract("db");
    Core_RegisterModuleContract("state");
    Core_RegisterModuleContract("admin");
    Core_RegisterModuleContract("security");
    Core_RegisterModuleContract("auth");
    Core_RegisterModuleContract("economy");
    Core_RegisterModuleContract("jobs");
    Core_RegisterModuleContract("housing");
    Core_RegisterModuleContract("vehicles");
    Core_RegisterModuleContract("factions");
    Core_RegisterModuleContract("government");
    Core_RegisterModuleContract("world");
    Core_RegisterModuleContract("social");
    Core_RegisterModuleContract("web");
    Core_RegisterModuleContract("ui", CORE_LIFECYCLE_RUNTIME_READY);

    Core_RegisterDefaultRuntimeModules();
    Core_AssertLifecycleStage(CORE_LIFECYCLE_BOOT);
    Core_PrintRuntimeRegistry();

    InitCore();
    DB_Initialize();
    Core_SetLifecycleStage(CORE_LIFECYCLE_DB_READY);
    DB_EnsureInventorySchema();
    DB_EnsureContactSchema();
    DB_EnsureCriminalRecordSchema();
    DB_EnsureVehicleModsSchema();
    DB_EnsureWebAccountSchema();
    DB_EnsureMapSchema();
    StateSystem_Initialize();
    StateIdentitySystem_Initialize();
    StateSpawnSystem_Initialize();
    StateBudgetSystem_Initialize();
    DistrictSystem_Initialize();
    StateServices_Initialize();
    MunicipalSystem_Initialize();
    AirportSystem_Initialize();
    StateTaxSystem_Initialize();
    MarketSystem_Initialize();
    DispatchSystem_Initialize();
    StateBank_Initialize();
    Core_SetLifecycleStage(CORE_LIFECYCLE_STATE_READY);
    AdminSystem_Initialize();
    StateAdminSystem_Initialize();
    AdminMappingSystem_Initialize();
    JobsCore_Initialize();
    StateJobs_Initialize();
    LoginSystem_Initialize();
    Core_SetLifecycleStage(CORE_LIFECYCLE_AUTH_READY);
    CameraSystem_Initialize();
    TutorialSystem_Initialize();
    GovernmentSystem_Initialize();
    HouseSystem_Initialize();
    BusinessSystem_Initialize();
    FactionSystem_Initialize();
    GangWarSystem_Initialize();
    GangFightSystem_Initialize();
    VehicleSystem_Initialize();
    ProgressionSystem_Initialize();
    AdminConfigSystem_Initialize();
    DynamicJobSystem_Initialize();
    LawSystem_Initialize();
    JailSystem_Initialize();
    EmergencySystem_Initialize();
    CrimeSystem_Initialize();
    TransportSystem_Initialize();
    CityEconomy_Initialize();
    ShopSystem_Initialize();
    PrisonSystem_Initialize();
    CourtSystem_Initialize();
    TradeLogistics_Initialize();
    PhoneSystem_Initialize();
    AdSystem_Initialize();
    EmsHospital_Initialize();
    InventorySystem_Initialize();
    HouseStorageSystem_Initialize();
    AccessorySystem_Initialize();
    WeaponShopSystem_Initialize();
    GarageSystem_Initialize();
    DealershipSystem_Initialize();
    TuningSystem_Initialize();
    UBBVehicleSystem_Initialize();
    FuelStationSystem_Initialize();
    WebBridge_Initialize();
    EventSystem_Initialize();
    GymSystem_Initialize();
    FishingSystem_Initialize();
    CasinoSystem_Initialize();
    StateEventSystem_Initialize();
    DrugSystem_Initialize();
    MissionSystem_Initialize();
    CityHallSystem_Initialize();
    AmmuSystem_Initialize();
    LandmarksSystem_Initialize();
    TerritorySystem_Initialize();
    DistrictEconomy_Initialize();
    TransportWorkers_Initialize();
    MissionBoard_Initialize();
    FactoryNetwork_Initialize();
    MarketplaceSystem_Initialize();
    StateGovernance_Initialize();
    ContrabandSystem_Initialize();
    TaxiSystem_Initialize();
    RelationshipSystem_Initialize();
    StateLedgerSystem_Initialize();
    RoleplayUI_Initialize();
    Core_SetLifecycleStage(CORE_LIFECYCLE_WORLD_READY);
    JobCenter_Initialize();
    TextWars_Initialize();
    StateControl_Initialize();
    UiHub_Initialize();
    Core_SetLifecycleStage(CORE_LIFECYCLE_RUNTIME_READY);
    Core_PrintLifecycleReport();

    print("==================================================");
    print("~w~ " SERVER_NAME " bereit");
    print("==================================================");
    return 1;
}

public OnGameModeExit()
{
    StateBank_Shutdown();
    AdminSystem_Shutdown();
    TextWars_Shutdown();
    JobsCore_Shutdown();
    DB_Shutdown();
    return 1;
}

public OnPlayerConnect(playerid)
{
    Core_RunPlayerConnectHooks(playerid);
    CameraSystem_PlayIntroScene(playerid);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    Core_RunPlayerDisconnectHooks(playerid, reason);
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (RoleplayUI_HandleCommand(playerid, cmdtext))
        return 1;

    if (UiHub_HandleCommand(playerid, cmdtext))
        return 1;

    if (TextWars_HandleCommand(playerid, cmdtext))
        return 1;

    if (AdminSystem_HandleCommand(playerid, cmdtext))
        return 1;

    if (JailSystem_HandleCommand(playerid, cmdtext))
        return 1;

    if (EmergencySystem_HandleCommand(playerid, cmdtext))
        return 1;

    if (GarageSystem_HandleCommand(playerid, cmdtext))
        return 1;

    if (WeaponShopSystem_HandleCommand(playerid, cmdtext))
        return 1;

    if (StateEventSystem_HandleCommand(playerid, cmdtext))
        return 1;

    if (SocialSystem_HandleCommand(playerid, cmdtext))
        return 1;

    if (RoleplayCommandSystem_HandleCmd(playerid, cmdtext))
        return 1;

    if (AdminMappingSystem_HandleCmd(playerid, cmdtext))
        return 1;

    if (strcmp(cmdtext, "/tutorial", true) == 0)
    {
        TutorialSystem_Start(playerid);
        return 1;
    }
    if (strcmp(cmdtext, "/city", true) == 0)
    {
        new stateid = StateSystem_GetPlayerState(playerid);
        if (StateSystem_IsValidState(stateid))
        {
            StateSpawnSystem_Apply(playerid, stateid);
            CameraSystem_PlayStateScene(playerid, stateid);
        }
        else
        {
            LoginSystem_SetPlayerHomeState(playerid, STATE_LOS_SANTOS);
            CameraSystem_PlayStateScene(playerid, STATE_LOS_SANTOS);
        }
        return 1;
    }
    if (strcmp(cmdtext, "/gov", true) == 0)
    {
        GovernmentSystem_ShowStateInfo(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/services", true) == 0)
    {
        StateServices_Show(playerid);
        MunicipalSystem_ShowOverview(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/treasury", true) == 0)
    {
        StateBudgetSystem_ShowOverview(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/spawn", true) == 0)
    {
        StateSpawnSystem_Apply(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/state", true) == 0)
    {
        new stateid = StateSystem_GetPlayerState(playerid);
        new stateName[32];
        new leaderName[24];
        StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
        StateSystem_GetLeaderName(stateid, leaderName, sizeof(leaderName));
        new msg[256];
        format(msg, sizeof(msg), "[STATE] %s | Budget: $%d | Steuer: %d%% | Leader: %s | Services: %d", stateName, StateSystem_GetBankCash(stateid), StateSystem_GetTaxRate(stateid), leaderName, StateServices_GetCount(stateid));
        SendClientMessage(playerid, 0x00D9FFFF, msg);
        return 1;
    }
    if (strcmp(cmdtext, "/municipal", true) == 0)
    {
        MunicipalSystem_ShowOverview(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/stateinfo", true) == 0)
    {
        new stateid = StateSystem_GetPlayerState(playerid);
        new stateName[32];
        new leaderName[24];
        StateSystem_GetStateName(stateid, stateName, sizeof(stateName));
        StateSystem_GetLeaderName(stateid, leaderName, sizeof(leaderName));
        new msg[192];
        format(msg, sizeof(msg), "[STATE] %s | Budget: $%d | Steuer: %d%% | Leader: %s | Services: %d", stateName, StateSystem_GetBankCash(stateid), StateSystem_GetTaxRate(stateid), leaderName, StateServices_GetCount(stateid));
        SendClientMessage(playerid, 0x00D9FFFF, msg);
        return 1;
    }
    if (strcmp(cmdtext, "/districts", true) == 0)
    {
        DistrictSystem_ShowDistricts(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/workers", true) == 0)
    {
        TransportWorkers_ShowWorkers(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/factories", true) == 0)
    {
        FactoryNetwork_ShowFactories(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/marketplace", true) == 0)
    {
        MarketplaceSystem_Show(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/governance", true) == 0)
    {
        StateGovernance_ShowOverview(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/contraband", true) == 0)
    {
        ContrabandSystem_Show(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/taxi", true) == 0)
    {
        TaxiSystem_ShowNetwork(playerid);
        return 1;
    }
    if (strcmp(cmdtext, "/district_economy", true) == 0)
    {
        DistrictEconomy_Show(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/tax", true) == 0)
    {
        StateTaxSystem_ShowSummary(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/market", true) == 0)
    {
        MarketSystem_ShowMarket(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/dispatch", true) == 0)
    {
        DispatchSystem_ShowQueue(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    if (strcmp(cmdtext, "/adminconfig", true) == 0)
    {
        if (AdminSystem_IsModerator(playerid))
        {
            AdminConfigSystem_ShowMenu(playerid);
            return 1;
        }
        return 1;
    }
    if (strcmp(cmdtext, "/stats", true) == 0)
    {
        ProgressionSystem_ShowStats(playerid);
        return 1;
    }
    if (strcmp(cmdtext, "/wanted", true) == 0)
    {
        LawSystem_ShowWanted(playerid);
        return 1;
    }
    if (strcmp(cmdtext, "/crime", true) == 0)
    {
        CrimeSystem_ShowCrimeInfo(playerid);
        return 1;
    }
    if (strcmp(cmdtext, "/routes", true) == 0)
    {
        TransportSystem_ShowRoutes(playerid);
        return 1;
    }
    if (strcmp(cmdtext, "/economy", true) == 0)
    {
        CityEconomy_ShowOverview(playerid);
        return 1;
    }
    if (strcmp(cmdtext, "/shops", true) == 0)
    {
        ShopSystem_ShowStateShops(playerid, StateSystem_GetPlayerState(playerid));
        return 1;
    }
    return 0;
}

public OnPlayerSpawn(playerid)
{
    StateSystem_OnPlayerSpawn(playerid);

    new stateid = Core_GetPlayerState(playerid);
    new homeState = Core_GetPlayerHomeState(playerid);
    new bool:hasCompletedOnboarding = (gTutorialStage[playerid] >= TUTORIAL_STAGE_FINISH);
    new bool:hasValidState = Core_IsValidState(stateid) && Core_IsValidState(homeState);

    if (LoginSystem_IsActive(playerid) && (!hasCompletedOnboarding || !hasValidState))
    {
        if (!hasValidState)
        {
            CameraSystem_Reset(playerid);
            StateSpawnSystem_Apply(playerid, Core_GetValidStateOrDefault(stateid, homeState));
        }
        else if (!hasCompletedOnboarding)
        {
            CameraSystem_Reset(playerid);
            StateSpawnSystem_Apply(playerid, homeState);
        }
        else
        {
            CameraSystem_Reset(playerid);
            StateSpawnSystem_Apply(playerid, homeState);
        }
        RoleplayUI_ShowDashboard(playerid);
        PhoneSystem_OnPlayerConnect(playerid);
        PrisonSystem_OnPlayerConnect(playerid);
    }
    else if (LoginSystem_IsActive(playerid))
    {
        CameraSystem_Reset(playerid);
        StateSpawnSystem_Apply(playerid, homeState);
        RoleplayUI_ShowDashboard(playerid);
        return 1;
    }
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (UiHub_OnDialogResponse(playerid, dialogid, response, listitem, inputtext))
        return 1;
    if (JobCenter_OnDialogResponse(playerid, dialogid, response, listitem, inputtext))
        return 1;
    if (StateControl_OnDialogResponse(playerid, dialogid, response, listitem, inputtext))
        return 1;
    if (TextWars_OnDialogResponse(playerid, dialogid, response, listitem, inputtext))
        return 1;
    return 0;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    return TextWars_OnClickPlayerTextDraw(playerid, playertextid) ? 1 : 0;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    TextWars_OnClickTextDraw(playerid, clickedid);
    return 0;
}

public OnPlayerEnterCheckpoint(playerid)
{
    JobCenter_OnEnterCheckpoint(playerid);
    return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
    TextWars_OnPlayerDeath(playerid, killerid);
    return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if ((newkeys & KEY_YES) && !(oldkeys & KEY_YES))
        UiHub_ShowMain(playerid);
    else if ((newkeys & KEY_NO) && !(oldkeys & KEY_NO))
        TextWars_Toggle(playerid);
    return 1;
}
