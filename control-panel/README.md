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

Danach ist das Dashboard unter `http://localhost:3001/` erreichbar.

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

## Veröffentlichungsstand

Das Control Panel ist als modernes Betriebs- und Monitoringmodul veröffentlicht und auf den erweiterten RP-Stand des Projekts abgestimmt. Es bildet die betriebliche Sicht für Treasury, State-Management, Fahrzeug-Ownership, House-/Property-Status und Admin-Informationen.

## Erweiterungsplan

1. Live-Admin-Aktionen im Panel
2. Fraktions- und Business-Management-Views
3. Treasury-Charting und Cashflow-Diagramme
4. Mission-/Factory-/Transport-Panel je Staat
5. UCP-/Forum-Bridge und Auth-Integration
6. Logs und Audit-Tracking

## Wichtig

Das Panel ist bewusst nur als Betriebs- und Managementschicht konzipiert. Die eigentliche Spiel- und Besitzlogik bleibt im SA:MP-Gamemode, während das Panel als sichere, lesende Schnittstelle für Zustände, Kassen und Statusdaten dient.
