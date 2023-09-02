#include "stdio.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	int i, a, n = 10, b[n];

	#pragma omp parallel num_threads(4) shared(a,b) private(i)
	{
		#pragma omp master
		{
			a = 10;
			printf("Master construct is executed by thread %d\n",
					omp_get_thread_num());
		}
		#pragma omp barrier
		#pragma omp for
			for (i = 0; i < n; i++)
				b[i] = a;
	} /*-- End of parallel region --*/
	printf("After the parallel region:\n");
	for (i=0; i<n; i++)
		printf("b[%d] = %d\n",i,b[i]);
	return 0;
}
