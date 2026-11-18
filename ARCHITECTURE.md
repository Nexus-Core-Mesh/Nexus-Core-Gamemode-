# Architektur — Nexus-Core-Gamemode

## 1. Überblick und Ziel der Systemarchitektur

Nexus-Core-Gamemode ist ein modularer GTA:SA-Reallife-Gamemode für SA:MP, der bewusst nicht als eine einzelne monolithische `gamemode.pwn`-Datei aufgebaut ist. Stattdessen basiert das Projekt auf einer klaren mehrschichtigen Architektur mit eigenständigen Bereichen für Core, State, Auth, Economy, Jobs, Factions, Housing, Inventory, UI, Vehicles, Law, World, Kommunikation und Web-Management.

Die Kernidee ist eine saubere Trennung von Verantwortlichkeiten, damit das System erweiterbar, testbar und für spätere Produktionsläufe stabil bleibt. Die grundlegenden Architekturbereiche sind:

- `gamemodes/gamemode.pwn` – Master-Entry-Point und Boot-Skript des gesamten Gamemodes
- `gamemodes/modules/` – komplette modulare Funktionalität von Core bis World
- `includes/` – SA:MP- und Projekt-Includes
- `config/` – Server- und Konfigurationsdateien
- `data/db/` – persistenter SQLite-Speicherpfad
- `Compiler/` – lokale Pawn-Toolchain und Serverruntime
- `Compiler/samp03/` – laufende SA:MP-Serverumgebung
- `scripts/` – Setup, Build, Deployment, Debugging

Das wichtigste Designprinzip lautet: Der Quellcode wird im Root entwickelt, gebaut und kompiliert, während der Serverordner nur die Laufzeitbinary und runtime-required Files enthält. Dadurch bleibt die Deployment-Kette reproduzierbar und frei von veralteten Alias-Dateien.

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

### 2.6 SQLite-Datenbank und Persistenzmodel

Das Projekt geht bewusst `SQLite-first` und nutzt den Pfad:

```text
data/db/gtasa_reallife.db
```

Dieser Pfad bildet die Basis für:

- Login- und Charakterdaten
- Inventar- und Safe-Strukturen
- Häuser und Fahrzeugsysteme
- Staatskassen und Wirtschaftslogs
- Zustandsdaten aus Economy, Jobs, Factions, Inventory und Fahrzeuge

Das DB-Setup ist Teil der Boot- und Runtime-Initialisierung, und die erforderlichen Verzeichnisse werden in den Scripts vorausgesetzt und angelegt.

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
