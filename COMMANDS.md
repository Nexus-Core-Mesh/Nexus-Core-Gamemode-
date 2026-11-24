# Nexus Core – Command-Übersicht (Ist-Stand)

Quelle: `gamemodes/gamemode.pwn` und `gamemodes/modules/**` (Stand der Analyse).
Alle Commands laufen über `OnPlayerCommandText` (kein zcmd/Y_CMD). Es gibt keine `CMD:`-Makros.

## Dispatch-Reihenfolge in `OnPlayerCommandText`

Der erste Handler, der `1` zurückgibt, gewinnt. Spätere Handler mit gleichem Command sind **verdeckt**.

1. `RoleplayUI_HandleCommand`
2. `UiHub_HandleCommand`
3. `TextWars_HandleCommand`
4. `AdminSystem_HandleCommand`
5. `JailSystem_HandleCommand`
6. `EmergencySystem_HandleCommand`
7. `GarageSystem_HandleCommand`
8. `WeaponShopSystem_HandleCommand`
9. `StateEventSystem_HandleCommand`
10. `SocialSystem_HandleCommand`
11. `RoleplayCommandSystem_HandleCmd`
12. `AdminMappingSystem_HandleCmd`
13. Inline-Commands direkt in `gamemode.pwn` (`/tutorial` … `/shops`)

Hinweis zur Spalte „Status“: **aktiv** = erreichbar, **verdeckt** = Code vorhanden, aber ein früherer Handler fängt den Command ab, **nicht angebunden** = Handler existiert, wird aber nirgends aufgerufen.

Admin-Ränge (`core.inc`): 1 Support, 2 Moderator, 3 State-Admin, 4 State-Leader, 5 Owner.

---

## 1. Spieler-/UI-Commands

### HUD / Dashboard (`modules/ui/roleplay_ui_system.inc`)
| Command | Alias | Funktion | Status |
|---|---|---|---|
| `/hud` | `/dashboard` | Öffnet das TextDraw-Dashboard | aktiv |
| `/hudhide` | – | Blendet das Dashboard aus | aktiv |

### UI-Hub (`modules/ui/ui_hub.inc`, dialogbasiert)
| Command | Alias | Funktion | Status |
|---|---|---|---|
| `/menu` | `/m` | Hauptmenü | aktiv |
| `/spawn` | – | Spawn im aktuellen Staat | aktiv (verdeckt Inline-`/spawn`) |
| `/inv` | `/inventory` | Inventar | aktiv |
| `/handy` | `/phone` | Handy (Kontakte, Nachrichten) | aktiv |
| `/biz` | `/business` | Business-Liste | aktiv |
| `/ammu` | `/weapons` | Ammu-Nation-Menü | aktiv (verdeckt `/weapons` des Waffenshops) |
| `/missions` | – | Missionsliste | aktiv |
| `/garage` | – | Garagen-Menü | aktiv (verdeckt `/garage` des Garage-Moduls) |
| `/tuning` | – | Tuning-Menü | aktiv |
| `/jobs` | `/jobcenter` | Job-Menü | aktiv |
| `/vehicle` | `/veh` | Fahrzeug-Menü | aktiv |
| `/statemenu` | `/leitstand` | Quick-Action-Menü | aktiv |
| `/quick` | `/hub` | Quick-Action-Menü | aktiv |

### Text-Wars (`modules/factions/text_wars.inc`)
| Command | Alias | Funktion | Status |
|---|---|---|---|
| `/textwars` | `/warmenu` | Kriegs-Hauptmenü (Dialog) | aktiv |
| `/warhud` | `/gangwar`, `/gangfight` | Klickbares Kriegs-Widget (PlayerTextDraws) | aktiv (verdeckt `/gangwar`-Namensraum) |
| `/territories` | – | Gebietsübersicht | aktiv |

### Soziales / Roleplay
| Command | Parameter | Modul | Status |
|---|---|---|---|
| `/me` | `<Aktivität>` | `social_system.inc` | aktiv |
| `/do` | `<Beschreibung>` | `social_system.inc` | aktiv |
| `/report` | `<Grund>` | `social_system.inc` | aktiv |
| `/help` | – | `social_system.inc` (Liste der wichtigsten Commands) | aktiv |
| `/me`, `/do`, `/report` | – | `roleplay_command_system.inc` (Platzhalter-Texte) | **verdeckt** durch Social |
| `/pm` | – | `roleplay_command_system.inc` (nur Platzhalter-Meldung, keine echte PM-Logik) | aktiv |
| `/gang` | – | `roleplay_command_system.inc` (Hinweistext) | aktiv |
| `/factions` | `/factioninfo` | Zeigt erste aktive Fraktion des eigenen Staates | aktiv |
| `/war` | – | Gangwar-Status | aktiv |
| `/fight` | – | Gangfight-Status | aktiv |
| `/job` | – | Eigene Job-Infos | aktiv |

### Fahrzeuge / Garage (`modules/vehicles/garage_system.inc`)
| Command | Funktion | Status |
|---|---|---|
| `/garage` | Garagenmenü | **verdeckt** durch UI-Hub |
| `/spawngarage` | Fahrzeug aus Garage spawnen | aktiv |

### Waffenshop (`modules/shops/weapon_shop_system.inc`)
| Command | Parameter | Status |
|---|---|---|
| `/weapons` | – (Katalog des Staates) | **verdeckt** durch UI-Hub |
| `/buyweapon` | `<WeaponID> [StateID]` | aktiv |

### Notruf / Polizei (`modules/emergency/`)
| Command | Parameter | Funktion | Status |
|---|---|---|---|
| `/ems` | `<PlayerID>` | EMS-Aktion (nur EMS) | aktiv |
| `/police` | – | Polizeiaktion (nur Polizei) | aktiv |
| `/stations` | – | Wachen/Stationen anzeigen | aktiv |
| `/dispatch` | – | Dispatch-Queue | aktiv (Inline in `gamemode.pwn`) |
| `/acceptcall` | `<ID>` | Notruf annehmen | aktiv (`DispatchSystem_HandleCommand` im `OnPlayerCommandText()`-Pfad angebunden) |

### Events (`modules/events/state_event_system.inc`)
| Command | Parameter | Rang | Status |
|---|---|---|---|
| `/events` | – | alle | aktiv |
| `/eventtoggle` | `<StateID>` | Moderator | aktiv |

### Tutorial
| Command | Funktion |
|---|---|
| `/tutorial` | Startet das Tutorial (Inline) |

---

## 2. Inline-Commands in `gamemode.pwn`

| Command | Funktion | Zielfunktion | Status |
|---|---|---|---|
| `/tutorial` | Tutorial starten | `TutorialSystem_Start` | aktiv |
| `/city` | Zur Heimatstadt + Kamerafahrt | `StateSpawnSystem_Apply`, `CameraSystem_PlayStateScene` | aktiv |
| `/gov` | Regierungsinfo des Staates | `GovernmentSystem_ShowStateInfo` | aktiv |
| `/services` | Staatliche Services + Kommunales | `StateServices_Show`, `MunicipalSystem_ShowOverview` | aktiv |
| `/treasury` | Budget/Treasury | `StateBudgetSystem_ShowOverview` | aktiv |
| `/spawn` | Spawn | `StateSpawnSystem_Apply` | **verdeckt** durch UI-Hub |
| `/state` | Staats-Kurzinfo | Inline-Text | **verdeckt** durch Admin-`/state` (zeigt nur Liste der Staaten) |
| `/stateinfo` | Staats-Kurzinfo (Budget, Steuer, Leader, Services) | Inline-Text | aktiv |
| `/municipal` | Kommunale Übersicht | `MunicipalSystem_ShowOverview` | aktiv |
| `/districts` | Bezirke | `DistrictSystem_ShowDistricts` | aktiv |
| `/workers` | Transportarbeiter | `TransportWorkers_ShowWorkers` | aktiv |
| `/factories` | Fabriknetzwerk | `FactoryNetwork_ShowFactories` | aktiv |
| `/marketplace` | Marktplatz | `MarketplaceSystem_Show` | aktiv |
| `/governance` | Staatsverwaltung | `StateGovernance_ShowOverview` | aktiv |
| `/contraband` | Schwarzmarkt | `ContrabandSystem_Show` | aktiv |
| `/taxi` | Taxi-Netzwerk | `TaxiSystem_ShowNetwork` | aktiv |
| `/district_economy` | Bezirkswirtschaft | `DistrictEconomy_Show` | aktiv |
| `/tax` | Steuerübersicht | `StateTaxSystem_ShowSummary` | aktiv |
| `/market` | Staatsmarkt | `MarketSystem_ShowMarket` | aktiv |
| `/dispatch` | Dispatch-Queue | `DispatchSystem_ShowQueue` | aktiv |
| `/adminconfig` | Admin-Konfig-TextDraw (Moderator+) | `AdminConfigSystem_ShowMenu` | aktiv |
| `/stats` | Spielerstatistik/Level | `ProgressionSystem_ShowStats` | aktiv |
| `/wanted` | Wanted-TextDraw | `LawSystem_ShowWanted` | aktiv |
| `/crime` | Crime-Tracker-TextDraw | `CrimeSystem_ShowCrimeInfo` | aktiv |
| `/routes` | Transportrouten-TextDraw | `TransportSystem_ShowRoutes` | aktiv |
| `/economy` | Stadtwirtschafts-TextDraw | `CityEconomy_ShowOverview` | aktiv |
| `/shops` | Shop-Übersicht-TextDraw | `ShopSystem_ShowStateShops` | aktiv |

---

## 3. Admin-Commands (`modules/admin/`)

### `admin_system.inc`
| Command | Alias | Rang / Bedingung | Funktion |
|---|---|---|---|
| `/admin` | `/mod` | Moderator oder Projektleiter | Admin-Menü (Dialog) |
| `/projectlead` | `/projectleader` | Projektleiter-Recht | Projektleiter-Zugang aktivieren |
| `/state` | – | alle | Liste der verfügbaren Staaten |
| `/tpstate` | `/setstateplayer` | `CanManageWorld` | Spieler in Staat teleportieren/setzen |
| `/config` | – | Moderator/Projektleiter | Konfigurationsmenü |
| `/togglewar` | – | Moderator/Projektleiter | Gangwar umschalten |
| `/setstate` | – | `CanManageWorld` | Staat-Einstellungen setzen |
| `/setstatecoord` | – | `CanManageWorld` | Staatskoordinaten setzen |
| `/spawnvehicle` | – | `CanManageWorld` | Fahrzeug spawnen |
| `/setcityhall` | – | `CanManageWorld` | Rathaus-Position setzen |
| `/setdealership` | – | `CanManageWorld` | Autohaus setzen |
| `/sethouse` | – | `CanManageWorld` | Haus setzen |
| `/setbusiness` | – | `CanManageWorld` | Business setzen |

### `admin_mapping_system.inc`
| Command | Parameter | Funktion |
|---|---|---|
| `/mapmenu` | – | Mapping-Menü/TextDraw |
| `/mapadd` | `<Name> <StateID>` | Position speichern |
| `/maptp` | `<ID>` | Zu Position teleportieren |
| `/maplist` | – | Positionen auflisten |
| `/mapzones` | – | Zonenliste |
| `/mapdel` | `<ID>` | Position löschen |

### Weitere Admin-Commands
| Command | Parameter | Rang | Modul |
|---|---|---|---|
| `/adminconfig` | – | Moderator | Inline |
| `/jail` | `<PlayerID> <Minuten> <StateID>` | Moderator | `law/jail_system.inc` |
| `/release` | `<PlayerID>` | Moderator | `law/jail_system.inc` |
| `/eventtoggle` | `<StateID>` | Moderator | `events/state_event_system.inc` |

---

## 4. Bekannte Auffälligkeiten

- `/acceptcall` ist nun über den zentralen `OnPlayerCommandText()`-Pfad erreichbar und als aktiv dokumentiert.
- Doppelte/verdeckte Commands: `/me`, `/do`, `/report` (RoleplayCommand), `/spawn`, `/garage`, `/weapons`, `/state` (Inline), `/gangwar`.
- `/pm` und `/gang` sind nur Platzhalter ohne Logik.
- Module ohne eigenen Command (nur indirekt über Menüs/Dialoge): Premium, Season, Inventar-Aktionen, Business, Haus, Jobcenter, State-Control (Leitstand-Dialoge).
- `Compiler/samp03/gamemodes/` ist eine **ältere Spiegelkopie** mit abweichendem Stand (es fehlen `events/season_system.inc` und `premium/`, viele weitere Dateien weichen ab); die obige Liste basiert auf `gamemodes/`.


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

