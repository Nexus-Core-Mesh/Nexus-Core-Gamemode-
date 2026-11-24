# Wirtschaftssysteme und Geldfluss

Dieses Dokument beschreibt die Wirtschafts- und Finanzarchitektur des Gamemodes.

## 1. Überblick

Das Projekt hat eine breit aufgestellte Wirtschaftschicht mit mehreren Modulen:

- `economy/state_bank_system.inc`
- `economy/state_budget_system.inc`
- `economy/state_tax_system.inc`
- `economy/market_system.inc`
- `economy/city_economy_system.inc`
- `economy/contraband_system.inc`
- `economy/dealership_system.inc`
- `economy/ammu_system.inc`
- `business/business_system.inc`
- `shops/shop_system.inc`
- `shops/weapon_shop_system.inc`
- `commerce/marketplace_system.inc`
- `world/district_economy.inc`

## 2. Grundmodell

Die Wirtschaft ist in drei Ebenen verteilt:

1. Spieler-Bank / Cash
2. Staatskasse / Budget / Tax
3. Verschiedene Markt- und Business-Subsysteme

`core/core.inc` definiert zentral:

- `E_PLAYER_BANK`
- `gPlayerBank`
- `gPlayerCash`
- `Core_SetPlayerBank()`
- `Core_ChangePlayerBank()`

Das ist eine zentrale Grundlage für das Geldsystem.

## 3. Player-Bank und Cash

`Core_SetPlayerBank()` setzt einen Betrag direkt in:

- `gPlayerData[playerid][E_PLAYER_BANK]`
- `gPlayerBank[playerid]`

`Core_ChangePlayerBank()` prüft:

- Ist der Betrag gültig?
- Ist nextBank negativ?
- Wenn negativ: reject

Das ist eine robuste Grundlage, damit Geld nicht unkontrolliert verloren geht.

## 4. Staatskassen

`state_bank_system.inc` verwaltet die Hauptkassen der Staaten.

`state_system.inc` definiert die State-Kassenwerte:

- `gStateBankCash[STATE_COUNT]`

Die Initialwerte sind:

- LS: 500000
- SF: 450000
- LV: 400000

Diese Kassen sind an die politische Struktur der 3 Staaten gekoppelt.

## 5. Steuerlogik

`state_tax_system.inc` verwaltet:

- `gStateTaxRate[STATE_COUNT]`
- `gStateTaxRevenue[STATE_COUNT]`

Diese Werte sind in `StateSystem_Initialize()` gesetzt:

- LS: 8%
- SF: 7%
- LV: 9%

Das ist wichtig für:

- Lohnsteuer
- Marktsteuer
- Staatseinnahmen
- Budget-/Treasury-Abstimmungen

## 6. Staatliches Budget

`state_budget_system.inc` und `state_budget_`-Funktionen zeigen die Preispolitik und die Treasury-Übersicht.

Das System erlaubt:

- Budgetübersicht je Staat
- Treasury-Info via Command `/treasury`
- State-gebundene staatliche Finanzen

## 7. Markt und Stadtwirtschaft

### `market_system.inc`

Zentrale staatliche Marktlogik.

### `city_economy_system.inc`

Zeigt die Stadtwirtschaft an:

- LS
- SF
- LV

Das betrifft alle relevanten Städte und übergeordnete Wirtschaftsstromlinien.

### `district_economy.inc`

Bezirks-/Distrikt-Wirtschaft.

Diese Logik zeigt, dass die Wirtschaft nicht nur pro Staat, sondern auch pro Gebiet / District funktioniert.

## 8. Business-System

`business/business_system.inc` ist das eigentliche Gewerbesystem.

Die daraus entstehenden Funktionen betreffen:

- Firmen-/Business-Registrierung
- Owner-Logik
- Auszahlungen
- Staatsanteil
- Geldflüsse in die Staatskasse

Das Modul ist an die Wirtschaft und an die State-Struktur gekoppelt.

## 9. Shop-Systeme

### `shops/shop_system.inc`

Laden- und Shop-Übersicht.

### `shops/weapon_shop_system.inc`

Waffenladen-System.

Befehle:

- `/weapons`
- `/buyweapon`

Das ist eine wichtige Verbindung zwischen Wirtschaft und Gameplay.

## 10. Marketplace / Contraband / Logistics

### `commerce/marketplace_system.inc`

Handelsplattform / Konsumgüter.

### `economy/contraband_system.inc`

Black-market logistics.

### `logistics/trade_logistics.inc`

Inter-State-Logistik und Schmuggelrouten.

Dies zeigt, dass das System nicht nur Banken und Preise kennt, sondern auch Handels- und Schmuggelwirtschaft organisiert.

## 11. Commands zur Wirtschaft

Im Gamemode gibt es mehrere Economy-Commands:

- `/treasury`
- `/marketplace`
- `/market`
- `/tax`
- `/district_economy`
- `/economy`
- `/shops`

Diese Commands werden direkt in `gamemode.pwn` oder durch UI-Hub aufgerufen.

## 12. Spieler- und Staatsfinanzen zusammen

Ein realistischer Ablauf ist:

1. Spieler arbeitet oder kauft etwas
2. Lohn geht an `Core_ChangePlayerBank()`
3. Staatlicher Anteil wird in State-/Budgetlogik eingezogen
4. Markt-/Trade-Systeme beeinflussen die Kassen
5. `StateBudgetSystem` und `StateTaxSystem` spiegeln das Ergebnis wider

## 13. Architekturelle Beobachtung

Die Wirtschaft ist bewusst verteilt, aber alle Teilmodule greifen in eine gemeinsame wirtschaftliche Perspektive:

- Geldfluss aus Spieler zu State
- State-Revenue
- Market- und Business-Logik
- Shops und Verkauf
- Black-market und Logistics

Das ist ein gutes Zeichen für ein modulares RP-System.

## 14. Fazit

Die Wirtschaft des Gamemodes ist ein sehr zentraler Kernbereich. Sie verbindet:

- Spielerbank
- State-Kasse
- Steuern und Budget
- Business
- Shop-Verkauf
- Marktplätze
- Black-Market und Logistik

Die Architektur ist breit und leistungsfähig, aber sie braucht langfristig eine klare Konsistenz durch die Core- und DB-Schicht, damit keine parallelen Geldquellen entstehen.


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

