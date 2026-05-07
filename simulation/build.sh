cd simulation
gcc -O3 -Wall -Wextra -Wuninitialized -o ../build.exe main.c lattice.c quantities.c ensembles.c utils.c 
# gcc -O3 -Wall -Wextra -Wuninitialized -o ../build.exe main.c lattice.c utils.c 
cd ..