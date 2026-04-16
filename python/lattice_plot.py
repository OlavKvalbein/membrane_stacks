import matplotlib.pyplot as plt
import numpy as np

L = 16
Lz = 8

filepath = "data/lattice_1000steps.txt"

spin = np.fromfile(filepath, dtype=np.int8, sep="").reshape((Lz, L, L))

fig, ax = plt.subplots(nrows=Lz)
for i in range(Lz):
    ax[i].imshow(spin[i,:,:], cmap="gray")
plt.show()
