#!/usr/bin/env python3
import argparse
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

COMMON_SERVER_BINARIES = [
    "samp03svr",
    "samp-server",
    "samp-server.exe",
    "gta_sa_server",
    "gta_sa_server.exe",
    "server",
]


def find_server_binary(root: Path):
    candidates = [root / binary for binary in COMMON_SERVER_BINARIES]
    candidates += list(root.rglob("samp*"))
    candidates += list(root.rglob("*server*"))
    for item in candidates:
        if item.is_file() and os.access(item, os.X_OK):
            return item
    return None


def find_log_file(root: Path):
    for path in [root / "logs" / "server.log", root / "log" / "server.log", root / "server_log.txt", root / "server.log"]:
        if path.exists():
            return path
    return root / "logs" / "server.log"


def tail_log(path: Path, lines: int = 40):
    if not path.exists():
        return
    with path.open("r", encoding="utf-8", errors="replace") as handle:
        data = handle.read().splitlines()
    for line in data[-lines:]:
        print(line)


def parse_args():
    parser = argparse.ArgumentParser(description="Server-Launcher mit Log-Überwachung")
    parser.add_argument("--root", type=str, default=".", help="Projektwurzel")
    parser.add_argument("--server-bin", type=str, default="", help="Pfad zum SA:MP-Server-Binary")
    parser.add_argument("--log", type=str, default="", help="Pfad zur Logdatei")
    parser.add_argument("--watch", action="store_true", help="Log-Datei nach dem Start beobachten")
    parser.add_argument("--check-only", action="store_true", help="Nur prüfen, ob Launcher-/Serverdaten vorhanden sind")
    return parser.parse_args()


def main():
    args = parse_args()
    root = Path(args.root).resolve()
    server_bin = Path(args.server_bin).expanduser() if args.server_bin else find_server_binary(root)
    log_path = Path(args.log).expanduser() if args.log else find_log_file(root)

    if not server_bin:
        print("[WARN] Kein SA:MP-Server-Binary im Projekt gefunden.")
        print("[INFO] Beispiel: --server-bin ./samp03svr")
        return 1

    if args.check_only:
        print(f"[INFO] Server-Binary gefunden: {server_bin}")
        print(f"[INFO] Log-Ziel: {log_path}")
        return 0

    log_path.parent.mkdir(parents=True, exist_ok=True)
    cmd = [str(server_bin)]
    print(f"[INFO] Starte Server: {' '.join(cmd)}")
    print(f"[INFO] Logdatei: {log_path}")

    with open(log_path, "a", encoding="utf-8") as log_handle:
        process = subprocess.Popen(cmd, stdout=log_handle, stderr=subprocess.STDOUT, cwd=str(root))
        try:
            while True:
                return_code = process.poll()
                if return_code is not None:
                    print(f"[INFO] Server beendet. Exit-Code: {return_code}")
                    break
                time.sleep(2)
                if args.watch:
                    tail_log(log_path, lines=20)
        except KeyboardInterrupt:
            print("\n[INFO] Server-Session per Ctrl+C beendet.")
            process.terminate()
            return 0

    return 0


if __name__ == "__main__":
    sys.exit(main())
