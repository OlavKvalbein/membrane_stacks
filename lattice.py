import numpy as np

class Lattice():
    MAX_TRIES_SELECTING = 1000

    def __init__(self, L, Lz, T_J_ratio, Jz_J_ratio):
        self.L = L
        self.Lz = Lz
        self.shape = (Lz, L, L)
        self.J_T_ratio = 1/T_J_ratio
        self.Jz_J_ratio = Jz_J_ratio

        self.rng = np.random.default_rng()
        self.init_spin()
        self.init_nesw_sum()
        self.init_ud_sum()

    def init_spin(self):
        self.spin = self.rng.choice([np.int8(-1),np.int8(1)], size=self.shape)
    
    def init_nesw_sum(self):
        self.nesw_sum = np.zeros(self.shape, dtype=np.int8)
        for z in range(self.Lz):
            for x in range(self.L):
                for y in range(self.L):
                    n = self.spin[z,y+1,x] if y < self.L-1 else 0
                    e = self.spin[z,y,x+1] if x < self.L-1 else 0
                    s = self.spin[z,y-1,x] if y > 0 else 0
                    w = self.spin[z,y,x-1] if x > 0 else 0
                    self.nesw_sum[z,y,x] = n + e + s + w

    def init_ud_sum(self):
        self.ud_sum = np.zeros(self.shape, dtype=np.int8)
        for z in range(self.Lz):
            for x in range(self.L):
                for y in range(self.L):
                    u = self.spin[z+1,y,x] if z < self.Lz-1 else 0
                    d = self.spin[z-1,y,x] if z > 0 else 0
                    self.ud_sum[z,y,x] = u + d

    # update grid by one MC_step
    def MC_step(self):
        for _ in range(self.L**2 * self.Lz):
            self.step()

    def step(self):
        z, x1, y1, x2, y2 = self.random_opposite_neighbors()
        deltaE = self.energy_diff(z, x1, y1, x2, y2) # acually DeltaE/T
        if deltaE < 0:
            self.exchange_spins(z, x1, y1, x2, y2)
        else:
            exchange_probability = np.exp(-deltaE)
            if self.rng.random() < exchange_probability:
                self.exchange_spins(z, x1, y1, x2, y2)

    def random_opposite_neighbors(self): # in the same xy-plane
        for _ in range(self.MAX_TRIES_SELECTING):
            z = self.rng.integers(self.Lz)
            x1, y1 = self.rng.integers(0, self.L, 2)
            x2, y2 = self.random_neighbor(x1, y1)
            if self.spin[z,y1,x1] != self.spin[z,y2,x2]:
                return z, x1, y1, x2, y2

    def random_neighbor(self, x1, y1):
        x2 = x1
        y2 = y1
        if 0 < x1 < self.L - 1:
            if 0 < y1 < self.L - 1:
                choice = self.rng.integers(4)
                if choice == 0: x2 -= 1
                elif choice == 1: x2 += 1
                elif choice == 2: y2 -= 1
                elif choice == 3: y2 += 1
            else:
                choice = self.rng.integers(3)
                if choice == 0: x2 -= 1
                elif choice == 1: x2 += 1
                elif choice == 2:
                    y2 = 1 if y1 == 0 else self.L - 2
        else:
            if 0 < y1 < self.L - 1:
                choice = self.rng.integers(3)
                if choice == 0:
                    x2 = 1 if x1 == 0 else self.L - 2
                elif choice == 1: y2 -= 1
                elif choice == 2: y2 += 1
            else:
                choice = self.rng.integers(2)
                if choice == 0:
                    x2 = 1 if x1 == 0 else self.L - 2
                if choice == 1:
                    y2 = 1 if y1 == 0 else self.L - 2
        return x2, y2

    def energy_diff(self, z, x1, y1, x2, y2):
        s1 = self.spin[z,y1,x1]
        N1 = self.nesw_sum[z,y1,x1]
        N2 = self.nesw_sum[z,y2,x2]
        M1 = self.ud_sum[z,y1,x1]
        M2 = self.ud_sum[z,y2,x2]
        return -self.J_T_ratio*2*s1*(N2-N1-2*s1+self.Jz_J_ratio*(M2-M1))

    def exchange_spins(self, z, x1, y1, x2, y2):
        # update neighbor sums
        self.nesw_sum[z,y1,x1] += 2*self.spin[z,y1,x1]
        self.nesw_sum[z,y2,x2] -= 2*self.spin[z,y1,x1]

        self.spin[z,y1,x2] *= -1
        self.spin[z,y2,x2] *= -1


def lattice_series(L, Lz, T_J_ratio, Jz_J_ratio, MC_steps):
    lattice = Lattice(L, Lz, T_J_ratio, Jz_J_ratio)
    series = [lattice.spin.copy()]

    for i in range(MC_steps):
        print(f"\rGenerating lattice series... MC step {i+1}/{MC_steps}", end="", flush=True)
        lattice.MC_step()
        series.append(lattice.spin.copy())
    return series

if __name__ == "__main__":
    series = lattice_series(4, 3, 2.0, 0.1, 100)
    print("all good!")