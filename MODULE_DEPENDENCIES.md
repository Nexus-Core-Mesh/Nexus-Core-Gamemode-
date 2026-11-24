# Modul-Abhängigkeiten und Systemverkettung

Dieses Dokument beschreibt die realen Abhängigkeiten zwischen den Modulen im aktuellen Stand von `gamemodes/` und `gamemodes/gamemode.pwn`.

## 1. Grundprinzip

Das Projekt ist bewusst modular aufgebaut, aber nicht vollständig entkoppelt. Die wichtigsten Regeln sind:

- `core/core.inc` ist die zentrale Quelle für Spielerdaten, State-IDs und Admin-Level.
- `db/db_interface.inc` ist der zentrale Persistenz-Entry-Point.
- `state/state_system.inc` ist die Quelle für die drei Staaten und deren Kernwerte.
- Die Initialisierung läuft in einer festen Reihenfolge durch `OnGameModeInit()`.
- Viele Systeme lesen Daten aus `Core_*`-Funktionen statt eigene parallele Spieler-Status-Quellen zu pflegen.

## 2. Initialisierungsreihenfolge

Die Reihenfolge in `gamemodes/gamemode.pwn` zeigt die reale Architektur:

1. `Core_*`-Initialisierung
2. `DB_Initialize()`
3. `StateSystem_Initialize()`
4. `StateIdentitySystem_Initialize()`
5. `StateSpawnSystem_Initialize()`
6. `StateBudgetSystem_Initialize()`
7. `DistrictSystem_Initialize()`
8. `StateServices_Initialize()`
9. `MunicipalSystem_Initialize()`
10. `AirportSystem_Initialize()`
11. `StateTaxSystem_Initialize()`
12. `MarketSystem_Initialize()`
13. `DispatchSystem_Initialize()`
14. `StateBank_Initialize()`
15. `AdminSystem_Initialize()`
16. `StateAdminSystem_Initialize()`
17. `AdminMappingSystem_Initialize()`
18. `JobsCore_Initialize()`
19. `StateJobs_Initialize()`
20. `LoginSystem_Initialize()`
21. `CameraSystem_Initialize()`
22. `TutorialSystem_Initialize()`
23. `GovernmentSystem_Initialize()`
24. `HouseSystem_Initialize()`
25. `BusinessSystem_Initialize()`
26. `FactionSystem_Initialize()`
27. `GangWarSystem_Initialize()`
28. `GangFightSystem_Initialize()`
29. `VehicleSystem_Initialize()`
30. `ProgressionSystem_Initialize()`
31. `AdminConfigSystem_Initialize()`
32. `DynamicJobSystem_Initialize()`
33. `LawSystem_Initialize()`
34. `JailSystem_Initialize()`
35. `EmergencySystem_Initialize()`
36. `CrimeSystem_Initialize()`
37. `TransportSystem_Initialize()`
38. `CityEconomy_Initialize()`
39. `ShopSystem_Initialize()`
40. `PrisonSystem_Initialize()`
41. `CourtSystem_Initialize()`
42. `TradeLogistics_Initialize()`
43. `PhoneSystem_Initialize()`
44. `AdSystem_Initialize()`
45. `EmsHospital_Initialize()`
46. `InventorySystem_Initialize()`
47. `HouseStorageSystem_Initialize()`
48. `AccessorySystem_Initialize()`
49. `WeaponShopSystem_Initialize()`
50. `GarageSystem_Initialize()`
51. `DealershipSystem_Initialize()`
52. `TuningSystem_Initialize()`
53. `UBBVehicleSystem_Initialize()`
54. `FuelStationSystem_Initialize()`
55. `WebBridge_Initialize()`
56. `EventSystem_Initialize()`
57. `GymSystem_Initialize()`
58. `FishingSystem_Initialize()`
59. `CasinoSystem_Initialize()`
60. `StateEventSystem_Initialize()`
61. `SeasonSystem_Initialize()`
62. `DrugSystem_Initialize()`
63. `MissionSystem_Initialize()`
64. `CityHallSystem_Initialize()`
65. `AmmuSystem_Initialize()`
66. `LandmarksSystem_Initialize()`
67. `TerritorySystem_Initialize()`
68. `DistrictEconomy_Initialize()`
69. `TransportWorkers_Initialize()`
70. `MissionBoard_Initialize()`
71. `FactoryNetwork_Initialize()`
72. `MarketplaceSystem_Initialize()`
73. `StateGovernance_Initialize()`
74. `ContrabandSystem_Initialize()`
75. `TaxiSystem_Initialize()`
76. `RelationshipSystem_Initialize()`
77. `StateLedgerSystem_Initialize()`
78. `RoleplayUI_Initialize()`
79. `JobCenter_Initialize()`
80. `TextWars_Initialize()`
81. `StateControl_Initialize()`
82. `PremiumSystem_Initialize()`
83. `UiHub_Initialize()`

## 3. Fachliche Abhängigkeiten

### Core -> State

`core/core.inc` definiert:

- `STATE_LOS_SANTOS`, `STATE_SAN_FIERRO`, `STATE_LAS_VENTURAS`
- gültige State-Grenzen
- globale `gPlayerData`-Struktur
- Validierung von `state_id`, `home_state`, `admin_level`

`state/state_system.inc` nutzt diese Werte und schreibt State-Kassen, Steuern, Spawns und Leader-Werte.

Die Abhängigkeit ist deshalb nicht nur funktional, sondern architektonisch: der State-Layer ist auf den Core-Layer angewiesen.

### Core -> DB

`db/db_interface.inc` enthält Runtime-Caches wie:

- `gDBPlayerStateCache`
- `gDBPlayerHomeStateCache`
- `gDBPlayerBankCache`
- `gDBPlayerCashCache`
- Inventar- und Storage-Caches

Das DB-Layer ist zwar eigenständig, aber es rehydratiert und synchronisiert Spielerdaten mit dem Core-Status. Das ist eine definierte Abhängigkeit, kein unabhängiger Parallel-Stack.

### State -> Economy

`state_system.inc` stellt Kassen und Steuerwerte bereit.

`state_budget_system.inc`, `state_tax_system.inc`, `state_bank_system.inc`, `market_system.inc`, `city_economy_system.inc` lesen bzw. verändern diese Werte.

Die Wirtschafts-Module sind also von der State-Quelle abhängig, auch wenn sie teilweise ihre eigenen Arrays verwenden.

### Economy -> Jobs / World / Factions

Wirtschafts- und Verwaltungsmodelle beeinflussen:

- Löhne in `jobs/jobs_core.inc`
- Staatsbeiträge in `business/business_system.inc`
- Tax-Revenue in `state_tax_system.inc`
- Einflusszonen in `world/territory_system.inc`
- Gangkriege in `factions/gangwar_system.inc` und `factions/text_wars.inc`

### UI -> Module

`ui/ui_hub.inc` dient als Orchestrator für Menü- und Dialog-Interfaces.

Es zeigt Daten aus vielen Quellen an:

- Inventar
- Handy/Telefon
- Business
- Fahrzeug/Garage
- Job-Center
- Ammu-Nation
- Massenaktionen

Es ist damit das zentrale UI-Frontend, aber kein eigener Speicher- oder Game-Logic-Layer.

## 4. Modul-Kategorien und direkte Verknüpfungen

### 4.1 Auth / Login / State

- `auth/login_system.inc`
- `camera/camera_system.inc`
- `tutorial/tutorial_system.inc`
- `state/state_system.inc`
- `state/state_spawn_system.inc`

Verknüpfung: Login -> Home-State -> Spawn -> Camera -> UI-Dashboard

### 4.2 Admin / State-Admin / World

- `admin/admin_system.inc`
- `admin/state_admin_system.inc`
- `admin/admin_mapping_system.inc`
- `admin/admin_config_system.inc`
- `ui/state_control_ui.inc`

Verknüpfung: Admin-Rang -> Rechteprüfung -> State-UI -> World-Operationen

### 4.3 Jobs / Mission / Progression

- `jobs/jobs_core.inc`
- `jobs/state_jobs.inc`
- `jobs/dynamic_job_system.inc`
- `jobs/job_center.inc`
- `missions/mission_system.inc`
- `progression/progression_system.inc`

Verknüpfung: Job-Auswahl -> Mission-Flow -> Level/Fortschritt -> Bank-/State-Lohnzahlungen

### 4.4 Economy / Business / Shops

- `economy/state_bank_system.inc`
- `economy/state_budget_system.inc`
- `economy/market_system.inc`
- `economy/city_economy_system.inc`
- `business/business_system.inc`
- `shops/shop_system.inc`
- `shops/weapon_shop_system.inc`

Verknüpfung: Staatliche Kasse -> Wirtschaft -> Shop-/Business-Aktionen -> Spieler-Bank

### 4.5 Housing / Inventory / Vehicles

- `housing/house_system.inc`
- `housing/house_storage_system.inc`
- `inventory/inventory_system.inc`
- `vehicles/vehicle_system.inc`
- `vehicles/garage_system.inc`
- `vehicles/tuning_system.inc`
- `vehicles/fuel_station_system.inc`

Verknüpfung: Besitz -> Inventar -> Fahrzeug -> Garage -> Tuning -> Kraftstoff

### 4.6 Law / Crime / Emergency

- `law/law_system.inc`
- `law/jail_system.inc`
- `crime/crime_system.inc`
- `emergency/emergency_system.inc`
- `emergency/dispatch_system.inc`
- `justice/prison_system.inc`
- `justice/court_system.inc`

Verknüpfung: Wanted / Crime -> Polizei / EMS -> Dispatch -> Jail / Court

### 4.7 Faction / War / Territory

- `factions/faction_system.inc`
- `factions/gangwar_system.inc`
- `factions/gang_fight_system.inc`
- `factions/text_wars.inc`
- `world/territory_system.inc`
- `world/district_economy.inc`

Verknüpfung: Fraktion -> Gebiet -> Krieg -> UI-Widget -> Status-Updates

## 5. Abhängigkeiten, die technisch relevant sind

### 5.1 Keine zweite State-Quelle

Das Projekt versucht, `core.inc` als Single Source of Truth zu etablieren. Das ist wichtig, da mehrere Module ihre eigenen lokalen Arrays haben. Der entwerfende Sinn ist klar:

- `Core_GetPlayerState()` liefert die legitimierte State-ID.
- Module, die einen State brauchen, sollten diese abfragen und keine eigene Prioritäts-Quelle setzen.

### 5.2 DB als Persistenz-Adapter

`db_interface.inc` kapselt Datenzugriff und Caches. Die echten Daten sind dabei nicht auf ein einziges Modul verteilt, sondern in mehreren Caches / Schemas organisiert.

### 5.3 UI als Orchestrator

Das UI-System ist nicht die Datenquelle. Es steuert nur die Präsentation und die Interaktion. Die eigentliche Logik sitzt in den Subsystemen.

## 6. Risikobereiche

Es gibt mehrere Bereiche, in denen die Abhängigkeit noch technisch schwach ist:

- Einige Module verfügen über `Show*`-Funktionen, aber keine echte Anbindung aus dem Gamemode-Flow.
- `RoleplayCommandSystem` und `SocialSystem` überlappen teilweise mit demselben Bereich.
- Einige Command-Handler existieren, aber der `OnPlayerCommandText`-Flow setzt sie nicht immer korrekt durch.
- Für das TextDraw-UI gibt es mehrere parallel nutzbare Draw-Mechaniken; das zeigt, dass das UI nicht komplett einheitlich abstrahiert ist.

## 7. Fazit

Das Projekt ist klar modular, aber die Abhängigkeiten sind bewusst geordnet und nicht rein isoliert. Die Kernstrukturen bilden ein funktionierendes Architekturbild:

- Core definiert die Wahrheit.
- DB versorgt und sichert.
- State bildet die politische Grundlage.
- Economy, Jobs, Factions, Housing, Vehicles und UI greifen auf diese Basis auf.

Wenn der Gamemode weiter ausgebaut wird, sollten neue Module immer in dieser Reihenfolge eingeordnet werden:

1. Core
2. DB
3. State
4. Economy / Jobs / Law
5. Housing / Inventory / Vehicles
6. UI / Social / Event / Web

Damit bleibt die Architektur stabil und gut auditierbar.


---
## Erweiterungsstand / Aktualisierung (2026-11-21)

Diese Ergänzung dient dazu, den bisherigen Dokumentationsstand auf den aktuellen Quellcode zu synchronisieren, ohne vorhandene Inhalte zu löschen oder zu überschreiben.

### Grundlage der Aktualisierung
- Basis: `gamemodes/gamemode.pwn` und `gamemodes/modules/**`
- Prüfkriterium: aktuelle Modulanbindung, Commands, UI-Initialisierung und TextDraw-Erzeugung
- Stand: source-level inspection, keine laufende Server-Validierung

### Aktueller Projektstatus
- Die Modulanbindung erfolgt über `OnGameModeInit()` und den Befehlshandler in `OnPlayerCommandText()`.
- Viele Systeme sind vorhanden und als aktive Kernmodule strukturell eingebunden.
- Einige Module und UI-Elemente sind angelegt, aber noch nicht vollständig in den aktiven Runtime-Flow eingehängt.
- Die maßgebliche Source-Quelle für alle Aussagen bleibt `gamemodes/`; `Compiler/samp03/gamemodes/` ist ein älterer Spiegelstand.

### Aktualisierungsprinzip
- Vorhandene Inhalte bleiben erhalten.
- Neue Informationen werden als Ergänzungen an das vorhandene Dokument angehängt.
- Statusangaben werden bei Bedarf als `aktiv`, `verdeckt`, `gebaut aber nicht angebunden` oder `vorbereitet` markiert.

### Querverweise zu den zentralen Dokus
- Commands: `COMMANDS.md`
- Module: `MODULES.md`
- TextDraws: `TEXTDRAWS.md`
- Moduldynamik: `MODULE_DEPENDENCIES.md`
- Admin-Architektur: `ADMIN_SYSTEMS.md`
- State-System: `STATE_SYSTEM.md`
- DB-Persistenz: `DATABASE_SCHEMA.md`
- UI-Flow: `UI_FLOW.md`
- Progression: `PLAYER_PROGRESSION.md`
- Fahrzeugsystem: `VEHICLE_SYSTEM.md`
- Wirtschaft: `ECONOMY_SYSTEMS.md`
- Faction/War: `FACTION_AND_WAR_SYSTEMS.md`
- Law/Emergency: `EMERGENCY_AND_LAW.md`
- Housing/Inventory: `HOUSING_AND_INVENTORY.md`
- Events/Season: `EVENTS_AND_SEASON.md`

### Kurzfazit
Das Projekt bleibt modular aufgebaut, mit einer klaren Trennung zwischen Core, State, Economy, Jobs, Fraktionen, UI und Persistenz. Die dokumentierte Architektur ist konsistent mit dem aktuellen Code-Stand, auch wenn einzelne Funktionen noch als vorbereitet oder nur teilweise angebunden erkennbar sind.


---

## Zusätzliche Erweiterung / Projekt-Lage (2026-11-21)

Diese Erweiterung dient dazu, den bisherigen Dokumentationsstand noch weiter auf den aktuellen Quellcode zu kalibrieren, ohne bestehende Informationen zu überschreiben.

### Was hier zusätzlich klarer wird
- Die Projektstruktur ist weiterhin als modularer SA:MP-Gamemode aufgebaut, mit klarer Trennung zwischen Core, DB, State, Economy, Jobs, Law, Faction, UI und World-Systems.
- Die wichtigsten Laufzeitpunkte bleiben `OnGameModeInit()` und `OnPlayerCommandText()`. Dadurch wird die aktive Funktionalität sauber von vorbereiteten, verdeckten oder nur im Code angelegten Systemen abgegrenzt.
- Die Dokumentation beschreibt bewusst den source-basierten Ist-Stand: Sie zeigt, was im Code aktiv, verknüpft, vorbereitet oder nur noch als Entwurf vorhanden ist.
- Viele Systeme sind bereits architektonisch sauber angelegt, aber nicht immer vollständig in den Spiel-Flow eingehängt. Das ist ein normaler Status bei modularen RP-Gamemodes in Aufbauphase.

### Wichtig für Weiterentwicklung
- Neue Funktionen sollten immer mit Status markiert werden: `aktiv`, `verdeckt`, `gebaut aber nicht angebunden`, `vorbereitet` oder `archiviert`.
- Befehls- und UI-Liste sollten mit der tatsächlichen Initialisierung im Haupt-Gamemode abgeglichen werden, damit keine falschen Annahmen über Live-Funktionalität entstehen.
- Module ohne aktive Einbindung sollten als „integrationsreif“ gekennzeichnet werden, aber nicht als fertig dokumentiert gelten, wenn sie in der aktuellen Laufzeit nicht erreichbar sind.

### Gesamtüberblick des aktuellen Stands
- Das Projekt ist bereits stark strukturiert und fachlich klar gegliedert.
- Es besitzt eine solide Kernarchitektur, eine wiederverwendbare Modullandschaft und eine klare Trennung zwischen Daten, Logik und UI.
- Die wichtigsten offenen Punkte liegen eher in der Finalisierung und Verknüpfung von einzelnen Systemen als in der Grundarchitektur selbst.
- Die Dokumente bleiben damit als echte Arbeitsbasis nutzbar: detailliert genug für Entwicklung, aber immer nachvollziehbar als Quellcode-basierte Bestandsaufnahme.

### Nächster sinnvoller Schritt
- Die nächste Stufe wäre keine weitere Neu-Dokumentation, sondern eine gezielte Runtime-Validierung: Compile, serverseitiges Testen und Abgleich mit den bereits dokumentierten aktiven Systemen.
- Danach kann man die Dokumentation mit einer „Live-Status-Checkliste“ ergänzen, die jeden Command, TextDraw und Modul nach „aktiv / verdeckt / nicht verbunden“ kategorisiert.


---

## Aktualisierter Stand (2026-11-24)

- Registrierungsflow: Passwort-Gate vor Tutorial/Spawn, M/W-Auswahl vor Skin-Picker, `DIALOG_UI_GENDER_PICK` aktiv.
- Tutorial: `TUTORIAL_SCENE_TIME` auf 11500 ms, Schutz gegen Start ohne Passwort/Geschlecht.
- Skin-Flow: `LoginSystem_ShowSkinPicker()` zeigt geschlechtsabhängige 10–15 Skins, validiert `0..311`, setzt `SetPlayerSkin` vor finalem Spawn.
- Spawn-Order: Home-State/State validiert, `StateSpawnSystem_Apply(...)` erst nach Skin-Setzung, dann `CameraSystem_ExitCameraMode`, `SetCameraBehindPlayer`, `TogglePlayerControllable(playerid, true)`, `RoleplayUI_ShowDashboard(playerid)`.
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut.
- SQLite-only + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

