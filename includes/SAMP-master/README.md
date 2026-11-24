# Alte SA-MP-Server-Referenz

> [!NOTE]
> Diese Doku wurde optisch aufbereitet, ohne technische Fakten, Pfade, Ports oder Projektregeln zu verändern.
> Fokus lag auf Lesbarkeit, Struktur, Übersichten und deutlicherer Orientierung.
## 🧭 Schnellüberblick

## ✅ Wichtig zu wissen

- Technische Fakten, Befehle, Pfade und Projektregeln bleiben bewusst unverändert.
- Port- und Systemtrennung bleiben konsistent, insbesondere für den 7780-Contract und den SQLite-only-Pfad.
- Die optische Aufwertung dient nur der Lesbarkeit, nicht der inhaltlichen Interpretation.

---

| Bereich | Inhalt |
| --- | --- |
| Dokument | `README.md` |
| Typ | Projekt-Readme |
| Zweck | Zugang, Überblick und Startpfad des Projekts |
| Wichtige Pfade | `gamemodes/`, `Compiler/samp03/`, `tools/` |

---

Dieses Verzeichnis enthält die veraltete SA-MP-Servervorlage und wird nicht weiter gepflegt.

Diese Datei dient nur noch als Referenz für ältere Builds und darf nicht als aktuelle Server- oder Projekt-Dokumentation verwendet werden.

Das aktive Projekt und die aktuelle Laufzeitlogik liegen im Repository-Root und im Serverordner `Compiler/samp03/`.


---

## Aktualisierter Stand (2026-11-24)

- Registrierungsflow: Passwort-Gate vor Tutorial/Spawn, M/W-Auswahl vor Skin-Picker, `DIALOG_UI_GENDER_PICK` aktiv.
- Tutorial: `TUTORIAL_SCENE_TIME` auf 11500 ms, Schutz gegen Start ohne Passwort/Geschlecht.
- Skin-Flow: `LoginSystem_ShowSkinPicker()` zeigt geschlechtsabhängige 10–15 Skins, validiert `0..311`, setzt `SetPlayerSkin` vor finalem Spawn.
- Spawn-Order: Home-State/State validiert, `StateSpawnSystem_Apply(...)` erst nach Skin-Setzung, dann `CameraSystem_ExitCameraMode`, `SetCameraBehindPlayer`, `TogglePlayerControllable(playerid, true)`, `RoleplayUI_ShowDashboard(playerid)`.
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut.
- SQLite-only + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

