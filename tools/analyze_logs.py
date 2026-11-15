#!/usr/bin/env python3
import argparse
import json
import re
import sys
from collections import Counter
from pathlib import Path

ERROR_KEYWORDS = [
    "error",
    "failed",
    "fatal",
    "critical",
    "exception",
    "panic",
    "unable",
    "not found",
    "missing",
    "invalid",
    "could not",
    "assert",
]

PATTERN_CLASSES = {
    "plugin_error": re.compile(r"(plugin|plugins?)\s+.*(failed|error|not found|missing|load)", re.I),
    "mysql_error": re.compile(r"(mysql|sql)\s+.*(error|failed|timeout|connect|refused)", re.I),
    "pawn_error": re.compile(r"(pawn|script|gamemode|include)\s+.*(error|failed|compile|missing|not found)", re.I),
    "server_error": re.compile(r"(fatal|critical|exception|panic|assert|segmentation|access violation)", re.I),
    "file_error": re.compile(r"(cannot|can't|unable|missing|not found)\s+(open|read|write|load)\s+.*(file|config|log|script)", re.I),
    "network_error": re.compile(r"(connect|bind|listen|socket|port|address)\s+.*(failed|error|refused|in use)", re.I),
}


def iter_log_files(root: Path):
    search_roots = [root, root / "logs", root / "log"]
    files = []
    for folder in search_roots:
        if folder.exists():
            files.extend(folder.rglob("*.log"))
            files.extend(folder.rglob("*.txt"))

    for path in root.glob("*.log"):
        files.append(path)
    for path in root.glob("*.txt"):
        if "log" in path.name.lower() or "server" in path.name.lower():
            files.append(path)

    unique = []
    seen = set()
    for item in files:
        if item.is_file() and item not in seen:
            unique.append(item)
            seen.add(item)
    return unique


def categorize_line(line: str):
    text = line.strip()
    if not text:
        return None
    for name, pattern in PATTERN_CLASSES.items():
        if pattern.search(text):
            return name
    lower = text.lower()
    for keyword in ERROR_KEYWORDS:
        if keyword in lower:
            return "generic_error"
    return None


def analyze_file(path: Path):
    hits = []
    with path.open("r", encoding="utf-8", errors="replace") as handle:
        for idx, line in enumerate(handle, 1):
            category = categorize_line(line)
            if category:
                hits.append({
                    "line": idx,
                    "category": category,
                    "message": line.strip(),
                })
    return hits


def summary_report(results):
    categories = Counter()
    for item in results:
        categories[item["category"]] += 1
    return categories


def print_report(root: Path):
    log_files = iter_log_files(root)
    if not log_files:
        print(f"[INFO] Keine Logdateien unter {root} gefunden.")
        return 0

    all_hits = []
    for file in sorted(log_files):
        hits = analyze_file(file)
        if hits:
            print(f"\n=== {file.relative_to(root)} ===")
            for entry in hits[:20]:
                print(f"[{entry['category']}] L{entry['line']}: {entry['message']}")
            all_hits.extend((file, hit) for hit in hits)

    if not all_hits:
        print(f"[INFO] Keine Fehler- oder Warnungszeilen in {len(log_files)} Logdatei(en) gefunden.")
        return 0

    categories = summary_report([hit for _, hit in all_hits])
    print("\n=== Zusammenfassung ===")
    for name, count in categories.most_common():
        print(f"{name}: {count}")

    print("\n=== Empfehlungen ===")
    if any(c in categories for c in ["plugin_error", "mysql_error", "file_error", "pawn_error"]):
        print("- Prüfe zuerst Plugin- und Include-Fehler im Server-Log.")
    if "mysql_error" in categories:
        print("- Prüfe DB-Verbindungsdaten, Host, Port und MySQL-Login.")
    if "file_error" in categories:
        print("- Prüfe server.cfg, sql.cfg und das Vorhandensein der referenzierten Dateien.")
    if "pawn_error" in categories:
        print("- Prüfe kompilierten Gamemode, Includes und die Include-Reihenfolge in gamemode.pwn.")
    return 0


def parse_args():
    parser = argparse.ArgumentParser(description="Analysiert SA:MP-/GameMode-Logdateien nach Fehlern")
    parser.add_argument("path", nargs="?", default=".", help="Pfad zu Logdateien oder Projektordner")
    return parser.parse_args()


def main():
    args = parse_args()
    root = Path(args.path).resolve()
    return print_report(root)


if __name__ == "__main__":
    sys.exit(main())
