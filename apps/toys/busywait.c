#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>

#define USEC_PER_SEC 1000000

bool timeval_lt(struct timeval tv1, struct timeval tv2){
  return (tv1.tv_sec < tv2.tv_sec || (tv1.tv_sec == tv2.tv_sec && tv1.tv_usec < tv2.tv_usec));
}

struct timeval timeval_normalize(struct timeval tv){
	while(tv.tv_usec >= USEC_PER_SEC)
	{
		++(tv.tv_sec);
		tv.tv_usec -= USEC_PER_SEC;
	}
	
	while(tv.tv_usec <= -USEC_PER_SEC)
	{
		--(tv.tv_sec);
		tv.tv_usec += USEC_PER_SEC;
	}
	
	if(tv.tv_usec < 0 && tv.tv_sec > 0)
	{
		/* Negative nanoseconds while seconds is positive.
		 * Decrement tv_sec and roll tv_usec over.
		*/
		
		--(tv.tv_sec);
		tv.tv_usec = USEC_PER_SEC - (-1 * tv.tv_usec);
	}
	else if(tv.tv_usec > 0 && tv.tv_sec < 0)
	{
		/* Positive nanoseconds while seconds is negative.
		 * Increment tv_sec and roll tv_usec over.
		*/
		
		++(tv.tv_sec);
		tv.tv_usec = -USEC_PER_SEC - (-1 * tv.tv_usec);
	}
	
	return tv;
}

void busy_wait (unsigned int wait_usec){
  struct timeval stop_time;
  gettimeofday(&stop_time, NULL);
  stop_time.tv_usec += wait_usec;
  stop_time = timeval_normalize(stop_time);
  struct timeval now;
  do {
    gettimeofday(&now, NULL);
  } while(timeval_lt(now, stop_time));

}

