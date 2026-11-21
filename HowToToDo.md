# HowToToDo – Roadmap, Ablauf und Leitpfad für die nächsten System-Updates

Dieser Leitfaden beschreibt den funktionalen Ablauf des Spielstarts und die Entwicklungspipeline für die nächsten System-Updates. Er ist bewusst als Arbeits- und Roadmap-Dokument aufgebaut:

- von Register → Login → Tutorial → Spawn → Heimatstadt
- über Befehlsfluss und Moduleinbindung
- bis hin zu klaren nächsten Update-Pfaden und Prioritäten

Das Dokument basiert auf der aktuellen Source-Struktur des Projekts und dient als technische Orientierung für die nächste Umsetzung, ohne alte Informationen zu löschen.

---

## 1. Ziel des Dokuments

Das Projekt ist bereits stark modular aufgebaut. Die nächste Phase sollte nicht mehr in zufälligen Feature-Additionen bestehen, sondern in einer klaren Reihenfolge:

1. Spielstart systematisch stabilisieren
2. Login/Auth-Flow sauber durchlaufen
3. Tutorial und First-Spawn definieren
4. Heimatstadt und State-Logik korrekt verbinden
5. UI-/Command-Flow konsolidieren
6. dann Economy, Jobs, Law, Factions und World-Systeme logisch erweitern

Dieser Ablauf ist die Grundlage für die nächste Roadmap.

---

## 2. Kernfluss: Register → Login → Tutorial → Spawn → Heimatstadt

Die wesentliche Logik sitzt im Haupt-Gamemode und den Modulen:

- `gamemodes/gamemode.pwn`
- `gamemodes/modules/auth/login_system.inc`
- `gamemodes/modules/tutorial/tutorial_system.inc`
- `gamemodes/modules/state/state_spawn_system.inc`
- `gamemodes/modules/state/state_system.inc`
- `gamemodes/modules/ui/ui_hub.inc`
- `gamemodes/modules/ui/roleplay_ui_system.inc`

### 2.1 Ablaufübersicht

```text
Player connects
    ↓
Auth-Status initialisieren
    ↓
Register oder Login
    ↓
State-/Heimatstadt-Auswahl
    ↓
Tutorial-System starten
    ↓
Spawn in Staat / Stadt positionieren
    ↓
HUD aktivieren
    ↓
Spieler landet im RP-Flow
```

### 2.2 Funktionaler Sinn des Flows

Der Ablauf ist nicht nur „Willkommensbildschirm + Spawn“. Er ist die zentrale Übergangslogik zwischen:

- Identität des Spielers
- Datenpersistenz
- politische Zugehörigkeit (State)
- Lebensstart im RP-Universum
- erste Interaktion mit Jobs, UI und Welt

Wenn diese Stufen nicht sauber funktionieren, bricht der gesamte RP-Flow zusammen.

---

## 3. Register-System – erster echter Einstieg

### 3.1 Zweck

Das Register-System ist der erste Entscheidungspunkt. Es entscheidet, ob der Spieler:

- neue Identität erstellt
- bestehende Daten lädt
- das Tutorial startet
- später auf die passende State-Struktur zugreift

### 3.2 Wichtige Dateien

- `gamemodes/modules/auth/login_system.inc`
- `gamemodes/modules/core/core.inc`
- `gamemodes/modules/db/db_interface.inc`

### 3.3 Verantwortlichkeiten

Das Register-/Login-System sollte die folgenden Dinge sauber übernehmen:

- Spielerprofil prüfen
- Datenbank- oder Dini-Cache laden
- Charakterdaten anlegen oder validieren
- State- und Admin-Status prüfen
- bei neuem Charakter die Initialisierungswerte setzen
- Weiterleitung in den Tutorial-/Spawn-Prozess

### 3.4 Designziel

Das System muss klar und robust sein:

- keine „sich selbst auflösende“ Charaktererstellung
- keine verschachtelte Logik über mehrere Dateien
- eindeutige Trennung von Auth- und Spawn-Workflow

### 3.5 Entwicklungsstatus

Aktuell vorhanden, aber in der Praxis noch als modularer Grundbau zu verstehen. Die nächste Stabilisierung sollte hier stattfinden.

---

## 4. Login-System – Auth- und Datenlogik

### 4.1 Zweck

Das Login-System ist die Brücke zwischen:

- Spielerverbindung
- gespeicherten Profilen
- World-State
- Auth-Status
- Spawn-/Tutorial-Weiterleitung

### 4.2 Wichtige Punkte

Das Login-System sollte immer auf folgende drei Dinge achten:

1. Spieler ist authentifiziert
2. Spielerprofil existiert oder wird angelegt
3. Spieler wird mit gültigen State-/Profil-Daten in die Welt gebracht

### 4.3 Logische Kette

```text
Player logs in
    ↓
Datenbank / Cache laden
    ↓
Profil validieren
    ↓
State-Werte prüfen
    ↓
Admin-/Premium-/Progression-Flags laden
    ↓
Tutorial / Spawn-Weiterleitung
```

### 4.4 Was als Nächstes sauber werden sollte

- Login-Flow eindeutig dokumentieren
- fehlende Zustände vermeiden
- unvollständige Spielerprofile verhindern
- Startwerte für neue Spieler definieren
- Standard-State-Zuweisung im Falle eines Fehlers sicher setzen

---

## 5. Tutorial-System – erster RP-Einstieg

### 5.1 Zweck

Das Tutorial ist der Übergang von „Account aktiv“ zu „ich bin in der Welt angekommen“. Es erklärt:

- Zustände/Regierungen
- Spielwelt und UI
- Jobs und Verdienst
- grundlegende Commands
- erste RP-Interaktion

### 5.2 Wichtige Dateien

- `gamemodes/modules/tutorial/tutorial_system.inc`
- `gamemodes/modules/camera/camera_system.inc`
- `gamemodes/modules/ui/roleplay_ui_system.inc`

### 5.3 Ablauf

```text
Tutorial startet
    ↓
Intro / Begrüßung
    ↓
Heimatstaat erklärt
    ↓
Basis-UI erklärt
    ↓
Commands demonstriert
    ↓
Erste Zielsetzung / erste RP-Aktion
    ↓
Tutorial beendet / Spawn aktiv
```

### 5.4 Was der Tutorial-Flow leisten muss

- kein bloßes Textfenster ohne Funktion
- keine reine reaktive Info-Box
- echte Übergangshilfe zum späteren RP-Flow

### 5.5 Best Practice für die nächsten Updates

Tutorial als zentrale Onboarding-Bridge behandeln:

- Login -> Tutorial -> Spawn
- Tutorial -> UI-Hub -> JobCenter -> World
- Tutorial muss keine „Nebenfunktion“ sein, sondern hat systemische Priorität

---

## 6. Spawn-System – Übergang in die Heimatstadt

### 6.1 Zweck

Der Spawn ist die eigentliche „Welt-Eingangsstufe“. Nach Tutorial oder Login darf der Spieler nicht mehr einfach irgendwo im Nirgendwo erscheinen, sondern an einem sinnvollen, statebezogenen Ort.

### 6.2 Wichtige Dateien

- `gamemodes/modules/state/state_spawn_system.inc`
- `gamemodes/modules/state/state_system.inc`
- `gamemodes/modules/auth/login_system.inc`
- `gamemodes/modules/camera/camera_system.inc`

### 6.3 Ziel

Die Heimatstadt muss logisch mit den folgenden Faktoren verbunden sein:

- Spieler-Home-State
- State-Profildaten
- Stadt/Oberfläche und Spawnzone
- City Hall / Verwaltung
- Job- und Economy-Standort

### 6.4 Richtiger Ablauf

```text
State ausgewählt
    ↓
Standard-Spawn für State geladen
    ↓
Heimatstadt etabliert
    ↓
Player spawn positionieren
    ↓
HUD / Dashboard aktivieren
    ↓
erste Interaktionsmöglichkeiten freischalten
```

### 6.5 Wichtig für die nächste Entwicklungsstufe

- kein Spawn ohne gültigen State
- kein Spawn ohne passende Position
- keine Heimatstadt ohne zugehörige Verwaltung/World-Identität

---

## 7. Heimatstadt – der echte Startpunkt in die RP-Welt

### 7.1 Definition

Die Heimatstadt ist nicht nur ein Spawnpunkt, sondern der erste echte RP-Ort. Der Spieler erlebt hier:

- seine Stadtidentität
- lokale Wirtschaft
- Jobs und Shops
- erste Interaktionen mit Behörden / Fraktionen / Services
- das Gefühl, Teil des Staates zu sein

### 7.2 Kernmodule

- `state/state_system.inc`
- `government/government_system.inc`
- `government/city_hall_system.inc`
- `state/state_services.inc`
- `state/district_system.inc`
- `economy/city_economy_system.inc`
- `ui/ui_hub.inc`

### 7.3 Was hier erreicht werden muss

- Spieler kennt seinen Staat
- Spieler kann schnell navigieren
- Spieler findet Jobs, Shops, Services
- Spieler erkennt, wo er sich in der politischen Struktur befindet

### 7.4 Qualitätssignal

Wenn der Spieler nach dem Spawn sofort versteht:

- wo er ist
- was der Staat bedeutet
- welche UI- und Command-Optionen ihn erwarten

Dann ist der Startbereich richtig aufgebaut.

---

## 8. Command-Flow – wie der Spieler in die Welt gelangt

Der Befehlsfluss läuft über `OnPlayerCommandText()` in der main file:

- `gamemodes/gamemode.pwn`

Wichtige Erkennungen:

- kein zcmd/YCMD-Framework
- `OnPlayerCommandText` entscheidet, welcher Handler greift
- erster Handler, der `1` zurückliefert, gewinnt
- spätere Duplicate sind oft verdeckt oder nicht erreichbar

### 8.1 Wichtigster Befehlspfad

```text
OnPlayerCommandText
    ↓
RoleplayUI_HandleCommand
    ↓
UiHub_HandleCommand
    ↓
TextWars_HandleCommand
    ↓
AdminSystem_HandleCommand
    ↓
Emergency / Garage / Weapon / State Event / Social
    ↓
RoleplayCommandSystem / Inline-Commands
```

### 8.2 Warum das für die Roadmap wichtig ist

Die Command-Kette zeigt, dass das Projekt bewusst strukturiert ist:

- UI und Hub haben hohe Priorität
- Verwaltung und World-Commands sind nachgeordnete Einheiten
- einige frühere Handler verdecken spätere Befehle
- ein direkter, sauberer Startpfad ist für das Onboarding entscheidend

### 8.3 Praktische Entwicklungsvorschrift

Bei neuen Befehlen immer prüfen:

- gehört er in `ui_hub`?
- gehört er in `social`?
- ist er staatlich oder adminseitig?
- ist er direkt im `gamemode.pwn` inline richtig?
- oder muss er in ein eigenes Modul ausgelagert werden?

---

## 9. Modul-Aufbau – die Systemlogik hinter dem Spielstart

Der Gamemode nutzt klare Module mit klarer fachlicher Trennung.

### 9.1 Grundsatzmodule

- `core/core.inc` – Basiswerte, lifecycle, zentrale Daten
- `db/db_interface.inc` – Datenpersistenz und Caches
- `state/state_system.inc` – State/Heimatstadt/Politik
- `admin/admin_system.inc` – Rechte und Verwaltung
- `ui/ui_hub.inc` – zentrale Menü- und Dialoglogik
- `ui/roleplay_ui_system.inc` – HUD/Dashboard

### 9.2 Onboarding-Module

- `auth/login_system.inc`
- `tutorial/tutorial_system.inc`
- `camera/camera_system.inc`
- `state/state_spawn_system.inc`

### 9.3 Folge-Module für die nächste Entwicklungsphase

- `economy/*.inc`
- `jobs/*.inc`
- `law/*.inc`
- `crime/crime_system.inc`
- `emergency/*.inc`
- `factions/*.inc`
- `housing/*.inc`
- `inventory/*.inc`
- `vehicles/*.inc`

### 9.4 Die richtige Reihenfolge

Die Initialisierung in `OnGameModeInit()` zeigt die Entwicklungslogik:

```text
Core → DB → State → Admin → Auth → Tutorial → Camera
    ↓
Government → Housing → Business → Factions → Vehicles
    ↓
Law → Emergency → Jobs → Economy → UI → World / Events / Premium
```

Das ist die Reihenfolge, an der man die nächsten System-Updates orientieren sollte.

---

## 10. Roadmap / Leitpfaden für nächste System-Updates

### Phase 1: Stabiler Startpfad

Ziel:
- Register und Login sauber und ohne Unklarheit
- Tutorial klar und verständlich
- Spawn mit korrektem State und Heimatstadt
- erste HUD-/UI-Ansicht zuverlässig aktiv

Umsetzung:
- `login_system.inc` prüfen
- `tutorial_system.inc` präzisieren
- `state_spawn_system.inc` mit Heimatstadt verknüpfen
- `ui/roleplay_ui_system.inc` mit Start-Status verbinden
- fehlende Übergänge zwischen Login und Spawn schließen

Definition of Done:
- Spieler kann sich registrieren oder einloggen
- Spieler landet in korrekter State-Stadt
- Tutorial startet standardisiert
- HUD zeigt relevante Grundwerte

---

### Phase 2: UI- und Command-Flow konsolidieren

Ziel:
- zentrale Befehlsstruktur sauber halten
- Menü- und Dialogbarkeit vereinheitlichen
- keine verdeckten/duplizierten Commands

Umsetzung:
- `OnPlayerCommandText` auditieren
- doppelte Befehle identifizieren und konsolidieren
- `ui_hub` als zentrale Frontend-Schicht stärken
- `/menu`, `/hud`, `/help`, `/spawn`, `/jobs` und verwandte Wege sauber verbinden

Definition of Done:
- Befehlsfluss ist nachvollziehbar
- weniger Überlappung
- Player erlebt klaren Einstieg in das Menü- und State-System

---

### Phase 3: Wirtschaft & Progression starten

Ziel:
- Geld, Bank, Progression und Jobs in den normalen Flow integrieren

Umsetzung:
- `economy`-Module mit Startwerten verbinden
- `progression/progression_system.inc` klar auf Level/XP/Rep ausrichten
- `job_center` und `jobs_core` an die Startlogik anbinden
- Reward- und Lohnsystem sauber validieren

Definition of Done:
- Spieler verdient nach Start sinnvoll Geld
- Progression ist nachvollziehbar
- Jobs sind in der Hauptwelt erreichbar

---

### Phase 4: Law, Emergency und Ordnung

Ziel:
- Polizei, Jail, Wanted, Crime und EMS funktionieren als kohärentes System

Umsetzung:
- `law/law_system.inc`
- `law/jail_system.inc`
- `crime/crime_system.inc`
- `emergency/emergency_system.inc`
- `dispatch_system.inc`

Definition of Done:
- Wanted- und Arrestlogik sind nachvollziehbar
- EMS/Polizei-Systeme sind in den RP-Flow einbezogen
- Reaktionen auf Verbrechen sind logisch und sichtbar

---

### Phase 5: World, Fraktionen und Besitzsysteme

Ziel:
- Fraktionen, Territorien, Häuser, Fahrzeuge, Inventar und Businesses bilden eine konsistente Welt

Umsetzung:
- `factions/*.inc`
- `housing/*.inc`
- `inventory/*.inc`
- `vehicles/*.inc`
- `business/business_system.inc`
- `world/territory_system.inc`

Definition of Done:
- Besitz und Eigentümer sind klar
- Fraktionen haben echte Weltwirkung
- Fahrzeuge, Häuser und Inventar sind in den Hauptspielpfad eingebunden

---

### Phase 6: Events, Season und Long-Term-World

Ziel:
- langfristige Dynamik und saisonale Spielwelt einbauen

Umsetzung:
- `events/state_event_system.inc`
- `events/season_system.inc`
- `web/web_bridge.inc`
- `marketing/ad_system.inc`

Definition of Done:
- Welt wird nicht statisch wahrgenommen
- Events und Saisons haben klare Trigger
- Spielwelt bleibt langfristig lebendig

---

## 11. Was als Nächstes wirklich priorisiert werden sollte

Wenn man die nächsten Updates strategisch planen will, dann sollte die Reihenfolge so aussehen:

### Priorität A – Startsysteme
- Register
- Login
- State-Auswahl
- Tutorial
- Spawn
- Heimatstadt
- HUD-/UI-Grundflow

### Priorität B – Grundspielerfluss
- Jobs
- Geld / Bank
- Progression
- Shops / Economy / Business

### Priorität C – Ordnung und Kontrolle
- Law
- Police / EMS
- Jail
- Wanted / Crime

### Priorität D – Welt und Einfluss
- Fraktionen
- Territorien
- Fahrzeuge
- Häuser
- Inventar

### Priorität E – Dynamik
- Events
- Seasons
- UI-Polish
- langfristige Welt-Trigger

---

## 12. Kurz-Entscheidungslogik für die Umsetzung

Damit die nächste Phase nicht unkoordiniert wird, sollte jede neue System-Erweiterung immer anhand dieser Fragen bewertet werden:

1. Ist das System für den Startpfad nötig?
2. Ist es direkt für Login / Tutorial / Spawn / Heimatstadt wichtig?
3. Ist es in `ui_hub` oder im Command-Flow erreichbar?
4. Ist es mit State-/Player-Profil-Daten verbunden?
5. Gibt es eine klare Datenpersistenz?
6. Ist das Modul bereits im `OnGameModeInit`-Initialisierungsfluss vorbereitet?
7. Ist der Player-Flow nach dem Update verständlich und nicht fragmentiert?

Wenn eine Antwort „nein“ ist, dann ist das System wahrscheinlich noch nicht in der richtigen Priorität.

---

## 13. Empfehlung für den nächsten offiziellen Update-Zyklus

Der sinnvollste nächste Zyklus wäre:

### Zyklus 1: „Onboarding & Start sauber machen“
- Register/Login klären
- Tutorial verfeinern
- Spawn an State/Heimatstadt koppeln
- HUD und UI-Startmenü konsolidieren
- Befehlsfluss prüfen und bereinigen

### Zyklus 2: „RP-Grundloop verankern“
- Jobs
- Cash/Bank
- Progression
- Shops
- erste Weltinteraktionen

### Zyklus 3: „Verwaltung & Ordnung“
- Law
- Emergency
- Police/EMS
- Wanted
- Jail

### Zyklus 4: „Welt & Einfluss“
- Fraktionen
- Territorien
- Houses
- Vehicles
- Inventory

### Zyklus 5: „Lebendige Welt“
- Events
- Season
- UI-Polish
- Performance und Stabilität

---

## 14. Gesamtfazit

Die zentrale Wahrheit des Projekts ist diese:

Das wichtigste System ist nicht ein einzelnes Feature, sondern der komplette Übergang vom Login über den Tutorial bis zur Heimatstadt und dem ersten echten RP-Start.

Diese Abfolge ist die richtige Leitlinie für die nächsten Updates. Wenn der Startpfad sauber funktioniert, lassen sich alle folgenden Systeme logisch aufbauen:

- Economy
- Jobs
- Law
- Faction
- World
- Vehicle
- Housing
- Inventory
- Events

Kurz gesagt:

Register → Login → Tutorial → Spawn → Heimatstadt

ist nicht nur der Spielstart, sondern die Grundlage der zukünftigen Architektur- und Systementwicklung.

---

## 15. Verweise zu den wichtigsten Dateien

- `gamemodes/gamemode.pwn`
- `gamemodes/modules/auth/login_system.inc`
- `gamemodes/modules/tutorial/tutorial_system.inc`
- `gamemodes/modules/state/state_spawn_system.inc`
- `gamemodes/modules/state/state_system.inc`
- `gamemodes/modules/ui/ui_hub.inc`
- `gamemodes/modules/ui/roleplay_ui_system.inc`
- `gamemodes/modules/core/core.inc`
- `gamemodes/modules/db/db_interface.inc`
- `COMMANDS.md`
- `MODULES.md`
- `PROJECT_COMPLETE_MASTER.md`

Die nächste Implementierungsphase sollte immer an diesem Leitpfad ausgerichtet werden.
