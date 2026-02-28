import numpy as np
import matplotlib.pyplot as plt
from lattice import lattice_series

def plot_series():
    L = 20
    Lz = 5
    T_J_ratio = 1.5
    Jz_J_ratio = 0.5

    show_after_steps = [0, 50, 100, 150, 200]
    MC_steps = max(show_after_steps)

    series = lattice_series(L, Lz, T_J_ratio, Jz_J_ratio, MC_steps)
    fig, ax = plt.subplots(nrows=Lz, ncols=len(show_after_steps))
    fig.supylabel("z")
    for j in range(len(show_after_steps)):
        ax[0][j].set_title(f"MC steps = {show_after_steps[j]}")
        for i in range(Lz):
            ax[i][j].imshow(series[show_after_steps[j]][i,:,:], cmap="gray")
    plt.show()

plot_series()