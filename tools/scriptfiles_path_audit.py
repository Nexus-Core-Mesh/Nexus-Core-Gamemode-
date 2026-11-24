#!/usr/bin/env python3
"""Offline audit of scriptfiles persistence paths against the source tree."""

import argparse
import re
from pathlib import Path

EXPECTED_DIRS = [
    'players', 'houses', 'house_storage', 'safes', 'vehicles', 'ubb_vehicles',
    'state/LS', 'state/SF', 'state/LV',
    'state_kasse/LS', 'state_kasse/SF', 'state_kasse/LV',
    'business', 'factions', 'inventory', 'temp', 'backup', 'logs'
]

PERSIST_PATTERNS = [
    'players/', 'houses/', 'house_storage/', 'safes/', 'vehicles/', 'ubb_vehicles/',
    'state/LS', 'state/SF', 'state/LV', 'state_kasse/LS', 'state_kasse/SF', 'state_kasse/LV',
    'business/', 'factions/', 'inventory/', 'temp/', 'backup/', 'logs/'
]

PATH_RE = re.compile(r'(?:(?:[A-Za-z0-9_./-]+/)?(?:players|houses|house_storage|safes|vehicles|ubb_vehicles|state|state_kasse|business|factions|inventory|temp|backup|logs))(?:/|\\)')


def find_project_files(root: Path):
    files = []
    for rel in ['gamemodes/gamemode.pwn', 'gamemodes/modules']:
        p = root / rel
        if p.is_file():
            files.append(p)
        elif p.is_dir():
            files.extend(sorted(p.rglob('*.pwn')))
            files.extend(sorted(p.rglob('*.inc')))
    return files


def audit_scriptfiles(root: Path):
    scriptfiles = root / 'Compiler' / 'samp03' / 'scriptfiles'
    missing_dirs = []
    for rel in EXPECTED_DIRS:
        full = scriptfiles / rel
        if not full.exists():
            missing_dirs.append(rel)

    source_hits = []
    for file in find_project_files(root):
        try:
            text = file.read_text(encoding='utf-8', errors='ignore')
        except OSError:
            continue
        if not text:
            continue
        for pattern in PERSIST_PATTERNS:
            if pattern in text:
                source_hits.append((file.relative_to(root).as_posix(), pattern))

    # dedupe and keep stable order
    seen = set()
    unique_hits = []
    for file, pattern in source_hits:
        key = (file, pattern)
        if key not in seen:
            seen.add(key)
            unique_hits.append((file, pattern))

    return missing_dirs, unique_hits


def main():
    parser = argparse.ArgumentParser(description='Validate SQLite-only scriptfiles paths against the source tree.')
    parser.add_argument('--root', default='.', help='Project root to scan')
    args = parser.parse_args()

    root = Path(args.root).resolve()
    missing_dirs, source_hits = audit_scriptfiles(root)

    print(f'Project root: {root}')
    print(f'Scriptfiles root: {root / "Compiler" / "samp03" / "scriptfiles"}')

    if missing_dirs:
        print('\nMissing expected scriptfiles directories:')
        for rel in missing_dirs:
            print(f' - {rel}')
    else:
        print('\nAll expected scriptfiles directories are present.')

    if source_hits:
        print('\nSource references to persistence paths:')
        for file, pattern in source_hits:
            print(f' - {file}: {pattern}')
    else:
        print('\nNo persistence path references found in source files.')

    return 0 if not missing_dirs else 1


if __name__ == '__main__':
    raise SystemExit(main())
