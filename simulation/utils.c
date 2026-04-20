#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "main.h"

// random double in [0, 1]
double rand01()
{
	return (double)rand() / RAND_MAX;
}

// random integer in [min, max]
int rand_i(int min, int max)
{
	return rand() % (max - min + 1) + min;
}