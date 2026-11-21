#!/usr/bin/env python3
"""Offline tool browser for the Nexus Core source tree."""

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
        if (resolved / "gamemodes" / "gamemode.pwn").is_file() and (resolved / "tools").is_dir():
            return resolved
    return Path.cwd().resolve()


REPO = resolve_repo_root()
TOOLS_DIR = REPO / "tools"
CHECKS = [
    ("1", "qemu_7780_readiness_check.py", ["python3", "tools/qemu_7780_readiness_check.py", "--root", "."]),
    ("2", "validate_server_cfg.py", ["python3", "tools/validate_server_cfg.py", "--root", "."]),
    ("3", "state_rehydration_guard.py", ["python3", "tools/state_rehydration_guard.py", "--root", "."]),
    ("4", "audit_modules_and_guards.py", ["python3", "tools/audit_modules_and_guards.py", "--root", "."]),
    ("5", "control_panel_manager.py status", ["python3", "tools/control_panel_manager.py", "status"]),
    ("6", "economy_and_state_ledger_check.py", ["python3", "tools/economy_and_state_ledger_check.py", "--root", "."]),
    ("0", "Zurueck", None),
]


def run_check(command):
    proc = subprocess.run(command, cwd=str(REPO), env={**os.environ, "REPO_ROOT": str(REPO)}, text=True, capture_output=True)
    lines = []
    if proc.stdout:
        lines.extend(proc.stdout.strip().splitlines())
    if proc.stderr:
        lines.extend(proc.stderr.strip().splitlines())
    return lines or [f"[OK] Befehl beendet mit Code {proc.returncode}."]


def draw(stdscr, idx, lines):
    stdscr.erase()
    h, w = stdscr.getmaxyx()
    safe_w = max(10, w - 2)
    stdscr.addnstr(0, 0, f"NEXUS TOOLS | Repo={REPO.name}", safe_w, curses.A_BOLD)
    stdscr.addnstr(1, 0, "-" * min(safe_w, 80), safe_w)
    row = 2
    for i, (key, label, _) in enumerate(CHECKS):
        if row >= h - 4:
            break
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
    lines = ["Offline-Checks aktiv; keine Live-Server-Anbindung erforderlich."]
    while True:
        draw(stdscr, idx, lines)
        ch = stdscr.getch()
        if ch in (ord("q"), ord("Q"), 27):
            return
        elif ch == curses.KEY_UP:
            idx = (idx - 1) % len(CHECKS)
        elif ch == curses.KEY_DOWN:
            idx = (idx + 1) % len(CHECKS)
        elif ch in (10, 13, curses.KEY_ENTER):
            _, label, command = CHECKS[idx]
            if command is None:
                return
            lines = [f"[RUN] {label}"] + run_check(command)
        elif 48 <= ch <= 57:
            k = chr(ch)
            for i, (mk, label, command) in enumerate(CHECKS):
                if mk == k:
                    idx = i
                    if command is None:
                        return
                    lines = [f"[RUN] {label}"] + run_check(command)
                    break


if __name__ == "__main__":
    curses.wrapper(main)
