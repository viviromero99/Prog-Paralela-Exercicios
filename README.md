# How to run MPI

mpicc file.c -o file

mpirun -np X --oversubscribe file


# How to run OpenMP

gcc file.c -o file -fopenmp

export OMP_NUM_THREADS=4

setenv OMP_NUM_THREADS 4

./file
