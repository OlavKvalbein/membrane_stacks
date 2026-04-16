#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "main.h"

typedef struct
{
	int n_steps;
	int n_samples;
	int burn_in_steps;	
	
	bool sample_specific_heat;
	double* H;       // List of hamiltonian at each step
	double* H_sq;	// List of hamiltonian^2 at each step

	bool sample_interconnectivity; // delta^2. TODO
	// something for delta^2
} SamplingData;

// allocates the space for variables sampled
void init_sampling_data(SamplingData* data)
{	
	if (data->sample_specific_heat) {
		double *H = (double*)malloc(data->n_samples*sizeof(double));
		double *H_sq = (double*)malloc(data->n_samples*sizeof(double));
	}

	if (data->sample_interconnectivity) {
		// TODO
	}
}

// samples the lattice for the relevant variables at a given sample number
// and adds it to the totals
void sample_lattice(const Lattice* lat, int sample_nr, SamplingData* data)
{
	if (data->sample_specific_heat) {
		double hamil = energy(lat);
		data->H[sample_nr] += hamil; 
		data->H_sq[sample_nr] += hamil*hamil;
	}

	if (data->sample_interconnectivity) {
		// TODO
	}
}

// runs an ensemble of lattices to some given steps and samples them underways.
void run_ensemble(Lattice* lat, int ensemble_size, SamplingData* data)
{
	init_sampling_data(data);

	for (int i = 0; i < ensemble_size; i++) {
		reset_lattice(lat);

		// do the burn in steps
		for (int step = 0; step < data->burn_in_steps; step++)
			do_step(lat);

		// sampling
		sample_lattice(lat, 0, data);
		int sample_period = (data->n_steps - data->burn_in_steps)/data->n_samples;
		for (int sample_nr = 1; sample_nr < data->n_samples; sample_nr++) {
			for (int i = 0; i < sample_period; i++)
				do_step(lat);
			sample_lattice(lat, sample_nr, data);
		}
	}

	// divide each sampled variable by ensemble size to get ensemble averages.
	for (int i = 0; i < data->n_samples; i++) {
		data->H[i] /= ensemble_size;
		data->H_sq[i] /= ensemble_size;
	}
}

int main()
{
	srand(time(NULL));

	int L = 64;
	int Lz = 32;
	double T = 1.5;
	double Jz = 0.5;
	Lattice lat = new_lattice(L, Lz, T, Jz);

	// SamplingData sampling_data = {
	// 	.n_steps = 1000,
	// 	.n_samples = 100,
	// 	.burn_in_steps = 0,
	// 	.sample_specific_heat = true,
	// 	.sample_interconnectivity = false,
	// };

	// run_ensemble(&lat, 10, &sampling_data);

	export_lattice(&lat, "data/big_lattice_Lz_32/start");

	int steps = 500;
	for (int i = 0; i < steps; i++) {
		do_step(&lat);
	}

	export_lattice(&lat, "data/big_lattice_Lz_32/middle");

	for (int i = 0; i < steps; i++) {
		do_step(&lat);
	}

	export_lattice(&lat, "data/big_lattice_Lz_32/end");

	// char after_path[100];
	// snprintf(after_path, sizeof(after_path),
	//     "data/lattice_%isteps.txt", steps);
	// export_lattice(&lat, after_path);
}