#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#define nthreads 2

int main (int argc, char** argv) {

	int my_rank;
	int p; // número de processos
	float a=0.0, b=1.0; // intervalo a calcular
	omp_set_num_threads(nthreads);
	/*OBS.: n par */
	int n=1024;// float número de trapezóides
	
	float h; // base do trapezóide
	float local_a, local_b; // intervalo local
	int local_n;  // número de trapezóides local
	float integral; // integral no meu intervalo
	float total; // integral total
	int source; // remetente da integral
	int dest=0; // destino das integrais (nó 0)
	int tag=200; // tipo de mensagem (único)
	int provided;
	
	MPI_Status status;
	
	/* Declaração da função que vai calcular a integral */
	void calcula(float local_a, float local_b, int local_n, float h, int my_rank);
	
	/* Inicializa o ambiente de execução */
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
	
	/* Determina o número de processos no comunicador */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	/* Determina o rank (identificação) do processo no comunicador */
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	h = (b-a) / n;
	local_n = n / p;
	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n * h;
	
	calcula(local_a, local_b, local_n, h, my_rank);

	if(my_rank == 0 ) { 

		total = integral;

		for(source=1; source<(p*nthreads); source++) {
			
			MPI_Recv(&integral, 1, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			total +=integral;
		}

		printf("Resultado: %f\n", total);
	}
	
	/* Termina o ambiente de execução */
	MPI_Finalize();
	return 0;
	
}
	
void calcula(float local_a, float local_b, int local_n, float h, int my_rank) {

	float integral;
	float x;

	float f(float x); // função a integrar

	x = local_a;
	integral = ( f(local_a) + f(local_b) ) / 2.0;

	#pragma omp parallel num_threads(nthreads)
	{
		#pragma omp for
			for(int i=1; i<=local_n; i++) {
				x += h;
				integral += f(x);
			}
		integral *= h;

		int tid = omp_get_thread_num();
		MPI_Send(&integral, 1, MPI_FLOAT, 0, tid, MPI_COMM_WORLD);

	}
}
	
float f(float x) {
	
	/* Valor de retorno */ 
	float fx; 

	/* Esta é a função a integrar, exemplo: função quadrática */
	fx = x * x;

	return fx;	

}

	

