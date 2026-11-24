# Fraktions- und War-Systeme

Dieses Dokument beschreibt die Fraktions-, Gangwar-, Gangfight- und Territorien-Architektur des Gamemodes.

## 1. Überblick

Die relevanten Module sind:

- `factions/faction_system.inc`
- `factions/gangwar_system.inc`
- `factions/gang_fight_system.inc`
- `factions/text_wars.inc`
- `world/territory_system.inc`
- `world/district_economy.inc`
- `social/roleplay_command_system.inc`

Sie bilden die Grundlage für:

- Fraktionen/Gangs
- Kämpfe und War-Status
- Gebietskontrolle
- Text-Wars-Widget
- Live-UI-Interaktion

## 2. Factions-System

`faction_system.inc` ist die Basis für Fraktionslogik und Gangsysteme.

Das System erlaubt:

- Fraktionszugehörigkeit
- Fraktionsinfo
- Gruppen- und Gang-Übersichten
- State-gebundene Fraktionenkontexte

## 3. Gangwar

`gangwar_system.inc` verwaltet den Gangwar-Prozess.

Es enthält ein statisches HUD mit TextDraws:

- `gGangWarTD[playerid][0]` → "GANGWAR STATUS"
- `gGangWarTD[playerid][1]` → War-Text
- `gGangWarTD[playerid][2]` → Faction-Text

Das zeigt, dass Gangwar nicht nur Logik, sondern auch eine sichtbare Informationsebene hat.

### Commands

- `/war`
- `/gangwar`
- `/warhud`

Die Befehle werden durch Roleplay-Command-System und TextWars-Handler aufgerufen.

## 4. Gangfight

`gang_fight_system.inc` verwaltet die kleinere, kurzfristigere Auseinandersetzung zwischen Fraktionen.

Es nutzt ein eigenes TextDraw-HUD:

- `gGangFightTD[playerid][0]` → "GANGFIGHT HUD"
- `gGangFightTD[playerid][1]` → Fight-Text
- `gGangFightTD[playerid][2]` → Score-Text

Die Befehle sind ebenfalls in `roleplay_command_system.inc` und `text_wars.inc` gelagert.

## 5. Text Wars

`text_wars.inc` ist das prominenteste Fraktions-/War-Frontend.

Es bietet:

- klickbare `PlayerTextDraw`-Buttons
- Live-Statusanzeige
- Dialoge für Kriegs- und Gebietsaktionen
- zugeschnittene Info und Auswahlmenüs

### Commands

- `/textwars`
- `/warmenu`
- `/warhud`
- `/gangwar`
- `/gangfight`
- `/territories`

### Interaktion

Das Widget nutzt:

- `TextWars_Show(playerid)`
- `TextWars_Refresh(playerid)`
- `TextWars_HandleButton(playerid, d)`
- `TextWars_OnClickPlayerTextDraw()`
- `TextWars_OnClickTextDraw()`

Die Buttons sind:

- STATUS
- GEBIETE
- ANGRIFF
- TRUPPEN
- SCHLIESSEN

## 6. Territorien und Einflusszonen

`world/territory_system.inc` ist die Grundlage für Gebiets- und Einflusszonen-Logik.

Es liefert:

- n-te Territoriums-ID
- Gebiet im Staat
- Verwaltung von Einflusszonen

Zusammen mit `district_system.inc` und `district_economy.inc` bildet es die Topografie der politischen Kontrolle ab.

## 7. Roleplay-Commands

`social/roleplay_command_system.inc` enthält mehrere Fraktions- und War-Commands:

- `/gang`
- `/factions`
- `/factioninfo`
- `/war`
- `/fight`

Sie liefern Hinweise und Status auf einen Blick, ohne immer komplette UI-Logik zu starten.

## 8. Verknüpfung zu State und Wirtschaft

Fraktionssysteme sind nicht rein sozial. Sie sind verknüpft mit:

- State- und Distrikt-Struktur
- Wirtschaft (Gebietseinnahmen)
- Admin-/World-Management
- Police/Law-System (bei Konflikten)

Das heißt: Fraktionen sind nicht nur reine Gruppen, sondern Teil der Spielwelt-Logik.

## 9. Realer Ablauf

Ein typischer Weg in der Spielwelt:

1. Spieler gehört einer Fraktion an
2. Fraktionsstatus wird über `FactionSystem_*` verwaltet
3. War-/Fight-System zeigt den aktuellen Konfliktstatus
4. `TextWars` zeigt die Live-Liste im UI
5. Territorien und Districts beeinflussen den War-Status
6. Lösungs- oder Kontrollmechaniken greifen in die State-Logik ein

## 10. Fazit

Die Fraktions- und War-Architektur ist eines der zentralen RP-Kernelemente des Projekts. Es verbindet:

- Politische Einheiten
- territoriale Kontrolle
- Kampfmechanik
- UI-Darstellung
- Spielerstatus und Faction-Identity

Das System ist gut modularisiert und nutzt klare Module für die verschiedenen Ebenen: Faction, War, Text-Wars, Gebiete und UI.


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

