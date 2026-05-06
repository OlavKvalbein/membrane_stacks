#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

typedef unsigned int uint;

static uint rng_state = 12345;

static inline uint fast_rand() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}
double rand01();
double lerp(double a, double b, double t);
#define len(arr) (sizeof(arr)/sizeof(arr[0]))
#define swap(arr, i, j) ({		\
	int temp = (arr)[i];		\
	(arr)[i] = (arr)[j];		\
	(arr)[j] = temp;			\
})

typedef struct Lattice
{
	int L;
	int Lz;
	double T;		// corresponds to T/J
	double Jz;		// corresponds to Jz/J
	
	char *spin;
	int len;
	// The boltzmann factors, i.e. the possible exchange probabilities.
	double boltzmann_table[2][9][9];
} Lattice;

typedef struct SamplingData
{
	int ensemble_size;
	int n_steps;
	int n_samples;
	int n_burn_in;

	int sample_period;
	
	bool sample_specific_heat;
	double* H;		// List of hamiltonian at each step
	double* H2;		// List of hamiltonian^2 at each step

	bool sample_delta2; // delta^2.
	double* delta2;
} SamplingData;

Lattice new_lattice(int L, int Lz, double T, double Jz);
void reset_spin(Lattice *lat);
void print_lattice(const Lattice* lat);
void export_lattice(const Lattice* lat, char* filepath);
void do_steps(Lattice* lat, int n);
char nesw_sum(const Lattice* lat, int z, int i, int j);
static inline char spin(const Lattice* lat, int z, int i, int j)
{
    return lat->spin[z * lat->L * lat->L + i * lat->L + j];
}

double energy(const Lattice* lat);
double delta2(const Lattice* lat);

void run_ensemble(Lattice* lat, SamplingData* data);
void export_sampling_data(const SamplingData* data, const Lattice* lat, char* filepath);


#endif