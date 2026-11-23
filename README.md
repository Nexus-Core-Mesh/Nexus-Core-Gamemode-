# GTA:SA Reallife Modular Gamemode

> [!NOTE]
> Diese Doku wurde optisch aufbereitet, ohne technische Fakten, Pfade, Ports oder Projektregeln zu verändern.
> Fokus lag auf Lesbarkeit, Struktur, Übersichten und deutlicherer Orientierung.

## ✅ Wichtig zu wissen

- Technische Fakten, Befehle, Pfade und Projektregeln bleiben bewusst unverändert.
- Port- und Systemtrennung bleiben konsistent, insbesondere für den 7780-Contract und den Dini-first-Pfad.
- Die optische Aufwertung dient nur der Lesbarkeit, nicht der inhaltlichen Interpretation.

---
## 🧭 Schnellüberblick

| Bereich | Inhalt |
| --- | --- |
| Dokument | `README.md` |
| Typ | Projekt-Readme |
| Fokus | Zugang, Überblick und Startpfad |
| Wichtige Pfade | `gamemodes/`, `Compiler/samp03/`, `tools/` |

---
## 📊 Veröffentlichungsstatus

Dieses Projekt ist als modularer, erweiterbarer GTA:SA-Reallife-Gamemode für einen 3-Staaten-Server dokumentiert. Der aktuelle Stand ist ein konsistenter Dini-first-/Fallback-Architektur- und Build-Validierungsstand, aber keine finale Live-Release-Freigabe für einen echten Ubuntu-VM-Produktionslauf. Der reale Freigabe-Gate bleibt erst dann erfüllt, wenn `pawncc`/`qemu-i386`, die vollständige `scriptfiles/`-Struktur, der echte Service-Start und die SQLite-/Control-Panel-Synchronisierung auf der Ziel-VM ohne offene Blocker bestanden sind.

Die Kernidee ist bewusst: keine monolithische `gamemode.pwn`-Logik, sondern ein klares Modul- und Layer-System mit separaten Bereichen für Core, State, Government, Economy, Housing, Jobs, Factions, Vehicles, Inventory, Services und Web-Management.

### Status-Tagging der aktuellen Projektlage

- `aktiv`: Kernpfade mit stabiler Einbindung in den Laufzeit- und Command-Flow (`core.inc`, `login_system.inc`, `ui_hub.inc`, `state_spawn_system.inc`, `OnPlayerCommandText()` / `OnPlayerSpawn()`)
- `verdeckt`: bereits im Code vorhanden, aber nicht als primärer Endanwender-Flow eingeblendet oder noch durch ein aktiveres System überlagert (`roleplay_ui_system`, Teilbereiche von Job-/Mission-/Jail-/Drug-/Shop-/Business-/Vehicle-/Inventory-/Event-/Season-/Web-Bridge)
- `gebaut aber nicht angebunden`: Architektur und Logik existieren, aber der aktive Routing-/UI-/State-Pfad ist noch nicht in den Haupt-Loop eingehängt (`mission_system`, `job_center`, gewisse spezielle Web-/Dialog-Views)
- `vorbereitet`: System- und Datenpfade strukuriert angelegt, aber noch keine feste End-to-End-Verknüpfung im aktiven RP-Flow (`season_system`, weiterführende Web-/Bridge-/Admin-Views, bestimmte Export-/Sync-Pfade)
- `nicht verbunden`: Module, Commands oder UI-Views, die im Quellenbaum vorhanden sind, aber in der aktiven Main-Loop und dem Hub-Flow keine ansprechbare Route besitzen (`reine Spezialansichten, legacy-scoped mini-views, einzelne nicht durch `UiHub_Ready()` geschützte Handler`)

Hinweis: Das GitHub-Root-README ist die sichtbare Projektübersicht auf mobil/Browser; das separate `control-panel/` ist ein eigenes Node-Tooling und nicht die Ursache für die Mobile-Renderprobleme in `README.md`.

### 📘 Kerneigenschaften

- 3-Staaten-System: Los Santos, San Fierro, Las Venturas
- eigene Staatskasse pro Staat
- eigene Verwaltung, Steuern, Revenue- und Treasury-Logik je Staat
- 3x/1Admin-Architektur mit globalem Admin, staatlichem Admin und State-Leitung
- Dini-first Persistenz mit robuster `scriptfiles/`-Fallback-Schicht und optionaler SQLite/MySQL-Autosync-Integration
- modulare Struktur für einzelne Systeme, die unabhängig ausgebaut werden können
- Textdraw-basierte Auth- und Login-Flows
- Heimatstadt-, Tutorial- und Intro-Camera-Systeme
- RP-optimierte Regierung-, Wirtschafts- und Fraktionsarchitektur
- Housing-, Inventory-, Vehicle-, Business- und Property-Logik mit eigener Ownership-Schicht
- Web-/Dashboard-Integration als separates Kontrollpanel mit SQLite-Read-Only-Lesezugriff
- `c++`-Mirror (optional / falls vorhanden): strukturtreuer Projektspiegel als C++-sichtbare Projektebene für Audit, Überblick und Tooling

### 🔧 Control-Panel-Setup und gemeinsamer Start

Das Projekt enthält zusätzlich ein separates, Node.js-basiertes Control Panel unter `control-panel/`. Der Startpfad ist bewusst nicht an den Live-SAMP-Server gebunden: Das Panel kann im Quellbaum ohne 7780-Serverzugriff vorbereitet und gestartet werden, solange `node`/`npm` auf dem Zielsystem verfügbar sind. Der durchgängige Quell- und Deploy-Pfad ist:

```bash
python3 tools/control_panel_manager.py ensure
python3 tools/control_panel_manager.py start
./scripts/deploy_server.sh
```

Zusätzlich sind vier Offline-TUI-/Autofix-Skripte im Projektbaum eingebettet und repo-gebunden erreichbar:

```bash
python3 nexus_hub_tui.py
python3 nexus_tools_tui.py
python3 nexus_autofix.py
python3 samp_tui.py
```

Diese Skripte laden den jeweiligen Implementierungsteil aus `tools/` und nutzen keine feste `/home/...`-Pfadangabe oder Live-7780-Anbindung. Sie bleiben damit kompatibel mit dem Source-First-Workflow, auch wenn keine lokale SAMP-Laufzeit oder Port 7780 aktiv ist.

Wichtige Regeln:

- `control-panel/server.js` liest nur lesend aus `data/db/gtasa_reallife.db` und ist keine primäre Schreibquelle.
- Der Gamemode selbst bleibt Dini-first über `Compiler/samp03/scriptfiles/`; dadurch bleibt die SQLite-Sicht nur dann korrekt, wenn Dini- und SQLite-Pfade synchronisiert wurden.
- Ein fehlendes `node`/`npm` oder eine unvollständige `npm install` blockiert das Panel nicht mehr den Gamemode-Start, sondern erzeugt nur eine Warnung und lässt den Server-Deploy weiterlaufen.
- Der Server-Deploy bleibt auf einen einzelnen Build-Target ausgerichtet: `Compiler/samp03/gamemodes/gtasa_reallife.amx`.

## 📊 Aktueller Entwicklungsstand

Das Projekt hat einen vollständigen modularen RP-Framework-Stand erreicht und ist als robuste Basis für reale SA:MP-Entwicklung dokumentiert. Wenn ein `c++`-Spiegel im Repository vorhanden ist, dient er als strukturtreuer Projektspiegel für Audit, Überblick und Tooling; er ist kein Laufzeitbestandteil des SA:MP-Servers. Die wichtigsten Systembereiche sind bereits als eigenständige Module angelegt und an die Haupt-Architektur angehängt. Der aktuelle Härtungsstand bezieht sich jedoch auf den aktuellen Code-/Build-Stand und die Architektur-Validierung; ein echter Live-Server-Start auf Ubuntu-VM bleibt noch als separate Produktionsprüfung auszuführen.

Zusätzlich wurden im Offline-/Audit-Pfad mehrere manuelle `python3`-Werkzeuge ergänzt, die ausschließlich auf CLI-Aufruf und ohne Serverstart funktionieren. Diese Hilfsprogramme sind bewusst schlank und standardbibliotheksbasiert: `log_code_bridge.py`, `code_line_extractor.py`, `log_guided_autocorrect.py` und `debug_trace_collector.py` dienen zur Zuordnung von Log-Ereignissen zu Quellcode, Zeilen-/NL-Extrakten, sicheren Dry-Run-Korrekturen und kompakten Diagnose-Exports.

### 🏗️ Umgesetzte Kernmodule

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

### 🏗️ Additiv ergänzte Systemausbau-Module

- `jobs/dynamic_job_system.inc` – dynamische Jobs je Staat mit state-gebundener Zuweisung, Aktiv-Status und Job-Bridge zu `JobsCore_*`.
- `missions/mission_system.inc` – staatlich gebundene Missionen mit Ablauf, Auszahlungslogik, aktiver Mission-Liste und validierter Belohnungsfluss in Bank-/Treasury-Pfade.
- `events/season_system.inc` – Jahreszeiten, staatliche Wirtschaftspulse, Job-/Wirtschafts-Modifier und Belohnungskriterien ohne parallele State-Shadow-Quelle.
- `business/business_system.inc` – Business-Registrierung, Einnahmen, Owner-Payouts und State-zugewiesene Treasury-Transfers mit validem State-Fallback.
- `law/law_system.inc` und `crime/crime_system.inc` – Wanted-Status, Verbrechens-Wärme, Polizei-/Staats-Interaktion und Textdraw-Tracking für Polizei-/Crime-UI.
- `jobs/jobs_core.inc` – zentrale Aufgaben-/Lohn-Pipeline mit staatlicher Kassenfinanzierung und steuerbarer Verteilung.
- `premium/premium_system.inc` – Premium-/VIP-System mit Laufzeiten 30/90/300 Tage sowie Permanent, validem Ablauf-Check, zentraler Core-Integration und Dini-first/Persistence-Bridge ohne parallele State-Shadow-Quelle.

### ✅ Aktueller Runtime-Status (Quellvalidierung)

- `OnPlayerCommandText()` bleibt in der Reihenfolge `UI-Hub -> RP-UI -> TextWars -> Admin -> Jail -> Dispatch -> Emergency -> Garage -> Weapon -> State-Event -> Social -> RoleplayCommand -> AdminMapping` verankert, sodass `/menu`, `/hud`, `/help`, `/spawn`, `/jobs` und `/acceptcall` praktisch in einer zentralen UI-/Hub-Schicht landen, ohne durch veraltete Shadow-Handlers zu fallen.
- `UiHub_HandleCommand()` ist die aktive, konsolidierte Entry-Point-Schicht; `RoleplayUI_HandleCommand()` bleibt als kompatibler Fallback für `/hud`/`/dashboard` aktiv, aber der Hub nimmt mit `UiHub_Ready()` den primären Guard- und Menü-Flow.
- Die 3-Staaten-Start-/Spawn-Logik bleibt an `Core_IsValidState()` / `Core_GetValidStateOrDefault()` / `StateSpawnSystem_Apply()` gekoppelt, sodass gültige `state_id` und `home_state` nicht mit `0` überschrieben werden.
- Build-/Compile-Gate: `bash scripts/build_gamemode.sh` muss weiterhin mit Exit-Code `0` und dem AMX-Ziel unter `Compiler/samp03/gamemodes/gtasa_reallife.amx` durchlaufen.

## 📘 Produktionsrelevante Stabilitätsbasis

Die letzte Erweiterungsphase war bewusst auf echte Produktionsrelevanz ausgerichtet. Dabei wurden die kritischen Übergabepunkte zwischen Kernmodulen, DB-Abstraktion, Inventar, Haussafes und Fahrzeugsystemen systematisch überarbeitet.

### 📘 Zusätzlicher Schutz für State-/Home-State-Transfers

Seit dem letzten Lauf wurden die angrenzenden Übergänge in `core.inc`, `login_system.inc`, `state_system.inc` und `db_interface.inc` zusätzlich auf den stabilen Pfad `Core_GetValidStateOrDefault()` + `DB_ValidateStateTransfer()` verdrahtet. Dadurch werden zwar gültige `state_id`/`home_state`-Werte nicht mehr mit `0` verworfen, aber sichere Fallbacks bleiben weiterhin auf `LS`/`SF`/`LV`-Zahlen validiert.

Die Transfer- und Rehydrate-Pfade bleiben jetzt in der Reihenfolge:
- Kontrolle validen States über `Core_GetValidStateOrDefault()`
- sichere Normalisierung in `DB_SetPlayerState()` / `DB_SavePlayerState()`
- DB-Write mit `INSERT OR IGNORE` als akkumulierender Schutz, statt eines direkten `0`-Overrides
- optionales `StateLedgerSystem_Transfer()` mit `StateLedgerSystem_ValidateTransfer()`

Damit bleiben die kritischen Pfade für Login/Disconnect/Spawn/Rehydrate, State-Transfers und Treasury-Aktualisierung konsistent und ohne parallelen Schatten-Status.

### ✅ Korrigierte Kernprinzipien

0. Valid-state-Guards bleiben harte Architekturregeln.
   - `login_system.inc` und andere Disconnect-/Reset-Pfade dürfen gültige `state_id`/`home_state`-Werte nicht mehr mit `0` zurücksetzen.
   - Bei Disconnect wird ein gültiger Standardwert `LS` wiederhergestellt, statt den Spielerdaten einen invaliden Null-Reset zu hinterlassen.
   - Zusätzliche Schutzpfade in `core.inc` und `state_system.inc` verankern denselben Standard bei Connect/Disconnect/Spawn/Rehydrate, damit kein Bereich einen noch gültigen Zustand durch einen Strang 0 verwirft.

1. Hook-Reihenfolge ist deterministisch.
   - `OnPlayerConnect()` ruft jetzt zentral die Modullader in einer klaren Reihenfolge auf.
   - Jeder State-/Inventory-/House-/Vehicle-Loader hat damit einen konsistenten Startpfad.

2. Lifecycle-Contract für Module ist jetzt explizit definiert.
   - `module_contract.inc` ergänzt den Boot- und Runtime-Flow mit definierter Stufenfolge: Boot → DB-ready → State-ready → Runtime-ready.
   - `Core_SetLifecycleStage()` und `Core_AssertLifecycleStage()` erzwingen ein vorhersehbares Startprotokoll, damit Module nicht implizit in falscher Reihenfolge initialisiert werden.

3. Dini-first-Atomarität und SQL-Sync-Abgrenzung wurden explizit getrennt.
   - `DB_BeginTransaction()`/`DB_CommitTransaction()` sind für optionale SQLite/MySQL-Sync- und Audit-Pfade relevant, aber sie ersetzen nicht die primäre Dini-first-Transaktionslogik im laufenden Serverzustand.
   - Der echte primäre Schreibpfad bleibt `scriptfiles/`-Dini; kritische Transfers müssen als temp-Datei → validate → rename ohne halbe Zustände abgewickelt werden.
   - SQLite/MySQL dient ergänzend als Autosync- oder Dashboard-Quelle und niemals als alleiniger Ersatz für den Dini-first-Save-Stack.

4. Schema- und Constraint-Logik wurde für die wichtigen Tabellen erweitert.
   - `houses` hat klare `ON DELETE CASCADE`-/`SET NULL`-Beziehungen.
   - `house_storage` und `ubb_vehicles` profitieren von konsistenterer Referenzlogik.
   - `inventory`, `house_storage_items`, `ubb_vehicles` sind mit gültigen Index- und Value-Grenzen versehen.

5. State- und Treasury-Logik wurde auf eine gemeinsame Quelle zurückgeführt.
   - `Core_*`-Bankwerte, `StateSystem_*`-Kassen und die Dini-/SQLite-Reload-Pfade bilden einen konsistenten Datenfluss; die primäre Live-Quelle bleibt jedoch der Dini-first-Storage in `scriptfiles/`.

6. Für kritische Systeme gibt es Guard-Checks gegen ungültige Werte.
   - Arrayslots, Item-Quantitäten und Fahrzeug-/Safe-Operationen prüfen explizit Grenzen und Zustände.

## 🏗️ Scriptfiles-/Dini-Laufzeitstruktur und 7780-Ordner

Neben den modularen Source-Dateien wurde die physische Laufzeitstruktur unter `Compiler/samp03/scriptfiles/` vollständig ergänzt, damit der Dini-first-Write-Pfad nicht an fehlenden Verzeichnissen scheitert. Die aktuelle Struktur umfasst die kritischen Speicherpfade für Spieler, Häuser, House-Storage, Safes, Fahrzeuge, UBB-Fahrzeuge, Staatskassen, Business-, Faction- und Inventardaten sowie temporäre und Backup-Dateien:

- `Compiler/samp03/scriptfiles/players/`
- `Compiler/samp03/scriptfiles/houses/`
- `Compiler/samp03/scriptfiles/house_storage/`
- `Compiler/samp03/scriptfiles/safes/`
- `Compiler/samp03/scriptfiles/vehicles/`
- `Compiler/samp03/scriptfiles/ubb_vehicles/`
- `Compiler/samp03/scriptfiles/state/LS/`, `state/SF/`, `state/LV/`
- `Compiler/samp03/scriptfiles/state_kasse/LS/`, `state_kasse/SF/`, `state_kasse/LV/`
- `Compiler/samp03/scriptfiles/business/`
- `Compiler/samp03/scriptfiles/factions/`
- `Compiler/samp03/scriptfiles/inventory/`
- `Compiler/samp03/scriptfiles/temp/`
- `Compiler/samp03/scriptfiles/backup/`
- `Compiler/samp03/scriptfiles/logs/`

Die Verzeichnisse wurden als echte Ordner mit `.gitkeep`-Markern im Git angelegt, damit leere Laufzeitordner auch auf dem Repository-Branch persistent bleiben, ohne dass `__pycache__/` oder `*.pyc` eingecheckt werden.

## 🏗️ 3-Staaten-Architektur

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

## 🏗️ Architektur-Regeln

1. `core.inc` ist die Single Source of Truth für Spielerstatus, Bank, Home-State und Admin-Level.
2. Neue Module dürfen keine zweite, parallele Player-Status-Quelle einführen.
3. Alle monetären Änderungen laufen über zentrale State-/Bank-/DB-APIs.
4. DB-Schema wird über `DB_Initialize()` und `DB_Ensure*Schema()` idempotent abgearbeitet.
5. Include-Reihenfolge in `gamemode.pwn` bleibt bindend: Core → DB → State → Admin → Auth → Economy → Jobs → Laws → Housing/Vehicle → Leisure/Web.
6. Neue Systeme müssen in `README.md` und in `TO-DO.md` dokumentiert werden.

## 📘 Modulübersicht

### 📘 Staat und Verwaltung

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

### 📘 Wirtschaft und Kasse

- `state_bank_system.inc`
- `state_tax_system.inc`
- `city_economy_system.inc`
- `market_system.inc`
- `marketplace_system.inc`
- `contraband_system.inc`
- `dispatch_system.inc`
- `dealership_system.inc`
- `ammu_system.inc`

### 🏗️ Jobs, soziale und RP-Systeme

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

### 📘 Besitz, Fahrzeuge und Inventar

- `inventory_system.inc`
- `house_system.inc`
- `house_storage_system.inc`
- `accessory_system.inc`
- `vehicle_system.inc`
- `garage_system.inc`
- `tuning_system.inc`
- `ubb_vehicle_system.inc`
- `fuel_station_system.inc`

### 📘 Sicherheit, Justiz und Notfall

- `law_system.inc`
- `jail_system.inc`
- `court_system.inc`
- `prison_system.inc`
- `crime_system.inc`
- `emergency_system.inc`
- `dispatch_system.inc`
- `ems_hospital.inc`

### 📘 State-Ledger und Treasury-Integrität

- `state_ledger_system.inc`
- zentrale State-/Treasury- und Budget-Transfers mit konsistenter Protokollierung pro Staat
- `StateLedgerSystem_Transfer()` und `StateLedgerSystem_ShowSummary()` als gemeinsame Treasury-API

## 🧩 Control Panel und Web-Management

Das separate Control Panel ist ein eigenes Web-Modul außerhalb des SA:MP-Gamemodes. Es liest dieselbe SQLite-Datenbasis und ist für Monitoring, Status-Checks, Admin-Übersicht und Betriebsdaten ausgelegt.

### 📘 Verfügbar

- `control-panel/server.js`
- `control-panel/public/index.html`
- `control-panel/public/app.js`
- `control-panel/README.md`

### 🧭 Ziel

- Live-Status des Servers
- Übersicht der Staatskassen
- Spieler-, Admin-, Job- und Fraktionslisten
- Business-, Vehicle- und House-Übersichten
- spätere UCP-/Admin-Aktionen ohne Pawn-Blockade

## 🖥️ Plugins und Includes

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

## ✅ Live-Betrieb auf Ubuntu-VM: Übergabepunkte und harte Regeln

Die reale Produktionsvalidierung muss auf der Ubuntu-VM erfolgen, nicht im Container. Dort stehen die echten Laufzeitbestandteile zur Verfügung: `pawncc`, `qemu-i386`, `samp.service` / `omp-server.service`, `Compiler/samp03/scriptfiles/` und `control-panel/server.js`.

### 📘 Manueller Offline-Workflow der Tools

Die neuen `tools/*.py`-Helfer sind bewusst auf keinen Hintergrundbetrieb oder Daemon-Start ausgelegt. Sie dienen ausschließlich der manuellen Analyse im Kontext des Repo-Standes und bleiben dabei strikt offline:

- `tools/log_code_bridge.py` liest Logdateien aus `Compiler/samp03/server_log.txt`, `samp7780_bg.log` oder einem übergebenen Pfad, prüft relevante Muster und exportiert strukturierte JSON/YSON-Brücken.
- `tools/code_line_extractor.py` liest gezielte Codezeilen/Zeilenbereiche mit Zeilennummern (`nl`-Stil) aus `gamemodes/gamemode.pwn` und `gamemodes/modules/` und kann optional JSON/YSON-Exports schreiben.
- `tools/log_guided_autocorrect.py` arbeitet standardmäßig im `--dry-run`-Modus und zeigt Vorschau/Diff, bevor mit `--apply` echte Backups und Korrekturen geschrieben werden.
- `tools/debug_trace_collector.py` sammelt kompakten Diagnose-Auszug aus Logs, `tools/*.py`-Ausgaben, SQLite-Dateien (read-only) und `Compiler/samp03/`-Artefakten ohne Serverstart.

### 📘 Kritische Übergangspunkte

1. Dini-first vs. Control Panel
   - Das Web-Dashboard ist offiziell als SQLite-Read-Only-Lesezugriff konzipiert.
   - Wenn der Gamemode im Alltag primär per Dini in `Compiler/samp03/scriptfiles/` schreibt, muss jeder relevante Save-Pfad entweder synchron in SQLite upserted werden oder das Node.js-Backend muss die INI-Dateien direkt lesen.
   - Ohne diese Synchronisation bleiben Staatskassen, Häuser und Spielerstatus in `control-panel/server.js` veraltet oder leer.

2. Atomarität bei Dini statt SQL-Transaction
   - `DB_BeginTransaction()`/`BEGIN EXCLUSIVE TRANSACTION` ist für SQL-Backends geeignet, aber Dini schreibt direkt in Textdateien ohne Transaktions-Rollback.
   - Für kritische Pfade (`StateLedgerSystem_Transfer()`, Haus-Safes, Fahrzeug- und Inventar-Transfers) muss die Speicherlogik absichtlich robust sein: temp-Datei schreiben, atomisch umbenennen, dabei keine halben Zustände zulassen.
   - Wenn ein Server-Crash mitten im Schreibvorgang auftritt, dürfen keine Werte als "abgebucht aber nicht gutgeschrieben" in scriptfiles/ landen.

3. Include-Reihenfolge und Guard-Layer
   - Die Reihenfolge `Core → DB → State → Admin → Auth → Economy → Jobs → Laws → Housing/Vehicle → Leisure/Web` bleibt bindend.
   - `core.inc` und `production_guard.inc` müssen mit `pawncc` gegen den echten Compiler laufen, damit Makro-, Tag- und Include-Konflikte sichtbar werden.
   - Gerade Dini-Wrapper und `DB_*`-Signaturen müssen gegen die reale Pawn-Toolchain geprüft werden, nicht nur in der Container-Struktur.

4. Echte scriptfiles/-Verzeichnisstruktur unter Linux
   - Dini legt Unterordner nicht automatisch an.
   - Vor dem Start der SAMP-/OMP-Daemons müssen alle erwarteten Ordner in `Compiler/samp03/scriptfiles/` physisch existieren, etwa für Spieler, Häuser, Safes, UBB-Fahrzeuge und die Staatskassen LS/SF/LV.
   - Schreibrechte und Dateierstellung müssen sauber auf den Servernutzer passen.

### 📊 Ubuntu-VM-Verifikation

Realer Live-Check auf der Ubuntu-VM:

```bash
cd /home/youruser/Nexus-Core-Gamemode
git pull --ff-only
qemu-i386 Compiler/pawncc/pawncc \
  -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" \
  gamemodes/gamemode.pwn \
  -iincludes \
  -i"$PWD/Compiler/samp03/include" \
  -d3

find Compiler/samp03/scriptfiles -maxdepth 2 -type d | sort
ls -ld Compiler/samp03/scriptfiles Compiler/samp03/scriptfiles/*
```

Danach wird verifiziert:

- `gtasa_reallife.amx` wird gebaut
- keine Makro-/Tag-Fehler aus der Dini-Umstellung
- `scriptfiles/` enthält die erwarteten Verzeichnisse mit Schreibrechten
- `samp.service`, `samp2.service` oder `omp-server.service` startet ohne Runtime-Fehler
- `control-panel/server.js` zeigt denselben Stand wie die Dini-Dateien bzw. liest dieselben Daten synchronisiert

### 📘 Abnahme-/Erfolgs-Kriterien

Die Übergabe gilt als erfüllt, wenn alle folgenden Kriterien in der echten Ubuntu-VM gleichzeitig erfüllt sind:

- Kompilierung: `pawncc` via `qemu-i386` läuft ohne blockierende Fehler und erzeugt die aktuelle `.amx` im `Compiler/samp03/gamemodes/`-Pfad.
- Persistenz & Reconnect: Registrierung, Login, Home-State-Wahl LS/SF/LV und Reconnect schreiben erwartete Schlüssel-Wert-Einträge in `scriptfiles/` und laden sie identisch wieder.
- Autosync & Control Panel: Nach einem Dini-Write landet der gleiche Stand in SQLite bzw. wird im Panel lesend konsistent dargestellt, ohne falsche Annahmen über Schreibrechte.
- Mirror & Doku: Änderungen an Modulen, c++-Spiegeln und Dokumentation bleiben zueinander synchronisiert; veraltete Pfade und tote Verweise sind nicht mehr enthalten.

## 📊 Verifikation und Einschränkungen

Dieser Container ist nicht die reale Produktionsumgebung; dort müssen die echten Ubuntu-VM-Checks laufen. Die Verifikation im aktuellen Container ist daher nur Teil-Validierung und keine endgültige Live-Bestätigung. Die inhaltliche Prüfung erfolgte über:

- strukturelle Konsistenz der Module
- Include-/Abhängigkeitsprüfung
- DB-Schema- und SQL-Constraint-Review
- Hook-/Lifecycle-Logik-Review
- Dini-first-/Fallback-Review und kritische Übergangspunkte
- Kontrollpanel- und API-Checks auf projektinternes Setup
- Sanierung kritischer Makro-/Include-Definitionskonflikte, damit die Core-, DB- und Guard-Layer in einer deterministischen Reihenfolge verwendbar sind

Ergebnis: Die Build- und Architektur-Validierung ist sauber, aber die vollständige Live-Prüfung auf der Ubuntu-VM mit `pawncc`, `qemu-i386`, `samp.service` und den realen Dini-Pfaden ist der entscheidende letzte Gate.

## 📊 Stabilisierung und Release-Status

Die Projektbasis wurde auf einen konsistenten, überprüfbaren Build-/Architektur-Stand gehoben. Nach der letzten Review- und Stabilisierungslauf wurde eine zentrale Korrektur durchgesetzt: gemeinsame Grenzwerte für Inventar, House-Storage und UBB-Fahrzeuge werden jetzt zentral in `core.inc` und `production_guard.inc` definiert, um Include-Reihenfolge- und Präprozessor-Konflikte zu vermeiden.

Zusätzlich wurden die kritischen Übergänge auf einen konsistenten Datenfluss gebracht:

- DB-Transaktions-Wrapper für optionale SQL-Sync-/Audit-Pfade
- definierte Hook-Reihenfolge im Connect-/Disconnect-Lifecycle
- Guards für Array-Slots und Stack-Grenzen
- State-Kassensystem als gemeinsame Single Source of Truth im Dini-first-Livepfad
- Schema-Definitionen für Inventar, House-Safe und UBB-Fahrzeuge mit validierten Constraints

Damit bleibt das Projekt als modularer, erweiterbarer Ausgangspunkt für ein reales 3-Staaten-Reallife-System nachvollziehbar, sauber strukturiert und für die letzte reale Ubuntu-VM-Validierung vorbereitet. Ein formeller Release-Status gilt erst mit erfolgreichem Live-Start, sauberem `pawncc`-Build und erfolgreichem Dini-/SQLite-/Control-Panel-Check auf der Ziel-VM.

## 📘 Projekt-Dokumentation

- `SEASON.md` – vollständige Season- und World-Progression-Konzeption für das RP-System
- `HowToSetup.md` – komplette Setup-, Server-, V-Server- und Deployment-Anleitung
- `Compiler/samp03/plugins/README.md` – dokumentiertes Plugin-Layout für den 7780-Contract mit transparentem Hinweis auf echte VM-/Host-Binaries und ohne erfundene Placeholder-Dateien

## 📘 Nächste Entwicklungsstufen

### 📘 Phase 1 – Production-safe persistence (architektonisch umgesetzt)

- vollständige DB-Load-/Save-Interfaces für Spieler-, House-Storage-, Inventory- und UBB-Fahrzeugpfade eingebaut
- zentrale Repair-/Validation- und Transaction-Guards für kritische Tabellen etabliert
- konsistente Cash-/State-/Bank-Bridge zwischen Core-, State- und DB-Layer ergänzt
- Backup-/Restore-Snapshot-Pfade als strukturierte Scaffold- und Operations-API definiert

### 🖥️ Phase 2 – Live-Gamemode-Validierung

- SA:MP/Pawn-Toolchain einrichten
- echtes Kompilieren des Gamemodes auf einem lokalen Server
- Verhalten unter mehreren Spielern testen
- echte DB- und State-Integration mit Live-Serverdaten verifizieren
- Datensynchronisation zwischen State-Logik und SQLite prüfen

### 🏗️ Phase 3 – Erweiterung des RP-Ökosystems

- tieferes Job- und Fraktions-Gameplay
- politische Entscheidungssysteme
- zusätzliche Events, Stadtinteraktionen und Dienstleistungs-Module
- noch stärkere Unterscheidung zwischen LS/SF/LV pro System

## 📘 Abschluss

Die Projektbasis ist jetzt als konsistenter modularer RP-Framework-Stand dokumentiert und für den aktuellen Build-/Architektur-Abschnitt verifiziert. Die Dokumentation ist breit genug für echtes Reallife-Gameplay, stabil genug für produktionsnahe Architekturreflexionen und modular genug, um künftig einzelne Komponenten ohne kompletten Rewrite zu erweitern. Eine formelle Live-Release-Freigabe bleibt jedoch explizit offen, bis der echte Ubuntu-VM-Produktionslauf mit Kompilierung, Dini-Persistenz, Autosync, Panel-Validierung und Server-Daemonen bestanden ist. Der im Repository gültige Runtime-Contract bleibt der 7780-Tree unter `Compiler/samp03/`, nicht Port 7777.

---

## 📊 Finaler Stand des Projekts

Der aktuelle Stand des Repositorys ist bewusst auf Stabilität, Source-Validity und realistische Runtime-Governance ausgerichtet. Es gilt weiterhin das harte Build-Contract: nur `Compiler/samp03/gamemodes/gtasa_reallife.amx` ist die gültige Ausgabe, der Build läuft über die lokale 32-Bit-Toolchain mit `qemu-i386`, und `core.inc` bleibt die zentrale Quelle für State-/Home-State-/Admin-/Project-Leader-Informationen.

### 📘 Verifiziertes Source-Level

- Single-Target-Output und Build-Sync mit `server.cfg`
- Lifecycle- und Include-Reihenfolge in `gamemode.pwn` unverändert eingehalten
- `INSERT OR IGNORE` und 0-Schutz bei kritischen State-Werten erhalten
- `strmid`-basierte Token-Extraktion beibehalten
- `INVALID_TEXT_DRAW`- und `SetCameraBehindPlayer()`-Baselines erhalten
- Dini-first-Temp/Validate/Rename-Pfade für kritische Transfers sowie SQLite-Read-Only-Sicht konsistent dokumentiert
- qemu-i386-Hotpath-Guards für `TextWars_Tick`, `UBBVehicleSystem_Connect` und ähnliche Laufzeit-Sensiblitäten beibehalten

### 📊 Offener, aber klar definierter Status

Der Code ist auf den aktuellen Standards des Projekts stabil und konsistent, aber der reale Live-Start bleibt ein separates Freigabe-Gate auf der Ubuntu-VM. Es wurde kein echter Serverstart auf Port 7780 oder 7777 unter Live-Betrieb durchgeführt, und genau dieser Zustand bleibt ausdrücklich gültig, bis die reale Runtime-Validierung freigegeben wird.

### 📘 Kurzfazit

Damit ist das Repository in einen sauberen, dokumentierten und architektonisch verständlichen Zwischenstand gebracht worden: Build-Kontrakt erfüllt, Hardening-Muster erhalten, Persistenzstrategie vorgezeichnet und der Proof-Point für die nächsten Live-Checks eindeutig definiert. Die endgültige Produktionsfreigabe bleibt weiterhin an den echten Live-Test gebunden.

---

## 📊 Additive Verifikationsnotiz – Q4 Source-Härtung

Diese Notiz ergänzt die bisherigen Dokumentationsabschnitte ohne sie zu überschreiben.

- Reale Build-Blocker direkt im Quellbaum bereinigt: fehlende `GetServerTime()`-Aufrufe wurden auf das in der Pawn-Toolchain verfügbare `gettime()`-Contract zurückgeführt; dadurch entfielen echte Compile-Fehler unter `qemu-i386`.
- Fehlende DB-Schnittstelle behoben: `DB_GetPlayerHomeState()` wurde in `gamemodes/modules/db/db_interface.inc` ergänzt, damit `state_system.inc` und die Connect-/Rehydrate-Pfade gegen gültige `home_state`-Werte wieder konsistent laufen.
- Premium-Persistenz bleibt Dini-first: `premium_system.inc` nutzt weiterhin den scriptfiles-/Dini-Pfad und validiert Ablauf-/Tierzustände ohne parallelen Shadow-State.
- Build-Check wieder grün: `bash scripts/build_gamemode.sh` lief erfolgreich durch und erzeugte `Compiler/samp03/gamemodes/gtasa_reallife.amx` ohne Live-Serverstart.
- Offline-Checks liefen durch: Python-Compile, Shell-Syntax, JS-Check (falls verfügbar) und die statischen Guard-Skripte blieben ohne Fehler.
- Offener Status bleibt bewusst unverändert: echte Live-Validierung auf Port 7780 muss weiterhin auf der realen Ubuntu-VM erfolgen; sie wurde hier nicht unterstellt.

---

## 📘 Additive Ist-Analyse – Gesamtprojekt 2026-11-20

Diese Ergänzung erweitert die vorhandene Dokumentation ohne bestehende Abschnitte zu überschreiben.

- Gesamt-Stand: Der Quellbaum ist als modularer Gamemode mit klarer Kern-/State-/DB-/UI-Architektur aktiv. Der zentrale Build- und Laufzeitcontract bleibt auf `Compiler/samp03/gamemodes/gtasa_reallife.amx`, `gamemode0 gtasa_reallife 1` und `port 7780` fixiert; Port `7777` bleibt kein aktiver Projekt-Contract.
- Core-/State-Quelle: `Compiler/samp03/gamemodes/modules/core/core.inc` bleibt die Single Source of Truth für `state`, `home_state`, Bank, Admin-/Projektleiterrechte und gemeinsame Validierung. Das gilt auch für `Core_IsValidState()`, `Core_GetValidStateOrDefault()`, `Core_SetPlayerState()` und `Core_SetPlayerHomeState()`.
- Lifecycle: `Compiler/samp03/gamemodes/gamemode.pwn` setzt die aktive Reihenfolge `BOOT -> DB_READY -> STATE_READY -> AUTH_READY -> WORLD_READY -> RUNTIME_READY`, mit Include- und Initialisierungsreihenfolge in der beschriebenen Architektur. Die Strukturen sind im derzeitigen Source-Stand aktiviert und konsistent.
- Dini-/SQLite-/MySQL: `Compiler/samp03/scriptfiles/` bleibt der Dini-first/Laufzeitpersistenzpfad mit `temp -> validate -> rename`-Design; `control-panel/server.js` liest `data/db/gtasa_reallife.db` schreibgeschützt und dient der Betriebsansicht. MySQL ist im aktuellen Quellbaum nur als Stub-/Nachlauf-Backend vorhanden und nicht als aktive Write-Route verknüpft.
- State-Guard: Die 0-Reset-Schutzlogik ist im Core-/State-/Login-/Disconnect-Stack aktiv. Gültige `state_id`- und `home_state`-Werte werden nicht auf 0 normiert; stattdessen werden Fallbacks auf valide Zustände verwendet.
- Guardrails: `TextWars_Tick`, frühe `If -> Return`-Guards, `SetCameraBehindPlayer()`, `TogglePlayerControllable`, `INVALID_TEXT_DRAW`-Konventionen und Wohn-/State-Normalisierung sind im Code als aktive Schutzmechanismen vorhanden.
- Build-Status: Der aktuelle Container fehlt die 32-Bit-Pawn-Toolchain; `bash build_server.sh` endet mit `qemu-i386: command not found` und `Compiler/samp03/gamemodes/gtasa_reallife.amx` ist im Container nicht produziert. Das betrifft die reale Kompilierung, nicht die Source-/Architektur-Analyse.
- Open Gaps: Der echte Live-Test auf Port 7780, die vollständige Dini-zu-SQLite-Transparenz in der Laufzeit, die finale AMX-Kompilierung und die tatsächliche serverseitige Prüfung der `scriptfiles/`-Struktur bleiben für die Zielumgebung offen und sind keine Release-Freigaben.

---

## 📘 Additive Validierungsnotiz – 2026-11-20

Diese Ergänzung erweitert den vorhandenen Status ohne bestehende Abschnitte zu verändern.

- Source-Härtung: In `Compiler/samp03/gamemodes/modules/auth/login_system.inc` und `Compiler/samp03/gamemodes/modules/core/core.inc` wurden frühe Connect-/Disconnect-Guards verschärft, damit gültige `state_id`/`home_state`-Werte nicht durch vorzeitige Default-Resets verworfen werden. Die Logik nutzt `Core_GetValidStateOrDefault()` vor einer Normalisierung oder einem Reset.
- SQLite-/Persistenzstand: `data/db/gtasa_reallife.db` wird als lokale Grundlage für die lesende Control-Panel-Perspektive behandelt; die Laufzeit-Persistenz bleibt Dini-first unter `Compiler/samp03/scriptfiles/` mit atomarem `temp -> validate -> rename`-Muster und ohne Live-Serverstart.
- Offline-Checks: `tools/qemu_7780_readiness_check.py` und `tools/audit_modules_and_guards.py` wurden ausgeführt. Der statische Check bestätigt die 7780-Architektur- und Single-Target-Contract-Regeln, während der Build-Pfad durch das fehlende `qemu-i386`-Binary in der aktuellen Umgebung blockiert ist.
- Build-Status: `bash build_server.sh` endet aktuell mit `qemu-i386: command not found`; damit bleibt die Quellvalidierung sauber, aber die reale AMX-Kompilierung auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` in dieser Umgebung ausgesetzt.
- Freigabe-Gate: Kein Live-Server auf Port 7780 oder 7777 wurde gestartet; der verbleibende Testbedarf ist weiterhin ein echter Laufzeit-Check auf Port 7780 mit funktionierendem `qemu-i386`/Pawn-Toolchain, validierter Scriptfiles-Struktur und auslaufender SQLite-/Dini-Abgleichung.

---

## 📘 Additive Abschluss-Validierung – 2026-11-21

Diese Ergänzung schließt den aktuellen Source-/Build-Lauf ab, ohne bestehende Abschnitte zu entfernen.

- `LoginSystem_AutoResolveFlow()`, `LoginSystem_AdvanceToStateSelection()`, `TutorialSystem_Next()`, `OnPlayerSpawn()` und `CameraSystem_ExitCameraMode()` wurden im aktuellen Build-Stand auf den festen 7780-Contract mit validem State-/Home-State-Fallback geprüft.
- Das Passwort-Only-Onboarding bleibt auf `DIALOG_STYLE_INPUT`, während Home-State-/Tutorial-Auswahl über Dialog-/Menüpfade laufen. Veraltete Chat-/Befehls-Platzhalter im Text wurden nicht erneut in die UI-Flusslogik eingeführt.
- Die Reconnect-/Spawn-Sicherung bleibt auf `Core_IsValidState()`, `Core_GetValidStateOrDefault()` und `DB_ValidateStateTransfer()` gebunden; gültige Zustände werden nicht in einem späteren Spawnpfad mit `0` überschrieben.
- Camera-/HUD-Reset konnte in der aktuellen Source-Architektur sauber über `SetCameraBehindPlayer()` + `TogglePlayerControllable(playerid, 1)` und `INVALID_TEXT_DRAW`-Checks abgeschlossen werden.
- Der gezielte Compile-Lauf auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` war erfolgreich; kein Live-Serverstart wurde im Zusammenhang mit dieser Freigabe ausgeführt.


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
