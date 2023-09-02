#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "ipp.h"
#include <sys/time.h>
#include "x86intrin.h"

int main (void)
{
	int i;
	printf ("\n");

	// Execution time using time() 
	time_t start1, end1;
	start1 = time(NULL);
	for (i=0; i<100000; i++);
	end1 = time(NULL);
	printf("The elapsed time using time() function is %ld seconds\n\n", (end1 -start1));

	// Execution time using gettimeofday()
	struct timeval start2, end2;
	gettimeofday(&start2, NULL);
	for (i=0; i<100000; i++);
	gettimeofday(&end2, NULL);
	long seconds = (end2.tv_sec - start2.tv_sec);
	long micros = ((seconds * 1000000) + end2.tv_usec) - (start2.tv_usec);
	printf("The elapsed time using gettimeofday() function is %ld seconds and %ld micro seconds\n\n",seconds, micros);
	
	// Execution time using ippGetCpuClocks(
	Ipp64u start3, end3;
	start3 = ippGetCpuClocks();
	for (i=0; i<100000; i++);
	end3   = ippGetCpuClocks();
	printf ("The elapsed time using ippGetCpuClocks() function is %d cpu cycles\n\n", (Ipp32s) (end3 - start3));

	return 0;
}
