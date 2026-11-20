# SA:MP-Plugins für den 7780-Laufpfad

Dieses Verzeichnis ist der dedizierte Plugin-Ordner für die lokale `Compiler/samp03/`-Runtime. Es dient ausschließlich der Offline-/Runtime-Organisation des Projekts; es enthält keine Live-Server-Startlogik und keine unautorisierte Port-7777-Konfiguration.

## Verwendeter Plugin-Stack

Der empfohlene SA:MP-Plugin-Stack für den 7780-Contract lautet:

- `crashdetect`
- `streamer`
- `sscanf2`
- `sqlite`
- `mysql`
- `zcmd`
- `foreach`
- `MapAndreas`
- `pawn-plus`

Für diese Umgebung wurden die verifizierten Linux-32-Bit-Binaries für den aktuell sicher nachweisbaren Teil des Stacks direkt heruntergeladen und im Projektordner abgelegt:

- `crashdetect.so` – `crashdetect` v4.22 Linux release
- `streamer.so` – `samp-streamer-plugin` v2.9.6 release
- `sscanf.so` – `sscanf` v2.13.8 Linux release
- `mysql.so` – `samp-plugin-mysql` 2.1.1 release

Diese Dateien wurden vor dem Ablegen als ELF-32-Bit-Shared-Objects verifiziert und sind hier im Repository mit eigener `plugins/`-Bestandsliste enthalten. Weitere Plugins aus dem Stack (`sqlite`, `zcmd`, `foreach`, `MapAndreas`, `pawn-plus`) konnten in dieser Umgebung nicht zuverlässig als echte Linux-32-Bit-ABI-Module gefunden bzw. verifiziert werden und sind daher noch als „auf Ziel-VM/Host nachzuziehen“ dokumentiert.

## Linux-/qemu-i386-Umgebung

Die Laufzeitstruktur bleibt strikt auf den 7780-Contract ausgerichtet: `gamemode0 gtasa_reallife 1` und `port 7780` sind die gültigen Werte; kein `7777`-Eintrag darf im Serverkonfig-/Runtime-Pfad aktiv sein. Der Plugin-Ordner dient nicht als Live-Start, sondern als statische Laufzeit-/Doc-Collection für den QEMU-i386-/Linux-32-Bit-Host. Die im Repository vorhandenen `.so`-Dateien wurden verifiziert und sind als reale ELF-32-Bit-Binaries markiert; keine Platzhalter, 404-HTML-Seiten oder Fake-Mechaniken wurden als Plugin-Dateien gespeichert.

Die physische Dini-/Persistenzstruktur unter `Compiler/samp03/scriptfiles/` wurde ebenfalls ergänzt. Dadurch bleiben Laufzeitpfade wie `players`, `houses`, `safes`, `vehicles`, `ubb_vehicles`, `state/LS`, `state/SF`, `state/LV`, `state_kasse/*` und `temp` trotz offline-Validierung immer vorhanden und bereit für das sichere `temp -> validate -> rename`-Schema.

Für den reproduzierbaren Build-/Runtime-Contract des Projekts gilt:

- Das aktive Runtime-Target bleibt `Compiler/samp03/gamemodes/gtasa_reallife.amx`.
- Die Build-Kette läuft mit `qemu-i386` und der lokalen 32-Bit-Toolchain aus `Compiler/pawncc/`.
- Die Server-Konfiguration bleibt auf `port 7780` eingerichtet; Port 7777 wird nicht als aktiver Projekt-Contract genutzt.
- Plugins werden im Ziel-Host bzw. der Ubuntu-VM entsprechend der vorhandenen Architektur korrekt platziert; sie sind kein Bestandteil des reinen Repository-Builds, solange keine verlässlichen, projektberechtigten Binär-Downloads/Artefakte vorliegen.

## Offline- und Commit-Richtlinie

- Keine erfundenen, beschädigten oder placeholderhaften Binary-Dateien im Git-Repository einchecken.
- Wenn reale Plugin-Binaries in der Umgebung verfügbar sind, werden sie in der Ziel-VM oder dem Host-Ordner sauber installiert und dokumentiert.
- Wenn keine verlässlichen Binär-Downloads/-Artefakte vorliegen, bleibt dieser Ordner als leeres, dokumentiertes Runtime-Layout bestehen, ohne falsche Annahmen über die Laufzeitumgebung zu erzeugen.

## Aktueller Stand in dieser Umgebung (2026-11-20)

Für die hier ausgeführte Prüfung waren GitHub-/Release-Endpunkte und direkte Downloadpfade für die empfohlenen SA:MP-Plugins zwar teilweise erreichbar. In der vorliegenden Umgebung konnten verifizierte 32-Bit-Linux-`*.so`-Artefakte für den nachweisbar verfügbaren Teil des Stacks sauber heruntergeladen und direkt im Repository abgelegt werden:

- `crashdetect.so`
- `streamer.so`
- `sscanf.so`
- `mysql.so`

Echte, verifizierte 32-Bit-Linux-Binaries für den vollständigen Stack (`sqlite`, `zcmd`, `foreach`, `MapAndreas`, `pawn-plus`) konnten in dieser Umgebung nicht reproduzierbar und sicher als projektberechtigte Binärdateien bestätigt werden. Einzelne Endpunkte lieferten 404, HTML-Seiten, Leerseiten oder keine belastbaren Release-Assets mit 32-Bit/Ubuntu-Ziel-ABI.

Das bedeutet exakt: Der Repository-Stand enthält derzeit echte, verifizierte ELF-32-Bit-Plugin-Binaries für den nachweisbaren Teil des Stacks; der komplette Plugin-Stack bleibt aber als Ziel-VM-/Host-Installation offen, sofern die restlichen 32-Bit-Linux-Binaries dort vertrauenswürdig bereitgestellt werden. Keine Platzhalter-/Fake-Binaries wurden erzeugt. Der Projekt-Contract bleibt weiterhin auf `port 7780`, `gamemode0 gtasa_reallife 1` und `Compiler/samp03/gamemodes/gtasa_reallife.amx` fixiert.

## Verknüpfung zum Projekt

Der Plugin-Stack ergänzt die Offline-Checks und den 7780-Contract, aber er ersetzt nicht den Single-Target-Build und die Konfigurationsprüfungen:

- `tools/qemu_7780_readiness_check.py`
- `tools/runtime_contract_guard.py`
- `tools/validate_server_cfg.py`

Diese Prüfungen dienen als statische Sicherheits- und Konsistenzlayer ohne Live-Serverstart.
