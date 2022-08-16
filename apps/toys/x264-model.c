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
#define NUM_ENCODE_TASKS 161
#define NUM_FRAMES 512
#define ENCODE_TASK_SIZE  50000L
#define READER_TASK_SIZE  1000L
#define ENCODE_TASK_RANGE 1100000L
long sim_taskQ[NUM_ENCODE_TASKS*NUM_FRAMES];
int task_len = (int) (sizeof(sim_taskQ)/sizeof(sim_taskQ[0]));

volatile long sum = 0;
int taskIdx = 0;

long *reader_thread(void *args){
  long local_sum = 0;
  for (volatile int i=0; i<READER_TASK_SIZE; i++){
    local_sum += i*i*i;
  }

}


void *
writer_thread(void *args){
  // pthread_barrier_wait(&bar);
  int id;
  long local_sum = 0;
  for (int i=0; i<NUM_ENCODE_TASKS; i++){
    pthread_mutex_lock(&idx_mutex);
      id = taskIdx++;
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
  srand(76543);
  for (int i=0; i< NUM_ENCODE_TASKS*NUM_FRAMES-1; i++){
      sim_taskQ[i] = ENCODE_TASK_SIZE+(rand()*1000000L) % (ENCODE_TASK_RANGE) ;
//      printf("%ld\n",sim_taskQ[i]);
  }
  sim_taskQ[NUM_ENCODE_TASKS*NUM_FRAMES-1] = ENCODE_TASK_SIZE/10;

}

int 
main(int argc, char *argv[])
{
  const int nt = 16;
  //return 0;
  int i;
  pthread_t thread[nt], reader_handle;
  int have_read = 0;
  // assert(pthread_mutex_init(&idx_mutex,NULL) == 0);
  // assert(pthread_mutex_init(&sum_mutex,NULL) == 0);
  // pthread_barrier_init(&bar, NULL, nt+1);

  init_tasks();

  for (i = 0; i < nt; i++)
    

  // pthread_barrier_wait(&bar);

  for (i = 0; i < NUM_FRAMES; i ++){
    if (have_read){
      assert(pthread_join(reader_handle, NULL) == 0);
    }
    assert(pthread_create(&reader_handle, NULL, reader_thread, NULL) == 0);
    have_read = 1;
    int tid = i % nt;
    if (i >= nt){
      int oldtid = (i-nt) % nt;
      assert(pthread_join(thread[tid], NULL) == 0);
    }
    assert(pthread_create(&thread[tid],NULL,writer_thread,NULL) == 0);
    
  }
  for (; i < NUM_FRAMES+nt; i++){
    int tid = i % nt;
    assert(pthread_join(thread[tid], NULL) == 0);
  }

  printf("Done. Total sum is %ld.\n", sum);

  // assert(pthread_mutex_destroy(&idx_mutex) == 0);
  // assert(pthread_mutex_destroy(&sum_mutex) == 0);

  return 0;
}

