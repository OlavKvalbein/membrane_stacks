import matplotlib.pyplot as plt
import numpy as np
from lattice import Lattice

def test_energy_diff():
	L = 10
	Lz = 5
	T_J_ratio = 1.0
	Jz_J_ratio = 0.0
	lattice = Lattice(L, Lz, T_J_ratio, Jz_J_ratio)

	spin = np.ones((Lz,L,L), dtype=np.int8)
	spin[0,0,0] = -1
	lattice.spin = spin

	DeltaE = lattice.energy_diff(0,0,0,1,1)
	pass

test_energy_diff()