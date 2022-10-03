# How to run MPI

mpicc file.c -o file

mpirun -np X --oversubscribe file


# How to run OpenMP

gcc file.c -o file -fopenmp

export OMP_NUM_THREADS=X

./file

time (ou /bin/time) -v (ou -verbose)
