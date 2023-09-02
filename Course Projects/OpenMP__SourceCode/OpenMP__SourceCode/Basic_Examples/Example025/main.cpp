#include "stdio.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	int TID, i, n = 10;
	int a[n];

	for (i = 0; i < n; i++)
		a[i] = 0;

	#pragma omp parallel for default(none) ordered schedule(runtime) \
						 private(i,TID) shared(n, a)
		for (i = 0; i < n; i++)
		{
			TID = omp_get_thread_num();
			printf("Thread %d updates a[%d]\n",TID,i);
			a[i] += i;
			#pragma omp ordered
			{
				printf("Thread %d prints value of a[%d] = %d\n",TID,i,a[i]);
			}
		} /*-- End of parallel for --*/
	return 0;
}
