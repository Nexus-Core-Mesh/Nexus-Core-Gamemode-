#!/usr/bin/env python3
"""Prüft API-Mismatches in Projektleiter-, Admin-, Spawn-, Fahrzeug- und Gangfight-Pfaden."""

from __future__ import annotations

import argparse
import re
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
TARGET_EXTENSIONS = {".pwn", ".inc"}

REQUIRED_DEFINITIONS = {
    "Core_IsProjectLeader",
    "AdminSystem_IsProjectLeader",
    "IsPlayerAdmin",
    "Core_IsValidState",
    "StateSpawnSystem_Apply",
    "VehicleSystem_GetFreeSlot",
    "VehicleSystem_SpawnAdminVehicle",
    "GangFightSystem_StartFight",
    "GangFightSystem_Tick",
    "TextWars_Tick",
    "PlayerToPoint",
}

CALLABLE_PATTERNS = [
    "Core_IsProjectLeader",
    "AdminSystem_IsProjectLeader",
    "IsPlayerAdmin",
    "Core_IsValidState",
    "StateSpawnSystem_Apply",
    "VehicleSystem_GetFreeSlot",
    "VehicleSystem_SpawnAdminVehicle",
    "GangFightSystem_StartFight",
    "GangFightSystem_Tick",
    "TextWars_Tick",
]


def iter_source_files(root: Path):
    for path in sorted(root.rglob("*")):
        if path.suffix.lower() in TARGET_EXTENSIONS:
            yield path


def scan_definitions(root: Path):
    defs = set()
    for path in iter_source_files(root):
        text = path.read_text(encoding="utf-8", errors="replace")
        for match in re.finditer(r"(?:stock|public)\s+(?:bool:|int:)?([A-Za-z_][A-Za-z0-9_]*)\s*\(", text):
            defs.add(match.group(1))
    return defs


def scan_calls(root: Path):
    calls = {}
    for name in CALLABLE_PATTERNS:
        calls[name] = []
    for path in iter_source_files(root):
        text = path.read_text(encoding="utf-8", errors="replace")
        for name in CALLABLE_PATTERNS:
            for match in re.finditer(rf"\b{name}\s*\(", text):
                calls[name].append(str(path.relative_to(REPO_ROOT)))
    return calls


def parse_args():
    parser = argparse.ArgumentParser(description="Prüft Projektleiter-/Admin-/Spawn-/Fahrzeug-/Gangfight-API-Mismatches.")
    parser.add_argument("--root", type=Path, default=REPO_ROOT, help="Pfad zum Repository")
    return parser.parse_args()


def main():
    args = parse_args()
    root = args.root.resolve()
    defined = scan_definitions(root)
    call_map = scan_calls(root)

    missing_defs = sorted(name for name in REQUIRED_DEFINITIONS if name not in defined)
    if missing_defs:
        print("[MISSING DEFINITIONS]")
        for name in missing_defs:
            print(f"  - {name}")
    else:
        print("[MISSING DEFINITIONS] none")

    print("\n[CALL SITES]")
    for name in sorted(call_map):
        files = sorted(set(call_map[name]))
        if files:
            print(f"  - {name}: {len(files)} Aufruf(e)")
            for item in files[:5]:
                print(f"      {item}")
        else:
            print(f"  - {name}: no matches")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
