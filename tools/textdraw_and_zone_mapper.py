#!/usr/bin/env python3
"""Mappt TextDraws, PlayerTextDraws und Zonen im Source-Baum."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
SOURCE_ROOT = REPO_ROOT / "gamemodes"

TEXTDRAW_PATTERN = re.compile(r"TextDrawCreate\s*\(\s*([0-9.]+)\s*,\s*([0-9.]+)\s*,\s*(?:\"([^\"]*)\"|([^\)]*))\)")
PLAYER_TEXTDRAW_PATTERN = re.compile(r"PlayerTextDrawCreate\s*\(\s*[^,]+\s*,\s*([0-9.]+)\s*,\s*([0-9.]+)\s*,\s*(?:\"([^\"]*)\"|([^\)]*))\)")
TEXTDRAW_DESTROY_PATTERN = re.compile(r"TextDrawDestroy\s*\(\s*([A-Za-z0-9_:]+)\s*\)")
PLAYER_TEXTDRAW_DESTROY_PATTERN = re.compile(r"PlayerTextDrawDestroy\s*\(\s*[^,]+\s*,\s*([A-Za-z0-9_:]+)\s*\)")

ZONE_PATTERNS = {
    "minmax": re.compile(r"(MinX|MaxX|MinY|MaxY)\s*[:=]\s*([-0-9.]+)", re.I),
    "radius": re.compile(r"(Radius|radius)\s*[:=]\s*([-0-9.]+)", re.I),
    "center": re.compile(r"(CenterX|CenterY|CenterZ|X|Y|Z)\s*[:=]\s*([-0-9.]+)", re.I),
}


def iter_source_files(root: Path):
    for path in sorted(root.rglob("*")):
        if path.suffix.lower() in {".pwn", ".inc"}:
            yield path


def collect_textdraws(root: Path):
    textdraws = []
    player_textdraws = []
    for path in iter_source_files(root):
        text = path.read_text(encoding="utf-8", errors="replace")
        for match in TEXTDRAW_PATTERN.finditer(text):
            x, y, label = match.groups()[:3]
            textdraws.append({
                "path": str(path.relative_to(REPO_ROOT)),
                "x": float(x),
                "y": float(y),
                "label": label or "",
            })
        for match in PLAYER_TEXTDRAW_PATTERN.finditer(text):
            x, y, label = match.groups()[:3]
            player_textdraws.append({
                "path": str(path.relative_to(REPO_ROOT)),
                "x": float(x),
                "y": float(y),
                "label": label or "",
            })
    return textdraws, player_textdraws


def collect_zone_markers(root: Path):
    markers = []
    for path in iter_source_files(root):
        text = path.read_text(encoding="utf-8", errors="replace")
        found = []
        for name, pattern in ZONE_PATTERNS.items():
            for match in pattern.finditer(text):
                key, value = match.groups()
                found.append({"kind": name, "key": key, "value": value})
        if found:
            markers.append({"path": str(path.relative_to(REPO_ROOT)), "markers": found})
    return markers


def parse_args():
    parser = argparse.ArgumentParser(description="Mappt TextDraws, PlayerTextDraws und Zonen im Source-Baum.")
    parser.add_argument("--root", type=Path, default=SOURCE_ROOT, help="Pfad zum gamemodes-Ordner")
    parser.add_argument("--json", action="store_true", help="Ausgabe als JSON")
    return parser.parse_args()


def main():
    args = parse_args()
    textdraws, player_textdraws = collect_textdraws(args.root)
    zones = collect_zone_markers(args.root)
    result = {
        "textdraws": textdraws,
        "player_textdraws": player_textdraws,
        "zones": zones,
    }
    if args.json:
        print(json.dumps(result, indent=2, ensure_ascii=False))
        return 0

    print("[TEXTDRAWS]")
    for item in textdraws[:25]:
        print(f"  - {item['path']} :: ({item['x']}, {item['y']}) :: {item['label']}")
    if not textdraws:
        print("  - keine TextDraws gefunden")

    print("\n[PLAYER TEXTDRAWS]")
    for item in player_textdraws[:25]:
        print(f"  - {item['path']} :: ({item['x']}, {item['y']}) :: {item['label']}")
    if not player_textdraws:
        print("  - keine PlayerTextDraws gefunden")

    print("\n[ZONES / BOUNDS / RADIUS]")
    for zone in zones[:25]:
        print(f"  - {zone['path']} :: {len(zone['markers'])} marker(s)")
        for marker in zone["markers"][:8]:
            print(f"      {marker['key']}={marker['value']}")
    if not zones:
        print("  - keine Zonenmarker gefunden")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
