#!/usr/bin/env python3
"""Manual debug trace collector for offline inspection.

This tool is intentionally read-only and does not start the server. It creates a
compact diagnostic snapshot from logs, DB metadata, scriptfiles, and build files.
"""

from __future__ import annotations

import argparse
import json
import sqlite3
import sys
from pathlib import Path


def read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except FileNotFoundError:
        return ""


def collect_db_info(db_path: Path):
    if not db_path.exists():
        return {"present": False, "error": "database missing"}
    conn = sqlite3.connect(f"file:{db_path}?mode=ro", uri=True)
    tables = []
    try:
        for row in conn.execute("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name"):
            table = row[0]
            columns = conn.execute(f"PRAGMA table_info({table})").fetchall()
            row_count = conn.execute(f"SELECT COUNT(*) FROM {table}").fetchone()[0]
            tables.append({
                "name": table,
                "columns": [column[1] for column in columns],
                "row_count": row_count,
            })
    finally:
        conn.close()
    return {"present": True, "tables": tables}


def collect_scriptfiles(root: Path):
    base = root / "Compiler" / "samp03" / "scriptfiles"
    if not base.exists():
        return {"present": False, "directories": []}
    dirs = []
    for path in sorted(base.rglob("*")):
        if path.is_dir():
            dirs.append(str(path.relative_to(root)))
    return {"present": True, "directories": dirs}


def collect_build_artifacts(root: Path):
    artifacts = []
    for path in sorted((root / "Compiler" / "samp03").rglob("*")):
        if path.is_file() and path.suffix.lower() in {".amx", ".db", ".log", ".cfg", ".txt", ".ini"}:
            stat = path.stat()
            artifacts.append({
                "path": str(path.relative_to(root)),
                "size": stat.st_size,
                "mtime": int(stat.st_mtime),
            })
    return artifacts


def collect_logs(root: Path):
    log_paths = []
    for candidate in [
        root / "Compiler/samp03/server_log.txt",
        root / "samp7780_bg.log",
        root / "server_log.txt",
    ]:
        if candidate.exists():
            log_paths.append(str(candidate.relative_to(root)))
    findings = []
    for log_path in log_paths:
        log_file = root / log_path
        lines = read_text(log_file).splitlines()
        for idx, line in enumerate(lines[:80], start=1):
            if any(token in line.lower() for token in ["error", "warning", "failed", "panic", "sscanf", "state", "camera", "textdraw", "spawn", "db"]):
                findings.append({"file": log_path, "line": idx, "text": line.strip()})
    return {"log_files": log_paths, "findings": findings}


def main():
    parser = argparse.ArgumentParser(description="Collect a compact offline debug trace for the 7780 project state.")
    parser.add_argument("--root", default=".", help="Project root")
    parser.add_argument("--output", help="Optional output path for JSON export")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    payload = {
        "database": collect_db_info(root / "data" / "db" / "gtasa_reallife.db"),
        "scriptfiles": collect_scriptfiles(root),
        "build_artifacts": collect_build_artifacts(root),
        "logs": collect_logs(root),
    }

    if args.output:
        out = Path(args.output)
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(json.dumps(payload, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        print(f"[OK] Wrote debug trace to {out}")
        return 0

    print(json.dumps(payload, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(main())
