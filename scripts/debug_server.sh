#!/usr/bin/env bash
set -Eeuo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SERVER_DIR="$ROOT_DIR/Compiler/samp03"
AMX_PATH="$SERVER_DIR/gamemodes/gtasa_reallife.amx"
LOG_PATH="$SERVER_DIR/server_log.txt"

echo "[debug] Project root: $ROOT_DIR"
if pgrep -af "samp03svr" >/dev/null 2>&1; then
  echo "[debug] SA:MP process is running"
  pgrep -af "samp03svr" || true
else
  echo "[debug] SA:MP process is NOT running"
fi

echo "[debug] AMX existence check"
if [ -f "$AMX_PATH" ]; then
  ls -l "$AMX_PATH"
else
  echo "[debug] MISSING: $AMX_PATH"
fi

echo "[debug] Server config"
if [ -f "$SERVER_DIR/server.cfg" ]; then
  grep -n "gamemode0\|plugins\|port" "$SERVER_DIR/server.cfg" || true
fi

echo "[debug] Database path"
if [ -f "$ROOT_DIR/data/db/gtasa_reallife.db" ]; then
  ls -l "$ROOT_DIR/data/db/gtasa_reallife.db"
else
  echo "[debug] MISSING: $ROOT_DIR/data/db/gtasa_reallife.db"
fi

if [ -f "$LOG_PATH" ]; then
  echo "[debug] Tailing log: $LOG_PATH"
  tail -n 50 "$LOG_PATH"
else
  echo "[debug] No server log at $LOG_PATH yet"
fi
