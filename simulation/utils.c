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

double lerp(double a, double b, double t)
{
	return (b - a) * t + a;
}
