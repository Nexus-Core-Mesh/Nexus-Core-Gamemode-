# Housing-, Inventory- und Besitzsysteme

Dieses Dokument beschreibt die Spielerdaten rund um Häuser, Inventare, Storage und Besitzlogik.

## 1. Überblick

Wichtige Module:

- `housing/house_system.inc`
- `housing/house_storage_system.inc`
- `housing/accessory_system.inc`
- `inventory/inventory_system.inc`
- `business/business_system.inc`
- `vehicles/garage_system.inc`

Diese Systeme bilden die Grundlage für:

- Häuser und Properties
- Lager und Safes
- Accessoires und Fashion
- Inventory-Slots und Item-Handling
- Besitz mit wirtschaftlichem Kontext

## 2. Core-Grundlagen

In `core/core.inc` gibt es feste Begrenzungen:

- `MAX_INVENTORY_SLOTS 24`
- `MAX_HOUSE_STORAGE_SLOTS 12`
- `MAX_HOUSES 50`
- `MAX_UBB_VEHICLES 100`

Das zeigt, dass die Projektarchitektur bewusst auf feste Grenzen setzt, um Überlauf oder unkontrollierte Zustände zu verhindern.

## 3. Inventory-System

`inventory/inventory_system.inc` bildet die zentrale Inventar-Logik. Es arbeitet mit:

- `MAX_INVENTORY_SLOTS`
- Item-Kategorien / Items
- Slot-basierter Speicherung
- Mengen und Metadaten
- Ein- und Ausgaben

### Wichtige Regeln

- Verbrauchbare Items werden nur dann verbraucht, wenn das entsprechende Verhalten aktiviert ist
- `Inventory_Remove()` ist der Kern der Verbrauchslogik
- Module greifen auf das Inventar nicht als freie globale Struktur zu, sondern über semantische Funktionen

## 4. House-System

`house_system.inc` verwaltet dynamische Häuser.

Es bildet:

- Hausbesitz
- Ownership
- house-based gameplay
- Häuser als Properties mit Einschränkungen

## 5. House Storage und House Safes

`house_storage_system.inc` erweitert die Hauslogik um Lager- und Safe-Mechaniken.

Die DB- und Cachesysteme enthalten dafür:

- `gDBHouseStorageCache`
- `gDBHouseStorageQtyCache`
- `gDBHouseStorageMetaCache`

Das zeigt, dass Lagerbereiche als definierte Speicherblöcke modelliert sind.

## 6. Accessory-System

`accessory_system.inc` modelliert:

- Kleidung/Fashion
- Accessoires
- dekorative oder spielerbezogene Ausstattungen

Dieses System ist eher RP- und Character-bezogen, aber es ist mit dem Besitz-/Inventar-Kontext verbunden.

## 7. Business und Häuser kombiniert

Business- und Hausmodule sind eng verwoben:

- Business zeigt Eigentümer, Payouts, Einnahmen
- House zeigt Besitz und Lager
- Inventory ist die direkte Gegenstandslogik

Damit wird ein klarer RP- und Besitzfluss sichtbar: Besitz -> Wirtschaft -> Items -> Räume.

## 8. Garagen und Fahrzeugbesitz

Garagen sind semantisch Teil der Besitzlogik, weil sie ein Fahrzeug aufhalten, ausgeben und aus dem Inventar-/Besitzmodell abrufen.

Die Garagen-Architektur ist ebenfalls mit:

- `vehicle_system.inc`
- `garage_system.inc`
- `fuel_station_system.inc`
- `ubb_vehicle_system.inc`

verbunden.

## 9. Sicherheitsprinzipien

Das Projekt verwendet bewusst technische Guards, um Laufzeitfehler zu verhindern:

- value bounds für Inventar-Slots
- House Storage Limits
- Zusammenpassung von Ownership- und Inventory-Checks
- produzierte Validierungsfunktionen in `production_guard.inc`

`production_guard.inc` weist explizit darauf hin, dass:

- Inventory-Slots
- House Storage Slots
- UBB vehicles

begrenzt sind.

## 10. Typische Nutzung

Ein realistischer Ablauf:

1. Spieler kauft oder erhält Haus
2. House Storage und Inventar werden initialisiert
3. Spieler erhält Gegenstände oder Fahrzeuge
4. Accessoires oder Ressourcennutzung erfolgt über Inventory
5. Garage / Fahrzeuge greifen auf den Besitz zu
6. Wirtschaftlich wirkt das alles zusammengenommen in den Business-/State-Modellen

## 11. Bedeutung für RP

Housing und Inventory sind fundamental für den Spielcharakter. Sie fungieren als Grundlage für:

- Eigentum
- Lagerung
- Wirtschaft
- Social-Status
- Fahrzeugzugang
- persönlicher Besitz und Haus-Mechanik

## 12. Fazit

Housing- und Inventory-System sind zentrale Lebensbereiche des Gamemodes. Sie verbinden:

- persönliches Eigentum
- interaktive Gegenstände
- Speicher
- Quests und Jobs
- Business
- Fahrzeug- und Garagenlogik

Sie sind damit eines der wichtigsten Grundlagen für einen detailreichen RP-Loop.


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

