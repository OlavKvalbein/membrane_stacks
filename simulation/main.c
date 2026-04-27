#include "main.h"

int main()
{
	srand(time(NULL));

	int L = 8;
	int Lz = 4;
	double T = 1.0;
	double Jz = 0.5;
	Lattice lat = new_lattice(L, Lz, T, Jz);

	SamplingData sampling_data = {
		.ensemble_size = 5,
		.n_steps = 10'000,
		.n_samples = 100,
		.n_burn_in = 0,
		.sample_specific_heat = true,
		.sample_interconnectivity = false,
	};

	double Ts[] = {1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.50};
	char filepath[200];
	for (int i = 0; i < sizeof(Ts)/sizeof(double); i++) {
		lat.T = Ts[i];
		run_ensemble(&lat, &sampling_data);
		snprintf(filepath, sizeof(filepath), "data/energy_timeseries/T=%1.3f.csv", Ts[i]);
		export_sampling_data(&sampling_data, filepath);
	}


	// export_lattice(&lat, "data/lattice/0");
	// do_steps(&lat, 10);
	// export_lattice(&lat, "data/lattice/1");
	// do_steps(&lat, 50);
	// export_lattice(&lat, "data/lattice/2");
	// do_steps(&lat, 100);
	// export_lattice(&lat, "data/lattice/3");
	// do_steps(&lat, 500);
	// export_lattice(&lat, "data/lattice/4");
	// do_steps(&lat, 1000);
	// export_lattice(&lat, "data/lattice/5");
	// do_steps(&lat, 5000);
	// export_lattice(&lat, "data/lattice/6");
	// do_steps(&lat, 10'000);
	// export_lattice(&lat, "data/lattice/7");
	// do_steps(&lat, 50'000);
	// export_lattice(&lat, "data/lattice/8");
}
