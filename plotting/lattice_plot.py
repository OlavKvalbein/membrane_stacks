import matplotlib.pyplot as plt
import numpy as np

def load_spin(filepath, L, Lz):
    with open(filepath) as f:
        string = f.read()
    spins = [int(c) for c in string]
    return np.array(spins).reshape((Lz, L, L))
    

def plot_timeseries(folderpath, n_lattices, L, Lz):
    lattices = [load_spin(folderpath + f"/{i}", L, Lz)
                for i in range(n_lattices)]

    fig, ax = plt.subplots(nrows=Lz, ncols=n_lattices)
    for i in range(n_lattices):
        for z in range(Lz):
            ax[z][i].imshow(lattices[i][z,:,:], cmap="gray")
    plt.show()

def plot_lattice(filepath, L, Lz):
    lattice = load_spin(filepath, L, Lz)
    fig, ax = plt.subplots(nrows=Lz)
    for z in range(Lz):
        ax[z].imshow(lattice[z,:,:], cmap="gray")
    plt.show()

def fancy_plot(filepath, L, Lz):
    spin = load_spin(filepath, L, Lz)
    up_color, down_color = '#ef476f', '#118ab2'

    fig, ax = plt.subplots(subplot_kw={'projection': '3d'})
    # 3. Plot each 2D slice as a flat layer in 3D space
    for z in range(Lz):
        # Create vertices for the grid cells (M x N grid requires M+1 x N+1 vertices)
        X, Y = np.meshgrid(np.arange(L + 1), np.arange(L + 1))
        Z = np.full_like(X, z)
        
        facecolors = np.empty((L, L), dtype=object)
        for r in range(L):
            for c in range(L):
                facecolors[r, c] = up_color if spin[z, r, c] == 1 else down_color
                
        # Draw the flat layer
        ax.plot_surface(X, Y, Z, facecolors=facecolors, shade=False)

        border_x = [0, L, L, 0, 0]
        border_y = [0, 0, L, L, 0]
        border_z = [z,z,z,z,z]
        
        # Draw a subtle dark gray line around the layer
        ax.plot(border_x, border_y, border_z, color='#2b2d42', linewidth=1.5, zorder=z+3)

    ax.set_axis_off()
    ax.set_proj_type("ortho")
    # ax.set_box_aspect((1, 1, 1.5))
    ax.view_init(elev=25, azim=-45)

    plt.savefig(filepath+".png", bbox_inches='tight')


L = 16
Lz = 4
fancy_plot("data/lattices/test2", L, Lz)