# Server-Readme – Nexus-Core-Gamemode

Dieses Verzeichnis ist der lokale SA:MP-Laufzeitordner des Projekts.

Wichtiger Hinweis:

- Der Quellcode liegt im Repository-Root unter `gamemodes/gamemode.pwn`.
- Der aktive Build schreibt die fertige Binary direkt als `Compiler/samp03/gamemodes/gtasa_reallife.amx`.
- Im Serverordner sind keine redundanten Alias-Dateien erlaubt.
- `server.cfg` muss exakt `gamemode0 gtasa_reallife 1` verwenden.
- Die primäre Persistenz läuft Dini-first über `Compiler/samp03/scriptfiles/`; SQLite/MySQL bleiben optional und dürfen den Bootstrap nicht blockieren.

Dieser Ordner dient ausschließlich der Serverausführung und nicht der Entwicklung oder dem Bearbeiten des Source-Codes.

## Plugin-Layout und 7780-Contract

Der Laufzeitordner enthält zusätzlich das dedizierte Plugin-Layout unter `Compiler/samp03/plugins/`. Dieses Layout dokumentiert den empfohlenen Stack (`crashdetect`, `streamer`, `sscanf2`, `sqlite`, `mysql`, `zcmd`, `foreach`, `MapAndreas`, `pawn-plus`) und merkt sich sauber, dass reale `.so`-/`.dll`-Binaries nur in der Zielumgebung bzw. der Ubuntu-VM vorhanden sein dürfen. Keine erfundenen oder placeholderhaften Binärdateien werden im Repository verankert.

Der aktive Projekt-Contract bleibt auf `gtasa_reallife.amx` unter `Compiler/samp03/gamemodes/` und `port 7780` fokussiert; Port 7777 bleibt kein Projekt-Contract und wird nicht als Live-Startziel verwendet.
