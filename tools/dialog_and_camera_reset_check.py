#!/usr/bin/env python3
"""Static audit for dialog IDs, camera resets, and cleanup paths.

This tool scans the source tree for dialog flows and reset calls without
starting the SA:MP runtime.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


def scan_files(root: Path):
    files = []
    for pattern in ("**/*.pwn", "**/*.inc"):
        files.extend(root.glob(pattern))
    files = sorted({f for f in files if f.is_file()})

    dialog_calls = []
    dialog_responses = []
    camera_resets = []
    invalid_textdraws = []
    toggle_controls = []
    items = []

    for file in files:
        text = file.read_text(encoding="utf-8", errors="replace")
        for match in re.finditer(r"ShowPlayerDialog\s*\(\s*([^,\)]+)\s*,\s*([^,\)]+)\s*,\s*([^,\)]+)\s*,\s*([^,\)]+)\s*,\s*([^\)]*)\)", text):
            dialog_calls.append((file, match.group(0)))
        if re.search(r"OnDialogResponse\s*\(", text):
            dialog_responses.append(file)
        if "SetCameraBehindPlayer()" in text or "SetCameraBehindPlayer" in text:
            camera_resets.append(file)
        if "INVALID_TEXT_DRAW" in text:
            invalid_textdraws.append(file)
        if "TogglePlayerControllable(playerid, 1)" in text:
            toggle_controls.append(file)

        if "SetCameraBehindPlayer()" in text and "TogglePlayerControllable(playerid, 1)" in text:
            items.append((file, "camera_and_control_reset_guard"))
        if "INVALID_TEXT_DRAW" in text and "Text:0" in text:
            items.append((file, "invalid_textdraw_legacy_pattern"))

    return {
        "dialog_calls": dialog_calls,
        "dialog_responses": dialog_responses,
        "camera_resets": camera_resets,
        "invalid_textdraws": invalid_textdraws,
        "toggle_controls": toggle_controls,
        "items": items,
    }


def main():
    parser = argparse.ArgumentParser(description="Prüft Dialog-IDs, Camera-Resets und UI-Cleanup-Pfade offline.")
    parser.add_argument("--root", default=".", help="Projektwurzel")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    result = scan_files(root)

    print("=== Dialog calls ===")
    if not result["dialog_calls"]:
        print("[OK] Keine ShowPlayerDialog()-Aufrufe gefunden.")
    else:
        for file, call in result["dialog_calls"][:40]:
            print(f"- {file.relative_to(root)} :: {call[:120]}")

    print("\n=== Dialog response handlers ===")
    if not result["dialog_responses"]:
        print("[WARN] Keine OnDialogResponse()-Handler erkannt.")
    else:
        for file in result["dialog_responses"][:20]:
            print(f"- {file.relative_to(root)}")

    print("\n=== Reset guards ===")
    for label, values in (
        ("camera_resets", result["camera_resets"]),
        ("invalid_textdraws", result["invalid_textdraws"]),
        ("toggle_controls", result["toggle_controls"]),
    ):
        print(f"{label}: {len(values)} file(s)")
        for file in values[:10]:
            print(f"  - {file.relative_to(root)}")

    if result["items"]:
        print("\n[WARN] Potenziell veraltete oder inkonsistente Reset-/Cleanup-Muster erkannt:")
        for file, reason in result["items"]:
            print(f"- {file.relative_to(root)} :: {reason}")
        return 1

    print("\n[OK] Dialog-/Camera-/Cleanup-Checks ohne offensichtliche Mismatches.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
