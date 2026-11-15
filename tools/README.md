# Debug- und Wartungswerkzeuge

Dieses Verzeichnis enthält Python 3 basierte Hilfswerkzeuge zur Fehleranalyse, Log-Überwachung und Server-Konfigurationsprüfung für den GTA:SA-Reallife-Modus.

## Verfügbare Tools

- `log_tail.py` – Live-Verfolgung von Server-/Plugin-Logs
- `analyze_logs.py` – automatische Analyse von Logdateien nach Fehlern, Warnungen und wiederkehrenden Problemen
- `validate_server_cfg.py` – prüft `server.cfg` und `sql.cfg` auf typische Konfigurationsfehler
- `autorun.py` – startet den Server, überwacht Logs und sammelt Fehlerstatus in einer Zusammenfassung

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
