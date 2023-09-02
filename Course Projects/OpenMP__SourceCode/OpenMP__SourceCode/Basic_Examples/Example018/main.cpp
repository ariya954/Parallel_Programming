#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	int TID, sum, sumLocal, i, n = 10;
	int a[n];

	for (i = 0; i < n; i++)
		a[i] = i;

	sum = 0;
	#pragma omp parallel shared(n, a, sum) private(TID, sumLocal)
	{
		TID = omp_get_thread_num();
		sumLocal = 0;
		#pragma omp for
			for (i = 0; i < n; i++)
				sumLocal += a[i];

		#pragma omp critical (update_sum)
		{
			sum += sumLocal;
			printf("TID=%d: \tsumLocal=%d \tsum = %d\n",TID, sumLocal, sum);
		}
	} /*-- End of parallel region --*/
	printf("Value of sum after parallel region: %d\n",sum);
	return 0;
}
