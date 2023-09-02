#include "stdio.h"
#include "ipp.h"
#include "omp.h"

#define		NUM_OF_THREADS		4

char ColorCodes[5][250] = {
	"\033[1m\033[30m",		// Bold BLACK
	"\033[1m\033[31m",		// Bold RED
	"\033[1m\033[32m",		// Bold GREEN
	"\033[1m\033[34m",		// Bold BLUE
	"\x1B[0m"				// Reset
};

int main(int argc, char * argv[])
{
	// #pragma omp parallel num_threads(4)
	// 	printf ("Hello World!!\n");

	char str [NUM_OF_THREADS][255];

	Ipp64u start;
	#pragma omp parallel num_threads (NUM_OF_THREADS) private (start)
	{
		start = ippGetCpuClocks();
		sprintf (str[omp_get_thread_num()], "%s(Start@ %u)\t T%d says: Hello World!!\n", ColorCodes[omp_get_thread_num()], start, omp_get_thread_num());
	}

	for (int i=0; i < NUM_OF_THREADS; i++)
		printf ("%s", str[i]);

	printf (ColorCodes[4]);
	return 0;
}
