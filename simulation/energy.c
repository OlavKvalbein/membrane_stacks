#include "main.h"

int sum1(const Lattice* lat)
{
    int sum = 0;
    for (int z = 1; z < lat->Lz+1; z++) // not indexing the 0-spin boundary
        for (int i = 1; i < lat->L; i++) 
            for (int j = 1; j < lat->L; j++) 
                sum += spin(lat, z, i, j)*nesw_sum(lat, z, i, j);
    return sum;
}

int sum2(const Lattice* lat)
{
    int sum = 0;
    for (int z = 1; z < lat->Lz+1; z++) 
        for (int i = 1; i < lat->L; i++) 
            for (int j = 1; j < lat->L; j++) 
                sum += spin(lat, z, i, j)*spin(lat, z+1, i, j);
    return sum;
}

// The hamiltionian in units of J.
double energy(const Lattice* lat)
{
    // H = - Sum (S_i,p * S_i,p') - Jz * Sum (S_i,p + S_i+1,p)
    //   = - Sum1 - Jz * Sum2
    double H = - sum1(lat) - lat->Jz*sum2(lat);
    return H;
}