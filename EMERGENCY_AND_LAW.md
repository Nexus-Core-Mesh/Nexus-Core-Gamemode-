# Emergency- und Law-Systeme

Dieses Dokument beschreibt die Sicherheits-, Rechts- und Notfallarchitektur des Gamemodes.

## 1. Überblick

Die wichtigsten Module sind:

- `law/law_system.inc`
- `law/jail_system.inc`
- `crime/crime_system.inc`
- `emergency/emergency_system.inc`
- `emergency/dispatch_system.inc`
- `justice/prison_system.inc`
- `justice/court_system.inc`
- `medical/ems_hospital.inc`

Zusammen bilden sie die Kernlogik für:

- Wanted-System
- Polizei-/EMS-Status
- Gefängnis- und Haftverwaltung
- Dispatch
- Notruf-/Alarmfunktionen
- Straf- und Gerichtswerte

## 2. Law-System

`law_system.inc` ist das zentrale Rechtssystem.

Es enthält TextDraws für:

- `gLawTD[playerid][0]` => "WANTED STATUS"
- `gLawTD[playerid][1]` => Wanted-Text
- `gLawTD[playerid][2]` => Role-Text

Das zeigt, dass Wanted-Status als sichtbares HUD in der Spielwelt aufgebaut ist.

### Command

- `/wanted`

Das ist im Gamemode direkt eingebunden.

## 3. Crime-System

`crime_system.inc` verwaltet:

- criminosas / heat / straftaten
- selbst definierte Crime-Infos
- TextDraw-Tracker

Das HUD zeigt:

- `CRIME TRACKER`
- Heat-Text
- Typ-Text

### Command

- `/crime`

## 4. Jail-System

`jail_system.inc` ist das Gefängnissystem.

### Commands

- `/jail <PlayerID> <Minuten> <StateID>`
- `/release <PlayerID>`

Es enthält ein Jail-HUD mit:

- `gJailTD[playerid][0]` => "JAIL HUD"
- `gJailTD[playerid][1]` => Rest- oder Boottext

Das Modul ist recht konkret und mit Admin-Rechten gekoppelt. Nur Moderatoren erhalten Zugriff.

## 5. Emergency-System

`emergency_system.inc` modelliert Polizei- und EMS-System.

### Commands

- `/ems`
- `/police`
- `/stations`

Das System prüft, ob der Spieler ein EMS- oder Polizeimitarbeiter ist. Wenn nicht, wird eine Berechtigungs-Meldung ausgegeben.

## 6. Dispatch-System

`dispatch_system.inc` ist die Einsatz- und Alarm-Verwaltung.

### Commands

- `/dispatch`
- `/acceptcall <ID>`

`/acceptcall` ist in der aktuellen Code-Analyse als nicht korrekt angebunden markiert, obwohl der Handler existiert. Das ist ein klarer Hinweis auf ein unvollständiges Anbindungsproblem.

## 7. Prison- und Court-Module

### `justice/prison_system.inc`

Verwaltet Haft- und Justizsysteme.

### `justice/court_system.inc`

Verwaltet Gerichtssysteme und rechtliche Prozesse.

Diese Module ergänzen die direkte Law-/Jail-Architektur und verankern den juristischen Rahmen in die Welt-Simulation.

## 8. EMS-Hospital-System

`medical/ems_hospital.inc` erweitert die Notfallarchitektur um Krankenhaus- und Versicherungssysteme.

Das ist die medizinische bzw. Wiederbelebungs- und Heilungsseite des Projekts.

## 9. Schnittstellen mit Core und Admin

Das Gesetz-/Notfall-System hängt eng mit Core-Werten zusammen:

- `E_PLAYER_WANTED`
- `E_PLAYER_JAIL_ACTIVE`
- `E_PLAYER_JAIL_TIME`
- `E_PLAYER_LAW_ROLE`
- `E_PLAYER_EMERGENCY_ROLE`
- `E_PLAYER_EMERGENCY_STATE`

Diese Werte sind in `core/core.inc` definiert und zeigen, dass Recht und Notfall Teil des zentralen Player-Profils sind.

## 10. Typischer Spielverlauf

Ein realistischer Ablauf im Projekt:

1. Spieler wird von Polizei oder EMS als Rolle markiert
2. Wanted-Level steigt durch Crime- oder Law-Aktionen
3. `LawSystem_ShowWanted()` zeigt den aktuellen Status
4. `CrimeSystem_ShowCrimeInfo()` zeigt die Strafaktualität
5. `DispatchSystem` kann anrufe oder Aufgaben verteilen
6. `JailSystem` kann den Spieler in Haft setzen
7. `Court` oder `Prison` übernehmen weitere Gericht-/Rechtslogik

## 11. Warum das System wichtig ist

Ohne diese Schicht hätte das RP-Projekt keine echte Rechtsgrundlage. Es wäre nicht möglich, eine clear Verantwortungsstruktur für:

- Polizei
- EMS
- Gang-/Crime-Status
- Haft
- Wanted
- Notfax-/Dispatch

zu modellieren.

## 12. Fazit

Das Emergency- und Law-System ist eines der Kern-RP-Systeme des Gamemodes. Es kombiniert:

- Criminal-State
- Legal-State
- Juridische Prozesse
- Notfallaktionen
- Zuständigkeiten und Rechte

Das macht es zu einem der wichtigsten Systembereiche für die Gesamtsimulation.


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
