#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "main.h"

int main()
{
    srand(time(NULL));

    Lattice lat = new_lattice(16, 8, 1.5, 0.5);

    int steps = 1000;
    for (int i = 0; i < steps; i++) {
        step(lat);
    }

    char after_path[100];
    snprintf(after_path, sizeof(after_path),
        "data/lattice_%isteps.txt", steps);
    export_lattice(lat, after_path);
}