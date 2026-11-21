# Nexus Core UI Flow Report

## 1) Aktueller Stand der Umsetzung

Dieser Bericht fasst den aktuellen Code- und Build-Stand für das Nexus-Core-Gamemode zusammen, mit Fokus auf den onboarding-/tutorial-basierten Übergang zum festen Spawnpunkt, das button-/menü-gesteuerte UI und die Kamera-/Perspektiv-Logik.

### 1.1 Kernarbeit und Zielbild

Das System läuft in der 7780-Sandbox grundsätzlich stabiler, und der Fokus lag auf einem klaren, menu-first Flow:

- Login/Register wird automatisch erkannt.
- Das Passwort ist die einzige freie Texteingabe.
- Alle weiteren Auswahlpunkte laufen über Buttons, Menüs oder Dialoge.
- Nach dem Tutorial wird der Spieler in einen sauberen State-/Spawn-Auswahlpfad geführt.
- Die Auswahl wird gespeichert, ohne gültige Werte mit `0` zu überschreiben.
- Danach wird der Spieler direkt an seinen festen Spawnpunkt gesetzt.
- Die Kamera wird sauber auf den Spieler zurückgesetzt, sobald der Auswahlpfad endet oder ein Wechsel/Reset erforderlich ist.

### 1.2 Geänderte Module und Funktionen

#### `gamemodes/modules/ui/roleplay_ui_system.inc`

Ziel: Basis-UI für die RP-Hub- und Onboarding-Struktur, inklusive übersichtlicher Menü-/Dialog-Gewichtung und sauberer Nutzen der verfügbaren Fläche.

Wichtige Aspekte:

- Oberflächenlogik für Auswahl-/Registrierungs- und Hub-Flows ergänzt bzw. konsolidiert.
- Menü-/Dialog-Einstieg auf Button-/Menüpfade ausgerichtet.
- Keine freie Text-Eingabe für State-/Spawn-Auswahl; alle entscheidenden Auswahlpunkte laufen über Dialog-/Menüpfade.
- UI-Hierarchie und Übergänge auf tutorial- und state-abhängige Flow-Ketten abgestimmt.

#### `gamemodes/modules/ui/ui_hub.inc`

Ziel: zentrale Menü-/Hub-Abbildung für den UI-Flow.

Wichtige Aspekte:

- State-Auswahl über Dialog-/Button-Flow und nicht über Chat-Befehle.
- Beim Abschluss der Auswahl wird der Wert persistiert und der Spieler direkt in das passende Spawn-/State-Szenario gesetzt.
- Beim Zustandsswitch bzw. beim Start von Kameraszenen wird der UI-Context sauber beendet bzw. zurückgesetzt.
- Die Auswahl wird in der DB und im Runtime-State konsistent gehalten, damit spätere Logins und Spawns denselben festen Spawnpunkt verwenden.

#### `gamemodes/modules/tutorial/tutorial_system.inc`

Ziel: Struktur des Tutorial- und Onboarding-Flows, mit sauberem Übergang zur State-/Spawn-Auswahl nach Abschluss.

Wichtige Aspekte:

- Tutorial-Stages werden über feste Zustände und Übergänge abgewickelt.
- Nach Abschluss wird die Flow-Kette nicht mehr auf einen undefinierten Default verborgen; sie springt in die gezielte State-/Spawn-Auswahl.
- Beim Übergang werden Kamera- und UI-Context sauber zurückgesetzt, damit keine Überlagerung aus alter Tutorial-Kamera und neuem Spawn-/Hub-Prozess entsteht.
- Der Übergang setzt die Auswahl und das Verhalten auf einen festen Spawnpunkt, statt nur den Wert temporär im Spielerobjekt zu verändern.

#### `gamemodes/modules/state/state_spawn_system.inc`

Ziel: Auswahl, Speicherung und Anwendung des festen Spawnpunkts.

Wichtige Aspekte:

- `StateSpawnSystem_Apply()` dient als zentrale Stelle für das Anwenden eines gewählten State/Spawns.
- Bei der Auswahl wird der State gültig normalisiert, persistiert und auf den festen Spawn angewendet.
- Der Spawn-/State-Dialog bzw. -Mauspfad wird in die feste Position überführt und nicht mehr nur als passiver UI-Context behandelt.
- Beim Anwenden der Auswahl wird die Kamera an den Spielerpivot zurückgesetzt, damit der Spawn nicht in einer veralteten Tutorial-/Drone-View landet.

#### `gamemodes/modules/camera/camera_system.inc`

Ziel: flexible Kamera-/Perspektiv-Basis, ohne den Main-Thread durch teure Loop- oder TextDraw-Heavy-Mechaniken zu belasten.

Wichtige Aspekte:

- Basis für freie Kamera-Ansichten, Drone-/Übersichtsansichten, Fokus-/Späher-/Event-Modi und verwandte Perspektivwechsel.
- Nutzung von `SetPlayerCameraPos` und `SetPlayerCameraLookAt` als Kern-Mechanik.
- Reset über `SetCameraBehindPlayer` statt über hackige Zustandsreste oder offene TextDraw-Überlagerungen.
- Beseitigt die Gefahr von `!= Text:0`-Fallen im Zusammenhang mit Kamera-/HUD-Reset, indem der Zustand eher über harte Flags/Variablen und saubere Camera-Reset-Funktionen verwaltet wird.
- Strenges Augenmerk auf niedrigen Callback-/Timer-Aufwand unter `qemu-i386`.

#### `gamemodes/modules/db/db_interface.inc`

Ziel: Persistenz der State-/Home-State-Werte und Schutz gegen inkonsistente Überschreibungen beim Join/Spawn.

Wichtige Aspekte:

- Persistenz von `state_id` und `home_state` über die vorhandene Datenbank-API.
- Gültige Werte werden nicht mehr versehentlich mit `0` überschrieben.
- Die DB-Funktionen arbeiten mit einem defensiven Pattern, damit `player_state` nicht durch einen Join-/Connect-/Spawn-Callback verloren geht.
- Die Speicherlogik ist auf das kombinierte Verhalten von Login, Tutorial, State-Selection und späteren Respawns ausgerichtet.

#### `gamemodes/gamemode.pwn`

Ziel: zentrale Einbindung der Flow-Kette und Konzept- bzw. Hook-Integration.

Wichtige Aspekte:

- Zentraler Connect-/Spawn-/Dialog-Lifecycle bleibt erhalten.
- Tutorial-/Login-/State-/Spawn-Pfaden sind an denselben Core-Flow angebunden.
- Die Kette ist auf den festen Spawnpunkt nach der Auswahl ausgelegt, statt auf ein einfaches „spawnen in default state 0“.

### 1.3 4-Zonen-HUD-Layout

Das HSO-/HUD-Konzept wurde weiter auf ein 4-Zonen-Layout ausgerichtet, das die Sichtbarkeit und den Informationsfluss für Spieler auf mobilen Clients besser handhabbar macht.

Die gestreckte Idee war:

- Zone 1: links oben: kompakter Status-/Live-Bereich.
- Zone 2: links unten: Player-/Hub-/Tutorial-Bereich.
- Zone 3: rechts oben/mitte: Fokus-/Event-/Status-/Text-Wars-Informationen.
- Zone 4: rechts unten: HUD-/Waffen-/Context-Gebiet, ohne Chat/Radar/Interaktion zu überlagern.

Ziel:

- Radar, Chat und Tutorial nicht aufeinander zu stacken.
- TextDraws in klaren Ankerbereich verlegen.
- Keine unkontrollierten Überlagerungen im oberen linken Bereich, die den Wechsel zwischen Login/Register/Tutorial und Spawn-UI blockieren.

### 1.4 Button- und Menü-Einstieg

Der Spielpfad ist klar auf ein menu-first Verhalten ausgerichtet:

- Passwort ist die einzige freie Texteingabe.
- Der State-/Spawnpfad wird per Button/Menu ausgewählt.
- Der Tutorial läuft ebenfalls über feste Auswahl-/Next-Schritte und nicht durch Chat-Kommandos.
- Die Entscheidung „Geburts-/Spawnpunkt bzw. Staat“ wird während bzw. direkt nach dem Tutorial getroffen, und der ausgewählte Wert wird wieder verwendet, auch beim nächsten Login oder Spawn.

### 1.5 Kamera-/Perspektiv-Basis

Die Kamera logischer Aufwand ist bewusst in eine zentrale, wiederverwendbare Funktionalität verschoben:

- `CameraSystem_Reset()` bzw. `CameraSystem_SetMode()` als Kern der Wiederverwendung.
- `SetPlayerCameraPos` und `SetPlayerCameraLookAt` als zentrale Perspektiv-Primitiven.
- Reuse in Tutorial-, Spawnpunkt-, allgemeiner UI-/Überblicks- und Event-/Drohnenperspektive.
- Kein starrer Hardcode auf nur eine Kamera-Variante; dieselbe Basis kann für freie Kameras, Sicht-/Event-Modi, Späher-/Fokus-Ansichten und Überblicksmodi wiederverwendet werden.

## 2) Erhaltene Hardening- und Persistenz-Maßnahmen

### 2.1 `INSERT OR IGNORE` bei `player_state`

Wichtige Maßnahme, die im bisherigen Lauf stabilisierend wirkte:

- Validierte Werte werden beim Join/Connect nicht durch einen späteren Reset auf `0` überschrieben.
- Wenn `player_state` oder verwandte State-/Home-State-Werte bereits gesetzt sind, wird ein ungültiger Lese-/Schreib-Overwrite vermieden.
- Das ist zentral für den festen Spawnpunkt nach Tutorial-/State-Auswahl.

### 2.2 Sichere Token-Extraktion statt fragiler `sscanf`-Pufferlogik

Das Projekt wurde in kritischen Eingabe- und Befehls-pfaden vor unkontrollierten Pufferüberläufen geschützt:

- Statt zu kleiner `sscanf`-Ziele mit knappem Puffer wurde auf sichere Token-Extraktion via `strmid`-basiertem Parsing gesetzt.
- Damit werden längere Befehle wie `register ...` und state-spezifische Befehle robust gegen Buffer-Overflows.
- Die Befehls- und Dialoglogik ist damit deutlich robuster unter mobilen Client-/Input-Variationen.

### 2.3 Entlastete Timer-/Callbacks

Unter `qemu-i386` war die Emulationslast der zentrale Risikofaktor. Daher wurden die betroffenen Bereiche bewusst entlastet:

- Text-/HUD-/Tick-Callbacks reduziert und klareren Ablaufprioritäten untergeordnet.
- Keine unnötige Tick-Spitze im Tutorial-/State-/Kamera-Flow.
- Fokus auf saubere, gezielte Trigger statt massiver Hintergrundschleifen.

### 2.4 `INVALID_TEXT_DRAW`-Prüfungen und sauberes Aufräumen

Die UI-/Camera-Integration wurde mit defensiven Prüfungen ergänzt:

- `INVALID_TEXT_DRAW` bzw. andere saubere Null-/Invalid-Checks verhindertet Überläufe, fehlerhafte Referenzen und unkontrollierte HUD-/Camera-Rückstände.
- Replay-/Reset-Mechanik ist gezielt auf Cleanups und Reset-Bereiche ausgerichtet.
- Disconnect/Exit-/Reset-Pfade wurden berücksichtigt, damit keine veraltete Kamera oder TextDraw-Referenz hängen bleibt.

### 2.5 Erfolgreicher Build

Der Build wurde erfolgreich ausgeführt:

- Befehl: `./build_server.sh`
- Ergebnis: erfolgreich durchgelaufen
- Exit-Code: `0`

Das ist ein wichtiger Nachweis, dass der aktuelle Source-Stand kompilierbar und in der Sandbox-Umgebung konsistent ist.

## 3) Offene Punkte und konkrete Live-Prüfungen

### 3.1 Offene Punkte

Die Umsetzung ist im Code klar weiterentwickelt, aber einzelne Punkte sind noch nicht vollständig live validiert:

1. End-to-end-Verifikation der Auswahlpfade auf der echten 7780-Instanz.
2. Nachweis, dass `player_state` und `home_state` nach Tutorial-/State-Auswahl nicht erneut auf `0` zurückfallen.
3. Prüfung, dass der feste Spawnpunkt auch nach späteren Logins und Spawns reproduzierbar gilt.
4. Verifikation, dass keine Kamera-/UI-Überlagerungen oder TextDraw-Reste nach Tutorial-/State-Ende hängen bleiben.
5. Prüfung der tatsächlichen Spiel- und Parse-Performance im realen `qemu-i386`-Runtime-Fenster.

### 3.2 Konkrete Live-Prüfsschritte

#### `server_log.txt`

- `server_log.txt` nach einem Join/Spawn/Tutorial-Lauf prüfen.
- Fokus auf:
  - Long callback execution detected
  - Timing-Warnungen
  - Spawn-/Connect-/Dialog-Fehler
  - `player_state`/`home_state`-Persistenzmeldungen

#### `tools/log_watcher.py`

- Log- und Runtime-Output mit `tools/log_watcher.py` beobachten.
- Dafür direkt am betroffenen 7780-Prozess oder im 7780-Testlauf nach dem Tutorial-/State-Auswahlpfad prüfen.
- Ziel: sicherstellen, dass keine neuen Callback-Spitzen oder Spawn-Stalls entstehen.

#### `gtasa_reallife.db`

- SQLite-DB prüfen:
  - `player_state`
  - `home_state`
  - `state_id`
  - zugehörige Player-/User-/Account-zeilen
- Verifizieren, dass der gewählte State nach Tutorial-/Spawn-Auswahl persistiert und bei erneutem Login nicht überschrieben wird.

#### `scriptfiles/`

- Prüfen, ob die UI- oder Dialog-Strings/Render-Assets sauber im scriptfiles-Set liegen und nicht mit alten HUD-/TextDraw-Definitionen kollidieren.
- Gegebenenfalls verifizieren, dass die Auswahl-/Button-Sichten und Dialog-IDs im erwarteten Schema liegen.

#### Client-Test

- Spieler auf 7780 mit dem mobilen Client (`Thunder - SAMP` oder vergleichbar) testen.
- Prüfen:
  - Tutorialabschluss
  - State-/Spawn-Auswahl via Button/Menu
  - Direktes Spawnen am festen Spawnpunkt
  - Kamera-Reset ohne Hängen oder Überlagerung
  - spätere Login-/Spawn-Wiederholungen mit unverändertem State

## 4) Kurzfazit

Der Code steht insgesamt deutlich besser da: der Tutorial-/State-/Spawn-Flow ist im Gamemode-Quellcode sauberer und kontrollierter verknüpft, das HUD ist auf klarere Zonen ausgerichtet, die Kamera wurde zu einer flexiblen Basisfunktion entwickelt, und die Persistenzlogik schützt gültige State-Werte vor dem bekannten `0`-Reset.

Der Build lief erfolgreich, aber die letzte Stufe ist die echte Laufzeit-Verifikation auf der 7780-Instanz mit `server_log.txt`, `tools/log_watcher.py`, `gtasa_reallife.db` und einem echten Client-Test. Ohne diese Live-Prüfung ist der Source-Stand nur als kompilierbar, aber noch nicht als vollständig runtime-validiert zu bewerten.

---

## Additive Abschluss-Validierung – 2026-11-21

Diese Abschlussnotiz ergänzt den Flow-Report ohne bestehende Abschnitte zu löschen.

- `LoginSystem_AutoResolveFlow()` erkennt Account-/Login-/Register-Pfad automatisch und lässt nur das Passwort als freie Texteingabe zu. Die anschließende Staat-/Home-State-Auswahl läuft über Menü-/Dialog-Optionen weiter.
- `TutorialSystem_Next()` führt nach Tutorial-Abschluss direkt in den State-/Spawn-Pfad, setzt `home_state` und `state_id` über valide Fallbacks und ruft `StateSpawnSystem_Apply()` auf, bevor die Kamera und der UI-Context zurückgesetzt werden.
- `OnPlayerSpawn()` hält den Tutorial-/State-Szene-Trigger bei gültigem `home_state` / `state_id` aktiv zurück und verhindert doppelte Szene-Starts bei bereits abgeschlossenem Onboarding.
- `CameraSystem_ExitCameraMode()` und `CameraSystem_ToggleFreeCamera()` sichern den Kamera-Reset über `SetCameraBehindPlayer()` und `TogglePlayerControllable(playerid, 1)` ab, damit die Menü-/Dialogführung nicht in einem blockierten Kamerazustand hängen bleibt.
- Der aktuelle Source-Stand wurde mit dem gezielten Single-Target-Build verifiziert; der Live-Server-Start bleibt separat freigegeben und wurde nicht durchgeführt.
