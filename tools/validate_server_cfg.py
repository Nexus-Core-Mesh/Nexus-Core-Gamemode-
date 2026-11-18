#!/usr/bin/env python3
import argparse
import re
import sys
from pathlib import Path

REQUIRED_KEYS = {
    "gamemode0": "gamemode0 <name> <mode>",
    "hostname": "hostname <name>",
    "maxplayers": "maxplayers <count>",
    "port": "port <port>",
    "plugins": "plugins <plugin list>",
}


def parse_cfg(path: Path):
    data = {}
    if not path.exists():
        return data
    for raw_line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        if " " not in line:
            continue
        key, value = line.split(None, 1)
        data[key.lower()] = value.strip()
    return data


def ensure_list(value: str):
    return [item.strip() for item in value.split() if item.strip()]


def validate_cfg(root: Path, do_fix: bool = False):
    issues = []
    server_cfg_path = root / "server.cfg"
    sql_cfg_path = root / "sql.cfg"

    if not server_cfg_path.exists():
        issues.append(f"[ERROR] server.cfg fehlt: {server_cfg_path}")
    else:
        cfg = parse_cfg(server_cfg_path)
        for key, hint in REQUIRED_KEYS.items():
            if key not in cfg:
                issues.append(f"[WARN] {key} fehlt in server.cfg ({hint})")

        if "gamemode0" in cfg:
            mode_name = cfg["gamemode0"].split()[0] if cfg["gamemode0"].split() else ""
            mode_file = root / "gamemodes" / f"{mode_name}.pwn"
            if mode_name and not mode_file.exists():
                issues.append(f"[WARN] Gamemode-Datei nicht gefunden: {mode_file}")

        if "plugins" in cfg:
            plugins = ensure_list(cfg["plugins"])
            if not plugins:
                issues.append("[WARN] plugins-Liste ist leer")
            else:
                for plugin in plugins:
                    plugin_path = root / "plugins" / plugin
                    if plugin_path.exists() is False and not plugin.endswith(".dll"):
                        if not (root / f"{plugin}").exists():
                            issues.append(f"[WARN] Plugin-Datei nicht gefunden: {plugin}")

        hostname = cfg.get("hostname", "")
        if not hostname or hostname.lower() in {"changeme", "localhost", "example", "default"}:
            issues.append("[WARN] hostname ist leer oder placeholderhaft")

        port_value = cfg.get("port", "")
        if port_value and not port_value.isdigit():
            issues.append("[WARN] port sollte numerisch sein")

        rcon_password = cfg.get("rcon_password", "")
        if rcon_password.lower() == "changeme":
            issues.append("[WARN] rcon_password verwendet Standardwert changeme")

    if sql_cfg_path.exists():
        sql_cfg = parse_cfg(sql_cfg_path)
        for key in ["mysql_host", "mysql_user", "mysql_db"]:
            if key not in sql_cfg:
                issues.append(f"[WARN] {key} fehlt in sql.cfg")
    else:
        issues.append(f"[INFO] sql.cfg fehlt; falls MySQL genutzt wird, bitte anlegen: {sql_cfg_path}")

    if do_fix and server_cfg_path.exists():
        text = server_cfg_path.read_text(encoding="utf-8", errors="replace")
        if "gamemode0" not in text.lower():
            text += "\n# auto-added by validator\ngamemode0 gtasa_reallife 1\n"
        if "hostname" not in text.lower():
            text += "hostname GTA:SA Reallife Modular\n"
        if "maxplayers" not in text.lower():
            text += "maxplayers 100\n"
        if "port" not in text.lower():
            text += "port 7777\n"
        if "plugins" not in text.lower():
            text += "plugins crashdetect streamer sscanf2 zcmd\n"
        if "rcon_password" not in text.lower():
            text += "rcon_password changeme\n"
        server_cfg_path.write_text(text, encoding="utf-8")
        print(f"[INFO] server.cfg wurde mit Standardwerten ergänzt: {server_cfg_path}")

    for issue in issues:
        print(issue)

    return 0 if not any(issue.startswith("[WARN]") or issue.startswith("[ERROR]") for issue in issues) else 1


def parse_args():
    parser = argparse.ArgumentParser(description="Validiert die SA:MP-Konfiguration")
    parser.add_argument("--root", type=str, default=".", help="Projektwurzel")
    parser.add_argument("--fix", action="store_true", help="Standardwerte ergänzen, falls Konfigurationsfelder fehlen")
    return parser.parse_args()


def main():
    args = parse_args()
    root = Path(args.root).resolve()
    return validate_cfg(root, do_fix=args.fix)


if __name__ == "__main__":
    sys.exit(main())
