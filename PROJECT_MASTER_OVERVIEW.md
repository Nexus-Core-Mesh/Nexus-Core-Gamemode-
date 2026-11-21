# Nexus Core – Projekt-Master-Übersicht

Diese Datei bündelt den aktuellen Stand aller vorhandenen Dokumentationen und bildet die Gesamtübersicht über den Gamemode, seine Module, Befehle, UI und Systemstruktur.

## 1. Projektziel und Gesamtarchitektur

Das Projekt ist ein modularer GTA:SA RP Gamemode mit klarer Systemarchitektur. Die Basis liegt in:

- `gamemodes/gamemode.pwn` als Hauptentrypoint
- `gamemodes/modules/` als Kern des Systems
- `core/`, `db/`, `state/`, `economy/`, `jobs/`, `law/`, `ui/`, `vehicles/`, `factions/`, `world/`, `housing/`, `inventory/`, `events/` als fachliche Bereiche

Die Kernidee ist:

- Core definiert die zentrale Spielerdaten- und State-Logik.
- DB ist der Persistenz-/Cache-Layer.
- State bildet die politische und geografische Struktur.
- Economy, Jobs, Factions, Vehicles, Housing und Law ergänzen den RP-Lebenszyklus.
- UI und TextDraws bilden die Sichtbarkeit für Spieler.

## 2. Die wichtigsten Dokumente im Projekt

### A. Befehlsdokumentation
- `COMMANDS.md`
- Fokus: alle aktiven, verdeckten und teilweise nicht angebundenen Befehle
- Wichtig: zeigt die echte Dispatch-Reihenfolge in `OnPlayerCommandText()`

### B. Modulübersicht
- `MODULES.md`
- Fokus: gesamte Modullandschaft und fachliche Einordnung
- Wichtig: zeigt 91 `.inc`-Module in 40 Kategorien/Ordnern

### C. TextDraw- und Box-Übersicht
- `TEXTDRAWS.md`
- Fokus: globales HUD, Login-Flow, Roleplay-Dashboard, Text-Wars, State-Status, Admin-Konfig, Mapping und nicht angeschlossene HUDs
- Wichtig: unterscheidet zwischen aktiv, gebaut und nicht verbunden

### D. Modul-Abhängigkeiten
- `MODULE_DEPENDENCIES.md`
- Fokus: Initialisierungsreihenfolge und fachliche Verknüpfungen
- Wichtig: beschreibt die realen Abhängigkeiten zwischen Core, State, Economy, Jobs, UI und DB

### E. Admin- und Rechtearchitektur
- `ADMIN_SYSTEMS.md`
- Fokus: Admin-Level, State-Admin, World-Management, Projektleiter-Rechte
- Wichtig: legt das Sicherheitsmodell des Gamemodes dar

### F. State-System
- `STATE_SYSTEM.md`
- Fokus: Los Santos, San Fierro, Las Venturas
- Wichtig: politische, wirtschaftliche und geografische Grundlage des Servers

### G. Persistenz- und Datenbankmodell
- `DATABASE_SCHEMA.md`
- Fokus: DB-Backend, Cache, Dini-first-Architektur, Transaktionslogik
- Wichtig: zeigt, wie Spieler, State, Inventory, Vehicles und Housing persistiert werden

### H. UI- und Flow-Architektur
- `UI_FLOW.md`
- Fokus: Login, Tutorial, UI-Hub, Dialoge, PlayerTextDraws, Roleplay-HUD
- Wichtig: zeigt den Spielfluss von Connect bis zur Interaktion

### I. Progression
- `PLAYER_PROGRESSION.md`
- Fokus: Level, XP, Reputation, Premium, Wanted, Social-Status
- Wichtig: zeigt, wie der Spieler über gameplay- und rp-basierte Systeme aufbaut

### J. Fahrzeuge
- `VEHICLE_SYSTEM.md`
- Fokus: Garage, Tuning, Tankstellen, UBB-Fahrzeuge, Besitzlogik
- Wichtig: eines der am stärksten ausgebauten Systems

### K. Wirtschaft
- `ECONOMY_SYSTEMS.md`
- Fokus: Staatskasse, Steuern, Budget, Markets, Business, Shops
- Wichtig: Geldfluss zwischen Spieler, State und Wirtschaft

### L. Faction und War
- `FACTION_AND_WAR_SYSTEMS.md`
- Fokus: Fraktionen, Gangs, Gangwar, Text-Wars, Territorien
- Wichtig: zentrale RP-Kampfsysteme

### M. Law und Emergency
- `EMERGENCY_AND_LAW.md`
- Fokus: Wanted, Jail, Crime, Dispatch, Police/EMS, Court
- Wichtig: Recht/Notruf/Haft als Kern-Gameplay-Schicht

### N. Housing und Inventory
- `HOUSING_AND_INVENTORY.md`
- Fokus: Häuser, Storage, Inventar, Accessoires, Besitzlogik
- Wichtig: Basis für Charakter- und Immobilien-Gameplay

### O. Events und Season
- `EVENTS_AND_SEASON.md`
- Fokus: Event-Mechanik, Season-Logik, Mission Board und temporale Welt
- Wichtig: macht die Welt dynamisch und nicht nur statisch

### P. Architecture Dokumente
- `ARCHITECTURE.md`
- Fokus: systemübergreifende Gestaltung, Verzeichnisstruktur, Build-Kette
- Wichtig: technische Gesamtperspektive

## 3. Projekt-Status nach Code-Stand

Der aktuelle Stand zeigt eine gut strukturierte Base-Architektur mit klaren fachlichen Bereichen. Die wichtigsten Punkte sind:

- Die Gesamtstruktur ist modular und klar organisiert.
- Die Kernlogik liegt in Core, State, DB und UI.
- Viele Subsysteme sind bereits konzeptionell vollständig aufgebaut.
- Einige UI-, Command- und Flow-Elemente sind vorhanden, aber noch nicht vollständig in den aktiven Laufzeit-Stack eingehängt.
- Es gibt verdeckte Commands, doppelte Handler und gebautes, aber nicht angemeldetetes UI.
- Die Datenbankarchitektur ist breit und vorbereitet, auch wenn die Laufzeitbasis weiterhin dynamisch und modular bleibt.

## 4. Rollen der wichtigsten Kernbereiche

### Core
- zentrale Player-Daten
- validierte State-IDs
- Admin-Level
- Player-Profile und Grundwerte

### State
- 3 Staaten: LS, SF, LV
- Politik, Treasury, Steuer, Leader
- Spawn- und Verwaltungslogik

### Economy
- Banken, Märkte, Shops, Taxes, Business
- Geldfluss und Staatsfinanzen
- Erträge und wirtschaftliche Interaktion

### Law / Emergency
- Wanted, Jail, Crime, Police, EMS, Dispatch
- Regeln und Sicherheitslogik
- Notfalldienste und rechtliche Mechanik

### UI / TextDraws
- Dashboard-HUD
- Dialogsysteme und Menüs
- TextWars / Popup-Widgets
- Login- und Tutorial-Flows

### Factions / World
- Fraktionen, Gangs, Gebiete, Territorien
- Kriegsstatus und Einflusszonen
- Weltstruktur und politische Grenzen

## 5. Hauptfazit

Das Projekt ist aktuell als ernsthaftes, modular aufgebautes RP-System erkennbar. Es ist nicht „nur ein einfacher Gamemode-Schnipsel“, sondern eine breit angelegte Architektur mit:

- Chat-Commands
- UI-Menüs
- TextDraw-HUDs
- DB-Persistenz
- Staatslogik
- Business-/Economy-System
- Fraktions- und Law-Mechanik
- Event- und Season-Kerne

Das zeigt: Das Projekt ist bereits in einem sehr ausgebauten Architektur- und Systemstatus. Die Dokumentation ist damit nicht nur ein Überblick, sondern ein realistischer Projekt-Archivstand der aktuellen Source-Struktur.

## 6. Nächste sinnvolle Schritte

Falls weiter dokumentiert werden soll, sind die sinnvollsten nächsten Themen:

- vollständige Befehls- und Rollenmatrizen pro Job/Fraktion
- detaillierte Job-/Business-/Vehicle-Flowcharts
- komplette Datenbank-Schema-Tabellen mit Feldern und Zweck
- reale UI-Dialog- und Button-Map pro Menü
- Checkliste für „aktiv vs. vorbereitet vs. verdeckt“

Diese Dokumente sind bereits in den .md-Dateien vorbereitet und in die Projektstruktur integriert.


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
