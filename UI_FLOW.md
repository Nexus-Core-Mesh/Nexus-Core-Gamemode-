# UI-Flow und Dialog-Architektur

Dieses Dokument beschreibt den realen UI-Flow des Gamemodes: Login, Tutorial, State-Selection, UI-Hub, Dialoge und TextDraw-HUD.

## 1. UI-Philosophie

Das Projekt verwendet eine gemischte UI-Architektur:

- TextDraw-basierte Anzeigen für Status/HUDs
- Dialoge für Menüs und Interaktion
- PlayerTextDraws für komplexere Widgets wie Text-Wars

Das ist deutlich in `gamemode.pwn`, `ui/ui_hub.inc`, `ui/roleplay_ui_system.inc`, `ui/state_control_ui.inc` und `ui/ui_dialog_ids.inc` sichtbar.

## 2. zentrale Dialog-ID-Verwaltung

`ui/ui_dialog_ids.inc` enthält die zentrale ID-Vergebung. Das ist wichtig, weil die Dialoge nicht wahllos erzeugt werden, sondern an feste Bereiche gebunden sind.

Beispiele für Dialog-Vorlagen:

- `DIALOG_UI_MAIN`
- `DIALOG_UI_STATE_MENU`
- `DIALOG_UI_AUTH_MAIN`
- `DIALOG_UI_AUTH_PASSWORD`
- `DIALOG_UI_STATE_PICK`
- `DIALOG_UI_VEHICLE_MENU`
- `DIALOG_UI_JOB_MENU`
- `DIALOG_UI_INV_LIST`
- `DIALOG_UI_PHONE_MAIN`
- `DIALOG_UI_BIZ_LIST`

## 3. UI-Hub als zentrales UI-Frontend

`ui/ui_hub.inc` ist das größte UI-Modul und steuert die wichtigsten interaktiven Menüs.

### Typische UI-Flows

- Login-/Register-Flow
- State-Auswahl-Flow
- Inventar-Flow
- Handy-/Telefon-Flow
- Business-Flow
- Fahrzeug-Flow
- Job-Flow
- Quick-Action-Menu

Die UI-Hub-Funktionen umgehen dabei nicht direkt die Modul-Logik, sondern rufen konkrete Module auf, z. B.:

- `UiHub_ShowInventory(playerid)`
- `UiHub_ShowPhone(playerid)`
- `UiHub_ShowBusinessList(playerid)`
- `UiHub_ShowGarages(playerid)`
- `UiHub_ShowJobMenu(playerid)`
- `UiHub_ShowVehicleMenu(playerid)`
- `UiHub_ShowQuickActionMenu(playerid)`

## 4. Command-Flow

Die Commands laufen über `OnPlayerCommandText()` in `gamemode.pwn`.

Beispiel:

- `/menu` -> `UiHub_ShowMain(playerid)`
- `/inv` -> `UiHub_ShowInventory(playerid)`
- `/phone` -> `UiHub_ShowPhone(playerid)`
- `/garage` -> `UiHub_ShowGarages(playerid)`
- `/jobs` -> `UiHub_ShowJobMenu(playerid)`
- `/hud` -> `RoleplayUI_ShowDashboard(playerid)`

Damit ist das UI-System klar mit den Chat-Kommandos verbunden.

## 5. Player-Login- und Tutorial-Flow

### `LoginSystem_Initialize()`

Das Login-System ist ein UI-/TextDraw-Flow.

Es erzeugt TextDraws für:

- Login-Titel
- Header
- Text
- Hinweis
- Info

Bei Registrierung entstehen weitere Draws:

- Titel
- Registrierungsblock
- Heimat-/Stadt-Auswahl
- Ablauf
- Tutorial-Hinweis

### `TutorialSystem_ShowStage(playerid, stage)`

Das Tutorial ist stufenweise und nutzt TextDraws:

- Welcome / Intro
- Heimat-/Regierungssektion
- Abschluss

Wenn der Spieler einen Zustand erreicht, wird `TutorialSystem_ShowStage()` aufgerufen und die korrespondierenden Draws zu diesem Schritt aktiviert.

## 6. Dashboard-UI: Roleplay HUD

`ui/roleplay_ui_system.inc` erzeugt ein HUD-Dashboard mit mehreren TextDraws.

Es ist in den Bereichen angeordnet:

- links oben: Spieler-/State-Block
- mitte: Event-/Status-Block
- rechts: Menü-/Context-Block

Die wichtigsten Funktionen:

- `RoleplayUI_Initialize()`
- `RoleplayUI_ShowDashboard(playerid)`
- `RoleplayUI_Hide(playerid)`
- `RoleplayUI_ExtractCommandToken()`

### Commands

- `/hud`
- `/dashboard`
- `/hudhide`

## 7. State-Control-UI

`ui/state_control_ui.inc` ergänzt das UI mit einem Leitstand-Interface für das 3-Staaten-System.

Das Modul enthält:

- `gStateControlTarget[MAX_PLAYERS]`
- `gStateControlTransferDir[MAX_PLAYERS]`
- `StateControl_Initialize()`
- `StateControl_CanManage()`

Es ist ein Interface für staatliche Management- und Transfer-Mechaniken, und es zeigt, dass das Projekt ein administratives UI-System für das Land- und State-Management besitzt.

## 8. Text-Wars UI

`factions/text_wars.inc` ist ein sehr interessantes UI-Widget.

Es benutzt `PlayerTextDraws` mit klickbaren Buttons:

- Status
- Gebiete
- Angriff
- Truppen
- Schließen

Das Widget ist beides:

- Informationstafel
- interaktive Oberfläche

Es nutzt `TextWars_Show()`, `TextWars_Refresh()`, `TextWars_Select()`, `TextWars_Hide()` und Mouse-Callbacks.

## 9. Dialoge statt TextDraws

Viele Menüs sind Dialog-First und nicht Textdraw-First, zum Beispiel:

- Admin-Menü
- Vehikelmenü
- Job-Menü
- Inventar-Liste
- Handy-Menü
- Business-Menü
- State-Pick/Dialog-Selection

`UiDialog_Show()` ist die gemeinsame Oberfläche für diese Interaktionen.

## 10. UI-Event-Flow in der Praxis

Ein typischer Spieler-Flow sieht so aus:

1. Spieler verbindet sich
2. Login- oder Register-TextDraws werden gezeigt
3. State-Auswahl ist aktiv
4. Tutorial startet je nach Status
5. `StateSpawnSystem_Apply()` setzt den Spawn
6. `RoleplayUI_ShowDashboard()` zeigt das Live-HUD
7. Befehle oder UI-Hub öffnen Menüs und Dialoge
8. Module liefern Daten und interaktive UI-Responses

## 11. Was das Projekt zeigt

Das UI-Design ist bewusst gemischt:

- TextDraws für schnelle Live-Statusanzeigen
- Dialoge für komplexe Auswahl-/Listenmenüs
- PlayerTextDraws für Widget-Interaktion

Das ist ein korrektes, wenn auch etwas heterogenes, SA:MP-Design für ein großes RP-Projekt.

## 12. Fazit

Das UI-System ist klar als Orchestrator aufgebaut. Die wichtigsten Interfaces sind:

- `ui_hub.inc` für das Hauptmenü
- `roleplay_ui_system.inc` für das Dashboard
- `state_control_ui.inc` für staatliche Verwaltung
- `text_wars.inc` für Kriegs-/Gebiets-Widget
- `ui_dialog_ids.inc` für zentrale Dialog-ID-Reservierung

Diese Schicht ist der eigentliche „Spielerschnittpunkt“ zwischen Logik und Benutzung.


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
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut.
- SQLite-only + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

