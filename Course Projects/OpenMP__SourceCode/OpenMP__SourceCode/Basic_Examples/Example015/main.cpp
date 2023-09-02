#include "stdio.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	int i, TID, vlen = 10, n = 2;
	int indx, a[vlen];
	for(i = 0; i < vlen; i++)
		a[i] = -i-1;
	indx = 4;
	#pragma omp parallel num_threads (3) default(none) \
			firstprivate(indx) private(i,TID) shared(n,a)
	{
		TID = omp_get_thread_num();
		indx = offset + n*TID;
		for(i = indx; i < indx + n; i++)
			a[i] = TID + 1;
	} /*-- End of parallel region --*/	printf("After the parallel region:\n");
	for (i = 0; i < vlen; i++)
		printf("a[%d] = %d\n", i, a[i]);
	return 0;
}
