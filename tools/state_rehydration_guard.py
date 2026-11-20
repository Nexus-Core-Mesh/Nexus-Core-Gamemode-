#!/usr/bin/env python3
"""Offline guard for state rehydration and home-state safety."""

import argparse
import os
import re
from pathlib import Path

DEFAULT_PATHS = [
    'gamemodes',
    'gamemodes/modules',
]

STATE_PATTERNS = [
    ('INSERT OR IGNORE', re.compile(r'INSERT\s+OR\s+IGNORE', re.I)),
    ('state_id', re.compile(r'\bstate_id\b', re.I)),
    ('home_state', re.compile(r'\bhome_state\b', re.I)),
    ('Core_IsValidState', re.compile(r'Core_IsValidState\s*\(', re.I)),
    ('SetPlayerState', re.compile(r'Core_SetPlayerState\s*\(|SetPlayerState\s*\(', re.I)),
    ('SetPlayerHomeState', re.compile(r'Core_SetPlayerHomeState\s*\(|SetPlayerHomeState\s*\(', re.I)),
]

ZERO_OVERWRITE_PATTERNS = [
    re.compile(r'if\s*\(.*state_id.*==\s*0.*\)', re.I),
    re.compile(r'if\s*\(.*home_state.*==\s*0.*\)', re.I),
    re.compile(r'\bstate_id\s*=\s*0\b', re.I),
    re.compile(r'\bhome_state\s*=\s*0\b', re.I),
]


def file_matches(path: Path):
    name = path.name.lower()
    return path.is_file() and (name.endswith('.pwn') or name.endswith('.inc') or name.endswith('.pawn'))


def collect_files(root: Path):
    results = []
    for base in DEFAULT_PATHS:
        folder = root / base
        if not folder.exists():
            continue
        for path in sorted(folder.rglob('*')):
            if file_matches(path):
                results.append(path)
    return results


def scan_file(path: Path):
    text = path.read_text(encoding='utf-8', errors='ignore')
    findings = []
    for label, pattern in STATE_PATTERNS:
        if pattern.search(text):
            findings.append(f'OK:{label}')
    for pattern in ZERO_OVERWRITE_PATTERNS:
        if pattern.search(text):
            findings.append(f'WARN:zero-overwrite-pattern')
    if 'state_id = 0' in text or 'home_state = 0' in text:
        findings.append('WARN:assigns-zero-directly')
    return findings


def main():
    parser = argparse.ArgumentParser(description='Offline state rehydration safety audit.')
    parser.add_argument('--root', default='.', help='Repository root to scan')
    args = parser.parse_args()

    root = Path(args.root).resolve()
    files = collect_files(root)
    entries = []
    issues = 0
    for file in files:
        findings = scan_file(file)
        if findings:
            entries.append((file, findings))
            if any('WARN:' in item for item in findings):
                issues += 1

    print(f'Scanned {len(files)} Pawn source files under {root}')
    for file, findings in entries:
        print(f'[{file.relative_to(root)}]')
        for item in findings:
            print(f'  - {item}')
    print(f'\nIssue count: {issues}')
    return 0 if issues == 0 else 1


if __name__ == '__main__':
    raise SystemExit(main())
