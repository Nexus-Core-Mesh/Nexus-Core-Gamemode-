#!/usr/bin/env python3
import argparse
import os
import sys
import time
from pathlib import Path

DEFAULT_KEYWORDS = [
    "error",
    "failed",
    "warning",
    "fatal",
    "critical",
    "plugin",
    "mysql",
    "sqlite",
    "sql",
    "pawn",
    "assert",
    "exception",
]


def find_candidate_logs(root: Path):
    candidates = []
    search_roots = [root, root / "logs", root / "log"]
    for folder in search_roots:
        if folder.exists():
            for item in folder.rglob("*.log"):
                candidates.append(item)
            for item in folder.rglob("*.txt"):
                if "server" in item.name.lower() or "log" in item.name.lower():
                    candidates.append(item)
    seen = set()
    ordered = []
    for p in candidates:
        if p not in seen:
            seen.add(p)
            ordered.append(p)
    return ordered


def tail_file(path: Path, lines: int = 60, keyword_filter: str = None):
    if not path.exists():
        print(f"[WARN] Logdatei nicht gefunden: {path}")
        return

    with path.open("r", encoding="utf-8", errors="replace") as handle:
        all_lines = handle.read().splitlines()

    if keyword_filter:
        filtered = [line for line in all_lines if keyword_filter.lower() in line.lower()]
    else:
        filtered = all_lines

    if not filtered:
        print(f"[INFO] Keine Einträge für '{keyword_filter}' in {path.name} gefunden.")
        return

    start = max(0, len(filtered) - lines)
    for line in filtered[start:]:
        print(line)


def parse_args():
    parser = argparse.ArgumentParser(description="Live-Log Viewer für SA:MP-Server")
    parser.add_argument("--log", type=str, default="", help="Pfad zur Logdatei")
    parser.add_argument("--keyword", type=str, default="", help="Nur Zeilen mit Stichwort anzeigen")
    parser.add_argument("--lines", type=int, default=60, help="Anzahl der zuletzt anzuzeigenden Zeilen")
    parser.add_argument("--watch", action="store_true", help="Datei in Echtzeit beobachten")
    parser.add_argument("--root", type=str, default=".", help="Projektwurzel")
    return parser.parse_args()


def main():
    args = parse_args()
    root = Path(args.root).resolve()
    log_path = None

    if args.log:
        log_path = Path(args.log).resolve()
    else:
        candidates = find_candidate_logs(root)
        if candidates:
            log_path = candidates[0]
            print(f"[INFO] Nutze Logdatei: {log_path}")
        else:
            print("[WARN] Keine Logdatei im Projekt gefunden. Beispiel: ./logs/server.log")
            return 1

    if not log_path.exists():
        print(f"[WARN] Datei nicht gefunden: {log_path}")
        return 1

    if args.watch:
        print(f"[INFO] Beobachte {log_path} (Ctrl+C zum Beenden)")
        last_size = 0
        while True:
            try:
                if log_path.exists():
                    size = log_path.stat().st_size
                    if size != last_size:
                        with log_path.open("r", encoding="utf-8", errors="replace") as handle:
                            handle.seek(max(0, last_size))
                            data = handle.read()
                            if data:
                                print(data, end="")
                        last_size = size
                time.sleep(1)
            except KeyboardInterrupt:
                print("\n[INFO] Log-Observer beendet.")
                break
        return 0

    keyword = args.keyword if args.keyword else ""
    tail_file(log_path, lines=args.lines, keyword_filter=keyword)
    return 0


if __name__ == "__main__":
    sys.exit(main())
