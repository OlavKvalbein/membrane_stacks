#ifndef MAIN_H
#define MAIN_H

double rand01();
int rand_i_range(int min, int max);

typedef struct
{
	int L;
	int Lz;
	double T;		// where k_B = J = 1
	double Jz;		// Jz = lambda = Jz/J
	char *spin;
} Lattice;

Lattice new_lattice(int L, int Lz, double T, double Jz);
void print_lattice(Lattice lat);
void export_lattice(Lattice lat, char* filepath);
void step(Lattice lat);

#endif