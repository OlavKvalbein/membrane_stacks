#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "main.h"

void set_spin(Lattice* lat, int z, int i, int j, char val)
{
	lat->spin[z*(lat->L+2)*(lat->L+2) + i*(lat->L+2) + j] = val; 
}

char spin(const Lattice* lat, int z, int i, int j)
{
	return lat->spin[z*(lat->L+2)*(lat->L+2) + i*(lat->L+2) + j];
}

#define fill(lat, z0, z1, i0, i1, j0, j1, expr) \
({												\
	for (int z = (z0); z < (z1); z++)			\
		for (int i = (i0); i < (i1); i++)		\
			for (int j = (j0); j < (j1); j++)	\
				set_spin(lat, z, i, j, (expr));	\
})

void reset_lattice(Lattice *lat)
{
	fill(lat, 1, lat->Lz+1, 1, lat->L+1, 1, lat->L+1, rand_i(0,1)*2 - 1);
}

Lattice new_lattice(int L, int Lz, double T, double Jz)
{
	char* spin = (char*)malloc((Lz+2)*(L+2)*(L+2)*sizeof(char));
	Lattice lat = {L, Lz, T, Jz, spin};

	fill(&lat, 0, Lz+2, 0, L+2, 0, L+2, 0);
	fill(&lat, 1, Lz+1, 1, L+1, 1, L+1, rand_i(0,1)*2 - 1);

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
	return spin(lat, z, i+1, j) + spin(lat, z, i-1, j)
		+ spin(lat, z, i, j+1) + spin(lat, z, i, j-1);
}

// Gives the energy change from flipping (z, i1, j1) and (z, i2, j2)
double energy_diff(const Lattice* lat, int z, int i1, int j1, int i2, int j2)
{
	char s = spin(lat, z, i1, j1);
	char N1 = nesw_sum(lat, z, i1, j1);
	char N2 = nesw_sum(lat, z, i2, j2);
	char U1 = spin(lat, z+1, i1, j1) + spin(lat, z-1, i1, j1);
	char U2 = spin(lat, z+1, i2, j2) + spin(lat, z-1, i2, j2);

	return 2*s*(N1 - N2 + 2*s + lat->Jz*(U1 - U2));
}

// Finds 2 random neighbors in the same plane.
// If opposite spin neighbors are found, then returns true
bool opposite_random_neighbors(const Lattice* lat, int* z, int* i1, int* j1, int* i2, int* j2)
{
	// (z, i1, j1) is not on the 0-spin boundary, (z, i2, j2) is maybe.
	*z = rand_i(1, lat->Lz + 1);
	*i1 = *i2 = rand_i(1, lat->L + 1);
	*j1 = *j2 = rand_i(1, lat->L + 1);

	int offset = rand_i(0,1)*2 - 1;
	if (rand01() < 0.5)
		*i2 += offset;
	else
		*j2 += offset;

	// Only output if spins are opposite.
	// This means points just inside the boundary are less likely to exchange, which makes sense.
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
	for (int i = 0; i < lat->Lz * lat->L * lat->L; i++)
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
	for (int z = 1; z < lat->Lz+1; z++) {
		for (int i = 1; i < lat->L+1; i++) {
			for (int j = 1; j < lat->L+1; j++) {
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
	
	// Spin (excluding 0-spin boundary)
	for (int z = 1; z < lat->Lz+1; z++) {
		for (int i = 1; i < lat->L+1; i++) {
			for (int j = 1; j < lat->L+1; j++) {
				char c = spin(lat, z, i, j) == 1 ? '1' : '0';
				fputc(c, file);
			}
		}
	}
    fclose(file);
	printf("Exported lattice to %s\n", filepath);
}