#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

typedef unsigned int uint;

extern uint rng_state;

static inline uint fast_rand() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static inline void seed_rng(uint seed) {
	rng_state = seed ? seed : 1;  // 0 is a bad seed for xorshift
}

double rand01();
double lerp(double a, double b, double t);
#define len(arr) (sizeof(arr)/sizeof(arr[0]))

// should both be divisible by 2
#define L 32
#define Lz 4

typedef struct
{
	double T;		// corresponds to T/J
	double Jz;		// corresponds to Jz/J
	
	char spin[Lz][L][L];
	// The boltzmann factors, i.e. the possible exchange probabilities.
	double boltzmann_table[2][9][9];
} Lattice;

typedef struct SamplingData
{
	int n_burn_in;
	int sample_period;
	int n_samples;
	int ensemble_size;
		
	double* H;		// Hamiltonian
	double* delta2;
} SamplingData;

// lattice.c
Lattice new_lattice(double T, double Jz);
void reset_spin(Lattice *lat);
void print_lattice(const Lattice* lat);
void export_lattice(const Lattice* lat, char* filepath);
void do_steps(Lattice* lat, int n);
char nesw_sum(const Lattice* lat, int z, int i, int j);

// quantities.c
double energy(const Lattice* lat);
double delta2(const Lattice* lat);

// ensembles.c
// #define ENSEMBLE_FEEDBACK
void run_ensemble(Lattice* lat, SamplingData* data);
void export_sampling_data(const SamplingData* data, const Lattice* lat, char* filepath);


#endif