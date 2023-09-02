#include <stdio.h>  
#include <ipp.h>
#include <omp.h>  
  
int main (void)
{
   Ipp64u start, end;
   int a[100000], b[100000], c[100000];

   start = ippGetCpuClocks();
   #pragma omp parallel num_threads(4) shared(a)
      #pragma omp for 
         for (int i = 0; i < 100000; ++i)
            a[i] = i;

   #pragma omp parallel num_threads(4) shared(b)
      #pragma omp for 
         for (int i = 0; i < 100000; ++i)
            b[i] = i;

   #pragma omp parallel num_threads(4) shared(a, b, c)
      #pragma omp for 
         for (int i = 0; i < 100000; ++i)
            c[i] = a[i] + b[i];

   end   = ippGetCpuClocks();
   printf ("Multiple parallel constructs take %u cycles\n", (Ipp32s) (end - start));

   start = ippGetCpuClocks();
   #pragma omp parallel num_threads(4) shared(a, b, c)
   {
      #pragma omp for 
         for (int i = 0; i < 100000; ++i)
            a[i] = i;
      #pragma omp for 
         for (int i = 0; i < 100000; ++i)
            b[i] = i;
      #pragma omp for 
         for (int i = 0; i < 100000; ++i)
            c[i] = a[i] + b[i];
   }
   
   end   = ippGetCpuClocks();
   printf ("Multiple work sharing constructs take %u cycles\n", (Ipp32s) (end - start));

   return 0;
}