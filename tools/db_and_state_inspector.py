#!/usr/bin/env python3
"""Schreibgeschützte Inspektion von SQLite-State und scriptfiles-Persistenz.

Ziel: die kritischen Datenpfade ohne Serverstart prüfen, damit DB- und SQLite-Zustände
vor einem Live-Upgrade schnell gegen Inkonsistenzen bewertet werden können.
"""

from __future__ import annotations

import argparse
import json
import sqlite3
from pathlib import Path
from typing import Iterable, List

REPO_ROOT = Path(__file__).resolve().parents[1]
DB_PATH = REPO_ROOT / "data" / "db" / "gtasa_reallife.db"
SCRIPTFILES_PATH = REPO_ROOT / "Compiler" / "samp03" / "scriptfiles"

CANDIDATE_COLUMNS = ("state_id", "home_state", "player_state")


def quote_identifier(name: str) -> str:
    return '"' + name.replace('"', '""') + '"'


def walk_files(root: Path) -> List[Path]:
    if not root.exists():
        return []
    return sorted(p for p in root.rglob("*") if p.is_file())


def summarize_scriptfiles(root: Path):
    directories = []
    files = 0
    sizes = 0
    if root.exists():
        for path in root.rglob("*"):
            if path.is_dir():
                directories.append(path)
            elif path.is_file():
                files += 1
                try:
                    sizes += path.stat().st_size
                except OSError:
                    pass
    return {
        "exists": root.exists(),
        "directory": str(root),
        "directory_count": len(directories),
        "file_count": files,
        "total_size_bytes": sizes,
        "top_directories": [str(p.relative_to(root)) for p in directories[:20]],
    }


def inspect_sqlite(db_path: Path):
    if not db_path.exists():
        return {"exists": False, "message": f"DB fehlt: {db_path}"}

    conn = sqlite3.connect(f"file:{db_path}?mode=ro", uri=True)
    try:
        tables = conn.execute(
            "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY name"
        ).fetchall()
        rows = []
        for (table_name,) in tables:
            try:
                row_count = conn.execute(f"SELECT COUNT(*) FROM {quote_identifier(table_name)}").fetchone()[0]
            except sqlite3.DatabaseError:
                row_count = -1
            columns = conn.execute(f"PRAGMA table_info({quote_identifier(table_name)})").fetchall()
            column_names = [item[1] for item in columns]
            suspicious = []
            for col in column_names:
                if col.lower() in CANDIDATE_COLUMNS:
                    try:
                        zeros = conn.execute(
                            f"SELECT COUNT(*) FROM {quote_identifier(table_name)} WHERE {quote_identifier(col)} = 0"
                        ).fetchone()[0]
                    except sqlite3.DatabaseError:
                        zeros = -1
                    suspicious.append({"column": col, "zero_count": zeros})
            rows.append({
                "table": table_name,
                "column_count": len(column_names),
                "columns": column_names,
                "row_count": row_count,
                "zero_checks": suspicious,
            })
        return {"exists": True, "tables": rows}
    finally:
        conn.close()


def write_snapshot(snapshot_path: Path, payload: dict) -> Path:
    snapshot_path.parent.mkdir(parents=True, exist_ok=True)
    snapshot_path.write_text(json.dumps(payload, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    return snapshot_path


def parse_args():
    parser = argparse.ArgumentParser(description="Schreibgeschützter DB- und Persistenz-Inspector.")
    parser.add_argument("--db", type=Path, default=DB_PATH, help="Pfad zur SQLite-Datei")
    parser.add_argument("--scriptfiles", type=Path, default=SCRIPTFILES_PATH, help="Pfad zu Compiler/samp03/scriptfiles")
    parser.add_argument("--snapshot", type=Path, default=None, help="Optionaler JSON-Snapshot-Pfad zum Speichern")
    return parser.parse_args()


def main():
    args = parse_args()
    db_report = inspect_sqlite(args.db)
    script_report = summarize_scriptfiles(args.scriptfiles)
    result = {
        "db": db_report,
        "scriptfiles": script_report,
    }

    if args.snapshot:
        snapshot_path = write_snapshot(args.snapshot, result)
        print(f"[SNAPSHOT] {snapshot_path}")

    if db_report.get("exists"):
        print("[DB] Tabellen und Zeilen: ")
        for table in db_report["tables"]:
            print(f"  - {table['table']}: rows={table['row_count']}, cols={table['column_count']}")
            for checker in table["zero_checks"]:
                if checker["zero_count"] and checker["zero_count"] > 0:
                    print(f"      WARN: {table['table']}.{checker['column']} => {checker['zero_count']} Null/0-Werte")
    else:
        print(f"[DB] WARN: {db_report.get('message')}")

    print("[SCRIPTFILES] " + json.dumps(script_report, ensure_ascii=False))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
