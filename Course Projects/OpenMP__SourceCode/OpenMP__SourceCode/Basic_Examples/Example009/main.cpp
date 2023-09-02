#include "stdio.h"
#include "omp.h"

#define		BOLD_BLACK		"\033[1m\033[30m"
#define		BOLD_RED		"\033[1m\033[31m"
#define		RESET			"\x1B[0m"


void funcA()
{
	printf (BOLD_BLACK "Thread %d is unning function A\n" RESET,
			omp_get_thread_num());
}

void funcB()
{
	printf (BOLD_RED"Thread %d is unning function B\n" RESET,
			omp_get_thread_num());
}

int main(int argc, char * argv[])
{
	#pragma omp parallel num_threads(4)
	{
		#pragma omp sections
		{
			#pragma omp section
				(void) funcA();
			#pragma omp section
				(void) funcB();
		}
	}
	return 0;
}
