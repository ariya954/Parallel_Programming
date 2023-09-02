#include "stdio.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	int i, sum, n = 10;
	int a[n];

	sum = 0;
	for (i = 0; i < n; i++)
		a[i] = i;
	#pragma omp parallel for num_threads(4) default(none)  \
		                     shared(n,a) reduction(+:sum)
		for (i = 0; i < n; i++)
			sum += a[i];
	/*-- End of parallel reduction --*/
	printf("Value of sum after parallel region: %d\n",sum);

	return 0;
}
