#!/usr/bin/env python3
import sqlite3
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DB_DIR = ROOT / "data" / "db"
SCHEMA_PATH = ROOT / "data" / "sql" / "schema.sql"
DB_PATH = DB_DIR / "gtasa_reallife.db"

DB_DIR.mkdir(parents=True, exist_ok=True)

conn = sqlite3.connect(DB_PATH)
conn.execute("PRAGMA foreign_keys = ON;")

with SCHEMA_PATH.open("r", encoding="utf-8") as handle:
    schema_sql = handle.read()

conn.executescript(schema_sql)
conn.commit()

print(f"[SQLite] Datenbank initialisiert: {DB_PATH}")
print("[SQLite] Schema erfolgreich angewendet.")
conn.close()
