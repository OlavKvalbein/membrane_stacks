import matplotlib.pyplot as plt
import numpy as np

L = 64
Lz = 5

start_path = "data/big_lattice/start"
end_path = "data/big_lattice/end"

start = np.fromfile(start_path, dtype=np.int8, sep="").reshape((Lz, L, L))
end = np.fromfile(end_path, dtype=np.int8, sep="").reshape((Lz, L, L))

fig, ax = plt.subplots(nrows=Lz)
for i in range(Lz):
    ax[i].imshow(start[i,:,:], cmap="gray")
plt.show()

fig, ax = plt.subplots(nrows=Lz)
for i in range(Lz):
    ax[i].imshow(end[i,:,:], cmap="gray")
plt.show()
