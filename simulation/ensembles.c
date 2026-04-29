#include "main.h"

// allocates the space for variables sampled
void init_sampling_data(SamplingData* data)
{
	data->sample_period = (data->n_steps - data->n_burn_in)/data->n_samples;

	if (data->sample_specific_heat) {
		data->H = (double*)malloc(data->n_samples*sizeof(double));
		data->H2 = (double*)malloc(data->n_samples*sizeof(double));
		for (int i = 0; i < data->n_samples; i++) {
			data->H[i] = 0;
			data->H2[i] = 0;
		}
	}

	if (data->sample_delta2) {
		data->delta2 = (double*)malloc(data->n_samples*sizeof(double));
		for (int i = 0; i < data->n_samples; i++) {
			data->delta2[i] = 0;
		}
	}
}

// samples the lattice for the relevant variables at a given sample number
// and adds it to the totals
void sample_lattice(const Lattice* lat, int sample_nr, SamplingData* data)
{
	if (data->sample_specific_heat) {
		double hamil = energy(lat);
		data->H[sample_nr] += hamil; 
		data->H2[sample_nr] += hamil*hamil;
	}

	if (data->sample_delta2) {
		data->delta2[sample_nr] += delta2(lat);
	}
}

// runs an ensemble of lattices to some given steps and samples them underways.
void run_ensemble(Lattice* lat, SamplingData* data)
{
	init_sampling_data(data);

	printf("Starting simulation. %i MC steps per ensemble member.\n", data->n_steps);
	printf("Ensemble member %i/%i done...", 0, data->ensemble_size);

	clock_t start_time = clock();

	for (int i = 0; i < data->ensemble_size; i++) {
		reset_lattice(lat);

		// burn in steps
		do_steps(lat, data->n_burn_in);

		// sampling steps
		sample_lattice(lat, 0, data);
		for (int sample_nr = 1; sample_nr < data->n_samples; sample_nr++) {
			do_steps(lat, data->sample_period);
			sample_lattice(lat, sample_nr, data);
		}
		
		// progress report
		printf("\rEnsemble member %i/%i done...", i+1, data->ensemble_size);
	}
	
	// divide each sampled variable by ensemble size to get ensemble averages.
	for (int i = 0; i < data->n_samples; i++) {
		data->H[i] /= data->ensemble_size;
		data->H2[i] /= data->ensemble_size;
		data->delta2[i] /= data->ensemble_size;
	}

	printf("\nTime used: %f sec.\n",
		(double)(clock() - start_time) / CLOCKS_PER_SEC);
}

void export_sampling_data(const SamplingData* data, const Lattice* lat, char* filepath)
{
    FILE* file = fopen(filepath, "w");

	// metadata
    fprintf(file, "# Ensemble size: %i, Steps: %i, Burn in steps: %i, Samples: %i\n",
        data->ensemble_size, data->n_steps, data->n_burn_in, data->n_samples);
	fprintf(file, "# L = %i, Lz = %i, T = %.3f, Jz = %.4f\n",
		lat->L, lat->Lz, lat->T, lat->Jz);

	// headers
	fprintf(file, "step,");
    if (data->sample_specific_heat)
        fprintf(file, "H,H^2,");
    if (data->sample_delta2)
        fprintf(file, "delta^2,");
    putc('\n', file);

	// data
    for (int i = 0; i < data->n_samples; i++) {
		int step = data->n_burn_in + i*data->sample_period;
		fprintf(file, "%i,", step);

        if (data->sample_specific_heat)
            fprintf(file, "%f,%f,", data->H[i], data->H2[i]);

        if (data->sample_delta2)
            fprintf(file, "%f,", data->delta2[i]);
        
        putc('\n', file);
    }

	fclose(file);
    printf("Exported sampling data to %s\n", filepath);
}