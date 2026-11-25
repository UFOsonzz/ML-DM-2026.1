import os
from pathlib import Path

from dotenv import load_dotenv
from roboflow import Roboflow

ROOT = Path(__file__).resolve().parents[1]
RAW = ROOT / "data" / "raw"

DATASETS = [
    ("data-science-173-msp9r", "motorcycle-helmet-detection", 2),
    ("mapua-university-n7qm7", "motorcycle-helmet-7d2gr", 2),
]

load_dotenv(ROOT / ".env")
api_key = os.getenv("ROBOFLOW_API_KEY")
if not api_key:
    raise SystemExit("Missing ROBOFLOW_API_KEY in .env")
rf = Roboflow(api_key=api_key)

for workspace, project, version in DATASETS:
    out_dir = RAW / project
    if (out_dir / "data.yaml").exists():
        continue
    rf.workspace(workspace).project(project).version(version).download("yolov8", location=str(out_dir))