#include "main.h"

// The hamiltionian in units of J.
double energy(const Lattice* lat)
{
	// H = - Sum (S_i,p * S_i,p') - Jz * Sum (S_i,p + S_i+1,p)
	//   = - sum1 - Jz * sum2
	int sum1 = 0;
	int sum2 = 0;
	// not summing over the 0-spin boundary
	for (int z = 1; z < lat->Lz+1; z++) { 
		for (int i = 1; i < lat->L+1; i++) {
		   for (int j = 1; j < lat->L+1; j++) {
				char s = spin(lat, z, i, j);
				sum1 += s * nesw_sum(lat, z, i, j);
				sum2 += s * spin(lat, z+1, i, j);
	}}}
	sum1 /= 2; // accounting for double summation

	double H = - sum1 - lat->Jz*sum2;
	return H;
}

double mean_spin(const Lattice* lat)
{
	double mean = 0;
	for (int i = 0; i < (lat->L+2)*(lat->L+2)*(lat->Lz+2); i++)
		mean += lat->spin[i];
	return mean / ((lat->L)*(lat->L)*(lat->Lz));
}

double mean_layer_spin(const Lattice* lat, int z)
{
	int mean = 0;
	for (int i = 1; i < lat->L+1; i++)
		for (int j = 1; j < lat->L+1; j++)
			mean += spin(lat, z, i, j);
	return (double)mean / (lat->L*lat->L);
}

double delta2(const Lattice* lat)
{
	// delta^2 = 1/(L^2*Lz^2) * Sum_i,j (Sum_z S_i,j,z - ~S_i,j,z)^2
	// My interpretation of eq. (4) in Hoshino, with rho -> i,j and i -> z.
	// The mean spin of layer z, ~S_i,j,z will be approx 0, or exactly 0 if the A/B ratio is 1.
	// The ensemble mean will be accounted for when delta2 is averaged over ensembles.
	// Then we could average the ensemble mean over a time where the lattice is equilibriated.

	// maybe cache the mean spin of each layer for efficiency

	int L = lat->L;
	int Lz = lat->Lz;

	double outer_sum = 0;
	for (int i = 1; i < L+1; i++) { 	// not summing over 0-spin boundary.
		for (int j = 1; j < L+1; j++) {
			double inner_sum = 0;
			for (int z = 1; z < Lz+1; z++)
				inner_sum += spin(lat, z, i, j) - mean_layer_spin(lat, z);
			outer_sum += inner_sum*inner_sum;
		}
	}

	return outer_sum/(L*L*Lz*Lz);
}