from collections import Counter
from pathlib import Path
import yaml

ROOT = Path(__file__).resolve().parents[1]
DATA_DIR = ROOT / "data" / "processed"

with open(DATA_DIR / "data.yaml") as f:
    names = yaml.safe_load(f)["names"]

for split in ["train", "valid", "test"]:
    counts = Counter()
    empty_files = 0
    widths, heights = [], []
    label_files = list((DATA_DIR / split / "labels").glob("*.txt"))

    for lf in label_files:
        lines = [l.strip() for l in
        lf.read_text().splitlines() if l.strip()]
        if not lines:
            empty_files += 1
            continue
        for line in lines:
            cls_id, _, _, w, h = map(float, line.split())
            counts[int(cls_id)] += 1
            widths.append(w)
            heights.append(h)

    total_boxes = sum(counts.values())

    print(f"\n=== {split.upper()} ({len(label_files)} images,{empty_files} empty, {total_boxes} boxes) ===")
    for idx, name in enumerate(names):
        n = counts[idx]
        pct = (n / total_boxes * 100) if total_boxes else 0
        print(f"  [{idx}] {name:<12}: {n:5d} ({pct:5.1f}%)")
    if widths:
        print(f"  Avg box size (w x h):{sum(widths)/len(widths):.3f} x{sum(heights)/len(heights):.3f}")