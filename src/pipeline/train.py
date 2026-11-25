from pathlib import Path
from ultralytics import YOLO

ROOT = Path(__file__).resolve().parents[2]
DATA_YAML = ROOT / "data" / "processed" / "data.yaml"
PRETRAINED_DIR = ROOT / "weights" / "pretrained"
PRETRAINED_DIR.mkdir(parents=True, exist_ok=True)
OUT_DIR = ROOT / "runs" / "pipeline"

model = YOLO(str(PRETRAINED_DIR / "yolov8n.pt"))

model.train(
    data=str(DATA_YAML),
    epochs=50,
    imgsz=640,
    batch=16,
    device="mps",
    patience=15,
    project=str(OUT_DIR),
    name="yolov8n_helmet",
    exist_ok=True,
)