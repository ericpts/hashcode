

import numpy as np;
import matplotlib.pyplot as plt;
import matplotlib.patches as patches;
import sys

a = [];
b = [];
c = [];
rects = [];

for line in sys.stdin:
    w = line.split();
    x = int(w[0]);
    y = int(w[1]);
    z = int(w[2]);
    a.append(x);
    b.append(y);
    c.append(z);

# rect = patches.Rectangle((x-ww,y-ww), 2*ww, 2*ww, linewidth=0.5, edgecolor='r', facecolor='none');

# b = sorted(b);

print(np.array(a));
print(np.array(b));
print(np.array(c));

# print(np.mean(y));
# print(np.std(y));
# print(y.shape);
# plt.scatter(np.arange(0, len(b), 1), b);

plt.scatter(np.array(b), np.array(a), s=2, c=np.array(c) / 100, cmap='RdYlGn');
# plt.axhline(np.mean(y), color='red', linewidth=1);

axes = plt.gca();
# axes.set_ylim([0, 100000]);

# axes.add_patch(rect);

plt.show();