#!/usr/bin/env bash
set -Eeuo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SERVER_DIR="$ROOT_DIR/Compiler/samp03"
TARGET_AMX="$SERVER_DIR/gamemodes/gtasa_reallife.amx"
PAWNCC_BIN="$ROOT_DIR/Compiler/pawncc/pawncc"
SOURCE_PWN="$ROOT_DIR/gamemodes/gamemode.pwn"

export SA_SERVER_ROOT="$SERVER_DIR"
export SQLITE_DB_PATH="$ROOT_DIR/data/db/gtasa_reallife.db"
export LD_LIBRARY_PATH="$ROOT_DIR/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

if [ ! -f "$PAWNCC_BIN" ]; then
  echo "[build] ERROR: Pawn compiler not found at $PAWNCC_BIN" >&2
  exit 1
fi

if [ ! -f "$SOURCE_PWN" ]; then
  echo "[build] ERROR: Source file not found at $SOURCE_PWN" >&2
  exit 1
fi

mkdir -p "$SERVER_DIR/gamemodes" "$SERVER_DIR/data/db" "$ROOT_DIR/data/db"
rm -f "$ROOT_DIR/gamemode.amx" "$SERVER_DIR/gamemodes/gamemode.amx" "$TARGET_AMX"

qemu-i386 "$PAWNCC_BIN" \
  -o"$TARGET_AMX" \
  "$SOURCE_PWN" \
  -iincludes \
  -i"$ROOT_DIR/Compiler/samp03/include" \
  -d3

find "$SERVER_DIR/gamemodes" -maxdepth 1 -type f -name '*.amx' ! -name 'gtasa_reallife.amx' -delete

if [ -f "$TARGET_AMX" ] && [ -s "$TARGET_AMX" ]; then
  echo "[build] gtasa_reallife.amx ready at $TARGET_AMX"
  ls -l "$TARGET_AMX"
else
  echo "[build] ERROR: gtasa_reallife.amx missing or empty after compile" >&2
  exit 1
fi
