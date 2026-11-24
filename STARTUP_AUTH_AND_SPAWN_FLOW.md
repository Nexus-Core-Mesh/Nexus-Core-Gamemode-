# Nexus Core Gamemode – Startup-, Login-, Register- und Spawn-Flow

Diese Datei beschreibt den **tatsächlich implementierten** Ablauf für den Einstieg eines Spielers: Connect → Accountstatus → Login **oder** Registrierung → (bei Neuanlage) Geschlecht → Skin → Stadtwahl → Tutorial → finaler Spawn.

Die Persistenz läuft ausschließlich über **SQLite** (`data/db/gtasa_reallife.db`). Es gibt keinen INI-/Datei-Fallback mehr. Der Einstieg ist komplett **dialog-first** – es gibt keine Login-/Register-/Stadt-TextDraw-Overlays mehr.

---

## 1. Überblick der zwei Pfade

**Bestehender Account (Login):**

1. Connect → Accountstatus aufgelöst (`DB_AccountExists`)
2. `LOGIN_STATE_LOGIN`
3. Passwort-Dialog (`DIALOG_STYLE_PASSWORD`)
4. Passwort gegen SQLite prüfen (`DB_VerifyPassword`)
5. Accountdaten laden (`DB_LoadAccount`)
6. `LOGIN_STATE_ACTIVE` → **direkter Spawn** in die gespeicherte Heimatstadt
7. Kamera/HUD/Steuerung frei – **kein Tutorial, keine Skin-Auswahl**

**Neuer Account (Registrierung):**

1. Connect → Accountstatus aufgelöst
2. `LOGIN_STATE_REGISTER`
3. Passwort-Dialog setzen
4. Geschlecht wählen (`DIALOG_UI_GENDER_PICK`: Männlich / Weiblich)
5. geschlechtsabhängige Skin-Auswahl (`DIALOG_UI_SKIN_PICK`)
6. Stadtwahl / Heimatstadt (`DIALOG_UI_STATE_PICK`: LS / SF / LV)
7. Tutorial-Kameraflug (9 Szenen à 11,5 s)
8. Account in SQLite anlegen (`DB_CreateAccount`, `INSERT`)
9. `LOGIN_STATE_ACTIVE` → finaler Spawn mit gültigem Skin und State

---

## 2. Grundprinzipien

### 2.1 Trennung von Account-Existenz und aktiver Session

- `LoginSystem_AutoResolveFlow` entscheidet ausschließlich anhand eines echten SQLite-Lookups (`DB_AccountExists` nach `GetPlayerName`) über Login vs. Register.
- `LoginSystem_IsActive` / `LoginSystem_ActivateSession` steuern den aktiven Zustand (`LOGIN_STATE_ACTIVE`).
- Ein Spieler gilt erst nach abgeschlossenem Auth-Flow als aktiv.

### 2.2 Login-States

```
LOGIN_STATE_NONE      // vor der Auflösung
LOGIN_STATE_REGISTER  // neuer Account, Onboarding läuft
LOGIN_STATE_LOGIN     // bestehender Account, Passwort ausstehend
LOGIN_STATE_ACTIVE    // authentifiziert und gespawnt
```

### 2.3 Gültige States werden nie mit 0 überschrieben

- State-IDs sind 0-basiert: `STATE_LOS_SANTOS = 0`, `STATE_SAN_FIERRO = 1`, `STATE_LAS_VENTURAS = 2`.
- `core.inc` ist die einzige Quelle für `state_id` / `home_state`.
- Normalisierung läuft über `Core_GetValidStateOrDefault(...)`; ungültige Werte fallen auf `STATE_LOS_SANTOS` zurück, gültige bleiben unangetastet.
- Beim Connect/Disconnect werden gültige State-Werte erhalten und nicht auf 0 gesetzt.

---

## 3. Connect

`OnPlayerConnect` (in `gamemode.pwn`) ruft deterministisch **genau einmal**:

1. `Core_RunPlayerConnectHooks(playerid)` – darin unter anderem **ein einziger** Aufruf von `LoginSystem_OnPlayerConnect`. Die registrierten Hook-Namen in `runtime_registry.inc` dienen nur der Debug-Auflistung und werden nicht zusätzlich dynamisch ausgeführt.
2. `LoginSystem_BeginSession(playerid)` – setzt den Spieler in den Spectating-Modus.
3. `CameraSystem_PlayIntroScene(playerid)` – feste Intro-Kameraposition.

`LoginSystem_OnPlayerConnect` normalisiert die State-Werte (ohne 0-Override), blendet alle alten TextDraws aus (`LoginSystem_HideLoginScreen` / `HideRegisterScreen` / `TutorialSystem_HideStage` / `CancelSelectTextDraw`) und ruft dann `LoginSystem_AutoResolveFlow`.

`OnPlayerConnect` bleibt schlank: Inventar und schwere Spielerdaten werden erst nach erfolgreichem Login geladen (`InventorySystem_OnPlayerAuthenticated` in `LoginSystem_ActivateSession`).

---

## 4. Accountstatus auflösen (`LoginSystem_AutoResolveFlow`)

1. Zustände/Flags zurücksetzen, alte TextDraws schließen.
2. `GetPlayerName` → `DB_SetAccountName`.
3. `DB_AccountExists(name)`:
   - **true** → `LOGIN_STATE_LOGIN`, Hinweis „Willkommen zurueck".
   - **false** → `LOGIN_STATE_REGISTER`, Hinweis „Neuer Account".
4. `CameraSystem_Reset`, `TogglePlayerControllable(false)`.
5. `UiHub_ShowPasswordInput(playerid)` zeigt den passenden `DIALOG_STYLE_PASSWORD` (Login vs. Registrierung).

---

## 5. Passwort (`DIALOG_UI_AUTH_PASSWORD` → `LoginSystem_SubmitPassword`)

- Eingabe wird auf Länge und sichere Zeichen geprüft (`UiDialog_IsSafeText`).
- **Registrierung:** Passwort (≥ 3 Zeichen) wird zwischengespeichert, `gPlayerPasswordReady = true`, dann `LoginSystem_CompleteReg`.
- **Login:** `DB_VerifyPassword` (gehashter Vergleich). Bei Fehler erneuter Dialog; bei Erfolg `DB_LoadAccount` → `LoginSystem_CompleteLogin`.

---

## 6. Login-Pfad (`LoginSystem_CompleteLogin`)

Für bestehende Accounts – **ohne** Tutorial oder Skin-Auswahl:

1. Heimatstadt aus den geladenen Daten bestimmen (gültig oder `STATE_LOS_SANTOS`).
2. `LoginSystem_ActivateSession(homeState)` → `LOGIN_STATE_ACTIVE`.
3. `LoginSystem_PrepareSpawnInfo` (Skin aus Account übernehmen, `SetSpawnInfo`).
4. `TogglePlayerSpectating(false)`, `SpawnPlayer`.
5. `StateSpawnSystem_Apply(homeState)`.
6. `RoleplayUI_ShowDashboard`.

---

## 7. Register-Pfad (`LoginSystem_CompleteReg`)

Deterministische Dialog-Weiche. Jeder Dialog ruft am Ende wieder `CompleteReg` auf und springt zum nächsten offenen Schritt:

1. Passwort nicht gesetzt → Passwort-Dialog.
2. Kein Geschlecht → `LoginSystem_RequireGenderSelection` (`DIALOG_UI_GENDER_PICK`).
3. Kein Skin → `LoginSystem_ShowSkinPicker` (`DIALOG_UI_SKIN_PICK`).
4. Keine Heimatstadt → `UiHub_ShowStatePick` (`DIALOG_UI_STATE_PICK`).
5. Tutorial nicht abgeschlossen → `TutorialSystem_Start`.
6. Alles erfüllt → `LoginSystem_FinalizeRegisterSpawn`.

### 7.1 Geschlecht (`DIALOG_UI_GENDER_PICK`)

- `0 = Männlich` (`GENDER_MALE`), `1 = Weiblich` (`GENDER_FEMALE`).
- gespeichert über `Core_SetPlayerGender` + `gPlayerData[...][E_PLAYER_GENDER]`.
- steuert die Skin-Liste.

### 7.2 Skin (`DIALOG_UI_SKIN_PICK`)

- geschlechtsabhängige Pools (je 15 gültige SA:MP-Skins), plus Fallback-Pool.
- `LoginSystem_ApplySkinChoice`: Bereich `0..311`, `SetPlayerSkin`, `Core_SetPlayerSkin`, `gPlayerData[...][E_PLAYER_SKIN]`.

### 7.3 Stadtwahl (`DIALOG_UI_STATE_PICK`)

- Liste: Los Santos / San Fierro / Las Venturas (Listindex = State-ID 0/1/2).
- `LoginSystem_SelectHomeState` setzt `state_id` + `home_state` zentral über `core.inc`; keine 0-Overrides gültiger Werte.

### 7.4 Tutorial (`tutorial_system.inc`)

- Guard: nur bei `LOGIN_STATE_REGISTER` und gesetztem Passwort.
- 9 Kameraszenen (`TUTORIAL_SCENE_COUNT = 9`) mit je `TUTORIAL_SCENE_TIME = 11500 ms`, automatisch weitergeschaltet über `TutorialSystem_AutoNext` (tokengesichert gegen doppelte Timer).
- Stufen laufen von `TUTORIAL_STAGE_INTRO (1)` bis `TUTORIAL_STAGE_FINISH (10)`.
- Bei `TUTORIAL_STAGE_FINISH`: TextDraws aufräumen, Kamera zurücksetzen, zurück in `LoginSystem_CompleteReg`, das nun den finalen Spawn auslöst.

### 7.5 Abschluss (`LoginSystem_FinalizeRegisterSpawn`)

1. `DB_CreateAccount` (`INSERT` in SQLite, inkl. gehashtem Passwort, Geschlecht, Skin, State).
2. `LoginSystem_ActivateSession` → `LOGIN_STATE_ACTIVE`.
3. `LoginSystem_PrepareSpawnInfo`, `TogglePlayerSpectating(false)`, `SpawnPlayer`.
4. `StateSpawnSystem_Apply`, `RoleplayUI_ShowDashboard`.

---

## 8. Finaler Spawn (`StateSpawnSystem_Apply`)

Gilt für beide Pfade und führt den Spieler sauber in die Welt:

1. State/Home-State validieren (gültig bleibt erhalten, sonst `STATE_LOS_SANTOS`).
2. zentral speichern (`Core_SetPlayerState` / `Core_SetPlayerHomeState`, `DB_SavePlayerState`).
3. Skin erneut sicherstellen (`SetPlayerSkin` + `Core_SetPlayerSkin`).
4. `SetPlayerInterior(0)`, `SetPlayerVirtualWorld(0)`.
5. alle Login-/Register-/Tutorial-TextDraws ausblenden, `CancelSelectTextDraw`.
6. `TogglePlayerSpectating(false)`.
7. `CameraSystem_ExitCameraMode`, `CameraSystem_Reset`, `SetCameraBehindPlayer`.
8. `TogglePlayerControllable(true)`.
9. `SetPlayerPos` + `SetPlayerFacingAngle` auf einen registrierten Spawnpunkt der Heimatstadt.
10. `RoleplayUI_ShowDashboard`.

Levelabhängiges Spieler-Wetter wird pro Spieler über `LoginSystem_ApplyPlayerWeather` (`SetPlayerWeather`) in `LoginSystem_OnPlayerSpawn` gesetzt.

---

## 9. TextDraw- und Overlay-Cleanup

- Vor jedem Übergang werden `gLoginTD` und `gTutorialTD` mit `INVALID_TEXT_DRAW`-Prüfung ausgeblendet, zerstört und zurückgesetzt, zusätzlich `CancelSelectTextDraw`.
- Es existieren keine Login-/Register-/Stadt-TextDraw-Overlays mehr; sämtliche Auth-Schritte laufen über Dialoge.
- `UiHub_Ready` blockiert aktive Auth-Dialoge nicht: Dialoge im Zustand `LOGIN_STATE_REGISTER` / `LOGIN_STATE_LOGIN` werden immer verarbeitet.

---

## 10. Persistenz (SQLite-only)

- `DB_SQLite_Initialize` öffnet `data/db/gtasa_reallife.db` und legt beim Start alle Tabellen via `CREATE TABLE IF NOT EXISTS` an (`players`, `player_inventory`, `houses`, `vehicles`, `states` u. a.).
- Auth-/Account-API (`DB_AccountExists`, `DB_VerifyPassword`, `DB_LoadAccount`, `DB_CreateAccount`, `DB_SaveAccount`) ist namensbasiert und ausschließlich SQLite-gestützt.
- Schreibpfade sind erst nach `DB_MarkAuthenticated(true)` frei.

---

## 11. Verbindliche Guard-Regeln

- Passwort muss vor Geschlecht/Skin/Tutorial gesetzt sein.
- Geschlecht muss vor der Skin-Auswahl gesetzt sein.
- kein Spawn ohne gültigen State und gültigen Skin.
- kein 0-Override gültiger `state_id` / `home_state`.
- keine Login-/Register-TextDraws nach erfolgreichem Ablauf sichtbar.
- Tutorial-Timer sind tokengesichert; bei Disconnect wird der Zustand sauber zurückgesetzt.
- `LoginSystem_OnPlayerConnect` / `LoginSystem_AutoResolveFlow` laufen genau einmal pro Connect.

---

## 12. Build/Compile

- Build-Befehl: `bash scripts/build_gamemode.sh`
- Ziel: `Compiler/samp03/gamemodes/gtasa_reallife.amx`
- Ergebnis: kompiliert mit **0 Errors / 0 Warnings**.

Live-Server-Tests auf Port 7780 wurden in dieser Umgebung nicht als echtes Playtest ausgeführt; die Verifikation basiert auf Code- und Compile-Validierung.
