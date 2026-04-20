#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main.h"

// allocates the space for variables sampled
void init_sampling_data(SamplingData* data)
{	
	if (data->sample_specific_heat) {
		data->H = (double*)malloc(data->n_samples*sizeof(double));
		data->H_sq = (double*)malloc(data->n_samples*sizeof(double));
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
void run_ensemble(Lattice* lat, SamplingData* data)
{
	init_sampling_data(data);

	for (int i = 0; i < data->ensemble_size; i++) {
		reset_lattice(lat);

		// burn in steps
		do_steps(lat, data->burn_in_steps);

		// sampling steps
		sample_lattice(lat, 0, data);
		int sample_period = (data->n_steps - data->burn_in_steps)/data->n_samples;
		for (int sample_nr = 1; sample_nr < data->n_samples; sample_nr++) {
			do_steps(lat, sample_period);
			sample_lattice(lat, sample_nr, data);
		}
	}

	// divide each sampled variable by ensemble size to get ensemble averages.
	for (int i = 0; i < data->n_samples; i++) {
		data->H[i] /= data->ensemble_size;
		data->H_sq[i] /= data->ensemble_size;
	}
}

void export_sampling_data(const SamplingData* data, char* filepath)
{
    FILE* file = fopen(filepath, "w");

    fprintf(file, "# Ensemble size: %i, Steps: %i, Burn in steps: %i, Samples: %i\n",
        data->ensemble_size, data->n_steps, data->burn_in_steps, data->n_samples);

    if (data->sample_specific_heat)
        fprintf(file, "H,H^2");
    if (data->sample_interconnectivity)
        ;//TODO
    putc('\n', file);

    for (int i = 0; i < data->n_samples; i++) {
        if (data->sample_specific_heat)
            fprintf(file, "%f,%f,", data->H[i], data->H_sq[i]);
        if (data->sample_interconnectivity)
            ;// TODO
        
        putc('\n', file);
    }

    printf("Exported sampling data to %s\n", filepath);
}