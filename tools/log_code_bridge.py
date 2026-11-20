#!/usr/bin/env python3
"""Bridge log findings to numbered source snippets.

This utility is intentionally offline and manual: it reads logs and then maps the
relevant keywords back to the source tree without starting the server.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Any

ROOT = Path(__file__).resolve().parents[1]


def read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except FileNotFoundError:
        return ""


def find_log_files(root: Path, explicit: str | None):
    candidates = []
    if explicit:
        candidates.append(Path(explicit))
    else:
        for name in ["Compiler/samp03/server_log.txt", "samp7780_bg.log", "server_log.txt"]:
            candidates.append(root / name)
    unique = []
    for path in candidates:
        if path.exists() and path.is_file() and path not in unique:
            unique.append(path)
    return unique


def scan_log(log_path: Path):
    text = read_text(log_path)
    if not text:
        return []
    findings = []
    pattern = re.compile(r"(error|warning|failed|fatal|panic|sscanf|long callback|state|db|spawn|camera|toggle|invalid|textdraw)", re.I)
    for idx, line in enumerate(text.splitlines(), start=1):
        if pattern.search(line):
            findings.append({"line": idx, "text": line.strip()})
    return findings


def source_hits_for_text(root: Path, text: str):
    tokens = [x for x in re.findall(r"[A-Za-z_][A-Za-z0-9_]{2,}", text) if len(x) >= 4]
    keywords = []
    for token in tokens:
        if token.lower() in {"error", "warning", "failed", "state", "spawn", "db", "toggle", "textdraw", "camera", "long", "callback", "player"}:
            continue
        keywords.append(token)
    unique = []
    for keyword in keywords[:12]:
        if keyword not in unique:
            unique.append(keyword)
    results = []
    for file in sorted((root / "gamemodes").rglob("*.pwn")) + sorted((root / "gamemodes").rglob("*.inc")):
        try:
            file_text = file.read_text(encoding="utf-8", errors="replace")
        except OSError:
            continue
        for keyword in unique:
            if re.search(re.escape(keyword), file_text, re.I):
                results.append({"file": str(file.relative_to(root)), "keyword": keyword})
                break
    return results


def to_yson(obj, indent: int = 0):
    pad = "  " * indent
    if isinstance(obj, dict):
        if not obj:
            return "{}"
        parts = []
        for key, value in obj.items():
            parts.append(f"{pad}  {json.dumps(str(key))}: {to_yson(value, indent + 1)}")
        return "{\n" + ",\n".join(parts) + f"\n{pad}}}"
    if isinstance(obj, list):
        if not obj:
            return "[]"
        parts = [f"{pad}  {to_yson(item, indent + 1)}" for item in obj]
        return "[\n" + ",\n".join(parts) + f"\n{pad}]"
    if isinstance(obj, str):
        return json.dumps(obj)
    if obj is None:
        return "null"
    return str(obj)


def main():
    parser = argparse.ArgumentParser(description="Bridge log findings to numbered source locations.")
    parser.add_argument("--root", default=str(ROOT), help="Repository root")
    parser.add_argument("--log", help="Explicit log file path")
    parser.add_argument("--output", help="Optional output path for JSON/YSON bridge export")
    parser.add_argument("--dry-run", action="store_true", help="Print a preview instead of writing output")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    log_paths = find_log_files(root, args.log)
    documents = []
    for log_path in log_paths:
        findings = scan_log(log_path)
        for item in findings:
            item["related_sources"] = source_hits_for_text(root, item["text"])
        documents.append({"log": str(log_path.relative_to(root)), "findings": findings})

    payload = {"bridge": documents}
    json_output = json.dumps(payload, ensure_ascii=False, indent=2)
    yson_output = to_yson(payload)

    if args.output and not args.dry_run:
        out = Path(args.output)
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(json_output + "\n", encoding="utf-8")
        out.with_suffix(".yson").write_text(yson_output + "\n", encoding="utf-8")
        print(f"[OK] Wrote bridge report to {out.parent}")
        return 0

    print(json_output if not args.dry_run else json_output)
    if args.dry_run:
        print("\n--- .yson preview ---")
        print(yson_output[:2000])
    return 0


if __name__ == "__main__":
    sys.exit(main())
