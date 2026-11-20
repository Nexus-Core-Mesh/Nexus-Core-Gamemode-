# Architektur — Nexus-Core-Gamemode

## 1. Überblick und Ziel der Systemarchitektur

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

---

## 2. Verzeichnisstruktur und Dateizuordnung

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

### 2.1 Master-Entry-Point: `gamemodes/gamemode.pwn`

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

### 2.2 Modulstruktur der eigentlichen Funktionalität

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

### 2.3 Includes und Hilfsbibliotheken

Die Includes sind nicht nur Nebensache, sondern Teil der Spiel- und Compiler-Architektur. Das Projekt nutzt SA:MP-Standardincludes sowie projektspezifische Hilfsbibliotheken. Diese Datei- und Codeebene macht den Quellcode lesbar, stark typisiert und teils modular aufgeteilt.

Wichtige. Aspekte:

- Standard-SAMP-Funktionen
- sscanf-Parsing für Befehle und Argumente
- Datenbankabstraktionen
- Hilfskonstanten und Shared-Utilities
- Modul- und UI-Schnittstellen

Im Build-Workflow werden these Includes über `-iincludes` und `-iCompiler/samp03/include` eingebunden, damit die Pawn-Compiler-Ausführung den kompletten Build-Kontext kennt.

### 2.4 Compiler- und Toolchain-Schicht

`Compiler/` ist die lokale Build-Umgebung. Sie ist bewusst physisch im Projekt enthalten, damit das Projekt reproduzierbar gebaut werden kann. Die beiden kritischen Bestandteile sind:

- `Compiler/pawncc/pawncc` – der lokale 32-Bit-Pawn-Compiler
- `Compiler/samp03/` – der Laufzeitordner des tatsächlichen Servers

Auf 64-Bit-Hosts muss dieser Compiler über `qemu-i386` ausgeführt werden, da es sich um ein 32-Bit-Binary handelt. Genau deshalb gibt es die Logik:

```bash
qemu-i386 Compiler/pawncc/pawncc ...
```

Damit bleibt die Toolchain konkret und unabhängig von externen Installationen.

### 2.5 Laufzeit- und Kantenfall-Guardrails

Im aktuellen Stand wurden die früheren Runtime-Risiken nicht bloß dokumentiert, sondern in den geeigneten Funktionspfaden direkt abgesichert:

- `DB_RehydratePlayerState()` prüft `state_id` und `home_state` mit `Core_IsValidState()` und überschreibt keine gültigen Werte mit `0` oder Fallbacks.
- `OnPlayerSpawn()` erkennt einen bereits validen `home_state`/`state_id` und startet weder Tutorial noch State-Szene erneut.
- `CameraSystem_Reset()` und `CameraSystem_ExitCameraMode()` sorgen für einen perfiden Reset-Pfad mit `SetCameraBehindPlayer()` und `TogglePlayerControllable(playerid, 1)`.
- `UiHub_`-Pfad und `RoleplayUI_`-Cleanup verwenden nur sichere `INVALID_TEXT_DRAW`-Prüfungen, um veraltete HUD-/Textdraw-Handles beim Wechsel von Menü, Spawn und Disconnect aufzuräumen.

Diese Vereinbarungen sind absichtlich kein „Wiederverschieben“ apt-ähnlicher Änderungen, sondern eine direkte Sicherheitsschicht für den Connect-/Spawn-/HUD-Flow.

### 2.5 Admin-/Projektleiter-Workflow und runtime contract

Im laufenden Spielbetrieb wurde der Administrations- und Projektleiter-Export bewusst als separater, nicht-öffentlich sichtbarer Zugriff ergänzt. Die Kernidee ist: normale Spieler-Hilfen und Standardmenüs bleiben unverändert, während autorisierte Admins und Projektleiter im System über eine identische Core-Prüfung und einen zentralen State-/Spawn-Flow Zugang erhalten.

Die relevanten Sicherheits- und Runtime-Regeln lauten:

- `Core_IsProjectLeader()` setzt den Projektleiter-Status ausschließlich über das zentrale `gPlayerData`-Profil und erlaubt keine zweite parallele Statusquelle.
- `AdminSystem_IsProjectLeader()` prüft zusätzlich den globalen Owner-/Admin-Pfad und hält den Zugriff sauber separiert von normalen Spieler-Menüs.
- `/projectlead`, `/setstate` und `/spawnvehicle` laufen über `AdminSystem_HandleCommand()` und nutzen die vorhandenen State-/Spawn-/Vehicle-Guards statt unkontrollierten direktem Schreiben in globale Arrays.
- `StateSpawnSystem_Apply()` bleibt der single entry point für State- und Spawn-Applikation; der Kamerarücksetzpfad läuft über `CameraSystem_Reset()` beziehungsweise `SetCameraBehindPlayer()` und verhindert so eine Rekursion in Tutorial-/State-Schleifen.
- Fahrzeugspawns gehen über `VehicleSystem_SpawnAdminVehicle()` und respektieren die Slot-/Limit-Checks des Fahrzeugsystems statt rohe Slot-Array-Schreibzugriffe im Admin-Context zu erlauben.
- Die Laufzeit-Hotpath-Guardrails wurden ergänzt: `TextWars_Tick()` prüft vor jedem Refresh aktivierte Gangfights und sichtbare Player-Widgets und beendet den Tick früh, sobald keine echte Aktivität mehr vorliegt. Damit bleiben die UI- und Faction-Hotpaths robust unter der qemu-i386-Umgebung mit begrenzten Ressourcen.

Diese Regeln sind bewusst zusammen mit dem vordefinierten Core-Contract in der Architektur verankert, damit spätere Module denselben Single-Source-of-Truth- und Hardening-Pfad verwenden.

### 2.5 Live-Server-Root und runtime contract

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

### 2.6 Dini-Persistenz und Fallback-Model

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

### 2.7 Produktions-Checkliste für echte Live-Umgebungen

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

### 2.8 Atomicity- und Recovery-Regeln für Dini-Schreibpfade

Dini arbeitet ohne SQL-Transaktions-Logik. Für kritische Pfade gilt deshalb eine explizite Datei-Atomicity-Policy:

- `StateLedgerSystem_Transfer()` darf nur dann Geld aus einer Kasse abbuchen und einer anderen gutschreiben, wenn beide Schreibschritte als Teil einer durchgehend validierten Datei-Transaktion erfolgen.
- `house_storage_system.inc` und Fahrzeug-/Inventar-Transfers müssen temp-Dateien verwenden und nur nach erfolgreichem Schreiben/Validieren atomar umbenennen.
- Wenn ein Lesevorgang auf eine teilweise oder beschädigte INI-Datei trifft, muss der Loader einen Repair- oder Fallback-Pfad aktivieren und die betroffenen Werte als ungültig markieren, statt sie als gültige Daten zu akzeptieren.
- Der SQLite-Autosync darf nur als ergänzende, asynchrone Konsistenzschicht dienen. Er kann nicht als Ersatz für die Dini-Atomicity- und Recovery-Regeln fungieren.
- Dadurch bleibt der reale Datenfluss: Dini-first-Save → validieren → atomar umbenennen → optional SQLite-Upsert/Replication → read-only Panel-Query. Ein fehlender oder veralteter SQLite-Update-Pfad zählt als operativer Mismatch, nicht als normaler Zustand.

Damit bleibt die Datenbasis zwischen Dini-first-Storage und SQLite-Read-Only-Panel in einem konsistenten, prüfbaren Zyklus: Save → validate → atomic rename → optional SQLite upsert → panel read.


---

## 3. Modul- und Lifecycle-Architektur

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

## 4. Build-, CI/CD- und Deployment-Pipeline

### 4.1 Lokaler Build

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

### 4.2 GitHub Actions Workflow

Die CI-Logik in `.github/workflows/build.yml` folgt dem gleichen Single-Target-Prinzip:

- Ordner vorbereiten
- Pawn-Compiler mit qemu-i386 ausführen
- Ausgabe als `Compiler/samp03/gamemodes/gtasa_reallife.amx`
- Artefakt hochladen

Der CI-Workflow ist damit synchron zum lokalen Build und verhindert ein Nicht-Match zwischen Build- und Runtime-Konfiguration.

### 4.3 Serverdeployment

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

### 4.4 Saubere Single-Target-Staging-Logik

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

## 5. Plugin-Reihenfolge und Runtime-Umgebung

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

## 6. Datenfluss und Spielsystem-Kopplung

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

## 7. Sicherheits- und Stabilitätsprinzipien

Die Codebasis enthält mehrere grundlegende Stabilitätsmechanismen:

- Lifecycle-Stufen für Initialisierung
- Guards gegen ungültige Zustände
- Datenbank-Transaktions- und Schema-Checks
- zentrale State- und Treasury-Regeln
- explizite Struktur bei Serverstart, DB-Initialisierung und Runtime-Ready

Das Projekt ist damit nicht nur aus funktionaler Sicht modular, sondern auch aus einer Stabilitätslogik heraus aufgebaut. Gerade bei SA:MP-Funktionen ist die Reihenfolge des Starts und die Konsistenz von State-/DB-/UI-Initialisierung elementar.

---

## 8. Verifikations- und Troubleshooting-Regeln

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

## 9. Fazit

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

### Schritt 3: Gamemode kompilieren

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

### Schritt 4: Serverkonfiguration prüfen

Die Serverkonfiguration muss mit dem Modusnamen übereinstimmen:

```cfg
gamemode0 gtasa_reallife 1
```

Beispiel für die passende Plugin-Reihenfolge:

```cfg
plugins crashdetect streamer sscanf2 sqlite mysql zcmd
```

### Schritt 5: SA:MP-Server starten

In den Server-Root wechseln und mit korrektem `LD_LIBRARY_PATH` starten:

```bash
cd Compiler/samp03
LD_LIBRARY_PATH=.:./plugins:/usr/lib32:/usr/lib/i386-linux-gnu ./samp03svr
```

---

## 5. Fehlerbehebung und Verifikation

### 5.1 Wie man erfolgreichen Start erkennt

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

### 5.2 Häufige Probleme

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

## 6. Schlussfazit der Architektur

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
