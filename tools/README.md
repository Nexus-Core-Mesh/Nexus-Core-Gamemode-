# Debug- und Wartungswerkzeuge

Dieses Verzeichnis enthält Python 3 basierte Hilfswerkzeuge zur Fehleranalyse, Log-Überwachung und Server-Konfigurationsprüfung für den GTA:SA-Reallife-Modus.

## Verfügbare Tools

- `log_tail.py` – Live-Verfolgung von Server-/Plugin-Logs
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
