import numpy as np

class Lattice():
    MAX_TRIES_SELECTING = 1000

    def __init__(self, L, Lz, T_J_ratio, Jz_J_ratio):
        self.L = L
        self.Lz = Lz
        self.J_T_ratio = 1/T_J_ratio
        self.Jz_J_ratio = Jz_J_ratio

        self.rng = np.random.default_rng()
        self.spin = self.init_spin()

    def init_spin(self):
        # (Lz+2,L+2,L+2) array of random spin in the interior and boundary of 0-spin
        spin = self.rng.choice([-1,1], size=(self.Lz+2, self.L+2, self.L+2))
        spin[0,:,:] = spin[-1,:,:] = 0
        spin[:,0,:] = spin[:,-1,:] = 0
        spin[:,:,0] = spin[:,:,-1] = 0
        return spin

    # update grid by one MC_step
    def MC_step(self):
        for _ in range(self.L**2 * self.Lz):
            self.step()

    # computes DeltaE
    def step(self):
        x1, y1, x2, y2, z = self.random_opposite_neighbors()
        deltaE = self.energy_diff(x1, y1, x2, y2, z) # acually DeltaE/T
        if deltaE < 0:
            self.exchange_spins(x1, y1, x2, y2, z)
        else:
            exchange_probability = np.exp(-deltaE)
            if self.rng.random() < exchange_probability:
                self.exchange_spins(x1, y1, x2, y2, z)

    def random_opposite_neighbors(self):
        for _ in range(self.MAX_TRIES_SELECTING):
            # generate random point not on the 0-spin boundary
            z = self.rng.integers(1,self.Lz+1)
            x1, y1 = self.rng.integers(1, self.L+1, 2)

            # generate neighbor possibly on the boundary
            offset = self.rng.choice([-1,1])
            if self.rng.random() < 0.5:
                x2 = x1 + offset
                y2 = y1
            else:
                x2 = x1
                y2 = y1 + offset

            if self.spin[z,y1,x1] == -self.spin[z,y2,x2]:
                return x1, y1, x2, y2, z
        print("could not find random opposite neighbors")

    def energy_diff(self, x1, y1, x2, y2, z):
        s1 = self.spin[z,y1,x1]
        N1 = self.nesw_sum(x1, y1, z)
        N2 = self.nesw_sum(x2, y2, z)
        U1 = self.spin[z+1,y1,x1] + self.spin[z-1,y1,x1]
        U2 = self.spin[z+1,y2,x2] + self.spin[z-1,y2,x2]
        return self.J_T_ratio*2*s1*(N1 - N2 + 2*s1 + self.Jz_J_ratio*(U1 - U2))
                
    def nesw_sum(self, x, y, z):
        return self.spin[z,y,x+1] + self.spin[z,y,x-1] + self.spin[z,y+1,x] + self.spin[z,y-1,x]

    def exchange_spins(self, x1, y1, x2, y2, z):
        self.spin[z,y1,x1] *= -1
        self.spin[z,y2,x2] *= -1
    
    def get_spin(self):
        return self.spin[1:-1,1:-1,1:-1].copy()

def lattice_series(L, Lz, T_J_ratio, Jz_J_ratio, MC_steps):
    lattice = Lattice(L, Lz, T_J_ratio, Jz_J_ratio)
    series = [lattice.get_spin()]

    for i in range(MC_steps):
        print(f"\rGenerating lattice series... MC step {i+1}/{MC_steps}", end="", flush=True)
        lattice.MC_step()
        series.append(lattice.get_spin())
    return series

if __name__ == "__main__":
    series = lattice_series(4, 3, 2.0, 0.1, 100)
    print("all good!")