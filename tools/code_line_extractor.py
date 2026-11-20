#!/usr/bin/env python3
"""Extract line-numbered code lines from the gamemode source tree.

This tool is intentionally manual: it can search a file or a set of files and
print or export numbered code snippets without starting the server.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path


def repo_root_from_file() -> Path:
    return Path(__file__).resolve().parents[1]


def iter_target_files(root: Path, include_path: str | None = None):
    base = Path(include_path) if include_path else root / "gamemodes"
    if base.is_file():
        return [base]
    files = []
    for candidate in [base, base / "modules"]:
        if candidate.exists():
            files.extend(sorted(candidate.rglob("*.pwn")))
            files.extend(sorted(candidate.rglob("*.inc")))
    seen = set()
    unique = []
    for path in files:
        resolved = path.resolve()
        if resolved not in seen:
            seen.add(resolved)
            unique.append(path)
    return unique


def find_lines(path: Path, pattern: str | None = None, start_line: int | None = None, end_line: int | None = None):
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    matches = []
    for index, line in enumerate(lines, start=1):
        if start_line is not None and index < start_line:
            continue
        if end_line is not None and index > end_line:
            continue
        if pattern:
            regex = re.compile(pattern, re.IGNORECASE)
            if not regex.search(line):
                continue
        matches.append({"line": index, "text": line})
    return matches


def export_yson(obj, indent: int = 0):
    pad = "  " * indent
    if isinstance(obj, dict):
        if not obj:
            return "{}"
        parts = []
        for key, value in obj.items():
            parts.append(f"{pad}  {json.dumps(str(key))}: {export_yson(value, indent + 1)}")
        return "{\n" + ",\n".join(parts) + f"\n{pad}}}"
    if isinstance(obj, list):
        if not obj:
            return "[]"
        parts = [f"{pad}  {export_yson(item, indent + 1)}" for item in obj]
        return "[\n" + ",\n".join(parts) + f"\n{pad}]"
    if isinstance(obj, str):
        return json.dumps(obj)
    if obj is None:
        return "null"
    return str(obj)


def main():
    parser = argparse.ArgumentParser(description="Extract numbered source lines from the gamemode tree.")
    parser.add_argument("--root", default=str(repo_root_from_file()), help="Repository root")
    parser.add_argument("--path", help="Single file or directory to inspect")
    parser.add_argument("--pattern", help="Regex pattern to filter lines")
    parser.add_argument("--start", type=int, help="First line number to include")
    parser.add_argument("--end", type=int, help="Last line number to include")
    parser.add_argument("--format", choices=["text", "json", "yson"], default="text")
    parser.add_argument("--output", help="Output file for JSON/YSON export")
    args = parser.parse_args()

    root = Path(args.root).resolve()
    include = args.path
    files = iter_target_files(root, include)
    results = []
    for path in files:
        if not path.exists():
            continue
        lines = find_lines(path, args.pattern, args.start, args.end)
        if lines:
            results.append({
                "file": str(path.relative_to(root)),
                "matches": lines,
            })

    if args.format == "text":
        if not results:
            print("[INFO] No matching lines found.")
            return 0
        for item in results:
            print(f"== {item['file']} ==")
            for entry in item["matches"]:
                print(f"{entry['line']:>5}: {entry['text']}")
        return 0

    payload = {"matches": results}
    text = json.dumps(payload, ensure_ascii=False, indent=2)
    if args.format == "yson":
        text = export_yson(payload)
    if args.output:
        out = Path(args.output)
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(text + "\n", encoding="utf-8")
        print(f"[OK] Wrote {len(results)} file result(s) to {out}")
        return 0
    print(text)
    return 0


if __name__ == "__main__":
    sys.exit(main())
