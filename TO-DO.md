# Nexus Core Gamemode – Erweiterungs-TO-DO

## Ziel

Das Projekt als modularen, erweiterbaren GTA:SA-Reallife-Gamemode mit 3-Staaten-Architektur, persistenten Spielerdaten und modernem State-/Economy-/Management-Stack weiterführen.

## Statusübersicht

### P0 – Kernsysteme und Stabilität
- [x] Projektstruktur auf modularen Layern aufbauen
- [x] Core-State-/Player-Modelle definieren
- [x] DB-Abstraktionsschicht mit Dini-first Fallback und optionaler SQLite/MySQL-Autosync etabliert
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
- [x] SA:MP/Pawn-Toolchain aufsetzen
- [x] compile-check + server-start test (lokaler `qemu-i386`-Build auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` erfolgreich)
- [ ] sozialer und wirtschaftlicher Load-Test mit mehreren Spielern
- [ ] Kassen-/State-Transaktionen gegen realen Spielbetrieb validieren

### Release-Milestone B1 – State Rehydration & Menu-First Onboarding
- [x] `DB_RehydratePlayerState()` in den Login-/Connect-Pfad eingebunden, ohne gültige State-/Home-Werte mit `0` zu überschreiben
- [x] `OnPlayerSpawn()` verhindert doppelte Tutorial-/State-Szenen bei gültigem Home-State oder State-ID
- [x] `CameraSystem_StartFreeCamera()`, `CameraSystem_StartDroneCamera()` und `CameraSystem_ToggleFreeCamera()` resetten sauber und stellen `SetCameraBehindPlayer()` + `TogglePlayerControllable(playerid, 1)` wieder her
- [x] State-/Home-Validierung durch `Core_IsValidState()` und `INSERT OR IGNORE`-Guardrails im DB-Pfad erhalten
- [x] Build mit der lokalen 32-Bit-Toolchain auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` bestätigt
- [x] Direkte Hur-bis-Gameplay-Hotfixe aus diesem Lauf ergänzt: `DB_RehydratePlayerState()` bleibt validierend, `OnPlayerSpawn()` erkennt vorhandenen `home_state`/`state_id` und springt nicht erneut in Tutorial-/State-Szene, `CameraSystem_ExitCameraMode()` kapselt Reset- und Control-Return sauber, `UiHub_ShowMain()` / `UiHub_ShowQuickActionMenu()` führen Camera-Reset/Exit über die Menüpfade aus.
- [x] Übergang `Connect -> Auto-Detect Login/Register -> Tutorial -> Heimatstaat (LS/SF/LV) -> StateSpawnSystem_Apply() -> CameraSystem_Reset() -> fester Spawnpunkt` als menu-first flow stabilisiert; die Zustände werden nicht bei Respawn/Reconnect erneut durch Chatbefehle ausgelöst.
- [x] Single-Target-Build-Pfad auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` konsolidiert; alte `gamemode.amx`-Duplikate und Legacy-Aliase werden im Build und im Serverordner bereinigt.

### Release-Milestone B2 – Admin / Projektleiter-System
- [x] Projektleiter-/Owner-Zugang als versteckter Spezialzugang in `core.inc` und `admin_system.inc` ergänzt; `Core_IsProjectLeader()` und `AdminSystem_IsProjectLeader()` sichern Zugriff ohne normale Spieler-Hilfen.
- [x] State-Setzen, Teleport und Spawnen über `StateSpawnSystem_Apply()`/`CameraSystem_Reset()` eingebaut; `Core_SetPlayerState()` und `Core_SetPlayerHomeState()` bleiben auf gültige State-IDs beschränkt.
- [x] Fahrzeug-Spawnen über `VehicleSystem_SpawnAdminVehicle()` integriert; Slot-/Limit-Checks verbleiben im Vehiclesystem und vermeiden direkte rohe Array-Schreibpfade.
- [x] Admin-/Projektleiter-Befehle `/projectlead`, `/setstate`, `/spawnvehicle` in `AdminSystem_HandleCommand()` ergänzt und auf gültige `strmid`/sscanf-Befehlsanalyse mit klaren Fehlermeldungen stabilisiert.
- [x] Aufbau und Dokumentation des Admin-/Projektleiter-Workflows im Code und Architektur-Report berücksichtigt; bestehende Abschnitte bleiben unverändert und die Erweiterung wird als neuer Release-Block ergänzt.
- [x] Zusätzliche Schleifen-/Leistungs-Guardrails eingebaut: `TextWars_Tick()` prüft vor jedem Refresh nun aktivierte Gangfights und sichtbare Player-Widgets und springt bei Inaktivität früh mit `return 1;` aus, um Long-callback-Execution unter der qemu-i386-Umgebung zu vermeiden.

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

---

## Offline-Tooling- und Audit-Status (ergänzt)

Zusätzlich zu den bestehenden Analyse-Helfern unter `tools/` wurden vier schlanke Python-Tooling-Module ergänzt, die ohne Serverstart, ohne Live-Test und ohne externe Abhängigkeiten arbeiten:

- `tools/db_and_state_inspector.py`: prüft SQLite-State- und Dini-Persistenzdaten schreibgeschützt und kann Vorher-/Nachher-Snapshots erzeugen.
- `tools/audit_modules_and_guards.py`: prüft Include-/Lifecycle-Reihenfolge, Guard-Muster, `Core_IsValidState()`, Camera-/HUD-Reset und Tick-/Loop-Guards.
- `tools/textdraw_and_zone_mapper.py`: listet TextDraws, PlayerTextDraws, Zonen- und HUD-Layout-Elemente in einer klaren Übersicht auf.
- `tools/projectlead_api_check.py`: gleicht Projektleiter-, Admin-, Spawn-, Fahrzeug-, Gangfight- und Zonen-API-Aufrufe über den Sourcetree ab.

Diese Werkzeuge sind bewusst auf 1,5 GB RAM / 2 CPU-Kerne und den Offline-Pfad unter `Compiler/samp03/` zugeschnitten; sie dienen ausschließlich der Quell- und Zustandsüberwachung, nicht als Live-Server-Test.

---

## Finaler Projektstand (ergänzt)

Der aktuelle Repository-Stand ist technisch auf den Single-Target-Contract und die Laufzeit-Architektur des Gamemodes ausgerichtet. Im Mittelpunkt steht weiterhin die generische, aber harte Erwartung: nur `Compiler/samp03/gamemodes/gtasa_reallife.amx` ist das gültige Runtime-Ziel, und der Build muss mit der lokalen 32-Bit-Pawn-Toolchain über `qemu-i386` erfolgen.

### Verifiziert im Source-Stand

- Single-Target-Build: `Compiler/samp03/gamemodes/gtasa_reallife.amx` bleibt die einzige gültige Ausgabe.
- Build-Kommando: `LD_LIBRARY_PATH="$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" qemu-i386 Compiler/pawncc/pawncc -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" gamemodes/gamemode.pwn -iincludes -iCompiler/samp03/include -d3`
- Include-/Lifecycle-Contract: `Core -> DB -> State -> Admin -> Auth -> Economy -> Jobs -> Laws -> Housing/Vehicle -> Leisure/Web` und `BOOT -> DB_READY -> STATE_READY -> AUTH_READY -> WORLD_READY -> RUNTIME_READY` bleiben unverändert an der zentralen Stelle in `gamemode.pwn` verankert.
- `core.inc` bleibt die alleinige Quelle für `state`, `home_state` und Projektleiter-/Admin-Rechte. Keine parallelen Schatten-State-Quellen wurden erneut eingeführt.
- Die vorhandenen Hardening-Guards bleiben erhalten: `INSERT OR IGNORE`, Null-/0-Schutz bei gültigen `state_id` und `home_state`, sichere `strmid`-Token-Extraktion statt unsicherer `sscanf`-Puffer, `INVALID_TEXT_DRAW` statt unsicherem `Text:0`-Vergleich, entzerrte Hotpaths in `TextWars_Tick` und `UBBVehicleSystem_Connect`.

### Zustandsstatus des Projekts

- UI- und Tutorial-Flow sind auf den menu-first-Workflow mit Auto-Login/Register-Detektion, Home-State-Auswahl LS/SF/LV und einmaligen Spawn-/Tutorial-Pfad fokussiert.
- Camera/Reset-Guards bleiben auf `SetCameraBehindPlayer()` + `TogglePlayerControllable(playerid, 1)` ausgelegt.
- Dini-first-Persistenz unter `Compiler/samp03/scriptfiles/` bleibt als primäre Sicherungsstrategie im Fokus; SQLite-Propagation bildet die konsistente Nebenebene für laufende Statusabgleiche.
- Die אפgepferten Zustände für Häuser, Inventar, Fahrzeuge, Safes und State-Transfers bleiben auf atomare Write-Temp/Validate/Rename-Muster ausgerichtet.

### Offener, aber klar definierter Status

- Kein Server-Start auf Port 7780 oder 7777 wurde durchgeführt, solange keine ausdrückliche Freigabe vorliegt.
- Live-Validierung bleibt ein separates Release-Gate, das erst nach dem echten Runtime-Test auf der Ubuntu-VM mit Logs, DB-Checks und Client-Flow erfolgen darf.
- Der aktuelle Stand ist als source-validiert, build-sicher und Architektur-konform anzusehen, aber nicht als Live-Server-freigegeben.

### Abschluss

Der Projekt-Tracker bleibt daher in einem klaren Zustand: technische Stabilität, single-target Build-Validität, Dini-/SQLite-Persistenzsicherung und Lifecycle-Konsistenz sind vollständig dokumentiert und im Code verankert. Die nächste Freigabe erfolgt nur durch echte Live-Validierung auf der Ubuntu-VM und nicht durch reine Compile-Sicherheit allein.

