#include 	"stdio.h"
#include 	"ipp.h"
#include 	"x86intrin.h"

#define		VECTOR_SIZE		  	16000000		

int main (void)
{
	Ipp64u start, end;
	Ipp64u time1, time2;

	float *fVec1, *fVec2;
	fVec1 = new float [VECTOR_SIZE];
	fVec2 = new float [VECTOR_SIZE];

	if (!fVec1 || !fVec2) {
		printf ("Memory allocation error!!\n");
		return 1;
	}
	// Initialize vectors with random numbers
	for (long i = 0; i < VECTOR_SIZE; i++)
		fVec1[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));

	// A[i] = A[i] + i; Serial implementation
	start = ippGetCpuClocks();
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fVec2[i] = fVec1[i] + i;
	end   = ippGetCpuClocks();
	time1 = end - start;

	// A[i] = A[i] + i; Parallel implementation
	__m128 vec, index;
	start = ippGetCpuClocks();
	for (long i = 0; i < VECTOR_SIZE; i+=4) {
		vec   = _mm_loadu_ps (&fVec1[i]);
		index = _mm_set_ps (i+3.0, i+2.0, i+1.0, i);
		vec   = _mm_add_ps (vec, index); 
		_mm_store_ps (&fVec2[i], vec);
	}
	end   = ippGetCpuClocks();
	time2 = end - start;

	printf ("\nSerial Run time = %d \n", (Ipp32s) time1);
	printf ("Parallel Run time = %d \n", (Ipp32s) time2);
	printf ("Speedup = %f\n\n", (float) (time1)/(float) time2);

	return 0;
}
