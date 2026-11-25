from pathlib import Path
from ultralytics import YOLO

ROOT = Path(__file__).resolve().parents[2]
DATA_YAML = ROOT / "data" / "processed" / "data.yaml"
BEST_WEIGHTS = ROOT / "runs" / "pipeline" / "yolov8n_helmet" / "weights" / "best.pt"

model = YOLO(str(BEST_WEIGHTS))
metrics = model.val(data=str(DATA_YAML), split="test", imgsz=640)

print("\n=== TEST RESULTS ===")
print(f"mAP@0.5     : {metrics.box.map50:.4f}")
print(f"mAP@0.5:0.95: {metrics.box.map:.4f}")
for i, name in model.names.items():
    p, r, ap50, ap = metrics.box.class_result(i)
    print(f"  [{i}] {name:<12} | P={p:.3f} R={r:.3f} AP50={ap50:.3f} AP={ap:.3f}")