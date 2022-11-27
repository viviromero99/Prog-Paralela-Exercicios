#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#define num_threads 2

int main (int argc, char** argv) {

	int my_rank;
	int p; // número de processos
	float a=0.0, b=1.0; // intervalo a calcular
	omp_set_num_threads(num_threads);
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
	
	MPI_Status status;
	
	/* Declaração da função que vai calcular a integral */
	float calcula(float local_a, float local_b, int local_n, float h);
	
	/* Inicializa o ambiente de execução */
	MPI_Init(&argc, &argv);
	
	/* Determina o número de processos no comunicador */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	/* Determina o rank (identificação) do processo no comunicador */
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	h = (b-a) / n;
	local_n = n / p;
	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n * h;
	
	integral = calcula(local_a, local_b, local_n, h);
	
	if(my_rank == 0) { /* Mestre */
		//printf("Primeiro if");
	
		total = integral;
		for(source=1; source<p; source++) {
			//printf("dentro do for %d", source);
			/* endereço da msg; count: Número máximo de elementos a serem recebidos; tipo do dado; source : rank do processo destino( * = MPI_ANY_SOURCE); tag: tipo de mensagem a receber ( * = MPI_ANY_TAG); comm: comunicador; status- uma estrutura que contém as informações struct MPI_Struct { int MPI_SOURCE; int MPI_TAG; int MPI_ERROR; }; */ 
			MPI_Recv(&integral, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);

			total +=integral;
		}
	
	}else{ /* Trabalhador*/
		
		/* endereço da msg; count: Número máximo de elementos a serem recebidos; tipo de dado; dest: rank do processo destino( * = MPI_ANY_SOURCE); tag: tipo de mensagem a receber ( * = MPI_ANY_TAG); comm: comunicador */
		MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
		//printf("bloco else");
	}
		
	if(my_rank == 0) 
		//printf("Segundo if");
		printf("Resultado: %f\n", total);
	
	/* Termina o ambiente de execução */
	MPI_Finalize();
	return 0;
	
}
	
float calcula(float local_a, float local_b, int local_n, float h) {

	float integral;


	float f(float x); // função a integrar
	integral = ( f(local_a) + f(local_b) ) / 2.0;


	
	#pragma omp parallel for reduction(+: integral)
	for(int i=1; i<=local_n; i++) {
		float x = local_a + i*h;
		integral += f(x);

	}
	integral *= h;
	return integral;
}
	
float f(float x) {
	
	/* Valor de retorno */ 
	float fx; 

	/* Esta é a função a integrar, exemplo: função quadrática */
	fx = x * x;

	return fx;	

}

	

