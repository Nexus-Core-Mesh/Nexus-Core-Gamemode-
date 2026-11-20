#!/usr/bin/env python3
"""Static readiness check for the 7780 target configuration.

This utility does not start the server; it only validates that the repository is
aligned with the single-target 7780 runtime layout.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


def read_text(path: Path):
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except FileNotFoundError:
        return ""


def check_server_cfg(root: Path):
    issues = []
    cfg = root / "server.cfg"
    content = read_text(cfg)
    if not content:
        issues.append("server.cfg fehlt oder ist leer")
    else:
        if re.search(r"(?m)^\s*gamemode0\s+gtasa_reallife\s+1\b", content):
            pass
        else:
            issues.append("server.cfg enthält nicht 'gamemode0 gtasa_reallife 1'")
        if re.search(r"(?m)^\s*port\s+7780\b", content):
            pass
        else:
            issues.append("server.cfg setzt Port nicht auf 7780")
        if "7777" in content and "7780" not in content:
            issues.append("server.cfg verweist auf 7777 statt 7780")

    runtime_cfg = root / "Compiler" / "samp03" / "server.cfg"
    runtime_content = read_text(runtime_cfg)
    if not runtime_content:
        issues.append("Compiler/samp03/server.cfg fehlt oder ist leer")
    else:
        if not re.search(r"(?m)^\s*gamemode0\s+gtasa_reallife\s+1\b", runtime_content):
            issues.append("Compiler/samp03/server.cfg enthält nicht 'gamemode0 gtasa_reallife 1'")
        if not re.search(r"(?m)^\s*port\s+7780\b", runtime_content):
            issues.append("Compiler/samp03/server.cfg setzt Port nicht auf 7780")

    amx_path = root / "Compiler" / "samp03" / "gamemodes" / "gtasa_reallife.amx"
    if not amx_path.exists():
        issues.append(f"AMX-Ziel fehlt: {amx_path}")
    else:
        print(f"[OK] AMX-Ziel vorhanden: {amx_path}")

    if (root / "Compiler" / "samp03" / "gamemodes" / "gamemode.amx").exists():
        issues.append("Stale runtime alias 'gamemode.amx' im server path gefunden")

    return issues


def main():
    parser = argparse.ArgumentParser(description="Statische 7780-Readiness-Prüfung ohne Serverstart.")
    parser.add_argument("--root", default=".", help="Projektwurzel")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    issues = check_server_cfg(root)

    if not issues:
        print("[OK] 7780-Readiness laut statischer Prüfung erfüllt.")
        return 0

    print("[WARN] 7780-Readiness-Probleme gefunden:")
    for issue in issues:
        print(f"- {issue}")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
