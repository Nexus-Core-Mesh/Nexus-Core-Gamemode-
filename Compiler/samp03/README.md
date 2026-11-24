# Server-Readme – Nexus-Core-Gamemode

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
| Wichtige Pfade | `gamemodes/`, `Compiler/samp03/`, `tools/` |

---
## 🖥️ Plugin-Layout und 7780-Contract

Der Laufzeitordner enthält zusätzlich das dedizierte Plugin-Layout unter `Compiler/samp03/plugins/`. Dieses Layout dokumentiert den empfohlenen Stack (`crashdetect`, `streamer`, `sscanf2`, `sqlite`, `mysql`, `zcmd`, `foreach`, `MapAndreas`, `pawn-plus`) und merkt sich sauber, dass reale `.so`-/`.dll`-Binaries nur in der Zielumgebung bzw. der Ubuntu-VM vorhanden sein dürfen. Keine erfundenen oder placeholderhaften Binärdateien werden im Repository verankert.

Der aktive Projekt-Contract bleibt auf `gtasa_reallife.amx` unter `Compiler/samp03/gamemodes/` und `port 7780` fokussiert; Port 7777 bleibt kein Projekt-Contract und wird nicht als Live-Startziel verwendet.


---

## Aktualisierter Stand (2026-11-24)

- Registrierungsflow: Passwort-Gate vor Tutorial/Spawn, M/W-Auswahl vor Skin-Picker, `DIALOG_UI_GENDER_PICK` aktiv.
- Tutorial: `TUTORIAL_SCENE_TIME` auf 11500 ms, Schutz gegen Start ohne Passwort/Geschlecht.
- Skin-Flow: `LoginSystem_ShowSkinPicker()` zeigt geschlechtsabhängige 10–15 Skins, validiert `0..311`, setzt `SetPlayerSkin` vor finalem Spawn.
- Spawn-Order: Home-State/State validiert, `StateSpawnSystem_Apply(...)` erst nach Skin-Setzung, dann `CameraSystem_ExitCameraMode`, `SetCameraBehindPlayer`, `TogglePlayerControllable(playerid, true)`, `RoleplayUI_ShowDashboard(playerid)`.
- Compile: `bash scripts/build_gamemode.sh` erfolgreich; `gtasa_reallife.amx` wird ohne neue Errors gebaut (nur legacy warning in `includes/dini.inc`).
- Dini-first + state-safe Fallback: gültige State-/Home-State-Werte werden nicht auf 0 zurückgesetzt; Fallback ist `STATE_LOS_SANTOS`.

