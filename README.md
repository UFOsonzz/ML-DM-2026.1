
Data Pipeline : download from roboflow -> preprocess

train - kept: 1545 - dropped: 0
valid - kept: 438 - dropped: 0
test - kept: 232 - dropped: 0

--> eda.py 

=== TRAIN (1545 images,3 empty, 4711 boxes) ===
  [0] full-faced  :  1865 ( 39.6%)
  [1] half-faced  :  2055 ( 43.6%)
  [2] invalid     :   529 ( 11.2%)
  [3] no helmet   :   262 (  5.6%)
  Avg box size (w x h):0.071 x0.144

=== VALID (438 images,1 empty, 1451 boxes) ===
  [0] full-faced  :   565 ( 38.9%)
  [1] half-faced  :   650 ( 44.8%)
  [2] invalid     :   159 ( 11.0%)
  [3] no helmet   :    77 (  5.3%)
  Avg box size (w x h):0.071 x0.142

=== TEST (232 images,2 empty, 822 boxes) ===
  [0] full-faced  :   323 ( 39.3%)
  [1] half-faced  :   367 ( 44.6%)
  [2] invalid     :    93 ( 11.3%)
  [3] no helmet   :    39 (  4.7%)
  Avg box size (w x h):0.071 x0.143


for yolo pipeline : train.py -> eval.py -> predict.py