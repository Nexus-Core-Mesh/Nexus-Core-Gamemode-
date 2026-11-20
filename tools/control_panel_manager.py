#!/usr/bin/env python3
"""Offline lifecycle helper for the Node-based control panel.

The project keeps the Pawn runtime source-first and does not require a live
7780 server or runtime to be available while the source tree is being updated.
This helper simply prepares and starts the read-only dashboard used for
monitoring the project state, backed by SQLite while the gamemode itself stays
Dini-first under Compiler/samp03/scriptfiles/.
"""

from __future__ import annotations

import argparse
import os
import signal
import subprocess
import sys
import time
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parents[1]
CONTROL_PANEL_DIR = ROOT_DIR / "control-panel"
DB_PATH = ROOT_DIR / "data" / "db" / "gtasa_reallife.db"
SCRIPTFILES_DIR = ROOT_DIR / "Compiler" / "samp03" / "scriptfiles"
DEFAULT_HOST = "127.0.0.1"
DEFAULT_PORT = 3001
PID_FILE = ROOT_DIR / ".control_panel.pid"
LOG_DIR = ROOT_DIR / "logs"
LOG_FILE = LOG_DIR / "control-panel.log"


def run_command(command, *, cwd=None, env=None, stdout=None, stderr=None):
    return subprocess.run(
        command,
        cwd=str(cwd) if cwd is not None else None,
        env=env,
        stdout=stdout,
        stderr=stderr,
        check=False,
    )


def node_available():
    result = run_command(["node", "--version"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return result.returncode == 0


def npm_available():
    result = run_command(["npm", "--version"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return result.returncode == 0


def ensure_paths():
    CONTROL_PANEL_DIR.mkdir(parents=True, exist_ok=True)
    LOG_DIR.mkdir(parents=True, exist_ok=True)
    DB_PATH.parent.mkdir(parents=True, exist_ok=True)
    SCRIPTFILES_DIR.mkdir(parents=True, exist_ok=True)


def read_pid_file():
    if not PID_FILE.exists():
        return None
    try:
        return int(PID_FILE.read_text(encoding="utf-8").strip())
    except ValueError:
        return None


def is_process_running(pid):
    if pid is None:
        return False
    try:
        os.kill(pid, 0)
        return True
    except OSError:
        return False


def ensure_dependencies(force=False):
    package_json = CONTROL_PANEL_DIR / "package.json"
    if not package_json.exists():
        print(f"[panel] Missing package.json at {package_json}", file=sys.stderr)
        return False

    node_modules = CONTROL_PANEL_DIR / "node_modules"
    if force or not node_modules.exists():
        print("[panel] Installing Node.js dependencies for control-panel/")
        result = run_command(["npm", "install", "--no-audit", "--no-fund"], cwd=CONTROL_PANEL_DIR)
        if result.returncode != 0:
            print("[panel] npm install failed", file=sys.stderr)
            return False
    return True


def build_env(host, port, db_path, scriptfiles_dir):
    env = os.environ.copy()
    env["HOST"] = host
    env["PORT"] = str(port)
    env["DB_PATH"] = str(db_path)
    env["SCRIPTFILES_PATH"] = str(scriptfiles_dir)
    env["SQLITE_DB_PATH"] = str(db_path)
    env["CONTROL_PANEL_HOST"] = host
    env["CONTROL_PANEL_PORT"] = str(port)
    env["CONTROL_PANEL_DB_PATH"] = str(db_path)
    env["CONTROL_PANEL_SCRIPTFILES_PATH"] = str(scriptfiles_dir)
    return env


def start_panel(host, port, db_path, scriptfiles_dir):
    ensure_paths()
    if not node_available():
        print("[panel] node not found; skipping control panel startup.", file=sys.stderr)
        return 1
    if not npm_available():
        print("[panel] npm not found; skipping control panel startup.", file=sys.stderr)
        return 1
    if not ensure_dependencies():
        return 1

    pid = read_pid_file()
    if pid is not None and is_process_running(pid):
        print(f"[panel] Already running with PID {pid}")
        return 0

    env = build_env(host, port, db_path, scriptfiles_dir)
    LOG_DIR.mkdir(parents=True, exist_ok=True)
    LOG_FILE.parent.mkdir(parents=True, exist_ok=True)

    with LOG_FILE.open("ab") as log_handle:
        process = subprocess.Popen(
            ["node", "server.js"],
            cwd=str(CONTROL_PANEL_DIR),
            env=env,
            stdout=log_handle,
            stderr=subprocess.STDOUT,
            start_new_session=True,
        )

    PID_FILE.write_text(str(process.pid), encoding="utf-8")
    time.sleep(1.5)

    if is_process_running(process.pid):
        print(f"[panel] Control panel started on http://{host}:{port} (PID {process.pid})")
        return 0

    print("[panel] Control panel failed to stay alive after startup.", file=sys.stderr)
    return 1


def stop_panel():
    pid = read_pid_file()
    if pid is None:
        print("[panel] No running panel PID found.")
        return 0

    if not is_process_running(pid):
        PID_FILE.unlink(missing_ok=True)
        print("[panel] Stale PID file removed.")
        return 0

    try:
        os.kill(pid, signal.SIGTERM)
        for _ in range(25):
            if not is_process_running(pid):
                break
            time.sleep(0.2)
    except OSError:
        pass

    PID_FILE.unlink(missing_ok=True)
    print(f"[panel] Stopped control panel PID {pid}")
    return 0


def panel_status():
    pid = read_pid_file()
    if pid is None:
        print("[panel] status: stopped")
        return 1
    if is_process_running(pid):
        print(f"[panel] status: running (PID {pid})")
        return 0
    PID_FILE.unlink(missing_ok=True)
    print("[panel] status: stale pidfile removed")
    return 1


def parse_args():
    parser = argparse.ArgumentParser(description="Prepare and manage the control panel for the Nexus Core gamemode project.")
    parser.add_argument("command", choices=["start", "stop", "status", "restart", "ensure"], nargs="?", default="status")
    parser.add_argument("--host", default=os.environ.get("CONTROL_PANEL_HOST", DEFAULT_HOST))
    parser.add_argument("--port", type=int, default=int(os.environ.get("CONTROL_PANEL_PORT", DEFAULT_PORT)))
    parser.add_argument("--db-path", default=os.environ.get("CONTROL_PANEL_DB_PATH", str(DB_PATH)))
    parser.add_argument("--scriptfiles-path", default=os.environ.get("CONTROL_PANEL_SCRIPTFILES_PATH", str(SCRIPTFILES_DIR)))
    parser.add_argument("--force-install", action="store_true")
    return parser.parse_args()


def main():
    args = parse_args()
    db_path = Path(args.db_path).resolve()
    scriptfiles_path = Path(args.scriptfiles_path).resolve()

    if args.command == "start":
        return start_panel(args.host, args.port, db_path, scriptfiles_path)
    if args.command == "stop":
        return stop_panel()
    if args.command == "restart":
        stop_panel()
        return start_panel(args.host, args.port, db_path, scriptfiles_path)
    if args.command == "ensure":
        ensure_paths()
        if not node_available() or not npm_available():
            print("[panel] Node.js or npm unavailable; control panel is not started in offline mode.", file=sys.stderr)
            return 1
        if not ensure_dependencies(force=args.force_install):
            return 1
        return 0
    return panel_status()


if __name__ == "__main__":
    raise SystemExit(main())
