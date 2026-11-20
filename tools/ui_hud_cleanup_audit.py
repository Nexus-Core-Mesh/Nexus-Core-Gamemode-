#!/usr/bin/env python3
"""Offline audit for camera reset, textdraw cleanup, and HUD cleanup patterns."""

import argparse
import re
from pathlib import Path

TARGETS = ['gamemodes', 'gamemodes/modules']
CHECKS = {
    'INVALID_TEXT_DRAW': re.compile(r'INVALID_TEXT_DRAW', re.I),
    'SetCameraBehindPlayer': re.compile(r'SetCameraBehindPlayer\s*\(', re.I),
    'TogglePlayerControllable': re.compile(r'TogglePlayerControllable\s*\(', re.I),
    'Text:0': re.compile(r'!=\s*Text:\s*0|==\s*Text:\s*0', re.I),
    'TextDrawDestroy': re.compile(r'TextDrawDestroy\s*\(', re.I),
    'PlayerTextDrawDestroy': re.compile(r'PlayerTextDrawDestroy\s*\(', re.I),
}


def find_files(root: Path):
    out = []
    for base in TARGETS:
        folder = root / base
        if folder.exists():
            out.extend(sorted(folder.rglob('*')))
    return [p for p in out if p.is_file() and p.suffix.lower() in {'.pwn', '.inc', '.pawn'}]


def main():
    parser = argparse.ArgumentParser(description='Offline audit for camera reset, TextDraw cleanup, and HUD reset guardrails.')
    parser.add_argument('--root', default='.', help='Repository root to scan')
    args = parser.parse_args()

    root = Path(args.root).resolve()
    files = find_files(root)
    findings = []
    for path in files:
        text = path.read_text(encoding='utf-8', errors='ignore')
        matches = []
        for label, pattern in CHECKS.items():
            if pattern.search(text):
                matches.append(label)
        if matches:
            findings.append((path.relative_to(root), matches))

    print(f'Scanned {len(files)} Pawn source files under {root}')
    for path, matches in findings:
        print(f'[{path}]')
        for item in matches:
            print(f'  - {item}')

    if not findings:
        print('No HUD/camera/textdraw cleanup patterns found; check was inconclusive.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
