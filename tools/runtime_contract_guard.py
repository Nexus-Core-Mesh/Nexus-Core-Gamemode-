#!/usr/bin/env python3
"""Offline guard for single-target runtime contract and 7780-only config hygiene."""

import argparse
import re
from pathlib import Path

FATAL_BAD_PORTS = [re.compile(r'\bport\s+7777\b', re.I), re.compile(r'\b7777\b')]
ACTIVE_PATTERNS = [
    re.compile(r'gamemode0\s+gtasa_reallife\s+1', re.I),
    re.compile(r'Compiler/samp03/gamemodes/gtasa_reallife\.amx', re.I),
    re.compile(r'gtasa_reallife\.amx', re.I),
    re.compile(r'port\s+7780', re.I),
]
ACTIVE_RELATIVE_PATHS = {
    'server.cfg',
    'config/server.cfg',
    'Compiler/samp03/server.cfg',
    'build_server.sh',
    'scripts/build_gamemode.sh',
    '.github/workflows/build.yml',
}


def should_ignore(path: Path):
    path_parts = set(path.parts)
    if 'SAMP-master' in path_parts or 'includes' in path_parts and 'SAMP-master' in path_parts:
        return True
    if 'Compiler' in path_parts and 'include' in path_parts and 'SAMP-master' in path_parts:
        return True
    if any(part.startswith('.git') for part in path.parts):
        return True
    return False


def relative_path(path: Path, root: Path):
    try:
        return path.relative_to(root).as_posix()
    except ValueError:
        return path.as_posix()


def scan_file(path: Path, root: Path):
    if should_ignore(path):
        return []

    rel = relative_path(path, root)
    if rel not in ACTIVE_RELATIVE_PATHS and path.name not in {'server.cfg', 'build_server.sh'} and 'build.yml' not in rel:
        return []

    text = path.read_text(encoding='utf-8', errors='ignore')
    problems = []
    for pattern in FATAL_BAD_PORTS:
        if pattern.search(text):
            if '7777' in rel or rel.endswith('server.cfg') or rel.endswith('build_server.sh'):
                problems.append('port-7777-reference')
    if rel.endswith('server.cfg') or rel.endswith('build_server.sh') or 'build.yml' in rel:
        if not any(pattern.search(text) for pattern in ACTIVE_PATTERNS):
            problems.append('missing-7780-target')
    return problems


def main():
    parser = argparse.ArgumentParser(description='Audit active runtime config/build files for the 7780 single-target contract.')
    parser.add_argument('--root', default='.', help='Repository root to scan')
    args = parser.parse_args()

    root = Path(args.root).resolve()
    files = []
    for rel in sorted(ACTIVE_RELATIVE_PATHS):
        p = root / rel
        if p.exists():
            files.append(p)
    files.extend(sorted(root.glob('**/server.cfg')))
    files.extend(sorted(root.glob('**/build_server.sh')))
    files.extend(sorted(root.glob('**/.github/workflows/*.yml')))
    seen = set()
    unique_files = []
    for p in files:
        if p.is_file():
            key = str(p.resolve())
            if key not in seen:
                seen.add(key)
                unique_files.append(p)

    findings = []
    for path in unique_files:
        problems = scan_file(path, root)
        if problems:
            findings.append((relative_path(path, root), problems))

    print(f'Checked {len(unique_files)} active runtime files under {root}')
    for path, problems in findings:
        print(f'[{path}]')
        for item in problems:
            print(f'  - {item}')

    if findings:
        print('\nRuntime contract drift detected in active build/config files.')
        return 1
    print('\nRuntime contract check passed: active build/config files are aligned with the 7780 contract.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
