#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tern/user.h"

pthread_mutex_t idx_mutex, sum_mutex;

#define NUM_TASKS 161
#define TASK_SIZE  600000000L
#define TASK_RANGE_SIZE 16
long sim_taskQ[NUM_TASKS];
int task_len = (int) (sizeof(sim_taskQ)/sizeof(sim_taskQ[0]));
int task_multiply_range[TASK_RANGE_SIZE] = {1, 1, 1, 1, 1, 
                                            1, 1, 1, 1, 1, 
                                            1, 1, 1, 1, 1, 
                                            1};

volatile long sum = 0;
int taskIdx = 0;


void *
thread(void *args)
{
  // pthread_barrier_wait(&bar);
  int id;
  long local_sum = 0;
  while (1){

    pcs_enter();
    pthread_mutex_lock(&idx_mutex);
    if (taskIdx < task_len){
      id = taskIdx++;
    } else {
      pthread_mutex_unlock(&idx_mutex);
      break;
    }
    // no_wait_pcs_enter();
    // no_wait_pcs_exit();
    pthread_mutex_unlock(&idx_mutex);
    pcs_exit();
    tern_dummy_sync();
    long sim_task_len = sim_taskQ[id];

    //printf("Working on task %d.\n", id);

    mark_task_start(1);
    for (volatile int i=0; i<sim_task_len; i++){
      local_sum += i*i*i;
    }
    mark_task_end(1);

  }

  sum += local_sum;
  return NULL;
}

void init_tasks(){
  for (int i=0; i< NUM_TASKS-1; i++){
      // sim_taskQ[i] = TASK_SIZE/2+(rand()%(TASK_SIZE));
      // sim_taskQ[i] = TASK_SIZE;
      sim_taskQ[i] = task_multiply_range[rand()%TASK_RANGE_SIZE] * TASK_SIZE;
//      printf("%ld\n",sim_taskQ[i]);
  }
  sim_taskQ[NUM_TASKS-1] = TASK_SIZE/10;

}

int 
main(int argc, char *argv[])
{
  const int nt = 16;
  //return 0;
  int i;
  pthread_t tid[nt];
  assert(pthread_mutex_init(&idx_mutex,NULL) == 0);
  assert(pthread_mutex_init(&sum_mutex,NULL) == 0);
  // pthread_barrier_init(&bar, NULL, nt+1);

  init_tasks();

  slock_next_n(nt);
  for (i = 0; i < nt; i++)
    assert(pthread_create(&tid[i],NULL,thread,NULL) == 0);

  // pthread_barrier_wait(&bar);


  for (i = 0; i < nt; i++)
    assert(pthread_join(tid[i], NULL) == 0);

  printf("Done. Total sum is %ld.\n", sum);

  assert(pthread_mutex_destroy(&idx_mutex) == 0);
  assert(pthread_mutex_destroy(&sum_mutex) == 0);

  return 0;
}

