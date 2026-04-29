#include "main.h"

void timeseries(char* folderpath, Lattice* lat, SamplingData* data)
{
	run_ensemble(lat, data);

	char filepath[500];
	// take the temperature as the file name.
	snprintf(filepath, sizeof(filepath), "%s/T=%1.3f.csv",
		folderpath, lat->T);
	export_sampling_data(data, lat, filepath);
}

int main()
{
	srand(time(NULL));

	int L = 8;
	int Lz = 4;
	double T = 2.0;
	double Jz = 0.5;
	Lattice lat = new_lattice(L, Lz, T, Jz);

	SamplingData sampling_data = {
		.ensemble_size = 50,
		.n_steps = 15'000,
		.n_samples = 100,
		.n_burn_in = 0,
		.sample_specific_heat = true,
		.sample_interconnectivity = false,
	};

	timeseries("data/energy_timeseries", &lat, &sampling_data);
	lat.T = 3.0;
	sampling_data.ensemble_size = 100;
	sampling_data.n_steps = 5000;
	timeseries("data/energy_timeseries", &lat, &sampling_data);

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
