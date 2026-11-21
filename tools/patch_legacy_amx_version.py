#!/usr/bin/env python3
"""Patch a Pawn AMX file back to the legacy SA-MP 0.3.7-compatible header format."""

import sys
from pathlib import Path

TARGET_OFFSET = 0x06
LEGACY_BYTES = b"\x08\x08"


def patch_amx(path: str) -> None:
    file_path = Path(path)
    if not file_path.exists():
        raise FileNotFoundError(f"AMX file not found: {file_path}")

    data = bytearray(file_path.read_bytes())
    if len(data) < TARGET_OFFSET + len(LEGACY_BYTES):
        raise ValueError(
            f"AMX file too small to contain a 2-byte AMX version field at offset 0x{TARGET_OFFSET:X}: {file_path}"
        )

    data[TARGET_OFFSET : TARGET_OFFSET + len(LEGACY_BYTES)] = LEGACY_BYTES
    file_path.write_bytes(data)

    patched = bytes(data[TARGET_OFFSET : TARGET_OFFSET + len(LEGACY_BYTES)])
    print(f"Successfully patched: {file_path} to AMX version 8.")
    print(f"Header bytes at 0x{TARGET_OFFSET:X}: {patched.hex()}")


if __name__ == "__main__":
    target = sys.argv[1] if len(sys.argv) > 1 else "Compiler/samp03/gamemodes/gtasa_reallife.amx"
    patch_amx(target)
