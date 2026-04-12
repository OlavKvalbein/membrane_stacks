#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"

// random double in [0, 1]
double rand01()
{
	return (double)rand() / RAND_MAX;
}

// random integer in [0, n]
int rand0n(int n)
{
	return rand() % (n+1);
}

// random integer in [min, max]
int rand_i_range(int min, int max)
{
	return rand() % (max - min + 1) + min;
}