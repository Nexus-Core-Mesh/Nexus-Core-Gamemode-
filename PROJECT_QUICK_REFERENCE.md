# Nexus Core – Schnellreferenz

Diese Datei ist die kurze, arbeitsfähige Referenz für die wichtigsten Bestandteile des Gamemodes. Sie dient als schnelle Orientierung ohne den vollen Detail-Text zu lesen.

## 1. Kernpunkte

- Hauptentrypoint: `gamemodes/gamemode.pwn`
- Hauptmodule: `gamemodes/modules/`
- Core-Quellwahrheit: `core/core.inc`
- State-System: `state/state_system.inc`
- DB-Interface: `db/db_interface.inc`
- UI-Hub: `ui/ui_hub.inc`
- Roleplay-HUD: `ui/roleplay_ui_system.inc`
- Text-Wars: `factions/text_wars.inc`

## 2. State-System

- Staaten: Los Santos, San Fierro, Las Venturas
- IDs: `0`, `1`, `2`
- Konstanten: `STATE_LOS_SANTOS`, `STATE_SAN_FIERRO`, `STATE_LAS_VENTURAS`
- Core-Validierung: immer `Core_IsValidState()` und `Core_NormalizePlayerState()`

## 3. Admin-System

- Ränge:
  - `1` Support
  - `2` Moderator
  - `3` State-Admin
  - `4` State-Leader
  - `5` Owner
- Relevante Funktionen:
  - `AdminSystem_IsModerator(playerid)`
  - `AdminSystem_IsStateAdmin(playerid)`
  - `AdminSystem_IsStateLeader(playerid)`
  - `AdminSystem_IsOwner(playerid)`
  - `AdminSystem_CanManageWorld(playerid)`

## 4. Befehle – wichtige aktive/zentralen Commands

### Core UI
- `/menu`
- `/inv`
- `/quick`
- `/jobs`
- `/garage`
- `/vehicle`
- `/hud`
- `/hudhide`

### Soziale und RP-Commands
- `/me`
- `/do`
- `/report`
- `/help`
- `/gang`
- `/war`
- `/fight`
- `/factions`

### Verwaltung und Admin
- `/admin`
- `/config`
- `/tpstate`
- `/setstate`
- `/mapmenu`
- `/eventtoggle`
- `/jail`
- `/release`

### Staats-/Wirtschafts-/UI-Commands
- `/treasury`
- `/stateinfo`
- `/market`
- `/tax`
- `/economy`
- `/shops`
- `/dispatch`
- `/wanted`
- `/crime`

## 5. TextDraw-Bereiche

### Aktive HUDs
- Login / Tutorial
- Roleplay Dashboard (`gRoleplayUIDraw`)
- Text-Wars (`gTWDraw`)
- Gangwar / Gangfight / Wanted / Crime
- Transport Routes / City Economy / Shop Overview
- Admin Config / Admin Mapping

### Gebaut, aber nicht angebunden
- Jail HUD
- Drug Market
- Dynamic Job List
- Mission Board

## 6. Module-Bereiche

### Core / Infrastruktur
- `core/`
- `db/`

### State / Government
- `state/`
- `government/`
- `management/`

### Economy / Business / Shops
- `economy/`
- `business/`
- `shops/`
- `commerce/`

### Jobs / Progression / Logistics
- `jobs/`
- `missions/`
- `progression/`
- `production/`
- `logistics/`

### Vehicles / Housing / Inventory
- `vehicles/`
- `housing/`
- `inventory/`

### Law / Emergency / Medical
- `law/`
- `crime/`
- `emergency/`
- `medical/`
- `justice/`

### Factions / World / Events
- `factions/`
- `world/`
- `events/`

### UI
- `ui/`
- `social/`
- `communication/`

## 7. Wichtigste Hinweise

- Der Code nutzt kein zcmd/YCMD, sondern eigene Befehlsabfragen in `OnPlayerCommandText()`.
- Der erste passende Handler gewinnt. Deshalb gibt es verdeckte/überlagerte Commands.
- Viele Module sind bereits vorhanden, aber einige sind nur ausgestattet, aber noch nicht über die aktive Laufzeit aufgerufen.
- Die wichtigste Quelle ist `gamemodes/`, nicht die alte Kopie unter `Compiler/samp03/gamemodes/`.

## 8. Kurzfazit

Das Projekt ist bereits ein weit ausgebautes RP-Gamemode-Projekt mit:

- klaren Modulen
- aktivem 3-State-System
- starkem UI-/TextDraw-Setup
- Wirtschafts- und Staatslogik
- Admin- und Security-Layer
- Faction-, Law- und Event-Systemen

Es ist eine solide, strukturierte Core-Architektur mit vielen bereits vordefinierten Systemen und einigen noch offenen Integrationspunkten.


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

