#include "main.h"

uint rng_state;

// exports 1 csv file
void timeseries(char* folderpath, Lattice* lat, SamplingData* data)
{
	run_ensemble(lat, data);

	char filepath[500];
	// take the temperature as the file name.
	snprintf(filepath, sizeof(filepath), "%s/T=%1.8f.csv",
		folderpath, lat->T);
	export_sampling_data(data, lat, filepath);
}

// exports 1 csv file for each temperature.
// Takes an initial lattice and samplingdata
void many_timeseries(char* folderpath, double T[], int length,
	Lattice* lat, SamplingData* data)
{
	printf("Starting simulation...\n");
	for (int i = 0; i < length; i++) {
		lat->T = T[i];
		timeseries(folderpath, lat, data);
		printf("\rDone with temperature %i/%i...", i+1, length);
	}
	printf("\nDone!\n");
}

int main()
{
	clock_t start_time = clock();

	// seed_rng(time(NULL));
	seed_rng(123456);
	
	// sample temperatures for each Jz = 0.1, 0.5, 1.0
	// Tmiddle is where we think T_c is
	//=======================================================
	// double Jz = 0.1;
	// char folder[] = "data/timeseries/L=32,Lz=8/J=0.1";
	// double Ts[] = {
	// 	1.75,
	// 	2.        , 2.05357143, 2.10714286, 2.16071429, 2.21428571,
	// 	2.26785714, 2.32142857, 2.375     , 2.42857143, 2.48214286,
	// 	2.53571429, 2.58928571, 2.64285714, 2.69642857, 2.75,	
	// 	3.  , 3.25, 3.5
	// };
	
	// double Jz = 0.5;
	// char folder[] = "data/timeseries/L=32,Lz=8/J=0.5";
	// double Ts[] = {
	// 	2.75,
	// 	3.0       , 3.05357143, 3.10714286, 3.16071429, 3.21428571,
	//   	3.26785714, 3.32142857, 3.375     , 3.42857143, 3.48214286,
	//    	3.53571429, 3.58928571, 3.64285714, 3.69642857, 3.75,
	// 	4.0,4.25,4.5,
	// };
	
	double Jz = 1.0;
	char folder[] = "data/timeseries/L=32,Lz=8/J=1.0";
	double Ts[] = {
		3.0,3.25,3.5,3.75,4.0,4.25,4.5,4.75,5.0
	};
	//=================================================
		
	double T = 3.0;		// this is whatever
	Lattice lat = new_lattice(T, Jz);

	SamplingData sampling_data = {
		.n_burn_in = 1e5,
		.n_samples = 100,
		.sample_period = 20, 	// same as Hoshino
		.ensemble_size = 2,
	};

	many_timeseries(folder, Ts, len(Ts), &lat, &sampling_data);

	printf("\nTotal program runtime: %.2f min.\n",
		(double)(clock() - start_time) / (CLOCKS_PER_SEC * 60));
}
