#!/usr/bin/env python3
"""Offline validation for gangfight and world-zone definitions.

This tool scans the source tree for zone-related data and reports suspicious
rectangle or radius-zone definitions without starting the SA:MP server.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


RECTANGLE_KEYS = ("MinX", "MaxX", "MinY", "MaxY")
CIRCLE_KEYS = ("X", "Y", "Z", "Radius")
TEXT_SUFFIXES = (".pwn", ".inc", ".txt")


def collect_files(root: Path):
    files = []
    for pattern in ("**/*.pwn", "**/*.inc", "**/*.txt"):
        files.extend(root.glob(pattern))
    unique = []
    seen = set()
    for file in files:
        if file.is_file() and file not in seen:
            unique.append(file)
            seen.add(file)
    return sorted(unique)


def scan_file(path: Path):
    text = path.read_text(encoding="utf-8", errors="replace")
    findings = []
    rect = {key: [] for key in RECTANGLE_KEYS}
    circle = {key: [] for key in CIRCLE_KEYS}

    for key in RECTANGLE_KEYS:
        for match in re.finditer(rf"\b{key}\b\s*[:=]\s*([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)", text):
            rect[key].append(float(match.group(1)))
    for key in CIRCLE_KEYS:
        for match in re.finditer(rf"\b{key}\b\s*[:=]\s*([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)", text):
            circle[key].append(float(match.group(1)))

    if any(rect.values()):
        values = {k: v for k, v in rect.items() if v}
        if len({len(v) for v in values.values()}) != 1:
            findings.append(("rectangle_mismatch", f"{path}: unvollständige Rechteck-Koordinaten: {values}"))
        else:
            mins = [values.get("MinX", 0), values.get("MinY", 0)]
            maxs = [values.get("MaxX", 0), values.get("MaxY", 0)]
            if mins[0] > maxs[0] or mins[1] > maxs[1]:
                findings.append(("rectangle_flip", f"{path}: Min/Max vertauscht: {values}"))
            if maxs[0] - mins[0] > 20000 or maxs[1] - mins[1] > 20000:
                findings.append(("rectangle_large", f"{path}: Rechteck sehr groß: {values}"))

    if any(circle.values()):
        radius_values = circle.get("Radius", [])
        x_values = circle.get("X", [])
        y_values = circle.get("Y", [])
        z_values = circle.get("Z", [])
        if radius_values and any(value <= 0 for value in radius_values):
            findings.append(("radius_invalid", f"{path}: Radius <= 0 gefunden: {radius_values}"))
        if radius_values and any(value > 2000 for value in radius_values):
            findings.append(("radius_large", f"{path}: Radius > 2000 gefunden: {radius_values}"))
        if x_values and y_values and not any(x_values or y_values):
            findings.append(("circle_empty", f"{path}: Kreiszentrum nicht plausibel"))
        if not radius_values and (x_values or y_values or z_values):
            findings.append(("circle_partial", f"{path}: Teilweise Kreis-/Zentrumsdaten ohne Radius: {circle}"))

    zone_hint = re.search(r"(?i)(zone|gangfight|arena|territory|safezone)", text)
    if zone_hint and not any(v for v in rect.values()) and not any(v for v in circle.values()):
        findings.append(("zone_missing_data", f"{path}: Zone-/Gangfight-Bezug ohne erkennbare Koordinaten"))

    return findings


def main():
    parser = argparse.ArgumentParser(description="Prüft Rechteck- und Kreis-Zonen auf Plausibilität.")
    parser.add_argument("--root", default=".", help="Projektwurzel")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    findings = []
    for file in collect_files(root):
        findings.extend(scan_file(file))

    if not findings:
        print(f"[OK] Keine auffälligen Rechteck-/Radius-Zonendefinitionen unter {root} gefunden.")
        return 0

    print(f"[WARN] {len(findings)} verdächtige Zone(n) gefunden:")
    for kind, message in findings[:80]:
        print(f"- [{kind}] {message}")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
