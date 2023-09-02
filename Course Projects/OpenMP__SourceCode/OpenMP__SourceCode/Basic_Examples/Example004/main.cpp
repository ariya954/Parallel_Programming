#include "stdio.h"
#include "ipp.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	printf ("Master thread (T%d) (before parallel construct)\n", omp_get_thread_num());
	#pragma omp parallel num_threads (4)
	{
		printf ("T%d says Hello World!!\n", omp_get_thread_num());
		if (omp_get_thread_num() == 2)
			printf("\tT%d says something else\n", omp_get_thread_num());
	}
	printf ("Master thread (T%d) (after parallel construct)\n", omp_get_thread_num());
	return 0;
}
