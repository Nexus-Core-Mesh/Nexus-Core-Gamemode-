#!/usr/bin/env python3
import curses
import json
import os
import subprocess
from pathlib import Path

REPO = Path(__file__).resolve().parent
SAMP_TUI = REPO / "samp_tui.py"
TOOLS_TUI = REPO / "nexus_tools_tui.py"
AUTOFIX_PY = REPO / "nexus_autofix.py"
ALERT_FILE = Path("/tmp/nexus_live_alert.json")
WATCHER_SESSION = "nexus_log_watcher"

MENU = [
    ("1", "S.A.M.P.T.U.I. (:7780 Server & Build)", "launch_samp"),
    ("2", "N.E.X.U.S. TOOLS TUI (Watcher & Log-Analyse)", "launch_tools"),
    ("3", "Auto-Fix sofort ausfuehren (nexus_autofix.py)", "run_autofix"),
    ("4", "Status & Alert-Details pruefen", "check_status"),
    ("0", "Beenden", "quit"),
]


def is_watcher_active() -> bool:
    try:
        res = subprocess.run(
            ["screen", "-ls"],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            timeout=3,
        )
        return WATCHER_SESSION in (res.stdout or "")
    except Exception:
        return False


def is_port_7780_up() -> bool:
    try:
        res = subprocess.run(
            ["bash", "-c", "ss -ulnp 2>/dev/null | grep -q ':7780'"],
            timeout=3,
        )
        return res.returncode == 0
    except Exception:
        return False


def read_alert_summary() -> str:
    if not ALERT_FILE.is_file():
        return "kein Alert"
    try:
        data = json.loads(ALERT_FILE.read_text(encoding="utf-8"))
        return f"{data.get('fix_id', 'ALERT')}: {data.get('summary', '')}"
    except Exception:
        return "Alert vorhanden (Lesefehler)"


def launch_script(stdscr, script_path: Path, wait_after: bool = False) -> list[str]:
    if not script_path.is_file():
        return [f"[FEHLER] Skript nicht gefunden: {script_path}"]

    curses.def_prog_mode()
    curses.endwin()
    try:
        res = subprocess.run(["python3", str(script_path)])
        lines = [f"[OK] {script_path.name} beendet (Code {res.returncode})."]
        if wait_after:
            out_log = Path("/tmp/nexus_autofix.log")
            if out_log.is_file():
                tail = out_log.read_text(encoding="utf-8", errors="replace").splitlines()[-8:]
                lines.extend(tail)
        return lines
    finally:
        curses.reset_prog_mode()
        stdscr.clear()
        curses.curs_set(0)
        stdscr.refresh()


def status_lines() -> list[str]:
    return [
        f"7780-Server : {'UP' if is_port_7780_up() else 'DOWN'}",
        f"Watcher     : {'ACTIVE (' + WATCHER_SESSION + ')' if is_watcher_active() else 'IDLE'}",
        f"Live-Alert  : {read_alert_summary()}",
        f"Skripte     : samp_tui.py={SAMP_TUI.is_file()} | nexus_tools_tui.py={TOOLS_TUI.is_file()} | nexus_autofix.py={AUTOFIX_PY.is_file()}",
    ]


def draw(stdscr, idx: int, lines: list[str]):
    stdscr.erase()
    h, w = stdscr.getmaxyx()
    safe_w = max(10, w - 2)

    header = f"N.E.X.U.S. HUB | 7780:{'UP' if is_port_7780_up() else 'DOWN'} | Watch:{'ON' if is_watcher_active() else 'OFF'}"
    stdscr.addnstr(0, 0, header, safe_w, curses.A_BOLD)
    stdscr.addnstr(1, 0, "-" * min(safe_w, 60), safe_w)

    row = 2
    for i, (key, label, _) in enumerate(MENU):
        if row >= h - 4:
            break
        prefix = ">" if i == idx else " "
        attr = curses.A_REVERSE if i == idx else curses.A_NORMAL
        stdscr.addnstr(row, 0, f"{prefix}[{key}] {label}", safe_w, attr)
        row += 1

    if row < h - 2:
        stdscr.addnstr(row, 0, "-" * min(safe_w, 60), safe_w)
        row += 1

    for line in lines:
        if row >= h - 1:
            break
        stdscr.addnstr(row, 0, line, safe_w)
        row += 1

    stdscr.refresh()


def main(stdscr):
    curses.curs_set(0)
    idx = 0
    lines = status_lines()

    while True:
        draw(stdscr, idx, lines)
        ch = stdscr.getch()

        if ch in (ord("q"), ord("Q"), 27):
            break
        elif ch == curses.KEY_UP:
            idx = (idx - 1) % len(MENU)
        elif ch == curses.KEY_DOWN:
            idx = (idx + 1) % len(MENU)
        elif ch in (10, 13, curses.KEY_ENTER):
            _, _, act = MENU[idx]
            if act == "quit":
                break
            if act == "launch_samp":
                lines = launch_script(stdscr, SAMP_TUI, wait_after=False)
            elif act == "launch_tools":
                lines = launch_script(stdscr, TOOLS_TUI, wait_after=False)
            elif act == "run_autofix":
                lines = launch_script(stdscr, AUTOFIX_PY, wait_after=True)
            elif act == "check_status":
                lines = status_lines()
        elif 48 <= ch <= 57:
            k = chr(ch)
            for i, (mk, _, act) in enumerate(MENU):
                if mk == k:
                    idx = i
                    if act == "quit":
                        return
                    if act == "launch_samp":
                        lines = launch_script(stdscr, SAMP_TUI, wait_after=False)
                    elif act == "launch_tools":
                        lines = launch_script(stdscr, TOOLS_TUI, wait_after=False)
                    elif act == "run_autofix":
                        lines = launch_script(stdscr, AUTOFIX_PY, wait_after=True)
                    elif act == "check_status":
                        lines = status_lines()
                    break


if __name__ == "__main__":
    curses.wrapper(main)
