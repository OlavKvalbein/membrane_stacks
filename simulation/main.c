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

	seed_rng(time(NULL));
	// seed_rng(123456);
	
	#define Jz_OPTION 2		// 1, 2 or 3
	// sample temperatures for each Jz = 0.1, 0.5, 1.0
	// Tmiddle is where we think T_c is
	//=======================================================
	#if Jz_OPTION == 1
	double Jz = 0.1;
	char folder[] = "data/timeseries/L=32,Lz=8/J=0.1";
	// double Ts[] = {
	// 	1.5       , 1.60526316, 1.71052632, 1.81578947, 1.92105263,
    //    	2.02631579, 2.13157895, 2.23684211, 2.34210526, 2.44736842,
    //    	2.55263158, 2.65789474, 2.76315789, 2.86842105, 2.97368421,
    //    	3.07894737, 3.18421053, 3.28947368, 3.39473684, 3.5
	// };
	#elif Jz_OPTION == 2
	double Jz = 0.5;
	char folder[] = "data/timeseries/L=32,Lz=8/J=0.5";
	// double Ts[] = {
	// 	2.5       , 2.60526316, 2.71052632, 2.81578947, 2.92105263,
    //    	3.02631579, 3.13157895, 3.23684211, 3.34210526, 3.44736842,
    //    	3.55263158, 3.65789474, 3.76315789, 3.86842105, 3.97368421,
    //    	4.07894737, 4.18421053, 4.28947368, 4.39473684, 4.5
	// };
	#elif Jz_OPTION == 3
	double Jz = 1.0;
	char folder[] = "data/timeseries/L=32,Lz=8/J=1.0";
	// double Ts[] = {
	// 	3.        , 3.10526316, 3.21052632, 3.31578947, 3.42105263,
    // 	3.52631579, 3.63157895, 3.73684211, 3.84210526, 3.94736842,
    // 	4.05263158, 4.15789474, 4.26315789, 4.36842105, 4.47368421,
    //    	4.57894737, 4.68421053, 4.78947368, 4.89473684, 5.
	// };
	#endif
	//=================================================
	
	// shared Ts:
	double Ts[] = {
	// 	1.        , 1.08163265, 1.16326531, 1.24489796, 1.32653061,
    //    1.40816327, 
	//    1.48979592, 1.57142857, 1.65306122, 1.73469388,	// these have 10^6 burn in steps 
    //    1.81632653, 
	   1.89795918, 1.97959184, 2.06122449, 2.14285714,
    //    2.2244898 , 2.30612245, 2.3877551 , 2.46938776, 2.55102041,
    //    2.63265306, 2.71428571, 2.79591837, 2.87755102, 2.95918367,
    //    3.04081633, 3.12244898, 3.20408163, 3.28571429, 3.36734694,
    //    3.44897959, 3.53061224, 3.6122449 , 3.69387755, 3.7755102 ,
    //    3.85714286, 3.93877551, 4.02040816, 4.10204082, 4.18367347,
    //    4.26530612, 4.34693878, 4.42857143, 4.51020408, 4.59183673,
    //    4.67346939, 4.75510204, 4.83673469, 4.91836735, 5.
	};
	
	SamplingData sampling_data = {
		.n_burn_in = 1e6,
		.n_samples = 1000,
		.sample_period = 20, 	// same as Hoshino
		.ensemble_size = 10,
	};
	
	Lattice lat = new_lattice(2.0, Jz);		// T argument is ignored
	// run_ensemble(&lat, &sampling_data);
	// export_sampling_data(&sampling_data, &lat, "data/timeseries/test.csv");

	// many_timeseries(folder, Ts, len(Ts), &lat, &sampling_data);

	do_steps(&lat, 1e6);
	export_lattice(&lat, "data/lattices/T=3.0");

	printf("\nTotal program runtime: %.2f min.\n",
		(double)(clock() - start_time) / (CLOCKS_PER_SEC * 60));
}
