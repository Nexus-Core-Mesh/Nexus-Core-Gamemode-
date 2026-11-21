#!/usr/bin/env python3
import json
import re
import secrets
import subprocess
import sys
from datetime import datetime
from pathlib import Path

REPO = Path(__file__).resolve().parent
RUNTIME = REPO / "Compiler" / "samp03"
ALERT_FILE = Path("/tmp/nexus_live_alert.json")
FIX_LOG = Path("/tmp/nexus_autofix.log")


def log_msg(text: str):
    line = f"[{datetime.now().strftime('%H:%M:%S')}] {text}"
    print(line)
    with FIX_LOG.open("a", encoding="utf-8") as f:
        f.write(line + "\n")


def classify_line(line: str):
    if "dein_startskript_fuer_7780.sh" in line:
        return "FIX_PLACEHOLDER_START", "Platzhalter-Startskript im Repo gefunden"
    if "Your password must be changed from the default password" in line or "changeme" in line:
        return "FIX_RCON_DEFAULT", "Standard-rcon_password erkannt"
    if "cannot open shared object file" in line or "Plugin-Datei nicht gefunden" in line:
        return "FIX_PLUGIN_SO", "Plugin-Pfad oder .so-Endung in server.cfg fehlerhaft"
    if "Run time error 19" in line or "Run time error 20" in line:
        return "FIX_RECOMPILE", "AMX-Laufzeitfehler erkannt – Neukompilierung nötig"
    if "[DB_OR_ERROR]" in line or "[SSCANF_WARN]" in line or "[LONG_CALLBACK]" in line:
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


def fix_placeholder_start():
    target = str(RUNTIME / "start_7780.sh")
    changed = 0
    for path in REPO.rglob("*"):
        if not path.is_file() or path.suffix not in {".sh", ".py", ".cfg", ".txt", ".md"}:
            continue
        if ".git" in path.parts:
            continue
        content = path.read_text(encoding="utf-8", errors="replace")
        if "./dein_startskript_fuer_7780.sh" in content:
            path.write_text(content.replace("./dein_startskript_fuer_7780.sh", target), encoding="utf-8")
            changed += 1
            log_msg(f"[FIXED] Platzhalter ersetzt in: {path}")
    if changed == 0:
        log_msg("[INFO] Kein Platzhalter './dein_startskript_fuer_7780.sh' mehr gefunden.")


def fix_rcon_default():
    new_pw = "Nexus" + secrets.token_hex(6)
    for cfg in [RUNTIME / "server.cfg", REPO / "server.cfg"]:
        if not cfg.is_file():
            continue
        lines = cfg.read_text(encoding="utf-8", errors="replace").splitlines()
        updated = []
        modified = False
        for line in lines:
            if line.strip().startswith("rcon_password") and "changeme" in line:
                updated.append(f"rcon_password {new_pw}")
                modified = True
            else:
                updated.append(line)
        if modified:
            cfg.write_text("\n".join(updated) + "\n", encoding="utf-8")
            log_msg(f"[FIXED] Neues rcon_password in {cfg} gesetzt.")


def fix_plugin_so():
    plugins_dir = RUNTIME / "plugins"
    cfg = RUNTIME / "server.cfg"
    if not plugins_dir.is_dir() or not cfg.is_file():
        log_msg("[WARN] plugins/-Ordner oder server.cfg fehlt.")
        return
    available = {p.stem: p.name for p in plugins_dir.glob("*.so")}
    lines = cfg.read_text(encoding="utf-8", errors="replace").splitlines()
    updated = []
    for line in lines:
        if line.strip().startswith("plugins "):
            tokens = line.split()[1:]
            fixed_tokens = []
            for tok in tokens:
                stem = tok[:-3] if tok.endswith(".so") else tok
                if stem in available:
                    fixed_tokens.append(available[stem])
                elif stem == "sscanf2" and "sscanf" in available:
                    fixed_tokens.append(available["sscanf"])
                else:
                    log_msg(f"[WARN] Kein passendes .so gefunden für: {tok}")
            updated.append("plugins " + " ".join(fixed_tokens))
        else:
            updated.append(line)
    cfg.write_text("\n".join(updated) + "\n", encoding="utf-8")
    log_msg("[FIXED] plugins-Zeile in server.cfg mit vorhandenen .so-Dateien abgeglichen.")


def fix_recompile():
    build_script = REPO / "build_server.sh"
    if not build_script.is_file():
        log_msg("[WARN] build_server.sh nicht gefunden.")
        return
    res = subprocess.run(["bash", str(build_script)], cwd=str(REPO), stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    log_msg(f"[BUILD] build_server.sh beendet mit Code {res.returncode}")


def apply_current_alert():
    if not ALERT_FILE.is_file():
        log_msg("[INFO] Keine aktive Live-Meldung in /tmp/nexus_live_alert.json.")
        return
    data = json.loads(ALERT_FILE.read_text(encoding="utf-8"))
    fix_id = data.get("fix_id")
    log_msg(f"[START] Bearbeite {fix_id}: {data.get('summary')}")
    if fix_id == "FIX_PLACEHOLDER_START":
        fix_placeholder_start()
    elif fix_id == "FIX_RCON_DEFAULT":
        fix_rcon_default()
    elif fix_id == "FIX_PLUGIN_SO":
        fix_plugin_so()
    elif fix_id == "FIX_RECOMPILE":
        fix_recompile()
    else:
        log_msg(f"[INFO] Kein automatischer Patch für {fix_id} hinterlegt: {data.get('raw')}")
    ALERT_FILE.unlink(missing_ok=True)
    log_msg("[OK] Alert quittiert und entfernt.")


if __name__ == "__main__":
    if len(sys.argv) >= 3 and sys.argv[1] == "--ingest":
        publish_alert(" ".join(sys.argv[2:]))
    else:
        apply_current_alert()
