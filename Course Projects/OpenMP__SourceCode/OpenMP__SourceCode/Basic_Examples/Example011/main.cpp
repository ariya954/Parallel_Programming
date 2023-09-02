#include "stdio.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	int i, n = 10;
	int a[n];

	for (i = 0; i < n; i++)
		a[i] = 0;


	#pragma omp parallel for shared(a, n)
		for (i = 0; i < n; i++)
			a[i] += i;

	for (i = 0; i < n; i++)
		printf ("a[%d] = %d\n", i, a[i]);

	return 0;
}
