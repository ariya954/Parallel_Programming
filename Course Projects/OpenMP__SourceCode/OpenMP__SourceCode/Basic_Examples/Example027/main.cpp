#include "stdio.h"
#include "omp.h"

int main(int argc, char * argv[])
{
	int TID;
	omp_set_nested (1);
	printf("Nested parallelism is %s\n",
			omp_get_nested() ? "supported" : "not supported");
	#pragma omp parallel num_threads(3) private (TID)
	{
		TID = omp_get_thread_num();
		printf("T%d executes the outer parallel region\n", TID);

		#pragma omp parallel num_threads(2)
		{
			printf(" T%d_%d executes inner parallel region\n", TID, 
					omp_get_thread_num());
		} /*-- End of inner parallel region --*/
	} /*-- End of outer parallel region --*/
	return 0;
}
