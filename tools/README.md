# Debug- und Wartungswerkzeuge

Dieses Verzeichnis enthält Python 3 basierte Hilfswerkzeuge zur Fehleranalyse, Log-Überwachung und Server-Konfigurationsprüfung für den GTA:SA-Reallife-Modus.

## Verfügbare Tools

- `log_tail.py` – Live-Verfolgung von Server-/Plugin-Logs
- `log_watcher.py` – manuelle Log-Beobachtung ohne Hintergrundbetrieb; ideal für `Compiler/samp03/server_log.txt` oder `samp7780_bg.log`
- `analyze_logs.py` – automatische Analyse von Logdateien nach Fehlern, Warnungen und wiederkehrenden Problemen
- `validate_server_cfg.py` – prüft `server.cfg` und `sql.cfg` auf typische Konfigurationsfehler
- `autorun.py` – startet den Server, überwacht Logs und sammelt Fehlerstatus in einer Zusammenfassung
- `db_and_state_inspector.py` – schreibgeschützte Prüfung von SQLite-/Dini-Zuständen ohne Serverstart
- `audit_modules_and_guards.py` – Source-Audit für Include-/Lifecycle-/Guard-Kontrakte und Reset-Checks
- `textdraw_and_zone_mapper.py` – Übersicht für HUD-, TextDraw-, Zone- und Gangfight-Elemente
- `projectlead_api_check.py` – statische API-Abgleichung für Projektleiter-, RCON-, Spawn-, Fahrzeug- und Zonen-Pfade
- `gangfight_zone_validator.py` – Plausibilitätsprüfung für Rechteck-/Radius-Zonendefinitionen
- `economy_and_state_ledger_check.py` – Read-only-Kontrolle von Geld-/State-/Ledger-/Dini-Abweichungen
- `dialog_and_camera_reset_check.py` – statische Prüfung von Dialog-IDs, Reset-Pfaden und Camera-Guards
- `qemu_7780_readiness_check.py` – 7780-Readiness-Prüfung ohne Start des Servers
- `log_code_bridge.py` – loggetriggerte Zuordnung von auffälligen Einträgen zu relevanten Quellcodezeilen mit `nl`-ähnlichem Output und JSON/YSON-Export
- `code_line_extractor.py` – gezielte Zeilensuche und Nummerierung in `gamemodes/gamemode.pwn` und `gamemodes/modules/` mit optionalem JSON/YSON-Export
- `log_guided_autocorrect.py` – manueller, log-basierter Autokorrektor im Dry-Run-Default mit Sicherungs-Backup und `--apply`-Pfad
- `debug_trace_collector.py` – kompakter Offline-Diagnose-Export aus Logs, SQLite-, Dini- und Build-Artefakten ohne Serverstart
- `state_rehydration_guard.py` – statische Prüfung auf gültige `state_id`/`home_state`-Aktualisierung, 0-Schutz und `INSERT OR IGNORE`-Muster
- `runtime_contract_guard.py` – Audit der 7780-Single-Target-Contract-Regeln und Verhinderung von `7777`-Drift in Konfigurations- und Build-Dateien
- `ui_hud_cleanup_audit.py` – prüft Camera-/TextDraw-/HUD-Cleanup-Muster wie `SetCameraBehindPlayer()`, `TogglePlayerControllable`, `INVALID_TEXT_DRAW` und lesbare Destroy-Pfade

## Beispiele

Log-Datei live ansehen:

```bash
python3 tools/log_tail.py --log ./logs/server.log
```

Log-Dateien analysieren:

```bash
python3 tools/analyze_logs.py ./logs/server.log
python3 tools/analyze_logs.py .
```

Server-Konfiguration prüfen:

```bash
python3 tools/validate_server_cfg.py --root .
python3 tools/validate_server_cfg.py --root . --fix
```

Server starten mit Log-Überwachung:

```bash
python3 tools/autorun.py --root . --server-bin ./samp03svr --log ./logs/server.log
```

## Empfehlungen

- Logs immer in einem eigenen Ordner wie `logs/` sammeln
- `server.log` und `server-console.log` getrennt halten
- vor dem Live-Deploy die Konfigurationsprüfung laufen lassen
- bei Fehlern zuerst `analyze_logs.py` und dann gezielt `log_tail.py` nutzen

## Hinweis

Diese Hilfswerkzeuge sind keine Plattform- oder Hosting-Umstellung. Sie sind bewusst als lokales Debug- und Wartungs-Tooling im Projekt selbst verankert, damit man ohne Wechsel zu externen Plattformen schnell auf Fehler und Konfigurationsprobleme reagieren kann.

## 7780- und Plugin-Contract

Die Offline-Checks unter `tools/` sind bewusst auf den Single-Target-Contract der `Compiler/samp03/`-Umgebung ausgerichtet. Die wichtigsten Prüfungen bleiben:

- `qemu_7780_readiness_check.py` – statische 7780-Readiness ohne Serverstart
- `runtime_contract_guard.py` – Audit für `gtasa_reallife.amx`, `gamemode0 gtasa_reallife 1`, `port 7780` und Verhinderung von `7777`-Drift
- `validate_server_cfg.py` – Konfigurationsprüfung für `server.cfg`/`sql.cfg`
- `db_and_state_inspector.py` – schreibgeschützte Inspektion des Dini-/SQLite-State-Pfads mit Fokus auf `scriptfiles/`

Der Plugin-Ordner `Compiler/samp03/plugins/` wird dabei als dokumentierter, transparent verwalteter Stack behandelt; es werden keine erdachten Binaries oder unautorisierte Port-7777-Konfigurationen im Repository verankert.

## Scriptfiles-/Persistenzstruktur

Die Laufzeitstruktur unter `Compiler/samp03/scriptfiles/` wurde für den Dini-first-Workflow ergänzt. Die kritischen Verzeichnisse für Spieler-, House-, Safe-, Fahrzeug-, Staatskasse-, Budget-, Business- und Inventory-Daten wurden physisch angelegt, damit die Arbeitslogik "temp -> validieren -> atomisch umbenennen" nicht an fehlenden Ordnern scheitert. Die Struktur enthält dabei:

- `players/`, `houses/`, `house_storage/`, `safes/`, `vehicles/`, `ubb_vehicles/`
- `state/LS/`, `state/SF/`, `state/LV/`
- `state_kasse/LS/`, `state_kasse/SF/`, `state_kasse/LV/`
- `business/`, `factions/`, `inventory/`, `temp/`, `backup/`, `logs/`

Diese Ordner dienen nur als Laufzeit- und Persistenz-Hülle. Sie werden ergänzt, ohne die Offline-Checks oder den 7780-Contract zu verletzen.
