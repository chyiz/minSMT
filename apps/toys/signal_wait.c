#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include "tern/user.h"
#include "busywait.h"

#define NUM_THREADS 2

volatile long sum = 0;
int pool1 = 0;
int pool2 = 0;
struct timeval prog_begin;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

static inline unsigned int time_diff (struct timeval *end, struct timeval *begin)
{
    unsigned int result;

    result = end->tv_sec - begin->tv_sec;
    result *= 1000000;     /* usec */
    result += end->tv_usec - begin->tv_usec;

    return result;
}

void *
thread1(void *args)
{
  struct timeval end;

  pthread_mutex_lock(&glb_mutex);
  // busy_wait(1);
  while (pool == 0)
    pthread_cond_wait(&cond, &glb_mutex);

  pool--;
  
  pthread_mutex_unlock(&glb_mutex);

  busy_wait(5000000);

  return NULL;
}

void *
thread2(void *args)
{
  struct timeval end;

  pthread_mutex_lock(&glb_mutex);
  // busy_wait(1);
  while (pool == 0)
    pthread_cond_wait(&cond, &glb_mutex);

  pool--;
  
  pthread_mutex_unlock(&glb_mutex);

  busy_wait(5000000);

  return NULL;
}

int 
main(int argc, char *argv[])
{
  gettimeofday(&prog_begin, NULL);

  if (argc < 2){
    printf("Usage: %s <num of threads>\n", argv[0]);
    return -1;
  }

  int num_threads = atoi(argv[1]);
  //return 0;
  int i;
  pthread_t tid[num_threads];

  slock_next_n(num_threads);
  for (i = 0; i < num_threads; i++){
    printf("Creating thread %d\n", i);
    assert(pthread_create(&tid[i],NULL,thread,NULL) == 0);
  }

  for (i = 0; i < num_threads; i++){
    pthread_mutex_lock(&glb_mutex);
    pool++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&glb_mutex);
  }
    


  for (i = 0; i < num_threads; i++)
    assert(pthread_join(tid[i], NULL) == 0);

  printf("Done.\n");

  assert(pthread_mutex_destroy(&glb_mutex) == 0);

  return 0;
}