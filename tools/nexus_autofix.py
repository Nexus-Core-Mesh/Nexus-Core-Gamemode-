#!/usr/bin/env python3
"""Offline-safe autofix for repo-local runtime config hints.

The project is source-first and does not start a live 7780 server. This script
only repairs obvious placeholder values in repo-local config and docs, while
preserving valid plugin list entries and other user-managed configuration.
"""

from __future__ import annotations

import json
import re
import secrets
import sys
from datetime import datetime
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
    for candidate in candidates:
        if (candidate / "gamemodes" / "gamemode.pwn").is_file() and (candidate / "tools").is_dir():
            return candidate.resolve()
    return Path.cwd().resolve()


REPO = resolve_repo_root()
RUNTIME = REPO / "Compiler" / "samp03"
ALERT_FILE = Path("/tmp/nexus_live_alert.json")
FIX_LOG = Path("/tmp/nexus_autofix.log")
PLACEHOLDER_START = "bash ./scripts/build_gamemode.sh"
SAFE_LINUX_PLUGIN_SUFFIXES = (".so", ".dll")


def log_msg(text: str):
    line = f"[{datetime.now().strftime('%H:%M:%S')}] {text}"
    print(line)
    with FIX_LOG.open("a", encoding="utf-8") as handle:
        handle.write(line + "\n")


def classify_line(line: str):
    lower = line.lower()
    if PLACEHOLDER_START in lower:
        return "FIX_PLACEHOLDER_START", "Platzhalter-Startskript im Repo gefunden"
    if "default password" in lower or "changeme" in lower or "rcon_password" in lower and "changeme" in lower:
        return "FIX_RCON_DEFAULT", "Standard-rcon_password erkannt"
    if "cannot open shared object file" in lower or "plugin-datei nicht gefunden" in lower or "plugin-datei fehlt" in lower:
        return "FIX_PLUGIN_SO", "Plugin-Pfad oder .so-Endung in server.cfg fehlerhaft"
    if "run time error 19" in lower or "run time error 20" in lower:
        return "FIX_RECOMPILE", "AMX-Laufzeitfehler erkannt – Neukompilierung nötig"
    if "[DB_OR_ERROR]" in lower or "[SSCANF_WARN]" in lower or "[LONG_CALLBACK]" in lower:
        return "MANUAL_INSPECT", "Laufzeit-Warnung/DB-Fehler zur Prüfung in S.A.M.P.T.U.I."
    return None, None


def publish_alert(raw_line: str):
    fix_id, summary = classify_line(raw_line)
    if not fix_id:
        return
    payload = {
        "timestamp": datetime.now().isoformat(timespec="seconds"),
        "fix_id": fix_id,
        "summary": summary,
        "raw": raw_line.strip(),
    }
    ALERT_FILE.write_text(json.dumps(payload, ensure_ascii=False, indent=2), encoding="utf-8")


def scan_for_placeholder_start() -> list[Path]:
    matches = []
    for path in REPO.rglob("*"):
        if not path.is_file() or path.suffix.lower() not in {".sh", ".py", ".cfg", ".txt", ".md"}:
            continue
        if ".git" in path.parts:
            continue
        try:
            content = path.read_text(encoding="utf-8", errors="replace")
        except OSError:
            continue
        if PLACEHOLDER_START in content:
            matches.append(path)
    return matches


def fix_placeholder_start():
    replacement = "bash ./scripts/build_gamemode.sh"
    changed = 0
    for path in scan_for_placeholder_start():
        content = path.read_text(encoding="utf-8", errors="replace")
        updated = content.replace(PLACEHOLDER_START, replacement)
        if updated != content:
            path.write_text(updated, encoding="utf-8")
            changed += 1
            log_msg(f"[FIXED] Platzhalter ersetzt in: {path}")
    if changed == 0:
        log_msg("[INFO] Kein Platzhalter 'bash ./scripts/build_gamemode.sh' mehr gefunden.")


def fix_rcon_default():
    new_pw = "Nexus" + secrets.token_hex(6)
    for cfg in [REPO / "server.cfg", RUNTIME / "server.cfg"]:
        if not cfg.is_file():
            continue
        lines = cfg.read_text(encoding="utf-8", errors="replace").splitlines()
        updated = []
        modified = False
        for line in lines:
            if line.strip().startswith("rcon_password") and "changeme" in line.lower():
                updated.append(f"rcon_password {new_pw}")
                modified = True
            else:
                updated.append(line)
        if modified:
            cfg.write_text("\n".join(updated) + "\n", encoding="utf-8")
            log_msg(f"[FIXED] rcon_password in {cfg} aktualisiert.")


def plugin_candidates() -> list[str]:
    root_plugins = REPO / "plugins"
    runtime_plugins = RUNTIME / "plugins"
    plugin_names = []
    for directory in [root_plugins, runtime_plugins]:
        if directory.is_dir():
            for path in sorted(directory.iterdir()):
                if path.is_file() and path.suffix.lower() in {".so", ".dll"}:
                    plugin_names.append(path.name)
    return plugin_names


def fix_plugin_so():
    plugin_names = plugin_candidates()
    if not plugin_names:
        log_msg("[INFO] Keine Plugin-Dateien im Repo gefunden; Plugin-Check unverändert.")
        return

    for cfg in [REPO / "server.cfg", RUNTIME / "server.cfg"]:
        if not cfg.is_file():
            continue
        text = cfg.read_text(encoding="utf-8", errors="replace")
        lines = text.splitlines()
        new_lines = []
        changed = False
        for line in lines:
            stripped = line.strip()
            if not stripped or stripped.startswith("#"):
                new_lines.append(line)
                continue
            if stripped.lower().startswith("plugins"):
                tokens = stripped.split()
                if len(tokens) >= 2:
                    current = tokens[1:]
                    cleaned = []
                    for item in current:
                        lower_item = item.lower()
                        if any(item.endswith(suffix) for suffix in SAFE_LINUX_PLUGIN_SUFFIXES):
                            cleaned.append(item)
                            continue
                        if item in plugin_names:
                            cleaned.append(item)
                            continue
                        if item.startswith("./") or item.startswith("plugins/"):
                            fixed = item.rsplit("/", 1)[-1]
                            if fixed in plugin_names:
                                cleaned.append(fixed)
                                changed = True
                                continue
                        if item in {"crashdetect", "streamer", "sscanf2", "zcmd"}:
                            cleaned.append(item)
                            continue
                        if item.startswith("plugin") and not item.endswith((".so", ".dll")):
                            # Keep the current config stable unless a real placeholder is present.
                            cleaned.append(item)
                            continue
                        cleaned.append(item)
                    if cleaned != current:
                        changed = True
                    new_lines.append("plugins " + " ".join(cleaned))
                    continue
            new_lines.append(line)
        if changed:
            cfg.write_text("\n".join(new_lines) + "\n", encoding="utf-8")
            log_msg(f"[FIXED] Plugin-Liste in {cfg} normalisiert.")


def fix_runtime_alias():
    stale_aliases = [
        REPO / "gamemode.amx",
        RUNTIME / "gamemodes" / "gamemode.amx",
    ]
    for path in stale_aliases:
        if path.exists():
            try:
                path.unlink()
                log_msg(f"[FIXED] Legacy alias entfernt: {path}")
            except OSError as exc:
                log_msg(f"[WARN] Legacy alias konnte nicht entfernt werden: {path} ({exc})")


def main():
    FIX_LOG.parent.mkdir(parents=True, exist_ok=True)
    FIX_LOG.write_text("", encoding="utf-8")
    log_msg(f"[START] Repo-root: {REPO}")
    fix_placeholder_start()
    fix_rcon_default()
    fix_plugin_so()
    fix_runtime_alias()
    log_msg("[DONE] Offline autofix abgeschlossen; kein Live-Server-Start erforderlich.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
