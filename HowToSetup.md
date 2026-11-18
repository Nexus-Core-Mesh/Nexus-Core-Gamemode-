# HowToSetup – Nexus-Core-Gamemode

## Überblick

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

## 1. Ordnerstruktur verstehen

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

## 2. Was genau ist wo?

### 2.1 Gamemode-Logik

Pfad:

- `gamemodes/gamemode.pwn`

Das ist die zentrale Einstiegspunkt-Datei, die den Server initialisiert und die Module einbindet. Dort liegen normalerweise:

- Include-Reihenfolge
- Serverstartup-Initialisierung
- OnGameModeInit / OnGameModeExit
- Module-Registration
- globale Event- und Lifecycle-Calls

Wenn man das Projekt auf einem Server laufen lassen will, ist diese Datei der wichtige Einstiegspunkt.

### 2.2 Module

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

### 2.3 Includes

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

### 2.4 Konfiguration

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

### 2.5 Datenbank

Pfad:

- `data/db/`
- `data/sql/`

Das Projekt ist SQLite-first aufgebaut. Das heißt, die primäre Persistenz erfolgt über lokale SQLite-Datenbanken und SQL-Skripte. Die Produktivkonfiguration kann später auf MySQL erweitert werden, aber die Basis ist heute auf SQLite ausgerichtet.

Typische Dateien:

- `data/db/gtasa_reallife.db`
- SQL-Skripte in `data/sql/`

### 2.6 Control Panel

Pfad:

- `control-panel/`

Das Control Panel ist eine Node.js-Anwendung mit Express und SQLite. Es dient zur Serverüberwachung, Adminansicht und Betriebsdatenabfrage. Das Frontend liegt unter `control-panel/public`.

Wichtige Dateien:

- `control-panel/package.json`
- `control-panel/server.js`
- `control-panel/public/index.html`
- `control-panel/public/app.js`

### 2.7 Compiler / Toolchain

Pfad:

- `Compiler/`
- `Compiler/pawncc/`
- `Compiler/samp03/`

Dort liegen die kompilierten bzw. lokal mitgelieferten Builder- und Pawn-Tools. Das Projekt erwartet je nach Server-Umgebung eine lokale oder vorinstallierte SA:MP-/Pawn-Toolchain, damit der Gamemode in eine `.amx`-Datei übersetzt werden kann.

---

## 3. Was du für einen V-Server brauchst

Wenn du das Projekt auf einem V-Server betreiben willst, brauchst du normalerweise diese Dinge:

1. Linux-Server (Ubuntu/Debian bevorzugt)
2. SA:MP-Server-Installation
3. Pawn-Compiler oder bereits kompilierte `.amx`
4. Zugriff auf das Projektverzeichnis
5. ggf. Node.js für das Control Panel
6. SQLite / MySQL je nach Setup
7. Systemd- oder Screen-Startskripte für den Server

### Empfohlene Grundstruktur auf dem V-Server

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

## 4. Server-Config richtig einrichten

### 4.1 `server.cfg`

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
port 7777
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
- `port` je nach Bedarf auf 7777 oder separat setzen
- `gamemode0 gtasa_reallife 1` muss auf den tatsächlich kompilierten Modus passen
- Plugins je nach Serverumgebung aktivieren oder entfernen

### 4.2 `config/server.cfg`

Im `config/`-Ordner liegt eine Projekt-Variante mit einer etwas moderneren Struktur. Das ist eher die Projektkonfiguration als die reine Server-Startdatei.

Diese Datei verwendet eine allgemeine Deployment-Konfiguration:

```cfg
# Deployment config for the modular RP server
# This file mirrors the runtime defaults used by the project and is intended for local/dev setups.

hostname "GTA:SA Reallife Modular"
maxplayers 100
port 7777
gamemode0 gtasa_reallife
plugins crashdetect streamer sscanf2 sqlite mysql zcmd
filterscripts
rcon_password "changeme"
```

**Wichtig:**

- Wenn du den Server produktiv aufsetzt, nimm die Datei aus `config/` als Vorlage.
- Kopiere sie an den richtigen SA:MP-Serverort und passe sie für deinen Host an.

### 4.3 `sql.cfg`

Diese Datei ist für SQL-spezifische Optionen bzw. das Anpassen der Datenbank-Verbindung vorgesehen. In vielen SA:MP-Setups wird sie zusammen mit Plugins genutzt. Für dieses Projekt ist die Nutzung von SQLite als Standard vorgesehen, weshalb die `database.ini` im `config/`-Ordner die jeweils maßgebliche Konfiguration ist.

---

## 5. Datenbank einrichten

### 5.1 SQLite-Setup

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

### 5.2 Verzeichnis vorbereiten

Auf dem Server musst du sicherstellen, dass das Datenbank-Verzeichnis existiert:

```bash
mkdir -p /home/youruser/sampserver/data/db
chmod -R 755 /home/youruser/sampserver/data
```

### 5.3 Datenbankdatei anlegen

Wenn das Projekt SQL-Skripte für Initialisierung enthält, siehst du sie im `data/sql`-Ordner. Dann kannst du den Server mit initialer DB-Erstellung starten oder die Skripte manuell ausführen.

Beispiel:

```bash
sqlite3 /home/youruser/sampserver/data/db/gtasa_reallife.db < /path/to/project/data/sql/schema.sql
```

Wenn keine SQL-Datei vorhanden ist, startest du den Gamemode mit aktivierter SQLite-Initialisierung und prüfst dann die Datenbank-Datei.

---

## 6. Projekt kompilieren

### 6.1 Voraussetzungen

Du brauchst eine funktionierende Pawn-Toolchain. Das Repository enthält Ordner unter `Compiler/`, aber der genaue Buildpfad hängt davon ab, ob der Server mit einer lokal bereitgestellten Toolchain kompiliert werden soll.

Dazu gehören typischerweise:

- `pawncc`
- `samp-npc` falls relevant
- passende Include-Pfade
- SA:MP-Header / Includes

Schau in `Compiler/pawncc` und `Compiler/samp03` nach, ob dort die lokalen Compiler-Artefakte liegen.

### 6.2 Beispiel für Kompilierung

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

### 6.3 Ausgabe

Das Ergebnis ist normalerweise:

- `gamemodes/gamemode.amx`

Die `.amx`-Datei wird anschließend in den SA:MP-Serverordner verschoben bzw. als `gamemode0`-Mode gestartet.

---

## 7. Gamemode auf dem V-Server einsetzen

### 7.1 SA:MP-Ordner vorbereiten

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

### 7.2 `.amx` kopieren

```bash
cp /path/to/project/gamemodes/gamemode.amx /home/youruser/sampserver/gamemodes/gtasa_reallife.amx
```

### 7.3 Config kopieren

```bash
cp /path/to/project/server.cfg /home/youruser/sampserver/server.cfg
```

### 7.4 Datenbankpfad prüfen

Stelle sicher, dass der Datenbankpfad auf dem Server wirklich so aussieht, wie die Script-Logik erwartet:

```ini
sqlite_path = data/db/gtasa_reallife.db
```

Wenn der Server im anderen Verzeichnis läuft, musst du den Pfad entsprechend anpassen.

---

## 8. Control Panel einrichten

Das Web-Panel liegt in `control-panel/`.

### 8.1 Abhängigkeiten installieren

```bash
cd /path/to/project/control-panel
npm install
```

### 8.2 Starten

```bash
npm start
```

oder im Entwicklungsmodus:

```bash
npm run dev
```

### 8.3 Default-Endpoint

Das Panel läuft in der Regel auf:

```text
http://IP:PORT
```

Je nach Serverkonfiguration und geöffneter Portfreigabe.

### 8.4 Zweck des Panels

Das Panel dient für:

- Serverstatus
- Spieler- und Admin-Übersicht
- DB-/State-Status
- Economy-/State-Checks
- Monitoring und Betriebsdaten

---

## 9. Systemd- oder Service-Setup auf Linux

Wenn du den SA:MP-Server permanent auf einem V-Server betreiben willst, ist ein Service sinnvoll.

### 9.1 Beispiel-Service

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

### 9.2 Logfiles prüfen

```bash
journalctl -u sampserver -f
```

---

## 10. Sicherheit und V-Server-Checklist

Bei einem produktiven V-Server solltest du diese Punkte beachten:

- `rcon_password` auf starkes Geheimnis setzen
- Ports nur nötig offen lassen
- DB-Dateien außerhalb des Webroots aufbewahren, wenn möglich
- Server Logs sichern
- `server.cfg` nicht öffentlich teilen
- API- oder Web-Endpoints nur intern freigeben
- Updates und Backups regelmäßig machen

---

## 11. Empfohlene Standard-Setup-Reihenfolge

Wenn du das Projekt von null an auf einem V-Server einrichtest, gehst du so vor:

1. SA:MP-Server installieren
2. Verzeichnisstruktur anlegen
3. `server.cfg` erzeugen oder anpassen
4. `gamemodes/gamemode.amx` kompilieren
5. `.amx` in den SA:MP-Server kopieren
6. `data/db/` vorbereiten
7. SQLite-Datenbank initialisieren
8. `config/database.ini` anpassen
9. Control Panel installieren
10. Server starten
11. Logs prüfen
12. Funktionen und Module testen

---

## 12. Typische Fehler und Lösungen

### Fehler: Gamemode wird nicht gefunden

Ursache:

- `gamemode0 gtasa_reallife 1` passt nicht zur `.amx`-Datei
- Datei liegt an anderem Pfad

Lösung:

- Name der `.amx` und der `server.cfg` divergieren
- `.amx` richtig in `gamemodes/` verschieben

### Fehler: Datenbank fehlt

Ursache:

- `data/db/gtasa_reallife.db` nicht angelegt
- SQLite-Initialisierung fehlt

Lösung:

- Verzeichnis anlegen
- Datenbank erzeugen
- SQL-Skripte aus `data/sql/` ausführen

### Fehler: Includes fehlen

Ursache:

- `includes/` nicht korrekt vorhanden
- der Compiler nutzt falsche Pfade

Lösung:

- Projektstruktur unverändert lassen
- Include-Pfad im Compiler korrekt setzen

### Fehler: Plugins fehlen

Ursache:

- in `server.cfg` und `plugins` werden Plugins erwartet
- Plugindateien fehlen im Serverordner

Lösung:

- Plugins installieren
- Plugin-Liste mit dem tatsächlichen Ordner vergleichen

### Fehler: RCON / Verbindung fehlschlägt

Ursache:

- `rcon_password` falsch
- Server blockiert Port / Firewall

Lösung:

- Passwort prüfen
- Firewall öffnen
- SA:MP-Port prüfen

---

## 13. Praktische Server-Umgebung

Für einen realen V-Server würde ich das so aufsetzen:

- Betriebssystem: Ubuntu 22.04 LTS oder Debian 12
- SA:MP-Server im Ordner `/srv/sampserver`
- Projektquellen im Ordner `/opt/nexus-core-gamemode`
- Datenbank unter `/srv/sampserver/data/db/`
- Logs unter `/var/log/samp/` oder im Serverordner `logs/`
- Web-Panel im eigenen Subdomain-Setup wie `panel.domain.tld`

---

## 14. Kurzfassung

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

## 15. Empfehlung für dieses Projekt

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

## 16. Abschluss

Dieses Repository ist kein bloßer Prototyp, sondern eine modulare SA:MP-Serverbasis mit klaren Strukturprinzipien, deutlich getrennten Bereichen und sauberem Setup-Pfad. Wenn du die beschriebenen Ordner, Konfigurationen und Installationsschritte befolgst, kannst du das Projekt vollständig als Server-Setup für einen V-Server betreiben und später weiter ausbauen.

Die beste Praxis ist immer:

- Projektquellen sauber halten
- Produktionskonfigurationen getrennt von Entwicklungsconfigs aufbewahren
- Datenbankpfade und Plugins prüfen
- Start- und Restart-Prozesse mit Service-Management automatisieren

Damit bleibt das Projekt stabil, sichtbar und produktionsnah nutzbar.
