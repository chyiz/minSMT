#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include "tern/user.h"
#include "busywait.h"


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
#define NUM_TASKS 2
const int nt = 2;
unsigned int task_len = NUM_TASKS;

volatile long sum = 0;
int taskIdx = 0;
struct timeval prog_begin;

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
  // pthread_barrier_wait(&bar);
  int id;
  long local_sum = 0;
  struct timeval end;
  while (id < task_len){
    pthread_mutex_lock(&idx_mutex);
    id++;
    // busy_wait(1);
    
    pthread_mutex_unlock(&idx_mutex);

    busy_wait(4000000);

    
    // gettimeofday(&end, NULL);
    // printf("%ld, %d, %d\n", pthread_self(), id, time_diff(&end, &prog_begin));
    pthread_mutex_lock(&idx_mutex);
    id++;
    // busy_wait(1);
    
    pthread_mutex_unlock(&idx_mutex);

    busy_wait(2000000);

  }

  // pthread_mutex_lock(&sum_mutex);
  sum += local_sum;
  // pthread_mutex_unlock(&sum_mutex);
  return NULL;
}

void *
thread2(void *args)
{
  // pthread_barrier_wait(&bar);
  int id;
  long local_sum = 0;
  struct timeval end;
  while (id < task_len){
    pthread_mutex_lock(&sum_mutex);
    id++;
    // busy_wait(1);
    
    pthread_mutex_unlock(&sum_mutex);

    busy_wait(3000000);

    
    // gettimeofday(&end, NULL);
    // printf("%ld, %d, %d\n", pthread_self(), id, time_diff(&end, &prog_begin));
    pthread_mutex_lock(&sum_mutex);
    id++;
    // busy_wait(1);
    
    pthread_mutex_unlock(&sum_mutex);

    busy_wait(4000000);

  }

  // pthread_mutex_lock(&sum_mutex);
  sum += local_sum;
  // pthread_mutex_unlock(&sum_mutex);
  return NULL;
}

int 
main(int argc, char *argv[])
{
  gettimeofday(&prog_begin, NULL);
  //return 0;
  int i;
  pthread_t tid1[nt], tid2[nt];
  assert(pthread_mutex_init(&idx_mutex,NULL) == 0);
  assert(pthread_mutex_init(&sum_mutex,NULL) == 0);
  // pthread_barrier_init(&bar, NULL, nt+1);

  slock_next_n(nt*2);
  for (i = 0; i < nt; i++){
    assert(pthread_create(&tid1[i],NULL,thread1,NULL) == 0);
    assert(pthread_create(&tid2[i],NULL,thread2,NULL) == 0);
  }

  // pthread_barrier_wait(&bar);


  for (i = 0; i < nt; i++){
    assert(pthread_join(tid1[i], NULL) == 0);
    assert(pthread_join(tid2[i], NULL) == 0);
  }

  printf("Done. Total sum is %ld.\n", sum);

  assert(pthread_mutex_destroy(&idx_mutex) == 0);
  assert(pthread_mutex_destroy(&sum_mutex) == 0);

  return 0;
}

