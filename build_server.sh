#!/usr/bin/env bash
set -Eeuo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"

ROOT_DIR="$(pwd)"
SERVER_DIR="$ROOT_DIR/Compiler/samp03"
TARGET_AMX="$SERVER_DIR/gamemodes/gamemode.amx"
LEGACY_ALIAS="$SERVER_DIR/gamemodes/gtasa_reallife.amx"
SOURCE_PWN="$ROOT_DIR/gamemodes/gamemode.pwn"
PAWNCC_BIN="$ROOT_DIR/Compiler/pawncc/pawncc"
PATCHER="$ROOT_DIR/tools/patch_legacy_amx_version.py"

mkdir -p "$SERVER_DIR/gamemodes" "$SERVER_DIR/data/db" "$ROOT_DIR/data/db" "$SERVER_DIR/plugins"

if [ ! -f "$PAWNCC_BIN" ]; then
  echo "[build_server] ERROR: local Pawn compiler not found at $PAWNCC_BIN" >&2
  exit 1
fi

if [ ! -f "$SOURCE_PWN" ]; then
  echo "[build_server] ERROR: source file not found at $SOURCE_PWN" >&2
  exit 1
fi

if [ ! -f "$PATCHER" ]; then
  echo "[build_server] ERROR: patcher not found at $PATCHER" >&2
  exit 1
fi

rm -f "$TARGET_AMX" "$LEGACY_ALIAS"

export SA_SERVER_ROOT="$SERVER_DIR"
export SQLITE_DB_PATH="$ROOT_DIR/data/db/gtasa_reallife.db"
export LD_LIBRARY_PATH="$ROOT_DIR/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

qemu-i386 "$PAWNCC_BIN" \
  -o"$TARGET_AMX" \
  "$SOURCE_PWN" \
  -i"$ROOT_DIR/includes" \
  -i"$SERVER_DIR/include" \
  -d3

python3 "$PATCHER" "$TARGET_AMX"
cp -f "$TARGET_AMX" "$LEGACY_ALIAS"

if [ ! -s "$TARGET_AMX" ] || [ ! -s "$LEGACY_ALIAS" ]; then
  echo "[build_server] ERROR: AMX build missing or empty after compile" >&2
  exit 1
fi

echo "[build_server] Ready: $TARGET_AMX"

if [ -f "$SERVER_DIR/server.cfg" ]; then
  echo "[build_server] Using server config: $SERVER_DIR/server.cfg"
fi

if command -v screen >/dev/null 2>&1; then
  echo "[build_server] Launching SA-MP server in a screen session named samp03svr"
  screen -dmS samp03svr bash -lc "cd '$SERVER_DIR' && export LD_LIBRARY_PATH='.:./plugins:/usr/lib32:/usr/lib/i386-linux-gnu${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}' && exec ./samp03svr"
else
  echo "[build_server] screen not installed; run the server manually with:"
  echo "  cd '$SERVER_DIR' && LD_LIBRARY_PATH='.:./plugins:/usr/lib32:/usr/lib/i386-linux-gnu${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}' ./samp03svr"
fi
