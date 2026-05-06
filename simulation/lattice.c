#include "main.h"

void set_spin(Lattice* lat, int z, int i, int j, char val)
{
	lat->spin[z*lat->L*lat->L + i*lat->L + j] = val; 
}

char spin(const Lattice* lat, int z, int i, int j)
{
	return lat->spin[z*lat->L*lat->L + i*lat->L + j];
}

void reset_lattice(Lattice *lat)
{
	// fill the spin array with 1's and -1's.
	int half = lat->len / 2;
	for (int i = 0; i < half; i++) {
		lat->spin[i] = 1;
		lat->spin[lat->len - 1 - i] = -1;
	}

	// Fisher-Yates shuffle
	for (int i = lat->len - 1; i > 0; i--) {
		int j = rand_i(0, i);

		swap(lat->spin, i, j);
	}
}

Lattice new_lattice(int L, int Lz, double T, double Jz)
{
	char* spin = (char*)malloc(Lz*L*L*sizeof(char));
	Lattice lat = {L, Lz, T, Jz, spin, L*L*Lz};

	reset_lattice(&lat);

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

// Gives the energy change from flipping (z, i1, j1) and (z, i2, j2)
double energy_diff(const Lattice* lat, int z, int i1, int j1, int i2, int j2)
{
	char s = spin(lat, z, i1, j1);
	char N1 = nesw_sum(lat, z, i1, j1);
	char N2 = nesw_sum(lat, z, i2, j2);
	char U1 = spin(lat, (z+1)%lat->Lz, i1, j1)
		+ spin(lat, (z-1+lat->Lz)%lat->Lz, i1, j1);
	char U2 = spin(lat, (z+1)%lat->Lz, i2, j2)
		+ spin(lat, (z-1+lat->Lz)%lat->Lz, i2, j2);

	return 2*s*(N1 - N2 + 2*s + lat->Jz*(U1 - U2));
}

// Finds 2 random neighbors in the same plane.
// If opposite spin neighbors are found, then returns true
bool opposite_random_neighbors(const Lattice* lat, int* z, int* i1, int* j1, int* i2, int* j2)
{
	*z = rand_i(0, lat->Lz);
	*i1 = rand_i(0, lat->L);
	*j1 = rand_i(0, lat->L);

	// choose a random offset
	int direction = rand() % 4;
	if (direction == 0)
		*i2 = (*i1 + 1) % lat->L;
	else if (direction == 1)
		*i2 = (*i1 - 1 + lat->L) % lat->L;
	else if (direction == 2)
		*j2 = (*j1 + 1) % lat->L;
	else
		*j2 = (*j1 - 1 + lat->L) % lat->L;

	// Only output if spins are opposite.
	if (spin(lat, *z, *i1, *j1) == -spin(lat, *z, *i2, *j2))
		return true;
	else
		return false;
}

// Finds 2 random neighbors. If they are opposite spin, then flips according to deltaE.
void try_flip(Lattice* lat)
{
	int z, i1, j1, i2, j2;
	// if random neighbors are opposite:
	if (opposite_random_neighbors(lat, &z, &i1, &j1, &i2, &j2)) {
		double deltaE = energy_diff(lat, z, i1, j1, i2, j2);
		if (deltaE < 0) {
			exchange_spin(lat, z, i1, j1, i2, j2);
		}
		else {
			double exchange_prob = exp(-deltaE / lat->T);
			if (rand01() < exchange_prob)
				exchange_spin(lat, z, i1, j1, i2, j2);
		}
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