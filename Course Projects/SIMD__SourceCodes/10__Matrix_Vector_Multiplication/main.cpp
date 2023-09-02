#include 	"stdio.h"
#include 	"ipp.h"
#include 	"x86intrin.h"

#define		SIZE		16000

int main (void)
{
	Ipp64u start, end;
	Ipp64u time1, time2;

	float *v1, *v2;
	float **m1;
	float fSRes = 0.0;
	float fSTmpRes[4];

	v1 = new float [SIZE];
	v2 = new float [SIZE];

	m1 = new float* [SIZE];
	for (int i = 0; i < SIZE; i++)
		m1[i] = new float[SIZE];

	for (int i = 0; i < SIZE; i++){
		v1[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));
		for (int j = 0; j < SIZE; j++)
			m1[i][j] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));
	}

	// Matrix-Vector multiplication, Serial implementation
	start = ippGetCpuClocks();
	for (long i = 0; i < SIZE; i++) {
		fSTmpRes[0] = fSTmpRes[1] = fSTmpRes[2] = fSTmpRes[3] = 0.0;
		for (long j = 0; j < SIZE; j+=4)
			fSTmpRes[0] += (v1[j] * m1[i][j]);
		for (long j = 0; j < SIZE; j+=4)
			fSTmpRes[1] += (v1[j+1] * m1[i][j+1]);
		for (long j = 0; j < SIZE; j+=4)
			fSTmpRes[2] += (v1[j+2] * m1[i][j+2]);
		for (long j = 0; j < SIZE; j+=4)
			fSTmpRes[3] += (v1[j+3] * m1[i][j+3]);
		fSRes = fSTmpRes[0] + fSTmpRes[1] + fSTmpRes[2] + fSTmpRes[3];
		v2 [i] = fSRes;
	}
	end   = ippGetCpuClocks();
	time1 = end - start;

	// Matrix-Vector multiplication, Parallel implementation
	start = ippGetCpuClocks();
	for (long i = 0; i < SIZE; i++) {
		__m128 sum = _mm_set1_ps(0.0f);
		for (long j = 0; j < SIZE; j+=4)
			sum = _mm_add_ps (sum, _mm_mul_ps (_mm_loadu_ps (&v1[j]), _mm_loadu_ps (&m1[i][j])));
		sum = _mm_hadd_ps (sum, sum);
		sum = _mm_hadd_ps (sum, sum);
		v2 [i] = _mm_cvtss_f32 (sum);
	}
	end   = ippGetCpuClocks();
	time2 = end - start;

	printf ("\nSerial Run time   = %u\n", (Ipp32s) time1);
	printf ("Parallel Run time = %u\n", (Ipp32s) time2);
	printf ("Speedup = %4.2f\n\n", (float) (time1)/(float) time2);

	return 0;
}
