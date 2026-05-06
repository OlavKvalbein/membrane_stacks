cd simulation
gcc -pg -no-pie -o2 -o ../pg_build.exe main.c lattice.c quantities.c ensembles.c utils.c
cd ..
./pg_build.exe
gprof pg_build.exe gmon.out > profile.txt