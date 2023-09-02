#include "stdio.h"
#include "ipp.h"
#include "omp.h"

char ColorCodes[5][250] = {
	"\033[1m\033[30m",		// Bold BLACK
	"\033[1m\033[31m",		// Bold RED
	"\033[1m\033[32m",		// Bold GREEN
	"\033[1m\033[34m",		// Bold BLUE
	"\x1B[0m"				// Reset
};

int main(int argc, char * argv[])
{
	int i, n = 10;
	char str [n][255];
	#pragma omp parallel num_threads(4) shared(n) private(i)
	{
		#pragma omp for
			for (i = 0; i < n; i++) {
				sprintf (str[i], "%s(@%u) T%d runs i=%d\n", ColorCodes[omp_get_thread_num()], ippGetCpuClocks()%10000000L, omp_get_thread_num(), i);
				printf (str[i]);
			}
	}
	printf (ColorCodes[4]);
	return 0;
}