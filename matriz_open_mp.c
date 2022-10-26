#include <omp.h> 
#include <stdlib.h> 
#include <stdio.h>

// #define SIZE 1000 /* tam matrizes */
// int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

#define SIZE 10 /* tam matrizes */

int A[10][10] = { {2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
				{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},};

int B[10][10] = { {10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
				{10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
				{10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
				{10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
				{10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
				{10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
				{10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
				{10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
				{10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
				{10, 10, 10, 10, 10, 10, 10, 10, 10, 10},};

int C[SIZE][SIZE];


int main(int argc, char *argv[]){ 
	int i,j,k,N;
	N=10;
	// fill_matrix(A);
	// fill_matrix(B);

	#pragma omp parallel for private(i,j,k) shared(A,B,C,N) schedule(dynamic, 2)

		for ( i = 0; i < N; i++ ){

			#pragma omp parallel for

			for ( j = 0; j < N; j++ ) {

				C[i][j] = 0.0;
				for ( k = 0; k < N; k++ ){

					C[i][j] = C[i][j] + A[i][k] * B[k][j];
				}
			}	
		}

	for (int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			printf(" %d ", C[i][j]);
		}
		printf("\n");
	}
}
