import numpy as np
import matplotlib.pyplot as plt
from lattice import lattice_series

def plot_series():
    L = 20
    Lz = 3
    T_J_ratio = 0.1
    Jz_J_ratio = 0.1

    show_after_steps = [0, 10, 100]
    MC_steps = max(show_after_steps)

    series = lattice_series(L, Lz, T_J_ratio, Jz_J_ratio, MC_steps)
    fig, ax = plt.subplots(nrows=Lz, ncols=len(show_after_steps))
    for j in range(len(show_after_steps)):
        for i in range(Lz):
            ax[i][j].imshow(series[show_after_steps[j]][i,:,:], cmap="gray")
            ax[i][j].set_title(f"MC steps = {show_after_steps[j]}")
    plt.show()

plot_series()