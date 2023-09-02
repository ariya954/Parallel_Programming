#include "stdio.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	int ic, i, n = 10000;

	ic = 0;
	#pragma omp parallel for num_threads(4) shared(n, ic) private(i)
		for (i = 0; i < n; i++)
			#pragma omp atomic
				ic = ic + 1;
	printf("counter = %d\n", ic);
	return 0;
}
