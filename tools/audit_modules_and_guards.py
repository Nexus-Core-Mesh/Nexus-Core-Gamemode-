#!/usr/bin/env python3
"""Audit der Kern-Guards, Lifecycle- und Include-Checks im Gamemode-Quelltext."""

from __future__ import annotations

import argparse
import re
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
GAMEMODE = REPO_ROOT / "gamemodes" / "gamemode.pwn"
MODULES = REPO_ROOT / "gamemodes" / "modules"

REQUIRED_INCLUDE_ORDER = [
    "core.inc",
    "db",
    "state",
    "admin",
    "auth",
    "economy",
    "jobs",
    "laws",
    "housing",
    "vehicle",
    "leisure",
    "web",
]

TARGET_GUARDS = {
    "insert_or_ignore": "INSERT OR IGNORE",
    "core_is_valid_state": "Core_IsValidState()",
    "camera_reset": "SetCameraBehindPlayer()",
    "camera_toggle": "TogglePlayerControllable",
    "invalid_text_draw": "INVALID_TEXT_DRAW",
    "sscanf_hint": "sscanf",
    "tick_guard": "TextWars_Tick",
}


def read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return ""


def get_include_order(path: Path):
    if not path.exists():
        return []
    text = read_text(path)
    includes = []
    for match in re.finditer(r'#include\s+["<](.+?)[">]', text):
        includes.append(match.group(1))
    return includes


def find_guard_hits(target_root: Path):
    hits = {}
    for key, needle in TARGET_GUARDS.items():
        hits[key] = []
    for file in sorted(target_root.rglob("*")):
        if file.suffix.lower() not in {".pwn", ".inc"}:
            continue
        text = read_text(file)
        for key, needle in TARGET_GUARDS.items():
            if needle in text:
                hits[key].append(str(file.relative_to(REPO_ROOT)))
    return hits


def check_lifecycle(text: str):
    required = [
        "CORE_LIFECYCLE_BOOT",
        "DB_READY",
        "STATE_READY",
        "AUTH_READY",
        "WORLD_READY",
        "RUNTIME_READY",
    ]
    found = []
    for marker in required:
        if marker in text:
            found.append(marker)
    return found


def scan_tick_and_loop_issues(text: str):
    items = []
    if "TextWars_Tick" in text:
        items.append("TextWars_Tick")
    if "GangFightSystem_Tick" in text:
        items.append("GangFightSystem_Tick")
    if re.search(r"for\s*\(\s*new\s+\w+\s*=\s*0\s*;\s*\w+\s*<\s*MAX_PLAYERS", text):
        items.append("MAX_PLAYERS loop")
    if "OnPlayerUpdate" in text:
        items.append("OnPlayerUpdate")
    if "Timer" in text and "CreateTimer" in text:
        items.append("Timer")
    return items


def parse_args():
    parser = argparse.ArgumentParser(description="Auditiert Gamemode-Guards und die Architektur-Checks.")
    parser.add_argument("--root", type=Path, default=REPO_ROOT, help="Projektwurzel")
    return parser.parse_args()


def main():
    args = parse_args()
    root = args.root.resolve()
    gamemode_path = root / "gamemodes" / "gamemode.pwn"
    modules_root = root / "gamemodes" / "modules"

    print("[INCLUDE ORDER]")
    include_order = get_include_order(gamemode_path)
    for item in include_order:
        print(f"  - {item}")
    print()

    lifecycle_text = read_text(gamemode_path)
    lifecycle_found = check_lifecycle(lifecycle_text)
    print("[LIFECYCLE] ")
    if lifecycle_found:
        for name in lifecycle_found:
            print(f"  - {name}")
    else:
        print("  - keine Lifecycle-Marker gefunden")
    print()

    print("[GUARD HIT SUMMARY]")
    hits = find_guard_hits(modules_root)
    for key, value in hits.items():
        print(f"  - {key}: {len(value)} Datei(en)")
        for entry in value[:5]:
            print(f"      {entry}")
    print()

    print("[TICK / LOOP / HOTPATH GUARDS]")
    loop_hits = []
    for file in sorted(modules_root.rglob("*.inc")):
        text = read_text(file)
        found = scan_tick_and_loop_issues(text)
        if found:
            loop_hits.append((file.relative_to(root).as_posix(), found))
    if not loop_hits:
        print("  - keine Tick-/Loop-Guard-Notizen gefunden")
    else:
        for rel_path, markers in loop_hits[:10]:
            print(f"  - {rel_path}: {', '.join(markers)}")
    print()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
