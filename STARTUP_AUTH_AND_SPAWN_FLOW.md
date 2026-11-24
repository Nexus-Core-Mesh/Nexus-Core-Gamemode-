# Nexus Core Gamemode – Startup-, Login-, Register- und Spawn-Flow

Diese Datei beschreibt den aktuellen, stabilisierten Ablauf für den Einstieg eines Spielers in den Gamemode: Login / Registrierung / Tutorial / Stadtwahl / Geschlecht / Skin-Auswahl / finaler Spawn.

Der Fokus liegt auf dem tatsächlich korrigierten Flow, der in der aktuellen Implementierung als gültige Reihenfolge verwendet wird.

---

## 1. Ziel des Flows

Der Einstieg darf nicht mehr durch eine Vermischung von Auth-Status, State-/Heimatstadt-Logik und Spawn-Logik blockiert oder verwirrt werden.

Die korrekte Reihenfolge ist:

1. Spieler verbindet sich
2. Accountstatus wird aufgelöst
3. bestehender Account geht in Login
4. neuer Account geht in Registrierung
5. Pflicht-Gates müssen erfüllt sein (`Passwort`, `Geschlecht M/W`)
6. Tutorial kann starten
7. Stadtwahl / Heimatstadt validieren und speichern
8. Skin-Auswahl anzeigen
9. finaler Spawn mit gültigem Skin und gültigem State
10. Kamera-/HUD-/Steuerungsfreigabe

---

## 2. Grundprinzipien der Korrektur

### 2.1 Trennung von Login- und State-Flow

Die Hauptursache für die bisherigen Blockaden war die Vermischung von:

- Login-/Register-Abschluss
- Home-State / Staat / Stadtzuordnung
- Tutorial-Start
- finalem Spawn

Diese Zustände wurden zu früh nebeneinander gesetzt, wodurch die Spiellogik in widersprüchliche Übergänge geriet.

Die feste Regel lautet jetzt:

- Auth-/Login-Zustand entscheidet nur darüber, ob der Spieler überhaupt im Spiel aktiv ist
- State-/Home-State-Entscheidung wird erst später validiert und gesetzt
- Spawn passiert nur nach gültiger Stadtwahl, gültigem Skin und gereinigtem Übergang

### 2.2 Login-State ist konsistent

Die Login-/Register-Logik arbeitet mit den Zuständen:

- `LOGIN_STATE_NONE`
- `LOGIN_STATE_REGISTER`
- `LOGIN_STATE_LOGIN`
- `LOGIN_STATE_ACTIVE`

Diese Zustände müssen immer sauber gesetzt und abgefragt werden. Ein Spieler darf nur dann als aktiv gelten, wenn der Auth-Flow tatsächlich abgeschlossen wurde.

### 2.3 Gültige State-Werte werden nicht mit 0 überschrieben

Ein gültiger State bzw. eine gültige Heimatstadt darf nicht durch einen Null-/Reset-Pfad vernichtet werden.

Regel:

- gültige Werte bleiben erhalten
- ungültige Werte werden auf einen gültigen Fallback normalisiert
- bevorzugter Fallback: `STATE_LOS_SANTOS`

---

## 3. Ablauf: Accountstatus auflösen

Beim Connect wird zunächst der Accountstatus geprüft.

### 3.1 Neuer Spieler

Wenn kein gültiger Account / keine gültigen Profilwerte vorliegen, geht der Spieler in:

- `LOGIN_STATE_REGISTER`

### 3.2 Bestehender Spieler

Wenn bereits Zugriff auf validierte Account-/Profil-/State-Werte existiert, geht der Spieler in:

- `LOGIN_STATE_LOGIN`

Damit wird vermieden, dass bestehende Accounts versehentlich erneut in die Registrierungs- oder Tutorial-Schleife geraten.

---

## 4. Ablauf: Login und Registrierung

### 4.1 Registrierung

Bei der Registrierung gilt eine starke Reihenfolge:

1. Passwort setzen
2. Geschlecht wählen (`M` oder `W`)
3. Geschlecht speichern
4. Passwort speichern
5. erst danach Tutorial freigeben

Das ist ein hard guard:

- Passwort fehlt → Tutorial darf nicht starten
- Geschlecht fehlt → Tutorial darf nicht starten
- beide fehlen → der Spieler bleibt im erforderlichen Dialog-/Auth-Gate

### 4.2 Login

Beim Login erfolgt der normale Validierungs- und Aktivierungspfad:

1. Spielername / Accountdaten validieren
2. Passwort prüfen
3. Login-Zustand auf `LOGIN_STATE_ACTIVE` setzen
4. aktive TextDraws/Overlays ausblenden
5. Tutorial oder direkt weiter zum nächsten logischen Schritt je nach Status

### 4.3 Auth-TextDraw-Cleanup

Beim erfolgreichen Abschluss von Login oder Register muss der Auth-/Register-Overlay sauber verschwinden:

- Login-TextDraws ausblenden
- Register-TextDraws ausblenden
- `CancelSelectTextDraw()` ausführen
- keine veralteten GTA:SA Reallife / LOGIN-TextDraws auf dem Bildschirm

Das verhindert, dass nach erfolgreichem Einstieg noch alte Menüs oder TextDraws im Spiel kleben bleiben.

---

## 5. Ablauf: Pflicht-Gate vor Tutorial

Der Tutorial darf erst dann loslaufen, wenn der Spieler die erforderlichen Grunddaten erfüllt hat.

### 5.1 Pflicht-Gate

Voraussetzung für Tutorial-Start:

- Passwort gesetzt
- Geschlecht gewählt (`M` oder `W`)
- Geschlecht im Spielerprofil gespeichert
- gegebenenfalls DB-/SQLite-Write abgeschlossen

Wenn diese nicht erfüllt sind:

- kein Tutorial-Start
- keine Kamerafahrt-Schleife
- kein frühes `StateSpawnSystem_Apply()`
- stattdessen muss der entsprechende Dialog offen bleiben

Damit wird verhindert, dass ein Spieler beim Onboarding vorzeitig in Spawn- oder State-Logik springt.

---

## 6. Ablauf: Tutorial mit 11,5 Sekunden pro Takt

Der Tutorial ist auf die Kamerafahrt-Schritte mit 11500 ms abgestimmt.

### 6.1 Stages

Die Reihenfolge ist:

- `TUTORIAL_STAGE_INTRO`
- `TUTORIAL_STAGE_HOME`
- `TUTORIAL_STAGE_GOV`
- `TUTORIAL_STAGE_FINISH`

Jeder Schritt läuft mit einem Takt von 11500 ms.

### 6.2 Guard-Regeln

Während der Tutorial-Kamera-Schleife darf kein verfrühter Spawn ausgelöst werden.

Daher gelten diese Regeln:

- kein Spawn, solange der Tutorial- oder Intro-Flow aktiv ist
- keine Kamera- bzw. Steuerungsfreigabe vor Abschluss des Tutorial-Flow
- bei Disconnect oder Abbruch: laufende Timer sauber beenden (`KillTimer`)

### 6.3 Ende des Tutorials

Wenn `TUTORIAL_STAGE_FINISH` erreicht wurde:

1. Tutorial-TextDraws aufräumen
2. Heimatstadt / Staat validieren
3. ggf. speichern
4. den Spieler nicht sofort spawnen
5. stattdessen `DIALOG_UI_SKIN_PICK` öffnen

Das ist der Punkt, an dem der Flow sauber in die Skin-Auswahl übergeht.

---

## 7. Ablauf: Stadtwahl / Heimatstadt

Nach erfolgreichem Tutorial-/Onboarding-Entscheid wird die Staat-/Heimatstadt-Logik aufgerufen.

### 7.1 Gültige Städte

Die gültigen State-Varianten sind:

- Los Santos
- San Fierro
- Las Venturas

### 7.2 Validierung

Vor dem Spawn muss die Auswahl validiert werden:

- gültige State-ID
- gültige Heimatstadt
- keine `0`-Werte als echte Auswahl
- Fallback nur auf gültigen Standardwert

Der Übergang zur Stadt-/State-Logik darf nicht zusammen mit dem Login-/Register-Reset verknüpft sein. Die Reihenfolge muss eindeutig sein.

---

## 8. Ablauf: Geschlechtsauswahl

Die Geschlechtslogik ist ein eigener, expliziter Schritt im Onboarding.

### 8.1 Auswahlwerte

- `M` = Männlich
- `W` = Weiblich

### 8.2 Speicherung

Das gewählte Geschlecht wird im Spielerprofil + Speicherlogik gespeichert.

Damit ist später die Auswertung für die Skin-Auswahl möglich:

- männliche Skins
- weibliche Skins
- Fallback wenn kein Geschlecht gesetzt ist

### 8.3 Guard

Ohne gültiges Geschlecht darf der Spieler nicht in den Tutorial-/Spawn-Flow übergehen.

---

## 9. Ablauf: Skin-Auswahl

Die Skin-Auswahl ist ein eigener Dialog-Schritt vor dem finalen Spawn.

### 9.1 Dialog

Beispiel:

- `DIALOG_UI_SKIN_PICK`

### 9.2 Auswahlbasis

Je nach gespeichertem Geschlecht werden passende Skins angezeigt:

- Männer: 10–15 gültige SA:MP-Skins
- Frauen: 10–15 gültige SA:MP-Skins
- Fallback: wenn kein Geschlecht gesetzt ist, ein sicherer Standardbereich oder generische Auswahl

### 9.3 Validierung

Die Skin-ID wird geprüft:

- gültiger Bereich `0..311`
- nur valide SA:MP-Skin-IDs
- keine ungültigen oder ungenutzten Werte

### 9.4 Setzen des Skins

Nach Auswahl wird der Skin gesetzt:

- `SetPlayerSkin(playerid, skinid)`
- `SetSpawnInfo(...)` vorbereiten

Wichtig: Das passiert vor dem finalen Spawn, nicht erst danach.

---

## 10. Ablauf: finaler Spawn

Der finale Spawn darf erst nach erfolgreicher Stadtwahl und Skin-Auswahl erfolgen.

### 10.1 Reihenfolge

Die tatsächliche Reihenfolge ist:

1. gültige Home-State / State validieren
2. Skin gültig wählen und speichern
3. `SetPlayerSkin(playerid, skinid)` aufrufen
4. `SetSpawnInfo(...)` vorbereiten
5. `StateSpawnSystem_Apply(playerid, safeState)` aufrufen
6. `CameraSystem_ExitCameraMode()`
7. `SetCameraBehindPlayer()`
8. `TogglePlayerControllable(playerid, true)`
9. `RoleplayUI_ShowDashboard(playerid)`

### 10.2 Warum diese Reihenfolge wichtig ist

Wenn der Skin erst nach dem Spawn oder nach dem State-Apply gesetzt wird, entstehen typischerweise:

- falscher Skin beim ersten Aufblitzen
- Kamera-/Spawn-Reset-Schleifen
- Starter-Overlay-Probleme
- doppelte bzw. verfrühte Übergänge

Die korrigierte Reihenfolge verhindert genau diese Effekte.

---

## 11. Guard-Regeln und Sicherheitsprinzipien

Die folgenden Regeln sind verbindlich:

- Passwort muss gesetzt sein, bevor der Tutorial luft
- Geschlecht muss gesetzt sein, bevor der Tutorial luft
- kein Spawn ohne validen State und validen Skin
- kein `0`-Override bei validen State-/Home-State-Werten
- keine Login-/Register-TextDraws nach erfolgreichem Ablauf sichtbar
- Timer müssen bei Abbruch/Disconnect sauber beendet werden
- UI-Gates dürfen aktive, gültige Logins nicht blockieren

---

## 12. Praktische Ergebnis-Logik

Der vollständige, derzeit gewünschte Ablauf sieht so aus:

- Connect
- Accountstatus prüfen
- Login oder Register
- Passwort-Gate
- Geschlecht-Gate (`M/W`)
- Tutorial mit 11,5s-Schritten
- Stadtwahl / Heimatstadt validieren
- Skin-Auswahl anzeigen
- Skin validieren und setzen
- finaler Spawn
- Kamera und Steuerung freigeben
- HUD aktivieren

---

## 13. Status der aktuellen Implementierung

Der aktuelle Stand ist mit den wichtigsten Kernpunkten umgesetzt:

- Auth-Flow und State-Flow getrennt
- Pflicht-Gate vor Tutorial
- M/W-Auswahl im Registrierungs-/Onboarding-Flow
- geschlechtsabhängige Skin-Auswahl vor finalem Spawn
- State-Validierung und Fallback-Logik hardened
- Tutorial-Timer stabilisiert
- TextDraw-/Overlay-Cleanup verbessert

Darüber hinaus ist der Projektbestand auf den aktuellen Stand dokumentiert und die großen Archivdateien wurden aus dem Repository entfernt.

---

## 14. Build/Compile-Prüfung

Der aktuelle Code-Stand wurde mit dem Build-Target validiert:

- `bash scripts/build_gamemode.sh`
- Ziel: `Compiler/samp03/gamemodes/gtasa_reallife.amx`

Ergebnis: Der Gamemode kompiliert ohne neue Build-Fehler.

Live-Server-Tests auf Port 7780 wurden in dieser Umgebung nicht als echtes Server-Playtest ausgeführt; die Verifikation hier basiert auf Code- und Compile-Validierung.

---

## 15. Kurzfazit

Der Einstieg ist jetzt grundsätzlich sauber aufgebaut:

- Auth und Spawn sind voneinander getrennt
- Passwort und Geschlecht sind echte Voraussetzungen
- Tutorial läuft nur nach gültigen Voraussetzungen
- Skin-Auswahl sitzt genau vor dem Spawn
- finaler Spawn greift mit validem State und validem Skin
- die UI-/TextDraw- und State-Blockaden wurden gezielt reduziert

Damit ist die Start- und Spawn-Kette im Kern in einer sicheren Reihenfolge aufgebaut, die die früheren Logik-Fallen deutlich reduziert.
