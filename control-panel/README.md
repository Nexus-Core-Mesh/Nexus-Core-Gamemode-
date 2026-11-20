# Nexus Core Control Panel

## Zweck

Das Control Panel ist ein eigenständiges Web-Managementmodul für den GTA:SA-Reallife-Gamemode. Es ist bewusst vom SA:MP-Pawn-Gamemode entkoppelt und liest dieselbe SQLite-Datenbank, damit Betriebsüberwachung, Administration und Weiterentwicklung ohne direkte Einbindung in die Spielserver-Logik möglich bleiben.

## Funktionen

- serverweite Zustandsübersicht
- Länder-/Staatskassenübersicht für LS, SF und LV
- Spielerdaten, Adminlisten, Jobs, Fraktionen und Businesses
- Haus-, Property-, Fahrzeug- und UBB-/Ownership-Status
- Event-, Market-, Dispatch- und Economy-Übersichten
- vorbereitete Admin-/UCP-/Webbridge-Integration

## Architektur

```text
control-panel/
├── README.md
├── package.json
├── server.js
├── public/
│   ├── index.html
│   ├── styles.css
│   └── app.js
├── .gitignore
└── node_modules/
```

## Voraussetzungen

- Node.js 18+
- Zugriff auf `../data/db/gtasa_reallife.db`
- gültiges SQLite-Schema mit Tabellen wie `states`, `state_bank`, `players`, `jobs`, `factions`, `houses`, `businesses`, `vehicle_mods`, `server_events`

## Starten

```bash
cd control-panel
npm install
npm start
```

Oder über das projektweite Offline-Tool:

```bash
python3 tools/control_panel_manager.py ensure
python3 tools/control_panel_manager.py start
```

Danach ist das Dashboard unter `http://localhost:3001/` erreichbar.

## Projektweite Integration

Das Panel ist bewusst ein optionales Betriebsmodul, nicht der primäre Spielzustand. Die zentrale Produktionsquelle bleibt der SA:MP-Gamemode mit Dini-first-Saves unter `Compiler/samp03/scriptfiles/`, während die Dashboard-Sicht lesend auf `data/db/gtasa_reallife.db` basiert.

Wenn das Panel auf dem Zielsystem nicht erreichbar ist, sollte der Server-Deploy nicht an dieser Abhängigkeit hängen. Der Projekt-Workflow ist daher:

```bash
python3 tools/control_panel_manager.py ensure
python3 tools/control_panel_manager.py start
./scripts/deploy_server.sh
```

Unter dieser Logik ist das Panel ein kontrollierter Zusatz-Worker: wenn `node`/`npm` oder die Datenbank fehlen, wird nur eine Warnung ausgegeben, die Serverinitialisierung läuft aber weiter, solange der eigentliche Gamemode-Build valid ist.

## API-Endpunkte

- `GET /api/overview`
- `GET /api/summary`
- `GET /api/states`
- `GET /api/players`
- `GET /api/admins`
- `GET /api/jobs`
- `GET /api/factions`
- `GET /api/houses`
- `GET /api/businesses`
- `GET /api/server-info`
- `GET /api/vehicle-mods`
- `GET /api/transport-workers`
- `GET /api/mission-board`
- `GET /api/factory-network`
- `GET /api/marketplace`
- `GET /api/governance`
- `GET /api/contraband`
- `GET /api/taxi`
- `GET /api/district-economy`
- `GET /api/web-sync`
- `GET /api/events`

## Designprinzipien

- nur lesender Zugriff auf SQLite
- stateless und leicht erweiterbar
- Fallbacks bei leeren bzw. unvollständigen Tabellen
- State-Normalisierung auf LS/SF/LV
- geteilte Datenbasis mit dem Gamemode, ohne direkte Asset-/Runtime-Abhängigkeiten
- reale Live-Validierung nur über die gleiche Zustandsquelle wie der Gamemode; bei Dini-first Persistenz muss SQLite entweder synchronisiert werden oder das Panel muss die INI-Dateien direkt lesen, sonst bleiben Staatskassen und House-/Player-Status veraltet
- die Datenbasis des Panels ist keine primäre Speicherquelle, sondern eine lesende Betriebsansicht; fehlende Synchronisation zählt als produktiver Mismatch, nicht als normaler Zustand
- der Panel-Read-Only-Zugriff ist im echten Betrieb nur dann korrekt, wenn der Dini-first-Save-Pfad mit einem validen SQLite-Upsert oder einer direkten INI-Lese-Sicherheitslage konsistent bleibt

## Produktionshinweis: Dini-first vs. SQLite-Read-Only

Das Panel ist bewusst lesend auf SQLite ausgelegt, aber der Gamemode läuft in der aktuellen Architektur Dini-first über `Compiler/samp03/scriptfiles/`. Das bedeutet für den echten Betrieb:

- Jeder Dini-Save-Vorgang muss in SQLite als Upsert oder in einer synchronen Replication-Schicht nachvollzogen werden
- Ohne diese Korrelation zeigt `control-panel/server.js` veraltete Werte für Kassen, Häuser, Fahrzeuge und Spieler an
- Kritische Transfers müssen atomar sein: kein halber Write in Dini, sonst liefert das Panel falsche Zahlen aus einer älteren oder leeren Datenbank
- Das Panel ist keine Ersatzquelle für den State, sondern eine lesende Betriebsansicht auf dieselbe Produktionsdatenbasis; seine Existenz ist nur dann korrekt, wenn Dini- und SQLite-Laufpfad konsistent synchronisiert sind

Das Panel ist also keine Ersatzquelle für den State, sondern eine lesende Betriebsansicht auf dieselbe Produktionsdatenbasis.

## Veröffentlichungsstand

Das Control Panel ist als modernes Betriebs- und Monitoringmodul für den aktuellen Projektstand dokumentiert und auf den erweiterten RP-Stand des Projekts abgestimmt. Es bildet die betriebliche Sicht für Treasury, State-Management, Fahrzeug-Ownership, House-/Property-Status und Admin-Informationen. Ein echter Produktionsbetrieb ist nur freigegeben, wenn der Dini-first-Save-Pfad und der SQLite-/Autosync-Pfad auf der Ziel-VM konsistent synchronisiert sind.

## Erweiterungsplan

1. Live-Admin-Aktionen im Panel
2. Fraktions- und Business-Management-Views
3. Treasury-Charting und Cashflow-Diagramme
4. Mission-/Factory-/Transport-Panel je Staat
5. UCP-/Forum-Bridge und Auth-Integration
6. Logs und Audit-Tracking

## Wichtig

Das Panel ist bewusst nur als Betriebs- und Managementschicht konzipiert. Die eigentliche Spiel- und Besitzlogik bleibt im SA:MP-Gamemode, während das Panel als sichere, lesende Schnittstelle für Zustände, Kassen und Statusdaten dient.
