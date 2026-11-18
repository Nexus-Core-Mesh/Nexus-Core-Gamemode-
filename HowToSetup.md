# HowToSetup – Der ultimative Einstieg für Einsteiger

## 1. Was hier genau passiert

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

## 2. So sieht das Projekt nach dem Entpacken aus

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
- `data/db/` ist der SQLite-Speicherort

---

## 3. Warum der Quellcode im Root liegt

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

## 4. Was bedeutet „Single-Target“-Modell?

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

## 5. Vorbereitungen: Ordner anlegen

Als Erstes solltest du den Server- und Datenbank-Pfad sauber vorbereiten.

### 5.1 In das Projekt wechseln

```bash
cd ~/Nexus-Core-Gamemode
pwd
ls -la
```

Du solltest den Root des Projekts sehen, inklusive Ordnern wie `gamemodes`, `includes`, `Compiler`, `scripts`, `config` und `data`.

### 5.2 Erforderliche Ordner erzeugen

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

## 6. Der Build-Schritt: Warum `qemu-i386` nötig ist

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

## 7. Der genaue komplette Build-Befehl, copy-paste-fertig

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

## 8. Nach dem Build automatisch prüfen

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

## 9. Wichtige Regeln: nie im Serverordner editieren

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

## 10. Konfiguration: `server.cfg` und der Modusname

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

## 11. SQLite-Datenbank vorbereiten

Das Projekt nutzt eine lokale SQLite-Datenbank, die im Root unter `data/db/` liegen soll.

### 11.1 Verzeichnis anlegen

```bash
mkdir -p data/db
ls -ld data/db
```

Die Datenbank selbst kann dann als Datei existieren, zum Beispiel:

```text
data/db/gtasa_reallife.db
```

Diese Datei bleibt für Laufzeit- und Persistenzdaten relevant.

### 11.2 Wichtig für den Runtime-Server

Zusätzlich sollte der Server-Root einen entsprechenden Laufzeitpfad besitzen:

```bash
mkdir -p Compiler/samp03/data/db
```

Damit ist die Umgebung für den Server-Start sauber vorbereitet und die SQLite-Initialisierung kann ohne Pfadfehler erfolgen.

---

## 12. Server starten: vollständiger Ablauf

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

## 13. Vollständiger End-to-End-Workflow für Einsteiger

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

## 14. Erfolgsprüfung: Wie erkenne ich, dass alles korrekt ist?

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

## 15. Häufige Fehler und wie man sie vermeidet

### Fehler 1: Falsche AMX-Datei im Serverordner

Symptom:

- Server startet nicht mit dem gewünschten Modus
- falscher oder alter Modus wird geladen

Lösung:

```bash
find Compiler/samp03/gamemodes -maxdepth 1 -type f -name '*.amx' -print
```

Wenn dort mehr als eine Datei steht, entferne alle Aliase außer `gtasa_reallife.amx`.

### Fehler 2: Server.cfg stimmt nicht mit dem AMX-Name überein

Symptom:

- Modus wird als unbekannt behandelt
- Spawn fällt auf Standardumgebung zurück

Lösung:

```cfg
gamemode0 gtasa_reallife 1
```

### Fehler 3: SQLite-Pfad fehlt

Symptom:

- DB-Fehler beim Start
- persistente Daten werden nicht verwaltet

Lösung:

```bash
mkdir -p data/db Compiler/samp03/data/db
```

### Fehler 4: Compiler auf 64-Bit-System ohne qemu

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

## 16. Die Kurzfassung, die man immer wieder braucht

Wenn du alles in einem Satz zusammenfassen willst:

- Der Code lebt im Root unter `gamemodes/gamemode.pwn`.
- Der Compiler kompiliert diesen Source direkt in `Compiler/samp03/gamemodes/gtasa_reallife.amx`.
- Der Server startet nur mit dieser Datei und `gamemode0 gtasa_reallife 1`.
- Es gibt keine Duplicate, keine zusätzlichen Alias-Dateien und keine Server-Ordner-Editierung.

Das ist die Grundregel des gesamten Projekts.
