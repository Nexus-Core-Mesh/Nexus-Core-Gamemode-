# State-System und 3-Staaten-Architektur

Das projektseitige Staatssystem ist eines der Kernstücke des Gamemodes. Die echte Grundlage liegt in `modules/state/state_system.inc` und in `modules/core/core.inc`.

## 1. Drei Staaten

Die relevanten State-Konstanten sind:

- `STATE_LOS_SANTOS = 0`
- `STATE_SAN_FIERRO = 1`
- `STATE_LAS_VENTURAS = 2`

Es gibt eine definierte State-Größe:

- `STATE_COUNT = 3`

Die Namen sind:

- `Los Santos`
- `San Fierro`
- `Las Venturas`

## 2. Zentrale Funktionen

### `StateSystem_IsValidState(stateid)`

Prüft, ob die ID im gültigen Bereich liegt:

- `stateid >= 0 && stateid < STATE_COUNT`

### `StateSystem_NormalizeStateId(stateid)`

Falls ein State-Wert ungültig ist, wird automatisch auf Los Santos zurückgefallen.

### `StateSystem_GetStateShortName(stateid, output[], len)`

Wandelt State-ID in Kurzform um:

- `0 -> LS`
- `1 -> SF`
- `2 -> LV`

### `StateSystem_IsEnabled(stateid)`

Prüft, ob der Staat aktiv ist.

## 3. Initialisierung

`StateSystem_Initialize()` setzt die wichtigsten Werte:

- Staatliche Kassen
- Steuersätze
- Tax-Revenue
- Infrastruktur-Revenue
- State-Policy-Strings
- State-Leader-Namen
- Enabled-State-Flags
- Spawnpositionen
- Register in die DB

Beispiel aus dem Code:

- LS-Kasse: 500000
- SF-Kasse: 450000
- LV-Kasse: 400000

Steuersätze:

- LS: 8%
- SF: 7%
- LV: 9%

## 4. State-Datenmodell

Die Staatssysteme speichern intern:

- `gStateBankCash[STATE_COUNT]`
- `gStateBank[STATE_COUNT]`
- `gGovernmentBudget[STATE_COUNT]`
- `gStateTaxRate[STATE_COUNT]`
- `gStateTaxRevenue[STATE_COUNT]`
- `gStateInfrastructureRevenue[STATE_COUNT]`
- `gStatePolicy[STATE_COUNT][24]`
- `gStateLeaderName[STATE_COUNT][24]`
- `gStateSpawnX/Y/Z[STATE_COUNT]`
- `gStateEnabled[STATE_COUNT]`

Das zeigt, dass Staatlichkeit nicht nur ein Name ist, sondern eine politische, finanzielle und geografische Einheit.

## 5. State-Spawn-System

`state_spawn_system.inc` ergänzt das System mit Staatsspezifischen Spawnpositionen für alle drei Staaten.

Das heißt:

- Jeder Spieler hat einen aktiven Staat
- Home-State und State sind getrennt modellierbar
- Spawn-Logik kann je nach zustandspaarem Staat entschieden werden

## 6. State- und Player-Übergänge

### `StateSystem_OnPlayerConnect(playerid)`

Beim Connect:

- Prüft, ob gültiger State und Home-State existieren
- lädt den gespeicherten State via DB
- setzt den Core-Status neu
- initialisiert Cash und State

### `StateSystem_OnPlayerDisconnect(playerid, reason)`

Beim Disconnect:

- persistert der aktuelle State wieder in die DB
- stellt State-Werte sicher
- speichert Cash und State

### `StateSystem_OnPlayerSpawn(playerid)`

Aktionslogik beim Spawn:

- liest aktuelle State-ID
- ermittelt den Namen des Staates
- sendet ein ClientMessage mit dem Staat

## 7. Beziehung zu `core.inc`

`core/core.inc` definiert die zentrale Regel:

- gültiger State muss im Bereich 0–2 liegen
- ungültige Werte werden normalisiert
- `Core_NormalizePlayerState()` setzt auf `STATE_LOS_SANTOS` zurück, falls nötig

Das ist intentional, damit das Projekt keine „verlorenen“ invalide Stats hat.

## 8. Staatliche Verwaltung

Die Verwaltungs- und Government-Module greifen auf den State-Layer zu:

- `government/government_system.inc`
- `government/state_governance.inc`
- `government/city_hall_system.inc`
- `management/municipal_system.inc`
- `management/airport_system.inc`
- `state_services.inc`
- `district_system.inc`
- `district_economy.inc`

Diese Module interpretieren den State nicht nur als ID, sondern als politische und wirtschaftliche Einheit.

## 9. Economy und Treasury-Mechanik

Die wichtigsten State-ökonomischen Daten sind:

- Staatskasse / Treasury
- Steuerquote
- Steuergewinn
- Infrastruktur-Revenue
- Budget

Diese Daten werden in `state_budget_system.inc` und `state_tax_system.inc` verwendet und sind mit den staatlichen Finanzen verbunden.

## 10. Leader-System

Der State-Code enthält ebenfalls:

- `gStateLeaderName[STATE_COUNT][24]`

Dadurch kann das Projekt jedem State einen Leader-String zuordnen, z. B.:

- `Mayor LS`
- `Mayor SF`
- `Mayor LV`

## 11. Staats-Services

`state_services.inc` bildet die Basis für staatliche Dienste. Die Komponenten können je Staat unterschiedliche Infrastruktur- und Service-Features modellieren, was das 3-Staaten-Konzept vollständig macht.

## 12. Typische Use-Cases

### Spieler wechselt in einen Staat

1. `Core_GetPlayerState()` prüft aktuellen State
2. `StateSystem_IsValidState()` prüft die Gültigkeit
3. `StateSystem_NormalizeStateId()` korrigiert ungültige Werte
4. die DB merkt den neuen State
5. die Spawn- und UI-Systeme übernehmen die Sicht

### Staatsbudget wird geändert

1. `StateSystem_Initialize()` initialisiert Bank/Tax/Revenue
2. `StateBudgetSystem_ShowOverview()` zeigt den Status
3. `MarketSystem_*` und `TownEconomy_*` greifen den State-Status auf

## 13. Wichtiges Designmerkmal

Das Projekt verwendet einen sehr klaren Landestruktur-Mechanismus:

- 3 Staaten
- 1 State-ID pro Spieler
- 1 Home-State pro Spieler
- 1 zentrale `Core_*`-Validierung

Damit bleibt die Welt stabil und die Wirtschaft kann auf klare geographische Zuständigkeiten aufbauen.

## 14. Fazit

Das State-System ist das politische und wirtschaftliche Rückgrat des Gamemodes. Es verbindet:

- Verwaltung
- Steuerlogik
- Staatliche Kasse
- Spawnmanagement
- Leader- und Servicesysteme
- wirtschaftlichen Zustand pro Staat

Ohne dieses System würde die ganze Welt-Simulationslogik des Modus zusammenbrechen.


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

