# Server-Readme – Nexus-Core-Gamemode

Dieses Verzeichnis ist der lokale SA:MP-Laufzeitordner des Projekts.

Wichtiger Hinweis:

- Der Quellcode liegt im Repository-Root unter `gamemodes/gamemode.pwn`.
- Der aktive Build schreibt die fertige Binary direkt als `Compiler/samp03/gamemodes/gtasa_reallife.amx`.
- Im Serverordner sind keine redundanten Alias-Dateien erlaubt.
- `server.cfg` muss exakt `gamemode0 gtasa_reallife 1` verwenden.
- Die primäre Persistenz läuft Dini-first über `Compiler/samp03/scriptfiles/`; SQLite/MySQL bleiben optional und dürfen den Bootstrap nicht blockieren.

Dieser Ordner dient ausschließlich der Serverausführung und nicht der Entwicklung oder dem Bearbeiten des Source-Codes.
