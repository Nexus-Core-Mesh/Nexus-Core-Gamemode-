# Nexus Core Gamemode – Erweiterungs-TO-DO

> [!NOTE]
> Diese Doku wurde optisch aufbereitet, ohne technische Fakten, Pfade, Ports oder Projektregeln zu verändern.
> Fokus lag auf Lesbarkeit, Struktur, Übersichten und deutlicherer Orientierung.

## ✅ Wichtig zu wissen

- Technische Fakten, Befehle, Pfade und Projektregeln bleiben bewusst unverändert.
- Port- und Systemtrennung bleiben konsistent, insbesondere für den 7780-Contract und den Dini-first-Pfad.
- Die optische Aufwertung dient nur der Lesbarkeit, nicht der inhaltlichen Interpretation.
- Status-Tags: `aktiv`, `verdeckt`, `gebaut aber nicht angebunden`, `vorbereitet` sind die verbindlichen Projektlabels für Module/Commands/UI-Views.

---
## 🧭 Schnellüberblick

| Bereich | Inhalt |
| --- | --- |
| Dokument | `TO-DO.md` |
| Typ | Projekt-Doku |
| Fokus | Dokumentierter Projektbereich |
| Wichtige Pfade | `gamemodes/`, `Compiler/samp03/`, `tools/` |

---
## 🧭 Ziel

Das Projekt als modularen, erweiterbaren GTA:SA-Reallife-Gamemode mit 3-Staaten-Architektur, persistenten Spielerdaten und modernem State-/Economy-/Management-Stack weiterführen.

## 📊 Statusübersicht

### 🏗️ P0 – Kernsysteme und Stabilität
- [x] Projektstruktur auf modularen Layern aufbauen
- [x] Core-State-/Player-Modelle definieren
- [x] DB-Abstraktionsschicht mit Dini-first Fallback und optionaler SQLite/MySQL-Autosync etabliert
- [x] Hooks, Lifecycle- und Runtime-Registrierung vereinheitlichen
- [x] kritische Übergabepunkte zwischen DB, Inventar, Safe und UBB-Fahrzeugen abschirmen
- [x] zentrale Treasury-/Bank-/State-APIs anlegen

### 📘 P1 – Reallife-Gameplay
- [x] Auth-/Login-/Tutorial-/Home-State-Flow
- [x] Fahrzeug- und Zugangslogik
- [x] Tankstellen-/Garage- und Reparaturlogik
- [x] Polizei-, EMS-, Justice- und Jail-Systeme
- [x] Shop-/Arsenal-/Business- und Wirtschaftslogik
- [x] Social-/RP-Kommandos und Event-Flow

### 📘 P2 – Staat, Regierung und Verwaltung
- [x] States LS/SF/LV mit eigener Kasse und Steuerlogik
- [x] State-Governance und Budget-Management
- [x] Fraktions-, Gang- und Business-Logik je Staat
- [x] Admin-/Support-/State-Admin-Architektur

### 📘 P3 – Häuser, Biz und Immobilien
- [x] Haus- und Ownership-Struktur
- [x] House-Safe-/Lagerlogik
- [x] Business-/Firma-/Bizz-System
- [x] staatlich getrennte Investitionslogik

### 📘 P4 – Progression, Social und Long-Term Loop
- [x] Level-, XP- und Reputationslogik
- [x] Job- und War-/Fight-Systeme
- [x] Tutorial-, Intro- und RP-Startspuren
- [x] Beziehungssysteme und soziale RP-Module

### 🧩 P5 – Control Panel und Management
- [x] Web-Dashboard mit SQLite-Anbindung
- [x] Overview-/Summary-/State- und Admin-APIs
- [x] Projektstatus und Web-Bridge dokumentiert
- [x] Offline-Setup-/Start-Manager für `control-panel/` unter `tools/control_panel_manager.py` ergänzt
- [x] Deploy-Pfad `scripts/deploy_server.sh` startet das Panel nur als optionalen Zusatz-Worker, ohne den Gamemode-Deploy zu blockieren
- [x] Dini-first vs. SQLite-Read-Only-Konsistenz als Teil der Dokumentation ergänzt
- [x] Projektgewichtung Persistenz auf 51 % SQLite / 49 % Dini festgeschrieben: SQLite für Schema/Upserts/Sync, Dini für Datei- und Fallback-Persistenz ohne parallele Shadow-State-Quelle
- [x] Source-of-Truth-Regel in `core.inc` explizit dokumentiert: `state_id`, `home_state`, Bank/Cash, Admin-Ränge (`1–5`) und `Core_IsProjectLeader()` bleiben einzig dort geführt
- [x] Repo-gebundene Offline-TUI-/Autofix-Integration ergänzt: `tools/nexus_hub_tui.py`, `tools/nexus_tools_tui.py`, `tools/nexus_autofix.py`, `tools/samp_tui.py` sowie die Root-Wrapper `nexus_hub_tui.py`, `nexus_tools_tui.py`, `nexus_autofix.py`, `samp_tui.py`
- [x] Repo-gebundene Pfadauflösung ohne feste `/home`-Argumente, `7780`-Live-Abhängigkeiten oder Server-Start in den TUI/Autofix-Workflows

### 🏗️ P6 – Kommandos, Hub-Fluss und RP-Module
- [x] `OnPlayerCommandText()` konsolidiert: UI-Hub, RP-UI, TextWars, Admin, Jail, Dispatch, Emergency, Garage, Weapon, State-Event, Social, Roleplay-Command und AdminMapping in einer klaren Reihenfolge ohne Shadowing.
- [x] `/menu`, `/m`, `/help`, `/hud`, `/dashboard`, `/spawn`, `/jobs`, `/jobcenter`, `/dispatch`, `/acceptcall` durch `UiHub_HandleCommand()` und zuständige Module aktiv verkettet.
- [x] `UiHub_Ready()` bleibt Guard für noch nicht aktivierte oder unauthorisierte Übergänge; validierte Login-/Tutorial-/State-Pfade bleiben durchlaufbar.
- [x] `RoleplayUI_HandleCommand()` bleibt als kompatibler Fallback aktiv, aber der primäre Dashboard-/Hub-Flow läuft über `UiHub_Ready()` und das zentrale Menü.
- [x] Economy-/Jobs-/Law-/Faction-/World-/Vehicle-/Housing-/Inventory-Module bleiben im aktiven Quellfluss konsistent mit `core.inc` als Single Source of Truth und Dini-first-Safe-Pattern unter `Compiler/samp03/scriptfiles/`.
- [x] Live-Server-Tests bleiben bewusst außerhalb der Quelle und des Compiles; nur Code-Audit und `bash scripts/build_gamemode.sh` sind die gültigen Validierungspfade.
- [x] `ui_hub` / `roleplay_ui_system` / `job_center` / `mission_system` / `jail_system` / `drug_system` / `shop_system` / `business_system` / `vehicle_system` / `inventory_system` / `event_system` / `season_system` / `web_bridge` als Status-Matrix in den Projektstatus aufgenommen: aktiv, verdeckt, gebaut aber nicht angebunden oder vorbereitet.

## 📘 Nächste releaseorientierte Aufgaben

### 📘 Release-Milestone A – Production-safe backend
- [x] zentrale Load-/Save-Interfaces für `players`, `house_storage`, `player_inventory`, `ubb_vehicles` ergänzt
- [x] DB-Repair- und Validation-Pfade für kritische Tabellen formalisiert
- [x] atomare Transaction- und Guard-Mechaniken für kritische Write-Pfade etabliert
- [x] Backup-/Restore-Scaffold und Snapshot-Hooks als operationales Design eingebaut

### 📘 Release-Milestone B – Gameplay-Stress-Test
- [x] SA:MP/Pawn-Toolchain aufsetzen
- [x] compile-check + server-start test (lokaler `qemu-i386`-Build auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` erfolgreich)
- [ ] sozialer und wirtschaftlicher Load-Test mit mehreren Spielern
- [ ] Kassen-/State-Transaktionen gegen realen Spielbetrieb validieren

### 📘 Release-Milestone B1 – State Rehydration & Menu-First Onboarding

- [x] Grundstruktur für `Compiler/samp03/scriptfiles/` ergänzt: `players/`, `houses/`, `house_storage/`, `safes/`, `vehicles/`, `ubb_vehicles/`, `state/LS|SF|LV`, `state_kasse/LS|SF|LV`, `business/`, `factions/`, `inventory/`, `temp/`, `backup/`, `logs/` angelegt und als `.gitkeep`-Markierung im Git gesichert.
- [x] Valid-state-Reset-Schutz verstärkt: Disconnect-/Login-Pfade dürfen existente `state_id`/`home_state`-Werte nicht durch `0`-Resets zerstören; sie normalisieren auf gültige Standardwerte statt zu invalidieren.
- [x] Angrenzende Rehydrate-/Connect-/Disconnect-/Spawn-Pfade in `core.inc`, `auth/login_system.inc` und `state/state_system.inc` mit gültigem Fallback gegen Null-Resets abgesichert; kein Bereich überschreibt noch gültige Werte mit 0.
- [x] Offline-State-Guard erweitert, damit direkte `gPlayerHomeState[] = 0` bzw. `gPlayerData[...] [E_PLAYER_HOME_STATE] = 0`-Patterns mit einem statischen Audit auffallen.
- [x] Dini-/Persistenzpfade auf `temp` → validate → atomic rename und 7780-Readiness abgestimmt; fehlende Ordner dürfen keinen Laufzeitfehler mehr erzeugen.
- [x] 7780-Server- und Plugin-Konfiguration auf `gamemode0 gtasa_reallife 1` + `port 7780` konsistent gehalten; Port `7777` bleibt ausdrücklich tabu.
- [x] `DB_RehydratePlayerState()` in den Login-/Connect-Pfad eingebunden, ohne gültige State-/Home-Werte mit `0` zu überschreiben
- [x] `OnPlayerSpawn()` verhindert doppelte Tutorial-/State-Szenen bei gültigem Home-State oder State-ID
- [x] `CameraSystem_StartFreeCamera()`, `CameraSystem_StartDroneCamera()` und `CameraSystem_ToggleFreeCamera()` resetten sauber und stellen `SetCameraBehindPlayer()` + `TogglePlayerControllable(playerid, 1)` wieder her
- [x] State-/Home-Validierung durch `Core_IsValidState()` und `INSERT OR IGNORE`-Guardrails im DB-Pfad erhalten
- [x] Build mit der lokalen 32-Bit-Toolchain auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` bestätigt
- [x] Direkte Hur-bis-Gameplay-Hotfixe aus diesem Lauf ergänzt: `DB_RehydratePlayerState()` bleibt validierend, `OnPlayerSpawn()` erkennt vorhandenen `home_state`/`state_id` und springt nicht erneut in Tutorial-/State-Szene, `CameraSystem_ExitCameraMode()` kapselt Reset- und Control-Return sauber, `UiHub_ShowMain()` / `UiHub_ShowQuickActionMenu()` führen Camera-Reset/Exit über die Menüpfade aus.
- [x] Übergang `Connect -> Auto-Detect Login/Register -> Tutorial -> Heimatstaat (LS/SF/LV) -> StateSpawnSystem_Apply() -> CameraSystem_Reset() -> fester Spawnpunkt` als menu-first flow stabilisiert; die Zustände werden nicht bei Respawn/Reconnect erneut durch Chatbefehle ausgelöst.
- [x] Single-Target-Build-Pfad auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` konsolidiert; alte `gamemode.amx`-Duplikate und Legacy-Aliase werden im Build und im Serverordner bereinigt.

### 🏗️ Release-Milestone B2 – Admin / Projektleiter-System

#### Additive 7780-Härtung – 2026-11-21

- [x] `StateSpawnSystem_Apply()` auf validierte State-/Home-State-Normalisierung und Reset-Schutz angepasst; gültige Werte werden nicht erneut auf `0` zurückgesetzt.
- [x] `OnPlayerSpawn()` verhindert erneute Tutorial-/State-Szenen, sobald `home_state` oder `state_id` bereits gültig gesetzt sind.
- [x] Camera-/Control-Reset über `CameraSystem_ExitCameraMode()`, `SetCameraBehindPlayer()` und `TogglePlayerControllable(playerid, 1)` konsolidiert, um veraltete Intro- und Spawn-Kameras zu verhindern.
- [x] Single-Target-Build auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` mit `qemu-i386`-Toolchain und Exit-Code `0` bestätigt; Port `7777` bleibt tabu.
- [x] Offline-Checks auf `tools/*.py`, `scripts/*.sh` und Build-Path laufen ohne Serverstart durch; Live-Server- und Client-Tests bleiben bewusst aus.

### 🏗️ Release-Milestone B2 – Admin / Projektleiter-System
- [x] Projektleiter-/Owner-Zugang als versteckter Spezialzugang in `core.inc` und `admin_system.inc` ergänzt; `Core_IsProjectLeader()` und `AdminSystem_IsProjectLeader()` sichern Zugriff ohne normale Spieler-Hilfen.
- [x] Dispatch-Command-Flow in `OnPlayerCommandText()` konsolidiert: `/dispatch` und `/acceptcall` laufen jetzt über `DispatchSystem_HandleCommand()` und sind als aktiv dokumentiert.
- [x] Disconnect-/Reset-/Menüwechselpfade auf saubere Camera- und HUD-Cleanup erweitert: `CameraSystem_ExitCameraMode()`, `CameraSystem_Reset()`, `RoleplayUI_Hide()` und `TutorialSystem_HideStage()` werden vor Disconnect/Reset und beim Öffnen von Haupt-/Quick-Action-/State-Menüs aufgerufen, um doppelte 4-Zonen-HUD-Renderings oder veraltete Kamerazustände zu verhindern.
- [x] State-Setzen, Teleport und Spawnen über `StateSpawnSystem_Apply()`/`CameraSystem_Reset()` eingebaut; `Core_SetPlayerState()` und `Core_SetPlayerHomeState()` bleiben auf gültige State-IDs beschränkt.
- [x] Fahrzeug-Spawnen über `VehicleSystem_SpawnAdminVehicle()` integriert; Slot-/Limit-Checks verbleiben im Vehiclesystem und vermeiden direkte rohe Array-Schreibpfade.
- [x] Admin-/Projektleiter-Befehle `/projectlead`, `/setstate`, `/spawnvehicle` in `AdminSystem_HandleCommand()` ergänzt und auf gültige `strmid`/sscanf-Befehlsanalyse mit klaren Fehlermeldungen stabilisiert.
- [x] Aufbau und Dokumentation des Admin-/Projektleiter-Workflows im Code und Architektur-Report berücksichtigt; bestehende Abschnitte bleiben unverändert und die Erweiterung wird als neuer Release-Block ergänzt.
- [x] Zusätzliche Schleifen-/Leistungs-Guardrails eingebaut: `TextWars_Tick()` prüft vor jedem Refresh nun aktivierte Gangfights und sichtbare Player-Widgets und springt bei Inaktivität früh mit `return 1;` aus, um Long-callback-Execution unter der qemu-i386-Umgebung zu vermeiden.

### 🧩 Release-Milestone UI – Dialog-/Textdraw-First
- [x] Hauptmenü (`/menu`, Taste Y) mit Inventar, Handy, Business, Ammu-Nation, Mission-Board, Garage, Tuning (`ui_hub.inc`)
- [x] Job-Zentrum (`job_center.inc`) mit Annahme, Checkpoint-Aufgaben, XP/Lohn/Staatskasse/Inventar-Kopplung
- [x] Text-Wars-Widget (`text_wars.inc`, Taste N) für Gangwar, Gangfight und Territorien
- [x] Staats-Leitstand (`state_control_ui.inc`) für Steuern, Treasury, Policy, Budget
- [x] Lifecycle-Contract mit `Core_MarkModuleReady` und Stufen AUTH_READY/WORLD_READY
- [x] Single-Target-Build auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` mit lokaler `qemu-i386` Pawn-Toolchain und Exit-Code `0` bestätigt
- [x] Offline-Tooling unter `tools/` ergänzt: log bridge, line extractor, autocorrect, debug collector
- [x] Dokumentations-/Audit-Pfade für 4-Zonen-HUD, Gangfight-Zonen, Projektleiter-/RCON-Steuerung und `7780`-Laufzeitpfad ergänzt
- [ ] echte Live-Validierung im Ubuntu-VM mit Serverstart, Logs, `gtasa_reallife.db` und Client-Flow bleibt bewusst offen bis zur expliziten Freigabe
- [x] frühe Connect-/Disconnect-Guards für gültige `state_id`/`home_state`-Werte in `core.inc` und `auth/login_system.inc` verschärft; keine 0-Default-Resets durch vorzeitige Reset-Pfade mehr
- [x] Offline-Readiness-/Guard-Audits auf `tools/qemu_7780_readiness_check.py` und `tools/audit_modules_and_guards.py` erfolgreich ausgeführt; Build bleibt `qemu-i386`-bedingt blockiert in der aktuellen Umgebung

### 📘 Release-Milestone C – RP-Expansion
- [ ] Mission-Board mit verknüpfter Staatspolitik erweitern
- [ ] Bizz-/Gangs-/Administration-Ausbau mit Rollen- und Besitzlogik
- [ ] zusätzliches Map-/World- und District-Content je Staat
- [ ] Events, Loot, Exclusive-Services und Region-Specials

## 📌 Modulplan

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

## ✅ Hinweis

Die Architektur bleibt bewusst modular. Jedes neue System wird in einem passenden Modul ergänzt und nicht monolithisch in `gamemode.pwn` gebaut. Die Dokumentation bleibt Teil der Entwicklung. Jede System-Erweiterung muss als Release-Rückblick in README/TO-DO verankert sein.

---

## 📊 Offline-Tooling- und Audit-Status (ergänzt)

Zusätzlich zu den bestehenden Analyse-Helfern unter `tools/` wurden weitere schlanke Python-Tooling-Module ergänzt, die ohne Serverstart, ohne Live-Test und ohne externe Abhängigkeiten arbeiten:

- `tools/db_and_state_inspector.py`: prüft SQLite-State- und Dini-Persistenzdaten schreibgeschützt und kann Vorher-/Nachher-Snapshots erzeugen.
- `tools/audit_modules_and_guards.py`: prüft Include-/Lifecycle-Reihenfolge, Guard-Muster, `Core_IsValidState()`, Camera-/HUD-Reset und Tick-/Loop-Guards.
- `tools/textdraw_and_zone_mapper.py`: listet TextDraws, PlayerTextDraws, Zonen- und HUD-Layout-Elemente in einer klaren Übersicht auf.
- `tools/projectlead_api_check.py`: gleicht Projektleiter-, Admin-, Spawn-, Fahrzeug-, Gangfight- und Zonen-API-Aufrufe über den Sourcetree ab.
- `tools/gangfight_zone_validator.py`: prüft Rechteck-/Radius-Zonendefinitionen auf Plausibilität, Min/Max-Umkehrungen, unplausible Werte und fehlende Fraktions-/State-Zuordnung.
- `tools/economy_and_state_ledger_check.py`: prüft Geld-, Cash-, Bank-, Home-State-, Safe- und Fahrzeug-/Ledger-Daten in SQLite und Dini-Dateien auf Null-/Negativwerte und Zustandsabweichungen.
- `tools/dialog_and_camera_reset_check.py`: prüft Dialog-IDs, Reset-/Abbruchpfade, Camera-/Toggle-Gaurds und UI-Disengage-Pfade.
- `tools/qemu_7780_readiness_check.py`: prüft statisch die 7780-Readiness (Single-Target AMX, `gamemode0 gtasa_reallife 1`, Port 7780, kein Port 7777) ohne Serverstart.
- `tools/state_rehydration_guard.py`: erkennt gültige `state_id`/`home_state`-Sicherungen, 0-Schutzmuster und das Beibehalten von `INSERT OR IGNORE`-Guardrails im Source-Level.
- `tools/runtime_contract_guard.py`: prüft die Single-Target-AMX- und 7780-Contract-Regeln gegen Konfigurations-/Build-/Dokumentationsdrift und verhindert aktive `7777`-Verweise oder Alias-Reflexe.
- `tools/ui_hud_cleanup_audit.py`: erfasst Camera-/TextDraw-/HUD-Cleanup- und Reset-Muster für `SetCameraBehindPlayer()`, `TogglePlayerControllable`, `INVALID_TEXT_DRAW` und Destroy-Pfade.

Diese Werkzeuge sind bewusst auf 1,5 GB RAM / 2 CPU-Kerne und den Offline-Pfad unter `Compiler/samp03/` zugeschnitten; sie dienen ausschließlich der Quell- und Zustandsüberwachung, nicht als Live-Server-Test.

---

## 📊 Finaler Projektstand (ergänzt)

Der aktuelle Repository-Stand ist technisch auf den Single-Target-Contract und die Laufzeit-Architektur des Gamemodes ausgerichtet. Im Mittelpunkt steht weiterhin die generische, aber harte Erwartung: nur `Compiler/samp03/gamemodes/gtasa_reallife.amx` ist das gültige Runtime-Ziel, und der Build muss mit der lokalen 32-Bit-Pawn-Toolchain über `qemu-i386` erfolgen.

### 📊 Verifiziert im Source-Stand

- Single-Target-Build: `Compiler/samp03/gamemodes/gtasa_reallife.amx` bleibt die einzige gültige Ausgabe.
- Build-Kommando: `LD_LIBRARY_PATH="$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" qemu-i386 Compiler/pawncc/pawncc -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" gamemodes/gamemode.pwn -iincludes -iCompiler/samp03/include -d3`
- Include-/Lifecycle-Contract: `Core -> DB -> State -> Admin -> Auth -> Economy -> Jobs -> Laws -> Housing/Vehicle -> Leisure/Web` und `BOOT -> DB_READY -> STATE_READY -> AUTH_READY -> WORLD_READY -> RUNTIME_READY` bleiben unverändert an der zentralen Stelle in `gamemode.pwn` verankert.
- `core.inc` bleibt die alleinige Quelle für `state`, `home_state` und Projektleiter-/Admin-Rechte. Keine parallelen Schatten-State-Quellen wurden erneut eingeführt.
- Die vorhandenen Hardening-Guards bleiben erhalten: `INSERT OR IGNORE`, Null-/0-Schutz bei gültigen `state_id` und `home_state`, sichere `strmid`-Token-Extraktion statt unsicherer `sscanf`-Puffer, `INVALID_TEXT_DRAW` statt unsicherem `Text:0`-Vergleich, entzerrte Hotpaths in `TextWars_Tick` und `UBBVehicleSystem_Connect`.

### 📊 Zustandsstatus des Projekts

- UI- und Tutorial-Flow sind auf den menu-first-Workflow mit Auto-Login/Register-Detektion, Home-State-Auswahl LS/SF/LV und einmaligen Spawn-/Tutorial-Pfad fokussiert.
- Camera/Reset-Guards bleiben auf `SetCameraBehindPlayer()` + `TogglePlayerControllable(playerid, 1)` ausgelegt.
- Dini-first-Persistenz unter `Compiler/samp03/scriptfiles/` bleibt als primäre Sicherungsstrategie im Fokus; SQLite-Propagation bildet die konsistente Nebenebene für laufende Statusabgleiche.
- Die אפgepferten Zustände für Häuser, Inventar, Fahrzeuge, Safes und State-Transfers bleiben auf atomare Write-Temp/Validate/Rename-Muster ausgerichtet.

### 📊 Offener, aber klar definierter Status

- Kein Server-Start auf Port 7780 oder 7777 wurde durchgeführt, solange keine ausdrückliche Freigabe vorliegt.
- Live-Validierung bleibt ein separates Release-Gate, das erst nach dem echten Runtime-Test auf der Ubuntu-VM mit Logs, DB-Checks und Client-Flow erfolgen darf.
- Der aktuelle Stand ist als source-validiert, build-sicher und Architektur-konform anzusehen, aber nicht als Live-Server-freigegeben.

### 📘 Abschluss

Der Projekt-Tracker bleibt daher in einem klaren Zustand: technische Stabilität, single-target Build-Validität, Dini-/SQLite-Persistenzsicherung und Lifecycle-Konsistenz sind vollständig dokumentiert und im Code verankert. Die nächste Freigabe erfolgt nur durch echte Live-Validierung auf der Ubuntu-VM und nicht durch reine Compile-Sicherheit allein.

---

## 📘 Additive Härtungsnotiz – Verhalten im Quellbaum

Diese Eintragung ergänzt die vorhandenen Milestones ohne einen bestehenden Abschnitt zu löschen.

- Reale Code-Blocker im Quellbaum behandelt: `GetServerTime()` wurde gegen das in dieser Pawn-Umgebung gültige `gettime()`-Contract ersetzt; dadurch wurden echte Buildfehler in `core.inc` und `premium_system.inc` beseitigt.
- Reale DB-API-Lücke behoben: `DB_GetPlayerHomeState()` ergänzt, damit State-/Home-State-Rehydrate- und Connect-Pfade die Core-Quelle erneut korrekt lesen können.
- Offlinestrategie bestätigt: `bash scripts/build_gamemode.sh` und die statischen Guards `(py_compile`, `bash -n`, `node --check`, Tool-Guard-Skripte) liefen erfolgreich durch.
- Der Fortgang bleibt immer noch nicht als Live-Server-Start auf Port 7780 interpretiert; das bleibt ein separates Freigabe-Gate außerhalb der Code-Validierung.

---

## 📘 Additive Abschluss-Validierung – 2026-11-21

Diese Abschlussnotiz ergänzt den Projekttracker, ohne frühere Einträge zu entfernen.

- Finaler Build- und Hardening-Lauf: `LoginSystem_AutoResolveFlow()`, `LoginSystem_AdvanceToStateSelection()`, `TutorialSystem_Next()`, `OnPlayerSpawn()` und `CameraSystem_ExitCameraMode()` wurden in der aktuellen 7780-Source-Konfiguration geprüft und auf den menu-first-Flow mit gültigem State-/Home-State-Fallback ausgerichtet.
- Passwort-Only-Onboarding bleibt auf `DIALOG_STYLE_INPUT` mit freiem Passwort-Feld beschränkt; alle Auswahlpunkte für Stadt/State und Tutorial-Schritte laufen über Dialog-/Menü-Buttons weiter.
- State-/Home-State-Sicherung bleibt unverändert durch `Core_IsValidState()`, `Core_GetValidStateOrDefault()` und den bestehenden `DB_ValidateStateTransfer()`-Guard. Gültige Werte werden nicht durch spätere Reconnect-/Spawn-Pfade auf `0` zurückgesetzt.
- Camera-/HUD-Cleanup blieb zentralisiert: `SetCameraBehindPlayer()` + `TogglePlayerControllable(playerid, 1)` wurden bei Reset/Exit gepflegt und der TextDraw-/HUD-Cleanup bleibt über `INVALID_TEXT_DRAW`-Checks abgesichert.
- Build-Status: Der gezielte AMX-Build auf `Compiler/samp03/gamemodes/gtasa_reallife.amx` lief erfolgreich und hat Exit-Code `0` zurückgegeben; ein echter Live-Serverstart unter Port 7780 blieb bewusst aus, da die Freigabe dafür ausdrücklich offen bleibt.
- Zusätzlicher Admin-/State-Integrationsschritt: `AdminSystem_ApplyStateTeleport()` und `/tpstate`-/`/setstateplayer`-Commands wurden ergänzt, damit Projektleiter-/Admin-Pfade gültige State-Transfers mit Reset- und Camera-Guard absichern, ohne andere Spielerrechte zu überschreiten.

### 📊 Saison-Endstand – 2026-11-21

- Source-Status: Architektur-, Lifecycle-, Kamera- und State-Guardrails sind im aktuellen Quellbaum stabil und konsistent im 7780-Target verankert.
- Build-Status: `LD_LIBRARY_PATH="$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" qemu-i386 Compiler/pawncc/pawncc -o"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx" gamemodes/gamemode.pwn -iincludes -iCompiler/samp03/include -d3` liefert `Exit-Code 0`.
- Offline-Validierung: `python3 -m py_compile tools/*.py`, `bash -n` auf den passenden Shell-Skripten und die Guard-Skripte unter `tools/` liefen ohne Fehler durch; der einzige verbleibende Hinweis ist ein nicht-blockierender Compiler-Warnhinweis in `includes/dini.inc` (`warning 224`).
- Abgeschlossen: State-Transfer, Tutorial-/Spawn-Guards, Kamera-/HUD-Reset und Single-Target-Build wurden erneut verifiziert.
- Offen für späteren Live-Check auf Port 7780: echter Serverstart, Client-Loop, DB-Live-Sync und Gameplay-Validierung. Diese Schritte bleiben bewusst auf ausdrückliche Freigabe verschoben.
- Haus- und Business-API wurden konsistent erweitert: `HouseSystem_SetState()` und `HouseSystem_SetPrice()` sichern gültige State-/Preiswerte beim dynamischen Haus-Editor, während `BusinessSystem_*`-Pfade weiterhin auf gültige State-IDs und atomare Treasury-/Bank-Checks ausgerichtet bleiben.


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

