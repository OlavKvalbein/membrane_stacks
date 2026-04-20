import matplotlib.pyplot as plt
import numpy as np

def load_spin(filepath, Lz, L):
    return np.fromfile(filepath, dtype=np.int8, sep="").reshape((Lz, L, L))

L = 64
Lz = 4

filepath = "data/big_lattice/middle"

spin = load_spin(filepath, Lz, L)

fig, ax = plt.subplots(nrows=Lz)
for i in range(Lz):
    ax[i].imshow(spin[i,:,:], cmap="gray")
plt.show()

filepath = "data/big_lattice/end"

spin = load_spin(filepath, Lz, L)

fig, ax = plt.subplots(nrows=Lz)
for i in range(Lz):
    ax[i].imshow(spin[i,:,:], cmap="gray")
plt.show()
