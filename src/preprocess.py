import hashlib
import shutil
from pathlib import Path

import cv2
import yaml

ROOT = Path(__file__).resolve().parents[1]
RAW = ROOT / "data" / "raw"
OUT = ROOT / "data" / "processed"
SPLITS = ["train", "valid", "test"]
NAMES = ["full-faced", "half-faced", "invalid", "no helmet"]


def norm(name):
    return name.lower().replace("-", " ")


shutil.rmtree(OUT, ignore_errors=True)
for split in SPLITS:
    (OUT / "images" / split).mkdir(parents=True)
    (OUT / "labels" / split).mkdir(parents=True)

name_to_id = {norm(n): i for i, n in enumerate(NAMES)}
seen = set()  

for ds in sorted(p for p in RAW.iterdir() if (p / "data.yaml").exists()):
    with open(ds / "data.yaml") as f:
        names = yaml.safe_load(f)["names"]
    id_map = {i: name_to_id[norm(n)] for i, n in enumerate(names)}

    for split in SPLITS:
        kept = 0
        dropped = 0

        for img_path in (ds / split / "images").glob("*"):
            label_path = ds / split / "labels" / (img_path.stem + ".txt")

            if not label_path.exists() or cv2.imread(str(img_path)) is None:
                dropped += 1
                continue

            digest = hashlib.md5(img_path.read_bytes()).hexdigest()
            if digest in seen:
                dropped += 1
                continue

            new_lines = []
            valid = True
            with open(label_path) as f:
                for line in f:
                    parts = line.split()
                    if len(parts) == 0:
                        continue
                    if len(parts) != 5 or int(parts[0]) not in id_map:
                        valid = False
                    elif any(float(v) < 0 or float(v) > 1 for v in parts[1:]):
                        valid = False
                    else:
                        parts[0] = str(id_map[int(parts[0])])
                        new_lines.append(" ".join(parts))

            if not valid:
                dropped += 1
                continue

            new_name = ds.name + "_" + img_path.name
            shutil.copy(img_path, OUT / "images" / split / new_name)
            label_out = OUT / "labels" / split / (Path(new_name).stem + ".txt")
            label_out.write_text("".join(line + "\n" for line in new_lines))
            seen.add(digest)
            kept += 1

        print(ds.name, split, "- kept:", kept, "- dropped:", dropped)

data = {
    "path": str(OUT.resolve()),
    "train": "images/train",
    "val": "images/valid",
    "test": "images/test",
    "names": NAMES,
}
with open(OUT / "data.yaml", "w") as f:
    yaml.dump(data, f, sort_keys=False)