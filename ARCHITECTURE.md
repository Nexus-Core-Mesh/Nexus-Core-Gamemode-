# Architektur — Nexus-Core-Gamemode

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
| Dokument | `ARCHITECTURE.md` |
| Typ | Architektur-Doku |
| Fokus | Systemaufbau, Module und Persistenzstruktur |
| Wichtige Pfade | `gamemodes/modules/`, `Compiler/samp03/scriptfiles/`, `data/db/` |

---
## 🧭 1. Überblick und Ziel der Systemarchitektur

Nexus-Core-Gamemode ist ein modularer GTA:SA-Reallife-Gamemode für SA:MP, der bewusst nicht als eine einzelne monolithische `gamemode.pwn`-Datei aufgebaut ist. Stattdessen basiert das Projekt auf einer klaren mehrschichtigen Architektur mit eigenständigen Bereichen für Core, State, Auth, Economy, Jobs, Factions, Housing, Inventory, UI, Vehicles, Law, World, Kommunikation und Web-Management.

Die Kernidee ist eine saubere Trennung von Verantwortlichkeiten, damit das System erweiterbar, testbar und für spätere Produktionsläufe stabil bleibt. Die grundlegenden Architekturbereiche sind:

- `gamemodes/gamemode.pwn` – Master-Entry-Point und Boot-Skript des gesamten Gamemodes
- `gamemodes/modules/` – komplette modulare Funktionalität von Core bis World
- `includes/` – SA:MP- und Projekt-Includes
- `config/` – Server- und Konfigurationsdateien
- `Compiler/samp03/scriptfiles/` – primärer Dini-Persistenzpfad für INI-Style-Dateispeicherung
- `data/db/` – optionale lokale SQLite/MySQL-Autosync-Schnittstelle, nicht als primäre Source of Truth für den laufenden Serverzustand
- `Compiler/` – lokale Pawn-Toolchain und Serverruntime
- `Compiler/samp03/` – laufende SA:MP-Serverumgebung
- `scripts/` – Setup, Build, Deployment, Debugging

Das wichtigste Designprinzip lautet: Der Quellcode wird im Root entwickelt, gebaut und kompiliert, während der Serverordner nur die Laufzeitbinary und runtime-required Files enthält. Dadurch bleibt die Deployment-Kette reproduzierbar und frei von veralteten Alias-Dateien. Die Architektur ist bewusst auf den Dini-first-Laufpfad ausgelegt; SQLite/MySQL sind optionale Konsistenz- und Monitoring-Schnittstellen, nicht die primäre Runtime-Quelle.

### Projektgewichtung Persistenz: 51 % SQLite / 49 % Dini

Die projektweite Persistenzstrategie bleibt bewusst hybrid:

- SQLite (51 %): primäre Konsistenz- und Synchronisationsschicht für Datenmodelle, Schema-Integrity, Upserts und Read-Only-/Dashboard-Views (`data/db/gtasa_reallife.db`, `control-panel/server.js`).
- Dini (49 %): primäre Runtime-/Save-Quelle unter `Compiler/samp03/scriptfiles/` mit `temp -> validate -> rename`-Mechanik, Reparaturpfaden und klaren Fallback-Checks ohne parallele Shadow-State-Speicher.

Darin bleibt `gamemodes/modules/core/core.inc` die einzige Source of Truth für State- und Bank-/Cash-Werte; Dini- und SQLite-Pfade dienen der verlässlichen Persistenz und der Operationssicht, aber nicht als zweite State-Quelle im laufenden Serverzustand.

---

## 🏗️ 2. Verzeichnisstruktur und Dateizuordnung

```text
Nexus-Core-Gamemode/
├── ARCHITECTURE.md
├── HowToSetup.md
├── HOW_TO_PLAY.md
├── README.md
├── SEASON.md
├── TO-DO.md
├── server.cfg
├── sql.cfg
├── config/
│   ├── server.cfg
│   └── ...
├── data/
│   └── db/
│       └── gtasa_reallife.db
├── gamemodes/
│   ├── gamemode.pwn
│   └── modules/
│       ├── admin/
│       ├── auth/
│       ├── business/
│       ├── camera/
│       ├── commerce/
│       ├── communication/
│       ├── core/
│       ├── crime/
│       ├── db/
│       ├── drugs/
│       ├── economy/
│       ├── emergency/
│       ├── events/
│       ├── factions/
│       ├── government/
│       ├── housing/
│       ├── inventory/
│       ├── jobs/
│       ├── justice/
│       ├── law/
│       ├── leisure/
│       ├── logistics/
│       ├── management/
│       ├── marketing/
│       ├── medical/
│       ├── missions/
│       ├── production/
│       ├── progression/
│       ├── shops/
│       ├── social/
│       ├── state/
│       ├── transport/
│       ├── tutorial/
│       ├── ui/
│       ├── utility/
│       ├── vehicles/
│       ├── web/
│       ├── workers/
│       ├── world/
│       └── ...
├── includes/
│   └── ...
├── Compiler/
│   ├── pawncc/
│   │   ├── pawncc
│   │   ├── libpawnc.so
│   │   └── ...
│   └── samp03/
│       ├── server.cfg
│       ├── gamemodes/
│       │   └── gtasa_reallife.amx
│       ├── plugins/
│       ├── filterscripts/
│       ├── data/
│       │   └── db/
│       └── ...
├── scripts/
│   ├── setup_environment.sh
│   ├── build_gamemode.sh
│   ├── deploy_server.sh
│   ├── debug_server.sh
│   └── bootstrap_sqlite.py
├── control-panel/
├── tools/
└── ...
```

### 📘 2.1 Master-Entry-Point: `gamemodes/gamemode.pwn`

`gamemodes/gamemode.pwn` ist das Herz des Projekts. Dort werden die wichtigsten Module in einer klaren Initialisierungsreihenfolge eingebunden und gestartet. Die relevante Struktur im Code zeigt den kompletten Boot-Stack:

- `Core_SetLifecycleStage()`
- `DB_Initialize()`
- `StateSystem_Initialize()`
- `JobsCore_Initialize()`
- `LoginSystem_Initialize()`
- `GovernmentSystem_Initialize()`
- `VehicleSystem_Initialize()`
- `JobCenter_Initialize()`
- `TextWars_Initialize()`
- `UiHub_Initialize()`
- `Core_SetLifecycleStage(CORE_LIFECYCLE_RUNTIME_READY)`

Das `OnGameModeInit()`-Bootsystem ist die zentrale Initialisierung, die entscheidet, in welcher Reihenfolge die wichtigsten Subsysteme geladen werden. Dazu gehört auch die Module-Contract-Registrierung, die den Zustand der Server-Initialisierung kontrolliert und die Systemstufen sauber hält.

### 🏗️ 2.2 Modulstruktur der eigentlichen Funktionalität

`gamemodes/modules/` bildet das im Projekt am stärksten ausgeprägte Designprinzip. Jede große Funktionalität ist isoliert und klar benannt.

#### Core
`core/` enthält die allgemeinen Runtime- und Systemwerte:

- Lifecycle-Status
- Runtime-Registrierung
- gemeinsame sprachliche und technische Guard-Checks
- globale Zustands- und Berechtigungslogik

#### Auth & Tutorial
`auth/` und `tutorial/` bilden den Einstieg in die Welt:

- Login- und Register-Fluss
- Home-State-Zuordnung
- Spawn- und Charakterinitialisierung
- Intro-/Tutorial-Nummerierung und erste Hilfe

#### State & Government
`state/` und `government/` modellieren die politische und wirtschaftliche Struktur der Welt:

- Drei-Staaten-System: LS, SF, LV
- Staatskassen
- Stadtverwaltungen
- Gesetze und Treasuries
- Distikt- und Territorialverwaltung

#### Economy & Shops
`economy/` und `shops/` bilden das Geld- und Handelsnetz:

- Kassen und Steuerlogik
- Märkte
- Fahrzeuge, Waffen und Ludotheken
- Wirtschaftssysteme und Verbrauchslogik

#### Jobs & Factions
`jobs/` und `factions/` arbeiten zusammen:

- legale Jobs und dynamische Aufgaben
- Fraktionszugehörigkeit
- Gang- und Wars-Mechanik
- Textdraw-/UI-basierte Strategieansicht

#### Housing, Inventory, Vehicles
Diese Module bilden die Grundlage für Besitz, Lagerung und mobile Spielmechanik:

- Häuser
- Safes und Storage
- Inventare
- Fahrzeuge
- Garagen
- Schlüssel, Motoren und UBB-Mechaniken

#### UI & Web
`ui/` und `web/` sorgen für Interaktion, Alltag und Betriebsmonitoring:

- UI-Hub
- State-Control-UI
- Dialoge, Textdraws und Menüs
- Web-Bridge für Auswertungen und Monitoring

### 📘 2.3 Includes und Hilfsbibliotheken

Die Includes sind nicht nur Nebensache, sondern Teil der Spiel- und Compiler-Architektur. Das Projekt nutzt SA:MP-Standardincludes sowie projektspezifische Hilfsbibliotheken. Diese Datei- und Codeebene macht den Quellcode lesbar, stark typisiert und teils modular aufgeteilt.

Wichtige. Aspekte:

- Standard-SAMP-Funktionen
- sscanf-Parsing für Befehle und Argumente
- Datenbankabstraktionen
- Hilfskonstanten und Shared-Utilities
- Modul- und UI-Schnittstellen

Im Build-Workflow werden these Includes über `-iincludes` und `-iCompiler/samp03/include` eingebunden, damit die Pawn-Compiler-Ausführung den kompletten Build-Kontext kennt.

### 📘 2.4 Compiler- und Toolchain-Schicht

`Compiler/` ist die lokale Build-Umgebung. Sie ist bewusst physisch im Projekt enthalten, damit das Projekt reproduzierbar gebaut werden kann. Die beiden kritischen Bestandteile sind:

Die Laufzeitstruktur unter `Compiler/samp03/scriptfiles/` ist ebenfalls Teil dieser Umgebung und wurde komplett auf die Dini-first-Save-/Load-Architektur ausgerichtet. Die kritischen Laufzeitordner wurden ergänzend angelegt, damit `temp` → prüfen → atomisch rename für Spieler-, House-, Safe-, Vehicle- und State-Daten nicht unvermittelt an fehlenden Ordnern scheitert:

- `players/`
- `houses/`
- `house_storage/`
- `safes/`
- `vehicles/`
- `ubb_vehicles/`
- `state/LS/`, `state/SF/`, `state/LV/`
- `state_kasse/LS/`, `state_kasse/SF/`, `state_kasse/LV/`
- `business/`, `factions/`, `inventory/`, `temp/`, `backup/`, `logs/`

Dadurch bleibt der Laufpfad für den 7780-Contract konsistent und frei von stummen Ordnerfehlern bei Offline-Checks oder späterem Host-Deploy.

- `Compiler/pawncc/pawncc` – der lokale 32-Bit-Pawn-Compiler
- `Compiler/samp03/` – der Laufzeitordner des tatsächlichen Servers

Der Plugin-Bereich unter `Compiler/samp03/plugins/` ist ebenfalls Teil dieser Laufzeitumgebung, aber bewusst rein dokumentarisch organisiert, sofern keine verlässlichen Ziel-VM-Binaries vorliegen. Der Repo-Stand hält keine erfundenen oder vorgeschobenen Plugin-Binaries; der real vorhandene Plugin-Stack (`crashdetect`, `streamer`, `sscanf2`, `sqlite`, `mysql`, `zcmd`, `foreach`, `MapAndreas`, `pawn-plus`) ist als offener Stack dokumentiert, mit klarer Unterscheidung zwischen Git-Repo und Ziel-VM-Installationen.

Auf 64-Bit-Hosts muss dieser Compiler über `qemu-i386` ausgeführt werden, da es sich um ein 32-Bit-Binary handelt. Genau deshalb gibt es die Logik:

```bash
LD_LIBRARY_PATH="$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" qemu-i386 Compiler/pawncc/pawncc -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" gamemodes/gamemode.pwn -iincludes -iCompiler/samp03/include -d3
```

Damit bleibt die Toolchain konkret und unabhängig von externen Installationen. Der echte Single-Target-Build pflegt nur diese eine AMX-Ausgabe; keine Duplikate unter `Compiler/samp03/gamemodes/` oder im Projektroot bleiben stehen.

### 📘 2.5 Offline-Tooling und manuelle Diagnose-Pfade

Das Repository enthält zusätzlich schlanke, standardbibliotheksbasierte Python-Helfer unter `tools/`, die ohne Live-Serverstart und ohne Hintergrundprozess arbeiten. Sie sind gezielt für die Offline-Analyse konzipiert und decken die wichtigsten auditierbaren Bereiche ab:

- `log_code_bridge.py` – Logs -> Source-Zuordnung mit vereinfachter JSON/YSON-Bridge
- `code_line_extractor.py` – Zeilen-/Match-Extraktion mit `nl`-ähnlicher Nummerierung
- `log_guided_autocorrect.py` – sichere Vorschau-/Diff-Analyse mit `--dry-run` und Backup bei `--apply`
- `debug_trace_collector.py` – kompakte Diagnose aus Logs, SQLite-State, scriptfiles und Build-Artefakten

Diese Werkzeuge nutzen nur die Standardbibliothek, sind manuell per CLI aufrufbar und bleiben bewusst außerhalb eines autostartenden oder daemonisierten Betriebs.

### 📘 2.5 Laufzeit- und Kantenfall-Guardrails

Im aktuellen Stand wurden die früheren Runtime-Risiken nicht bloß dokumentiert, sondern in den geeigneten Funktionspfaden direkt abgesichert:

- `DB_RehydratePlayerState()` prüft `state_id` und `home_state` mit `Core_IsValidState()` und überschreibt keine gültigen Werte mit `0` oder Fallbacks.
- `OnPlayerSpawn()` erkennt einen bereits validen `home_state`/`state_id` und startet weder Tutorial noch State-Szene erneut.
- `CameraSystem_Reset()` und `CameraSystem_ExitCameraMode()` sorgen für einen perfiden Reset-Pfad mit `SetCameraBehindPlayer()` und `TogglePlayerControllable(playerid, 1)`.
- `UiHub_`-Pfad und `RoleplayUI_`-Cleanup verwenden nur sichere `INVALID_TEXT_DRAW`-Prüfungen, um veraltete HUD-/Textdraw-Handles beim Wechsel von Menü, Spawn und Disconnect aufzuräumen.

Diese Vereinbarungen sind absichtlich kein „Wiederverschieben“ apt-ähnlicher Änderungen, sondern eine direkte Sicherheitsschicht für den Connect-/Spawn-/HUD-Flow.

### 🖥️ 2.5 Admin-/Projektleiter-Workflow und runtime contract

Im laufenden Spielbetrieb wurde der Administrations- und Projektleiter-Export bewusst als separater, nicht-öffentlich sichtbarer Zugriff ergänzt. Die Kernidee ist: normale Spieler-Hilfen und Standardmenüs bleiben unverändert, während autorisierte Admins und Projektleiter im System über eine identische Core-Prüfung und einen zentralen State-/Spawn-Flow Zugang erhalten.

Die relevanten Sicherheits- und Runtime-Regeln lauten:

- `Core_IsProjectLeader()` setzt den Projektleiter-Status ausschließlich über das zentrale `gPlayerData`-Profil und erlaubt keine zweite parallele Statusquelle.
- `AdminSystem_IsProjectLeader()` prüft zusätzlich den globalen Owner-/Admin-Pfad und hält den Zugriff sauber separiert von normalen Spieler-Menüs.
- `/projectlead`, `/setstate` und `/spawnvehicle` laufen über `AdminSystem_HandleCommand()` und nutzen die vorhandenen State-/Spawn-/Vehicle-Guards statt unkontrollierten direktem Schreiben in globale Arrays.
- `StateSpawnSystem_Apply()` bleibt der single entry point für State- und Spawn-Applikation; der Kamerarücksetzpfad läuft über `CameraSystem_Reset()` beziehungsweise `SetCameraBehindPlayer()` und verhindert so eine Rekursion in Tutorial-/State-Schleifen.
- Fahrzeugspawns gehen über `VehicleSystem_SpawnAdminVehicle()` und respektieren die Slot-/Limit-Checks des Fahrzeugsystems statt rohe Slot-Array-Schreibzugriffe im Admin-Context zu erlauben.
- Die Laufzeit-Hotpath-Guardrails wurden ergänzt: `TextWars_Tick()` prüft vor jedem Refresh aktivierte Gangfights und sichtbare Player-Widgets und beendet den Tick früh, sobald keine echte Aktivität mehr vorliegt. Damit bleiben die UI- und Faction-Hotpaths robust unter der qemu-i386-Umgebung mit begrenzten Ressourcen.

Diese Regeln sind bewusst zusammen mit dem vordefinierten Core-Contract in der Architektur verankert, damit spätere Module denselben Single-Source-of-Truth- und Hardening-Pfad verwenden.

### 🔧 2.5 Live-Server-Root und runtime contract

`Compiler/samp03/` ist das aktive SA:MP-Laufzeit-Root. In diesem Ordner liegt
a real laufende Serverumgebung:

- `server.cfg`
- `samp03svr`
- `plugins/`
- `filterscripts/`
- `gamemodes/`
- `data/db/`

Das entscheidende Laufzeit-Contract lautet:

```text
Compiler/samp03/gamemodes/gtasa_reallife.amx
```

und

```cfg
gamemode0 gtasa_reallife 1
```

Diese beiden Einträge müssen identisch zusammenpassen. Jede Abweichung führt zu Laufzeitproblemen wie falschem Modus- oder Spawnfluss.

### 📘 2.6 Dini-Persistenz und Fallback-Model

Das Projekt bootet jetzt Dini-first und nutzt den Pfad:

```text
Compiler/samp03/scriptfiles/
```

Die persistente Laufzeitbasis liegt dabei im INI-/Dateifallsystem, mit optionalem SQLite/MySQL-Autosync im laufenden Betrieb. Damit werden diese Datenbereiche abgesichert:

- Login- und Charakterdaten
- Inventar- und Safe-Strukturen
- Häuser und Fahrzeugsysteme
- Staatskassen und Wirtschaftslogs
- Zustandsdaten aus Economy, Jobs, Factions, Inventory und Fahrzeuge

Das Dini-Setup ist Teil der Start- und Runtime-Initialisierung, und die erforderlichen Verzeichnisse müssen vor dem Start der Daemons physisch existieren. Die Pflege dieser Strukturen ist Teil der produktiven Betriebsbereitschaft, nicht nur ein optionaler Fallback.

Wichtiger Produktionshinweis für Ubuntu-VM/Linux:

- `Compiler/samp03/scriptfiles/` ist der primäre Persistenzpfad, aber Unterordner wie `players/`, `houses/`, `safes/`, `vehicles/` und `state/` müssen vor dem Start der Daemons physisch angelegt werden.
- Dini schreibt zeilenweise in Textdateien; es gibt keine SQL-Transaktions-Atomicity. Deshalb müssen kritische Transfers als atomare Datei-Operationen umgesetzt werden: write-temp → validate → rename.
- Der optional Sync zu SQLite/MySQL darf nicht als alleinige Datenquelle dienen, weil `control-panel/server.js` laut Spezifikation read-only auf SQLite arbeitet und deshalb veraltete oder leere Werte anzeigen kann, wenn Dini-Änderungen nicht auch mit einem konsistenten Upsert propagiert werden.

### 🖥️ 2.7 Produktions-Checkliste für echte Live-Umgebungen

Vor dem Start von `samp.service`, `samp2.service` oder `omp-server.service` auf der Ubuntu-VM müssen diese Voraussetzungen erfüllt sein:

```bash
mkdir -p Compiler/samp03/scriptfiles/players \
         Compiler/samp03/scriptfiles/houses \
         Compiler/samp03/scriptfiles/safes \
         Compiler/samp03/scriptfiles/vehicles \
         Compiler/samp03/scriptfiles/ubb \
         Compiler/samp03/scriptfiles/backups \
         Compiler/samp03/scriptfiles/state/ls \
         Compiler/samp03/scriptfiles/state/sf \
         Compiler/samp03/scriptfiles/state/lv
chmod 755 Compiler/samp03/scriptfiles \
         Compiler/samp03/scriptfiles/players \
         Compiler/samp03/scriptfiles/houses \
         Compiler/samp03/scriptfiles/safes \
         Compiler/samp03/scriptfiles/vehicles \
         Compiler/samp03/scriptfiles/ubb \
         Compiler/samp03/scriptfiles/backups \
         Compiler/samp03/scriptfiles/state/ls \
         Compiler/samp03/scriptfiles/state/sf \
         Compiler/samp03/scriptfiles/state/lv
```

Danach gilt:

- `pawncc` muss den Gamemode ohne Compile-Fehler erzeugen
- `gtasa_reallife.amx` muss in `Compiler/samp03/gamemodes/` liegen
- `server.cfg` muss exakt `gamemode0 gtasa_reallife 1` enthalten
- `control-panel/server.js` muss dieselben Live-Daten wie Dini/INI lesen oder den SQLite-Upsert-Pfad synchron halten
- Spieler-, House-, Safe-, Fahrzeug- und State-Transfers müssen nach einem Crash nicht in halben Zuständen stehen

### ✅ 2.8 Atomicity- und Recovery-Regeln für Dini-Schreibpfade

Dini arbeitet ohne SQL-Transaktions-Logik. Für kritische Pfade gilt deshalb eine explizite Datei-Atomicity-Policy:

- `StateLedgerSystem_Transfer()` darf nur dann Geld aus einer Kasse abbuchen und einer anderen gutschreiben, wenn beide Schreibschritte als Teil einer durchgehend validierten Datei-Transaktion erfolgen.
- `house_storage_system.inc` und Fahrzeug-/Inventar-Transfers müssen temp-Dateien verwenden und nur nach erfolgreichem Schreiben/Validieren atomar umbenennen.
- Wenn ein Lesevorgang auf eine teilweise oder beschädigte INI-Datei trifft, muss der Loader einen Repair- oder Fallback-Pfad aktivieren und die betroffenen Werte als ungültig markieren, statt sie als gültige Daten zu akzeptieren.
- Der SQLite-Autosync darf nur als ergänzende, asynchrone Konsistenzschicht dienen. Er kann nicht als Ersatz für die Dini-Atomicity- und Recovery-Regeln fungieren.
- Dadurch bleibt der reale Datenfluss: Dini-first-Save → validieren → atomar umbenennen → optional SQLite-Upsert/Replication → read-only Panel-Query. Ein fehlender oder veralteter SQLite-Update-Pfad zählt als operativer Mismatch, nicht als normaler Zustand.

Damit bleibt die Datenbasis zwischen Dini-first-Storage und SQLite-Read-Only-Panel in einem konsistenten, prüfbaren Zyklus: Save → validate → atomic rename → optional SQLite upsert → panel read.

---

## 🏗️ 3. Modul- und Lifecycle-Architektur

Die Bootreihenfolge des Gamemodes ist nicht randomisiert. Sie ist bewusst aufgebaut und folgt den systematischen Lifecycle-Stufen im Code:

1. `CORE_LIFECYCLE_BOOT`
2. `CORE_LIFECYCLE_DB_READY`
3. `CORE_LIFECYCLE_STATE_READY`
4. `CORE_LIFECYCLE_AUTH_READY`
5. `CORE_LIFECYCLE_WORLD_READY`
6. `CORE_LIFECYCLE_RUNTIME_READY`

Diese Schichten sind wichtig, weil Module voneinander abhängig sind. Ein Job-System kann nicht sauber funktionieren, wenn die State- oder Auth-Layer nicht initialisiert sind. Analoges gilt für Häuser, Fahrzeuge, UI und Fraktionssysteme.

So erklärt sich die Reihenfolge im `OnGameModeInit()`:

- Core initialisieren
- DB initialisieren
- State initialisieren
- Economy/Market/Tax/Bank initialisieren
- Auth/Tutorial/Camera initialisieren
- Government/Housing/Business/Factions initialisieren
- Vehicles/Inventory/Workshop/UBB initialisieren
- UI, JobCenter, TextWars, StateControl und UiHub final aktivieren

Diese Reihenfolge sorgt dafür, dass alle Module mit gültigen, vorbereiteten Daten und Grundzuständen starten.

---

## 🔧 4. Build-, CI/CD- und Deployment-Pipeline

### 🔧 4.1 Lokaler Build

Der lokale Build folgt genau dieser Logik:

```bash
cd ~/Nexus-Core-Gamemode
mkdir -p data/db Compiler/samp03/gamemodes Compiler/samp03/data/db
qemu-i386 Compiler/pawncc/pawncc \
  -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" \
  gamemodes/gamemode.pwn \
  -iincludes \
  -iCompiler/samp03/include \
  -d3
```

Wesentlich ist hier:

- Source bleibt im Repository-Root
- Output ist strikt `gtasa_reallife.amx`
- nur eine Datei im Server-Ordner
- keine Doppelnutzung oder Alias-Dateien

### 📘 4.2 GitHub Actions Workflow

Die CI-Logik in `.github/workflows/build.yml` folgt dem gleichen Single-Target-Prinzip:

- Ordner vorbereiten
- Pawn-Compiler mit qemu-i386 ausführen
- Ausgabe als `Compiler/samp03/gamemodes/gtasa_reallife.amx`
- Artefakt hochladen

Der CI-Workflow ist damit synchron zum lokalen Build und verhindert ein Nicht-Match zwischen Build- und Runtime-Konfiguration.

### 🔧 4.3 Serverdeployment

`scripts/deploy_server.sh` setzt die Laufzeitkonfiguration und startet den Server mit korrektem `LD_LIBRARY_PATH`:

```bash
gamemode0 gtasa_reallife 1
```

Danach wird der tatsächliche SA:MP-Server aus dem Serverroot gestartet:

```bash
cd Compiler/samp03
LD_LIBRARY_PATH=.:./plugins:/usr/lib32:/usr/lib/i386-linux-gnu ./samp03svr
```

Damit ist die Laufzeitumgebung vollständig auf den aktiven Gamemode abgestimmt.

### 📘 4.4 Saubere Single-Target-Staging-Logik

Das Projekt verwendet bewusst ein Single-Target-Staging ohne Duplicate:

```text
Root-Source: gamemodes/gamemode.pwn
    ↓
Compiler: qemu-i386 Compiler/pawncc/pawncc
    ↓
Runtime Target: Compiler/samp03/gamemodes/gtasa_reallife.amx
```

Es gibt keine zweite akzeptierte Staging-Stufe wie `gamemode.amx` oder `gtasa_reallife-copy.amx`. Das verhindert:

- veraltete AMX-Instanzen
- doppelte Laufzeitartefakte
- fehlerhafte Modusinitialisierung
- Default-Spawn-Fallbacks

---

## 🖥️ 5. Plugin-Reihenfolge und Runtime-Umgebung

Die Serverkonfiguration reflektiert die benötigten SA:MP-Plugins und deren Reihenfolge. Die zentrale `server.cfg` enthält typischerweise die Einträge:

```cfg
plugins crashdetect streamer sscanf2 sqlite mysql zcmd
```

Diese Reihenfolge ist nicht trivial. Sie ist logisch aufgebaut:

- `crashdetect` – Crash-Erkennung und Stabilitätsprüfung
- `streamer` – Objekte, Vehicles, Map-Streaming
- `sscanf2` – String-Argument-Parsing für Befehle und Input
- `sqlite` – Datenbankzugriff
- `mysql` – optionales späteres Datenbank-Upgrade
- `zcmd` – Befehls-Parser

Diese Plug-ins bilden die Grundlage für Reallife-Workflows wie Jobs, Inventory, State-Management, Factions und Vehicle-Logic. Ohne sie kann der Gamemode teilweise nicht sauber funktionieren.

---

## 🏗️ 6. Datenfluss und Spielsystem-Kopplung

Das Projekt ist modular, aber eng gekoppelt. Beispiel:

- `StateSystem` definiert Staaten und Spawn-Logik
- `Economy` arbeitet mit Steuern und Kassen
- `Jobs` sind staatlich verankert
- `FactionSystem` arbeitet mit Territory, Wars, Einfluss
- `Housing` verwaltet Häuser und Safes
- `Inventory` verwaltet Items und Lager
- `VehicleSystem` verwaltet Fahrzeugbesitz, Schlüssel, Motorzustand und UBB
- `UI` bündelt die Sichtbarkeit und die Interaktionsfläche

Das bedeutet: Die Datenflüsse sind nicht verloren, sondern durch zentrale Interfaces über Module und Datenbank verbunden. Die Projektarchitektur versucht, die Zahl der parallel existierenden Zustandsquellen klein zu halten und dabei eine klare, reproduzierbare, modular nutzbare Basis zu schaffen.

---

## ✅ 7. Sicherheits- und Stabilitätsprinzipien

Die Codebasis enthält mehrere grundlegende Stabilitätsmechanismen:

- Lifecycle-Stufen für Initialisierung
- Guards gegen ungültige Zustände
- Datenbank-Transaktions- und Schema-Checks
- zentrale State- und Treasury-Regeln
- explizite Struktur bei Serverstart, DB-Initialisierung und Runtime-Ready

Das Projekt ist damit nicht nur aus funktionaler Sicht modular, sondern auch aus einer Stabilitätslogik heraus aufgebaut. Gerade bei SA:MP-Funktionen ist die Reihenfolge des Starts und die Konsistenz von State-/DB-/UI-Initialisierung elementar.

---

## ✅ 8. Verifikations- und Troubleshooting-Regeln

Ein sauberer Build gilt als erfolgreich, wenn:

- `gamemodes/gamemode.pwn` ohne fatalen Compile-Abbruch kompilieren kann
- die Output-Datei exakt `Compiler/samp03/gamemodes/gtasa_reallife.amx` existiert
- die AMX-Datei nicht leer ist
- `server.cfg` `gamemode0 gtasa_reallife 1` verwendet
- der Server beim Start die erwartete Reallife-Welt lädt

Typische Fehlerquellen:

- fehlende `qemu-i386`-Umgebung
- fehlendes `data/db/`-Verzeichnis
- Server-Root ohne `gamemodes/gtasa_reallife.amx`
- doppelte `.amx`-Dateien im gleichen Ordner
- Serverkonfiguration falsch mit dem Modusnamen synchronisiert

---

## 📘 9. Fazit

Nexus-Core-Gamemode ist als modularer, klar strukturierter, langlebiger SA:MP-Gamemode entworfen. Die Kernarchitektur basiert auf:

- klarer Entrypoint-Logik
- modularem, besser wartbarem Code
- sauberem Lifecycle- und Boot-Flow
- eigener Serverruntime im `Compiler/samp03/`-Ordner
- striktem Single-Target-Model für die Runtime-Datei
- lokaler SQLite-Persistenz mit klaren Dateipfaden

Das ist die entscheidende Grundlage dafür, dass der Server denselben Gamemode zuverlässig lädt und nicht in einen Default-Blueberry- oder Fallback-Spawn fällt.

Das sichert die korrekte Initialisierung von:

```text
data/db/gtasa_reallife.db
```

### 📘 Schritt 3: Gamemode kompilieren

Der Compiler muss exakt den korrekten Modusnamen schreiben:

```bash
cd ~/Nexus-Core-Gamemode
mkdir -p data/db Compiler/samp03/gamemodes Compiler/samp03/data/db
export LD_LIBRARY_PATH="$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
qemu-i386 Compiler/pawncc/pawncc \
  -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" \
  gamemodes/gamemode.pwn \
  -iincludes \
  -iCompiler/samp03/include \
  -d3
```

Das Ergebnis muss genau hier liegen:

```text
Compiler/samp03/gamemodes/gtasa_reallife.amx
```

### 🔧 Schritt 4: Serverkonfiguration prüfen

Die Serverkonfiguration muss mit dem Modusnamen übereinstimmen:

```cfg
gamemode0 gtasa_reallife 1
```

Beispiel für die passende Plugin-Reihenfolge:

```cfg
plugins crashdetect streamer sscanf2 sqlite mysql zcmd
```

### 🔧 Schritt 5: SA:MP-Server starten

In den Server-Root wechseln und mit korrektem `LD_LIBRARY_PATH` starten:

```bash
cd Compiler/samp03
LD_LIBRARY_PATH=.:./plugins:/usr/lib32:/usr/lib/i386-linux-gnu ./samp03svr
```

---

## 📊 5. Fehlerbehebung und Verifikation

### 🔧 5.1 Wie man erfolgreichen Start erkennt

Wenn alles korrekt initialisiert wurde, zeigt die Konsole typischerweise an:

- der Gamemode `gtasa_reallife` wird geladen
- der Server hört auf dem konfigurierten Port, meist `7780`
- der Player spawnt im Reallife-Modus statt im Standard-CJ-/Blueberry-Spawn
- keine Warnung „Game Mode: Unknown“ erscheint

Typische Erfolgszeilen:

```text
[gamemode] Loading gamemode: gtasa_reallife
[server] Listening on port 7780
[gamemode] GameModeInit finished
```

### 📘 5.2 Häufige Probleme

#### Fehlende Bibliotheks-Pfade für den Compiler

Symptome:

- `libpawnc.so: cannot open shared object file`
- Pawn-Compiler startet nicht

Lösung:

```bash
export LD_LIBRARY_PATH="$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
```

#### Fehlendes SQLite-Verzeichnis

Symptome:

- Datenbank kann nicht erstellt werden
- Laufzeitfehler beim Boot

Lösung:

```bash
mkdir -p data/db
```

#### Falscher AMX-Name oder veraltete Duplikate

Symptome:

- `Game Mode: Unknown`
- Server startet in Standard- oder Fallback-Verhalten
- Blueberry-Spawn statt der eigentlichen Reallife-Umgebung

Lösung:

- veraltete AMX-Dateien entfernen
- nur diese Datei behalten:

```text
Compiler/samp03/gamemodes/gtasa_reallife.amx
```

- `server.cfg` auf `gamemode0 gtasa_reallife 1` prüfen

#### Plugin- oder Library-Pfadprobleme

Symptome:

- Plugins werden nicht gefunden
- Server startet nicht sauber

Lösung:

```bash
cd Compiler/samp03
LD_LIBRARY_PATH=.:./plugins:/usr/lib32:/usr/lib/i386-linux-gnu ./samp03svr
```

#### Compile-Warnungen

Pawn-Projekte erzeugen oft Warnungen, ohne den Build komplett zu stoppen. Das ist nicht immer kritisch, aber sollte überprüft werden. Solche Warnungen können auf:

- Funktions-Tag-Konflikte
- Symbol-/Typ-Fehler
- unnötige Ausdrücke oder unvollständige Definitionen

hinweisen.

Der Build gilt als erfolgreich, wenn die Kompilierung ohne fatalen Abbruch durchläuft und das AMX korrekt im Zielpfad entsteht.

---

## 🏗️ 6. Schlussfazit der Architektur

Das Projekt ist bewusst in klare Schichten aufgebaut:

- `gamemode.pwn` bootet das System
- `modules/` enthalten die eigentliche Spiel- und Wirtschaftslogik
- `includes/` stellen gemeinsame Funktionen bereit
- `config/` enthält Server- und Datenbankkonfiguration
- `data/db/` hält den persistenten Zustand
- `Compiler/` stellt die lokale Pawn-Toolchain bereit
- `Compiler/samp03/` ist die aktive Serverumgebung

Die wichtigste Betriebsregel lautet einfach und muss immer eingehalten werden:

- kompilieren mit der lokalen Pawn-Toolchain
- Ausgabe nur als `gtasa_reallife.amx`
- exakt im Pfad `Compiler/samp03/gamemodes/`
- `server.cfg` identisch auf `gtasa_reallife` synchronisieren
- Server mit dem korrekten `LD_LIBRARY_PATH` starten

Damit wird der gewünschte Gamemode zuverlässig geladen und ein veralteter Default-Blueberry-Spawn verhindert.

---

## 🏗️ 7. Finaler Systemstand und Release-Guardrails

Der aktuelle Architektur-Stand ist bewusst auf Stabilität, Wiederholbarkeit und Single-Source-of-Truth ausgelegt. Im Kern bleibt `core.inc` die zentrale Quelle für Spielerstatus, Home-State, Projektleiter-/Admin-Rechte und State-Validierung. Jede andere Stelle, die denselben Datensatz verwaltet, muss als Fallback oder ausgehender Leser fungieren und darf keine eigene, unabhängige Schattensicht etablieren.

### 🔧 7.1 Build- und Runtime-Vertrag im Detail

1. Single Target
   - Die einzige gültige AMX-Ausgabe bleibt `Compiler/samp03/gamemodes/gtasa_reallife.amx`.
   - Aliase wie `gamemode.amx` und andere Kopien sind in diesem Repository-Kontext nicht Teil des gültigen Builds.
   - Der `server.cfg`-Eintrag bleibt auf `gamemode0 gtasa_reallife 1` synchronisiert.

2. Toolchain
   - Der Build läuft immer lokal mit der 32-Bit-Pawn-Toolchain über `qemu-i386`.
   - Das erforderliche Kommando bleibt unverändert:
     `LD_LIBRARY_PATH="$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" qemu-i386 Compiler/pawncc/pawncc -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" gamemodes/gamemode.pwn -iincludes -iCompiler/samp03/include -d3`

3. Lifecycle und Include-Reihenfolge
   - Include-Reihenfolge im laufenden Code: `Core -> DB -> State -> Admin -> Auth -> Economy -> Jobs -> Laws -> Housing/Vehicle -> Leisure/Web`
   - Lifecycle-Folge: `BOOT -> DB_READY -> STATE_READY -> AUTH_READY -> WORLD_READY -> RUNTIME_READY`
   - Diese Reihenfolge ist nicht als weicher Hinweis, sondern als Architekturvertrag zu verstehen.

4. Hardening Guards
   - `INSERT OR IGNORE` bleibt mit der DB-Sicht kompatibel und verhindert das Überschreiben gültiger Statewerte.
   - Gültige `state_id` und `home_state` dürfen niemals durch `0` oder Standardwerte zurückgesetzt werden.
   - `strmid`-basierte Token-Extraktion bleibt der bevorzugte Parsing-Pfad, insbesondere für Passwörter, Befehls-Input und Dialog-Parameter.
   - `INVALID_TEXT_DRAW` und SetCameraBehindPlayer() bleiben die Muster für sauberes Reset- und Cleanup-Verhalten.

### 📘 7.2 Persistenz- und State-Sicherungsstrategie

Die Persistenzarchitektur basiert auf einem doppelt abgestützten Modell:

- primär: Dini-first-Textdateien unter `Compiler/samp03/scriptfiles/`
- sekundär: SQLite-Upsert nach `data/db/gtasa_reallife.db` für konsistenze Sicht und Web-/Dashboard-Read-Only-Lesezugriff

Diese Kombination ist für kritische Pfade notwendig, weil Dini-Transaktionen zwar atomar im Dateisystem über Temp/Validate/Rename umgesetzt werden können, SQLite aber für die Operational View und spätere Analyse dienen soll. Die Engine darf nie in den Zustand eines halben Transfers geraten, wenn ein Write abgebrochen wird oder ein Crash passiert.

Wesentliche kritische Transfers bleiben daher auf einen konsistenten Ablauf festgelegt:

- Write-Temp
- Validate
- Rename
- nachfolgende SQLite-Upsert-Anbindung

Das Muster ist explizit für Haus-Safes, Inventar, Fahrzeuge, Player-States und Ledger-Transfers vorgesehen.

### 🧩 7.3 UI-, Tutorial- und Spawn-Sicherheit

Der komplette Start-/Onboarding-/Spawn-Lebenszyklus muss in diesem Projekt als eine zusammenhängende Zustandsmaschine verstanden werden:

`Connect -> Auto-Detect/Login/Register -> Tutorial -> State/Home-State-Auswahl -> StateSpawnSystem_Apply() -> Camera-Reset -> fester Spawnpunkt`

Dabei gilt:

- Keine Chat-Command-Verpflichtung für den Einstieg.
- Keine erneute Triggerung von Tutorial-/State-Szene bei validem `state_id` / gültigem `home_state`.
- `OnPlayerSpawn()` muss nach erfolgreichem Onboarding die bereits gültigen Werte nicht erneut überschreiben.
- Beim Disconnect müssen Flags, TextDraws, Kamera- und Dialogzustände sauber zurückgesetzt und der Zustand als vollständig „frei“ gelten.

Wichtig ist, dass die Sicherung nicht nur im Einzelfall, sondern im Gesamtpfad wirkt: Der Login/Spawn/Reset-/Disconnect-Zyklus darf keine Default-Werte wieder in die aktive Session injizieren.

### 📘 7.4 Welt-, Admin- und Wirtschafts-Integration

Die modularen Subsysteme sind im Relation-Design aufeinander abgestimmt:

- `core.inc` verwaltet die Grundzustände und Rechte
- `admin_system.inc` kontrolliert Admin-/ProjectLeader-Rechte und Karten-/Welt-Editoren
- `state_spawn_system.inc` übernimmt Home-State-/Spawn-Logik
- `government/` und `state/` bilden politische und verwaltungsbezogene Zustände
- `vehicles/`, `housing/`, `business/`, `factions/`, `economy/`, `jobs/`, `laws/` bilden die wirtschaftlichen und siedlungsbezogenen Abläufe
- `ui_hub.inc` und `roleplay_ui_system.inc` sorgen als sichtbare Oberfläche für den Zugang zu diesen Pfaden

Das zentrale Prinzip ist die Lückenlosigkeit: Jeder Modulaufruf muss seine Existenz und Berechtigung durch die tatsächliche API verifizieren, keine indirekten, losen oder mehrfach shadowed Zustände verwenden und keine halben Transaktionen zulassen.

### 📘 7.5 Laufzeit-Guardrails unter qemu-i386

Die Laufzeit ist bewusst auf ein begrenztes Ressourcenszenario ausgelegt:

- keine großen Schleifen über `MAX_PLAYERS` ohne Early-Return
- keine aktiven Ticks bei Inaktivität
- keine Belastung durch wiederholte UI-Updates oder Camera-/Widget-Schleifen ohne echte Aktivität
- sichere Parsing-Pfade statt unsicherer `sscanf`-Puffer
- reduzierte Hotpaths in `TextWars_Tick` und `UBBVehicleSystem_Connect`

Die Kriterien gelten als Architektur-Anforderung, nicht als Option. Sie sind Teil der Stabilität der kompletten Laufzeitumgebung unter QEMU/32-Bit-Pawn.

### 📊 7.6 Offline-Tooling und Code-Audits

Um die Laufzeitarchitektur mit Architektur- und Persistenz-Sicherheit zu verifizieren, ohne einen Live-Server zu starten, wurden zusätzliche Python-Tools als schlanke Offline-Checks eingeführt:

- `tools/db_and_state_inspector.py`: prüft die SQLite-Konsistenz und die Dini-/scriptfiles-Struktur, ohne Daten zu ändern.
- `tools/audit_modules_and_guards.py`: kontrolliert Include-/Lifecycle-Reihenfolge, Guards, Camera-Reset, TextDraw-/State-Checks und Punkte der qemu-i386-Ressourcensicherung.
- `tools/textdraw_and_zone_mapper.py`: listet TextDraws, Widget-Zonen und eventuelle Gebiet-/Radius-Definitionen übersichtlich auf.
- `tools/projectlead_api_check.py`: identifiziert API-Mismatches bei Projektleiter-/RCON-/Spawn-/Fahrzeug-/Gangfight-Pfaden frühzeitig.
- `tools/gangfight_zone_validator.py`: prüft Rechteck-/Radius-Zonentypen auf Plausibilität, Min/Max-Umkehrungen, große Bereiche und fehlende Zuordnungen.
- `tools/economy_and_state_ledger_check.py`: kontrolliert den schreibgeschützten Ledger-/Cash-/State-/House-/Safe-/Vehicle-Status in DB und scriptfiles.
- `tools/dialog_and_camera_reset_check.py`: prüft Dialogpfade, Trigger und Camera-/Cleanup-Reset-Guards.
- `tools/qemu_7780_readiness_check.py`: validiert statisch die 7780-Readiness ohne Start des Servers, inklusive Single-Target AMX und Port-Checks.

Diese Skripte ergänzen die bestehende `tools/`-Sammlung und sollen den Verlauf zwischen Build, Persistenz und systemweiterem Quellcheck dokumentieren, ohne den Laufzeitbetrieb zu provozieren.

### 📊 7.7 Release-Status

Die Codebasis ist als source-validiert und architektonisch konsistent einzuordnen, aber die final reale Freigabe bleibt weiterhin ein Live-Release-Gate auf der Ubuntu-VM. Eine reine Compile-Sicherheit genügt nicht, um den Server als produktiv freigegeben zu betrachten. Entscheidend sind tatsächlicher Serverstart, echte Log-Prüfung und Client-/State-/DB-Verifikation im realen Laufzeitkontext.

---

## 🏗️ 8. Schlussfazit der Architektur

Das Projekt bleibt modular, konsistent und auf den Single-Target-Build ausgerichtet. Die Kernprinzipien lauten:

- nur `gtasa_reallife.amx` als gültige Runtime-Ausgabe
- `core.inc` als Single Source of Truth
- strikte Include-/Lifecycle-Reihenfolge
- Dini-first-Persistenz mit SQLite-Read-Only-Sicht
- sichere Login-/Tutorial-/State-/Spawn-Maschine
- saubere Hud-/Camera-/Reset- und Disconnect-Pfade
- harte Guards gegen Default-State-Verfälschung, halbe Transfers und qemu-i386-Overhead

Damit ist die Projektarchitektur für den nächsten echten Live-Check vorbereitet: Der Build ist klar, die Logik konsistent, die Hardening-Muster sind wiederverwendbar und die letzte Freigabe gilt weiterhin nur nach erfolgreicher Laufzeitvalidierung auf der Ubuntu-VM.

---

## 🔧 Additive Architektur-Notiz: Source-Blocker und Build-Konsistenz

Diese Ergänzung dokumentiert die echten Quellbaum-Funde ohne bestehende Abschnitte zu verändern.

- Ein reproduzierbarer Compile-Blocker war der fehlende `GetServerTime`-Symbol im Pawn-Quellbaum. Der Code wurde auf das im Projekt verwendete `gettime()`-API zurückgeführt und damit auch in `core.inc` sowie `premium_system.inc` konsistent gemacht.
- Ein zweiter realer Blocker war die fehlende `DB_GetPlayerHomeState()`-Funktion. Sie wurde in `gamemodes/modules/db/db_interface.inc` ergänzt. Dadurch bleiben `state_system.inc` und die Connect-/Rehydrate-Pfade bezüglich `home_state` wieder korrekt mit `core.inc` verbunden.
- Die Source-Validierung ist damit wieder in der echten Build-Contract-Achse: `gamemodes/gamemode.pwn` kompiliert mit `qemu-i386` in `Compiler/samp03/gamemodes/gtasa_reallife.amx` und erfüllt weiterhin die Architektur-Regel „ein Ziel, kein 7777-Drift“.
- Die Persistenz-/State-Arbeit bleibt Dini-first, mit SQLite-Read-Only als ergänzender Sicht auf die Datenbank; die hier behobenen Fehler lagen im Source-/Contract-Level, nicht in einer Live-7780-Umgebung.


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
