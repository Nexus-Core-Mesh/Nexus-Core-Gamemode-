# Nexus Core – Modulübersicht (Ist-Stand)

Quelle: `gamemodes/gamemode.pwn` (Includes ab Zeile 19, Initialisierung in `OnGameModeInit`) und `gamemodes/modules/**`.
Insgesamt **91 `.inc`-Module** in 40 Kategorien-Ordnern (DB-Backends `db_sqlite`/`db_mysql` werden über `db_interface` eingebunden bzw. sind optional). Staaten: Los Santos, San Fierro, Las Venturas.
Legende Bedienung: **Cmd** = Chat-Command, **TD** = TextDraw, **Dlg** = Dialog, **–** = nur interne Logik/Hooks.
Details zu Commands: `COMMANDS.md`, zu TextDraws: `TEXTDRAWS.md`.

## 1. Core / Infrastruktur (`core/`, `db/`)
| Modul | Datei | Aufgabe |
|---|---|---|
| Core | `core/core.inc` | Version, Lifecycle-Stages, Admin-Ränge (1–5), State-Konstanten, Modul-Ready-Flags, Helper (`Core_*`) |
| Production Guard | `core/production_guard.inc` | Begrenzungen (Inventar-Slots, Haus-Storage, UBB-Fahrzeuge) und Sicherheits-Checks |
| Runtime Registry | `core/runtime_registry.inc` | Laufzeit-Registry der Systeme/Zustände |
| Module Contract | `core/module_contract.inc` | Lifecycle-Verträge (`minStage`, max. 48 Module) |
| DB Interface | `db/db_interface.inc` | Persistenz-API, **Dini-First** (Caches + Dini-Fallback), größte Datei (~1000 Zeilen) |
| DB SQLite | `db/db_sqlite.inc` | SQLite-Backend (vorhanden; Autostart läuft über Dini) |
| DB MySQL | `db/db_mysql.inc` | MySQL-4.1-Adapter, **bereit aber nicht aktiv** |

## 2. Auth / Onboarding / Kamera
| Modul | Datei | Aufgabe | Bedienung |
|---|---|---|---|
| Login | `auth/login_system.inc` | Login/Registrierung, Heimatstaat-Wahl | TD + Dlg |
| Tutorial | `tutorial/tutorial_system.inc` | Mehrstufiges Einführungs-Tutorial | TD, Cmd `/tutorial` |
| Kamera | `camera/camera_system.inc` | Szenen-/Kamerafahrten je Staat | – |

## 3. Staat / Regierung / Verwaltung
| Modul | Datei | Aufgabe |
|---|---|---|
| State System | `state/state_system.inc` | Drei Staaten, Kasse, Steuersatz, Leader (zentrale Quelle) |
| State Identity | `state/state_identity_system.inc` | Identität/Wirtschaftsprofil pro Staat |
| State Services | `state/state_services.inc` | Staatliche Service-Infrastruktur |
| State Spawn | `state/state_spawn_system.inc` | Spawns für LS/SF/LV |
| Districts | `state/district_system.inc` | Bezirke/Stadtviertel |
| Government | `government/government_system.inc` | Drei Staatsregierungen |
| City Hall | `government/city_hall_system.inc` | Rathäuser |
| State Governance | `government/state_governance.inc` | Verwaltung je Bundesstaat |
| Municipal | `management/municipal_system.inc` | Kommunale Dienste |
| Airport | `management/airport_system.inc` | Flughafen-Netzwerk |
| State Ledger | `management/state_ledger_system.inc` | Treasury-Ledger |
| State Control UI | `ui/state_control_ui.inc` | Klickbarer Leitstand (Dlg), Steuer/Policy/Transfer |

## 4. Admin
| Modul | Datei | Aufgabe |
|---|---|---|
| Admin System | `admin/admin_system.inc` | 3x/1-Adminsystem, Menü, Set-Commands |
| State Admin | `admin/state_admin_system.inc` | State-Admin-Rechte |
| Admin Config | `admin/admin_config_system.inc` | Ingame-Konfiguration (TD) |
| Admin Mapping | `admin/admin_mapping_system.inc` | Mapping-/Teleport-System (TD + Cmds) |

## 5. Wirtschaft / Handel
| Modul | Datei | Aufgabe |
|---|---|---|
| State Bank | `economy/state_bank_system.inc` | Staatskassen |
| State Tax | `economy/state_tax_system.inc` | Steuer-/Revenue-Logik |
| State Budget | `economy/state_budget_system.inc` | Treasury/Budget-Übersicht |
| Market | `economy/market_system.inc` | Staatsmarkt |
| City Economy | `economy/city_economy_system.inc` | Dynamische Stadtwirtschaft (TD) |
| Dealership | `economy/dealership_system.inc` | Staatliche Autohäuser |
| Ammu | `economy/ammu_system.inc` | Ammu-Nation |
| Contraband | `economy/contraband_system.inc` | Schwarzmarkt-Logistik |
| Marketplace | `commerce/marketplace_system.inc` | Handelsplattform/Konsumgüter |
| District Economy | `world/district_economy.inc` | Bezirkswirtschaft |
| Business | `business/business_system.inc` | Businesses, Einkommen minus Staatsanteil |
| Shops | `shops/shop_system.inc` | Läden (TD) |
| Weapon Shop | `shops/weapon_shop_system.inc` | Waffenladen (`/weapons`, `/buyweapon`) |
| Premium | `premium/premium_system.inc` | Premium-System |
| Ads | `marketing/ad_system.inc` | Werbenetzwerk |

## 6. Jobs / Produktion / Logistik
| Modul | Datei | Aufgabe |
|---|---|---|
| Jobs Core | `jobs/jobs_core.inc` | Job-Typen, Aufgaben, Lohn (Staatskasse/Lohnsteuer) |
| State Jobs | `jobs/state_jobs.inc` | Staatsspezifische Jobs, Lohnfaktor |
| Dynamic Jobs | `jobs/dynamic_job_system.inc` | Dynamische Jobliste (TD, derzeit ohne Aufrufer) |
| Job Center | `jobs/job_center.inc` | Annahme, Aufträge, Fortschritt (Dlg) |
| Missions | `missions/mission_system.inc` | Missions-/Delivery-System (TD, derzeit ohne Aufrufer) |
| Mission Board | `utility/mission_board.inc` | Aufgabenliste |
| Factory Network | `production/factory_network.inc` | Produktionsnetzwerk |
| Trade Logistics | `logistics/trade_logistics.inc` | Inter-State-Logistik, Schmuggelrouten |
| Transport Workers | `workers/transport_workers.inc` | Transportarbeiter |
| Progression | `progression/progression_system.inc` | Level/Fortschritt |

## 7. Transport / Fahrzeuge
| Modul | Datei | Aufgabe |
|---|---|---|
| Vehicle System | `vehicles/vehicle_system.inc` | Fahrzeuge mit Staatslogik |
| Garage | `vehicles/garage_system.inc` | Garagen/Fahrzeuglager |
| Tuning | `vehicles/tuning_system.inc` | Tuning |
| UBB Vehicle | `vehicles/ubb_vehicle_system.inc` | Besitzsystem für UBB/Portal |
| Fuel Stations | `vehicles/fuel_station_system.inc` | Tankstellen/Benzinpreise |
| Transport | `transport/transport_system.inc` | Taxi-/Bus-/Truck-Routen (TD) |
| Taxi | `transport/taxi_system.inc` | Taxi-Netzwerk |

## 8. Gesetz / Justiz / Notfall / Medizin
| Modul | Datei | Aufgabe |
|---|---|---|
| Law | `law/law_system.inc` | Wanted/Arrest/Polizei (TD) |
| Jail | `law/jail_system.inc` | Gefängnis (`/jail`, `/release`, TD-HUD ohne Aufrufer) |
| Crime | `crime/crime_system.inc` | Crime-Tracker (TD) |
| Prison | `justice/prison_system.inc` | Justiz/Haft |
| Court | `justice/court_system.inc` | Gericht |
| Emergency | `emergency/emergency_system.inc` | Polizei/EMS |
| Dispatch | `emergency/dispatch_system.inc` | Notruf-Queue (`/dispatch`, `/acceptcall` aktiv angebunden) |
| EMS Hospital | `medical/ems_hospital.inc` | Krankenhäuser/Versicherung |
| Drugs | `drugs/drug_system.inc` | Drogenmarkt (TD, derzeit ohne Aufrufer) |

## 9. Fraktionen / Kriege / Welt
| Modul | Datei | Aufgabe |
|---|---|---|
| Faction | `factions/faction_system.inc` | Fraktionen/Gangs |
| Gangwar | `factions/gangwar_system.inc` | Gangwar (TD) |
| Gang Fight | `factions/gang_fight_system.inc` | Gangfights (TD) |
| Text Wars | `factions/text_wars.inc` | Klickbares Kriegs-Widget + Dialoge |
| Territory | `world/territory_system.inc` | Territorien/Einflusszonen |
| Landmarks | `world/landmarks_system.inc` | Stadtmarken |

## 10. Wohnen / Inventar / Soziales / Freizeit
| Modul | Datei | Aufgabe |
|---|---|---|
| House | `housing/house_system.inc` | Dynamische Häuser |
| House Storage | `housing/house_storage_system.inc` | Safes/Lager |
| Accessories | `housing/accessory_system.inc` | Accessoires/Fashion |
| Inventory | `inventory/inventory_system.inc` | Relationales Inventar |
| Phone | `communication/phone_system.inc` | Handy/SIM |
| Social | `social/social_system.inc` | `/me`, `/do`, `/report`, `/help` |
| Roleplay Commands | `social/roleplay_command_system.inc` | Zusatz-Commands (teilweise verdeckt) |
| Relationship | `social/relationship_system.inc` | Beziehungen |
| Gym | `leisure/gym_system.inc` | Fitness |
| Fishing | `leisure/fishing_system.inc` | Angeln |
| Casino | `leisure/casino_system.inc` | Casino |

## 11. Events / Season / Web
| Modul | Datei | Aufgabe |
|---|---|---|
| Event System | `events/event_system.inc` | Event-Start/-Ende |
| State Events | `events/state_event_system.inc` | Staatliche Events/News (`/events`) |
| Season | `events/season_system.inc` | Season-System (siehe `SEASON.md`) |
| Web Bridge | `web/web_bridge.inc` | UCP/Web-Anbindung |

## 12. UI
| Modul | Datei | Aufgabe |
|---|---|---|
| UI Dialog IDs | `ui/ui_dialog_ids.inc` | Zentrale Dialog-ID-Bereiche, `UiDialog_*`-Wrapper |
| UI Hub | `ui/ui_hub.inc` | Dialog-First-Menüs (größtes UI-Modul) |
| Roleplay UI | `ui/roleplay_ui_system.inc` | HUD-Dashboard (TD) |
| State Control UI | `ui/state_control_ui.inc` | Leitstand |

## Hinweise
- Initialisierungsreihenfolge: DB → State → Admin → Jobs/Login → Kamera/Tutorial → Government/House/Business → Fraktionen → … → UI-Hub (zuletzt). Siehe `OnGameModeInit` in `gamemode.pwn`.
- Dialog-Responses laufen über `UiHub`, `JobCenter`, `StateControl`, `TextWars` (Reihenfolge in `OnDialogResponse`).
- Mehrere Module sind schlanke Grundgerüste (40–100 Zeilen) mit Initialisierung, Daten und Anzeige; Ausbau laut `TO-DO.md`.
- `Compiler/samp03/gamemodes/` ist eine ältere Kopie; maßgeblich ist `gamemodes/`.


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

