#include "stdio.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	printf ("Number of cores = %d\n", omp_get_num_procs());
	printf ("Number of threads = %d\n", omp_get_num_threads());
	printf ("Maximum number of threads = %d\n", omp_get_max_threads());
	printf ("Netsed parallel construct is %s\n", omp_get_nested() ? "supported" : "not-supported");
	return 0;
}
