#!/usr/bin/env python3
"""Read-only ledger and state consistency scanner.

Checks SQLite state data and SQLite/scriptfiles folders for obvious value drift,
negative balances, and zero-value anomalies without starting the server.
"""

from __future__ import annotations

import argparse
import json
import sqlite3
import sys
from pathlib import Path


NUMERIC_KEYS = {
    "money", "bank", "cash", "balance", "state_id", "home_state",
    "house_id", "safe_id", "vehicle_id", "price", "amount",
    "total", "value", "debt", "profit", "loss"
}


def iter_db_tables(db_path: Path):
    conn = sqlite3.connect(f"file:{db_path}?mode=ro", uri=True)
    try:
        rows = conn.execute("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name").fetchall()
        return [row[0] for row in rows]
    finally:
        conn.close()


def inspect_db(db_path: Path):
    if not db_path.exists():
        return {"status": "missing", "path": str(db_path), "tables": []}

    conn = sqlite3.connect(f"file:{db_path}?mode=ro", uri=True)
    try:
        tables = [row[0] for row in conn.execute("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name")]
        summary = {"status": "ok", "path": str(db_path), "tables": []}
        for table in tables:
            try:
                columns = [row[1] for row in conn.execute(f"PRAGMA table_info('{table}')")]
                count = conn.execute(f"SELECT COUNT(*) FROM '{table}'").fetchone()[0]
                zero_hits = []
                negative_hits = []
                for column in columns:
                    lower = column.lower()
                    if lower in NUMERIC_KEYS or any(key in lower for key in ("money", "cash", "balance", "state", "home")):
                        try:
                            rows = conn.execute(f"SELECT COUNT(*) FROM '{table}' WHERE CAST(" + column + " AS INTEGER) = 0").fetchone()[0]
                            if rows:
                                zero_hits.append((column, rows))
                            neg_rows = conn.execute(f"SELECT COUNT(*) FROM '{table}' WHERE CAST(" + column + " AS INTEGER) < 0").fetchone()[0]
                            if neg_rows:
                                negative_hits.append((column, neg_rows))
                        except Exception:
                            continue
                summary["tables"].append({
                    "table": table,
                    "columns": columns,
                    "row_count": count,
                    "zero_like_values": zero_hits,
                    "negative_values": negative_hits,
                })
            except Exception as exc:
                summary["tables"].append({"table": table, "error": str(exc)})
        return summary
    finally:
        conn.close()


def inspect_scriptfiles(root: Path):
    base = root / "Compiler" / "samp03" / "scriptfiles"
    summary = {"path": str(base), "status": "missing", "directories": [], "files": 0}
    if not base.exists():
        return summary
    summary["status"] = "ok"
    for directory in sorted(base.rglob("*")):
        if directory.is_dir():
            summary["directories"].append(str(directory.relative_to(root)))
    summary["files"] = sum(1 for _ in base.rglob("*") if _.is_file())
    return summary


def scan_text_files(root: Path):
    issues = []
    for path in sorted((root / "Compiler" / "samp03" / "scriptfiles").rglob("*")):
        if not path.is_file():
            continue
        try:
            text = path.read_text(encoding="utf-8", errors="replace")
        except Exception:
            continue
        lowered = text.lower()
        if any(token in lowered for token in ("state_id=0", "home_state=0", "state_id= 0", "home_state= 0")):
            issues.append(f"{path}: suspicious zero state values in file content")
        if any(token in lowered for token in ("money=-", "cash=-", "balance=-", "bank=-")):
            issues.append(f"{path}: negative balance-like content detected")
    return issues


def write_snapshot(path: Path, payload: dict):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(payload, indent=2, sort_keys=True), encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description="Prüft SQLite-/Scriptfiles-Zustände ohne Serverstart.")
    parser.add_argument("--root", default=".", help="Projektwurzel")
    parser.add_argument("--snapshot", default="", help="Optionaler Pfad für einen JSON-Snapshot")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    db_path = root / "data" / "db" / "gtasa_reallife.db"
    report = {
        "db": inspect_db(db_path),
        "scriptfiles": inspect_scriptfiles(root),
        "file_issues": scan_text_files(root),
    }

    print("=== DB CHECK ===")
    print(json.dumps(report["db"], indent=2, sort_keys=True))
    print("\n=== SCRIPTFILES CHECK ===")
    print(json.dumps(report["scriptfiles"], indent=2, sort_keys=True))
    print("\n=== FILE-LEVEL ISSUES ===")
    if not report["file_issues"]:
        print("[OK] Keine offensichtlichen Null-/Negativwert-Anomalien in scriptfiles gefunden.")
    else:
        for issue in report["file_issues"]:
            print(f"[WARN] {issue}")

    if args.snapshot:
        out = Path(args.snapshot).resolve()
        write_snapshot(out, report)
        print(f"\n[INFO] Snapshot gespeichert unter {out}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
