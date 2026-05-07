#include "main.h"

// random double in [0, 1]
double rand01()
{
	// return (double)rand() / RAND_MAX;

	// 2^32 = 4294967296, the max value of fast_rand
	return fast_rand() * (1.0/4294967296.0);
}

// random integer in [min, max]
int rand_i(int min, int max)
{
	return fast_rand() % (max - min + 1) + min;
}

double lerp(double a, double b, double t)
{
	return (b - a) * t + a;
}