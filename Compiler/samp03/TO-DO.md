# Nexus Core Gamemode – Erweiterungs-TO-DO

## Ziel

Das Projekt als modularen, erweiterbaren GTA:SA-Reallife-Gamemode mit 3-Staaten-Architektur, persistenten Spielerdaten und modernem State-/Economy-/Management-Stack weiterführen.

## Statusübersicht

### P0 – Kernsysteme und Stabilität
- [x] Projektstruktur auf modularen Layern aufbauen
- [x] Core-State-/Player-Modelle definieren
- [x] DB-Abstraktionsschicht und SQLite-First-Design etablieren
- [x] Hooks, Lifecycle- und Runtime-Registrierung vereinheitlichen
- [x] kritische Übergabepunkte zwischen DB, Inventar, Safe und UBB-Fahrzeugen abschirmen
- [x] zentrale Treasury-/Bank-/State-APIs anlegen

### P1 – Reallife-Gameplay
- [x] Auth-/Login-/Tutorial-/Home-State-Flow
- [x] Fahrzeug- und Zugangslogik
- [x] Tankstellen-/Garage- und Reparaturlogik
- [x] Polizei-, EMS-, Justice- und Jail-Systeme
- [x] Shop-/Arsenal-/Business- und Wirtschaftslogik
- [x] Social-/RP-Kommandos und Event-Flow

### P2 – Staat, Regierung und Verwaltung
- [x] States LS/SF/LV mit eigener Kasse und Steuerlogik
- [x] State-Governance und Budget-Management
- [x] Fraktions-, Gang- und Business-Logik je Staat
- [x] Admin-/Support-/State-Admin-Architektur

### P3 – Häuser, Biz und Immobilien
- [x] Haus- und Ownership-Struktur
- [x] House-Safe-/Lagerlogik
- [x] Business-/Firma-/Bizz-System
- [x] staatlich getrennte Investitionslogik

### P4 – Progression, Social und Long-Term Loop
- [x] Level-, XP- und Reputationslogik
- [x] Job- und War-/Fight-Systeme
- [x] Tutorial-, Intro- und RP-Startspuren
- [x] Beziehungssysteme und soziale RP-Module

### P5 – Control Panel und Management
- [x] Web-Dashboard mit SQLite-Anbindung
- [x] Overview-/Summary-/State- und Admin-APIs
- [x] Projektstatus und Web-Bridge dokumentiert

## Nächste releaseorientierte Aufgaben

### Release-Milestone A – Production-safe backend
- [x] zentrale Load-/Save-Interfaces für `players`, `house_storage`, `player_inventory`, `ubb_vehicles` ergänzt
- [x] DB-Repair- und Validation-Pfade für kritische Tabellen formalisiert
- [x] atomare Transaction- und Guard-Mechaniken für kritische Write-Pfade etabliert
- [x] Backup-/Restore-Scaffold und Snapshot-Hooks als operationales Design eingebaut

### Release-Milestone B – Gameplay-Stress-Test
- [ ] SA:MP/Pawn-Toolchain aufsetzen
- [ ] compile-check + server-start test
- [ ] sozialer und wirtschaftlicher Load-Test mit mehreren Spielern
- [ ] Kassen-/State-Transaktionen gegen realen Spielbetrieb validieren

### Release-Milestone UI – Dialog-/Textdraw-First
- [x] Hauptmenü (`/menu`, Taste Y) mit Inventar, Handy, Business, Ammu-Nation, Mission-Board, Garage, Tuning (`ui_hub.inc`)
- [x] Job-Zentrum (`job_center.inc`) mit Annahme, Checkpoint-Aufgaben, XP/Lohn/Staatskasse/Inventar-Kopplung
- [x] Text-Wars-Widget (`text_wars.inc`, Taste N) für Gangwar, Gangfight und Territorien
- [x] Staats-Leitstand (`state_control_ui.inc`) für Steuern, Treasury, Policy, Budget
- [x] Lifecycle-Contract mit `Core_MarkModuleReady` und Stufen AUTH_READY/WORLD_READY
- [ ] Compile-Check: Repo nutzt Pseudo-Pawn (`void`/`bool`/`int` Rückgabetypen) und ist mit pawncc noch nicht kompilierbar

### Release-Milestone C – RP-Expansion
- [ ] Mission-Board mit verknüpfter Staatspolitik erweitern
- [ ] Bizz-/Gangs-/Administration-Ausbau mit Rollen- und Besitzlogik
- [ ] zusätzliches Map-/World- und District-Content je Staat
- [ ] Events, Loot, Exclusive-Services und Region-Specials

## Modulplan

- [x] `core/`
- [x] `db/`
- [x] `state/`
- [x] `admin/`
- [x] `auth/`
- [x] `economy/`
- [x] `jobs/`
- [x] `gangs/`
- [x] `housing/`
- [x] `business/`
- [x] `law/`
- [x] `crime/`
- [x] `shops/`
- [x] `drugs/`
- [x] `missions/`
- [x] `transport/`
- [x] `vehicles/`
- [x] `control-panel/`

## Hinweis

Die Architektur bleibt bewusst modular. Jedes neue System wird in einem passenden Modul ergänzt und nicht monolithisch in `gamemode.pwn` gebaut. Die Dokumentation bleibt Teil der Entwicklung. Jede System-Erweiterung muss als Release-Rückblick in README/TO-DO verankert sein.
