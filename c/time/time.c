#include <time.h>
#include <stdio.h>
#include <sys/time.h>

int main(void)
{
   struct timespec ts;
   struct timeval tv;
   time_t sec;

   clock_gettime(CLOCK_REALTIME, &ts);
   sec = time(NULL);
   gettimeofday(&tv, NULL);

   printf("clock_gettime[%ld]\n", ts.tv_sec);
   printf("         time[%ld]\n", sec);
   printf(" gettimeofday[%ld]\n", tv.tv_sec);

   return 0;
}
