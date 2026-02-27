import matplotlib.pyplot as plt
import numpy as np
from lattice import Lattice

class TestLattice(Lattice):
	def set_spin(self, spin):
		self.spin = spin
		self.init_nesw_sum()
		self.init_ud_sum()

def test_energy_diff():
	L = 10
	Lz = 5
	T_J_ratio = 1.0
	Jz_J_ratio = 0.0
	lattice = TestLattice(L, Lz, T_J_ratio, Jz_J_ratio)

	spin = np.ones((Lz,L,L), dtype=np.int8)
	# spin[0,0,0] = -1
	lattice.set_spin(spin)

	DeltaE = lattice.energy_diff(0,0,0,3,3)
	pass

test_energy_diff()