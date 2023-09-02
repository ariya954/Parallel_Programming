#include "stdio.h"
#include "unistd.h"
#include "time.h"
#include "omp.h"

int main(int argc, char * argv[])
{

	time_t t;
	struct tm tm;

	int TID;
	#pragma omp parallel private(TID, t, tm)
	{
		TID = omp_get_thread_num();
		if (TID < omp_get_num_threads()/2 )
			sleep (3);
		t = time(NULL);
		tm = *localtime(&t);
		printf("Thread %d befor barrier at %d:%d:%d\n", TID, tm.tm_hour, tm.tm_min, tm.tm_sec);
		#pragma omp barrier
		t = time(NULL);
		tm = *localtime(&t);
		printf("Thread %d after barrier at %d:%d:%d\n", TID, tm.tm_hour, tm.tm_min, tm.tm_sec);
	} /*-- End of parallel region --*/	
	return 0;
}
