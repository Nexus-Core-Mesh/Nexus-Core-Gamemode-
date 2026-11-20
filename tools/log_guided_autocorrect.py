#!/usr/bin/env python3
"""Manual, log-guided autocorrection helper for stability fixes.

The tool is intentionally conservative: by default it performs a dry-run preview,
shows a diff-like summary, and only writes changes when --apply is passed.
"""

from __future__ import annotations

import argparse
import difflib
import json
import re
import shutil
import sys
from pathlib import Path


def read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except FileNotFoundError:
        return ""


def iter_log_files(root: Path):
    candidates = [
        root / "Compiler/samp03/server_log.txt",
        root / "samp7780_bg.log",
        root / "server_log.txt",
    ]
    return [p for p in candidates if p.exists()] 


def extract_log_findings(path: Path):
    text = read_text(path)
    if not text:
        return []
    hits = []
    for idx, line in enumerate(text.splitlines(), start=1):
        lowered = line.lower()
        if any(token in lowered for token in ["error", "warning", "failed", "panic", "sscanf", "callback", "state", "spawn", "camera", "dpick", "textdraw"]):
            hits.append({"line": idx, "text": line.rstrip()})
    return hits


def find_code_matches(root: Path):
    candidates = []
    for pattern in [
        r"!=\s*Text:\s*0",
        r"==\s*Text:\s*0",
        r"sscanf\s*\(",
        r"SetCameraBehindPlayer\s*\(",
        r"TogglePlayerControllable\s*\([^\n]*,\s*1\s*\)",
    ]:
        regex = re.compile(pattern, re.I)
        for file in sorted((root / "gamemodes").rglob("*.pwn")) + sorted((root / "gamemodes").rglob("*.inc")):
            content = read_text(file)
            if regex.search(content):
                candidates.append({
                    "file": str(file.relative_to(root)),
                    "pattern": pattern,
                    "matches": regex.findall(content),
                    "line_count": content.count("\n") + 1,
                })
    return candidates


def render_preview(file_path: Path, original: str, updated: str):
    diff = difflib.unified_diff(original.splitlines(), updated.splitlines(), fromfile=str(file_path), tofile=f"{file_path} (patched)", lineterm="")
    return "\n".join(diff)


def apply_patch(path: Path, original: str):
    updated = original
    updated = re.sub(r"!=\s*Text:\s*0", "!= Text:INVALID_TEXT_DRAW", updated)
    updated = re.sub(r"==\s*Text:\s*0", "== Text:INVALID_TEXT_DRAW", updated)
    if updated != original:
        backup = path.with_suffix(path.suffix + ".bak")
        if not backup.exists():
            shutil.copy2(path, backup)
        path.write_text(updated, encoding="utf-8")
        return True, backup
    return False, None


def write_exports(root: Path, records, output: Path | None):
    if output is None:
        return
    output.parent.mkdir(parents=True, exist_ok=True)
    json_path = output
    yson_path = output.with_suffix(".yson")
    json_path.write_text(json.dumps(records, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    yson_path.write_text(serialize_yson(records) + "\n", encoding="utf-8")


def serialize_yson(obj, indent: int = 0):
    pad = "  " * indent
    if isinstance(obj, dict):
        if not obj:
            return "{}"
        parts = []
        for key, value in obj.items():
            parts.append(f"{pad}  {json.dumps(str(key))}: {serialize_yson(value, indent + 1)}")
        return "{\n" + ",\n".join(parts) + f"\n{pad}}}"
    if isinstance(obj, list):
        if not obj:
            return "[]"
        parts = [f"{pad}  {serialize_yson(item, indent + 1)}" for item in obj]
        return "[\n" + ",\n".join(parts) + f"\n{pad}]"
    if isinstance(obj, str):
        return json.dumps(obj)
    if obj is None:
        return "null"
    return str(obj)


def main():
    parser = argparse.ArgumentParser(description="Manual log-guided autocorrect with dry-run preview.")
    parser.add_argument("--root", default=".", help="Project root")
    parser.add_argument("--log", help="Optional explicit log file")
    parser.add_argument("--output", help="Optional report file for JSON/YSON export")
    parser.add_argument("--apply", action="store_true", help="Apply safe replacements; default is dry-run")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    log_paths = [Path(args.log)] if args.log else iter_log_files(root)
    findings = []
    for log_file in log_paths:
        if log_file.exists():
            findings.extend(extract_log_findings(log_file))

    code_matches = find_code_matches(root)
    records = {
        "log_findings": findings,
        "candidate_code_issues": code_matches,
        "dry_run": not args.apply,
    }

    if args.output:
        write_exports(root, records, Path(args.output))

    if not args.apply:
        print("[DRY-RUN] Preview only; no files were written.")
        print(json.dumps(records, ensure_ascii=False, indent=2)[:4000])
        if not code_matches:
            print("[INFO] No obvious sources matched the safety patterns.")
        return 0

    changed = []
    for issue in code_matches:
        path = root / issue["file"]
        if not path.exists():
            continue
        original = read_text(path)
        patched, backup = apply_patch(path, original)
        if patched:
            changed.append({"file": issue["file"], "backup": str(backup) if backup else None, "preview": render_preview(path, original, read_text(path))[:1200]})

    print(json.dumps({"applied": changed}, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(main())
