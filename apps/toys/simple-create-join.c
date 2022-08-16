#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
#define NUM_TASKS 24
#define TASK_SIZE  100000000L
long sim_taskQ[NUM_TASKS];
int task_len = (int) (sizeof(sim_taskQ)/sizeof(sim_taskQ[0]));

volatile long sum = 0;
int taskIdx = 0;


void *
thread(int arg)
{
  // pthread_barrier_wait(&bar);
  long local_sum = 0;
  long sim_task_len = sim_taskQ[arg];

    //printf("Working on task %d.\n", id);

    for (volatile int i=0; i<sim_task_len; i++){
      local_sum += i*i*i;
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

int 
main(int argc, char *argv[])
{
  const int nt = 24;
  //return 0;
  int i;
  pthread_t tid[nt];

  init_tasks();

  slock_next_n(nt);
  for (i = 0; i < nt; i++)
    assert(pthread_create(&tid[i],NULL,thread,i) == 0);

  // pthread_barrier_wait(&bar);


  for (i = 0; i < nt; i++)
    assert(pthread_join(tid[i], NULL) == 0);

  printf("Done. Total sum is %ld.\n", sum);


  return 0;
}

