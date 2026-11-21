# Spielerfortschritt und Progression

Dieses Dokument beschreibt das Progressions- und Levelsystem des Gamemodes.

## 1. Kernidee

Das Progressionssystem ist in `modules/progression/progression_system.inc` verankert. Es bildet zusammen mit `core/core.inc` das zentrale Modell für Spielerentwicklung.

Die wichtigsten Core-Werte sind:

- `E_PLAYER_LEVEL`
- `E_PLAYER_XP`
- `E_PLAYER_REP`
- `E_PLAYER_WANTED`
- `E_PLAYER_BANK`
- `E_PLAYER_PREMIUM_*`

## 2. Core-Profile für Progression

In `core/core.inc` sind folgende Konstanten relevant:

- `E_PLAYER_LEVEL = 4`
- `E_PLAYER_XP = 5`
- `E_PLAYER_REP = 6`
- `E_PLAYER_WANTED = 8`
- `E_PLAYER_PREMIUM_TIER = 15`
- `E_PLAYER_PREMIUM_EXPIRES = 16`
- `E_PLAYER_PREMIUM_PERMANENT = 17`

Das zeigt, dass die Progression nicht bloß ein einzelnes Modul ist, sondern ein Teil des zentralen Player-Profils.

## 3. Default-Werte

`Core_ResetPlayerData()` setzt bei Neuinitialisierung:

- Level = 1
- Bank = 2500
- Cash = 2500
- Wanted = 0
- Admin-Level = 0
- Premium = 0

Das macht den Spieler-Start klar und stabil.

## 4. Progression im Modul

`ProgressionSystem_Initialize()` zeigt, dass der Fortschritt als echtes System initialisiert wird:

- `[PROGRESSION] Levelsystem und Spielerfortschritt initialisiert`

Typische Logik im Modul ist:

- Sichtbarmachen von Statistiken
- Fortschrittscheck bei Level-up
- Spieler-Rep-Kennzahlen
- Spielerstatus- und Job-Verbundenheit

## 5. Statistiken und Überblick

Das Projekt hat in `gamemode.pwn` den Command:

- `/stats`

Dieser führt `ProgressionSystem_ShowStats(playerid)` aus.

Das zeigt, dass Progression als sichtbares UI-Element im Spiel eingebunden ist.

## 6. Verbindung zu Jobs und Staat

Progression ist nicht isoliert. Sie greift auf andere Modulsysteme zu:

- Jobsystem liefert Berufserfolge und Lohn
- State-System liefert wirtschaftlichen und politischen Status
- Wanted/Crime-System beeinflusst RP-Status und Polizei-Interaktion
- Premium-System erweitert die Spielerrolle und Vorteile

## 7. Wanted / Law / Crime in der Progression

Die Progression interagiert auch mit Rechts-/Kriminalitätsstatus:

- `E_PLAYER_WANTED`
- `LawSystem_ShowWanted(playerid)`
- `CrimeSystem_ShowCrimeInfo(playerid)`

Das bedeutet: Ein Spieler kann nicht nur Leveln, sondern auch seinen kriminellen, rechtlichen und sozialen Status aufbauen.

## 8. Premium und langfristige Auszeichnungen

Im Core-Und-Premium-Design stehen Werte wie:

- Premium-Tier
- Premium-Ablaufzeit
- permanenter Premium-Status

Dies zeigt, dass das Player-Profil nicht nur Level und Bank, sondern auch langfristige Statusrechte verwaltet.

## 9. Typische Progression-Logik im Projekt

Ein realistischer Verlauf:

1. Spieler startet mit Level 1 und 2500$ Bank
2. Jobs, Law, Economy, Business und State-Systeme beeinflussen seine Entwicklung
3. `E_PLAYER_XP` und `E_PLAYER_REP` können wachsen
4. `E_PLAYER_LEVEL` kann ansteigen
5. `LawSystem` und `CrimeSystem` beeinflussen Status und Korrektheit der RP-Identität
6. Premium-System kann bestimmte Vorteile hinzufügen

## 10. Warum das wichtig ist

Progression ist eine zentrale Verknüpfung zwischen:

- Spielerverhalten
- Wirtschaft
- Staat
- Fraktionen
- Jobs
- RP-Status

Ohne die Progressionslogik würden die meisten anderen Systeme merkbar an Tiefe verlieren.

## 11. Risiken und Ausbaupunkte

Der aktuelle Stand zeigt eine solide Basis, aber die echte Ausprägung der Progression hängt stark vom übrigen Modul-Stack ab:

- noch nicht jede Progressionsfunktion hat einen klaren sichtbaren UI-Auslöser
- einige Systembereiche sind vorbereitet, aber nicht vollständig orchestriert
- das klare Level-/XP-Modell ist vorhanden, aber die komplette Belohnungslogik kann erweitert werden

## 12. Fazit

Das Progressionssystem ist keineswegs nur eine Info-Zeile. Es ist ein fester Bestandteil der Game-Loop, weil es:

- Spielerdaten steuert
- Upgrade-Mechanik mit anderen Systemen verknüpft
- mit Reputation, Jobs, Wirtschaft und Wanted-System zusammenarbeitet
- Premium-/VIP-Elemente einschließt

Das macht es zu einem Kernpunkt der RP- und Gameplay-Architektur.


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
