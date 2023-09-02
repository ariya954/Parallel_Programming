#include "stdio.h"
#include "omp.h"

char ColorCodes[5][250] = {
	"\033[1m\033[30m",		// Bold BLACK
	"\033[1m\033[31m",		// Bold RED
	"\033[1m\033[32m",		// Bold GREEN
	"\033[1m\033[34m",		// Bold BLU
	"\x1B[0m"				// Reset
};

int main(int argc, char * argv[])
{
	char str [4][255];
	#pragma omp parallel num_threads(4)
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				sprintf (str[omp_get_thread_num()], "%sT%d does one thing\n", ColorCodes[omp_get_thread_num()], omp_get_thread_num());
				printf (str[omp_get_thread_num()]);
			}
			#pragma omp section
			{
				sprintf (str[omp_get_thread_num()], "%sT%d does another thing\n", ColorCodes[omp_get_thread_num()], omp_get_thread_num());
				printf (str[omp_get_thread_num()]);
			}
			#pragma omp section
			{
				sprintf (str[omp_get_thread_num()], "%sT%d does some other thing\n", ColorCodes[omp_get_thread_num()], omp_get_thread_num());
				printf (str[omp_get_thread_num()]);
			}
			#pragma omp section
			{
				sprintf (str[omp_get_thread_num()], "%sT%d does different thing\n", ColorCodes[omp_get_thread_num()], omp_get_thread_num());
				printf (str[omp_get_thread_num()]);
			}
			#pragma omp section
			{
				sprintf (str[omp_get_thread_num()], "%sT%d does totally different thing\n", ColorCodes[omp_get_thread_num()], omp_get_thread_num());
				printf (str[omp_get_thread_num()]);
			}
		}
	}

	printf (ColorCodes[4]);
	return 0;
}
