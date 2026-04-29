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

double mean_col_spin(const Lattice* lat, int i, int j)
{
	double mean = 0;
	for (int z = 1; z < lat->Lz+1; z++)
		mean += spin(lat, z, i, j);
	return mean / lat->Lz;
}

double delta2(const Lattice* lat)
{	// NOTE: this does not make sense. always return 0.
	// delta^2 = 1/(L^2*Lz^2) * Sum_i,j (Sum_z (S_i,j,z - ~S_i,j))^2
	// My interpretation of eq. (4) in Hoshino, with rho -> i,j and i -> z.
	// ~S_i,j is the mean of the spin in the (i,j) column. Could very well be wrong.
	// The ensemble mean will be accounted for when delta2 is averaged over ensembles.
	// Then we could average the ensemble mean over a time where the lattice is equilibriated.

	double outer_sum = 0;
	for (int i = 1; i < lat->L+1; i++) { 	// not summing over 0-spin boundary.
		for (int j = 1; j < lat->L+1; j++) {
			double inner_sum = 0;
			for (int z = 1; z < lat->Lz+1; z++)
				inner_sum += spin(lat, z, i, j) - mean_col_spin(lat, i, j);
			outer_sum += inner_sum*inner_sum;
		}
	}

	return outer_sum/(lat->L*lat->L*lat->Lz*lat->Lz);
}