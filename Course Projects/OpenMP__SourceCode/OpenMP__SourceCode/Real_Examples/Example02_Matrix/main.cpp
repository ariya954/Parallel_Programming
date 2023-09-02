#include 	"stdio.h"
#include 	"stdlib.h"
#include 	"ipp.h"
#include 	"time.h"
#include 	"x86intrin.h"

#define		SIZE		500

int main (void)
{
	Ipp64u start, end;
	Ipp64u time1, time2;

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

	printf ("=== Matrix Multiplication  =============================================\n");

    // ----  First serial version --------------------------------------------------------
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
    printf ("Serial run time (Ver. 1) = %u\n", (Ipp32s) time1);

    // ----  Second serial version -------------------------------------------------------
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
    time1 = end - start;
    printf ("Serial run time (Ver. 2) = %u\n", (Ipp32s) time1);

	// ----  Third serial version -------------------------------------------------------
    start = ippGetCpuClocks();

	// Transpose m2
    for (int i = 0; i < SIZE; i++)
        for (int j = i + 1; j < SIZE; j++)
        {
        	float fTemp = m2[i][j];
        	m2[i][j] = m2[j][i];
        	m2[j][i] = fTemp;
        }
    
    // Matrix multiplication
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
    time1 = end - start;
    printf ("Serial run time (Ver. 3) = %u\n", (Ipp32s) time1);


    // ----  SIMD parallel version -------------------------------------------------------
	start = ippGetCpuClocks();
	
    // Transpose m2
    for (int i = 0; i < SIZE; i++)
        for (int j = i + 1; j < SIZE; j++)
        {
        	float fTemp = m2[i][j];
        	m2[i][j] = m2[j][i];
        	m2[j][i] = fTemp;
        }

    // Matrix multiplication
    for (int i = 0; i < SIZE; i++) {
         for (int j = 0; j < SIZE; j++) {
             __m128 vec = _mm_setzero_ps();
             for (int k = 0; k < SIZE; k += 4)
                 vec = _mm_add_ps(vec, _mm_mul_ps(_mm_loadu_ps(&m1[i][k]), 
                                       _mm_loadu_ps(&m2[j][k])));
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
    time2 = end - start;
    printf ("SIMD run time = %u\n", (Ipp32s) time2);
	printf ("Speedup = %4.2f\n\n", (double) time1 / (double) time2);


    // ----  OpenMP parallel version -------------------------------------------------------
    start = ippGetCpuClocks();
    
    // Transpose m2
    #pragma omp parallel for num_threads(4)
        for (int i = 0; i < SIZE; i++)
            for (int j = i + 1; j < SIZE; j++)
            {
                float fTemp = m2[i][j];
                m2[i][j] = m2[j][i];
                m2[j][i] = fTemp;
            }

    // Matrix multiplication
    #pragma omp parallel for num_threads(4)
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
    #pragma omp parallel for num_threads(4)
        for (int i = 0; i < SIZE; i++)
            for (int j = i + 1; j < SIZE; j++)
            {
                float fTemp = m2[i][j];
                m2[i][j] = m2[j][i];
                m2[j][i] = fTemp;
            }

    end   = ippGetCpuClocks();
    time2 = end - start;
    printf ("OpenMP run time = %u\n", (Ipp32s) time2);
    printf ("Speedup = %4.2f\n\n", (double) time1 / (double) time2);

   // ----  OpenMP/SIMD parallel version --------------------------------------------------
    start = ippGetCpuClocks();
    
    // Transpose m2
    #pragma omp parallel for num_threads(4)
        for (int i = 0; i < SIZE; i++)
            for (int j = i + 1; j < SIZE; j++)
            {
                float fTemp = m2[i][j];
                m2[i][j] = m2[j][i];
                m2[j][i] = fTemp;
            }

    // Matrix multiplication
    #pragma omp parallel for num_threads(4)
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
    #pragma omp parallel for num_threads(4)
        for (int i = 0; i < SIZE; i++)
            for (int j = i + 1; j < SIZE; j++)
            {
                float fTemp = m2[i][j];
                m2[i][j] = m2[j][i];
                m2[j][i] = fTemp;
            }
    
    end   = ippGetCpuClocks();
    
    time2 = end - start;
    printf ("OpenMP+SIMD run time = %u\n", (Ipp32s) time2);
    printf ("Speedup = %4.2f\n\n", (double) time1 / (double) time2);

	return 0;
}
