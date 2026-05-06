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

void set_spin(Lattice* lat, int z, int i, int j, char val)
{
	lat->spin[z*lat->L*lat->L + i*lat->L + j] = val; 
}

void reset_spin(Lattice *lat)
{
	// fill the spin array with 1's and -1's.
	int half = lat->len / 2;
	for (int i = 0; i < half; i++) {
		lat->spin[i] = 1;
		lat->spin[lat->len - 1 - i] = -1;
	}

	// Fisher-Yates shuffle
	for (int i = lat->len - 1; i > 0; i--) {
		int j = fast_rand() % (i + 1);

		swap(lat->spin, i, j);
	}
}

Lattice new_lattice(int L, int Lz, double T, double Jz)
{
	char* spin = (char*)malloc(Lz*L*L*sizeof(char));
	Lattice lat = {L, Lz, T, Jz, spin, .len=L*L*Lz};
	precompute_boltzmann_table(&lat);

	reset_spin(&lat);
	return lat;
}

// Assumes spins at (z, i1, j1) and (z, i2, j2) are opposite
void exchange_spin(Lattice* lat, int z, int i1, int j1, int i2, int j2)
{
	char s = spin(lat, z, i1, j1);
	set_spin(lat, z, i1, j1, -s);
	set_spin(lat, z, i2, j2, s);
}

// spin sum of the 4 neighbors in the plane 
char nesw_sum(const Lattice* lat, int z, int i, int j)
{
	return spin(lat, z, (i+1)%lat->L, j)
		+ spin(lat, z, (i-1+lat->L)%lat->L, j)
		+ spin(lat, z, i, (j+1)%lat->L)
		+ spin(lat, z, i, (j-1+lat->L)%lat->L);
}

// Finds 2 random neighbors in the same plane.
// Returns 0 if same spin, otherwise returns s1
char opposite_random_neighbors(const Lattice* lat,
	int* z, int* i1, int* j1, int* i2, int* j2)
{
	// x & (y - 1) = x % y for y a power of 2. Could use this for slight speed up.
	uint r1 = fast_rand();
    *z  = r1 % lat->Lz;           // bits from r1
    *i1 = *i2 = (r1 >> 8) % lat->L;
    *j1 = *j2 = (r1 >> 16) % lat->L;

	// choose a random offset
    int direction = (r1 >> 24) % 4; // top 8 bits
	if (direction == 0)
		*i2 = (*i2 + 1) % lat->L;
	else if (direction == 1)
		*i2 = (*i2 - 1 + lat->L) % lat->L;
	else if (direction == 2)
		*j2 = (*j2 + 1) % lat->L;
	else
		*j2 = (*j2 - 1 + lat->L) % lat->L;

	char s1 = spin(lat, *z, *i1, *j1);
	char s2 = spin(lat, *z, *i2, *j2);
	if (s1 == s2) return 0;
	return s1;
}

// Finds 2 random neighbors.
// If they are opposite spin, then flips according to boltzmann factor.
void try_flip(Lattice* lat) {
    int z, i1, j1, i2, j2;
    char s = opposite_random_neighbors(lat, &z, &i1, &j1, &i2, &j2);
    if (s) {
        char N1 = nesw_sum(lat, z, i1, j1);
        char N2 = nesw_sum(lat, z, i2, j2);
        int zup = (z+1) % lat->Lz;
        int zdn = (z-1+lat->Lz) % lat->Lz;
        char U1 = spin(lat, zup, i1, j1) + spin(lat, zdn, i1, j1);
        char U2 = spin(lat, zup, i2, j2) + spin(lat, zdn, i2, j2);

		// table lookup version
        // double prob = boltzmann_factor(lat, s, N1, N2, U1, U2);
        // if (rand01() < prob)
        //     exchange_spin(lat, z, i1, j1, i2, j2);

		// directly compute version, actually faster
		double dE = 2.0*s*(N1 - N2 + 2*s + lat->Jz*(U1 - U2));
		if (dE < 0 || rand01() < exp(-dE / lat->T))
    		exchange_spin(lat, z, i1, j1, i2, j2);
    }
}

// Takes one Monte-Carlo step.
void do_step(Lattice* lat)
{
	for (int i = 0; i < lat->len; i++)
		try_flip(lat);
}

void do_steps(Lattice* lat, int n)
{
	for (int i = 0; i < n; i++)
		do_step(lat);
}

void print_lattice(const Lattice* lat)
{
	printf("L = %i, Lz = %i\nT = %f, Jz = %f\n",
		lat->L, lat->Lz, lat->T, lat->Jz);

	printf("Spin:\n");
	for (int z = 0; z < lat->Lz; z++) {
		for (int i = 0; i < lat->L; i++) {
			for (int j = 0; j < lat->L; j++) {
				char s = spin(lat, z, i, j) == 1 ? '1' : '0';
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
	// 	lat->L, lat->Lz, lat->T, lat->Jz);
	
	for (int z = 0; z < lat->Lz; z++) {
		for (int i = 0; i < lat->L; i++) {
			for (int j = 0; j < lat->L; j++) {
				char c = spin(lat, z, i, j) == 1 ? '1' : '0';
				fputc(c, file);
			}
		}
	}
    fclose(file);
	printf("Exported lattice to %s\n", filepath);
}

