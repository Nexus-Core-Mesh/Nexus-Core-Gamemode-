# Root-level plugin mirror

> [!NOTE]
> Diese Doku wurde optisch aufbereitet, ohne technische Fakten, Pfade, Ports oder Projektregeln zu verändern.
> Fokus lag auf Lesbarkeit, Struktur, Übersichten und deutlicherer Orientierung.

## ✅ Wichtig zu wissen

- Technische Fakten, Befehle, Pfade und Projektregeln bleiben bewusst unverändert.
- Port- und Systemtrennung bleiben konsistent, insbesondere für den 7780-Contract und den Dini-first-Pfad.
- Die optische Aufwertung dient nur der Lesbarkeit, nicht der inhaltlichen Interpretation.

---
## 🧭 Schnellüberblick

| Bereich | Inhalt |
| --- | --- |
| Dokument | `README.md` |
| Typ | Projekt-Readme |
| Fokus | Zugang, Überblick und Startpfad |
| Wichtige Pfade | `plugins/`, `Compiler/samp03/plugins/`, `Compiler/samp03/scriptfiles/` |

---
## 📘 Present and verified

- crashdetect.so - downloaded from the upstream crashdetect v4.22 Linux release
- streamer.so - downloaded from the upstream samp-streamer-plugin v2.9.6 release
- sscanf.so - downloaded from the upstream sscanf v2.13.8 Linux release
- mysql.so - downloaded from the upstream samp-plugin-mysql 2.1.1 release

## 🏗️ Not yet verified in this repository as real Linux runtime modules

- sqlite
- zcmd
- foreach
- MapAndreas
- pawn-plus

These remain pending on the actual target VM or host where the 32-bit Linux SAMP runtime is installed. No placeholder or fake binaries are committed.
