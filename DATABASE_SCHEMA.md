# Datenbankschema und Persistenz-Architektur

Dieses Dokument beschreibt die Persistenz- und DB-Schicht des Gamemodes im aktuellen Codestand.

## 1. Überblick

Der Gamemode hat eine zentrale DB-Abstraktionsschicht in:

- `gamemodes/modules/db/db_interface.inc`
- `gamemodes/modules/db/db_sqlite.inc`
- `gamemodes/modules/db/db_mysql.inc`

Die Kernidee ist:

- Dini/Fallback-File-Storage ist der primäre Laufzeitpfad
- SQLite/MySQL sind als Backup-, Auto-Sync- oder Dashboard-Layer vorgesehen
- `DB_Initialize()` und `DB_RepairCriticalTables()` sollen die wichtigsten Speicherbereiche idempotent sichern

## 2. DB-Backend

`db_interface.inc` definiert:

- `DB_BACKEND_SQLITE = 0`
- `DB_BACKEND_MYSQL = 1`

Der aktive Backend-Status ist standardmäßig auf SQLite gesetzt:

- `new gDBBackend = DB_BACKEND_SQLITE;`

Das bedeutet: Das Projekt ist derzeit auf eine SQLite-/Dini-Hybridlogik ausgerichtet, statt einen live aktiven MySQL-Write-Stack zu benutzen.

## 3. Dini-first-Strategie

Der Projektkontext und die Doku betonen: Der reale primäre Persistenzpfad ist Dini unter `Compiler/samp03/scriptfiles/`.

Die relevanten Laufzeit-/Verzeichnisbereiche sind:

- `players/`
- `houses/`
- `house_storage/`
- `safes/`
- `vehicles/`
- `ubb_vehicles/`
- `state/LS/`, `state/SF/`, `state/LV/`
- `state_kasse/LS/`, `state_kasse/SF/`, `state_kasse/LV/`
- `business/`
- `factions/`
- `inventory/`
- `temp/`
- `backup/`
- `logs/`

Dieser Pfad ist in der Architektur-Doku explizit beschrieben и als primäre Persistenzlogik vorgesehen.

## 4. Runtime-Caches

`db_interface.inc` definiert mehrere sehr relevante Cache-Blöcke:

### Spieler-Caches

- `gDBPlayerStateCache[MAX_PLAYERS]`
- `gDBPlayerHomeStateCache[MAX_PLAYERS]`
- `gDBPlayerBankCache[MAX_PLAYERS]`
- `gDBPlayerCashCache[MAX_PLAYERS]`
- `gDBPlayerInventoryCache[MAX_PLAYERS][MAX_INVENTORY_SLOTS]`
- `gDBPlayerInventoryQtyCache[MAX_PLAYERS][MAX_INVENTORY_SLOTS]`
- `gDBPlayerInventoryMetaCache[MAX_PLAYERS][MAX_INVENTORY_SLOTS][32]`

### Haus-/Storage-Caches

- `gDBHouseStorageCache[MAX_HOUSES][MAX_HOUSE_STORAGE_SLOTS]`
- `gDBHouseStorageQtyCache[MAX_HOUSES][MAX_HOUSE_STORAGE_SLOTS]`
- `gDBHouseStorageMetaCache[MAX_HOUSES][MAX_HOUSE_STORAGE_SLOTS][32]`

### UBB-Fahrzeug-Caches

- `gDBUBBVehicleModelCache[MAX_UBB_VEHICLES]`
- `gDBUBBVehicleOwnerCache[MAX_UBB_VEHICLES]`
- `gDBUBBVehicleStateCache[MAX_UBB_VEHICLES]`
- `gDBUBBVehicleFuelCache[MAX_UBB_VEHICLES]`
- `gDBUBBVehiclePriceCache[MAX_UBB_VEHICLES]`

Das zeigt, dass die DB-Schicht bewusst Runtime- und Persistenzdaten in Caches vorhält, um in GTA:SA-Pawn-Umgebungen schnell auf Daten zuzugreifen.

## 5. Tabellen-/Schema-Repair-Funktionen

`DB_RepairCriticalTables()` ruft eine Reihe von Schema-Helfern auf:

- `DB_EnsureStateSchema()`
- `DB_EnsureStateBankSchema()`
- `DB_EnsureHouseSchema()`
- `DB_EnsurePlayerVehicleSchema()`
- `DB_EnsurePropertySchema()`
- `DB_EnsureInventorySchema()`
- `DB_EnsureInventoryIntegrity()`
- `DB_EnsureHouseStorageSchema()`
- `DB_EnsureHouseStorageIntegrity()`
- `DB_EnsureAccessorySchema()`
- `DB_EnsureContactSchema()`
- `DB_EnsureCriminalRecordSchema()`
- `DB_EnsureVehicleModsSchema()`
- `DB_EnsureUBBVehicleSchema()`
- `DB_EnsureUBBVehicleIntegrity()`
- `DB_EnsureFuelStationSchema()`
- `DB_EnsureWebAccountSchema()`
- `DB_EnsureMapSchema()`

Das ist ein gutes Zeichen: Die Datenbank- und Persistenz-Schicht ist nicht einfach nur eine Singleton-API, sondern ein ernsthaftes Schema-Monitoring.

## 6. Transaktionen und Atomarität

`db_interface.inc` definiert ebenfalls Transaktionsfunktionen:

- `DB_BeginTransaction()`
- `DB_CommitTransaction()`
- `DB_RollbackTransaction()`
- `DB_ExecuteAtomicWrite()`
- `DB_ExecuteTransaction()`
- `DB_TransactionWriteWithGuard()`

Diese Funktionen sind wichtig für:

- kritische State-Transitions
- sichere Save-/Load-Operationen
- Steuer-/Budget-Schreibvorgänge
- komplexe Inventar- und Fahrzeug-Updates

## 7. SQLite- und MySQL-Backend

### SQLite

`db_sqlite.inc` ist der aktive SQLite-Stapel. Das Projekt hält ihn als Standard für lokale Datenbank-Reads/Writes.

### MySQL

`db_mysql.inc` ist vorhanden und vorbereitet, aber der Code zeigt, dass der Gamemode bisher bewusst nicht vollständig auf aktiven MySQL-Write-Flow setzt.

Das ist ein klares Architekturmerkmal:

- SQLite wird laufend als lokale Basis oder Sync-Layer genutzt
- MySQL ist für spätere Ausbaustufen vorbereitet
- Dini bleibt im primären Produktivpfad robust und unabhängig

## 8. Verbindung zum Core

`core/core.inc` enthält die zentralen Spieler- und State-Werte. `db_interface.inc` nutzt diese als Grundlage für den Datenfluss:

- `gPlayerData[playerid][E_PLAYER_STATE]`
- `gPlayerData[playerid][E_PLAYER_HOME_STATE]`
- `gPlayerData[playerid][E_PLAYER_BANK]`
- `gPlayerData[playerid][E_PLAYER_WANTED]`
- `gPlayerData[playerid][E_PLAYER_JAIL_TIME]`

Damit stimmt die Persistenzarchitektur mit dem Kernmodell überein.

## 9. Typische Persistenz-Abläufe

### Player-Connect

Beim Connect:

- `StateSystem_OnPlayerConnect()`
- `DB_EnsurePlayerRecord()`
- `DB_RehydratePlayerState()`
- `DB_LoadPlayerState()`
- `Core_SetPlayerState()`
- `Core_SetPlayerHomeState()`
- `DB_SavePlayerState()`

### Player-Disconnect

Beim Disconnect:

- `StateSystem_OnPlayerDisconnect()`
- `DB_SavePlayerState()`
- Cash und State werden gesichert

### State-Transfer

Bei wichtigen Übergängen:

- State-Werte validieren
- Fallback auf gültigen State
- Transaktion / Sicherung
- Save- und Reload-Pfad

## 10. Sicherheits- und Strukturprinzipien

Das Projekt versucht, die Datenbank-Logik robust zu machen durch:

- Validierung vor Save
- Integrität-Checks (`*_Integrity`)
- Transaction guards
- Backup snapshots
- zentrale Rehydrate-/Persistenz-APIs

## 11. Wichtigste Beobachtung

Die DB-Architektur ist bewusst nicht eine rohe „direkte SQL-Schicht“, sondern ein mehrstufiges System aus:

1. Core- und State-Level
2. Runtime-Caches
3. DB-Adapter-und-Schema-Checks
4. Save/Load-APIs
5. Dini- oder SQLite-Backing-Storage

Das ist ein starkes Design für einen modulares SA:MP-Projekt.

## 12. Fazit

Die Persistenzlogik ist ein zentraler Architekturteil des Gamemodes. Sie verbindet:

- Player-Profil
- State-Informationen
- Inventory / House / Vehicle / UBB
- Cash und Banksysteme
- Wirtschaft und Verwaltung

Das System ist erweiterbar, aber es verlangt die Einhaltung der Helfer- und Validierungsregeln. Genau deshalb ist es wichtig, dass neue Module immer auf die bestehende `Core`- und `DB`-Struktur aufsetzen.


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
