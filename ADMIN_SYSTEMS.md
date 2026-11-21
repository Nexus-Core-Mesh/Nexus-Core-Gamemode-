# Admin-Systeme und Rechtemodell

Dieses Dokument beschreibt die Admin-Architektur im aktuellen Gamemode-Stamm aus `gamemodes/modules/admin/*` und `gamemodes/modules/core/core.inc`.

## 1. Überblick

Das Projekt verwendet ein 3x/1-Adminmodell:

- 1x Support
- 1x Moderator
- 1x State-Admin
- 1x State-Leader
- 1x Owner

Die relevanten Konstanten in `core/core.inc` sind:

- `ADMIN_RANK_SUPPORT = 1`
- `ADMIN_RANK_MODERATOR = 2`
- `ADMIN_RANK_STATE_ADMIN = 3`
- `ADMIN_RANK_STATE_LEADER = 4`
- `ADMIN_RANK_OWNER = 5`

Das System ist im `admin_system.inc` implementiert und läuft parallel zur zentralen `gPlayerData`-Struktur im Core.

## 2. Kernprinzipien

### 2.1 Single Source of Truth

`core/core.inc` enthält die zentralen Werte:

- `E_PLAYER_ADMIN_LEVEL`
- `E_PLAYER_ADMIN_STATE`
- `E_PLAYER_PROJECT_LEADER`

Dadurch werden Admin-Flags nicht als zweite, unabhängige Wahrheit in einem anderen Modul geführt.

### 2.2 State- und World-Rechte

Das Projekt unterscheidet:

- globales Admin-Level
- staatliches Admin-Level
- Weltverwaltung (World Management)
- Projektleiter-Rechte

Diese Unterscheidung ist in Funktionen wie `AdminSystem_CanManageWorld()`, `AdminSystem_IsModerator()`, `AdminSystem_IsStateLeader()` und `AdminSystem_IsProjectLeader()` sichtbar.

## 3. Administrationslevel

### Support

- Base-Level 1
- niedrige Rechte
- dient als Einstieg in administrative Funktionen

### Moderator

- Level 2
- Zugriff auf die typischen Moderationsbefehle
- `AdminSystem_IsModerator(playerid)` prüft diesen Zugriff

### State Admin

- Level 3
- dient für staatliche Administration und State-spezifische Aufgaben

### State Leader

- Level 4
- staatlicher Führungsstatus
- darf Aufgaben verwalten, die staatsspezifisch sind

### Owner

- Level 5
- höchster Rang
- wird für globale bzw. projektweite Rollen benutzt

## 4. Admin-Funktionen

### `AdminSystem_Initialize()`

Setzt Initialisierungslogs:

- `[ADMIN] System initialisiert`
- `[ADMIN] 3x/1Admin System aktiviert`

### `AdminSystem_OnPlayerConnect()`

- setzt `gAdminLevel[playerid] = 0`
- setzt `gAdminState[playerid] = -1`
- setzt Core-Werte auf 0 / -1
- normalisiert den State des Spielers

### `AdminSystem_OnPlayerDisconnect()`

- setzt Zugangswerte wieder zurück
- entfernt lokale Rechte für die Session

## 5. Zugriffprüfung

### `AdminSystem_IsProjectLeader(playerid)`

Gibt true zurück, wenn der Spieler:

- `Core_IsProjectLeader(playerid)` ist,
- `StateAdminSystem_IsGlobalOwner(playerid)` ist,
- oder `AdminSystem_IsOwner(playerid)` ist.

### `AdminSystem_IsOwner(playerid)`

Prüft:

- `gAdminLevel[playerid] >= ADMIN_RANK_OWNER`
- oder Core-Level entsprechend
- oder `StateAdminSystem_IsGlobalOwner(playerid)`

### `AdminSystem_IsStateLeader(playerid)`

Prüft globale oder State-gebundene Leitungsrechte.

### `AdminSystem_IsStateAdmin(playerid)`

Prüft staatliche Adminrechte.

### `AdminSystem_IsModerator(playerid)`

Jeder Spieler mit Moderator- oder höherer Berechtigung kann diesen Check passieren.

## 6. World-Management

`AdminSystem_CanManageWorld(playerid)` ist ein entscheidender Guard.

Er erlaubt:

- `IsPlayerAdmin(playerid)`
- `AdminSystem_IsProjectLeader(playerid)`
- `AdminSystem_IsOwner(playerid)`
- `AdminSystem_IsStateLeader(playerid)`

Das ist die Grundlage für Welt-/Staats-Operationen wie:

- `/tpstate`
- `/setstate`
- `/setstatecoord`
- `/spawnvehicle`
- `/setcityhall`
- `/setdealership`
- `/sethouse`
- `/setbusiness`

## 7. Project Leader

`AdminSystem_ActivateProjectLeader(playerid)` setzt:

- `Core_SetPlayerProjectLeader(playerid, 1)`
- `Core_SetPlayerAdminLevel(playerid, ADMIN_RANK_OWNER)`
- `AdminSystem_ResetCameraState(playerid)`

Damit wird der Spieler in die höchste Auszeichnung des Projekts gebracht, sofern er ausreichend berechtigt ist.

## 8. Admin-Commands

Diese Commands sind in `admin_system.inc` und `admin_mapping_system.inc` definiert.

### Globales Admin-Menü

- `/admin`
- Alias: `/mod`

Bedingung:

- `AdminSystem_IsModerator(playerid)` oder `AdminSystem_IsProjectLeader(playerid)`

Reaktion:

- `UiDialog_Show(playerid, DIALOG_UI_ADMIN_MENU, ...)`

### Projektleiter-Aktivierung

- `/projectlead`
- Alias: `/projectleader`

Wenn Berechtigungen passen:

- Projektleiter-Zugang aktivieren
- Kamera zurücksetzen
- Erfolgsmeldung senden

### Staatsinfo

- `/state`

Sendet nur eine kurze Mitteilung über die verfügbaren Staaten.

### State-Teleport

- `/tpstate`
- Alias: `/setstateplayer`

Erfordert `AdminSystem_CanManageWorld(playerid)`. Der Befehl verwendet `AdminSystem_ApplyStateTeleport()`.

### Konfiguration

- `/config`

Nur Moderator oder Projektleiter.

### Gangwar-Umschaltung

- `/togglewar`

Nur Moderator oder Projektleiter.

### Staats-/Weltsystem-Setzer

- `/setstate`
- `/setstatecoord`
- `/spawnvehicle`
- `/setcityhall`
- `/setdealership`
- `/sethouse`
- `/setbusiness`

Diese Funktionen sind an `CanManageWorld` gebunden.

## 9. Admin-Mapping-System

`admin/admin_mapping_system.inc` erweitert die Administrator-Funktionen um Karten- und Positionssysteme.

### Commands

- `/mapmenu`
- `/mapadd <Name> <StateID>`
- `/maptp <ID>`
- `/maplist`
- `/mapzones`
- `/mapdel <ID>`

Diese Befehle sind für globale Mapping- und Positionierungs-Tasks gedacht und arbeiten mit Sprach-/Texthilfen statt ohne UI-Dialogen.

## 10. Weitere Admin-Interfaces

### `AdminConfigSystem_ShowMenu(playerid)`

Das Modul `admin/admin_config_system.inc` zeigt ein TextDraw-basiertes Admin-Konfigurationsmenu.

Es enthält:

- Staatstitel
- State-Header
- State-Text
- Steuerinformationen
- Gangwar-Status
- Konfigurationsbefehle
- Beispiele
- Live-Hinweis

### `StateControl_UI`

`ui/state_control_ui.inc` ist ein spezieller Leitstand für das 3-Staaten-System. Er wird von State-Leadern und Admins verwendet, um Arbeits- und Transferzustände zu kontrollieren.

## 11. Sicherheits- und Rechte-Design

Die Rechte sind bewusst über mehrere Schichten verteilt:

1. Core-Level
2. Admin-Level
3. State Administration
4. Projektleiter / Owner
5. konkrete UI-Guard-Checks

Das verhindert, dass einfache UI-Klicks oder direkte Befehle ohne passende Rechte auf das System zugreifen.

## 12. Typische Rollen-Logik

Ein idealer Ablauf im Projekt ist:

- Spieler startet
- Core setzt Admin- und State-Werte zurück
- Admin-System prüft globale oder staatliche Rechte
- UI oder Befehl fragt die passende Funktion ab
- `CanManageWorld` oder `IsModerator` entscheidet über Zugriff

## 13. Fazit

Das Admin-System ist stark modular und klar aufgebaut. Es ist jedoch auch ein gutes Beispiel dafür, wie im Projekt Rechte verteilt sind:

- Globale Rechte in Core und Admin-System
- State-weise Rechte in State-Admin-Systemen
- UI- und Mapping-Logik in separaten Modulen

Diese Aufteilung ist gut für Wartung und Skalierung, aber sie erfordert die strikte Einhaltung der Core-Architektur, damit keine Parallelrechte entstehen.


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
