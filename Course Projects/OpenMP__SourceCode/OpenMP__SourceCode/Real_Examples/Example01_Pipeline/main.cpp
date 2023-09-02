#include <stdio.h>  
#include <unistd.h>
#include <ipp.h>
#include <omp.h> 
#include <time.h>

char ColorCodes[5][250] = {
   "\033[1m\033[30m",      // Bold BLACK
   "\033[1m\033[31m",      // Bold RED
   "\033[1m\033[32m",      // Bold GREEN
   "\033[1m\033[34m",      // Bold BLUE
   "\x1B[0m"               // Reset
};
  
void ReadFromFile (int data_pack, int tid)
{
   char str [256];
   time_t t;
   struct tm tm;

   t = time(NULL);
   tm = *localtime(&t);
  
   sprintf (str, "%s @(%d:%d:%d)-> Reading data[%d] by T%d ...\n", 
            ColorCodes[tid], tm.tm_hour, tm.tm_min, tm.tm_sec, data_pack, tid);
   printf (str);
   printf (ColorCodes[4]);
   sleep (1.1);
}

void WriteIntoFile (int data_pack, int tid)
{
   char str [255];
   time_t t;
   struct tm tm;

   t = time(NULL);
   tm = *localtime(&t);

   sprintf (str, "%s @(%d:%d:%d)-> Writing data[%d] by T%d ...\n", 
            ColorCodes[tid], tm.tm_hour, tm.tm_min, tm.tm_sec, data_pack, tid);
   printf (str);
   printf (ColorCodes[4]);
   sleep (1.2);
}

void ProcessData (int data_pack, int data_item, int tid)
{
   char str [255];
   time_t t;
   struct tm tm;

   t = time(NULL);
   tm = *localtime(&t);
  
   sprintf (str, "%s @(%d:%d:%d)-> Processing data[%d][%d] by T%d ...\n", 
            ColorCodes[tid], tm.tm_hour, tm.tm_min, tm.tm_sec, data_pack, data_item, tid);
   printf (str);
   printf (ColorCodes[4]);
   sleep (0.2);
}

int main (void)
{
   #pragma omp parallel num_threads(4)
   {
      // preload data to be used in first iteration of the i-loop 
      #pragma omp single
      {
         ReadFromFile(0, omp_get_thread_num());
      }
      
      for (int i = 0; i < 20; i++) {
         // preload data for next iteration of the i-loop 
         #pragma omp single nowait
         {
            ReadFromFile(i + 1, omp_get_thread_num());
         }
         #pragma omp for schedule(dynamic)
            for (int j = 0; j < 5; j++)
               ProcessData(i, j, omp_get_thread_num()); // here is the work 
         // there is a barrier at the end of this loop 
         #pragma omp single nowait
         {
            WriteIntoFile(i, omp_get_thread_num());
         }
      } // threads immediately move on to next iteration of i-loop 
   } // one parallel region encloses all the work

   return 0;
}