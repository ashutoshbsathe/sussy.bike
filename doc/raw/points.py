import json
import numpy as np
import matplotlib.pyplot as plt
import triangle as tr
a = (2, 2)
b = (3, 2.125)
c = (6, 3)
d = (9, 2)
e = (10.25, 3.875)
f = (9, 4.25)
g = (8.375, 5)
h = (8.25, 6)
i = (7, 7.125) # copy again if looked weird
j = (4.75, 7.25)
k = (4, 6.375)
l = (1.5, 7.5)
m = (1.5, 7)
n = (4, 6.125)
o = (4, 5.625)
p = (4.375, 5)
q = (6, 3.5)
r = (3.375, 2.5)
s = (4.625, 4.75)
t = (6, 4)
u = (8, 6)
v = (7, 7)
w = (5, 7)
x = (4.375, 5.5)
y = (3, 2.5)
z = (2.125, 2.25)

boundary = [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, y, z]
h1 = [q, r, s]
h2 = [t, u, v, w, x]

normalize_x_min = min(x_i[0] for x_i in boundary)
normalize_x_max = max(x_i[0] for x_i in boundary)
normalize_y_min = min(y_i[0] for y_i in boundary)
normalize_y_max = max(y_i[0] for y_i in boundary)

boundary = [(x_i - normalize_x_min, normalize_y_max - y_i - normalize_y_min) for (x_i, y_i) in boundary]
h1 = [(x_i - normalize_x_min, normalize_y_max - y_i - normalize_y_min) for (x_i, y_i) in h1]
h2 = [(x_i - normalize_x_min, normalize_y_max - y_i - normalize_y_min) for (x_i, y_i) in h2]

"""
plt.fill(
    [x_i[0] for x_i in boundary],
    [x_i[1] for x_i in boundary]
)
plt.fill(
    [x_i[0] for x_i in h1],
    [x_i[1] for x_i in h1]
)
plt.fill(
    [x_i[0] for x_i in h2],
    [x_i[1] for x_i in h2]
)
plt.show()
"""

segments = []
count = 0
for idx in range(len(boundary)):
    idx1 = count + idx
    idx2 = count + (idx + 1) % len(boundary)
    segments.append((idx1, idx2))
count = len(boundary)
for idx in range(len(h1)):
    idx1 = count + idx
    idx2 = count + (idx + 1) % len(h1)
    segments.append((idx1, idx2))
count = len(boundary) + len(h1)
for idx in range(len(h2)):
    idx1 = count + idx
    idx2 = count + (idx + 1) % len(h2)
    segments.append((idx1, idx2))
h1_pt = np.mean(h1, axis=0)
h1_pt = (h1_pt[0], h1_pt[1])
h2_pt = np.mean(h2, axis=0)
h2_pt = (h2_pt[0], h2_pt[1])
bike = dict(
    vertices = np.array(boundary + h1 + h2, dtype=np.float32),
    segments = np.array(segments, dtype=np.uint32),
    holes = np.array([h1_pt, h2_pt], dtype=np.float32)
)
bike_tr = tr.triangulate(bike, 'p')
with open('bike_body.json', 'w') as f:
    json.dump({k: v.tolist() if isinstance(v, np.ndarray) else v for k, v in bike_tr.items()}, f, indent=4)
tr.compare(plt, bike, bike_tr)
plt.show()
