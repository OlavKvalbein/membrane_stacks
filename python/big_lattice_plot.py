
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from matplotlib.colors import to_rgba
 
L = 64
 
start_path  = "data/big_lattice_Lz_8_1m/start"
middle_path = "data/big_lattice_Lz_8_1m/middle"
end_path    = "data/big_lattice_Lz_8/end"


GREEN = "#2E8B2E"
BROWN = "#C65A1E"
LAYER_SPACING = 3.0     # vertical spacing between layers in the plot
ALPHA = 1.0             # layer opacity (1.0 = solid)
CELL_EDGE = False       # set True for cell-level gridlines (slow)
 
 
def load_lattice(path, L=L, Lz=None):
    raw = np.fromfile(path, dtype=np.int8)
    if Lz is None:
        assert raw.size % (L * L) == 0
        Lz = raw.size // (L * L)
    return (raw == ord('1')).astype(np.uint8).reshape((Lz, L, L))
 
 
def _merge_cells_to_rects(mask):
   
    rects = []
    Ly, Lx = mask.shape
    for y in range(Ly):
        row = mask[y]
        x = 0
        while x < Lx:
            if row[x]:
                x0 = x
                while x < Lx and row[x]:
                    x += 1
                rects.append((x0, y, x, y + 1))
            else:
                x += 1
    return rects
 
 
def _rects_to_polys(rects, z):

    polys = np.empty((len(rects), 4, 3), dtype=float)
    for i, (x0, y0, x1, y1) in enumerate(rects):
        polys[i, 0] = (x0, y0, z)
        polys[i, 1] = (x1, y0, z)
        polys[i, 2] = (x1, y1, z)
        polys[i, 3] = (x0, y1, z)
    return polys
 
 
def plot_stacked_layers(ax, lattice, spacing=LAYER_SPACING, title=""):
    
    Lz, Ly, Lx = lattice.shape
    green_rgba = to_rgba(GREEN, alpha=ALPHA)
    brown_rgba = to_rgba(BROWN, alpha=ALPHA)
 
    for z in range(Lz):
        layer = lattice[z]
        up_mask   = layer == 1
        down_mask = layer == 0
 
       
        up_rects   = _merge_cells_to_rects(up_mask)
        down_rects = _merge_cells_to_rects(down_mask)
 
        h = z * spacing
 
        if up_rects:
            up_polys = _rects_to_polys(up_rects, h)
            pc = Poly3DCollection(
                up_polys,
                facecolors=green_rgba,
                edgecolors='none' if not CELL_EDGE else (0, 0, 0, 0.2),
                linewidths=0 if not CELL_EDGE else 0.3,
            )
            ax.add_collection3d(pc)
 
        if down_rects:
            down_polys = _rects_to_polys(down_rects, h)
            pc = Poly3DCollection(
                down_polys,
                facecolors=brown_rgba,
                edgecolors='none' if not CELL_EDGE else (0, 0, 0, 0.2),
                linewidths=0 if not CELL_EDGE else 0.3,
            )
            ax.add_collection3d(pc)
 
    ax.set_xlim(0, Lx)
    ax.set_ylim(0, Ly)
    ax.set_zlim(0, (Lz - 1) * spacing + 1)
    # Aspect ratio: keep x, y square, let z follow the stacked height
    ax.set_box_aspect((Lx, Ly, Lx * 0.6))
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z (layer)")
    ax.set_title(title, fontsize=13)
    ax.view_init(elev=22, azim=-55)
 
 
if __name__ == "__main__":
    start  = load_lattice(start_path)
    middle = load_lattice(middle_path)
    #end    = load_lattice(end_path)
 
    print(f"shapes: start={start.shape}, middle={middle.shape}")
 
    # 1 row x 3 cols
    fig = plt.figure(figsize=(18, 6))
    ax1 = fig.add_subplot(1, 3, 1, projection='3d')
    ax2 = fig.add_subplot(1, 3, 2, projection='3d')
    #ax3 = fig.add_subplot(1, 3, 3, projection='3d')
 
    plot_stacked_layers(ax1, start,  title="0 MCS")
    plot_stacked_layers(ax2, middle, title="500000 MCS")
    #plot_stacked_layers(ax3, end,    title="10000 MCS")
 
    plt.tight_layout()
    plt.savefig("figures/lattice_stacked.png", dpi=150, bbox_inches='tight')
    plt.show()
 