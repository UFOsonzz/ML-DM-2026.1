import shutil
from pathlib import Path

import cv2
import yaml

ROOT = Path(__file__).resolve().parents[1]
RAW = ROOT / "data" / "raw"
OUT = ROOT / "data" / "processed"

with open(RAW / "data.yaml") as f:
    names = yaml.safe_load(f)["names"]

for split in ["train", "valid", "test"]:
    (OUT / split / "images").mkdir(parents=True, exist_ok=True)
    (OUT / split / "labels").mkdir(parents=True, exist_ok=True)

    kept = 0
    dropped = 0

    for img_path in (RAW / split / "images").glob("*"):
        label_path = RAW / split / "labels" / (img_path.stem + ".txt")

        if not label_path.exists():
            dropped += 1
            continue

        if cv2.imread(str(img_path)) is None:
            dropped += 1
            continue

        valid = True
        with open(label_path) as f:
            for line in f:
                parts = line.split()
                if len(parts) == 0:
                    continue
                if len(parts) != 5:
                    valid = False
                elif int(parts[0]) < 0 or int(parts[0]) >= len(names):
                    valid = False
                elif any(float(v) < 0 or float(v) > 1 for v in parts[1:]):
                    valid = False

        if not valid:
            dropped += 1
            continue

        shutil.copy(img_path, OUT / split / "images" / img_path.name)
        shutil.copy(label_path, OUT / split / "labels" / label_path.name)
        kept += 1

    print(split, "- kept:", kept, "- dropped:", dropped)

data = {
    "path": str(OUT.resolve()),
    "train": "train/images",
    "val": "valid/images",
    "test": "test/images",
    "names": names,
}
with open(OUT / "data.yaml", "w") as f:
    yaml.dump(data, f, sort_keys=False)