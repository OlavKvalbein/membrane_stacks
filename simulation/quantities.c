#include "main.h"

// The hamiltionian in units of J.
double energy(const Lattice* lat)
{
	// H = - Sum (S_i,p * S_i,p') - Jz * Sum (S_i,p + S_i+1,p)
	//   = - sum1 - Jz * sum2
	int sum1 = 0;
	int sum2 = 0;

	for (int z = 0; z < Lz; z++) { 
		for (int i = 0; i < L; i++) {
		   for (int j = 0; j < L; j++) {
				char s = lat->spin[z][i][j];
				sum1 += s * nesw_sum(lat, z, i, j);
				sum2 += s * lat->spin[(z+1)%Lz][i][j];
	}}}
	sum1 /= 2; // accounting for double summation

	double H = - sum1 - lat->Jz*sum2;
	return H;
}

double delta2(const Lattice* lat)
{
	// delta^2 = 1/(L^2*Lz^2) * Sum_i,j (Sum_z S_i,j,z - ~S_i,j,z)^2
	// My interpretation of eq. (4) in Hoshino, with rho -> i,j and i -> z.
	// The mean spin of layer z, ~S_i,j,z will be approx 0, or exactly 0 if the A/B ratio is 1.
	// The ensemble mean will be accounted for when delta2 is averaged over ensembles.
	// Then we could average the ensemble mean over a time where the lattice is equilibriated.

	double outer_sum = 0;
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			int inner_sum = 0;
			for (int z = 0; z < Lz; z++)
				inner_sum += lat->spin[z][i][j];	// mean layer spin = 0
			outer_sum += inner_sum*inner_sum;
		}
	}

	return outer_sum/(L*L*Lz*Lz);
}