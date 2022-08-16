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
#define NUM_TASKS 161
#define TASK_SIZE  100L
long sim_taskQ[NUM_TASKS];
int task_len = (int) (sizeof(sim_taskQ)/sizeof(sim_taskQ[0]));

volatile long sum = 0;
int taskIdx = 0;


void *
thread(void *args)
{
  // pthread_barrier_wait(&bar);
  int id;
  long local_sum = 0;
  while (1){
    pthread_mutex_lock(&idx_mutex);
    if (taskIdx < task_len){
      id = taskIdx++;
    } else {
      pthread_mutex_unlock(&idx_mutex);
      break;
    }
    
    pthread_mutex_unlock(&idx_mutex);

    long sim_task_len = sim_taskQ[id];

    //printf("Working on task %d.\n", id);

    for (volatile int i=0; i<sim_task_len; i++){
      local_sum += i*i*i;
    }

  }

  // pthread_mutex_lock(&sum_mutex);
  sum += local_sum;
  // pthread_mutex_unlock(&sum_mutex);
  return NULL;
}

void init_tasks(){
  for (int i=0; i< NUM_TASKS-1; i++){
      sim_taskQ[i] = TASK_SIZE+(rand()*1000000L) % (TASK_SIZE<<3) ;
//      printf("%ld\n",sim_taskQ[i]);
  }
  sim_taskQ[NUM_TASKS-1] = TASK_SIZE/10;

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
  int nt = 4;
  //return 0;
  int i;
  struct timeval begin, end;
  pthread_t tid[32];
  assert(pthread_mutex_init(&idx_mutex,NULL) == 0);
  assert(pthread_mutex_init(&sum_mutex,NULL) == 0);
  // pthread_barrier_init(&bar, NULL, nt+1);

  init_tasks();


  slock_next_n(nt);
  
  gettimeofday(&begin, NULL);
  for (i = 0; i < nt; i++)
    assert(pthread_create(&tid[i],NULL,thread,NULL) == 0);
  gettimeofday(&end, NULL);
  printf("Pthread create loop time, %d, %d\n", nt, time_diff1(&end, &begin));

  // pthread_barrier_wait(&bar);


  for (i = 0; i < nt; i++)
    assert(pthread_join(tid[i], NULL) == 0);

  nt = 16;
  slock_next_n(nt);
  gettimeofday(&begin, NULL);
  for (i = 0; i < nt; i++)
     assert(pthread_create(&tid[i],NULL,thread,NULL) == 0);
  gettimeofday(&end, NULL);
  printf("Pthread create loop time, %d, %d\n", nt, time_diff1(&end, &begin));

  for (i=0; i<nt; i++)
     pthread_join(tid[i], NULL);

  nt = 12;
  slock_next_n(nt);
  gettimeofday(&begin, NULL);
  for (i=0; i<nt; i++)
     pthread_create(&tid[i], NULL, thread, NULL);
  gettimeofday(&end, NULL);
  printf("Pthread create loop time, %d, %d\n", nt, time_diff1(&end, &begin));

  for (i=0; i<nt; i++)
     pthread_join(tid[i], NULL);
  
 
   nt = 32;
   slock_next_n(nt);
   gettimeofday(&begin, NULL);
   for (i=0; i<nt; i++)
      pthread_create(&tid[i], NULL, thread, NULL);
   gettimeofday(&end, NULL);
   printf("Pthread create loop time, %d, %d\n", nt, time_diff1(&end, &begin));

   for (i=0; i<nt; i++)
      pthread_join(tid[i], NULL);
  printf("Done. Total sum is %ld.\n", sum);

  assert(pthread_mutex_destroy(&idx_mutex) == 0);
  assert(pthread_mutex_destroy(&sum_mutex) == 0);

  return 0;
}

