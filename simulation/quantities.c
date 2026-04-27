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