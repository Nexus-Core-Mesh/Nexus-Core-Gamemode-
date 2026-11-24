# Nexus Core Gamemode – Vollständige Master-Dokumentation

Diese Datei bündelt den aktuellen Stand des Projekts in einer einzigen ausführlichen Übersicht. Sie ist eine komprimierte, aber vollständige Zusammenfassung der vorhandenen Architektur-, Modul-, Befehls-, UI- und TextDraw-Dokumentation.

Hinweis: Diese Dokumentation basiert auf dem aktuellen Quellcode der Projektstruktur `gamemodes/` und den zugehörigen Modulen. Sie beschreibt den source-basierten Ist-Stand, nicht eine Live-Server-Verifikation in einem laufenden SA:MP-Server.

---

## 1. Überblick

Das Projekt ist ein modularer GTA:SA RP-Gamemode mit klarer fachlicher Aufteilung. Die Hauptbasis liegt in:

- `gamemodes/gamemode.pwn` – Einstiegspunkt und Boot-Flow
- `gamemodes/modules/` – das eigentliche System- und Funktions-Set
- `includes/` – SA:MP-Includes, Helper und allgemeine Bibliotheken
- `Compiler/` – lokale Build- und Laufzeitumgebung
- `data/` und `scriptfiles/` – Persistenz-/Runtime-Pfade

Das allgemeine Designprinzip ist:

- modulare Aufteilung
- feste Core-Architektur
- zentrale Player- und State-Logik
- getrennte UI-, DB-, Economy-, Job-, Faction-, World- und Law-Layer
- mehrere Systeme sind bereits vorbereitet, aber nicht immer vollständig in den aktiven Spiel-Flow eingehängt

---

## 2. Kernarchitektur

### 2.1 Core-Layer

Der Core ist die Grundlage des Gamemodes. Die wichtigste Datei ist:

- `gamemodes/modules/core/core.inc`

Darin liegen zentrale Konstanten und Datenstrukturen wie:

- `STATE_LOS_SANTOS = 0`
- `STATE_SAN_FIERRO = 1`
- `STATE_LAS_VENTURAS = 2`
- `STATE_COUNT = 3`
- `MAX_INVENTORY_SLOTS = 24`
- `MAX_HOUSE_STORAGE_SLOTS = 12`
- `MAX_HOUSES = 50`
- `MAX_UBB_VEHICLES = 100`

Zusätzlich wichtig:

- `E_PLAYER_STATE`
- `E_PLAYER_HOME_STATE`
- `E_PLAYER_ADMIN_LEVEL`
- `E_PLAYER_ADMIN_STATE`
- `E_PLAYER_LEVEL`
- `E_PLAYER_XP`
- `E_PLAYER_REP`
- `E_PLAYER_BANK`
- `E_PLAYER_WANTED`
- `E_PLAYER_JAIL_ACTIVE`
- `E_PLAYER_JAIL_TIME`
- `E_PLAYER_LAW_ROLE`
- `E_PLAYER_EMERGENCY_ROLE`
- `E_PLAYER_EMERGENCY_STATE`
- `E_PLAYER_PROJECT_LEADER`
- `E_PLAYER_PREMIUM_TIER`
- `E_PLAYER_PREMIUM_EXPIRES`
- `E_PLAYER_PREMIUM_PERMANENT`

Der Core definiert die zentrale Spielerstruktur:

- `gPlayerData[MAX_PLAYERS][E_PLAYER_PROFILE_COUNT]`
- `gPlayerBank[MAX_PLAYERS]`
- `gPlayerCash[MAX_PLAYERS]`

Der Core ist somit die zentrale Quelle der Wahrheit für:

- State
- Admin-Level
- Bank/Cash
- Jail/Wanted
- Premium- und Projektstatus

### 2.2 DB-Layer

Die Persistenz liegt in:

- `gamemodes/modules/db/db_interface.inc`
- `db_sqlite.inc`
- `db_mysql.inc`

Der DB-Layer enthält:

- Caches für Spieler, Inventar, Häuser, Fahrzeuge
- Schema-Repair-Funktionen
- Transaction-Logik
- Backup-/Restore-Funktionen
- Dini- und SQLite-/MySQL-Hybridstruktur

Wichtige Backend-Definitionen:

- `DB_BACKEND_SQLITE = 0`
- `DB_BACKEND_MYSQL = 1`

Die Standardkonfiguration zeigt klar: der Gamemode ist derzeit primär auf eine flexiblere, modular-eingebaute Persistenzarchitektur ausgerichtet, wobei die Laufzeitproblematik mit Dini- und SQLite-/MySQL-Pfaden sauber getrennt wird.

### 2.3 State-Layer

Das State-System ist in:

- `gamemodes/modules/state/state_system.inc`

Darin gibt es die drei Hauptstaaten:

- Los Santos
- San Fierro
- Las Venturas

Die wichtigsten Werte:

- `STATE_LOS_SANTOS = 0`
- `STATE_SAN_FIERRO = 1`
- `STATE_LAS_VENTURAS = 2`
- `STATE_COUNT = 3`

Das System verwaltet:

- State-Kassen
- Geld-/Treasury-Status
- Steuersätze
- Infrastruktur-Revenue
- Policy- und Leader-Namen
- Spawn-Positionen
- Enabled-Flags pro Staat

Das zeigt: Das Projekt baut nicht nur eine Welt im Sinne von Map und Spawn auf, sondern eine politik- und wirtschaftsbasierte Staatsstruktur.

---

## 3. Initialisierungsreihenfolge

Die echte Reihenfolge ist im Haupt-Gamemode sichtbar. Die Initialisierung läuft über `OnGameModeInit()` und folgt einer klaren Boot-Reihenfolge. Die wichtigsten Schritte sind:

1. Core- und Runtime-Setup
2. DB-Initialisierung
3. State-Initialisierung
4. Admin-/State-Admin-Setup
5. Login-/Tutorial-/Auth-Flow
6. Jobs und Progression
7. Economy und State-Budget
8. Government, Districts, Services
9. Business, Housing, Inventory
10. Factions und Territory-System
11. Vehicles und Garages
12. Law, Jail, Emergency, Dispatch
13. UI-Hub, Text-Wars, Dashboard
14. Event-, Season- und Web-Module

Das zeigt die Architektur als mehrschichtigen Stack, nicht als ein einziges monolithisches System.

---

## 4. Modullandschaft

Das Projekt enthält sehr viele Module. Die Haupträume sind:

### 4.1 Core & Infrastruktur

- `core/core.inc`
- `core/production_guard.inc`
- `core/runtime_registry.inc`
- `core/module_contract.inc`
- `db/db_interface.inc`
- `db/db_sqlite.inc`
- `db/db_mysql.inc`

Funktion:

- zentrale Datenbank- und Runtime-Grundlagen
- Lebenszyklus- und Guards
- Sicherheitslimits
- vereinheitlichte zentrale Spielerdaten

### 4.2 Auth, Login und Tutorial

- `auth/login_system.inc`
- `tutorial/tutorial_system.inc`
- `camera/camera_system.inc`

Funktion:

- Login/Register-Flow
- Staatenauswahl
- Tutorial-Stufen
- Kamerafahrten und Intro-Erlebnisse

### 4.3 State, Government, Management

- `state/state_system.inc`
- `state/state_identity_system.inc`
- `state/state_services.inc`
- `state/state_spawn_system.inc`
- `state/district_system.inc`
- `government/government_system.inc`
- `government/city_hall_system.inc`
- `government/state_governance.inc`
- `management/municipal_system.inc`
- `management/airport_system.inc`
- `management/state_ledger_system.inc`
- `ui/state_control_ui.inc`

Funktion:

- politische Struktur
- Staatlichkeit
- Budget, Treasuries, politische Verwaltung
- lokale und übergeordnete Dienste

### 4.4 Admin-Systeme

- `admin/admin_system.inc`
- `admin/state_admin_system.inc`
- `admin/admin_config_system.inc`
- `admin/admin_mapping_system.inc`

Funktion:

- Admin-Rangmodell
- World-Management
- Teleport-/Map-/State-Tools
- Admin-Menüs und Konfigurationsui

### 4.5 Wirtschaft und Shops

- `economy/state_bank_system.inc`
- `economy/state_tax_system.inc`
- `economy/state_budget_system.inc`
- `economy/market_system.inc`
- `economy/city_economy_system.inc`
- `economy/dealership_system.inc`
- `economy/ammu_system.inc`
- `economy/contraband_system.inc`
- `commerce/marketplace_system.inc`
- `business/business_system.inc`
- `shops/shop_system.inc`
- `shops/weapon_shop_system.inc`
- `premium/premium_system.inc`
- `marketing/ad_system.inc`

Funktion:

- Staats- und Spieler-Geldfluss
- Steuern, Kassen und Budget
- Shops, Ammu, Business und Marktplätze
- Premium-/Werbe-/Einkommenssysteme

### 4.6 Jobs, Missions, Progression und Logistik

- `jobs/jobs_core.inc`
- `jobs/state_jobs.inc`
- `jobs/dynamic_job_system.inc`
- `jobs/job_center.inc`
- `missions/mission_system.inc`
- `utility/mission_board.inc`
- `production/factory_network.inc`
- `logistics/trade_logistics.inc`
- `workers/transport_workers.inc`
- `progression/progression_system.inc`

Funktion:

- Jobsystem
- Missionen
- Level-/XP-Progression
- Produktions- und Transportlogistik

### 4.7 Fahrzeuge und Transport

- `vehicles/vehicle_system.inc`
- `vehicles/garage_system.inc`
- `vehicles/tuning_system.inc`
- `vehicles/ubb_vehicle_system.inc`
- `vehicles/fuel_station_system.inc`
- `transport/transport_system.inc`
- `transport/taxi_system.inc`

Funktion:

- Fahrzeugbesitz
- Garage- und Spawn-Mechanik
- Tuning
- Tankstellen
- Transportrouten
- Taxi-/Transportlogik

### 4.8 Law, Crime, Emergency und Justice

- `law/law_system.inc`
- `law/jail_system.inc`
- `crime/crime_system.inc`
- `justice/prison_system.inc`
- `justice/court_system.inc`
- `emergency/emergency_system.inc`
- `emergency/dispatch_system.inc`
- `medical/ems_hospital.inc`
- `drugs/drug_system.inc`

Funktion:

- Wanted- und Crime-Modelle
- Police/EMS- und Dispatch-System
- Haft, Court und Straflogik
- Drogenmarkt
- Notfallmanagement

### 4.9 Fraktionen, Kriege, World und Territorien

- `factions/faction_system.inc`
- `factions/gangwar_system.inc`
- `factions/gang_fight_system.inc`
- `factions/text_wars.inc`
- `world/territory_system.inc`
- `world/landmarks_system.inc`
- `world/district_economy.inc`

Funktion:

- Fraktions- und Gangsystem
- Kriegsstatus und Gebietskontrolle
- Wettbewerbs- und territoriale Dynamik
- Text-Wars-Widget

### 4.10 Housing, Inventory, Accessoires, Social

- `housing/house_system.inc`
- `housing/house_storage_system.inc`
- `housing/accessory_system.inc`
- `inventory/inventory_system.inc`
- `communication/phone_system.inc`
- `social/social_system.inc`
- `social/roleplay_command_system.inc`
- `social/relationship_system.inc`
- `leisure/gym_system.inc`
- `leisure/fishing_system.inc`
- `leisure/casino_system.inc`

Funktion:

- Häuser und Eigentum
- Lager, Safes und Inventory
- Telefon/Kommunikation
- Social- und RP-Kommandos
- Freizeit- und Lebensstil-Features

### 4.11 Events, Season und Web

- `events/event_system.inc`
- `events/state_event_system.inc`
- `events/season_system.inc`
- `web/web_bridge.inc`

Funktion:

- temporale Weltlogik
- State-Events und Season-System
- Web-/UCP-Anbindung

### 4.12 UI und Dialoge

- `ui/ui_dialog_ids.inc`
- `ui/ui_hub.inc`
- `ui/roleplay_ui_system.inc`
- `ui/state_control_ui.inc`

Funktion:

- Hauptmenüs
- Dialog-Interfaces
- HUD-Dashboard
- State-Control- und Admin-UI

---

## 5. Commands – echte Übersicht

Die Befehle laufen über `OnPlayerCommandText()` und nicht über eine klassische `zcmd`- oder `YCMD`-Implementierung. Wichtig: Der erste passende Handler gewinnt; spätere Handler mit gleichem Namen sind daher sichtbar „verdeckt“.

### 5.1 Spieler-/UI-Commands

#### HUD / Dashboard
- `/hud` – Dashboard öffnen
- `/dashboard` – Alias/Variant
- `/hudhide` – Dashboard ausblenden

#### UI-Hub
- `/menu`
- `/m` – Alias
- `/spawn`
- `/inv`
- `/inventory`
- `/handy`
- `/phone`
- `/biz`
- `/business`
- `/ammu`
- `/weapons`
- `/missions`
- `/garage`
- `/tuning`
- `/jobs`
- `/jobcenter`
- `/vehicle`
- `/veh`
- `/statemenu`
- `/leitstand`
- `/quick`
- `/hub`

#### Text-Wars / Fraktion / War
- `/textwars`
- `/warmenu`
- `/warhud`
- `/gangwar`
- `/gangfight`
- `/territories`
- `/war`
- `/fight`
- `/factions`
- `/factioninfo`

#### Soziales / RP
- `/me`
- `/do`
- `/report`
- `/help`
- `/pm`
- `/gang`
- `/job`

#### Fahrzeuge und Shop
- `/garage`
- `/spawngarage`
- `/weapons`
- `/buyweapon`

#### Notruf / Police / EMS
- `/ems`
- `/police`
- `/stations`
- `/dispatch`
- `/acceptcall`

#### Events
- `/events`
- `/eventtoggle`

#### Tutorial
- `/tutorial`

### 5.2 Inline-Commands in `gamemode.pwn`

- `/tutorial`
- `/city`
- `/gov`
- `/services`
- `/treasury`
- `/spawn`
- `/state`
- `/stateinfo`
- `/municipal`
- `/districts`
- `/workers`
- `/factories`
- `/marketplace`
- `/governance`
- `/contraband`
- `/taxi`
- `/district_economy`
- `/tax`
- `/market`
- `/dispatch`
- `/adminconfig`
- `/stats`
- `/wanted`
- `/crime`
- `/routes`
- `/economy`
- `/shops`

### 5.3 Admin-Commands

#### `admin_system.inc`
- `/admin`
- `/mod`
- `/projectlead`
- `/projectleader`
- `/state`
- `/tpstate`
- `/setstateplayer`
- `/config`
- `/togglewar`
- `/setstate`
- `/setstatecoord`
- `/spawnvehicle`
- `/setcityhall`
- `/setdealership`
- `/sethouse`
- `/setbusiness`

#### Mapping-System
- `/mapmenu`
- `/mapadd`
- `/maptp`
- `/maplist`
- `/mapzones`
- `/mapdel`

#### Law/Jail/Events
- `/jail`
- `/release`
- `/eventtoggle`

### 5.4 Wichtige Auffälligkeiten

- `/acceptcall` ist als Handler vorhanden, aber nicht aktiv angebunden
- `/spawn`, `/garage`, `/weapons` und `/state` sind teilweise durch höhere Prioritäts-Handler verdeckt
- `/pm` und `/gang` sind eher Platzhalter/Info-Notizen als echte Gameplay-Logik
- Viele System-Commands existieren, aber sind nur indirekt über Dialoge oder UI-Aktionen erreichbar

---

## 6. Admin- und Rechtearchitektur

Das Projekt hat ein klar definiertes Adminmodell mit Rängen:

- `ADMIN_RANK_SUPPORT = 1`
- `ADMIN_RANK_MODERATOR = 2`
- `ADMIN_RANK_STATE_ADMIN = 3`
- `ADMIN_RANK_STATE_LEADER = 4`
- `ADMIN_RANK_OWNER = 5`

Das Admin-System prüft Rechte in mehreren Ebenen:

- globaler Admin-Level
- State-Admin-Level
- Projektleiter-Rechte
- World-Management-Berechtigung

Wichtige Aussagen:

- `AdminSystem_CanManageWorld(playerid)` ist die zentrale Security-Guard-Funktion
- `AdminSystem_IsProjectLeader()` prüft Projektleiterstatus
- `AdminSystem_IsOwner()` prüft Owner-Berechtigung
- `AdminSystem_IsStateLeader()` prüft staatliche Führungsrollen
- `AdminSystem_IsModerator()` prüft niedrigere Moderationsrechte

Die Rechte sind bewusst verteilt, damit Ghost-Checks, UI-DoS-Lücken und ungewollte World-Modificationen minimiert werden.

---

## 7. State-System und politische Struktur

Das State-System ist eines der wichtigsten Kernstücke des Gamemodes. Es ist die Basis für:

- drei Staaten
- Budget und Treasury
- Steuerlogik
- Leader-Names
- Sicherheit und Verwaltung
- geografische Spielwelt-Positionen

Die drei grundlegenden Staaten sind:

- Los Santos (0)
- San Fierro (1)
- Las Venturas (2)

Die State-Initialisierung setzt:

- Bankguthaben pro Staat
- Steuersätze pro Staat
- Revenue-Werte
- Policy-Strings
- Leader-Namen
- Spawnkoordinaten
- Enabled-Flags

Zusätzlich gibt es eine enge Beziehung zwischen:

- `core.inc`
- `state_system.inc`
- `state_spawn_system.inc`
- `government_system.inc`
- `district_system.inc`
- `state_budget_system.inc`
- `state_tax_system.inc`

---

## 8. Datenbank- und Persistenzmodell

### 8.1 Backend-Struktur

- `DB_BACKEND_SQLITE`
- `DB_BACKEND_MYSQL`

Der aktivere Standardpfad ist ein flexibler DB-/Cache-Layer mit SQLite- und MySQL-Adapteroption. Dini-, Scriptfiles- und Runtime-Safe-Ordner sind als Teil der Speicherarchitektur vorgesehen.

### 8.2 Cache- und Persistenzbereiche

Die DB-Schicht kümmert sich um:

- Spielerstatus
- Home-State
- Bank/Cash
- Inventory
- House Storage
- Fahrzeuge
- UBB-Fahrzeuge
- Liefer-/State-/Property-Daten

Eingebaute Backup-/Repair-Funktionen:

- `DB_RepairCriticalTables()`
- `DB_ValidateCriticalTables()`
- `DB_CreateBackupSnapshot()`
- `DB_RestoreBackupSnapshot()`

### 8.3 Wichtige Persistenzprinzipien

- Core dient als Source of Truth
- DB dient zur Synchronisation und Speicherung
- In-Memory-Caches werden mit Persistenzdaten synchronisiert
- Transaktionen und Guard-Funktionen sichern kritische Writes

---

## 9. Wirtschaftssysteme

Die Wirtschaft ist breit verteilt und ein Kernbereich des Servers. Wichtige Module:

- `economy/state_bank_system.inc`
- `economy/state_budget_system.inc`
- `economy/state_tax_system.inc`
- `economy/market_system.inc`
- `economy/city_economy_system.inc`
- `economy/dealership_system.inc`
- `economy/ammu_system.inc`
- `economy/contraband_system.inc`
- `business/business_system.inc`
- `shops/shop_system.inc`
- `shops/weapon_shop_system.inc`
- `commerce/marketplace_system.inc`
- `world/district_economy.inc`

Zusammen bilden sie:

- Staatskassen
- Steuern und Treasury
- Märkte
- Einkaufssysteme
- Business- und Shop-Logik
- Distrikt- und Stadtwirtschaft

Ein Spieler profitiert über:

- Lohn
- Eigentum
- Shopping
- Business-Erträge
- Fahrzeugkäufe
- Wirtschafts- und State-Transaktionen

---

## 10. Jobs, Progression und Spielerentwicklung

Das System enthält ein Progressionsmodell mit:

- Level
- XP
- Reputation
- Wanted
- Premium-Tier
- Premium-Expires
- Premium-Permanent

Zentrale Werte in Core:

- `E_PLAYER_LEVEL`
- `E_PLAYER_XP`
- `E_PLAYER_REP`
- `E_PLAYER_WANTED`
- `E_PLAYER_PREMIUM_TIER`

Jobsysteme sind in den Modulen:

- `jobs/jobs_core.inc`
- `jobs/state_jobs.inc`
- `jobs/job_center.inc`
- `jobs/dynamic_job_system.inc`
- `missions/mission_system.inc`
- `production/factory_network.inc`
- `logistics/trade_logistics.inc`

Diese Module modellieren:

- Rollen und Arbeitsprozesse
- Fortschritt
- Belohnung
- Aufgabe und Ausführung
- wirtschaftlichen Zugang zum Spiel

---

## 11. Law, Crime, Emergency und Justice

Das Rechtssystem ist sehr stark ausgebaut und verbindet mehrere Layer. Wichtige Module:

- `law/law_system.inc`
- `law/jail_system.inc`
- `crime/crime_system.inc`
- `emergency/emergency_system.inc`
- `emergency/dispatch_system.inc`
- `justice/prison_system.inc`
- `justice/court_system.inc`
- `medical/ems_hospital.inc`

Das System verwaltet:

- Wanted-Level
- Polizei-/EMS-Status
- Jail-/Release-Prozesse
- Dispatch-Queue
- Crime-Tracker
- Gericht und Haftlogik

Die Kernwerte für dieses System liegen teilweise direkt im Core:

- `E_PLAYER_WANTED`
- `E_PLAYER_JAIL_ACTIVE`
- `E_PLAYER_JAIL_TIME`
- `E_PLAYER_LAW_ROLE`
- `E_PLAYER_EMERGENCY_ROLE`
- `E_PLAYER_EMERGENCY_STATE`

---

## 12. Fahrzeuge, Garagen und Transport

Das Fahrzeugsystem ist extensiv aufgebaut. Kernmodule:

- `vehicles/vehicle_system.inc`
- `vehicles/garage_system.inc`
- `vehicles/tuning_system.inc`
- `vehicles/ubb_vehicle_system.inc`
- `vehicles/fuel_station_system.inc`
- `transport/transport_system.inc`
- `transport/taxi_system.inc`

Das System abbildet:

- Fahrzeugbesitz
- Garage-Spawning
- Fahrzeugmodifikationen
- Kraftstoffmanagement
- Fahrzeugpreise und -status
- Transport-/Taxi-/Routenlogik

Die DB-Schicht enthält dafür klar auffällige Laufzeitcaches, z. B.:

- `gDBUBBVehicleModelCache`
- `gDBUBBVehicleOwnerCache`
- `gDBUBBVehicleStateCache`
- `gDBUBBVehicleFuelCache`
- `gDBUBBVehiclePriceCache`

---

## 13. Fraktionen, Kriege und Territorien

Das Projekt behandelt Fraktionssysteme als reale Welt- und Machtstruktur. Wichtige Module:

- `factions/faction_system.inc`
- `factions/gangwar_system.inc`
- `factions/gang_fight_system.inc`
- `factions/text_wars.inc`
- `world/territory_system.inc`
- `world/district_economy.inc`

Es gibt mehrere Ebenen:

- Fraktionszugehörigkeit
- Gebietskontrolle
- Kriegs- und Kampfstatus
- War-HUD / Text-Wars-Widget
- territoriale Dynamik und Einflusszonen

Das `text_wars.inc`-System ist besonders wichtig, weil es ein aktives klickbares PlayerTextDraw-Widget einsetzt. Es nutzt die SA:MP-TextDraw-Callbacks und bildet eine direkte interaktive UI-Schicht für Kriegs-/Gebietslogik.

---

## 14. Housing, Inventory und Besitzsysteme

Das Projekt enthält ein klares Housing-/Inventory-Modell mit:

- `housing/house_system.inc`
- `housing/house_storage_system.inc`
- `housing/accessory_system.inc`
- `inventory/inventory_system.inc`

Wichtige Grundwerte aus dem Core:

- `MAX_INVENTORY_SLOTS = 24`
- `MAX_HOUSE_STORAGE_SLOTS = 12`
- `MAX_HOUSES = 50`

Diese Bereiche behandeln:

- Hausbesitz
- Lager/Safes
- Inventar-Slots
- Accessoires und Kleidung
- Besitz und Gegenstände mit RP-Kontext

Das macht das Projekt nicht nur zu einem Welt-/Wirtschaftsmod, sondern auch zu einem Charakter- und Besitzsystem.

---

## 15. UI-Flow und Dialogarchitektur

Das UI-System ist gemischt aus:

- Dialogen
- TextDraws
- PlayerTextDraws
- HUD-Bereichen

Zentrale UI-Dateien:

- `ui/ui_dialog_ids.inc`
- `ui/ui_hub.inc`
- `ui/roleplay_ui_system.inc`
- `ui/state_control_ui.inc`

Typische UI-Flows:

- Login-/Register-Flow
- State-/Home-State-Auswahl
- Inventory- und Business-Dialoge
- Job- und Vehicle-Menüs
- Quick Actions
- State-Leitstand
- Text-Wars-Widget

Das Projekt nutzt die klassische SA:MP-Methodik: Dialoge für Listen/Optionen, TextDraws für HUD-Status, PlayerTextDraws für interaktive Widgets.

---

## 16. TextDraws, HUDs und Boxes

Das Projekt hat eine breite TextDraw-Architektur. Die wichtigsten Bereiche sind:

### 16.1 Login / Tutorial
- `auth/login_system.inc`
- `tutorial/tutorial_system.inc`

Funktion:

- Login-/Registrierungs-Display
- Tutorial-Dialoge und Texte
- Intro-/Tutorial-Übersicht

### 16.2 Dashboard / Roleplay UI
- `ui/roleplay_ui_system.inc`

Funktion:

- aktuelle Spielerdaten
- State-Status
- context / live info panels
- HUD-Bereich

### 16.3 Text-Wars
- `factions/text_wars.inc`

Funktion:

- klickbares Widget
- Buttons: STATUS, GEBIETE, ANGRIFF, TRUPPEN, SCHLIESSEN
- PlayerTextDraws mit interaktiver Auswahl

### 16.4 Status-HUDs
- Gangwar
- Gangfight
- Wanted
- Crime Tracker
- Transport Routes
- City Economy
- Shop Overview
- Admin Config
- Admin Mapping

### 16.5 Gebaut, aber nicht angebunden
- Jail HUD
- Drug Market
- Job List
- Mission Board

Das zeigt, dass viele UI-Bereiche bereits existieren, aber nicht in den aktiven Runtime-Flow eingehängt sind.

---

## 17. Aktive vs. nicht aktive Systeme

### Aktiv und klar eingebunden
- Core
- State-System
- Admin-System
- UI-Hub
- Roleplay-HUD
- Login-/Tutorial-Flow
- Economy- und Shopsysteme
- Faction-/War-/Text-Wars-Systeme
- Law-/Emergency-Systeme
- Vehicle-/Garage-Systeme

### Aktiv, aber teilweise verdeckt oder überlagert
- `/spawn`
- `/garage`
- `/weapons`
- `/state`
- mehrere social/RP-Commands
- `gangwar`- und `war`-Begriffe unter mehreren Aliases

### Gebaut, aber nicht vollständig angebunden
- Jail-HUD
- Drug Market
- Mission Board
- Job List
- Dynamic Job System UI
- Dispatch accept-call flow

Das ist kein Fehler per se, sondern der allgemeine Stand eines wegen modularen großen Projekts, bei dem mehrere Module schon existieren, aber noch nicht aus dem laufenden Flow vollständig verknüpft sind.

---

## 18. Wichtige Besonderheiten des Projekts

### 18.1 Keine typische CMD-Framework-Struktur

Das Projekt nutzt keine klassische zcmd/YCMD-Lösung im engeren Sinne. Der Befehlshandler läuft über `OnPlayerCommandText` und vergleicht Strings direkt oder ruft Module-Handler auf.

### 18.2 Priorität der Handler

Die Dispatch-Reihenfolge ist wichtig. Wenn ein Handler `1` zurückgibt, ist der Befehl damit „abgefangen“. Das führt zu verdeckten bzw. überlappten Befehlen.

### 18.3 Core als Truth Layer

Das Design versucht, `core.inc` als zentrale Quelle für Spielerstatus und Validierung zu etablieren. Damit bleiben Module konsistent und keine parallelen Status-Stores entstehen.

### 18.4 UI und Logik sind getrennt

UI-Module zeigen Daten an und leiten Interaktion weiter, aber die eigentliche Spiel-Logik sitzt in fachlichen Modulen. Das ist sauber und modular.

### 18.5 Dini- und DB-first mit Modularisierung

Die Persistenzarchitektur ist für SA:MP-Umgebungen angepasst und geht deutlich über ein nur einmaliges `mysql`- oder `sqlite`-Setup hinaus. Das Projekt versucht, Caches, Schemas, erste Validierung und backupsicheres Schreiben zusammenzubringen.

---

## 19. Gesamtfazit

Das Project ist eine breit angelegte, hochmodulare RP-Welt mit klarer innerer Architektur. Es verbindet:

- Core
- State
- Admin
- Economy
- Jobs
- Vehicles
- Housing
- Inventory
- Law
- Emergency
- Factions
- World
- UI
- TextDraws
- Events
- Season
- Persistence
- Web-Bridge

Das Projekt hat bereits sehr viel klassische SA:MP-RP-Struktur und konsistentes Modul-Design. Gleichzeitig zeigt es auch, dass viele Bereiche noch im Ausbau sind: Einige Areale sind vorhanden, funktionieren aber noch nicht vollständig im aktuellen Spiel- und UI-Flow. Das ist kein Widerspruch zur Architektur, sondern typisch für ein großes, modular entwickeltes RP-Projekt in einem frühen bis mittleren Ausbauzustand.

Die Dokumentation beschreibt daher nicht nur den „Wunschzustand“, sondern den aktuellen, quellbasierte Ist-Zustand mit klarer Unterscheidung von:

- aktiv
- vorbereitet
- verdeckt
- gebaut aber nicht angebunden

---

## 20. Verzeichnis der relevanten Dokumente

- `COMMANDS.md`
- `MODULES.md`
- `TEXTDRAWS.md`
- `MODULE_DEPENDENCIES.md`
- `ADMIN_SYSTEMS.md`
- `STATE_SYSTEM.md`
- `DATABASE_SCHEMA.md`
- `UI_FLOW.md`
- `PLAYER_PROGRESSION.md`
- `VEHICLE_SYSTEM.md`
- `ECONOMY_SYSTEMS.md`
- `FACTION_AND_WAR_SYSTEMS.md`
- `EMERGENCY_AND_LAW.md`
- `HOUSING_AND_INVENTORY.md`
- `EVENTS_AND_SEASON.md`
- `ARCHITECTURE.md`
- `PROJECT_MASTER_OVERVIEW.md`
- `PROJECT_QUICK_REFERENCE.md`
- `PROJECT_COMPLETE_MASTER.md`

Diese Sammlung bildet den aktuellen Dokumentationsstand des Projekts in konsistenter Form ab.

---

## 21. Abschluss

Damit liegt die vollständige Projektbeschreibung in einer zentralen Master-Datei vor. Sie fasst die wichtigsten Bereiche zusammen und verknüpft sie mit den bestehenden Detaildokumenten, ohne die bisherigen Informationen zu verlieren.

Die wesentlichen Erkenntnisse sind:

- Das Projekt ist bereits in einem strukturierten und fachlich klaren Ausbauzustand.
- Viele Systeme sind vorhanden und gut organisiert.
- Einige Bereiche sind noch nicht vollständig durch den aktiven Spiel-Flow verbunden.
- Die Dokumentation entspricht dem aktuellen Quellcode-Stand und ist damit für weitere Entwicklung, Analyse, Erweiterung und Abstimmung ein wertvolles Grundlagenwerk.


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
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut (nur legacy warning in `includes/dini.inc`).
- Dini-first + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

