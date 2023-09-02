#include "stdio.h"
#include "unistd.h"
#include "ipp.h"
#include "omp.h"

#define		RESET 		"\x1B[0m"

char ColorCodes[4][250] = {
	"\033[1m\033[30m",		// Bold BLACK
	"\033[1m\033[31m",		// Bold RED
	"\033[1m\033[32m",		// Bold GREEN
	"\033[1m\033[34m"
};

int main(int argc, char * argv[])
{
	int i, n = 10;
	int a = 0, b[n];
	char str  [n][255];
	char str1 [255];
	#pragma omp parallel shared(a, b, n) private(i)
	{
		#pragma omp single //nowait
		{
			//sleep (1);
			a = 10;
			sprintf (str1, "%s(@ %u) T%d executes single construct\n", ColorCodes[omp_get_thread_num()], ippGetCpuClocks(), omp_get_thread_num());
			printf(str1);
			printf(RESET);
		} /* A barrier is automatically inserted here */

		#pragma omp for
			for (i = 0; i < n; i++) {
				b[i] = a;
				sprintf (str[i], "%s(@ %u) T%d does b[%d]=%d\n", ColorCodes[omp_get_thread_num()], ippGetCpuClocks(), omp_get_thread_num(), i, a);
			}
	} /*-- End of parallel region --*/

	printf("After the parallel region:\n");
	for (i = 0; i < n; i++)
		printf(str[i]);
	printf(RESET);
	return 0;
}
