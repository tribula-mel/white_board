#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

/* 10 millisec timeout */
#define TIMEOUT_MSEC 10000000

static void yield_cpu(unsigned long timeout)
{
   int rc = -1;

   struct timespec ts_req = {
      .tv_sec = 0,
      .tv_nsec = timeout
   };
   struct timespec ts_rem;

   while (1) {
      rc = nanosleep(&ts_req, &ts_rem);
      if (rc == -1) {
         if (errno == EINTR) {
            ts_req.tv_nsec = ts_rem.tv_nsec;
            continue;
         } else {
            fprintf(stderr, "error during nanosleep(): %s\n", strerror(errno));
	    break;
         }
      }
      break;
   }
}

int main(void)
{
   while (1) {
      yield_cpu(TIMEOUT_MSEC);
   }

   return 0;
}
