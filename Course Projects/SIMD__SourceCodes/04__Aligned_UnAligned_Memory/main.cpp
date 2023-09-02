#include "stdio.h"
#include "stdlib.h"
#include "ipp.h"
#include "x86intrin.h"

#define		ARRAY_SIZE			65536

int main (void)
{
	Ipp64u start, end;
	int i;
	__m128 v1, v2;

	__attribute__((aligned(16))) float fPtrA [ARRAY_SIZE];
	float fPtrU [ARRAY_SIZE];

	for (i=0; i<ARRAY_SIZE; i++) {
		fPtrA[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));
		fPtrA[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));
	}

	start = ippGetCpuClocks();
	for (i=0; i<ARRAY_SIZE; i+=4) {
		v1 = _mm_loadu_ps (&fPtrU[i]);
	}
	end   = ippGetCpuClocks();
	printf ("\nUnaligned memory access takes %d cycles\n", (Ipp32s) (end - start));

	start = ippGetCpuClocks();
	for (i=0; i<ARRAY_SIZE; i+=4) {
		v2 = _mm_load_ps (&fPtrA[i]);
	}
	end   = ippGetCpuClocks();
	printf ("\nAligned memory access takes %d cycles\n\n", (Ipp32s) (end - start));

	return 0;
}
