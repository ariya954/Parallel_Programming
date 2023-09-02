#include "stdio.h"
#include "ipp.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	#pragma omp parallel
	{
		printf ("T%d says Hello World!!\n", omp_get_thread_num());
		if (omp_get_thread_num() == 2)
			printf("\tT%d says something else!!\n", omp_get_thread_num());
	}

	// Ipp64u start;
	// #pragma omp parallel num_threads (4)
	// {
	// 	start = ippGetCpuClocks();

	// 	printf ("(Start@ %u)\tT%d says Hello World!!\n", start, omp_get_thread_num());
	// 	if (omp_get_thread_num() == 2)
	// 		printf("\t(@ %u)\tT%d says something else!!\n", ippGetCpuClocks(), omp_get_thread_num());
	// }
	return 0;
}
