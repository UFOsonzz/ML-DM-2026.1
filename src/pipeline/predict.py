from pathlib import Path
import cv2
from ultralytics import YOLO

ROOT = Path(__file__).resolve().parents[2]
BEST_WEIGHTS = ROOT / "runs" / "pipeline" / "yolov8n_helmet" / "weights" / "best.pt"
TEST_IMAGES = ROOT / "data" / "processed" / "test" / "images"
SAVE_DIR = ROOT / "runs" / "pipeline" / "predictions"
SAVE_DIR.mkdir(parents=True, exist_ok=True)

model = YOLO(str(BEST_WEIGHTS))

for img_path in sorted(TEST_IMAGES.glob("*"))[:10]: 
    img = cv2.imread(str(img_path))
    results = model.predict(img, imgsz=640, conf=0.25, verbose=False)[0]

    for box in results.boxes:
        x1, y1, x2, y2 = map(int, box.xyxy[0])
        cls_id = int(box.cls[0])
        conf = float(box.conf[0])
        label = model.names[cls_id]

        color = (0, 0, 255) if label == "no helmet" else (0, 200, 0)
        cv2.rectangle(img, (x1, y1), (x2, y2), color, 2)
        cv2.putText(img, f"{label} {conf:.2f}", (x1, max(y1 - 6, 15)),
            cv2.FONT_HERSHEY_SIMPLEX, 0.55, color, 2)

    cv2.imwrite(str(SAVE_DIR / img_path.name), img)

print(f"Saved predictions to {SAVE_DIR}")