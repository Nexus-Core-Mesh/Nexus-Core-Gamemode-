#!/usr/bin/env python3
from pathlib import Path
import runpy
import sys

REPO = Path(__file__).resolve().parent
TARGET = REPO / "tools" / "nexus_hub_tui.py"
if TARGET.exists():
    sys.path.insert(0, str(REPO / "tools"))
    runpy.run_path(str(TARGET), run_name="__main__")
else:
    raise SystemExit(f"[ERROR] Missing project script: {TARGET}")
