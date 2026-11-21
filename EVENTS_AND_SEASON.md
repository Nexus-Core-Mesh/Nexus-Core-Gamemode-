# Events, Season und temporale Weltlogik

Dieses Dokument beschreibt die temporalen und Ereignis-systematischen Module des Gamemodes.

## 1. Überblick

Die wichtigsten Module sind:

- `events/event_system.inc`
- `events/state_event_system.inc`
- `events/season_system.inc`
- `utility/mission_board.inc`
- `state_event_system.inc`
- `social/relationship_system.inc`

Sie dienen als zeitliche, wirtschaftliche und weltliche Modulationsschicht.

## 2. Event-System

`event_system.inc` bildet die generische Eventlogik ab.

Es enthält Initialisierung und Events mit Start-/Ende-Mechanik:

- `[EVENT] Event-System initialisiert`
- `[EVENT] Event gestartet`

Das zeigt, dass das Projekt ein eigenes eventbasiertes Weltmodell besitzt, welches nicht nur statische Interaktion, sondern dynamische Weltänderungen modelliert.

## 3. State-Event-System

`state_event_system.inc` ist die staatliche Eventschicht.

### Commands

- `/events`
- `/eventtoggle <StateID>`

Das modul ist für:

- News-Logik
- Staatliche Event-Übersichten
- Eventzustand pro State
- Admin-Interaktion

## 4. Season-System

`season_system.inc` bildet die Jahreszeiten-Logik ab. Das Modul ist mit einem eigenen Dokument `SEASON.md` versehen. Das sagt bereits, dass es ein echtes erklärenwertes System ist.

Das System ist relevant für:

- Jahreszeitliche Dynamik
- Zustandsänderungen in Wirtschaft oder Jobs
- Wirtschaftspulse
- Belohnungslogik

## 5. Mission Board

`utility/mission_board.inc` ergänzt die Event- und Season-Architektur mit einer Aufgabenliste.

Es modelliert:

- mission board
- verteilte Aufgaben
- Feedback / Anzeige im UI

## 6. Beziehungen und Sozialstatus

`social/relationship_system.inc` ergänzt die Welt mit sozialen Beziehungen. Das ist zwar kein reines Event-System, aber semantisch Teil der Game-World-Logik.

Diese Beziehungen beeinflussen:

- Social RP
- Fraktions-/Business-Verbindungen
- Community-Interactions

## 7. Kombination von Event und Wirtschaft

Die Event- und Season-Module sind bewusst mit Weltwirtschaft und Jobs verbunden:

- saisonale Änderungen
- state- und economy-abhängige Zustände
- systematische Events und Aufgaben

Das macht das Projekt inagogisch nicht nur statisch, sondern zyklisch und zeitlich variabel.

## 8. Spieler-Flow

Ein typischer Event-Flow kann so aussehen:

1. Staatliches Event wird aktiviert
2. `StateEventSystem_ShowEvents()` zeigt die Anzeige
3. `EventSystem_*` startet die Laufzeitlogik
4. `SeasonSystem_*` beeinflusst War- oder Wirtschaft-Status
5. Mission Board zeigt zuständige Aufgaben an
6. Spieler kann auf Missionen oder Events reagieren

## 9. Bedeutung für das Roleplay

Events und Season schaffen ein Gefühl von dynamischer Welt. Das macht das Projekt deutlich „lebendiger“ und stärker als ein bloßes Spawn-/Bank-Setup.

## 10. Fazit

Das Event-/Season-System ist eines der wichtigsten Elemente für einen lebendigen RP-Server. Es verbindet:

- temporale Weltlogik
- Staats-Event-Logik
- Saison-Dynamik
- Missionen und Aufgaben
- soziale und wirtschaftliche Abhängigkeiten

Damit zieht es die Welt in einen zyklischen Ablauf, der die Umgebung nicht statisch, sondern laufend veränderbar macht.


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
