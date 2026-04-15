#ifndef MAIN_H
#define MAIN_H

double rand01();
int rand_i(int min, int max);

typedef struct
{
	int L;
	int Lz;
	double T;		// corresponds to T/J
	double Jz;		// corresponds to Jz/J
	char *spin;
} Lattice;

Lattice new_lattice(int L, int Lz, double T, double Jz);
void reset_lattice(Lattice *lat);
void print_lattice(const Lattice* lat);
void export_lattice(const Lattice* lat, char* filepath);
void step(Lattice* lat);
char nesw_sum(const Lattice* lat, int z, int i, int j);
char spin(const Lattice* lat, int z, int i, int j);

double energy(const Lattice* lat);

#endif