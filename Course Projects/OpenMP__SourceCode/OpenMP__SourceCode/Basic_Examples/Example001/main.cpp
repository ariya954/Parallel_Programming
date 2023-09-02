#include "stdio.h"
#include "ipp.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	// #pragma omp parallel num_threads(8)
	// 	printf ("Hello World!!\n");	

	// #pragma omp parallel num_threads(8)
	// 	printf ("T%d: Hello World!!\n", omp_get_thread_num());	


	Ipp64u start;
	#pragma omp parallel num_threads(4) private (start)
	{
		start = ippGetCpuClocks();
		printf ("(Start@ %u)\tT%d says: Hello World!!\n", start, omp_get_thread_num());
	}
	
	return 0;
}
