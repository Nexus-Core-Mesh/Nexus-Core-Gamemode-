# Nexus Core – TextDraw-Übersicht (Ist-Stand)

Quelle: `gamemodes/modules/**`. Zentrale Wrapper in `ui/ui_dialog_ids.inc`: `UiDialog_TextDrawCreate`, `UiDialog_CreatePlayerTextDraw`.

Es gibt **16 Per-Player-Arrays globaler `Text:`-TextDraws** (pro Spieler erzeugt, danach `Show/Hide/Destroy`) und **1 echtes `PlayerText:`-Widget** (Text-Wars, klickbar). Koordinaten sind 640×480-Raster. „Box“ = TextDraw mit Hintergrundbox (`UseBox`).

Klick-Callbacks (`gamemode.pwn`): `OnPlayerClickPlayerTextDraw` und `OnPlayerClickTextDraw` werden nur von Text-Wars genutzt.

## Übersicht

| # | Modul / Datei | Array | Draws | Auslöser | Status |
|---|---|---|---|---|---|
| 1 | Login (`auth/login_system.inc`) | `gLoginTD[8]` | 5 (Login) / 6 (Registrierung) | Connect/Auth-Flow | aktiv |
| 2 | Tutorial (`tutorial/tutorial_system.inc`) | `gTutorialTD[4]` | 2–3 je Stufe | `/tutorial`, Auth-Flow | aktiv |
| 3 | Roleplay-Dashboard (`ui/roleplay_ui_system.inc`) | `gRoleplayUIDraw[10]` | 10 | `/hud`, `/dashboard`, Spawn; `/hudhide` | aktiv |
| 4 | Text-Wars (`factions/text_wars.inc`) | `gTWDraw[11]` (PlayerText) | 11 (6 Anzeige + 5 Buttons) | `/warhud`, `/gangwar`, `/gangfight`, Menü | aktiv, klickbar |
| 5 | Gangwar (`factions/gangwar_system.inc`) | `gGangWarTD[8]` | 3 | `/war` | aktiv |
| 6 | Gangfight (`factions/gang_fight_system.inc`) | `gGangFightTD[8]` | 3 | `/fight` | aktiv |
| 7 | Wanted (`law/law_system.inc`) | `gLawTD[8]` | 3 | `/wanted` | aktiv |
| 8 | Crime Tracker (`crime/crime_system.inc`) | `gCrimeTD[8]` | 3 | `/crime` | aktiv |
| 9 | Jail-HUD (`law/jail_system.inc`) | `gJailTD[8]` | 2 | `JailSystem_ShowHUD` – **kein Aufrufer** | gebaut, nicht angebunden |
| 10 | Transport-Routen (`transport/transport_system.inc`) | `gTransportTD[8]` | 3 | `/routes` | aktiv |
| 11 | Stadtwirtschaft (`economy/city_economy_system.inc`) | `gCityEconomyTD[8]` | 4 | `/economy` | aktiv |
| 12 | Shop-Übersicht (`shops/shop_system.inc`) | `gShopTD[8]` | 1 + bis 6 Shops | `/shops` | aktiv |
| 13 | Admin-Konfig (`admin/admin_config_system.inc`) | `gAdminConfigTD[8]` | 8 | `/adminconfig`, `/config` (Moderator+) | aktiv |
| 14 | Admin-Mapping (`admin/admin_mapping_system.inc`) | `gAdminMapTD[12]` | 3 + bis 6 Positionen | `/mapmenu` | aktiv |
| 15 | Drogenmarkt (`drugs/drug_system.inc`) | `gDrugTD[8]` | 1 + Drogentypen | `DrugSystem_ShowMarket` – **kein Aufrufer** | gebaut, nicht angebunden |
| 16 | Job-Liste (`jobs/dynamic_job_system.inc`) | `gDynamicJobTD[8]` | 2 + bis 6 Jobs | `DynamicJobSystem_ShowStateJobs` – **kein Aufrufer** | gebaut, nicht angebunden |
| 17 | Mission-Board (`missions/mission_system.inc`) | `gMissionTD[8]` | 1 + bis 6 Missionen | `MissionSystem_ShowMissionList` – **kein Aufrufer** | gebaut, nicht angebunden |

Hinweis: Die Draws 3 sowie 5–8, 10–14 werden bei erneutem Aufruf zuerst entfernt (Hide/Destroy), dann neu aufgebaut.

---

## Details je TextDraw-Box

### 1. Login / Registrierung – `gLoginTD` (zentriert, x=320, `UiDialog_TextDrawCreate`)
- Login (5): Titel y=40 · Header y=80 · Text y=120 · Hinweis y=150 · Info y=178.
- Registrierung (6): Titel y=40 · Registrier-Text y=75 · Heimat y=110 · Städte y=140 · Ablauf y=170 · Tutorial-Hinweis y=195.
- Passworteingabe läuft über Dialog `DIALOG_UI_AUTH_PASSWORD`, nicht über TextDraw.

### 2. Tutorial – `gTutorialTD` (x=320, Titel y=120, Text y=160, ggf. Intro y=185)
- Stufe 0 „Willkommen“: Titel + Welcome + Intro (3 Draws)
- Stufe „Heimat“: Titel + Beschreibung (2)
- Stufe „Regierung“: Titel + Beschreibung (2)
- Stufe „Abschluss“: Titel + Beschreibung (2)

### 3. Roleplay-Dashboard – `gRoleplayUIDraw` (links oben)
| Idx | Position | Inhalt |
|---|---|---|
| 0 | 12 / 18 | Box links (Spielerblock) |
| 1 | 18 / 20 | Spieler-Header |
| 2 | 18 / 36 | Staat-Zeile |
| 3 | 18 / 49 | Fortschritt/Level |
| 4 | 18 / 62 | Sonstiges (Geld usw.) |
| 5 | 220 / 18 | Box Mitte |
| 6 | 226 / 20 | „LIVE EVENT / RUECKSPIEGEL“ |
| 7 | 226 / 36 | „Status: Stadt-/State-Updates und Systemmeldungen“ |
| 8 | 470 / 18 | Box rechts |
| 9 | 476 / 20 | „KONTEXT \| MENU / TEXT-WARS“ |

### 4. Text-Wars-Widget – `gTWDraw` (PlayerTextDraws, rechts, klickbar)
| Idx | Name | Position | Inhalt |
|---|---|---|---|
| 0 | `TW_DRAW_BOX` | 470 / 150 → 632, Höhe 17.6 | Hintergrundbox `0x000000AA` |
| 1 | `TW_DRAW_TITLE` | 476 / 153 | „TEXT-WARS“ (rot) |
| 2 | `TW_DRAW_WAR` | 476 / 172 | Gangwar-Zeile (weiß) |
| 3 | `TW_DRAW_FIGHT` | 476 / 186 | Gangfight-Zeile (gelb) |
| 4 | `TW_DRAW_LAND` | 476 / 200 | Gebiets-Zeile (grün) |
| 5 | `TW_DRAW_FACTION` | 476 / 214 | Eigene Fraktion (blau) |
| 6 | `TW_BTN_STATUS` | 513 / 232 | Button „STATUS“ |
| 7 | `TW_BTN_LAND` | 592 / 232 | Button „GEBIETE“ |
| 8 | `TW_BTN_ATTACK` | 513 / 252 | Button „ANGRIFF“ |
| 9 | `TW_BTN_TROOPS` | 592 / 252 | Button „TRUPPEN“ |
| 10 | `TW_BTN_CLOSE` | 552 / 272 | Button „SCHLIESSEN“ |

Buttons: Box `0x224466BB`, selectable, 70×14. Live-Update über Timer `TextWars_Tick`. ESC gibt den Mauszeiger frei; Widget bleibt sichtbar. Buttons ≠ Schließen öffnen Dialoge (Bereich `DIALOG_UI_RANGE_WAR_*`).

### 5–8. Status-HUDs (zentriert x=320)
| TextDraw | Zeilen (y) |
|---|---|
| Gangwar | „GANGWAR STATUS“ 35 · Kriegs-Zeile 60 · Fraktions-Zeile 84 |
| Gangfight | „GANGFIGHT HUD“ 30 · Fight-Zeile 58 · Score-Zeile 82 |
| Wanted | „WANTED STATUS“ 30 · Wanted-Level 62 · Rolle 86 |
| Crime Tracker | „CRIME TRACKER“ 30 · Heat 62 · Verbrechenstyp 86 |

### 9. Jail-HUD – `gJailTD` (nicht angebunden)
„JAIL HUD“ y=30 · Rest-/Bootzeit y=62.

### Zusätzliche Statusklarstellung
- `/dispatch` und `/acceptcall` sind in der Main-Command-Chain aktiv angebunden; sie laufen über `DispatchSystem_HandleCommand()` und nicht als separate TextDraw-UI.
- Die übrigen als „nicht angebunden“ markierten Anzeigen (`DrugSystem_ShowMarket`, `DynamicJobSystem_ShowStateJobs`, `MissionSystem_ShowMissionList` etc.) bleiben weiterhin nur als Bauzustand mit vorhandenen `Show*`-Funktionen dokumentiert.

### 10–12. Infoboards (x=320)
| TextDraw | Zeilen (y) |
|---|---|
| Transport-Routen | „TRANSPORT ROUTES“ 30 · Staat 60 · Route 84 |
| Stadtwirtschaft | „CITY ECONOMY“ 28 · LS 58 · SF 82 · LV 106 |
| Shop-Übersicht | „SHOP OVERVIEW“ 30 · bis zu 6 Shop-Zeilen darunter |

### 13. Admin-Konfig – `gAdminConfigTD` (x=320, y 28 → 182, 8 Draws)
Titel 28 · State-Header 52 · State-Text 74 · Steuer 94 · Gangwar 116 · Config-Commands 138 · Beispiel 160 · Live-Hinweis 182.

### 14. Admin-Mapping – `gAdminMapTD`
„MAPPING CONTROL“ y=30 · Header y=58 · bis zu 6 Positionszeilen (y=82 + 18·i, `ID: Name`) · Command-Hinweis y=200 („/mapadd /maptp /mapdel /maplist“). Array-Größe 12.

### 15–17. Nicht angebundene Listen
| TextDraw | Aufbau |
|---|---|
| Drogenmarkt | „DRUG MARKET“ y=28 + je Drogentyp eine Zeile (Staatsbestand/Preis) |
| Job-Liste | „JOB LIST“ y=30 · Titel y=55 · bis 6 Jobzeilen |
| Mission-Board | „MISSION BOARD“ y=30 · bis 6 Missionszeilen |

Die zugehörigen `Show*`-Funktionen sind vorhanden, werden aber in `gamemode.pwn` und anderen Modulen nirgends aufgerufen (`Hide*` ebenso). Anbindung z. B. per Command oder über `UiHub` steht noch aus.

---

## Dialoge statt TextDraws
Die meisten Menüs (UI-Hub, Inventar, Handy, Business, Garage, Tuning, Job-Menü, Leitstand, Admin-Menü, Text-Wars-Auswahl) nutzen `ShowPlayerDialog` über `UiDialog_Show` und sind keine TextDraws.

## Beobachtungen
- Textdraws der Gruppen 5–8/10–14 erhalten keine Klick-Logik, nur Anzeige (Hide über Timer/erneuten Aufruf).
- Nur Text-Wars verwendet `PlayerText:` (empfohlen für spätere Umstellung der übrigen HUDs, spart Global-TD-Limit).
- `Compiler/samp03/gamemodes/` ist ein älterer Spiegelstand; maßgeblich ist `gamemodes/`.


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

