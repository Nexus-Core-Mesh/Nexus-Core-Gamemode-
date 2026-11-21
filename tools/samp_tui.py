#!/usr/bin/env python3
"""Offline S.A.M.P. readiness dashboard for the source tree."""

from __future__ import annotations

import curses
import os
import subprocess
import sys
from pathlib import Path


def resolve_repo_root() -> Path:
    here = Path(__file__).resolve()
    candidates = [
        here.parent.parent,
        here.parent,
        Path.cwd(),
        Path.home() / "Nexus-Core-Gamemode",
        Path.home() / "Nexus-Core-Gamemode-",
        Path.home() / "studious-fishstick",
    ]
    seen = set()
    for candidate in candidates:
        resolved = candidate.resolve()
        if resolved in seen:
            continue
        seen.add(resolved)
        if (resolved / "gamemodes" / "gamemode.pwn").is_file() and (resolved / "Compiler").is_dir():
            return resolved
    return Path.cwd().resolve()


REPO = resolve_repo_root()
TARGET = REPO / "Compiler" / "samp03" / "gamemodes" / "gtasa_reallife.amx"
SERVER_CFG = REPO / "server.cfg"
RUNTIME_CFG = REPO / "Compiler" / "samp03" / "server.cfg"
MENU = [
    ("1", "Pruefe 7780-Contract", "contract"),
    ("2", "Pruefe server.cfg", "cfg"),
    ("3", "Pruefe AMX Ziel", "amx"),
    ("4", "Zeige Build-Commands", "build"),
    ("0", "Zurueck", "quit"),
]


def run_local(command):
    proc = subprocess.run(command, cwd=str(REPO), env={**os.environ, "REPO_ROOT": str(REPO)}, text=True, capture_output=True)
    lines = []
    if proc.stdout:
        lines.extend(proc.stdout.strip().splitlines())
    if proc.stderr:
        lines.extend(proc.stderr.strip().splitlines())
    return lines or [f"[OK] Code {proc.returncode}"]


def build_lines():
    lines = [
        f"Repo Root: {REPO}",
        f"AMX Ziel : {'OK' if TARGET.exists() else 'MISSING'} {TARGET}",
        f"server.cfg: {'OK' if SERVER_CFG.exists() else 'MISSING'}",
        f"runtime cfg: {'OK' if RUNTIME_CFG.exists() else 'MISSING'}",
        "Mode: offline-only; no server start or port 7780 launch performed.",
    ]
    return lines


def draw(stdscr, idx, lines):
    stdscr.erase()
    h, w = stdscr.getmaxyx()
    safe_w = max(10, w - 2)
    stdscr.addnstr(0, 0, "S.A.M.P. TUI | Offline-Readiness", safe_w, curses.A_BOLD)
    stdscr.addnstr(1, 0, "-" * min(safe_w, 80), safe_w)
    row = 2
    for i, (key, label, _) in enumerate(MENU):
        prefix = ">" if i == idx else " "
        attr = curses.A_REVERSE if i == idx else curses.A_NORMAL
        stdscr.addnstr(row, 0, f"{prefix}[{key}] {label}", safe_w, attr)
        row += 1
    if row < h - 2:
        stdscr.addnstr(row, 0, "-" * min(safe_w, 80), safe_w)
        row += 1
    for line in lines[: max(0, h - row - 2)]:
        stdscr.addnstr(row, 0, line[:safe_w], safe_w)
        row += 1
    stdscr.refresh()


def main(stdscr):
    curses.curs_set(0)
    idx = 0
    lines = build_lines()
    while True:
        draw(stdscr, idx, lines)
        ch = stdscr.getch()
        if ch in (ord("q"), ord("Q"), 27):
            return
        elif ch == curses.KEY_UP:
            idx = (idx - 1) % len(MENU)
        elif ch == curses.KEY_DOWN:
            idx = (idx + 1) % len(MENU)
        elif ch in (10, 13, curses.KEY_ENTER):
            _, _, action = MENU[idx]
            if action == "quit":
                return
            elif action == "contract":
                lines = ["[RUN] qemu_7780_readiness_check.py"] + run_local(["python3", "tools/qemu_7780_readiness_check.py", "--root", "."])
            elif action == "cfg":
                lines = ["[RUN] validate_server_cfg.py"] + run_local(["python3", "tools/validate_server_cfg.py", "--root", "."])
            elif action == "amx":
                lines = ["[RUN] AMX target check"] + ([f"[OK] {TARGET}"] if TARGET.exists() else [f"[MISSING] {TARGET}"])
            elif action == "build":
                lines = [
                    "Build target: Compiler/samp03/gamemodes/gtasa_reallife.amx",
                    "Command: LD_LIBRARY_PATH=\"$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}\" qemu-i386 Compiler/pawncc/pawncc -o\"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx\" gamemodes/gamemode.pwn -iincludes -iCompiler/samp03/include -d3",
                ]
        elif 48 <= ch <= 57:
            k = chr(ch)
            for i, (mk, _, action) in enumerate(MENU):
                if mk == k:
                    idx = i
                    if action == "quit":
                        return
                    if action == "contract":
                        lines = ["[RUN] qemu_7780_readiness_check.py"] + run_local(["python3", "tools/qemu_7780_readiness_check.py", "--root", "."])
                    elif action == "cfg":
                        lines = ["[RUN] validate_server_cfg.py"] + run_local(["python3", "tools/validate_server_cfg.py", "--root", "."])
                    elif action == "amx":
                        lines = ["[RUN] AMX target check"] + ([f"[OK] {TARGET}"] if TARGET.exists() else [f"[MISSING] {TARGET}"])
                    elif action == "build":
                        lines = [
                            "Build target: Compiler/samp03/gamemodes/gtasa_reallife.amx",
                            "Command: LD_LIBRARY_PATH=\"$PWD/Compiler/pawncc${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}\" qemu-i386 Compiler/pawncc/pawncc -o\"$PWD/Compiler/samp03/gamemodes/gtasa_reallife.amx\" gamemodes/gamemode.pwn -iincludes -iCompiler/samp03/include -d3",
                        ]
                    break


if __name__ == "__main__":
    curses.wrapper(main)
