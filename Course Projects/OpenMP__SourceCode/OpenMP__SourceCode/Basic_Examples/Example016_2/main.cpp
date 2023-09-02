#include "stdio.h"
#include "unistd.h"
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
	int i, j, n = 9;
	char str [n][255];
	#pragma omp parallel for num_threads(4) default(none) schedule(runtime) \
							 private(i,j) shared(n, ColorCodes, str)
		for (i = 0; i < n; i++)
		{
			sprintf(str[i], "%sT%d executes Iteration %d \n", ColorCodes[omp_get_thread_num()],
					omp_get_thread_num(), i);
			printf(str[i]);
			for (j = 0; j < i; j++)
				sleep (.2);
		} /*-- End of parallel for --*/

	printf (ColorCodes[4]);
	return 0;
}
