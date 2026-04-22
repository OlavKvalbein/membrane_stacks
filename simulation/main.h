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

typedef struct
{
	int ensemble_size;
	int n_steps;
	int n_samples;
	int n_burn_in;	
	
	bool sample_specific_heat;
	double* H;       // List of hamiltonian at each step
	double* H_sq;	// List of hamiltonian^2 at each step

	bool sample_interconnectivity; // delta^2. TODO
	// something for delta^2
} SamplingData;

Lattice new_lattice(int L, int Lz, double T, double Jz);
void reset_lattice(Lattice *lat);
void print_lattice(const Lattice* lat);
void export_lattice(const Lattice* lat, char* filepath);
void do_steps(Lattice* lat, int n);
char nesw_sum(const Lattice* lat, int z, int i, int j);
char spin(const Lattice* lat, int z, int i, int j);

double energy(const Lattice* lat);

void run_ensemble(Lattice* lat, SamplingData* data);
void export_sampling_data(const SamplingData* data, char* filepath);


#endif