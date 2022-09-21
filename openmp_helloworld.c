#include <omp.h>
#include <stdio.h>
void main ( ){
	
	#pragma omp parallel
	{
		int ID = 0;
		printf("hello(%d)", ID);
		printf("world(%d)\n",ID);
	}
}