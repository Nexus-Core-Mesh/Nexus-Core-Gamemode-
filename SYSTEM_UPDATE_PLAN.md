# Nexus Core – System Update Plan

Dieses Dokument ist ein praktischer Arbeitsplan für die nächsten System-Updates. Es ist bewusst in konkrete Aufgaben, Verantwortlichkeiten und Reihenfolge gegliedert, damit das Projekt nicht nur strukturiert, sondern auch umsetzbar bleibt.

---

## 1. Ziel des Plans

Der Gamemode ist bereits modular aufgebaut. Die nächsten Updates müssen in klaren, prüfbaren Schritten erfolgen:

1. Startpfad stabilisieren
2. Auth / Tutorial / Spawn / Heimatstadt klar machen
3. UI und Commands vereinheitlichen
4. Wirtschaft und Jobs ausbauen
5. Law / Emergency / World ergänzen
6. Events / Season / Langzeitdynamik erweitern

---

## 2. Arbeitsprinzipien

- nie ohne klaren Startpfad erweitern
- keine neuen Features ohne Verbindung zum State-/Player-Flow
- jede System-Erweiterung muss in `gamemode.pwn` und Modul-Logik sauber verlinkt sein
- UI und Commands immer als zentrale Zugriffsschicht behandeln
- „vorbereitet“ und „aktiv“ müssen klar getrennt sein

---

## 3. Arbeitsreihenfolge

### Phase 1 – Onboarding und Start

#### Aufgaben

- Register-/Login-Flow finalisieren
- Tutorial weiter ausbauen
- Spawn-Logik im State-Flow absichern
- Heimatstadt als sichtbaren RP-Startpunkt festlegen
- HUD und erste Menülogik stabilisieren

#### Kernmodule

- `auth/login_system.inc`
- `tutorial/tutorial_system.inc`
- `camera/camera_system.inc`
- `state/state_spawn_system.inc`
- `state/state_system.inc`
- `ui/ui_hub.inc`
- `ui/roleplay_ui_system.inc`

#### Ergebnis

Der neue Spieler erkennt sofort:
- wer er ist
- wo er sich befindet
- was der Staat bedeutet
- welche Grundsysteme ihn erwarten

---

### Phase 2 – Grundspiel und Progression

#### Aufgaben

- JobCenter und Job-Logik weiter vernetzen
- Economy als sichtbaren Progressionsmotor etablieren
- Cash / Bank / Player-Stats verknüpfen
- Progression sichtbar machen
- Shops, Business und erste Kaufwege integrieren

#### Kernmodule

- `jobs/*.inc`
- `economy/*.inc`
- `business/business_system.inc`
- `progression/progression_system.inc`
- `shops/*.inc`
- `commerce/marketplace_system.inc`

#### Ergebnis

Der Spieler versteht, wie er verdient, spart, investiert und im RP-System aufsteigt.

---

### Phase 3 – Ordnung und Verhaltenslogik

#### Aufgaben

- Wanted-System sauber definieren
- Jail/Prison-Mechaniken vernetzen
- Crime-/Heat-System sichtbar machen
- Police / EMS / Dispatch ausbauen
- Spielerkonflikte in klaren Regeln darstellen

#### Kernmodule

- `law/law_system.inc`
- `law/jail_system.inc`
- `crime/crime_system.inc`
- `emergency/*.inc`
- `justice/*.inc`
- `dispatch_system.inc`

#### Ergebnis

Die Welt bekommt klare Konsequenzen und strukturierte soziale Regeln.

---

### Phase 4 – Besitz, Fraktionen und Welt

#### Aufgaben

- Fraktionssystem sichtbar und nutzbar machen
- Territorien und Einflusszonen verknüpfen
- Häuser, Garagen, Inventar und Fahrzeuge verbinden
- Besitz als echte Weltmechanik etablieren

#### Kernmodule

- `factions/*.inc`
- `world/*.inc`
- `housing/*.inc`
- `inventory/*.inc`
- `vehicles/*.inc`
- `business/business_system.inc`

#### Ergebnis

Die Welt wirkt nicht mehr wie leere Umgebung, sondern als echte, bewohnbare und strategische RP-Welt.

---

### Phase 5 – Dynamische Welt und Langzeitspiel

#### Aufgaben

- Events und State-Events sauber verknüpfen
- Season-Logik in den Spielalltag einbinden
- UI-Polish und World-Feedback ergänzen
- Premium- und Long-Term-Systeme einbauen

#### Kernmodule

- `events/*.inc`
- `events/season_system.inc`
- `web/web_bridge.inc`
- `premium/premium_system.inc`
- `ui/*.inc`

#### Ergebnis

Die Welt bleibt langfristig lebendig und verändert sich über Zeit hinweg.

---

## 4. Konkrete Task-Liste pro Bereich

### 4.1 Start-/Onboarding-Tasks

- [ ] Login-Flow prüfen und auf bereits vorhandene Profilwerte abstimmen
- [ ] Register-Flow mit State-Zuweisung verknüpfen
- [ ] Tutorial-Phasen definieren und auf tatsächlichen Ablauf abgleichen
- [ ] Spawn in Heimatstadt verifizieren
- [ ] HUD nach Tutorial/Spawn korrekt starten
- [ ] erste UI-Hub-Links prüfen und konsolidieren

### 4.2 Economy- und Job-Tasks

- [ ] JobCenter im Startpfad prüfen
- [ ] Lohnsystem mit State- und Progression-Logik verbinden
- [ ] Cash-/Bank-Flow klar ausarbeiten
- [ ] Shops und Business an Economy anbinden
- [ ] Progressionswerte sichtbar machen

### 4.3 Law-/Emergency-Tasks

- [ ] Wanted-System validieren
- [ ] Jail-/Prison-Logik sichtbar und nutzbar machen
- [ ] Police- und EMS-Flow prüfen
- [ ] Dispatch-Aktivitäten an das Spieler- und State-System koppeln
- [ ] Crime- und Heat-Tracker sauber anzeigen

### 4.4 World-/Faction-Tasks

- [ ] Fraktionen auf State-/Territory-Logik abgleichen
- [ ] Besitzsysteme verknüpfen
- [ ] Häuser und Garagen in Start- und RP-Loop einbauen
- [ ] Inventar logischer mit Nutzung und Besitz verbinden
- [ ] Fahrzeuge als echtes Besitz- und Nutzungssystem nutzen

### 4.5 Dynamic-World-Tasks

- [ ] Event-Trigger definieren
- [ ] Season-System auf relevante Weltwerte koppeln
- [ ] UI-Feedback und Dialoge vereinheitlichen
- [ ] Langzeit-Mechaniken tune und stabilisieren

---

## 5. Task-Qualitätsregeln

Jede Aufgabe sollte immer mit diesen Fragen bewertet werden:

- Ist die Funktion für den Startpfad relevant?
- Ist die Aufgabe mit State-/Player-Daten verknüpft?
- Ist sie direkt im `gamemode.pwn`- oder Modul-Flow erreichbar?
- Ist sie für eine spätere Interaktion gut nachvollziehbar?
- Hat sie klare UI-/Command-Interaktion?

Nur wenn diese Punkte erfüllt sind, zählt die Aufgabe als echte System-Verbesserung.

---

## 6. Priorisierte Umsetzungsreihenfolge

### Priorität 1
- Login/Register
- Tutorial
- Spawn
- Heimatstadt
- HUD / UI-Start

### Priorität 2
- Jobs
- Economy
- Progression
- Shops/Business

### Priorität 3
- Law
- Emergency
- Crime
- Dispatch

### Priorität 4
- Fraktionen
- Territories
- Housing
- Vehicles
- Inventory

### Priorität 5
- Events
- Season
- Premium / Long-Term-Systeme

---

## 7. Empfehlungen für die nächste Arbeitsrunde

Die nächste Runde sollte nicht mit einem „großen Feature“ beginnen, sondern mit dem Startsystem:

- Login/Register sauber und sichtbar
- Tutorial verständlich und vollständig
- Spawn/Heimatstadt korrekt und logisch
- UI-Flow stabil

Danach folgt die gemeinsame Wirtschaft und Job-Verankerung. Erst danach sollten Law, Faction und World-Features weiter ausgebaut werden.

---

## 8. Gesamtfazit

Das Projekt ist bereits in einer guten Bauphase. Die nächsten Updates müssen sich auf den richtigen Kernpfad orientieren:

Start → Grundspiel → Regeln → Welt → Dynamik

Wenn diese Reihenfolge eingehalten wird, bleibt das Projekt stabil, umsetzbar und langfristig erweiterbar.

---

## 9. Verweise

- `gamemodes/gamemode.pwn`
- `gamemodes/modules/auth/login_system.inc`
- `gamemodes/modules/tutorial/tutorial_system.inc`
- `gamemodes/modules/state/state_spawn_system.inc`
- `gamemodes/modules/ui/ui_hub.inc`
- `gamemodes/modules/ui/roleplay_ui_system.inc`
- `HowToToDo.md`
- `ROADMAP_DEVELOPER.md`
- `IMPLEMENTATION_BLUEPRINT.md`


---

## Aktualisierter Stand (2026-11-24)

- Registrierungsflow: Passwort-Gate vor Tutorial/Spawn, M/W-Auswahl vor Skin-Picker, `DIALOG_UI_GENDER_PICK` aktiv.
- Tutorial: `TUTORIAL_SCENE_TIME` auf 11500 ms, Schutz gegen Start ohne Passwort/Geschlecht.
- Skin-Flow: `LoginSystem_ShowSkinPicker()` zeigt geschlechtsabhängige 10–15 Skins, validiert `0..311`, setzt `SetPlayerSkin` vor finalem Spawn.
- Spawn-Order: Home-State/State validiert, `StateSpawnSystem_Apply(...)` erst nach Skin-Setzung, dann `CameraSystem_ExitCameraMode`, `SetCameraBehindPlayer`, `TogglePlayerControllable(playerid, true)`, `RoleplayUI_ShowDashboard(playerid)`.
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut (nur legacy warning in `includes/dini.inc`).
- Dini-first + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

