#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "main.h"

typedef struct
{
	int nr_steps;
	int burn_in_steps;	
	int sample_period;
	int nr_samples;

	bool sample_spin;

	bool sample_specific_heat;
	double* H;       // List of hamiltonian at each step
	double* H_sq;	// List of hamiltonian^2 at each step

	bool sample_interconnectivity; // delta^2. TODO
	// something for delta^2
} SamplingData;

SamplingData new_sampling_data(
	int nr_steps, int burn_in_steps, int sample_period, 
	bool sample_specific_heat, bool sample_interconnectivity
){
	int nr_samples = (nr_steps - burn_in_steps) / sample_period; // not correct

	double *H = (double*)malloc(nr_samples*sizeof(double));
	double *H_sq = (double*)malloc(nr_samples*sizeof(double));
}

// samples the lattice for the relevant variables at a given sample number
// and adds it to the totals
void sample(const Lattice* lat, int sample_nr, SamplingData* data)
{
	double hamil = energy(lat);
	data->H[sample_nr] += hamil; 
	data->H_sq[sample_nr] += hamil*hamil;
}

// runs an ensemble of lattices to some given steps and samples them underways.
void run_ensemble(Lattice* lat, int ensemble_size, SamplingData* data)
{
	for (int i = 0; i < ensemble_size; i++) {
		reset_lattice(lat);
		int sample_nr = 0;
		for (int step = 0; step < data->nr_steps; step++) {
			// are we to sample?
			if (step >= data->burn_in_steps && step % data->sample_period == 0) {
				sample(lat, sample_nr, data);
				sample_nr++;
			}
			do_step(lat);
		} // this for-loop potentially misses the last sample. 
	}
	// divide each sampled variable by ensemble size to get ensemble averages.
	for (int i = 0; i < data->nr_samples; i++) {
		data->H[i] /= ensemble_size;
		data->H_sq[i] /= ensemble_size;
	}
}

int main()
{
	srand(time(NULL));

	int L = 16;
	int Lz = 8;
	double T = 1.5;
	double Jz = 0.5;
	Lattice lat = new_lattice(L, Lz, T, Jz);

	int steps = 100;
	for (int i = 0; i < steps; i++) {
		do_step(&lat);
	}

	// char after_path[100];
	// snprintf(after_path, sizeof(after_path),
	//     "data/lattice_%isteps.txt", steps);
	// export_lattice(&lat, after_path);
}