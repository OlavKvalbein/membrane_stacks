#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "main.h"

int main()
{
    srand(time(NULL));

    int L = 16;
    int Lz = 8;
    double T = 1.5;
    double Jz = 0.5;
    Lattice lat = new_lattice(L, Lz, T, Jz);

    int steps = 100;
    for (int i = 0; i < steps; i++) {
        step(&lat);
    }

    // char after_path[100];
    // snprintf(after_path, sizeof(after_path),
    //     "data/lattice_%isteps.txt", steps);
    // export_lattice(&lat, after_path);
}