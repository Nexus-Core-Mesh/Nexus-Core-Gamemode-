#!/usr/bin/env bash
set -Eeuo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SERVER_DIR="$ROOT_DIR/Compiler/samp03"
CFG_PATH="$SERVER_DIR/server.cfg"
DB_PATH="$ROOT_DIR/data/db/gtasa_reallife.db"
MODE_NAME="gtasa_reallife"

mkdir -p "$ROOT_DIR/data/db" "$SERVER_DIR/gamemodes" "$SERVER_DIR/data/db"

cat > "$CFG_PATH" <<EOF
# SA:MP Server Configuration
# Base game mode
gamemode0 ${MODE_NAME} 1

# Plugins order
plugins crashdetect streamer sscanf2 zcmd

# Server details
hostname GTA:SA Reallife Modular
maxplayers 100
port 7777
query 1
lanmode 0
rcon_password changeme
announce 0
weburl http://localhost
maxnpc 0

# Logging
logtimeformat [%H:%M:%S]
EOF

if [ ! -f "$DB_PATH" ]; then
  python3 "$ROOT_DIR/scripts/bootstrap_sqlite.py"
fi

if [ ! -f "$SERVER_DIR/gamemodes/gtasa_reallife.amx" ]; then
  echo "[deploy] ERROR: Missing runtime AMX: $SERVER_DIR/gamemodes/gtasa_reallife.amx" >&2
  exit 1
fi

export SA_SERVER_ROOT="$SERVER_DIR"
export SQLITE_DB_PATH="$DB_PATH"
export LD_LIBRARY_PATH="$SERVER_DIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}:/usr/lib32:/usr/lib/i386-linux-gnu"

echo "[deploy] Starting SA:MP server from $SERVER_DIR"
cd "$SERVER_DIR"
exec env LD_LIBRARY_PATH="$LD_LIBRARY_PATH" ./samp03svr
