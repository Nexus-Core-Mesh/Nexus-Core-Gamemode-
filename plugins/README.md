# Root-level plugin mirror

> [!NOTE]
> Diese Doku wurde optisch aufbereitet, ohne technische Fakten, Pfade, Ports oder Projektregeln zu verändern.
> Fokus lag auf Lesbarkeit, Struktur, Übersichten und deutlicherer Orientierung.

## ✅ Wichtig zu wissen

- Technische Fakten, Befehle, Pfade und Projektregeln bleiben bewusst unverändert.
- Port- und Systemtrennung bleiben konsistent, insbesondere für den 7780-Contract und den SQLite-only-Pfad.
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


---

## Aktualisierter Stand (2026-11-24)

- Registrierungsflow: Passwort-Gate vor Tutorial/Spawn, M/W-Auswahl vor Skin-Picker, `DIALOG_UI_GENDER_PICK` aktiv.
- Tutorial: `TUTORIAL_SCENE_TIME` auf 11500 ms, Schutz gegen Start ohne Passwort/Geschlecht.
- Skin-Flow: `LoginSystem_ShowSkinPicker()` zeigt geschlechtsabhängige 10–15 Skins, validiert `0..311`, setzt `SetPlayerSkin` vor finalem Spawn.
- Spawn-Order: Home-State/State validiert, `StateSpawnSystem_Apply(...)` erst nach Skin-Setzung, dann `CameraSystem_ExitCameraMode`, `SetCameraBehindPlayer`, `TogglePlayerControllable(playerid, true)`, `RoleplayUI_ShowDashboard(playerid)`.
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut.
- SQLite-only + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

