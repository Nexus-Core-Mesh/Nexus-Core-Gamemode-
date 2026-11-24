# Nexus Core – Implementation Blueprint

Dieses Dokument ist der detaillierte Umsetzungsplan für die nächsten System-Updates. Es beschreibt nicht nur den Ablauf, sondern die konkreten Aufgaben, Abhängigkeiten und Ziele pro Entwicklungsphase.

---

## 1. Grundprinzip

Die Architektur ist modular. Die Abhängigkeiten sind klar. Deshalb sollte die Umsetzung in Phasen erfolgen, keine Features breit verteilt und unkoordiniert ergänzen.

Die Kernlogik bleibt:

Register → Login → Tutorial → Spawn → Heimatstadt → RP-Loop

---

## 2. Phase 1 – Startpfad finalisieren

### Ziel

Die erste Spielzeit muss klar und stabil sein.

### Aufgaben

#### 2.1 Register-/Login-Flow
- Prüfen, ob neue Spieler korrekt angelegt werden
- sicherstellen, dass bestehende Spielerprofil-Daten sauber geladen werden
- Standardwerte für neue Accounts definieren
- State-/Admin-/Premium-Flags korrekt setzen

#### 2.2 Tutorial-Flow
- einen klaren Übergang vom Login zum Tutorial herstellen
- Beschreibung der ersten RP-Grundlagen ergänzen
- UI-Elemente und Übergänge sauber verknüpfen
- Abschluss des Tutorials mit korrektem Spawn verbinden

#### 2.3 Spawn-System
- genaue Zuordnung von State zu Heimatstadt
- Positionen und Spawn-Locations klar definieren
- nicht nur zufällige Spawns, sondern logische Startbereiche

#### 2.4 HUD-/UI-Start
- Dashboard nach dem Spawn aktivieren
- Grundinformationen direkt sichtbar machen
- erste Befehls- und Menü-Optionen klar erreichbar machen

### Abhängigkeiten

- `auth/login_system.inc`
- `tutorial/tutorial_system.inc`
- `camera/camera_system.inc`
- `state/state_spawn_system.inc`
- `state/state_system.inc`
- `ui/roleplay_ui_system.inc`
- `ui/ui_hub.inc`

### Acceptance Criteria

- Neuer Spieler kann registrieren
- vorhandener Spieler kann einloggen
- Startort ist statebezogen
- Tutorial endet an definierter Stelle
- HUD zeigt die wichtigsten Werte direkt nach dem Spawn

---

## 3. Phase 2 – Grundspiel-Loop verankern

### Ziel

Der Spieler soll verstehen, wie er verdienen, interagieren und sich im RP-System positionieren kann.

### Aufgaben

#### 3.1 Jobs ausbauen
- JobCenter in den Startfluss einbinden
- State- und Job-Logik sauber kombinieren
- Aufgaben und Belohnungen klar definieren
- Lohn-/Progression-Logik formalisieren

#### 3.2 Economy
- Cash, Bank und Staatskasse sauber über alle Bereiche verbinden
- Preise und Ausgaben logisch auf State- oder City-Ebene ausrichten
- Revenue-/Steuerlogik nachvollziehbar dokumentieren

#### 3.3 Progression
- XP, Level, Reputation und Aktionsschritte klar definieren
- Fortschritt und Belohnung mit Jobs/Economy verknüpfen
- Status- und UI-Zeiten konsistent anzeigen

#### 3.4 Shops / Business
- Shop-System und Business-Flow validieren
- erste nutzbare Interaktionspunkte definieren
- State- und City-bezogene Preise integrieren

### Abhängigkeiten

- `jobs/*.inc`
- `economy/*.inc`
- `business/business_system.inc`
- `progression/progression_system.inc`
- `shops/*.inc`
- `commerce/marketplace_system.inc`

### Acceptance Criteria

- Spieler kann Jobs annehmen und verstehen
- Spieler erhält klare Belohnungen
- Geld-/Banksystem ist nachvollziehbar
- Progression ist sichtbar und auf dem Startpfad verankert

---

## 4. Phase 3 – Order, Rules und Sicherheit

### Ziel

Die Welt braucht klare Regeln, Reaktionen und Verantwortlichkeit.

### Aufgaben

#### 4.1 Law-System
- Wanted-System mit klaren Regeln
- Arrest-, Straf- und Remission-Mechaniken definieren
- Gefängnis- und Prozesspfad verknüpfen

#### 4.2 Crime-System
- Verbrechensarten, Heat, Warnstufen und verzögerte Reaktion dokumentieren
- relevante UI-Anzeige ergänzen

#### 4.3 Emergency
- Polizei/EMS-Interface sauber verknüpfen
- Dispatch-Flow definieren
- Notfallreaktionen verlässlich organisieren

### Abhängigkeiten

- `law/law_system.inc`
- `law/jail_system.inc`
- `crime/crime_system.inc`
- `emergency/emergency_system.inc`
- `emergency/dispatch_system.inc`
- `justice/*.inc`

### Acceptance Criteria

- Gesetzes- und Sicherheitslogik ist nachvollziehbar
- Wanted-System funktioniert innerhalb des RP-Systems
- Police / EMS / Dispatch sind klar erreichbar
- Verbrechen erzeugen sichtbare Konsequenzen

---

## 5. Phase 4 – Welt, Besitztum und Einfluss

### Ziel

Die ganze Welt muss als konsistente, interaktive Umgebung wirken.

### Aufgaben

#### 5.1 Factions
- Fraktionslogik sauber mit dem State-System verbinden
- Gang-/Faction-Influence sichtbar machen
- Gruppen- und Territorial-Mechanik definieren

#### 5.2 Territories
- Territorien und Einflusszonen logisch verknüpfen
- Kampf-/Kriegs-/Gebietslogik an UI/Dashboard anbinden

#### 5.3 Housing / Properties
- Häuser, Eigentum und Zugriff realisieren
- Stadt-/State-Bindung an Immobilien ergänzen

#### 5.4 Vehicles
- Fahrzeugbesitz, Garage, Tuning und Spawn verknüpfen
- Fahrzeuge als Teil der wirtschaftlichen Welt behandeln

#### 5.5 Inventory
- Inventar- und Item-Logik mit Besitz, Jobs und Shops verbinden
- sichtbare Konsistenz bei Ausrüstung und Nutzung

### Abhängigkeiten

- `factions/*.inc`
- `world/*.inc`
- `housing/*.inc`
- `vehicles/*.inc`
- `inventory/*.inc`
- `business/business_system.inc`

### Acceptance Criteria

- Besitz ist klar nachvollziehbar
- Fraktionen haben echte Weltwirkung
- Fahrzeuge sind im normalen Spielpfad nutzbar
- Inventar ist mit Jobs, Besitz und Shops konsistent

---

## 6. Phase 5 – Dynamik und Langzeitwelt

### Ziel

Die Welt soll langfristig lebendig wirken.

### Aufgaben

#### 6.1 Events
- Event- und State-Events verknüpfen
- Spieler-Trigger und Event-Belohnungen definieren

#### 6.2 Season-System
- Saisonale Logik und zeitliche Weltänderungen einbauen
- UI der Saison korrekt darstellen

#### 6.3 UI-Polish
- gleiche Bedienung und visuelle Hülle für alle Kernbereiche
- Start-/HUD-/Hub-/Dialogsysteme vereinheitlichen

#### 6.4 Long-Term-Progression
- Premium-/Season-/Event-Logik als erweitertes Langzeitfeature einbauen

### Abhängigkeiten

- `events/*.inc`
- `events/season_system.inc`
- `web/web_bridge.inc`
- `premium/premium_system.inc`
- `ui/*.inc`

### Acceptance Criteria

- Events sind künftig explizit nutzbar
- Saisonlogik ist nachvollziehbar und nicht „statisch“
- Welt wirkt nicht nur als Start-Umgebung, sondern als lebendige Umgebung

---

## 7. Cross-Cutting Tasks

Diese Aufgaben gelten für alle Phasen:

- Architektur-Checks auf zentrale Module
- Befehls- und UI-Überprüfung
- Datenpersistenz validieren
- State-/Player-Werte konsistent halten
- uneingehängte Module als „vorbereitet“ markieren
- keine unklaren Spawns oder untestbaren Features einbauen

---

## 8. Entscheidungsregeln für die Umsetzung

Vor jedem neuen Update sollte geprüft werden:

1. Ist das System Teil des Startpfads?
2. Ist es direkt für Spielerinteraktion wichtig?
3. Ist es mit State-/Player-Daten verknüpft?
4. Ist es in `gamemode.pwn` und den Modulen korrekt eingebunden?
5. Ist der UI-/Command-Flow klar und nicht überladen?
6. Gibt es eine sinnvolle Persistenz- oder Datenstruktur?
7. Kann der Fortschritt im nächsten Spieltest nachvollzogen werden?

Wenn mehrere dieser Fragen negativ sind, muss das Feature eher nach hinten verschoben werden.

---

## 9. Empfohlene Reihenfolge der nächsten PRs / Update-Serien

### Update-Serie 1: Start- und Onboarding
- Login/Register
- Tutorial
- Spawn
- Heimatstadt
- HUD-/UI-Start

### Update-Serie 2: Grundeconomy
- Jobs
- Progression
- Cash/Bank
- Shops/Business

### Update-Serie 3: Ordnung und Welt
- Law
- Crime
- Emergency
- Dispatch

### Update-Serie 4: Interaktion und Besitz
- Factions
- Territories
- Houses
- Vehicles
- Inventory

### Update-Serie 5: Lebenswelt
- Events
- Seasons
- UI-Polish
- Premium / Long-Term-Systems

---

## 10. Kurzfazit

Die Umsetzung muss vom Spielerstart zum ganzen RP-Lebenszyklus laufen. Nicht irgendein Feature ist wichtig, sondern der koordinierte Aufbau einer vollständigen, konsistenten Welt.

Die Reihenfolge lautet:

Startsysteme → Grundspiel → Ordnung → Welt → Dynamik

Wenn diese Logik eingehalten wird, bleibt der Gamemode gut strukturiert, testbar und erweiterbar.

---

## 11. Verweise

- `gamemodes/gamemode.pwn`
- `gamemodes/modules/core/core.inc`
- `gamemodes/modules/auth/login_system.inc`
- `gamemodes/modules/tutorial/tutorial_system.inc`
- `gamemodes/modules/state/state_spawn_system.inc`
- `gamemodes/modules/ui/ui_hub.inc`
- `gamemodes/modules/ui/roleplay_ui_system.inc`
- `HowToToDo.md`
- `ROADMAP_DEVELOPER.md`


---

## Aktualisierter Stand (2026-11-24)

- Registrierungsflow: Passwort-Gate vor Tutorial/Spawn, M/W-Auswahl vor Skin-Picker, `DIALOG_UI_GENDER_PICK` aktiv.
- Tutorial: `TUTORIAL_SCENE_TIME` auf 11500 ms, Schutz gegen Start ohne Passwort/Geschlecht.
- Skin-Flow: `LoginSystem_ShowSkinPicker()` zeigt geschlechtsabhängige 10–15 Skins, validiert `0..311`, setzt `SetPlayerSkin` vor finalem Spawn.
- Spawn-Order: Home-State/State validiert, `StateSpawnSystem_Apply(...)` erst nach Skin-Setzung, dann `CameraSystem_ExitCameraMode`, `SetCameraBehindPlayer`, `TogglePlayerControllable(playerid, true)`, `RoleplayUI_ShowDashboard(playerid)`.
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut (nur legacy warning in `includes/dini.inc`).
- Dini-first + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

