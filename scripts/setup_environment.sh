#!/usr/bin/env bash
set -Eeuo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

if [ "$(id -u)" -ne 0 ] && command -v sudo >/dev/null 2>&1; then
  SUDO="sudo"
else
  SUDO=""
fi

if command -v apt-get >/dev/null 2>&1; then
  if [ -n "$SUDO" ]; then
    "$SUDO" dpkg --add-architecture i386
    "$SUDO" apt-get update
    "$SUDO" DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      ca-certificates \
      curl \
      wget \
      qemu-user \
      libc6-i386 \
      lib32gcc-s1 \
      lib32stdc++6 \
      zlib1g:i386 \
      libgcc-s1:i386
  else
    dpkg --add-architecture i386
    apt-get update
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      ca-certificates \
      curl \
      wget \
      qemu-user \
      libc6-i386 \
      lib32gcc-s1 \
      lib32stdc++6 \
      zlib1g:i386 \
      libgcc-s1:i386
  fi
else
  echo "[setup] No apt-get detected; skipping system package installation." >&2
fi

mkdir -p "$ROOT_DIR/data/db" \
  "$ROOT_DIR/Compiler/samp03/gamemodes" \
  "$ROOT_DIR/Compiler/samp03/data/db" \
  "$ROOT_DIR/Compiler/samp03/plugins" \
  "$ROOT_DIR/Compiler/samp03/filterscripts" \
  "$ROOT_DIR/Compiler/samp03/scriptfiles"

if [ -f "$ROOT_DIR/Compiler/pawncc/pawncc" ]; then
  echo "[setup] Local Pawn toolchain detected: $ROOT_DIR/Compiler/pawncc/pawncc"
else
  echo "[setup] WARNING: Local Pawn toolchain not found at $ROOT_DIR/Compiler/pawncc/pawncc" >&2
fi

echo "[setup] Runtime directories ready."
echo "[setup] Build target must be: $ROOT_DIR/Compiler/samp03/gamemodes/gtasa_reallife.amx"
