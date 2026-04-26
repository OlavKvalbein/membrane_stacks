#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "main.h"

int main()
{
	srand(time(NULL));

	int L = 32;
	int Lz = 8;
	double T = 1.63;
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

	export_lattice(&lat, "data/lattice32_T=1.63_0.5/0");
	do_steps(&lat, 10);
	export_lattice(&lat, "data/lattice32_T=1.63_0.5/1");
	do_steps(&lat, 50);
	export_lattice(&lat, "data/lattice32_T=1.63_0.5/2");
	do_steps(&lat, 100);
	export_lattice(&lat, "data/lattice32_T=1.63_0.5/3");
	do_steps(&lat, 5000);
	export_lattice(&lat, "data/lattice32_T=1.63_0.5/4");
	do_steps(&lat, 10000);
	export_lattice(&lat, "data/lattice32_T=1.63_0.5/5");
	do_steps(&lat, 50000);
	export_lattice(&lat, "data/lattice32_T=1.63_0.5/6");
	do_steps(&lat, 100000);
	export_lattice(&lat, "data/lattice32_T=1.63_0.5/7");
	do_steps(&lat, 1000000);
	export_lattice(&lat, "data/lattice32_T=1.63_0.5/8");
}