#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "main.h"

int main()
{
    srand(time(NULL));

    // Lattice lat = new_lattice(4, 2, 2.0, 0.5);
    Lattice lat = {0};
    lat = new_lattice(4, 2, 1.5, 0.5);

    print_lattice(lat);
    export_lattice(lat, "data.txt");

    int steps = 100;
    for (int i = 0; i < steps; i++) {
        step(lat);
    }
    
    char after_path[100];
    snprintf(after_path, sizeof(after_path),
        "data/lattice_%isteps.txt", steps);
    export_lattice(lat, after_path);
}