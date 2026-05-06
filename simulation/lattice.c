#include "main.h"

void precompute_boltzmann_table(Lattice* lat) {
    for (int s = -1; s <= 1; s += 2)
    for (int dN = -8; dN <= 8; dN += 2)   // N1-N2
    for (int dU = -4; dU <= 4; dU++) {     // U1-U2
        double dE = 2.0*s*(dN + 2*s + lat->Jz*dU);
        double factor = (dE < 0) ? 1.0 : exp(-dE / lat->T);
        lat->boltzmann_table[(s+1)/2][(dN+8)/2][dU+4] = factor;
    }
}

static inline double boltzmann_factor(const Lattice* lat,
                                       char s, char N1, char N2,
                                       char U1, char U2) {
    return lat->boltzmann_table[(s+1)/2][(N1-N2+8)/2][U1-U2+4];
}

void reset_spin(Lattice *lat)
{
	// fill the spin array with 1's and -1's.
    int n = 0;
    for (int z = 0; z < Lz; z++)
    for (int i = 0; i < L; i++)
    for (int j = 0; j < L; j++)
        lat->spin[z][i][j] = (n++ < L*L*Lz/2) ? 1 : -1;

	// Fisher-Yates shuffle
    for (int n = L*L*Lz - 1; n > 0; n--) {
        int m = fast_rand() % (n + 1);

        // Convert flat indices to 3D
        int z1 = n / (L*L), i1 = (n / L) % L, j1 = n % L;
        int z2 = m / (L*L), i2 = (m / L) % L, j2 = m % L;

        char tmp = lat->spin[z1][i1][j1];
        lat->spin[z1][i1][j1] = lat->spin[z2][i2][j2];
        lat->spin[z2][i2][j2] = tmp;
    }
}

Lattice new_lattice(double T, double Jz)
{
	Lattice lat = {.T = T, .Jz = Jz};
	reset_spin(&lat);
	precompute_boltzmann_table(&lat);

	return lat;
}

// Finds 2 random neighbors in the same plane.
// Returns 0 if same spin, otherwise returns s1
char opposite_random_neighbors(const Lattice* lat,
	int* z, int* i1, int* j1, int* i2, int* j2)
	{
		// x & (y - 1) = x % y for y a power of 2. Could use this for slight speed up.
		uint r1 = fast_rand();
		*z  = r1 % Lz;           // bits from r1
		*i1 = *i2 = (r1 >> 8) % L;
		*j1 = *j2 = (r1 >> 16) % L;
		
		// choose a random offset
		int direction = (r1 >> 24) % 4; // top 8 bits
		if (direction == 0)
		*i2 = (*i2 + 1) % L;
		else if (direction == 1)
		*i2 = (*i2 - 1 + L) % L;
		else if (direction == 2)
		*j2 = (*j2 + 1) % L;
		else
		*j2 = (*j2 - 1 + L) % L;
		
		char s1 = lat->spin[*z][*i1][*j1];
		char s2 = lat->spin[*z][*i2][*j2];
		if (s1 == s2) return 0;
		return s1;
	}
	
	// spin sum of the 4 neighbors in the plane 
	char nesw_sum(const Lattice* lat, int z, int i, int j)
	{
		return lat->spin[z][(i+1)%L][j]
		+ lat->spin[z][(i-1+L)%L][j]
		+ lat->spin[z][i][(j+1)%L]
		+ lat->spin[z][i][(j-1+L)%L];
	}
	
// Finds 2 random neighbors.
// If they are opposite spin, then flips according to boltzmann factor.
void try_flip(Lattice* lat) {
	int z, i1, j1, i2, j2;
	char s = opposite_random_neighbors(lat, &z, &i1, &j1, &i2, &j2);
	if (s) { // if neighbors have opposite spin:
		char N1 = nesw_sum(lat, z, i1, j1);
		char N2 = nesw_sum(lat, z, i2, j2);
		int zup = (z+1) % Lz;
		int zdn = (z-1+Lz) % Lz;
		char U1 = lat->spin[zup][i1][j1] + lat->spin[zdn][i1][j1];
		char U2 = lat->spin[zup][i2][j2] + lat->spin[zdn][i2][j2];

		// table lookup version
		// double prob = boltzmann_factor(lat, s, N1, N2, U1, U2);
		// if (rand01() < prob) {
		// 	lat->spin[z][i1][j1] *= -1;
		// 	lat->spin[z][i2][j2] *= -1;
		// }

		// directly compute version, actually faster
		double dE = 2.0*s*(N1 - N2 + 2*s + lat->Jz*(U1 - U2));
		if (dE < 0 || rand01() < exp(-dE / lat->T)) {
			lat->spin[z][i1][j1] *= -1;
			lat->spin[z][i2][j2] *= -1;
		}
	}
}

void do_steps(Lattice* lat, int n)
{
	for (int i = 0; i < n; i++)
	for (int j = 0; j < L*L*Lz; j++)
		try_flip(lat);
}

void print_lattice(const Lattice* lat)
{
	printf("L = %i, Lz = %i\nT = %f, Jz = %f\n",
		L, Lz, lat->T, lat->Jz);

	printf("Spin:\n");
	for (int z = 0; z < Lz; z++) {
		for (int i = 0; i < L; i++) {
			for (int j = 0; j < L; j++) {
				char s = lat->spin[z][i][j] == 1 ? '1' : '0';
				putchar(s);
			}
			printf("\n");
		}
		printf("\n");
	}
}

// Exports the lattice into a text file
void export_lattice(const Lattice* lat, char* filepath)
{	
    FILE* file = fopen(filepath, "w");
	if (file == NULL)
		perror("could not write file");

	// Parameters
	// fprintf(file, "L = %i, Lz = %i\nT = %f, Jz = %f\n",
	// 	L, Lz, lat->T, lat->Jz);
	
	for (int z = 0; z < Lz; z++) {
		for (int i = 0; i < L; i++) {
			for (int j = 0; j < L; j++) {
				char c = lat->spin[z][i][j] == 1 ? '1' : '0';
				fputc(c, file);
			}
		}
	}
    fclose(file);
	printf("Exported lattice to %s\n", filepath);
}

