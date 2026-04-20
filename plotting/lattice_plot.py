import matplotlib.pyplot as plt
import numpy as np

def load_spin(filepath, L, Lz):
    return np.fromfile(filepath, dtype=np.int8, sep="").reshape((Lz, L, L))

def plot_timeseries(folderpath, n_lattices, L, Lz):
    lattices = [load_spin(folderpath + f"/{i}", L, Lz)
                for i in range(n_lattices)]

    fig, ax = plt.subplots(nrows=Lz, ncols=n_lattices)
    for i in range(n_lattices):
        for z in range(Lz):
            ax[z][i].imshow(lattices[i][z,:,:], cmap="gray")
    plt.show()

L = 16
Lz = 4
plot_timeseries("data/lattice", 9, L, Lz)