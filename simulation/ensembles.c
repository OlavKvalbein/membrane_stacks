#include "main.h"

// allocates the space for variables sampled
void init_sampling_data(SamplingData* data)
{
	data->H = (double*)malloc(data->n_samples*sizeof(double));
	data->delta2 = (double*)malloc(data->n_samples*sizeof(double));

	for (int i = 0; i < data->n_samples; i++) {
		data->H[i] = 0;
		data->delta2[i] = 0;
	}
}

// samples the lattice for the relevant variables at a given sample number
// and adds it to the totals
void sample_lattice(const Lattice* lat, int sample_nr, SamplingData* data)
{
	data->H[sample_nr] += energy(lat); 
	data->delta2[sample_nr] += delta2(lat);
}

// runs an ensemble of lattices to some given steps and samples them underways.
void run_ensemble(Lattice* lat, SamplingData* data)
{
	init_sampling_data(data);

	#ifdef ENSEMBLE_FEEDBACK
	printf("Starting simulation. %i MC steps per ensemble member.\n", data->n_steps);
	printf("Ensemble member %i/%i done...", 0, data->ensemble_size);
	clock_t start_time = clock();
	#endif


	for (int i = 0; i < data->ensemble_size; i++) {
		reset_spin(lat);

		// burn in steps
		do_steps(lat, data->n_burn_in);

		// sampling steps
		sample_lattice(lat, 0, data);
		for (int sample_nr = 1; sample_nr < data->n_samples; sample_nr++) {
			do_steps(lat, data->sample_period);
			sample_lattice(lat, sample_nr, data);
		}
		
		#ifdef ENSEMBLE_FEEDBACK
		// progress report
		printf("\rEnsemble member %i/%i done...", i+1, data->ensemble_size);
		#endif
	}
	
	// divide each sampled variable by ensemble size to get ensemble averages.
	for (int i = 0; i < data->n_samples; i++) {
		data->H[i] /= data->ensemble_size;
		data->delta2[i] /= data->ensemble_size;
	}

	#ifdef ENSEMBLE_FEEDBACK
	printf("\nTime used: %f sec.\n",
		(double)(clock() - start_time) / CLOCKS_PER_SEC);
	#endif
}

void export_sampling_data(const SamplingData* data, const Lattice* lat, char* filepath)
{
	FILE* file = fopen(filepath, "w");

	// metadata
	fprintf(file, "# Ensemble size: %i, Sample period: %i, Burn in steps: %i, Samples: %i\n",
		data->ensemble_size, data->sample_period, data->n_burn_in, data->n_samples);
	fprintf(file, "# L = %i, Lz = %i, T = %.3f, Jz = %.4f\n",
		L, Lz, lat->T, lat->Jz);

	// headers
	fprintf(file, "step,H,delta^2\n");

	// data
	for (int i = 0; i < data->n_samples; i++) {
		int step = data->n_burn_in + i*data->sample_period;
		fprintf(file, "%i,%f,%f\n",
			step, data->H[i],data->delta2[i]);
	}

	fclose(file);
	#ifdef ENSEMBLE_FEEDBACK
	printf("Exported sampling data to %s\n", filepath);
	#endif
}