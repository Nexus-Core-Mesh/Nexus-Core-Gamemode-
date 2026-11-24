# HowToSetup – Der ultimative Einstieg für Einsteiger

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
| Dokument | `HowToSetup.md` |
| Typ | Setup-Guide |
| Fokus | Einrichtung, Build und Laufzeitkonfiguration |
| Wichtige Pfade | `gamemodes/gamemode.pwn`, `Compiler/samp03/gamemodes/gtasa_reallife.amx`, `data/db/` |

---
## 📘 1. Was hier genau passiert

Dieses Projekt ist kein normales „einfach nur Server-Ordner anlegen“-Setup. Es ist ein sauberes Source-to-Server-Deployment mit klarer Trennung zwischen:

- Quellcode im Repository-Root
- Build-Toolchain in `Compiler/`
- Laufzeit-Server in `Compiler/samp03/`
- persistente Daten in `data/db/`

Der wichtigste Grundsatz ist dieser:

- Der echte Quellcode liegt immer im Repository-Root unter `gamemodes/gamemode.pwn`.
- Der Compiler liest genau diese Datei.
- Der Compiler schreibt die fertige Binärdatei direkt in den Laufzeit-Ordner als `Compiler/samp03/gamemodes/gtasa_reallife.amx`.
- Diese Datei ist die einzige gültige Laufzeitdatei.
- Es gibt keine zweite `.amx`-Datei im Serverpfad.
- `server.cfg` muss exakt `gamemode0 gtasa_reallife 1` enthalten.

Wenn dieser Ablauf nicht eingehalten wird, kann der Server einen falschen Modus laden, den Default-Spawn verwenden oder im schlimmsten Fall den Modus als `Game Mode: Unknown` behandeln.

---

## 📘 2. So sieht das Projekt nach dem Entpacken aus

Wenn du die ZIP-Datei entpackst, solltest du ungefähr diese Struktur sehen:

```text
~/Nexus-Core-Gamemode/
├── ARCHITECTURE.md
├── HOW_TO_PLAY.md
├── HowToSetup.md
├── README.md
├── SEASON.md
├── TO-DO.md
├── config/
│   ├── server.cfg
│   └── ...
├── control-panel/
│   ├── README.md
│   └── ...
├── data/
│   └── db/
├── gamemodes/
│   ├── gamemode.pwn
│   └── modules/
├── includes/
├── scripts/
│   ├── setup_environment.sh
│   ├── build_gamemode.sh
│   ├── deploy_server.sh
│   └── debug_server.sh
├── Compiler/
│   ├── pawncc/
│   └── samp03/
│       ├── gamemodes/
│       ├── plugins/
│       ├── filterscripts/
│       ├── include/
│       ├── server.cfg
│       └── samp03svr
├── server.cfg
├── sql.cfg
└── tools/
```

Wichtige Einzelheiten:

- `gamemodes/gamemode.pwn` ist der zentrale Einstiegspunkt
- `gamemodes/modules/` enthält das komplette modulare Spielsystem
- `Compiler/pawncc/pawncc` ist der lokale 32-Bit-Pawn-Compiler
- `Compiler/samp03/` ist der echte Server-Root für SA:MP
- `Compiler/samp03/scriptfiles/` ist der Dini-/INI-Speicherort für das Start- und Persistenz-Fallback
- `data/db/` bleibt nur optionaler SQLite-/Autosync-Pfad, darf die Serverinitialisierung aber nicht erzwingen

---

## 📘 3. Warum der Quellcode im Root liegt

Die wichtigste Mentalität im Projekt lautet:

- Dateien werden nicht im Serverordner bearbeitet.
- Der Serverordner ist ausschließlich Laufzeit- und Startumgebung.
- Der echte Code lebt im Repository-Root.

Das ist der Grund, warum `gamemodes/gamemode.pwn` der Einstiegspunkt ist. Der Compiler liest genau diese Datei und kompiliert sie. Die Datei ist nicht „nur ein File im Server“, sondern das gesamte Source-Design des Projekts.

Der Buildweg ist hiermit klar:

```text
Repository-Root
    └── gamemodes/gamemode.pwn
            ↓
       pawncc via qemu-i386
            ↓
Compiler/samp03/gamemodes/gtasa_reallife.amx
```

Nicht so:

```text
Compiler/samp03/gamemodes/...
    ↓
manuell mehrere .amx-Dateien anlegen
    ↓
Server lädt das falsche oder veraltete File
```

Das ist ein Fehler, weil der SA:MP-Server den Modusnamen in `server.cfg` mit dem tatsächlich vorhandenen `.amx`-Dateinamen synchronisieren muss.

---

## 📘 4. Was bedeutet „Single-Target“-Modell?

Das Projekt arbeitet mit einem einzigen gültigen Runtime-Ziel:

```text
Compiler/samp03/gamemodes/gtasa_reallife.amx
```

Das ist der einzige erlaubte Ausgabe-Name.

Streng verboten sind:

```text
Compiler/samp03/gamemodes/gamemode.amx
Compiler/samp03/gamemodes/gtasa_reallife.amx.bak
Compiler/samp03/gamemodes/unknown.amx
```

Dieser Grundsatz ist nicht nur sauber, sondern essenziell. Wenn mehrere `.amx`-Dateien im gleichen Ordner liegen, kann der Server eine veraltete Datei wählen, den falschen Modus laden oder in das Standardverhalten fallen.

Das führt typischerweise zu:

- `Game Mode: Unknown`
- falscher oder veralteter Modusstart
- Blueberry-/CJ-Defaultspawn statt realer Reallife-Welt

Das Projekt muss daher exakt eine aktive AMX-Datei im Runtime-Ordner kennen:

```text
Compiler/samp03/gamemodes/gtasa_reallife.amx
```

---

## 🔧 5. Vorbereitungen: Ordner anlegen

Als Erstes solltest du den Server- und Datenbank-Pfad sauber vorbereiten.

### 📘 5.1 In das Projekt wechseln

```bash
cd ~/Nexus-Core-Gamemode
pwd
ls -la
```

Du solltest den Root des Projekts sehen, inklusive Ordnern wie `gamemodes`, `includes`, `Compiler`, `scripts`, `config` und `data`.

### 📘 5.2 Erforderliche Ordner erzeugen

```bash
mkdir -p data/db \
         Compiler/samp03/gamemodes \
         Compiler/samp03/data/db \
         Compiler/samp03/plugins \
         Compiler/samp03/filterscripts \
         Compiler/samp03/scriptfiles
```

Was diese Befehle tun:

- `data/db/` erstellt den SQLite-Pfad für die lokale Datenbank
- `Compiler/samp03/gamemodes/` ist der Laufzeitordner für das fertige `.amx`
- `Compiler/samp03/data/db/` ist der Laufzeit-Datenbankpfad innerhalb des Server-Roots
- `plugins/` und `filterscripts/` sind für heruntergeladene Server-Erweiterungen und Scripte gedacht
- `scriptfiles/` reserviert die Platzierung für persisting server-side script data

---

## 🔧 6. Der Build-Schritt: Warum `qemu-i386` nötig ist

Auf einem 64-Bit-Host kann der Pawn-Compiler aus `Compiler/pawncc/pawncc` nicht direkt als normales 32-Bit-Binary ausgeführt werden. Deshalb wird er über `qemu-i386` gestartet.

Das ist der Kernbefehl:

```bash
cd ~/Nexus-Core-Gamemode
qemu-i386 Compiler/pawncc/pawncc \
  -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" \
  gamemodes/gamemode.pwn \
  -iincludes \
  -iCompiler/samp03/include \
  -d3
```

Erläuterung:

- `qemu-i386` startet den 32-Bit-Compiler auf einem 64-Bit-System
- `Compiler/pawncc/pawncc` ist der eigentliche Pawn-Compiler
- `-o` definiert das genaue Output-Ziel
- `gamemodes/gamemode.pwn` ist der zentrale Source-Einstieg
- `-iincludes` fügt die Projekt-Includes hinzu
- `-iCompiler/samp03/include` fügt den SA:MP-Laufzeit-Include-Pfad hinzu
- `-d3` aktiviert den normalen Debug-/Compile-Output für die Bauphase

Das Wichtigste:

```bash
-o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx"
```

Das heißt exakt:

```text
Compiler/samp03/gamemodes/gtasa_reallife.amx
```

Nicht `gamemode.amx`, nicht `reallife.amx`, nicht irgendeine Kopie. Nur `gtasa_reallife.amx`.

---

## 🔧 7. Der genaue komplette Build-Befehl, copy-paste-fertig

Wenn du das Projekt sauber neu bauen willst, benutze genau diesen Block:

```bash
cd ~/Nexus-Core-Gamemode
mkdir -p data/db \
         Compiler/samp03/gamemodes \
         Compiler/samp03/data/db \
         Compiler/samp03/plugins \
         Compiler/samp03/filterscripts \
         Compiler/samp03/scriptfiles

export LD_LIBRARY_PATH="$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

qemu-i386 Compiler/pawncc/pawncc \
  -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" \
  gamemodes/gamemode.pwn \
  -iincludes \
  -iCompiler/samp03/include \
  -d3
```

Was dieser Block macht:

1. Wechselt in das Projektroot
2. legt die nötigen Ordner an
3. setzt den Compiler-Pfad korrekt
4. übersetzt das Spielermodul aus `gamemodes/gamemode.pwn`
5. schreibt die fertige `.amx` direkt in den Laufzeitordner

---

## 🔧 8. Nach dem Build automatisch prüfen

Direkt nach dem Kompilieren prüfst du, ob wirklich genau die richtige Datei entstanden ist:

```bash
ls -l Compiler/samp03/gamemodes
```

Erwartete Ausgabe:

```text
Compiler/samp03/gamemodes/gtasa_reallife.amx
```

Wenn du zusätzlich alle `.amx`-Dateien in diesem Ordner auflisten willst:

```bash
find Compiler/samp03/gamemodes -maxdepth 1 -type f -name '*.amx' -print
```

Erwartung:

```text
Compiler/samp03/gamemodes/gtasa_reallife.amx
```

Wenn dort etwas anderes oder zusätzliches auftaucht, ist die Build-Logik nicht sauber.

---

## 🔧 9. Wichtige Regeln: nie im Serverordner editieren

Das ist ein absoluter No-Go:

- Source-Dateien nicht im `Compiler/samp03/`-Ordner editieren
- keine extra `.amx`-Kopien im Serverpfad erzeugen
- keine `gamemode.amx`-Alias-Datei im Laufzeitordner behalten
- keine Aktualisierung des Server-Quellcodes durch Kopieren in den Spielserver-Ordner

Wenn du etwas im Modus ändern willst, geht der Weg immer so:

1. `gamemodes/gamemode.pwn` oder passende Module unter `gamemodes/modules/` bearbeiten
2. Build erneut starten
3. fertig kompilierte `gtasa_reallife.amx` im Serverpfad prüfen
4. Server neu starten

Das ist der offizielle und sichere Workflow.

---

## 🔧 10. Konfiguration: `server.cfg` und der Modusname

Die Serverkonfiguration muss exakt denselben Namen verwenden wie die Kompilierausgabe.

Erwartete Zeile:

```cfg
gamemode0 gtasa_reallife 1
```

Das muss in der korrekten Datei stehen, idealerweise in:

- `server.cfg` (Root-Projekt)
- `Compiler/samp03/server.cfg` (laufende Server-Instanz)

Der genaue Zusammenhang ist:

```text
server.cfg -> gamemode0 gtasa_reallife 1
      ↓
AMX Datei -> Compiler/samp03/gamemodes/gtasa_reallife.amx
```

Wenn diese beiden Dinge nicht korrekt zusammenpassen, kann es zu folgender Symptomatik kommen:

- Server startet mit falschem Modus
- Spiel startet in Standardwelt oder Blueberry-Spawn
- Modus zeigt sich als unbekannt oder nicht geladen

Deshalb muss die Datei immer genau den Namen `gtasa_reallife.amx` tragen.

---

## 📘 11. SQLite-Datenbank vorbereiten

Das Projekt nutzt eine lokale Dini-/INI-Base als primäre Persistenz, die im Laufzeitroot unter `Compiler/samp03/scriptfiles/` liegen soll. Optional kann weiterhin eine lokale SQLite-Datenbank unter `data/db/` existieren, aber sie darf den Bootstrap nicht erzwingen.

### 📘 11.1 Verzeichnis anlegen

```bash
mkdir -p data/db
ls -ld data/db
```

Die Datenbank selbst kann dann als Datei existieren, zum Beispiel:

```text
data/db/gtasa_reallife.db
```

Diese Datei bleibt für Laufzeit- und Persistenzdaten relevant.

### 🔧 11.2 Wichtig für den Runtime-Server

Zusätzlich sollte der Server-Root einen entsprechenden Laufzeitpfad besitzen:

```bash
mkdir -p Compiler/samp03/data/db
```

Damit ist die Umgebung für den Server-Start sauber vorbereitet und die SQLite-Initialisierung kann ohne Pfadfehler erfolgen.

---

## 🔧 12. Server starten: vollständiger Ablauf

Wenn alles gebaut und vorbereitet ist, startest du den SA:MP-Server so:

```bash
cd ~/Nexus-Core-Gamemode/Compiler/samp03
LD_LIBRARY_PATH=.:./plugins:/usr/lib32:/usr/lib/i386-linux-gnu ./samp03svr
```

Erläuterung:

- `cd` wechselt in den Live-Server-Root
- `LD_LIBRARY_PATH` setzt die benötigten Bibliotheken und Plugin-Pfade
- `./samp03svr` startet den Serverprozess

Das ist die empfohlene Startform, damit der Server die benötigten 32-Bit- und Plugin-Umgebungen korrekt findet.

---

## 📘 13. Vollständiger End-to-End-Workflow für Einsteiger

Wenn du alles in einem Schritt ausführen willst, nutze diesen Block:

```bash
cd ~/Nexus-Core-Gamemode
mkdir -p data/db \
         Compiler/samp03/gamemodes \
         Compiler/samp03/data/db \
         Compiler/samp03/plugins \
         Compiler/samp03/filterscripts \
         Compiler/samp03/scriptfiles

export LD_LIBRARY_PATH="$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

qemu-i386 Compiler/pawncc/pawncc \
  -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" \
  gamemodes/gamemode.pwn \
  -iincludes \
  -iCompiler/samp03/include \
  -d3

ls -l Compiler/samp03/gamemodes

cd Compiler/samp03
LD_LIBRARY_PATH=.:./plugins:/usr/lib32:/usr/lib/i386-linux-gnu ./samp03svr
```

Das ist der komplette und saubere Ablauf.

---

## 📘 14. Erfolgsprüfung: Wie erkenne ich, dass alles korrekt ist?

Wenn der Build und der Start sauber liefen, siehst du in der Serverkonsole typischerweise folgende Merkmale:

- der Gamemode wird als `gtasa_reallife` geladert
- der Server läuft auf dem konfigurierten Port
- keine Warnung `Game Mode: Unknown`
- kein Default-Spawn im Standard-CJ-/Blueberry-Setup
- der Spieler spawnt in der echten Reallife-Welt

Beispielhafte Konsolenausgabe:

```text
[gamemode] Loading gamemode: gtasa_reallife
[server] Listening on port 7780
[gamemode] GameModeInit finished
```

Wenn du diese Meldungen siehst, ist der Source-Flow sauber und der Server nutzt die korrekte AMX-Datei.

---

## 📘 15. Häufige Fehler und wie man sie vermeidet

### 🔧 Fehler 1: Falsche AMX-Datei im Serverordner

Symptom:

- Server startet nicht mit dem gewünschten Modus
- falscher oder alter Modus wird geladen

Lösung:

```bash
find Compiler/samp03/gamemodes -maxdepth 1 -type f -name '*.amx' -print
```

Wenn dort mehr als eine Datei steht, entferne alle Aliase außer `gtasa_reallife.amx`.

### 🔧 Fehler 2: Server.cfg stimmt nicht mit dem AMX-Name überein

Symptom:

- Modus wird als unbekannt behandelt
- Spawn fällt auf Standardumgebung zurück

Lösung:

```cfg
gamemode0 gtasa_reallife 1
```

### 📘 Fehler 3: SQLite-Pfad fehlt

Symptom:

- DB-Fehler beim Start
- persistente Daten werden nicht verwaltet

Lösung:

```bash
mkdir -p data/db Compiler/samp03/data/db
```

### 🏗️ Fehler 4: Compiler auf 64-Bit-System ohne qemu

Symptom:

- Pawn-Compiler läuft nicht korrekt
- System meldet fehlende 32-Bit-Ausführung oder ABI-Fehler

Lösung:

```bash
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install -y qemu-user libc6-i386 lib32gcc-s1 lib32stdc++6
```

---

## 📘 16. Die Kurzfassung, die man immer wieder braucht

Wenn du alles in einem Satz zusammenfassen willst:

- Der Code lebt im Root unter `gamemodes/gamemode.pwn`.
- Der Compiler kompiliert diesen Source direkt in `Compiler/samp03/gamemodes/gtasa_reallife.amx`.
- Der Server startet nur mit dieser Datei und `gamemode0 gtasa_reallife 1`.
- Es gibt keine Duplicate, keine zusätzlichen Alias-Dateien und keine Server-Ordner-Editierung.

Das ist die Grundregel des gesamten Projekts.
---

# Alternative Projektansicht

# HowToSetup – Nexus-Core-Gamemode

## 🧭 Überblick

Dieses Repository ist ein modularer GTA:SA-Reallife-Gamemode für SA:MP. Es ist bewusst nicht als ein einziges monolithisches `gamemode.pwn` gebaut, sondern als Mehrschichten-Projekt mit getrennten Bereichen für Core, State, Economy, Jobs, Factions, Housing, Inventory, UI, Auth und Web-Management.

Das Projekt ist so organisiert, dass man auf einem normalen V-Server oder eigenem Linux-Host den Gamemode starten, konfigurieren und betreiben kann, ohne das gesamte Projekt in einem einzelnen Pfad zu vermischen.

Wichtige Grundidee:

- `gamemodes/gamemode.pwn` ist der Einstiegspunkt
- `gamemodes/modules/` enthält die eigentliche Spiel-Logik in modularem Aufbau
- `config/` enthält Server- und DB-Konfiguration
- `data/db/` enthält SQLite-Dateien
- `control-panel/` enthält das Web-Management-Interface
- `includes/` enthält SA:MP- und Projekt-Includes
- `Compiler/` enthält die lokale Pawn-/Compiler-Umgebung

---

## 🏗️ 1. Ordnerstruktur verstehen

Im Root des Projekts liegen diese Hauptbereiche:

- `gamemodes/`
  - `gamemode.pwn` – Haupt-Gamemode-Datei, Startpunkt des Projekts
  - `modules/` – modulare Systemlogik

- `includes/`
  - SA:MP-Includes und projektbezogene Helfer
  - wichtiger Teil für die Kompilierung und Server-Integration

- `config/`
  - Serverkonfigurationen und Datenbankkonfiguration
  - z. B. `server.cfg`, `database.ini`

- `data/`
  - Laufzeit- und Persistenzdateien
  - `data/db/` für die SQLite-Datenbank
  - `data/sql/` für SQL-Skripte / Schema-Dateien

- `control-panel/`
  - Node.js-Webinterface zum Server-Status, Monitoring und Admin-Checks

- `Compiler/`
  - lokale Compiler-/Toolchain-Umgebung, falls im Projekt enthalten

- `scripts/`
  - Hilfs- und Setup-Skripte

- `tools/`
  - Log-/Analyse-/Utility-Skripte

- `server.cfg`
  - Root-Level Server-Konfigurationsdatei für SA:MP

- `sql.cfg`
  - mögliche SQL- oder Datenbank-Optionen für den Server

---

## 📘 2. Was genau ist wo?

### 📘 2.1 Gamemode-Logik

Pfad:

- `gamemodes/gamemode.pwn`

Das ist die zentrale Einstiegspunkt-Datei, die den Server initialisiert und die Module einbindet. Dort liegen normalerweise:

- Include-Reihenfolge
- Serverstartup-Initialisierung
- OnGameModeInit / OnGameModeExit
- Module-Registration
- globale Event- und Lifecycle-Calls

Wenn man das Projekt auf einem Server laufen lassen will, ist diese Datei der wichtige Einstiegspunkt.

### 🏗️ 2.2 Module

Pfad:

- `gamemodes/modules/`

Hier sind die einzelnen Spielsysteme untergebracht. Beispielbereiche:

- `core/` – gemeinsame Spielerdaten, State- und Core-Logik
- `state/` – Staats-, Treasury-, Spawn- und Verwaltungslogik
- `jobs/` – Jobsysteme und Job-Übergänge
- `factions/` – Fraktionen, Wars, Gang- und Verbandslogik
- `ui/` – UI-, Dialog- und Textdraw-Komponenten
- `admin/` – Adminsystem
- `auth/` – Login/Register/Tutorial-Flow
- `db/` – Datenbank-Abstraktion
- `economy/` – Wirtschaft, Kasse, Preise, Shops
- `business/` – Firmen-/Geschäftslogik
- `housing/` – Häuser, Safes, Inventare
- `inventory/` – Inventar, Item-Logik
- `vehicles/` – Fahrzeugsysteme
- `tutorial/` – Tutorial-/Intro-Logik

Die modulares Struktur ist wichtig, weil jedes System unabhängig entwickelt und später erweitert werden kann.

### 📘 2.3 Includes

Pfad:

- `includes/`

Diese Ordner enthalten:

- SA:MP Basisklassen
- Projekt-Helfer
- Sonderincludefiles
- DB-/String-/Timer-/Utility-Helpers
- generische Tools, die im Gamemode eingebunden werden

Beispiele aus dem Projekt:

- `includes/a_samp.inc`
- `includes/sscanf2.inc`
- `includes/mysql.inc`
- `includes/streamer.inc`
- `includes/progressbars.inc`
- `includes/regex.inc`
- `includes/string.inc`

Diese Dateien müssen korrekt in der Projektstruktur und Compiler-Umgebung vorhanden sein, sonst schlägt die Kompilierung fehl.

### 📘 2.4 Konfiguration

Pfad:

- `config/server.cfg`
- `config/database.ini`
- `server.cfg`
- `sql.cfg`

Dort werden die wichtigsten Laufzeitwerte definiert:

- Servername
- Port
- Maximalspieler
- Plugins
- RCON-Passwort
- Datenbankpfad
- Login-/DB-Parameter

### 📘 2.5 Datenbank

Pfad:

- `data/db/`
- `data/sql/`

Das Projekt ist Dini-first aufgebaut. Das heißt, die primäre Persistenz erfolgt über lokale INI-/Datei-Storage im Laufzeitroot unter `Compiler/samp03/scriptfiles/`. Optional kann eine SQLite-/MySQL-Autosync-Schicht im laufenden Betrieb aktiviert werden, aber sie darf die Serverinitialisierung nicht erzwingen.

Typische Dateien:

- `data/db/gtasa_reallife.db`
- SQL-Skripte in `data/sql/`

### 🧩 2.6 Control Panel

Pfad:

- `control-panel/`

Das Control Panel ist eine Node.js-Anwendung mit Express und SQLite. Es dient zur Serverüberwachung, Adminansicht und Betriebsdatenabfrage. Das Frontend liegt unter `control-panel/public`.

Wichtige Dateien:

- `control-panel/package.json`
- `control-panel/server.js`
- `control-panel/public/index.html`
- `control-panel/public/app.js`

### 📘 2.7 Compiler / Toolchain

Pfad:

- `Compiler/`
- `Compiler/pawncc/`
- `Compiler/samp03/`

Dort liegen die kompilierten bzw. lokal mitgelieferten Builder- und Pawn-Tools. Das Projekt erwartet je nach Server-Umgebung eine lokale oder vorinstallierte SA:MP-/Pawn-Toolchain, damit der Gamemode in eine `.amx`-Datei übersetzt werden kann.

---

## 🔧 3. Was du für einen V-Server brauchst

Wenn du das Projekt auf einem V-Server betreiben willst, brauchst du normalerweise diese Dinge:

1. Linux-Server (Ubuntu/Debian bevorzugt)
2. SA:MP-Server-Installation
3. Pawn-Compiler oder bereits kompilierte `.amx`
4. Zugriff auf das Projektverzeichnis
5. ggf. Node.js für das Control Panel
6. SQLite / MySQL je nach Setup
7. Systemd- oder Screen-Startskripte für den Server

### 🔧 Empfohlene Grundstruktur auf dem V-Server

```bash
/home/youruser/
├── sampserver/
│   ├── server.cfg
│   ├── gamemodes/
│   │   └── gtasa_reallife.amx
│   ├── plugins/
│   ├── filterscripts/
│   ├── includes/
│   ├── data/
│   │   └── db/
│   └── logs/
├── nexus-core/
│   ├── config/
│   ├── control-panel/
│   ├── gamemodes/
│   ├── includes/
│   └── data/
└── scripts/
```

Das Projekt selbst kann direkt im Server-Verzeichnis liegen oder als Quellrepo im `/opt` bzw. `/srv` abgelegt werden, während der Server nur die kompilierten Dateien und Configs nutzt.

---

## 🔧 4. Server-Config richtig einrichten

### 🔧 4.1 `server.cfg`

Das Projekt enthält eine Standard-Datei:

```cfg
# SA:MP Server Configuration
# Base game mode
gamemode0 gtasa_reallife 1

# Plugins order
plugins crashdetect streamer sscanf2 sqlite mysql zcmd

# Server details
hostname GTA:SA Reallife Modular
maxplayers 100
port 7780
query 1
lanmode 0
rcon_password changeme
announce 0
weburl http://localhost
maxnpc 0

# Logging
logtimeformat [%H:%M:%S]
```

Das ist die Basis. Für einen echten V-Server musst du das an deine Infrastruktur anpassen:

- `hostname` auf deinen Servernamen setzen
- `rcon_password` ändern
- `port` auf den aktiven Host-Port der lokalen 7780-Umgebung setzen
- `gamemode0 gtasa_reallife 1` muss auf den tatsächlich kompilierten Modus passen
- Plugins je nach Serverumgebung aktivieren oder entfernen

### 🔧 4.2 `config/server.cfg`

Im `config/`-Ordner liegt eine Projekt-Variante mit einer etwas moderneren Struktur. Das ist eher die Projektkonfiguration als die reine Server-Startdatei.

Diese Datei verwendet eine allgemeine Deployment-Konfiguration:

```cfg
# Deployment config for the modular RP server
# This file mirrors the runtime defaults used by the project and is intended for local/dev setups.

hostname "GTA:SA Reallife Modular"
maxplayers 100
port 7780
gamemode0 gtasa_reallife 1
plugins crashdetect streamer sscanf2 sqlite mysql zcmd
filterscripts
rcon_password "changeme"
```

**Wichtig:**

- Wenn du den Server produktiv aufsetzt, nimm die Datei aus `config/` als Vorlage.
- Kopiere sie an den richtigen SA:MP-Serverort und passe sie für deinen Host an.

### 📘 4.3 `sql.cfg`

Diese Datei ist für SQL-spezifische Optionen bzw. das Anpassen der Datenbank-Verbindung vorgesehen. In vielen SA:MP-Setups wird sie zusammen mit Plugins genutzt. Für dieses Projekt ist die Nutzung von SQLite als Standard vorgesehen, weshalb die `database.ini` im `config/`-Ordner die jeweils maßgebliche Konfiguration ist.

---

## 📘 5. Datenbank einrichten

### 🔧 5.1 SQLite-Setup

Die Standard-Konfiguration im Projekt ist SQLite:

```ini
[database]
backend = sqlite
sqlite_path = data/db/gtasa_reallife.db
sqlite_timeout = 30

[mysql]
enabled = false
host = localhost
port = 3306
user = root
password = changeme
database = gtasa_reallife
```

Das bedeutet:

- SQLite wird benutzt
- die Datei liegt in `data/db/gtasa_reallife.db`
- wenn die Datei nicht existiert, braucht das Projekt beim Start eine Initialisierung
- die Datenbank sollte auf dem V-Server persistent sein

### 📘 5.2 Verzeichnis vorbereiten

Auf dem Server musst du sicherstellen, dass das Datenbank-Verzeichnis existiert:

```bash
mkdir -p /home/youruser/sampserver/data/db
chmod -R 755 /home/youruser/sampserver/data
```

### 📘 5.3 Datenbankdatei anlegen

Wenn das Projekt SQL-Skripte für Initialisierung enthält, siehst du sie im `data/sql`-Ordner. Dann kannst du den Server mit initialer DB-Erstellung starten oder die Skripte manuell ausführen.

Beispiel:

```bash
sqlite3 /home/youruser/sampserver/data/db/gtasa_reallife.db < /path/to/project/data/sql/schema.sql
```

Wenn keine SQL-Datei vorhanden ist, startest du den Gamemode mit aktivierter SQLite-Initialisierung und prüfst dann die Datenbank-Datei.

---

## 📘 6. Projekt kompilieren

### 📘 6.1 Voraussetzungen

Du brauchst eine funktionierende Pawn-Toolchain. Das Repository enthält Ordner unter `Compiler/`, aber der genaue Buildpfad hängt davon ab, ob der Server mit einer lokal bereitgestellten Toolchain kompiliert werden soll.

Dazu gehören typischerweise:

- `pawncc`
- `samp-npc` falls relevant
- passende Include-Pfade
- SA:MP-Header / Includes

Schau in `Compiler/pawncc` und `Compiler/samp03` nach, ob dort die lokalen Compiler-Artefakte liegen.

### 📘 6.2 Beispiel für Kompilierung

Eine typische Pawn-Kompilierung sieht so aus:

```bash
./Compiler/pawncc/pawncc -;+ -( +) -\
    -v -i./includes -i./gamemodes -d3 gamemodes/gamemode.pwn
```

Je nach Projekt-Setup kann der Befehl leicht variieren. Das Wichtigste ist:

- `includes` korrekt im Include-Pfad
- `gamemodes/gamemode.pwn` als Quell-Datei
- Ausgabe als `.amx`

Wenn du mit der lokalen Repo-Toolchain arbeitest, nutze die im Projekt vorhandenen Pfade und nicht einen globalen Linux-Pawn-Compiler, wenn der Projekt-Wrapper dafür vorgesehen ist.

### 📘 6.3 Ausgabe

Das Ergebnis ist normalerweise:

- `Compiler/samp03/gamemodes/gtasa_reallife.amx`

Die `.amx`-Datei wird direkt als der einzige akzeptierte Laufzeitmodus im Serverordner verwendet. Ein Alias oder zusätzliches `gamemode.amx` im selben Ordner ist nicht erlaubt.

---

## 🔧 7. Gamemode auf dem V-Server einsetzen

### 📘 7.1 SA:MP-Ordner vorbereiten

Beispiel:

```bash
/home/youruser/sampserver/
├── server.cfg
├── gamemodes/
│   └── gtasa_reallife.amx
├── plugins/
├── filterscripts/
├── data/
│   └── db/
│       └── gtasa_reallife.db
├── logs/
└── server
```

### 📘 7.2 `.amx` bereitstellen

```bash
cp /path/to/project/Compiler/samp03/gamemodes/gtasa_reallife.amx /home/youruser/sampserver/gamemodes/gtasa_reallife.amx
```

### 📘 7.3 Config kopieren

```bash
cp /path/to/project/server.cfg /home/youruser/sampserver/server.cfg
```

### 📘 7.4 Datenbankpfad prüfen

Stelle sicher, dass der Datenbankpfad auf dem Server wirklich so aussieht, wie die Script-Logik erwartet:

```ini
sqlite_path = data/db/gtasa_reallife.db
```

Wenn der Server im anderen Verzeichnis läuft, musst du den Pfad entsprechend anpassen.

---

## 🧩 8. Control Panel einrichten

Das Web-Panel liegt in `control-panel/`.

### 🔧 8.1 Abhängigkeiten installieren

```bash
cd /path/to/project/control-panel
npm install
```

### 🔧 8.2 Starten

```bash
npm start
```

oder im Entwicklungsmodus:

```bash
npm run dev
```

### 📘 8.3 Default-Endpoint

Das Panel läuft in der Regel auf:

```text
http://IP:PORT
```

Je nach Serverkonfiguration und geöffneter Portfreigabe.

### 🧩 8.4 Zweck des Panels

Das Panel dient für:

- Serverstatus
- Spieler- und Admin-Übersicht
- DB-/State-Status
- Economy-/State-Checks
- Monitoring und Betriebsdaten

---

## 🔧 9. Systemd- oder Service-Setup auf Linux

Wenn du den SA:MP-Server permanent auf einem V-Server betreiben willst, ist ein Service sinnvoll.

### 📘 9.1 Beispiel-Service

Erstelle eine Datei wie `/etc/systemd/system/sampserver.service`:

```ini
[Unit]
Description=SA:MP Nexus Core Gamemode Server
After=network.target

[Service]
Type=simple
User=youruser
WorkingDirectory=/home/youruser/sampserver
ExecStart=/home/youruser/sampserver/server
Restart=on-failure
RestartSec=5

[Install]
WantedBy=multi-user.target
```

Dann aktivieren:

```bash
sudo systemctl daemon-reload
sudo systemctl enable sampserver
sudo systemctl start sampserver
sudo systemctl status sampserver
```

### 📘 9.2 Logfiles prüfen

```bash
journalctl -u sampserver -f
```

---

## 🔧 10. Sicherheit und V-Server-Checklist

Bei einem produktiven V-Server solltest du diese Punkte beachten:

- `rcon_password` auf starkes Geheimnis setzen
- Ports nur nötig offen lassen
- DB-Dateien außerhalb des Webroots aufbewahren, wenn möglich
- Server Logs sichern
- `server.cfg` nicht öffentlich teilen
- API- oder Web-Endpoints nur intern freigeben
- Updates und Backups regelmäßig machen

---

## 🔧 11. Empfohlene Standard-Setup-Reihenfolge

Wenn du das Projekt von null an auf einem V-Server einrichtest, gehst du so vor:

1. SA:MP-Server installieren
2. Verzeichnisstruktur anlegen
3. `server.cfg` erzeugen oder anpassen
4. `Compiler/samp03/gamemodes/gtasa_reallife.amx` mit der lokalen Pawn-Toolchain kompilieren
5. die fertige `.amx` in den SA:MP-Serverpfad als `gamemodes/gtasa_reallife.amx` übernehmen
6. `data/db/` vorbereiten
7. SQLite-Datenbank initialisieren
8. `config/database.ini` anpassen
9. Control Panel installieren
10. Server starten
11. Logs prüfen
12. Funktionen und Module testen

---

## 📘 12. Typische Fehler und Lösungen

### 📘 Fehler: Gamemode wird nicht gefunden

Ursache:

- `gamemode0 gtasa_reallife 1` passt nicht zur `.amx`-Datei
- Datei liegt an anderem Pfad

Lösung:

- Name der `.amx` und der `server.cfg` divergieren
- `.amx` richtig in `gamemodes/` verschieben

### 📘 Fehler: Datenbank fehlt

Ursache:

- `data/db/gtasa_reallife.db` nicht angelegt
- SQLite-Initialisierung fehlt

Lösung:

- Verzeichnis anlegen
- Datenbank erzeugen
- SQL-Skripte aus `data/sql/` ausführen

### 📘 Fehler: Includes fehlen

Ursache:

- `includes/` nicht korrekt vorhanden
- der Compiler nutzt falsche Pfade

Lösung:

- Projektstruktur unverändert lassen
- Include-Pfad im Compiler korrekt setzen

### 🖥️ Fehler: Plugins fehlen

Ursache:

- in `server.cfg` und `plugins` werden Plugins erwartet
- Plugindateien fehlen im Serverordner

Lösung:

- Plugins installieren
- Plugin-Liste mit dem tatsächlichen Ordner vergleichen

### 📘 Fehler: RCON / Verbindung fehlschlägt

Ursache:

- `rcon_password` falsch
- Server blockiert Port / Firewall

Lösung:

- Passwort prüfen
- Firewall öffnen
- SA:MP-Port prüfen

---

## 🔧 13. Praktische Server-Umgebung

Für einen realen V-Server würde ich das so aufsetzen:

- Betriebssystem: Ubuntu 22.04 LTS oder Debian 12
- SA:MP-Server im Ordner `/srv/sampserver`
- Projektquellen im Ordner `/opt/nexus-core-gamemode`
- Datenbank unter `/srv/sampserver/data/db/`
- Logs unter `/var/log/samp/` oder im Serverordner `logs/`
- Web-Panel im eigenen Subdomain-Setup wie `panel.domain.tld`

---

## 📘 14. Kurzfassung

Wenn du dieses Projekt vollständig auf einem V-Server betreiben willst, dann:

1. Das Repository sauber klonen
2. Die Server-Config mit `server.cfg` und `config/server.cfg` anpassen
3. Die kompilierten SA:MP-Gamemode-Dateien in `gamemodes/` einsetzen
4. SQLite-Datenbank anlegen und `config/database.ini` richtig setzen
5. Das Control Panel mit Node.js starten
6. Den SA:MP-Server als Service betreiben
7. Logs und DB kontinuierlich prüfen

Damit ist das Projekt vollständig als eigener, modularer Reallife-Gamemode auf einem V-Server lauffähig und betreibbar.

---

## 📘 15. Empfehlung für dieses Projekt

Das Projekt ist bereits als modularer Basis-Gamemode angelegt und eignet sich gut für:

- lokalen Testbetrieb
- V-Server-Deployment
- strukturierte Erweiterung einzelner Module
- langfristige Entwicklung ohne kompletten Rewrite

Wenn du es produktiv betreibst, solltest du bei einer echten Live-Umgebung zusätzlich prüfen:

- DB-Schema und Initialisierung
- Permission-Handling für Admins
- Backups und Recovery
- Web- und Panel-Access
- Server-Updates und Plugin-Konsistenz

---

## 📘 16. Abschluss

Dieses Repository ist kein bloßer Prototyp, sondern eine modulare SA:MP-Serverbasis mit klaren Strukturprinzipien, deutlich getrennten Bereichen und sauberem Setup-Pfad. Wenn du die beschriebenen Ordner, Konfigurationen und Installationsschritte befolgst, kannst du das Projekt vollständig als Server-Setup für einen V-Server betreiben und später weiter ausbauen.

Die beste Praxis ist immer:

- Projektquellen sauber halten
- Produktionskonfigurationen getrennt von Entwicklungsconfigs aufbewahren
- Datenbankpfade und Plugins prüfen
- Start- und Restart-Prozesse mit Service-Management automatisieren

Damit bleibt das Projekt stabil, sichtbar und produktionsnah nutzbar.


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
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut (nur legacy warning in `includes/dini.inc`).
- Dini-first + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

