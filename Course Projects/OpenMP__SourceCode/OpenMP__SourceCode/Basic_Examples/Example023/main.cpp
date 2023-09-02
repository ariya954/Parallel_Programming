#include "stdio.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	int TID, n = 5;
	#pragma omp parallel if (n > 5) default(none) \
						  private(TID) shared(n)
	{
		TID = omp_get_thread_num();
		#pragma omp single
		{
			printf("Value of n = %d\n",n);
			printf("Number of threads in parallel region: %d\n",
						omp_get_num_threads());
		}
		printf("Print statement executed by thread %d\n",TID);
	} /*-- End of parallel region --*/
	return 0;
}
