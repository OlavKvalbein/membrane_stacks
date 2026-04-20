import matplotlib.pyplot as plt
import numpy as np

def load_spin(filepath, Lz, L):
    return np.fromfile(filepath, dtype=np.int8, sep="").reshape((Lz, L, L))

L = 16
Lz = 4

start = load_spin("data/big_lattice/start", Lz, L)
middle = load_spin("data/big_lattice/middle", Lz, L)
end = load_spin("data/big_lattice/end", Lz, L)

fig, ax = plt.subplots(nrows=Lz, ncols=3)
for i in range(Lz):
    ax[i][0].imshow(start[i,:,:], cmap="gray")
for i in range(Lz):
    ax[i][1].imshow(middle[i,:,:], cmap="gray")
for i in range(Lz):
    ax[i][2].imshow(end[i,:,:], cmap="gray")
plt.show()
