#include "stdio.h"
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
	int i, a, n = 5;
	char str [n][255];
	#pragma omp parallel for num_threads(3) private(i) lastprivate(a)
		for (i = 0; i < n; i++) {
			a = i + 1;
			sprintf(str[i], "%sT%d finds a = %d for i = %d\n", ColorCodes[omp_get_thread_num()], 
					omp_get_thread_num(), a, i);
			printf (str[i]);
		} /*-- End of parallel for --*/
	printf (ColorCodes[4]);
	printf ("The value of a after parallel construct is %d\n", a);
	return 0;
}
