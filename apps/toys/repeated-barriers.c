#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "tern/user.h"

pthread_mutex_t idx_mutex, sum_mutex;
// pthread_barrier_t bar;

//long sim_taskQ[] = { 2e8, 2e8, 3e8, 2e8, 3e8,2e8, 1e8, 3e8, 
//                    2e8, 3e8, 2e8, 2e8, 3e8,2e8, 1e8, 3e8,
//                    3e8, 2e8, 1e8, 2e8, 2e8,2e8, 2e8, 3e8,
//                    2e8, 2e8, 2e8, 2e8, 3e8,3e8, 2e8, 3e8,
//                    3e8, 3e8, 1e8, 3e8, 2e8,2e8, 3e8, 1e8};

// long sim_taskQ[] = { 3e8, 3e8, 3e8, 3e8, 3e8,3e8, 3e8, 3e8, 
//                     3e8, 3e8, 3e8, 3e8, 3e8,3e8, 3e8, 3e8,
//                     3e8, 3e8, 3e8, 3e8, 3e8,3e8, 3e8, 3e8,
//                     3e8, 3e8, 3e8, 3e8, 3e8,3e8, 3e8, 3e8,
//                     3e8, 3e8, 3e8, 3e8, 3e8,3e8, 3e8, 3e8,};
#define NUM_THREADS 16
#define TASK_SIZE  35e4L
#define REPEATS 4000

long sim_taskQ[NUM_THREADS];
pthread_mutex_t fg_locks[NUM_THREADS];
#define NUM_LOCKS 59323491
long lock_mod = TASK_SIZE / NUM_LOCKS * REPEATS;

volatile long sum = 0;
int taskIdx = 0;

pthread_barrier_t bar;
// long lock_count[NUM_THREADS];

void *
thread(void *args)
{
  // pthread_barrier_wait(&bar);
  int id = (int) args;
  long local_sum = 0;
  volatile long i, j;
  for (int i=0; i<REPEATS; i++){

    long sim_task_len = sim_taskQ[id];

    //printf("Working on task %d.\n", id);

    for (j=0; j<TASK_SIZE; j++){
      local_sum += j*j*j;
      if (j % lock_mod == 0 ){
        no_wait_pcs_enter();
        pthread_mutex_lock(&fg_locks[id]);
        pthread_mutex_unlock(&fg_locks[id]);
        no_wait_pcs_exit();
      }
    }

    pthread_barrier_wait(&bar);
  }

  // pthread_mutex_lock(&sum_mutex);
  sum += local_sum;
  // pthread_mutex_unlock(&sum_mutex);
  return NULL;
}

static inline unsigned int time_diff1 (
    struct timeval *end, struct timeval *begin)
{
    unsigned long result;

    result = end->tv_sec - begin->tv_sec;
    result *= 1000000;     /* usec */
    result += end->tv_usec - begin->tv_usec;

    return result;
}

int 
main(int argc, char *argv[])
{
  //return 0;
  int i;
  struct timeval begin, end;
  pthread_t tid[32];
  
  pthread_barrier_init(&bar, NULL, NUM_THREADS);
  for (i = 0; i < NUM_THREADS; i++){
    pthread_mutex_init(&fg_locks[i], NULL);
  }

  printf("lock mod: %ld\n", lock_mod);
  slock_next_n(NUM_THREADS);
  
  // gettimeofday(&begin, NULL);
  for (i = 0; i < NUM_THREADS; i++)
    assert(pthread_create(&tid[i],NULL,thread,(void *)i) == 0);
  // gettimeofday(&end, NULL);
  // printf("Pthread create loop time, %d, %d\n", nt, time_diff1(&end, &begin));

   for (i=0; i<NUM_THREADS; i++)
      pthread_join(tid[i], NULL);
  printf("Done. Total sum is %ld.\n", sum);

  pthread_barrier_destroy(&bar);

  return 0;
}

