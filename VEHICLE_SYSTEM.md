# Fahrzeugsysteme und Fahrzeug-Architektur

Dieses Dokument beschreibt die Fahrzeug-, Garage- und Transport-Logik im aktuellen Gamemode-Stand.

## 1. Überblick

Das Projekt hat mehrere Module für Fahrzeuge:

- `vehicles/vehicle_system.inc`
- `vehicles/garage_system.inc`
- `vehicles/tuning_system.inc`
- `vehicles/fuel_station_system.inc`
- `vehicles/ubb_vehicle_system.inc`
- `economy/dealership_system.inc`
- `economy/ammu_system.inc`

Diese Systeme bilden zusammen die Fahrzeugsimulation rund um Besitz, Garage, Tuning, UBB-Fahrzeuge und Tankstellen.

## 2. Fahrzeugsystem-Basis

`vehicle_system.inc` ist das zentrale Fahrzeugsystem. Es initialisiert das Fahrzeugmodell und die Staatslogik.

Wichtige Merkmale:

- Fahrzeugsystem mit Staatslogik
- Position, Eigentümer, Staatsschnittstelle
- Fahrzeugverwaltung und -init

## 3. Garagen-System

`garage_system.inc` ist für Garagen und Fahrzeuglager zuständig.

### Commands

- `/garage`
- `/spawngarage`

Der Befehl `/garage` ist als Interface im UI-Hub überlagert, was auf eine vereinfachte Garagen-Interaktion hindeutet.

### Typische Funktionalität

- Fahrzeug aus Garage spawnen
- Fahrzeuglager/Ausstattung
- Verschiedene Garagen-Properties je Staat bzw. je Stadt

## 4. Tuning-System

`tuning_system.inc` erweitert das Fahrzeugmodell um Modifikationen.

Funktionen:

- Fahrzeug-RÄder / Optik / Einstellungs-Details
- Modifikationszustand
- späteres Anpassen für RP- und Member-Features

## 5. Fuel-Station-System

`fuel_station_system.inc` verwaltet:

- Tankstellen
- Benzinpreise
- Tankvorgänge
- eventuell bestimmte Zustands-/Staatslogik

Das ist ein wichtiges ökonomisches Subsystem im Fahrzeugbereich.

## 6. Dealership-System

`economy/dealership_system.inc` bildet die staatlichen Autohäuser ab.

Es setzt sich mit Fahrzeugverkauf und -verteilung auseinander und ist mit dem allgemeinen Fahrzeugbesitzmodell verbunden.

## 7. UBB Vehicle System

`ubb_vehicle_system.inc` ist ein eigener Bereich für Fahrzeuge mit Portal-/UBB-Integration.

Es verwaltet:

- ownership-related vehicle data
- UBB-Fahrzeug-Besitz
- Fahrzeugmodelle mit Owner/State-Attributen
- Price/Fuel/State-Flags

Die DB-Schicht enthält dafür explizite Cache-Arrays:

- `gDBUBBVehicleModelCache`
- `gDBUBBVehicleOwnerCache`
- `gDBUBBVehicleStateCache`
- `gDBUBBVehicleFuelCache`
- `gDBUBBVehiclePriceCache`

Das zeigt, dass das Fahrzeugmodell diesbezüglich recht weit entwickelt ist.

## 8. Fahrzeug- und Besitzlogik

Die Fahrzeugarchitektur verbindet:

- Player-Ownership
- State-Ownership oder State-Bezug
- Garage- und Spawn-Mechanik
- Tuning und Modifikationen
- Kraftstoff und Tankstellen
- Fahrzeugpreise und Verkauf

## 9. Interaktion mit Economy

Fahrzeuge sind eng mit Wirtschaft verbunden:

- Verkaufspreise
- Staatskasse bei Fahrzeughandel
- Geldfluss bei Autohäusern und Shops
- Tankstelle und Verbrauchskosten

## 10. Interaktion mit UI

Die Fahrzeug-Daten sind in UI-Hub- und Dialog-Mechaniken eingebunden:

- `/vehicle`
- `/veh`
- `/garage`
- Tablist-Dialoge für Fahrzeugstatus

Das erlaubt dem Spieler, Fahrzeuge über die Menüs zu steuern und zu überwachen.

## 11. Typischer Player-Flow

Ein realistischer Ablauf wäre:

1. Spieler verbindet sich
2. Garagen-Menu oder Fahrzeug-Menu wird geöffnet
3. Fahrzeug wird übernommen oder gespawnt
4. Fahrzeug besitzt einen State-/Owner-Kontext
5. Tuning oder Tankstellen werden genutzt
6. Dealership oder Verkauf setzt Preise und Eigentum in die Logik

## 12. Architekturelle Beobachtung

Das Fahrzeugsystem ist auf mehrere Subsysteme aufgeteilt, aber vom Design her logisch gruppiert:

- Basislogik
- Garage
- Tuning
- Kraftstoff
- UBB-Besitz
- Dealership

Das ist ein gutes Beispiel für modulare Erweiterung ohne monolithischen Code.

## 13. Fazit

Die Fahrzeugarchitektur ist eines der am stärksten ausgebauten Bereiche des Gamemodes. Sie hat bereits:

- Fahrzeugbesitz
- Garagen
- Modifikationen
- Tankstellen
- Händler
- UBB-/Portal-Logik

Da es mehrere Teilmodule gibt, ist die Architektur gut für Erweiterungen, muss aber bei neuen Features sauber an die zentrale Core- und DB-Logik angebunden bleiben.


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
