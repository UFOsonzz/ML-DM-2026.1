import os
from pathlib import Path

from dotenv import load_dotenv
from roboflow import Roboflow

ROOT = Path(__file__).resolve().parents[1]
OUT_DIR = ROOT / "data" / "raw"
VERSION = 2 

if not OUT_DIR.exists() or not any(OUT_DIR.iterdir()):
    load_dotenv(ROOT / ".env")
    api_key = os.getenv("ROBOFLOW_API_KEY")
    if not api_key:
        raise SystemExit("Missing ROBOFLOW_API_KEY in .env")
    rf = Roboflow(api_key=api_key)
    project = rf.workspace("data-science-173-msp9r").project("motorcycle-helmet-detection")
    project.version(VERSION).download("yolov8", location=str(OUT_DIR))

for split in ("train", "valid", "test"):
    n_images = len(list((OUT_DIR / split / "images").glob("*")))
    print(f"{split}: {n_images} images")