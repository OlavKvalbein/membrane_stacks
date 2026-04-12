cd c
# gcc main.c lattice.c utils.c -o3 -Wall -Wextra -o ../simulation.exe
gcc main.c lattice.c utils.c -g -Wall -Wextra -o ../simulation.exe
cd ..
./simulation.exe
