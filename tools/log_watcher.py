#!/usr/bin/env python3
"""Manual log watcher for the SA:MP runtime.

This tool is intentionally offline and is designed for direct CLI use only.
It does not start the server, launch background daemons, or require any
external dependency beyond the Python standard library.
"""

from __future__ import annotations

import argparse
import os
import sys
import time
from pathlib import Path


def find_default_log(root: Path) -> Path | None:
    candidates = [
        root / "Compiler" / "samp03" / "server_log.txt",
        root / "samp7780_bg.log",
        root / "server_log.txt",
    ]
    for candidate in candidates:
        if candidate.exists():
            return candidate
    return None


def tail_lines(path: Path, lines: int = 60, keyword: str | None = None):
    if not path.exists():
        print(f"[WARN] Log file not found: {path}")
        return 1

    with path.open("r", encoding="utf-8", errors="replace") as handle:
        all_lines = handle.read().splitlines()

    if keyword:
        filtered = [line for line in all_lines if keyword.lower() in line.lower()]
    else:
        filtered = all_lines

    if not filtered:
        if keyword:
            print(f"[INFO] No lines containing '{keyword}' found in {path}")
        else:
            print(f"[INFO] No data found in {path}")
        return 0

    start = max(0, len(filtered) - lines)
    for line in filtered[start:]:
        print(line)
    return 0


def watch_file(path: Path, lines: int = 60, keyword: str | None = None):
    last_size = 0
    if path.exists():
        last_size = path.stat().st_size
    try:
        while True:
            if not path.exists():
                time.sleep(1)
                continue
            size = path.stat().st_size
            if size != last_size:
                with path.open("r", encoding="utf-8", errors="replace") as handle:
                    handle.seek(last_size)
                    chunk = handle.read()
                    if chunk:
                        print(chunk, end="")
                last_size = size
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n[INFO] log watcher stopped.")
    return 0


def parse_args():
    parser = argparse.ArgumentParser(description="Manual log watcher for SA:MP build and runtime logs.")
    parser.add_argument("--root", default=".", help="Project root (default: current directory)")
    parser.add_argument("--log", help="Explicit log file to inspect")
    parser.add_argument("--lines", type=int, default=60, help="Number of tail lines to show")
    parser.add_argument("--keyword", help="Optional keyword filter")
    parser.add_argument("--watch", action="store_true", help="Follow the file continuously (manual CLI only)")
    return parser.parse_args()


def main():
    args = parse_args()
    root = Path(args.root).resolve()
    log_path = Path(args.log).resolve() if args.log else find_default_log(root)

    if log_path is None:
        print("[WARN] No server log found. Supply --log or place one in Compiler/samp03/server_log.txt.")
        return 1

    if args.watch:
        print(f"[INFO] Watching {log_path} (Ctrl+C to stop)")
        return watch_file(log_path, lines=args.lines, keyword=args.keyword)

    return tail_lines(log_path, lines=args.lines, keyword=args.keyword)


if __name__ == "__main__":
    sys.exit(main())
