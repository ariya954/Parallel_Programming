#include 	"stdio.h"
#include 	"ipp.h"
#include 	"x86intrin.h"

#define		VECTOR_SIZE		  	16000000

int main (void)
{
	Ipp64u start, end;
	Ipp64u time1, time2;

	float fSTmpRes[4];
	float fSRes;
	float fVRes;

	float *fArray;
	fArray = new float [VECTOR_SIZE];

	if (!fArray) {
		printf ("Memory allocation error!!\n");
		return 1;
	}
	// Initialize vectors with random numbers
	for (long i = 0; i < VECTOR_SIZE; i++)
		fArray[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));

	// Serial implementation
	start = ippGetCpuClocks();
	fSRes = 0.0;
	for (long i = 0; i < VECTOR_SIZE; i++)
		fSRes += fArray[i];
	end   = ippGetCpuClocks();
	time1 = end - start;

	// Parallel implementation
	start = ippGetCpuClocks();
	__m128 vec;
	__m128 sum = _mm_set1_ps(0.0f);
	for (long i = 0; i < VECTOR_SIZE; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
		sum = _mm_add_ps (sum, vec);
	}
	sum = _mm_hadd_ps (sum, sum);
	sum = _mm_hadd_ps (sum, sum);
	fVRes = _mm_cvtss_f32 (sum);
	end   = ippGetCpuClocks();
	time2 = end - start;

	printf ("\nSerial Result   = %f \nParallel Result = %f\n", fSRes, fVRes);
	printf ("Serial Run time = %d \n", (Ipp32s) time1);
	printf ("Parallel Run time = %d \n", (Ipp32s) time2);
	printf ("Speedup = %f\n\n", (float) (time1)/(float) time2);

	return 0;
}
