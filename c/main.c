#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "main.h"

int main()
{
	srand(time(NULL));

	int L = 64;
	int Lz = 4;
	double T = 1.5;
	double Jz = 0.5;
	Lattice lat = new_lattice(L, Lz, T, Jz);

	// SamplingData sampling_data = {
	// 	.ensemble_size = 5,
	// 	.n_steps = 1000,
	// 	.n_samples = 100,
	// 	.burn_in_steps = 0,
	// 	.sample_specific_heat = true,
	// 	.sample_interconnectivity = false,
	// };

	// run_ensemble(&lat, &sampling_data);

	// export_sampling_data(&sampling_data, "data/sampling.csv");

	export_lattice(&lat, "data/big_lattice/start");

	do_steps(&lat, 100);

	export_lattice(&lat, "data/big_lattice/middle");

	do_steps(&lat, 200);

	export_lattice(&lat, "data/big_lattice/end");
}