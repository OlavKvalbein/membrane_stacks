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
	Lattice lat = new_lattice(L, Lz, 1.5, Jz);

	SamplingData sampling_data = {
		.n_steps = 10'000,
		.ensemble_size = 1,
		.n_samples = 200,
		.n_burn_in = 0,
		.sample_specific_heat = true,
		.sample_delta2 = true,
	};

	// char folder[] = "data/timeseries/L=16,Lz=8";
	// double Tmin = 1.5;
	// double Tmax = 2.5;
	// double T[20];
	// for (int i = 0; i < len(T); i++)
	// 	T[i] = lerp(Tmin, Tmax, (double)i/len(T));
	
	// many_timeseries(folder, T, len(T), &lat, &sampling_data);

	// double T2[] = {2.625,2.75,3.0,3.25,3.5};
	// lower temperatures don't need to run as far to equilibriate.
	// sampling_data.n_steps = 5'000;
	// many_timeseries(folder, T2, len(T2), &lat, &sampling_data);
	run_ensemble(&lat, &sampling_data);
	export_sampling_data(&sampling_data, &lat,
		"data/timeseries/delta2.csv");

	printf("\nTotal program runtime: %.2f min.\n",
		(double)(clock() - start_time) / (CLOCKS_PER_SEC * 60));
}
