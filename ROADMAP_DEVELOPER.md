# Nexus Core – Developer Roadmap

Dieses Dokument dient als kompakte Entwicklungsvorlage für die nächsten System-Updates. Es fokussiert sich auf die reale Reihenfolge, in der das Projekt stabil und sinnvoll weitergebaut werden kann.

## 1. Ziel

Das Projekt ist bereits stark modular aufgebaut. Die nächste Phase sollte nicht durch „Feature-Sammeln“, sondern durch eine klare Reihenfolge erfolgen:

1. Startpfad stabilisieren
2. Auth-/Login-/Tutorial-/Spawn-Flow verifizieren
3. State- und Heimatstadt-Logik korrekt verknüpfen
4. UI-/Command-Flow vereinheitlichen
5. Economy und Jobs ausbauen
6. Law, Emergency, Factions und World-Systems ergänzen
7. Events, Season und Long-Term-Dynamik erweitern

---

## 2. Kernpfad: Register → Login → Tutorial → Spawn → Heimatstadt

Das ist der wichtigste Leitpfaden für alle aktuellen und zukünftigen Updates.

```text
Connect
  → Register/Login
  → State- und Heimatstadt-Auswahl
  → Tutorial
  → Spawn
  → HUD aktiv
  → RP-Loop beginnt
```

Diese Reihenfolge ist die fundamentale Ausgangsbasis. Wenn dieser Pfad nicht sauber funktioniert, wirkt jede spätere Erweiterung instabil.

---

## 3. Prioritätsstufen

### Priorität A – Startsysteme

- Register-System
- Login-System
- State-Auswahl
- Tutorial
- Spawn-System
- Heimatstadt-Logik
- HUD / UI-Startflow

Ziel: Der Spieler soll ohne Unsicherheit in die Welt kommen.

### Priorität B – Grundspiel

- Jobs
- Economy
- Progression
- Shops
- Business
- Bank / Cash-Flow

Ziel: Spieler kann verstehen, wie er verdienen und sich weiterentwickeln kann.

### Priorität C – Ordnung und Sicherheitslogik

- Law
- Wanted
- Jail
- Crime
- Police / EMS
- Dispatch

Ziel: Die Welt bekommt Regeln, Reaktionen und Spielmechanik.

### Priorität D – Welt und Einfluss

- Factions
- Territories
- Housing
- Vehicles
- Inventory
- Business- und Besitz-Modelle

Ziel: Die Welt wird konsistent und interaktiv.

### Priorität E – Dynamik und Langzeitleben

- Events
- Seasons
- UI-Polish
- Long-Term-Content

Ziel: Die Welt bleibt lebendig und langfristig überzeugend.

---

## 4. Modul-Leitpfad

### Start-/Onboarding

- `auth/login_system.inc`
- `tutorial/tutorial_system.inc`
- `camera/camera_system.inc`
- `state/state_spawn_system.inc`
- `state/state_system.inc`
- `ui/ui_hub.inc`
- `ui/roleplay_ui_system.inc`

### System-Grundlagen

- `core/core.inc`
- `db/db_interface.inc`
- `admin/admin_system.inc`
- `progression/progression_system.inc`

### Economy / Jobs

- `economy/*.inc`
- `jobs/*.inc`
- `business/business_system.inc`
- `commerce/marketplace_system.inc`

### Law / Emergency / World

- `law/*.inc`
- `crime/crime_system.inc`
- `emergency/*.inc`
- `factions/*.inc`
- `world/*.inc`
- `housing/*.inc`
- `inventory/*.inc`
- `vehicles/*.inc`

### Dynamic Layer

- `events/*.inc`
- `season/*.inc`
- `web/web_bridge.inc`
- `premium/premium_system.inc`

---

## 5. Entwicklungsrichtlinien

- Der Spawn darf niemals ohne gültigen State erfolgen.
- Login und Tutorial müssen als festes Start-Subsystem behandelt werden.
- `OnPlayerCommandText()` muss sauber und nachvollziehbar bleiben.
- Jeder neue Bereich muss in `gamemode.pwn` und dem passenden Modul-Stack verknüpft sein.
- Module mit aktivem Code, aber fehlender Einbindung gelten als „vorbereitet“ und nicht als fertig.
- UI- und Dialog-Flow sollten auf `ui_hub` und klare zentralisierte Menüs laufen, nicht auf verstreute Einzelwege.

---

## 6. Nächste sinnvolle Arbeitsreihenfolge

### Phase 1 – Startpfad stabilisieren
- Login + Register sauber testen
- Tutorial-Flow abschließen
- Spawn an Heimatstadt und State koppeln
- HUD / Dashboard initialisieren

### Phase 2 – Grundspiellauf
- Jobs verlinken
- Economy und Cash-Deck nachvollziehbar machen
- Progression definieren
- Shops / Business / Bank sauber verknüpfen

### Phase 3 – Ordnung und Regeln
- Wanted / Jail / Crime
- Police / EMS / Dispatch
- Konfliktlogik und Spielstatus

### Phase 4 – Weltvernetzung
- Factions / Territories / Housing / Vehicles / Inventory
- Besitz- und Eigentumslogik als zentrale Weltmechanik

### Phase 5 – Dynamische Welt
- Events / Seasons / UI-Polish
- Long-Term-Content und Systemzyklen

---

## 7. Entscheidungsregel

Vor jeder großen Erweiterung sollte immer diese Frage beantwortet werden:

„Ist dieses System direkt Teil des Start- und Spielpfads oder nur eine spätere Zusatzfunktion?“

Wenn die Antwort „mittelfristig“ ist, ist das System nicht die nächste Priorität.

---

## 8. Kurzfazit

Das Projekt braucht keine zufällige Feature-Erweiterung, sondern eine klare Entwicklungslogik:

Startsysteme → Grundspiel → Ordnung → Welt → Dynamik

Wenn diese Reihenfolge eingehalten wird, bleibt das Projekt langfristig konsistent, testbar und besser skalierbar.

---

## 9. Relevante Basisdateien

- `gamemodes/gamemode.pwn`
- `gamemodes/modules/auth/login_system.inc`
- `gamemodes/modules/tutorial/tutorial_system.inc`
- `gamemodes/modules/state/state_spawn_system.inc`
- `gamemodes/modules/state/state_system.inc`
- `gamemodes/modules/ui/ui_hub.inc`
- `gamemodes/modules/ui/roleplay_ui_system.inc`
- `COMMANDS.md`
- `MODULES.md`
- `HowToToDo.md`


---

## Aktualisierter Stand (2026-11-24)

- Registrierungsflow: Passwort-Gate vor Tutorial/Spawn, M/W-Auswahl vor Skin-Picker, `DIALOG_UI_GENDER_PICK` aktiv.
- Tutorial: `TUTORIAL_SCENE_TIME` auf 11500 ms, Schutz gegen Start ohne Passwort/Geschlecht.
- Skin-Flow: `LoginSystem_ShowSkinPicker()` zeigt geschlechtsabhängige 10–15 Skins, validiert `0..311`, setzt `SetPlayerSkin` vor finalem Spawn.
- Spawn-Order: Home-State/State validiert, `StateSpawnSystem_Apply(...)` erst nach Skin-Setzung, dann `CameraSystem_ExitCameraMode`, `SetCameraBehindPlayer`, `TogglePlayerControllable(playerid, true)`, `RoleplayUI_ShowDashboard(playerid)`.
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut (nur legacy warning in `includes/dini.inc`).
- Dini-first + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

