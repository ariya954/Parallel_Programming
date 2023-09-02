#include "stdio.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	omp_lock_t lck;
	int i, localSum, sum = 0, TID, n = 10;
	omp_init_lock(&lck);
	#pragma omp parallel num_threads(4) shared(sum) \
	                     private(i, localSum, TID)
	{
		TID = omp_get_thread_num();
		localSum = 0;
		for (i = 0; i < n; i++)
			localSum ++;
		omp_set_lock(&lck);
		sum += localSum;
		omp_unset_lock(&lck);
	}
	omp_destroy_lock(&lck);
	printf ("The sum is %d\n", sum);

	return 0;
}
