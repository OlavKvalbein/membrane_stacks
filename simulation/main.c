#include "main.h"

// exports 1 csv file
void timeseries(char* folderpath, Lattice* lat, SamplingData* data)
{
	run_ensemble(lat, data);

	char filepath[500];
	// take the temperature as the file name.
	snprintf(filepath, sizeof(filepath), "%s/T=%1.3f.csv",
		folderpath, lat->T);
	export_sampling_data(data, lat, filepath);
}

// exports 1 csv file for each temperature.
// Takes an initial lattice and samplingdata
void many_timeseries(char* folderpath, double* T, int length,
	Lattice* lat, SamplingData* data)
{
	for (int i = 0; i < length; i++) {
		lat->T = T[i];
		timeseries(folderpath, lat, data);
	}
}

int main()
{
	clock_t start_time = clock();
	srand(time(NULL));

	int L = 16;
	int Lz = 8;
	double Jz = 0.1;
	Lattice lat = new_lattice(L, Lz, 0.0, Jz);

	SamplingData sampling_data = {
		.n_steps = 15'000,
		.ensemble_size = 10,
		.n_samples = 200,
		.n_burn_in = 0,
		.sample_specific_heat = true,
		.sample_interconnectivity = false,
	};

	char folder[] = "data/timeseries/L=16,Lz=8";
	double T1[] = {1.5,1.625,1.75,1.875,2.0,2.125,2.25,2.375,2.5};
	
	many_timeseries(folder, T1, len(T1), &lat, &sampling_data);

	// double T2[] = {2.625,2.75,3.0,3.25,3.5};
	// lower temperatures don't need to run as far to equilibriate.
	// sampling_data.n_steps = 5'000;
	// many_timeseries(folder, T2, len(T2), &lat, &sampling_data);


	printf("\nTotal program runtime: %.2f min.\n",
		(double)(clock() - start_time) / (CLOCKS_PER_SEC * 60));
}
