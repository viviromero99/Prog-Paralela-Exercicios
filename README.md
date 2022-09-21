# How to run MPI

mpicc file.c -o file

mpirun -np X --oversubscribe file


# How to run OpenMP

gcc file.c -o file -fopenmp

gcc openmp_helloworld.c -o hello -fopenmp

./file
