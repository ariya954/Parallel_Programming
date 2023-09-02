#include 	"stdio.h"
#include 	"stdlib.h"
#include 	"omp.h"


int fib (int n)
{
	int x, y; 
	
	if (n < 2) 
		return n;
	x = fib(n - 1);
	y = fib(n - 2);
	return (x+y);
}

int parallel_fib (int n)
{
	int x, y; 
	
	if (n < 2) 
		return n;

	
	#pragma omp task shared(x) shared(n)
	{
		printf ("T%d --> Fib(%d) \n", omp_get_thread_num (), n-1);
		x = parallel_fib(n - 1);
	}

	#pragma omp task shared(y) shared(n)
	{
		printf ("T%d --> Fib(%d) \n", omp_get_thread_num (), n-2);
		y = parallel_fib(n - 2);
	}
	#pragma omp taskwait

	printf ("T%d --> Fib(%d) + Fib(%d) \n", omp_get_thread_num (), n-1, n-2);
	return (x+y);
}


int main (void)
{
	int NW = 10;

    // ----  Serial fib  --------------------------------------------------------
	printf ("Serial: Fib(%d) = %d \n", NW, fib (NW));

   // ----  OpenMP parallel version -------------------------------------------------------
    #pragma omp parallel num_threads (4)
    {
    	#pragma omp master
    		printf ("Parallel: Fib(%d) = %d \n", NW, parallel_fib (NW));
    }

	return 0;
}
