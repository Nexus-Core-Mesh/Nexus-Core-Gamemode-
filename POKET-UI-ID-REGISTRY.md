# Nexus Core UI-ID-Registry

## Ziel
Zentrale, dokumentierte Übersicht aller aktiv genutzten Dialog-/UI-ID-Bereiche und ihrer Modulzuordnung. Damit lassen sich UI-Fluss, Dialog-Response-Routing und TextDraw-/HUD-Slots nachvollziehbar verwalten, ohne doppelte ID-Belegungen oder unklare Injektionspunkte.

## Dialog-ID-Range-Übersicht

### HUB / Allgemeine UI (9000..9099)
- 9000 = DIALOG_UI_MAIN — Hauptmenü / Hub-Einstieg
- 9010 = DIALOG_UI_INV_LIST — Inventar-Liste
- 9011 = DIALOG_UI_INV_ACTION — Inventar-Aktion
- 9012 = DIALOG_UI_INV_GIVE_TARGET — Inventar-Transfer: Zielspieler
- 9013 = DIALOG_UI_INV_GIVE_QTY — Inventar-Transfer: Menge
- 9020 = DIALOG_UI_PHONE_MAIN — Handy-Hauptmenü
- 9021 = DIALOG_UI_PHONE_ADD — Kontakt hinzufügen
- 9022 = DIALOG_UI_PHONE_SMS — SMS / Nachricht verfassen
- 9030 = DIALOG_UI_BIZ_LIST — Business-Liste
- 9031 = DIALOG_UI_BIZ_ACTION — Business-Aktion
- 9040 = DIALOG_UI_AMMU_LIST — Ammu-Nation-Liste
- 9041 = DIALOG_UI_AMMU_CONFIRM — Kauf-/Ausrüstungs-Bestätigung
- 9050 = DIALOG_UI_MISSION_LIST — Mission-Board / Missionen
- 9051 = DIALOG_UI_MISSION_CONFIRM — Mission-Annahme-Bestätigung
- 9060 = DIALOG_UI_GARAGE_LIST — Garage-Liste
- 9061 = DIALOG_UI_GARAGE_ACTION — Garage-Aktion
- 9070 = DIALOG_UI_TUNING_LIST — Tuning-Liste
- 9071 = DIALOG_UI_TUNING_CONFIRM — Tuning-Bestätigung
- 9072 = DIALOG_UI_STATE_PICK — State-/Stadt-Auswahl
- 9073 = DIALOG_UI_VEHICLE_STATUS — Fahrzeug-Status
- 9074 = DIALOG_UI_VEHICLE_MENU — Fahrzeug-Menü
- 9075 = DIALOG_UI_JOB_MENU — Job-Menü
- 9076 = DIALOG_UI_STATE_MENU — Schnellzugriff / Staatsinfo
- 9077 = DIALOG_UI_STATS_MENU — Spieler-Stats / Dashboard
- 9078 = DIALOG_UI_ADMIN_MENU — Admin-Menü
- 9079 = DIALOG_UI_AUTH_MAIN — Login-/Register-Menü
- 9080 = DIALOG_UI_AUTH_PASSWORD — Passwort-Eingabe
- 9081 = DIALOG_UI_AUTH_STATE — Login-/State-Auswahl

### Jobs (9100..9119)
- 9100 = DIALOG_JOB_MAIN — Job-Hauptmenü / Job-Zentrum
- 9101 = DIALOG_JOB_LIST — verfügbare Jobs
- 9102 = DIALOG_JOB_CONFIRM — Job-Annahme-Bestätigung
- 9103 = DIALOG_JOB_STATUS — Job-Fortschritt / Status
- 9104 = DIALOG_JOB_QUIT — Job-Kündigung / Bestätigung

### State / Regierung (9120..9159)
- 9120 = DIALOG_STATE_MAIN — Staats-Leitstand / Behördenmenü
- 9121 = DIALOG_STATE_PICK — Staat wählen
- 9122 = DIALOG_STATE_ACTION — Staatsübersicht / Kasse / Leistungen
- 9123 = DIALOG_STATE_TAX_INPUT — Steuersatz-Input
- 9124 = DIALOG_STATE_TRANSFER_INPUT — Treasury-Transfer: Betrag
- 9125 = DIALOG_STATE_TRANSFER_DIR — Treasury-Transfer: Richtung
- 9126 = DIALOG_STATE_POLICY_INPUT — Politik-/Policy-Input
- 9127 = DIALOG_STATE_WAGE_INPUT — Job-Lohnfaktor-Input
- 9128 = DIALOG_STATE_BUDGET_INPUT — Budget-Genehmigung / Staatskassen-Budget

### War / Fraktionen / Territorien (9160..9199)
- 9160 = DIALOG_WAR_MAIN — Text-Wars Hauptmenü
- 9161 = DIALOG_WAR_TERRITORY_LIST — Territorienliste
- 9162 = DIALOG_WAR_TERRITORY_ACTION — Gebiet/Beanspruchung
- 9163 = DIALOG_WAR_INFO — War-Status / Info
- 9164 = DIALOG_WAR_FACTION_PICK — Faktion wählen / Angriffsübernahme

## Modulzuordnungen

### Aktiv angebunden
- UI-Hub / Main-Dialog: ui_hub.inc
- State-Leitstand: state_control_ui.inc
- Job-Zentrum: job_center.inc
- Mission-Board: mission_system.inc
- Text-Wars: text_wars.inc
- Inventar-System: inventory_system.inc
- Fahrzeug-System: vehicle_system.inc / garage_system.inc / tuning_system.inc
- Auth-Flow: login_system.inc
- Tutorial-Flow: tutorial_system.inc
- HUD/Dashboard: roleplay_ui_system.inc

### Bereits im Code dokumentiert/gesichert
- `INVALID_TEXT_DRAW`-Checks bei TextDraw-Hide/Destroy
- sichere Dialog- und Übergangsstrukturen über `UiDialog_Show()`
- optionale Eingaben bleiben optional, kein Chat-Befehlszwang

## TextDraw-/HUD-Gruppen
- login_system.inc / register-flow / tutorial
- roleplay_ui_system.inc / HUD dashboard
- mission_system.inc / mission board
- law_system.inc / wanted status
- jail_system.inc / jail HUD
- crime_system.inc / crime tracker
- gangwar_system.inc / gang war HUD
- shop_system.inc / shop overview
- transport_system.inc / route HUD
- city_economy_system.inc / city economy
- dynamic_job_system.inc / job list
- admin_config_system.inc / admin map + config

## Regeln / Leitplanken
- Keine UI-ID doppelt belegen.
- Neue Dialoge nur in freiem, reserviertem Bereich ergänzen.
- TextDraw-/PlayerTextDraw-Handles nach `INVALID_TEXT_DRAW` prüfen, bevor versteckt/destroyed wird.
- UI-Hauptpfad primär über Dialoge, klickbare TextDraws und `J`-Aktionen in Reichweite.
- Player-ID / Player-Name / Betrag nur als optionale Eingabe, nie als Pflichtweg.

## Build-Status
- Projektkompilierung erfolgreich mit: `bash scripts/build_gamemode.sh`
- Exit-Code: 0
- Warnung: `sqlite.inc` warning 224 (nicht blockierend)

## Verwendete Dateien im Projekt
- gamemodes/gamemode.pwn
- gamemodes/modules/ui/ui_dialog_ids.inc
- gamemodes/modules/ui/ui_id_registry.inc
- gamemodes/modules/ui/ui_hub.inc
- gamemodes/modules/ui/state_control_ui.inc
- gamemodes/modules/ui/roleplay_ui_system.inc
- gamemodes/modules/jobs/job_center.inc
- gamemodes/modules/factions/text_wars.inc
- gamemodes/modules/missions/mission_system.inc
- gamemodes/modules/inventory/inventory_system.inc


---

## Aktualisierter Stand (2026-11-24)

- Registrierungsflow: Passwort-Gate vor Tutorial/Spawn, M/W-Auswahl vor Skin-Picker, `DIALOG_UI_GENDER_PICK` aktiv.
- Tutorial: `TUTORIAL_SCENE_TIME` auf 11500 ms, Schutz gegen Start ohne Passwort/Geschlecht.
- Skin-Flow: `LoginSystem_ShowSkinPicker()` zeigt geschlechtsabhängige 10–15 Skins, validiert `0..311`, setzt `SetPlayerSkin` vor finalem Spawn.
- Spawn-Order: Home-State/State validiert, `StateSpawnSystem_Apply(...)` erst nach Skin-Setzung, dann `CameraSystem_ExitCameraMode`, `SetCameraBehindPlayer`, `TogglePlayerControllable(playerid, true)`, `RoleplayUI_ShowDashboard(playerid)`.
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut.
- SQLite-only + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

