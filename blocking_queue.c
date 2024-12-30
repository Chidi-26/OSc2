// Student : Chidi Ikwunze ID : 20473929
#include "blocking_queue.h"
#include "utilities.h"

#include <assert.h>
#include <stdio.h>

void blocking_queue_terminate(BlockingQueueT* queue) {
  assert(queue); //ensures que exsists

  pthread_mutex_lock(&queue->queueLock);
  queue->terminate = 1; // setting termination to 1 - means termination
  for(int i = 0; i < list_length(queue->list) + 1; i++){
    sem_post(&queue->semQue);
  }

  pthread_mutex_unlock(&queue->queueLock);
}

void blocking_queue_create(BlockingQueueT* queue) {
 
  queue->list = list_create();
  assert(queue->list); //ensures list is created
  pthread_mutex_init(&queue->queueLock, NULL); //initialising mutex
  queue->terminate = 0; //Setting flag to 0, showing that queue hasnt been terminated
}

void blocking_queue_destroy(BlockingQueueT* queue) {
  assert(queue); //ensures que exsists


  list_destroy(queue->list); //destroys list

  pthread_mutex_destroy(&queue->queueLock); //destroying mutex
}

void blocking_queue_push(BlockingQueueT* queue, unsigned int value) {
  assert(queue);//ensures que exsists
  

  pthread_mutex_lock(&queue->queueLock);
  //If termination is activated dont allow value to be added
  if(queue->terminate == 1){
    pthread_mutex_unlock(&queue->queueLock); //unlock if termination = 1
    return; // returns false
  }
  list_append(queue->list, value); //adds to list
  sem_post(&queue->semQue); //wakes up and unblocks one pop
  pthread_mutex_unlock(&queue->queueLock); 
}

int blocking_queue_pop(BlockingQueueT* queue, unsigned int* value) {
  assert(queue); //ensures que exsists
  assert(value); //ensures value exists


  while (1){
    sem_wait(&queue->semQue); //waits for push to wake up semaphore
    pthread_mutex_lock(&queue->queueLock); //locks mutex

    if(queue->terminate == 1){
    pthread_mutex_unlock(&queue->queueLock);
    return 1;
    }
  if(!list_empty(queue->list) && queue->terminate == 0){
    *value = list_pop_front(queue->list);
    pthread_mutex_unlock(&queue->queueLock);
    return 0;
    
    }
  }
  pthread_mutex_unlock(&queue->queueLock);
  return 1;

}

int blocking_queue_empty(BlockingQueueT* queue) {
  assert(queue); //Ensuring queue exsists
  pthread_mutex_lock(&queue->queueLock); //Locking queue mutex

  if(list_empty(queue->list)){
    pthread_mutex_unlock(&queue->queueLock); //Unlocking queue mutex
    return 1;
  }
  pthread_mutex_unlock(&queue->queueLock); //Unlocking queue mutex
  return 0;
}

int blocking_queue_length(BlockingQueueT* queue) {
  assert(queue); //ensures que exsists
  int lengthList = 0;

  pthread_mutex_lock(&queue->queueLock);
  lengthList = list_length(queue->list);
  pthread_mutex_unlock(&queue->queueLock);

  return lengthList;
}
