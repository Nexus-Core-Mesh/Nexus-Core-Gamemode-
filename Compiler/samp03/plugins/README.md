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

Diese Plugins sind in der Zielumgebung je nach Host- / VM-Setup und Laufzeitarchitektur zu installieren. Die lokale Repository-Struktur hält bewusst keinen Platzhalter- oder Pseudo-Binary in Git fest, da die echte Plugin-ABI (`.dll` auf Windows, `.so` unter Linux/Ubuntu-VM, ggf. qemu-i386-Emulation) host- und zielabhängig ist.

## Linux-/qemu-i386-Umgebung

Für den reproduzierbaren Build-/Runtime-Contract des Projekts gilt:

- Das aktive Runtime-Target bleibt `Compiler/samp03/gamemodes/gtasa_reallife.amx`.
- Die Build-Kette läuft mit `qemu-i386` und der lokalen 32-Bit-Toolchain aus `Compiler/pawncc/`.
- Die Server-Konfiguration bleibt auf `port 7780` eingerichtet; Port 7777 wird nicht als aktiver Projekt-Contract genutzt.
- Plugins werden im Ziel-Host bzw. der Ubuntu-VM entsprechend der vorhandenen Architektur korrekt platziert; sie sind kein Bestandteil des reinen Repository-Builds, solange keine verlässlichen, projektberechtigten Binär-Downloads/Artefakte vorliegen.

## Offline- und Commit-Richtlinie

- Keine erfundenen, beschädigten oder placeholderhaften Binary-Dateien im Git-Repository einchecken.
- Wenn reale Plugin-Binaries in der Umgebung verfügbar sind, werden sie in der Ziel-VM oder dem Host-Ordner sauber installiert und dokumentiert.
- Wenn keine verlässlichen Binär-Downloads/-Artefakte vorliegen, bleibt dieser Ordner als leeres, dokumentiertes Runtime-Layout bestehen, ohne falsche Annahmen über die Laufzeitumgebung zu erzeugen.

## Verknüpfung zum Projekt

Der Plugin-Stack ergänzt die Offline-Checks und den 7780-Contract, aber er ersetzt nicht den Single-Target-Build und die Konfigurationsprüfungen:

- `tools/qemu_7780_readiness_check.py`
- `tools/runtime_contract_guard.py`
- `tools/validate_server_cfg.py`

Diese Prüfungen dienen als statische Sicherheits- und Konsistenzlayer ohne Live-Serverstart.
