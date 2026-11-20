#!/usr/bin/env python3
"""Offline season/state consistency audit for the source tree."""

import argparse
import re
from pathlib import Path

REQUIRED_SEASON_MARKERS = [
    'SeasonSystem_Initialize',
    'SeasonSystem_GetEconomyMultiplier',
    'SeasonSystem_GetJobModifier',
    'StateLedgerSystem_Transfer',
    'STATE_LOS_SANTOS',
    'STATE_SAN_FIERRO',
    'STATE_LAS_VENTURAS'
]


def scan_for_markers(root: Path):
    files = []
    for pattern in ['gamemodes/**/*.pwn', 'gamemodes/**/*.inc']:
        files.extend(root.glob(pattern))
    results = {}
    for marker in REQUIRED_SEASON_MARKERS:
        results[marker] = []
    for file in sorted(set(files)):
        if file.is_file():
            try:
                text = file.read_text(encoding='utf-8', errors='ignore')
            except OSError:
                continue
            for marker in REQUIRED_SEASON_MARKERS:
                if marker in text:
                    results[marker].append(file.relative_to(root).as_posix())
    return results


def check_state_dirs(root: Path):
    scriptfiles = root / 'Compiler' / 'samp03' / 'scriptfiles'
    required = [
        scriptfiles / 'state' / 'LS',
        scriptfiles / 'state' / 'SF',
        scriptfiles / 'state' / 'LV',
        scriptfiles / 'state_kasse' / 'LS',
        scriptfiles / 'state_kasse' / 'SF',
        scriptfiles / 'state_kasse' / 'LV',
    ]
    missing = [str(p.relative_to(root)) for p in required if not p.exists()]
    return missing


def main():
    parser = argparse.ArgumentParser(description='Check season + state integration consistency in source files.')
    parser.add_argument('--root', default='.', help='Project root to scan')
    args = parser.parse_args()

    root = Path(args.root).resolve()
    markers = scan_for_markers(root)
    missing_dirs = check_state_dirs(root)

    print(f'Project root: {root}')
    print('\nMarker coverage:')
    for marker in REQUIRED_SEASON_MARKERS:
        hits = markers[marker]
        print(f' - {marker}: {len(hits)} file(s)')
        for hit in hits[:5]:
            print(f'     {hit}')

    if missing_dirs:
        print('\nMissing state directories:')
        for item in missing_dirs:
            print(f' - {item}')
    else:
        print('\nAll required state subdirectories are present.')

    if missing_dirs or any(len(v) == 0 for v in markers.values()):
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
