#!/usr/bin/env python3
from collections import deque
import curses
import json
import os
import re
import subprocess
import time
from pathlib import Path

REPO = Path(__file__).resolve().parent
BASE_DIR = str(REPO)
SAMP_DIR = str(REPO / "Compiler" / "samp03")
PAWNCC_DIR = str(REPO / "Compiler" / "pawncc")

START_SCRIPT = str(REPO / "Compiler" / "samp03" / "start_7780.sh")
STOP_CMD = "pkill -f 'samp03svr|qemu-i386' || true"
BUILD_SCRIPT = str(REPO / "scripts" / "build_gamemode.sh")
BG_LOG = str(REPO / "samp7780_bg.log")
SERVER_LOG = str(REPO / "Compiler" / "samp03" / "server_log.txt")
PORT = "7780"
ALERT_FILE = "/tmp/nexus_live_alert.json"
AUTOFIX_SCRIPT = str(REPO / "nexus_autofix.py")

CMD_GUIDE_TEXT = (
    "Seite [5/C] Befehle (Enter leer / ESC = Zurueck):\n"
    "  cat <datei> [a:b] : Datei/Zeilen anzeigen (z.B. cat gamemode.pwn 1:25)\n"
    "  nl  <datei> [a:b] : Zeilen mit Nummerierung (ideal fuer Backtrace/Compiler)\n"
    "  var <name>        : Sucht Variable/Enum/DIALOG_* in gamemodes & includes\n"
    "  dep <name>        : Sucht #include/forward/public/Aufrufe in Modulen\n"
    "  web <host/url>    : Prueft HTTP-Header per curl -I"
)

MENU_ITEMS = [
    ("1", "s", "Start/Restart 7780"),
    ("2", "b", "Build Gamemode"),
    ("3", "r", "Refresh Stats & Log"),
    ("4", "l", "Toggle Log (server/bg)"),
    ("5", "c", "Page: Cmd (cat/nl/var/dep/web)"),
    ("6", "m", "Page: Paths & Modules"),
    ("7", "x", "Stop 7780 Server"),
    ("8", "z", "Clear Output / Reset"),
    ("9", "f", "Fix Live Alert"),
    ("0", "q", "Quit TUI"),
]

PAIR_WHITE = 1
PAIR_NEON_BLUE = 2
PAIR_NEON_PINK = 3
PAIR_GREEN = 4
PAIR_RED = 5
PAIR_BLUE = 6

RAINBOW_PAIRS = [
    PAIR_RED,
    PAIR_NEON_PINK,
    PAIR_NEON_BLUE,
    PAIR_BLUE,
    PAIR_GREEN,
    PAIR_WHITE,
]


def init_colors():
    if not curses.has_colors():
        return
    curses.start_color()
    curses.use_default_colors()
    curses.init_pair(PAIR_WHITE, curses.COLOR_WHITE, curses.COLOR_BLACK)
    curses.init_pair(PAIR_NEON_BLUE, curses.COLOR_CYAN, curses.COLOR_BLACK)
    curses.init_pair(PAIR_NEON_PINK, curses.COLOR_MAGENTA, curses.COLOR_BLACK)
    curses.init_pair(PAIR_GREEN, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(PAIR_RED, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(PAIR_BLUE, curses.COLOR_BLUE, curses.COLOR_BLACK)


def style(pair_id: int, extra: int = 0) -> int:
    if curses.has_colors():
        return curses.color_pair(pair_id) | extra
    return extra


def run_cmd(cmd: str, timeout: float = 5.0) -> tuple[int, str]:
    try:
        proc = subprocess.run(
            ["bash", "-c", cmd],
            capture_output=True,
            text=True,
            errors="replace",
            timeout=timeout,
        )
        out = (proc.stdout or "") + (proc.stderr or "")
        return proc.returncode, out.strip()
    except subprocess.TimeoutExpired:
        return 124, f"Timeout ({timeout:.1f}s): {cmd}"


def existing_dirs(candidates: list[str]) -> list[str]:
    found = []
    for p in candidates:
        rp = os.path.realpath(p)
        if os.path.isdir(rp) and rp not in found:
            found.append(rp)
    return found


def list_direct_or_subfiles(directories: list[str], exts: tuple[str, ...]) -> list[str]:
    result = []
    for d in directories:
        if not os.path.isdir(d):
            continue
        for root, _, files in os.walk(d):
            rel_depth = os.path.relpath(root, d).count(os.sep)
            if root != d and rel_depth > 1:
                continue
            for fn in sorted(files):
                if fn.endswith(exts):
                    result.append(os.path.join(root, fn))
    return sorted(set(result))


def get_project_dirs() -> dict[str, list[str]]:
    return {
        "pawno_inc": existing_dirs([
            os.path.join(SAMP_DIR, "pawno", "include"),
            os.path.join(SAMP_DIR, "include"),
            os.path.join(BASE_DIR, "includes"),
            os.path.join(BASE_DIR, "include"),
            os.path.join(PAWNCC_DIR, "include"),
        ]),
        "gamemodes": existing_dirs([
            os.path.join(BASE_DIR, "gamemodes"),
            os.path.join(SAMP_DIR, "gamemodes"),
        ]),
        "scriptfiles": existing_dirs([
            os.path.join(SAMP_DIR, "scriptfiles"),
            os.path.join(BASE_DIR, "scriptfiles"),
        ]),
        "filterscripts": existing_dirs([
            os.path.join(SAMP_DIR, "filterscripts"),
            os.path.join(BASE_DIR, "filterscripts"),
        ]),
    }


def resolve_target_file(token: str) -> str | None:
    token = token.strip()
    if not token:
        return None
    dirs = get_project_dirs()
    search_roots = (
        [BASE_DIR, SAMP_DIR, PAWNCC_DIR]
        + dirs["gamemodes"]
        + dirs["pawno_inc"]
        + dirs["filterscripts"]
        + dirs["scriptfiles"]
    )
    for root in search_roots:
        cand = os.path.join(root, token)
        if os.path.isfile(cand):
            return os.path.realpath(cand)
    if os.path.isfile(token):
        return os.path.realpath(token)

    for root in search_roots:
        if not os.path.isdir(root):
            continue
        for cur, _, files in os.walk(root):
            if token in files:
                return os.path.realpath(os.path.join(cur, token))
    return None


def nl_slice(filepath: str, start_line: int = 1, end_line: int | None = None) -> str:
    try:
        with open(filepath, "r", encoding="utf-8", errors="replace") as f:
            lines = f.readlines()
    except OSError as exc:
        return f"Lesefehler ({filepath}): {exc}"

    total = len(lines)
    if total == 0:
        return f"{filepath}: (Datei ist leer)"
    s_idx = max(1, start_line)
    e_idx = total if end_line is None else min(total, max(s_idx, end_line))
    width = max(1, len(str(e_idx)))
    out = [f"Datei: {filepath} (Zeilen {s_idx}-{e_idx} von {total})"]
    for ln in range(s_idx, e_idx + 1):
        clean = lines[ln - 1].rstrip("\r\n")
        out.append(f"{ln:>{width}}: {clean}")
    return "\n".join(out)


def inspect_paths_and_modules() -> str:
    dirs = get_project_dirs()
    pwn_files = list_direct_or_subfiles(dirs["gamemodes"], (".pwn",))
    inc_files = list_direct_or_subfiles(dirs["pawno_inc"], (".inc",))
    fs_files = list_direct_or_subfiles(dirs["filterscripts"], (".pwn", ".amx"))

    sf_entries = []
    for sd in dirs["scriptfiles"]:
        for fn in sorted(os.listdir(sd)):
            if not fn.startswith(".git"):
                sf_entries.append(os.path.relpath(os.path.join(sd, fn), BASE_DIR))

    gm_includes = []
    for path in pwn_files:
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                content = f.read()
        except OSError:
            continue
        for m in re.finditer(r"^\s*#include\s+[<\"]([^>\"]+)[>\"]", content, re.M):
            gm_includes.append(m.group(1))

    uniq_gm_inc = list(dict.fromkeys(gm_includes))

    lines = [
        "Seite [6/M] Pfade & Module (ESC = Zurueck):",
        f"  Pawno/Inc : {', '.join(dirs['pawno_inc']) or 'nicht gefunden'} ({len(inc_files)} .inc)",
        f"  Gamemodes : {', '.join(os.path.relpath(p, BASE_DIR) for p in pwn_files) or 'keine'}",
        f"  GM-Includes: {', '.join(uniq_gm_inc[:10])}{' ...' if len(uniq_gm_inc) > 10 else ''}",
        f"  Filterscr : {', '.join(os.path.relpath(p, BASE_DIR) for p in fs_files) or 'keine'}",
        f"  Scriptfile: {', '.join(sf_entries[:8]) or 'leer'} ({', '.join(dirs['scriptfiles']) or '-'})",
    ]
    return "\n".join(lines)


def handle_custom_command(raw_cmd: str) -> tuple[bool, str, str]:
    cmd = raw_cmd.strip()
    if not cmd:
        return True, "Zurueck", CMD_GUIDE_TEXT

    parts = cmd.split()
    head = parts[0].lower()

    if head in ("help", "h", "?"):
        return True, "Hilfe (cat / nl / var / dep / web)", CMD_GUIDE_TEXT

    if head in ("nl", "cat") and len(parts) >= 2:
        target = resolve_target_file(parts[1])
        if not target:
            return False, f"{head} {parts[1]}", "Datei nicht gefunden in gamemodes, pawno/include, includes, filterscripts oder scriptfiles."
        s_line = 1
        e_line = 12
        if len(parts) >= 3:
            rng = parts[2]
            sep = ":" if ":" in rng else ("-" if "-" in rng else None)
            if sep:
                a, b = rng.split(sep, 1)
                s_line = int(a) if a.isdigit() else 1
                e_line = int(b) if b.isdigit() else s_line + 11
            elif rng.isdigit():
                s_line = int(rng)
                e_line = s_line + 11
        shown = f"{head} {os.path.relpath(target, BASE_DIR)} ({s_line}:{e_line})"
        return True, shown, nl_slice(target, s_line, e_line)

    dirs = get_project_dirs()
    scan_dirs = list(dict.fromkeys(dirs["gamemodes"] + dirs["pawno_inc"] + dirs["filterscripts"]))
    scan_q = " ".join(f"'{d}'" for d in scan_dirs) if scan_dirs else f"'{BASE_DIR}'"

    if head == "var" and len(parts) >= 2:
        sym = parts[1]
        shell_q = (
            f"grep -rnE '\\b{re.escape(sym)}\\b|new\\s+{re.escape(sym)}|enum\\s+{re.escape(sym)}' "
            f"{scan_q} 2>/dev/null | head -n 8"
        )
        rc, out = run_cmd(shell_q, timeout=6.0)
        desc = f"grep (Variable/Enum/Symbol) '{sym}':"
        return bool(out), f"var {sym}", f"{desc}\n{out}" if out else f"{desc}\nKeine Treffer."

    if head == "dep" and len(parts) >= 2:
        sym = parts[1]
        shell_q = (
            f"grep -rnE '#include.*{re.escape(sym)}|forward\\s+{re.escape(sym)}|public\\s+{re.escape(sym)}|\\b{re.escape(sym)}\\(' "
            f"{scan_q} 2>/dev/null | head -n 8"
        )
        rc, out = run_cmd(shell_q, timeout=6.0)
        desc = f"grep (Abhaengigkeit #include/forward/public/Aufruf) '{sym}':"
        return bool(out), f"dep {sym}", f"{desc}\n{out}" if out else f"{desc}\nKeine Treffer."

    if head == "web" and len(parts) >= 2:
        target_url = parts[1]
        if not target_url.startswith(("http://", "https://")):
            target_url = "http://" + target_url
        shell_q = f"curl -I -s --max-time 4 '{target_url}' | head -n 5"
        rc, out = run_cmd(shell_q, timeout=6.0)
        desc = f"curl -I auf {target_url}:"
        return (rc == 0 and bool(out)), f"web {target_url}", f"{desc}\n{out}" if out else f"{desc}\nKeine Antwort."

    rc, out = run_cmd(cmd, timeout=10.0)
    return rc == 0, cmd, out or f"(exit={rc}) OK"


def prompt_for_command(
    stdscr,
    stats: dict,
    active_log: str,
    h_scroll: int,
) -> str | None:
    curses.curs_set(1)
    stdscr.nodelay(True)
    buf = []
    last_phase = -1
    last_draw = 0.0

    try:
        while True:
            now = time.monotonic()
            led_phase = int(now) % len(RAINBOW_PAIRS)
            if led_phase != last_phase or now - last_draw >= 0.35 or last_draw == 0.0:
                prompt_y = draw_ui(
                    stdscr,
                    stats,
                    4,
                    "cmd",
                    active_log,
                    "Befehl eingeben (cat / nl / var / dep / web)",
                    CMD_GUIDE_TEXT,
                    True,
                    h_scroll,
                    led_phase,
                )
                _, w = stdscr.getmaxyx()
                safe_w = max(12, w - 3)
                label = "Eingabe (leer/ESC=Zurueck): "
                input_x = min(len(label), safe_w - 2)
                max_input = max(1, safe_w - input_x - 1)
                stdscr.move(prompt_y, 0)
                stdscr.clrtoeol()
                stdscr.addnstr(prompt_y, 0, label, safe_w, style(PAIR_NEON_PINK, curses.A_BOLD))
                typed = "".join(buf)[-max_input:]
                stdscr.addnstr(prompt_y, input_x, typed, max_input, style(PAIR_WHITE))
                stdscr.move(prompt_y, min(safe_w, input_x + len(typed)))
                stdscr.refresh()
                last_phase = led_phase
                last_draw = now

            ch = stdscr.getch()
            if ch == -1:
                time.sleep(0.05)
                continue
            if ch == curses.KEY_RESIZE:
                curses.update_lines_cols()
                last_draw = 0.0
                continue
            if ch == 27:
                return None
            if ch in (10, 13, curses.KEY_ENTER):
                line = "".join(buf).strip()
                return line if line else None
            if ch in (curses.KEY_BACKSPACE, 127, 8):
                if buf:
                    buf.pop()
                last_draw = 0.0
            elif 32 <= ch <= 126 or ch >= 160:
                buf.append(chr(ch))
                last_draw = 0.0
    finally:
        curses.curs_set(0)
        stdscr.nodelay(True)


def collect_stats() -> dict:
    _, ps_out = run_cmd("ps -ef | grep -E 'qemu-i386|samp03svr' | grep -v grep || true", timeout=2.5)
    _, ss_out = run_cmd(f"ss -ulnp | grep ':{PORT}' || true", timeout=2.5)
    _, mem_out = run_cmd(
        "free -h | awk '/^Mem:/ {print \"RAM \" $3 \"/\" $2 \" (\" $7 \" free)\"}'",
        timeout=2.0,
    )

    port_up = bool(ss_out.strip())
    pids = []
    for row in ps_out.splitlines()[:3]:
        parts = row.split()
        if len(parts) >= 2:
            pids.append(parts[1])
    pid_str = f"PIDs: {', '.join(pids)}" if pids else "PIDs: none"
    return {
        "ram": mem_out or "RAM ?",
        "port_up": port_up,
        "pid_str": pid_str,
    }


def read_numbered_log(path: str, max_lines: int = 5) -> list[str]:
    if not os.path.exists(path):
        return [f"1: fehlt: {path}"]
    try:
        with open(path, "r", encoding="utf-8", errors="replace") as handle:
            recent = deque((line.rstrip("\r\n") for line in handle), maxlen=max_lines)
    except OSError as exc:
        return [f"1: Lesefehler: {exc}"]

    if not recent:
        return ["1: (leer)"]

    width = len(str(len(recent)))
    return [f"{i:>{width}}: {line}" for i, line in enumerate(recent, start=1)]


def put_line(stdscr, y: int, x: int, text: str, safe_w: int, h_offset: int, attr: int):
    if safe_w <= 0 or y < 0 or h_offset >= len(text):
        return
    rest = text[h_offset:]
    if len(rest) > safe_w:
        fitted = (rest[: max(0, safe_w - 1)] + "~") if safe_w >= 2 else rest[:safe_w]
    else:
        fitted = rest
    stdscr.addnstr(y, x, fitted, safe_w, attr)


def read_live_alert() -> dict | None:
    if not os.path.isfile(ALERT_FILE):
        return None
    try:
        with open(ALERT_FILE, "r", encoding="utf-8", errors="replace") as f:
            data = json.load(f)
        return data if isinstance(data, dict) and data.get("fix_id") else None
    except Exception:
        return None


def draw_led_rule(stdscr, y: int, safe_w: int, phase: int, color_offset: int = 0):
    if safe_w <= 0 or y < 0:
        return
    pair_id = RAINBOW_PAIRS[(phase + color_offset) % len(RAINBOW_PAIRS)]
    attr = style(pair_id, curses.A_BOLD)
    line = "─" * safe_w
    try:
        stdscr.addnstr(y, 0, line, safe_w, attr)
    except curses.error:
        stdscr.addnstr(y, 0, "-" * safe_w, safe_w, attr)


def draw_ui(
    stdscr,
    stats: dict,
    selected_idx: int,
    view_mode: str,
    active_log: str,
    last_cmd: str,
    last_output: str,
    last_ok: bool,
    h_scroll: int,
    led_phase: int,
) -> int:
    stdscr.bkgd(" ", style(PAIR_WHITE))
    stdscr.erase()
    h, w = stdscr.getmaxyx()
    safe_w = max(1, w - 2)

    if h < 9 or w < 15:
        put_line(stdscr, 0, 0, "Fenster zu klein", safe_w, 0, style(PAIR_RED, curses.A_BOLD))
        stdscr.refresh()
        return 2

    if h <= 18:
        log_lines_count = 2
    elif h <= 26:
        log_lines_count = 3
    else:
        log_lines_count = 5

    log_block_h = 2 + log_lines_count
    log_rule_y = max(4, h - log_block_h)
    log_header_y = log_rule_y + 1
    center_end_y = log_rule_y

    mode_tag = {"main": "MAIN", "cmd": "CMD", "modules": "MODS"}.get(view_mode, "MAIN")
    title = f"S.A.M.P.T.U.I. :{PORT} [{mode_tag}] H:{h_scroll}"
    put_line(stdscr, 0, 0, title, safe_w, 0, style(PAIR_NEON_BLUE, curses.A_BOLD))

    y = 1
    port_label = f":{PORT} UP" if stats["port_up"] else f":{PORT} DOWN"
    port_attr = style(PAIR_GREEN, curses.A_BOLD) if stats["port_up"] else style(PAIR_RED, curses.A_BOLD)
    prefix_stat = f"{stats['ram']} | "
    put_line(stdscr, y, 0, prefix_stat, safe_w, h_scroll, style(PAIR_WHITE))
    vis_a = max(0, len(prefix_stat) - h_scroll)
    if vis_a < safe_w:
        off_b = max(0, h_scroll - len(prefix_stat))
        put_line(stdscr, y, vis_a, port_label, safe_w - vis_a, off_b, port_attr)
    y += 1

    if y < center_end_y:
        put_line(stdscr, y, 0, stats["pid_str"], safe_w, h_scroll, style(PAIR_WHITE))
        y += 1

    alert_data = read_live_alert()
    if alert_data and y < center_end_y:
        alert_txt = f"ALERT [{alert_data.get('fix_id')}]: {alert_data.get('summary', '')} [9/F = Fix]"
        put_line(stdscr, y, 0, alert_txt, safe_w, h_scroll, style(PAIR_RED, curses.A_BOLD))
        y += 1

    if y < center_end_y:
        draw_led_rule(stdscr, y, safe_w, led_phase, color_offset=0)
        y += 1

    if view_mode == "main":
        for idx, (num_key, letter_key, label) in enumerate(MENU_ITEMS):
            if y >= center_end_y:
                break
            selected = idx == selected_idx
            marker = ">" if selected else " "
            entry = f"{marker}[{num_key}/{letter_key.upper()}] {label}"
            attr = style(PAIR_NEON_PINK, curses.A_BOLD) if selected else style(PAIR_WHITE)
            put_line(stdscr, y, 0, entry, safe_w, h_scroll, attr)
            y += 1

        if y < center_end_y and last_cmd:
            put_line(stdscr, y, 0, f"Cmd: {last_cmd}", safe_w, h_scroll, style(PAIR_NEON_BLUE, curses.A_BOLD))
            y += 1

        out_lines = last_output.splitlines() if last_output else [
            "Bereit | [5/C] Cmd | [6/M] Pfade & Module | [8/Z] Reset"
        ]
        out_attr = style(PAIR_GREEN if last_ok else PAIR_RED) if last_output else style(PAIR_WHITE)
        for out_line in out_lines:
            if y >= center_end_y:
                break
            put_line(stdscr, y, 0, out_line, safe_w, h_scroll, out_attr)
            y += 1
        prompt_y = min(center_end_y - 1, max(y, 3))
    else:
        nav_hint = "[ESC] Zurueck | [8/Z] Reset"
        put_line(stdscr, y, 0, nav_hint, safe_w, h_scroll, style(PAIR_NEON_PINK, curses.A_BOLD))
        y += 1

        prompt_y = y
        if view_mode == "cmd":
            y += 1

        if last_cmd and y < center_end_y:
            put_line(stdscr, y, 0, f"Cmd: {last_cmd}", safe_w, h_scroll, style(PAIR_NEON_BLUE, curses.A_BOLD))
            y += 1

        out_lines = last_output.splitlines() if last_output else []
        out_attr = style(PAIR_GREEN if last_ok else PAIR_RED) if last_output else style(PAIR_WHITE)
        for out_line in out_lines:
            if y >= center_end_y:
                break
            put_line(stdscr, y, 0, out_line, safe_w, h_scroll, out_attr)
            y += 1

    draw_led_rule(stdscr, log_rule_y, safe_w, led_phase, color_offset=2)

    log_path = SERVER_LOG if active_log == "server" else BG_LOG
    put_line(
        stdscr,
        log_header_y,
        0,
        f"Log[{active_log}] {os.path.basename(log_path)}:",
        safe_w,
        h_scroll,
        style(PAIR_NEON_BLUE, curses.A_BOLD),
    )
    log_lines = read_numbered_log(log_path, max_lines=log_lines_count)
    for offset, line in enumerate(log_lines[:log_lines_count]):
        row_y = log_header_y + 1 + offset
        if row_y < h:
            lower = line.lower()
            is_bad = any(tok in lower for tok in ("error", "fail", "debug", "backtrace", "long callback", "timeout"))
            line_attr = style(PAIR_RED, curses.A_BOLD) if is_bad else style(PAIR_WHITE)
            put_line(stdscr, row_y, 0, line, safe_w, h_scroll, line_attr)

    stdscr.refresh()
    return prompt_y


def trigger_action(
    stdscr,
    action_key: str,
    view_mode: str,
    active_log: str,
    stats: dict,
    h_scroll: int,
):
    if action_key == "z":
        return "main", active_log, "", "", True
    if action_key == "f":
        if not os.path.exists(AUTOFIX_SCRIPT):
            return "main", active_log, "autofix", f"Skript fehlt: {AUTOFIX_SCRIPT}", False
        rc, out = run_cmd(f"python3 '{AUTOFIX_SCRIPT}'", timeout=60.0)
        return "main", active_log, "nexus_autofix.py", out or f"[{rc}] OK", rc == 0

    if action_key == "s":
        rc, out = run_cmd(START_SCRIPT, timeout=15.0)
        return "main", active_log, START_SCRIPT, f"[{rc}] {out}" if out else f"[{rc}] OK", rc == 0

    if action_key == "x":
        rc, out = run_cmd(STOP_CMD, timeout=8.0)
        return "main", active_log, STOP_CMD, f"[{rc}] {out}" if out else f"[{rc}] Server gestoppt", rc == 0

    if action_key == "b":
        if os.path.exists(BUILD_SCRIPT):
            rc, out = run_cmd(BUILD_SCRIPT, timeout=60.0)
            return "main", active_log, BUILD_SCRIPT, f"[{rc}] {out[-350:]}" if out else f"[{rc}] OK", rc == 0
        return "main", active_log, BUILD_SCRIPT, "Build-Skript nicht gefunden.", False

    if action_key == "r":
        return view_mode, active_log, "refresh", "Status & Logs aktualisiert.", True

    if action_key == "l":
        new_log = "bg" if active_log == "server" else "server"
        return view_mode, new_log, f"log -> {new_log}", os.path.basename(SERVER_LOG if new_log == "server" else BG_LOG), True

    if action_key == "m":
        return "modules", active_log, "Pfade & Module", inspect_paths_and_modules(), True

    if action_key == "c":
        user_input = prompt_for_command(stdscr, stats, active_log, h_scroll)
        if user_input is None:
            return "main", active_log, "", "", True
        ok, cmd_lbl, out_txt = handle_custom_command(user_input)
        return "cmd", active_log, cmd_lbl, out_txt, ok

    return view_mode, active_log, "", "", True


def main(stdscr):
    curses.curs_set(0)
    stdscr.nodelay(True)
    stdscr.keypad(True)
    init_colors()

    selected_idx = 0
    view_mode = "main"
    active_log = "server"
    last_cmd = ""
    last_output = ""
    last_ok = True
    h_scroll = 0

    stats = collect_stats()
    last_stats_tick = time.monotonic()
    last_draw_tick = 0.0
    last_phase = -1

    while True:
        now = time.monotonic()
        led_phase = int(now) % len(RAINBOW_PAIRS)

        if now - last_stats_tick >= 3.0:
            stats = collect_stats()
            last_stats_tick = now
            last_draw_tick = 0.0

        if led_phase != last_phase or now - last_draw_tick >= 0.35 or last_draw_tick == 0.0:
            draw_ui(
                stdscr,
                stats,
                selected_idx,
                view_mode,
                active_log,
                last_cmd,
                last_output,
                last_ok,
                h_scroll,
                led_phase,
            )
            last_draw_tick = now
            last_phase = led_phase

        key = stdscr.getch()
        if key == -1:
            time.sleep(0.05)
            continue

        if key == curses.KEY_RESIZE:
            curses.update_lines_cols()
            last_draw_tick = 0.0
        elif key == 27 and view_mode != "main":
            view_mode = "main"
            last_cmd = ""
            last_output = ""
            last_ok = True
            last_draw_tick = 0.0
        elif key == curses.KEY_UP:
            selected_idx = (selected_idx - 1) % len(MENU_ITEMS)
            last_draw_tick = 0.0
        elif key == curses.KEY_DOWN:
            selected_idx = (selected_idx + 1) % len(MENU_ITEMS)
            last_draw_tick = 0.0
        elif key == curses.KEY_LEFT:
            h_scroll = max(0, h_scroll - 8)
            last_draw_tick = 0.0
        elif key == curses.KEY_RIGHT:
            h_scroll += 8
            last_draw_tick = 0.0
        elif key in (10, 13, curses.KEY_ENTER):
            _, action_key, _ = MENU_ITEMS[selected_idx]
            if action_key == "q":
                break
            view_mode, active_log, last_cmd, last_output, last_ok = trigger_action(
                stdscr, action_key, view_mode, active_log, stats, h_scroll
            )
            if action_key in ("s", "x", "r", "f"):
                stats = collect_stats()
                last_stats_tick = time.monotonic()
            last_draw_tick = 0.0
        elif key in (ord("q"), ord("Q"), ord("0")):
            break
        else:
            mapped = None
            for idx, (num_k, letter_k, _) in enumerate(MENU_ITEMS):
                if key in (ord(num_k), ord(letter_k.lower()), ord(letter_k.upper())):
                    selected_idx = idx
                    mapped = letter_k
                    break
            if mapped:
                view_mode, active_log, last_cmd, last_output, last_ok = trigger_action(
                    stdscr, mapped, view_mode, active_log, stats, h_scroll
                )
                if mapped in ("s", "x", "r", "f"):
                    stats = collect_stats()
                    last_stats_tick = time.monotonic()
                last_draw_tick = 0.0


def run_tui():
    try:
        curses.wrapper(main)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    run_tui()
