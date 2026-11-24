# Nexus Core Gamemode – Startup Flow Summary

## 1. Ziel

Der Einstieg des Spielers muss sauber und logisch erfolgen:

1. Connect
2. Accountstatus prüfen
3. Login oder Register
4. Pflicht-Gates erfüllen
5. Tutorial starten
6. Stadtwahl / Heimatstadt validieren
7. Skin-Auswahl
8. finaler Spawn

---

## 2. Reihenfolge der richtigen Logik

### A) Auth-Flow

- bestehender Account → Login
- neuer Account → Register
- Login-/Register-State muss konsistent sein
- `LOGIN_STATE_NONE`, `LOGIN_STATE_REGISTER`, `LOGIN_STATE_LOGIN`, `LOGIN_STATE_ACTIVE`

### B) Pflicht-Gates vor Tutorial

Vor dem Tutorial darf kein Spieler weiterlaufen, wenn:

- Passwort nicht gesetzt ist
- Geschlecht `M` oder `W` nicht gewählt wurde

Wenn ein Gate fehlt:

- Tutorial startet nicht
- Kamerafahrten starten nicht
- Spawn wird nicht ausgelöst
- der passende Dialog bleibt offen

### C) Tutorial

- 11,5 Sekunden pro Takt
- `TUTORIAL_STAGE_INTRO -> HOME -> GOV -> FINISH`
- Timer müssen bei Disconnect/Abbruch sauber beendet werden
- kein Spawn während aktiver Tutorial-Kamera-Schleife

### D) Stadtwahl / Heimatstadt

- gültige Städte: Los Santos, San Fierro, Las Venturas
- bei gültiger Auswahl: State/Home-State validieren und speichern
- gültige Werte dürfen nicht mit `0` überschrieben werden
- Fallback nur auf gültigen Standardwert

### E) Skin-Auswahl

- eigener Dialog-Schritt vor dem Spawn
- Auswahl je nach Geschlecht (`M` / `W`)
- 10–15 gültige SA:MP-Skins je Geschlecht
- Fallback bei fehlendem Geschlecht
- Skin-ID validieren (`0..311`)

### F) finaler Spawn

Die Reihenfolge muss sein:

1. State validieren
2. Skin wählen und speichern
3. `SetPlayerSkin(playerid, skinid)`
4. `SetSpawnInfo(...)`
5. `StateSpawnSystem_Apply(playerid, safeState)`
6. `CameraSystem_ExitCameraMode()`
7. `SetCameraBehindPlayer()`
8. `TogglePlayerControllable(playerid, true)`
9. `RoleplayUI_ShowDashboard(playerid)`

Keine Skin-Änderung nach dem Spawn, kein doppelter Reset und kein verfrühter Spawn.

---

## 3. Wichtige Guard-Regeln

- Login und State-/Spawn-Logik müssen getrennt bleiben
- gültige State-/Home-State-Werte dürfen nicht auf 0 fallen
- keine veralteten Login-/Register-TextDraws nach erfolgreichem Einstieg
- aktive Spieler dürfen nicht durch falsche UI-Gates blockiert werden
- Tutorial darf nicht ohne Passwort und Geschlecht starten

---

## 4. Build-Status

Der aktuelle Stand wurde mit dem Build-Target validiert:

- `bash scripts/build_gamemode.sh`
- Ziel: `Compiler/samp03/gamemodes/gtasa_reallife.amx`

Ergebnis:

- Build ohne neue Fehler
- Gamemode präpariert für den stabilen Startup-/Spawn-Flow

---

## 5. Kurzfazit

Der Einstieg wurde auf eine logische, stabilere Reihenfolge gebracht:

Passwort + Geschlecht → Tutorial → Stadtwahl → Skin-Auswahl → Spawn

Damit ist die Start- und Spawn-Kette in der aktuellen Version konsistent und um die früheren Logikfallen reduziert.
