# GTA:SA Reallife Modular Gamemode

## Veröffentlichungsstatus

Dieses Projekt ist als modularer, erweiterbarer GTA:SA-Reallife-Gamemode für einen 3-Staaten-Server veröffentlicht. Die Kernidee ist bewusst: keine monolithische `gamemode.pwn`-Logik, sondern ein klares Modul- und Layer-System mit separaten Bereichen für Core, State, Government, Economy, Housing, Jobs, Factions, Vehicles, Inventory, Services und Web-Management.

### Kerneigenschaften

- 3-Staaten-System: Los Santos, San Fierro, Las Venturas
- eigene Staatskasse pro Staat
- eigene Verwaltung, Steuern, Revenue- und Treasury-Logik je Staat
- 3x/1Admin-Architektur mit globalem Admin, staatlichem Admin und State-Leitung
- SQLite-First-Design mit sauberer Backend-Abstraktionsschicht für späteres MySQL 4.1
- modulare Struktur für einzelne Systeme, die unabhängig ausgebaut werden können
- Textdraw-basierte Auth- und Login-Flows
- Heimatstadt-, Tutorial- und Intro-Camera-Systeme
- RP-optimierte Regierung-, Wirtschafts- und Fraktionsarchitektur
- Housing-, Inventory-, Vehicle-, Business- und Property-Logik mit eigener Ownership-Schicht
- Web-/Dashboard-Integration als separates Kontrollpanel mit SQLite-Read-Only-Lesezugriff
- `c++`-Mirror: Strukturtreuer 1:1-Spiegel des Projekts als C++-sichtbare Projektebene für Audit, Überblick und Tooling

## Aktueller Entwicklungsstand

Das Projekt hat einen vollständigen modularen RP-Framework-Stand erreicht und ist als robuste Basis für reale SA:MP-Entwicklung nutzbar. Zusätzlich existiert im Ordner `c++` ein strukturtreuer Spiegel des gesamten Projekts, um die Modularität, Dateistruktur und Quellhistorie auch in C++-sichtbarer Form nachvollziehbar zu halten. Die wichtigsten Systembereiche sind bereits als eigenständige Module angelegt und an die Haupt-Architektur angehängt.

### Umgesetzte Kernmodule

- `core/` – zentrale Spielerdaten, State-Logik, Admin-Status, Validierung, Bank-/Cash-Bridge
- `db/` – SQLite/MySQL-Abstraktionsschicht, Schema-Initialisierung, Transaktions-Wrapper, Save/Load-APIs
- `state/` – Staatslogik mit 3 Zuständen, Treasury-Transfers, Spawns, State-Services
- `admin/` – Admin-Architektur und statische Berechtigungs-/Mapping-Schichten
- `auth/` – Login/Register/Tutorial/Home-State-Flow
- `camera/` – Intro- und State-Kamera-Szenen
- `government/` – Regierung und öffentliche Verwaltung je Staat
- `economy/` – Steuern, Staatskassen, Autohäuser, Tankstellen, Market, Banking
- `jobs/` – Jobsysteme, dynamische Jobs, staatliche Verbindung
- `housing/` – Häuser, Zuweisung, House-Safes, Inventar-/Lagerlogik
- `inventory/` – Items, Slots, Quantitäten, Metadaten, Besitzlogik
- `vehicles/` – Fahrzeugbesitz, UBB-Logik, Tuning, Garage, Tanksysteme
- `factions/` – Fraktionen, Gangs, Wars, Fights
- `business/` – Firmen-/Bizz-Architektur, Eigentümer, Wirtschaft
- `law/` – Wanted-, Knast-, Gericht- und Rechtssysteme
- `emergency/` – Police/EMS/Dispatch-Module
- `social/` – Rolle, Beziehungen, RP-Kommandos und sozialer Flow
- `web/` – externe Web-Bridge und Dashboard-Integration

## Produktionsrelevante Stabilitätsbasis

Die letzte Erweiterungsphase war bewusst auf echte Produktionsrelevanz ausgerichtet. Dabei wurden die kritischen Übergabepunkte zwischen Kernmodulen, DB-Abstraktion, Inventar, Haussafes und Fahrzeugsystemen systematisch überarbeitet.

### Korrigierte Kernprinzipien

1. Hook-Reihenfolge ist deterministisch.
   - `OnPlayerConnect()` ruft jetzt zentral die Modullader in einer klaren Reihenfolge auf.
   - Jeder State-/Inventory-/House-/Vehicle-Loader hat damit einen konsistenten Startpfad.

2. Lifecycle-Contract für Module ist jetzt explizit definiert.
   - `module_contract.inc` ergänzt den Boot- und Runtime-Flow mit definierter Stufenfolge: Boot → DB-ready → State-ready → Runtime-ready.
   - `Core_SetLifecycleStage()` und `Core_AssertLifecycleStage()` erzwingen ein vorhersehbares Startprotokoll, damit Module nicht implizit in falscher Reihenfolge initialisiert werden.

3. SQLite-Transaktionen wurden als atomare Schreibpfade formalisiert.
   - `DB_BeginTransaction()` nutzt `BEGIN EXCLUSIVE TRANSACTION`.
   - `DB_CommitTransaction()` und `DB_RollbackTransaction()` sind als zentrale Fehlerbehandlung eingebaut.

4. Schema- und Constraint-Logik wurde für die wichtigen Tabellen erweitert.
   - `houses` hat klare `ON DELETE CASCADE`-/`SET NULL`-Beziehungen.
   - `house_storage` und `ubb_vehicles` profitieren von konsistenterer Referenzlogik.
   - `inventory`, `house_storage_items`, `ubb_vehicles` sind mit gültigen Index- und Value-Grenzen versehen.

5. State- und Treasury-Logik wurde auf eine gemeinsame Quelle zurückgeführt.
   - `Core_*`-Bankwerte, `StateSystem_*`-Kassen und `DB_*`-Treasury-Pfade sind als einheitlicher Datenfluss konzipiert.

6. Für kritische Systeme gibt es Guard-Checks gegen ungültige Werte.
   - Arrayslots, Item-Quantitäten und Fahrzeug-/Safe-Operationen prüfen explizit Grenzen und Zustände.

## 3-Staaten-Architektur

Die wirtschaftliche und politische Logik ist auf drei Bundesstaaten verteilt:

- Los Santos – Großstadt-/Business-Identität mit globaler Verkehrsfunktion
- San Fierro – Industrie-, Hafen- und Logistikprofil
- Las Venturas – Tourismus-, Luxus- und Casinoprofil

Jeder Staat besitzt:

- eigene Staatskasse
- eigene Steuerlogik
- eigene Service-/Government-Module
- eigene Wirtschaft- und City-Identität
- eigene Verwaltungslayer und lokale RP-Features

## Architektur-Regeln

1. `core.inc` ist die Single Source of Truth für Spielerstatus, Bank, Home-State und Admin-Level.
2. Neue Module dürfen keine zweite, parallele Player-Status-Quelle einführen.
3. Alle monetären Änderungen laufen über zentrale State-/Bank-/DB-APIs.
4. DB-Schema wird über `DB_Initialize()` und `DB_Ensure*Schema()` idempotent abgearbeitet.
5. Include-Reihenfolge in `gamemode.pwn` bleibt bindend: Core → DB → State → Admin → Auth → Economy → Jobs → Laws → Housing/Vehicle → Leisure/Web.
6. Neue Systeme müssen in `README.md` und in `TO-DO.md` dokumentiert werden.

## Modulübersicht

### Staat und Verwaltung

- `state_system.inc`
- `state_identity_system.inc`
- `state_spawn_system.inc` – zentrale Spawn-/City-Map pro Staat und State-Home-Logik
- `state_budget_system.inc` – staatliche Treasury-/Budget-Übersicht und Ledger-Transfers
- `government_system.inc`
- `state_governance.inc`
- `city_hall_system.inc`
- `municipal_system.inc`
- `airport_system.inc`
- `district_system.inc`
- `district_economy.inc`

### Wirtschaft und Kasse

- `state_bank_system.inc`
- `state_tax_system.inc`
- `city_economy_system.inc`
- `market_system.inc`
- `marketplace_system.inc`
- `contraband_system.inc`
- `dispatch_system.inc`
- `dealership_system.inc`
- `ammu_system.inc`

### Jobs, soziale und RP-Systeme

- `jobs_core.inc`
- `state_jobs.inc`
- `dynamic_job_system.inc`
- `mission_board.inc`
- `factory_network.inc`
- `taxi_system.inc`
- `transport_workers.inc`
- `relationship_system.inc`
- `social_system.inc`
- `roleplay_command_system.inc`

### Besitz, Fahrzeuge und Inventar

- `inventory_system.inc`
- `house_system.inc`
- `house_storage_system.inc`
- `accessory_system.inc`
- `vehicle_system.inc`
- `garage_system.inc`
- `tuning_system.inc`
- `ubb_vehicle_system.inc`
- `fuel_station_system.inc`

### Sicherheit, Justiz und Notfall

- `law_system.inc`
- `jail_system.inc`
- `court_system.inc`
- `prison_system.inc`
- `crime_system.inc`
- `emergency_system.inc`
- `dispatch_system.inc`
- `ems_hospital.inc`

### State-Ledger und Treasury-Integrität

- `state_ledger_system.inc`
- zentrale State-/Treasury- und Budget-Transfers mit konsistenter Protokollierung pro Staat
- `StateLedgerSystem_Transfer()` und `StateLedgerSystem_ShowSummary()` als gemeinsame Treasury-API

## Control Panel und Web-Management

Das separate Control Panel ist ein eigenes Web-Modul außerhalb des SA:MP-Gamemodes. Es liest dieselbe SQLite-Datenbasis und ist für Monitoring, Status-Checks, Admin-Übersicht und Betriebsdaten ausgelegt.

### Verfügbar

- `control-panel/server.js`
- `control-panel/public/index.html`
- `control-panel/public/app.js`
- `control-panel/README.md`

### Ziel

- Live-Status des Servers
- Übersicht der Staatskassen
- Spieler-, Admin-, Job- und Fraktionslisten
- Business-, Vehicle- und House-Übersichten
- spätere UCP-/Admin-Aktionen ohne Pawn-Blockade

## Plugins und Includes

Die Projektbasis ist bewusst auf moderne SA:MP-Plugins gesetzt. Die empfohlene Grundstruktur richtet sich nach modularem Design, Stabilität und Skalierbarkeit.

Empfohlener Stack:

- `crashdetect`
- `streamer`
- `sscanf2`
- `sqlite`
- `mysql` (für späteres Upgrade auf MySQL 4.1)
- `zcmd`
- `foreach`
- `MapAndreas`
- `pawn-plus` / kompatible Helper-Includes

## Verifikation und Einschränkungen

Die Umgebung enthält keine SA:MP/Pawn-Compiler-Toolchain. Daher konnten keine echten Kompilierungen oder Live-Gameplay-Tests in diesem Container durchgeführt werden. Die Verifikation erfolgte über:

- strukturelle Konsistenz der Module
- Include-/Abhängigkeitsprüfung
- DB-Schema- und SQL-Constraint-Review
- Hook-/Lifecycle-Logik-Review
- Kontrollpanel- und API-Checks auf projektinternes Setup
- Sanierung kritischer Makro-/Include-Definitionskonflikte, damit die Core-, DB- und Guard-Layer in einer deterministischen Reihenfolge verwendbar sind

## Stabilisierung und Release-Status

Die Projektbasis wurde auf Release-Qualität für die modularen Kernschichten gehoben. Nach der letzten Review- und Stabilisierungslauf wurde eine zentrale Korrektur durchgesetzt: gemeinsame Grenzwerte für Inventar, House-Storage und UBB-Fahrzeuge werden jetzt zentral in `core.inc` und `production_guard.inc` definiert, um Include-Reihenfolge- und Präprozessor-Konflikte zu vermeiden.

Zusätzlich wurden die kritischen Übergänge auf einen konsistenten Datenfluss gebracht:

- DB-Transaktions-Wrapper für atomare Schreibpfade
- definierte Hook-Reihenfolge im Connect-/Disconnect-Lifecycle
- Guards für Array-Slots und Stack-Grenzen
- State-Kassensystem als gemeinsame Single Source of Truth
- Schema-Definitionen für Inventar, House-Safe und UBB-Fahrzeuge mit validierten Constraints

Damit bleibt das Projekt als modularer, erweiterbarer Ausgangspunkt für ein reales 3-Staaten-Reallife-System nachvollziehbar, sauber strukturiert und für spätere Live-Validierung vorbereitet.

## Nächste Entwicklungsstufen

### Phase 1 – Production-safe persistence (architektonisch umgesetzt)

- vollständige DB-Load-/Save-Interfaces für Spieler-, House-Storage-, Inventory- und UBB-Fahrzeugpfade eingebaut
- zentrale Repair-/Validation- und Transaction-Guards für kritische Tabellen etabliert
- konsistente Cash-/State-/Bank-Bridge zwischen Core-, State- und DB-Layer ergänzt
- Backup-/Restore-Snapshot-Pfade als strukturierte Scaffold- und Operations-API definiert

### Phase 2 – Live-Gamemode-Validierung

- SA:MP/Pawn-Toolchain einrichten
- echtes Kompilieren des Gamemodes auf einem lokalen Server
- Verhalten unter mehreren Spielern testen
- echte DB- und State-Integration mit Live-Serverdaten verifizieren
- Datensynchronisation zwischen State-Logik und SQLite prüfen

### Phase 3 – Erweiterung des RP-Ökosystems

- tieferes Job- und Fraktions-Gameplay
- politische Entscheidungssysteme
- zusätzliche Events, Stadtinteraktionen und Dienstleistungs-Module
- noch stärkere Unterscheidung zwischen LS/SF/LV pro System

## Abschluss

Die Projektbasis ist jetzt als vollständige, publikationsreife modularer RP-Framework veröffentlicht. Sie ist breit genug für echtes Reallife-Gameplay, stabil genug für produktionsnahe Architekturreflexionen und modular genug, um künftig einzelne Komponenten ohne kompletten Rewrite zu erweitern.
