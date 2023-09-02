#include 	"stdio.h"
#include 	"stdlib.h"
#include 	"ipp.h"
#include 	"time.h"
#include 	"x86intrin.h"

#define		SIZE		512

int main (void)
{
	Ipp64u start, end;
	Ipp64u time1, time2, time3, time4;

	float **m1, **m2, **m3;

	m1 = new float* [SIZE];
	m2 = new float* [SIZE];
	m3 = new float* [SIZE];
	for (int i = 0; i < SIZE; i++){
		m1[i] = new float[SIZE];
		m2[i] = new float[SIZE];
		m3[i] = new float[SIZE];
	}

	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++) {
			m1[i][j] = (float) rand();
			m2[i][j] = (float) rand();
		}
	}

    // Matrix - Matrix Multiplication, Serial implementation (Ver. 1)
	start = ippGetCpuClocks();
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            m3[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                m3[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    end   = ippGetCpuClocks();
    time1 = end - start;
    
    // Matrix - Matrix Multiplication, Serial implementation (Ver. 2)
	start = ippGetCpuClocks();
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            float fTemp = 0;
            for (int k = 0; k < SIZE; k++) {
                fTemp += m1[i][k] * m2[k][j];
            }
			m3[i][j] = fTemp;
        }
    }
    end   = ippGetCpuClocks();
    time2 = end - start;

    // Matrix - Matrix Multiplication, Serial implementation (Ver. 3)
	start = ippGetCpuClocks();
	// Transpose m2
    for (int i = 0; i < SIZE; i++)
        for (int j = i + 1; j < SIZE; j++)
        {
        	float fTemp = m2[i][j];
        	m2[i][j] = m2[j][i];
        	m2[j][i] = fTemp;
        }
    // Multiply
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            float fTemp = 0;
            for (int k = 0; k < SIZE; k++) {
                fTemp += m1[i][k] * m2[j][k];
            }
            m3[i][j] = fTemp;
        }
    }
    // Transpose m2
    for (int i = 0; i < SIZE; i++)
        for (int j = i + 1; j < SIZE; j++)
        {
        	float fTemp = m2[i][j];
        	m2[i][j] = m2[j][i];
        	m2[j][i] = fTemp;
        }
    end   = ippGetCpuClocks();
    time3 = end - start;
    
    // Matrix - Matrix Multiplication, Parallel implementation
	start = ippGetCpuClocks();
	// Transpose m2
    for (int i = 0; i < SIZE; i++)
        for (int j = i + 1; j < SIZE; j++)
        {
        	float fTemp = m2[i][j];
        	m2[i][j] = m2[j][i];
        	m2[j][i] = fTemp;
        }
    // Multiply
    for (int i = 0; i < SIZE; i++) {
         for (int j = 0; j < SIZE; j++) {
             __m128 vec = _mm_setzero_ps();
             for (int k = 0; k < SIZE; k += 4)
                 vec = _mm_add_ps(vec, _mm_mul_ps(_mm_loadu_ps(&m1[i][k]), _mm_loadu_ps(&m2[j][k])));
             vec = _mm_hadd_ps(vec, vec);
             vec = _mm_hadd_ps(vec, vec);
             _mm_store_ss(&m3[i][j], vec);
         }
     }
	// Transpose m2
    for (int i = 0; i < SIZE; i++)
        for (int j = i + 1; j < SIZE; j++)
        {
        	float fTemp = m2[i][j];
        	m2[i][j] = m2[j][i];
        	m2[j][i] = fTemp;
        }
    end   = ippGetCpuClocks();
    time4 = end - start;

    printf ("\nSerial Run time (Ver. 1) = %u\n", (Ipp32s) time1);
    printf ("Serial Run time (Ver. 2) = %u  [speedup (V2 vs. V1) = %4.2f]\n", (Ipp32s) time2, (double) time1 / (double) time2);
    printf ("Serial Run time (Ver. 2) = %u  [speedup (V3 vs. V2) = %4.2f]\n", (Ipp32s) time3, (double) time1 / (double) time3);
    printf ("Parallel Run time = %u\n", (Ipp32s) time4);
	printf ("\nFake Speedup = %4.2f\n", (double) time1 / (double) time4);
    printf ("True Speedup = %4.2f\n\n", (double) time3 / (double) time4);

	return 0;
}
