#!/usr/bin/env python3
import curses
import os
import subprocess
from pathlib import Path

REPO = Path(__file__).resolve().parent
RUNTIME = REPO / "Compiler" / "samp03"
TOOLS = REPO / "tools"

SERVER_LOG = RUNTIME / "server_log.txt"
BG_LOG = REPO / "samp7780_bg.log"
WATCHER_OUT = Path("/tmp/nexus_watcher.log")
SESSION_NAME = "nexus_log_watcher"

MENU = [
    ("1", "Live-Watcher STARTEN (server_log.txt -> screen)", "watch_server"),
    ("2", "Live-Watcher STARTEN (samp7780_bg.log -> screen)", "watch_bg"),
    ("3", "Live-Watcher STOPPEN (screen beenden)", "watch_stop"),
    ("4", "Watcher-Output anzeigen (letzte 25 Zeilen)", "watch_show"),
    ("5", "analyze_logs.py -> Runtime (Compiler/samp03)", "analyze_runtime"),
    ("6", "analyze_logs.py -> Root (samp7780_bg.log)", "analyze_root"),
    ("7", "validate_server_cfg.py -> Runtime-Check", "validate_cfg"),
    ("8", "log_tail.py / server_log.txt (letzte 25 Zeilen)", "tail_server"),
    ("9", "Alle Skripte in tools/ auflisten", "list_tools"),
    ("0", "Beenden", "quit"),
]


def run_cmd(cmd, cwd=REPO):
    try:
        res = subprocess.run(
            cmd,
            cwd=str(cwd),
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            timeout=15,
        )
        out = res.stdout.strip() or f"(Exit-Code {res.returncode}, keine Ausgabe)"
    except Exception as exc:
        out = f"[FEHLER] {exc}"
    return out.splitlines()


def watcher_running():
    res = subprocess.run(
        ["screen", "-ls"],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )
    return SESSION_NAME in (res.stdout or "")


def start_watcher(target_file: Path):
    if not target_file.is_file():
        return [f"[FEHLER] Datei existiert nicht: {target_file}"]
    if watcher_running():
        return [f"[INFO] Watcher läuft bereits in screen '{SESSION_NAME}'."]
    WATCHER_OUT.write_text("", encoding="utf-8")
    cmd = (
        f"python3 -u '{TOOLS / 'log_watcher.py'}' '{target_file}' --follow 2>&1 | "
        f"while IFS= read -r line; do "
        f"echo \"$line\" >> '{WATCHER_OUT}'; "
        f"python3 '{HOME / 'nexus_autofix.py'}' --ingest \"$line\"; "
        f"done"
    )
    subprocess.run(
        ["screen", "-dmS", SESSION_NAME, "bash", "-lc", cmd],
        cwd=str(RUNTIME),
    )
    return [
        f"[OK] Watcher gestartet in screen '{SESSION_NAME}'",
        f"     Ziel: {target_file}",
        f"     Log : {WATCHER_OUT}",
    ]


def stop_watcher():
    if not watcher_running():
        return ["[INFO] Kein aktiver Watcher gefunden."]
    subprocess.run(["screen", "-S", SESSION_NAME, "-X", "quit"])
    return [f"[OK] screen-Session '{SESSION_NAME}' beendet."]


def handle_action(action):
    if action == "watch_server":
        return start_watcher(SERVER_LOG)
    if action == "watch_bg":
        return start_watcher(BG_LOG)
    if action == "watch_stop":
        return stop_watcher()
    if action == "watch_show":
        if not WATCHER_OUT.is_file():
            return ["[INFO] Noch kein Watcher-Output vorhanden."]
        lines = WATCHER_OUT.read_text(encoding="utf-8", errors="replace").splitlines()
        return lines[-25:] or ["(Watcher-Log ist noch leer)"]
    if action == "analyze_runtime":
        return run_cmd(["python3", str(TOOLS / "analyze_logs.py"), str(RUNTIME)], cwd=RUNTIME)
    if action == "analyze_root":
        return run_cmd(["python3", str(TOOLS / "analyze_logs.py"), str(REPO)], cwd=REPO)
    if action == "validate_cfg":
        return run_cmd(["python3", str(TOOLS / "validate_server_cfg.py"), "--root", str(REPO)], cwd=RUNTIME)
    if action == "tail_server":
        if not SERVER_LOG.is_file():
            return [f"[INFO] Nicht vorhanden: {SERVER_LOG}"]
        lines = SERVER_LOG.read_text(encoding="utf-8", errors="replace").splitlines()
        return lines[-25:] or ["(server_log.txt ist leer)"]
    if action == "list_tools":
        return run_cmd(["ls", "-la", str(TOOLS)], cwd=REPO)
    return []


def draw(stdscr, idx, output_lines):
    stdscr.clear()
    h, w = stdscr.getmaxyx()
    status = "ACTIVE" if watcher_running() else "IDLE"
    header = f"N.E.X.U.S. TOOLS TUI | Watcher: {status} | Root: ~/Nexus-Core-Gamemode"
    stdscr.addnstr(0, 0, header, w - 1, curses.A_BOLD)
    stdscr.addnstr(1, 0, "-" * min(w - 1, 78), w - 1)

    row = 2
    for i, (key, label, _) in enumerate(MENU):
        prefix = ">" if i == idx else " "
        attr = curses.A_REVERSE if i == idx else curses.A_NORMAL
        stdscr.addnstr(row, 0, f"{prefix}[{key}] {label}", w - 1, attr)
        row += 1

    stdscr.addnstr(row, 0, "-" * min(w - 1, 78), w - 1)
    row += 1
    stdscr.addnstr(row, 0, "Ausgabe (Pfeile/Ziffern + Enter | r = Refresh | q = Quit):", w - 1, curses.A_BOLD)
    row += 1

    avail = max(1, h - row - 1)
    for line in output_lines[-avail:]:
        stdscr.addnstr(row, 0, line, w - 1)
        row += 1

    stdscr.refresh()


def main(stdscr):
    curses.curs_set(0)
    idx = 0
    output = ["Bereit. Waehle eine Aktion per Ziffer oder Pfeiltasten."]

    while True:
        draw(stdscr, idx, output)
        ch = stdscr.getch()

        if ch in (ord("q"), ord("Q"), 27):
            break
        elif ch in (ord("r"), ord("R")):
            output = handle_action("watch_show")
        elif ch == curses.KEY_UP:
            idx = (idx - 1) % len(MENU)
        elif ch == curses.KEY_DOWN:
            idx = (idx + 1) % len(MENU)
        elif ch in (10, 13):
            _, _, act = MENU[idx]
            if act == "quit":
                break
            output = handle_action(act)
        elif 48 <= ch <= 57:
            key = chr(ch)
            for i, (m_key, _, act) in enumerate(MENU):
                if m_key == key:
                    idx = i
                    if act == "quit":
                        return
                    output = handle_action(act)
                    break


if __name__ == "__main__":
    curses.wrapper(main)
