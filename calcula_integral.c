#include <omp.h>
#include <stdio.h>

static int ntrapezios = 100000;
double h;
#define NUM_THREADS 4

double calcula(double local_a, double local_b, int local_n, double h) {

    double integral;
    double x, i;
    double f(double x); // função a integrar
    integral = ( f(local_a) + f(local_b) ) /2.0;
    x = local_a;

    for( i=1; i<=local_n; i++) {
        x += h;
        integral += f(x);
    }

    integral *= h;
    return integral;
}

void main (){ 
	int i, nthreads; 
	double integral[NUM_THREADS], a=0.0, b=1.0, total;

	for(i=0; i<nthreads; i++) integral[i] = 0.0;

	h = (b-a) / ntrapezios;
	omp_set_num_threads (NUM_THREADS);

	#pragma omp parallel
	{ 
		int i, id, nthrds, local_n;
		double local_a, local_b, integral_local;

		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if(id == 0) nthreads = nthrds;

		local_n = ntrapezios / nthrds;
	    local_a = a + id * local_n * h;
	    local_b = local_a + local_n * h;

		integral[id] = calcula(local_a, local_b, local_n, h);

	}
	for(i=0, total; i<nthreads; i++)total +=integral[i];
	printf("Resultado: %f\n", total);

}


double f(double x) {

    double fx; // valor de retorno

    // esta é a função a integrar
    // exemplo: função quadrática

    fx = x * x;

    return fx;
}