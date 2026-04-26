
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from matplotlib.colors import to_rgba
 
L = 16
 


GREEN = "#2E8B2E"
BROWN = "#C65A1E"
LAYER_SPACING = 1.0     # vertical spacing between layers in the plot
LAYER_THICKNESS = 0.8   # how fat each layer is; must be <= LAYER_SPACING
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
 
 
def _rects_to_boxes(rects, z, thickness):
    """Convert 2D rects to 3D box polygons (6 faces each) centered at height z."""
    z0 = z - thickness / 2
    z1 = z + thickness / 2
    # Each rect -> 6 quads (top, bottom, 4 sides). We return shape (n_rects*6, 4, 3).
    boxes = np.empty((len(rects) * 6, 4, 3), dtype=float)
    for k, (x0, y0, x1, y1) in enumerate(rects):
        b = k * 6
        # top (z1) and bottom (z0)
        boxes[b+0] = [(x0,y0,z1), (x1,y0,z1), (x1,y1,z1), (x0,y1,z1)]
        boxes[b+1] = [(x0,y0,z0), (x1,y0,z0), (x1,y1,z0), (x0,y1,z0)]
        # sides
        boxes[b+2] = [(x0,y0,z0), (x1,y0,z0), (x1,y0,z1), (x0,y0,z1)]
        boxes[b+3] = [(x0,y1,z0), (x1,y1,z0), (x1,y1,z1), (x0,y1,z1)]
        boxes[b+4] = [(x0,y0,z0), (x0,y1,z0), (x0,y1,z1), (x0,y0,z1)]
        boxes[b+5] = [(x1,y0,z0), (x1,y1,z0), (x1,y1,z1), (x1,y0,z1)]
    return boxes
 
 
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
            up_polys = _rects_to_boxes(up_rects, h, LAYER_THICKNESS)
            pc = Poly3DCollection(
                up_polys,
                facecolors=green_rgba,
                edgecolors='none' if not CELL_EDGE else (0, 0, 0, 0.2),
                linewidths=0 if not CELL_EDGE else 0.3,
            )
            ax.add_collection3d(pc)
 
        if down_rects:
            down_polys = _rects_to_boxes(down_rects, h, LAYER_THICKNESS)
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
    base = "data/lattice32_T=1.63_0.5"   # one place to set the folder
    snapshots = [
        (f"{base}/0", "10 MCS"),
        (f"{base}/1", "50 MCS"),
        (f"{base}/2", "100 MCS"),
        (f"{base}/3", "5000 MCS"),
        (f"{base}/4", "10000 MCS"),
        (f"{base}/5", "50000 MCS"),
        (f"{base}/6", "100000 MCS"),
        (f"{base}/7", "1000000 MCS"),
    ]

    # Load all lattices
    lattices = [(load_lattice(path), title) for path, title in snapshots]
    for lat, title in lattices:
        print(f"{title}: shape={lat.shape}")

    # 2 rows x 4 cols
    nrows, ncols = 2, 4
    fig = plt.figure(figsize=(5 * ncols, 4 * nrows))
    for idx, (lat, title) in enumerate(lattices):
        ax = fig.add_subplot(nrows, ncols, idx + 1, projection='3d')
        plot_stacked_layers(ax, lat, title=title)
    T = 1.63
    Jz = 0.1
    fig.suptitle(f"T = {T}, $J_z$ = {Jz}", fontsize=14, y=0.98)
    plt.tight_layout()
    plt.savefig("figures/lattice32_T=1.63_Jz=0.5.png", dpi=150, bbox_inches='tight')
    plt.show()
 